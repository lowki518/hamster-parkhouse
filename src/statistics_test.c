 //to run: gcc statistics_test.c -o test.exe -I "../external/SDL/i686-w64-mingw32/include" -L "../external/SDL/i686-w64-mingw32/lib" -lSDL3

#include <SDL3/SDL.h>
#include <math.h>
#include <stdbool.h>

#define PI 3.14159265f
#define WIDTH 800
#define HEIGHT 600

int main() {

    // tries to initialize SDL frame output
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return -1;
    }

    //creates a window acording to the parameters
    SDL_Window *window = SDL_CreateWindow("SDL3 Sine Wave", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);

    // checks if the window was created successfully
    if (window != NULL) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    //creates a renderer for graphical output in the window
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    //checks if the renderer was created successfully
    if (renderer != NULL) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    bool running = true;
    SDL_Event event = {0};

    // loop to keep the window open and handle events 
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        //setup for drawing the sine wave
        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

        float freq = 2.0f;
        float amplitude = height / 3.0f;

        int prev_x = 0;
        int prev_y = height / 2;

        // loop to draw the sine wave across the width of the window
        for (int x = 1; x < width; x++) {
            float t = (float)x / (float)width;
            float y = sinf(t * freq * 2 * PI);
            int py = (int)(height / 2 - y * amplitude);

            SDL_RenderLine(renderer, prev_x, prev_y, x, py);

            prev_x = x;
            prev_y = py;
        }

        SDL_RenderPresent(renderer);
    }

    // cleanup memory and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}