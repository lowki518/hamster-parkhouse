/* 
Messed around with SDL, to see if its usable for the project.
The Glow Effect and hovering was done with the help of copilot, since math is hard and I'm
not an expert with SDL.
Code based of Tutorials from Programming Rainbow and Christopher Medina.

To run this version of gcc is needed: i686-w64-mingw32 and this command
gcc statistics_test.c -o test.exe -I "../external/SDL/i686-w64-mingw32/include" -I "../external/SDL3_ttf/i686-w64-mingw32/include" -L "../external/SDL/i686-w64-mingw32/lib"  -L "../external/SDL3_ttf/i686-w64-mingw32/lib"  -lSDL3 -lSDL3_ttf

*/
// Build with SDL3 + SDL3_ttf
// Example (clang): clang main.c -o app `sdl3-config --cflags --libs` -lSDL3_ttf
// Make sure SDL3_ttf is available and your font path is correct.

// Build with SDL3 + SDL3_ttf
// Example (clang): clang main.c -o app `sdl3-config --cflags --libs` -lSDL3_ttf
// Make sure SDL3_ttf is available and your FONT_PATH is correct.

#include "statistics_output_lib.h"

// ================== Layout ==================
#define WIDTH             1500
#define HEIGHT            1000
#define FONT_PATH         "../external/Font/ARIAL.TTF"
#define FONT_SIZE         12

// Layout constants
#define TOP_BAR_H         44.0f   // reserved space for close button
#define SIDE_BTN_W        50.0f
#define SIDE_BTN_H        50.0f
#define SIDE_PAD          10.0f   // padding between plot and side buttons
#define EDGE_MARGIN       10.0f   // window margin to place side buttons
#define CLOSE_SIZE        28.0f
#define CLOSE_MARGIN       8.0f

// ================== Booleans ==================
#define TRUE  1
#define FALSE 0

// ================== Data ==================

#define DATASET_COUNT 5
static Series datasets[DATASET_COUNT];
static int currentDataset = 0;
char dataset_name[DATASET_COUNT][32] = {"Data1", "Data2", "Data3", "Data4", "Data5"};

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL init failed: %s", SDL_GetError());
        return -1;
    }

    // Seed RNG
    srand((unsigned)time(NULL));

    // Prepare datasets
    for (int i = 0; i < DATASET_COUNT; ++i) {
        createRandomDataset(&datasets[i],
                            /*nMin=*/120, /*nMax=*/600,  // 2 to 10 hours of minutes
                            /*vMin=*/0.0f, /*vMax=*/100.0f);
    }

    // Borderless window
    SDL_Window *window = SDL_CreateWindow("Plotter", WIDTH, HEIGHT, SDL_WINDOW_BORDERLESS);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    // Initial plot rect (will be recalculated every frame)
    PlotArea plot = createPlotArea(EDGE_MARGIN + SIDE_BTN_W + SIDE_PAD, TOP_BAR_H + EDGE_MARGIN,
                                   WIDTH - (EDGE_MARGIN + SIDE_BTN_W + SIDE_PAD)*2,  // overwritten by computeLayout
                                   HEIGHT - TOP_BAR_H - 2.0f*EDGE_MARGIN,            // overwritten by computeLayout
                                   renderer);

    TTF_Font * font = openFont(FONT_PATH, FONT_SIZE);
    if (!font) {
        for (int i = 0; i < DATASET_COUNT; ++i) freeDataset(&datasets[i]);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Buttons
    Button leftBtn     = {{0,0,0,0}, FALSE, FALSE, 0.0f, 0.0f};
    Button rightBtn    = {{0,0,0,0}, FALSE, FALSE, 0.0f, 0.0f};
    Button closeBtn    = {{0,0,0,0}, FALSE, FALSE, 0.0f, 0.0f};

    // Resampled buffer (recomputed when inner width or dataset changes)
    float *resampled = NULL;
    int resampledCount = 0;
    int lastInnerPixelWidth = -1;
    int lastDatasetIndex = -1;

    // Current Y range
    float yMin = 0.0f, yMax = 1.0f;

    int running = TRUE;
    SDL_Event event;
    Uint64 last = 0;

    while (running) {
        Uint64 now = SDL_GetTicks();
        float dt = (last == 0) ? 0.016f : (float)(now - last) / 1000.0f;
        last = now;

        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        // Layout
        LayoutMetrics L;
        computeLayout(width, height, &L, &plot, &leftBtn, &rightBtn, &closeBtn);

        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        updateButtonHover(&leftBtn, mouseX, mouseY);
        updateButtonHover(&rightBtn, mouseX, mouseY);
        updateButtonHover(&closeBtn, mouseX, mouseY);

        int leftClicked = 0, rightClicked = 0;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = FALSE;

            leftClicked  |= handleNavButtonEvent(&leftBtn,  &event);
            rightClicked |= handleNavButtonEvent(&rightBtn, &event);
            handleCloseButtonEvent(&closeBtn, &event, &running);

            // ESC to quit
            if (event.type == SDL_EVENT_KEY_UP && event.key.key == SDLK_ESCAPE) {
                running = FALSE;
            }
        }

        // Change dataset on clicks (wrap 0..4)
        if (leftClicked) {
            currentDataset = (currentDataset + DATASET_COUNT - 1) % DATASET_COUNT;
        }
        if (rightClicked) {
            currentDataset = (currentDataset + 1) % DATASET_COUNT;
        }

        updateButtonAnimation(&leftBtn, dt);
        updateButtonAnimation(&rightBtn, dt);
        updateButtonAnimation(&closeBtn, dt);

        // Resample if needed (inner width or dataset changed)
        float left, right, top, bottom;
        innerBounds(&plot, &left, &right, &top, &bottom);
        int innerPixelWidth = (int)fmaxf(2.0f, floorf(right - left));

        if (innerPixelWidth != lastInnerPixelWidth || currentDataset != lastDatasetIndex) {
            if (resampled) { free(resampled); resampled = NULL; }
            Series *S = &datasets[currentDataset];
            resampled = resampleUniform(S->v, S->n, innerPixelWidth);
            resampledCount = innerPixelWidth;
            lastInnerPixelWidth = innerPixelWidth;
            lastDatasetIndex = currentDataset;

            // Recompute Y range for the *original* dataset (smoother labels)
            computeYRange(S->v, S->n, &yMin, &yMax);
        }

        // Draw frame
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        // Plot: background -> grid & ticks -> axes -> labels -> graph -> UI
        drawPlotBackground(renderer, &plot);

        // Grid & ticks (with the requested rules)
        drawYTicksAndGrid(renderer, font, &plot, yMin, yMax);
        drawXTicksAndGridMinutes(renderer, font, &plot, datasets[currentDataset].n);

        // Axes and labels
        drawAxesWithArrows(renderer, &plot);
        drawAxisLabels(renderer, font, &plot, "Minutes", dataset_name[currentDataset]);

        // Graph (resampled for smoothness)
        drawGraph(renderer, &plot,
                  resampled ? resampled : datasets[currentDataset].v,
                  resampled ? resampledCount : datasets[currentDataset].n,
                  yMin, yMax);

        // UI: nav buttons
        drawButton(renderer, &leftBtn);
        drawButton(renderer, &rightBtn);
        drawArrow(renderer, &leftBtn.rect,  -1, leftBtn.hoverAnim);
        drawArrow(renderer, &rightBtn.rect,  1, rightBtn.hoverAnim);

        // Close button
        drawButton(renderer, &closeBtn);
        drawXIcon(renderer, &closeBtn.rect, closeBtn.hoverAnim);

        SDL_RenderPresent(renderer);
    }

    if (resampled) free(resampled);
    for (int i = 0; i < DATASET_COUNT; ++i) freeDataset(&datasets[i]);

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}