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

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ================== Configuration ==================
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

// ================== Types ==================
typedef struct {
    SDL_FRect rect;
    int hovered;
    int pressed;
    float hoverAnim;
    float pressAnim;
} Button;

typedef struct {
    SDL_FRect rect;
    float padding; // inner padding for axes/labels
} PlotArea;

typedef struct {
    float topBar;
    float leftBtnPad;   // window-left margin for left button
    float rightBtnPad;  // window-right margin for right button
} LayoutMetrics;

typedef struct {
    float *v;
    int n;
} Series;

// ================== Globals: 5 datasets, current index ==================
#define DATASET_COUNT 5
static Series datasets[DATASET_COUNT];
static int currentDataset = 0;
char dataset_name[DATASET_COUNT][32] = {"Data1", "Data2", "Data3", "Data4", "Data5"};

// ================== Math / Utility ==================
static inline float lerp(float a, float b, float t) { return a + (b - a) * t; }

static inline int isPointInsideRect(float x, float y, const SDL_FRect * rect) {
    return (x >= rect->x && x <= rect->x + rect->w &&
            y >= rect->y && y <= rect->y + rect->h);
}

static inline void innerBounds(PlotArea *p, float *left, float *right, float *top, float *bottom) {
    *left   = p->rect.x + p->padding;
    *bottom = p->rect.y + p->rect.h - p->padding;
    *right  = p->rect.x + p->rect.w - 20.0f; // keep arrow space
    *top    = p->rect.y + 20.0f;             // keep arrow space
}

static inline int nearlyEqual(float a, float b, float tol) {
    return fabsf(a - b) <= tol;
}

// ================== Buttons & Animations ==================
static void updateButtonHover(Button * btn, float mouseX, float mouseY) {
    btn->hovered = isPointInsideRect(mouseX, mouseY, &btn->rect);
}

static void updateButtonAnimation(Button * btn, float dt) {
    const float speed = 10.0f;
    float targetHover = btn->hovered ? 1.0f : 0.0f;
    float targetPress = btn->pressed ? 1.0f : 0.0f;
    btn->hoverAnim += (targetHover - btn->hoverAnim) * speed * dt;
    btn->pressAnim += (targetPress - btn->pressAnim) * speed * dt;
}

static void drawGlow(SDL_Renderer * renderer, const SDL_FRect * rect, float intensity) {
    int layers = 6;
    for (int i = 0; i < layers; i++) {
        float expand = (float)i * 4.0f;
        float alpha = (1.0f - (float)i / layers) * 80.0f * intensity;
        SDL_SetRenderDrawColor(renderer, 0, 200, 255, (Uint8)alpha);
        SDL_FRect glowRect = {
            rect->x - expand,
            rect->y - expand,
            rect->w + expand * 2,
            rect->h + expand * 2
        };
        SDL_RenderFillRect(renderer, &glowRect);
    }
}

static void drawButton(SDL_Renderer * renderer, const Button * btn) {
    // Glow
    drawGlow(renderer, &btn->rect, btn->hoverAnim);

    // Fill (animated color)
    SDL_Color base  = {0, 150, 255, 255};
    SDL_Color hover = {0, 200, 255, 255};
    float t = btn->hoverAnim;
    Uint8 r = (Uint8)lerp(base.r, hover.r, t);
    Uint8 g = (Uint8)lerp(base.g, hover.g, t);
    Uint8 b = (Uint8)lerp(base.b, hover.b, t);
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    // Press shrink
    float pressScale = 1.0f - 0.1f * btn->pressAnim;
    float w = btn->rect.w * pressScale;
    float h = btn->rect.h * pressScale;
    float x = btn->rect.x + (btn->rect.w - w) / 2.0f;
    float y = btn->rect.y + (btn->rect.h - h) / 2.0f;

    SDL_FRect rct = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rct);
}

static void drawArrow(SDL_Renderer * renderer, const SDL_FRect * rect, int direction, float hoverAnim) {
    int cx = (int)(rect->x + rect->w / 2);
    int cy = (int)(rect->y + rect->h / 2);
    int size = (int)(rect->h * 0.33f + 3 * hoverAnim);

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    if (direction == -1) {
        SDL_RenderLine(renderer, cx + size, cy - size, cx - size, cy);
        SDL_RenderLine(renderer, cx - size, cy,       cx + size, cy + size);
    } else {
        SDL_RenderLine(renderer, cx - size, cy - size, cx + size, cy);
        SDL_RenderLine(renderer, cx + size, cy,       cx - size, cy + size);
    }
}

// Centered “X” icon
static void drawXIcon(SDL_Renderer *renderer, const SDL_FRect *rect, float hoverAnim) {
    float cx = rect->x + rect->w * 0.5f;
    float cy = rect->y + rect->h * 0.5f;
    float arm = fminf(rect->w, rect->h) * 0.35f - 1.5f * hoverAnim;
    float x1 = cx - arm, y1 = cy - arm;
    float x2 = cx + arm, y2 = cy + arm;

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderLine(renderer, x1, y1, x2, y2);
    SDL_RenderLine(renderer, x1, y2, x2, y1);
}

// Return 1 when a click is completed (mouse up while hovered & was pressed)
static int handleNavButtonEvent(Button * btn, const SDL_Event * event) {
    int clicked = 0;
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (btn->hovered && !btn->pressed) btn->pressed = TRUE;
    } else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        if (btn->pressed && btn->hovered) clicked = 1;
        btn->pressed = FALSE;
    }
    return clicked;
}

static void handleCloseButtonEvent(Button *btn, const SDL_Event *event, int *running) {
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (btn->hovered && !btn->pressed) btn->pressed = TRUE;
    }
    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        if (btn->pressed && btn->hovered) *running = FALSE;
        btn->pressed = FALSE;
    }
}


static TTF_Font * openFont(const char * path, int size) {
    if (TTF_Init() < 0) {
        SDL_Log("TTF init failed: %s", SDL_GetError());
        return NULL;
    }
    TTF_Font * font = TTF_OpenFont(path, size);
    if (!font) {
        SDL_Log("Failed to load font: %s", SDL_GetError());
        return NULL;
    }
    return font;
}

static void drawText(SDL_Renderer * renderer, TTF_Font * font, const char * text, float x, float y) {
    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, 0, color);
    if (!surface) return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) { SDL_DestroySurface(surface); return; }

    SDL_FRect dst = {x, y, (float)surface->w, (float)surface->h};
    SDL_RenderTexture(renderer, texture, NULL, &dst);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

// ================== Plot, Axes, Ticks ==================
static PlotArea createPlotArea(float x, float y, float w, float h, SDL_Renderer * renderer) {
    PlotArea p;
    p.rect.x = x; p.rect.y = y; p.rect.w = w; p.rect.h = h;
    p.padding = 60.0f;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &p.rect);
    return p;
}

static void drawPlotBackground(SDL_Renderer * renderer, PlotArea * p) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &p->rect);
}

static void drawAxesWithArrows(SDL_Renderer *renderer, PlotArea *p) {
    float left, right, top, bottom;
    innerBounds(p, &left, &right, &top, &bottom);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // X axis
    SDL_RenderLine(renderer, left, bottom, right, bottom);
    // Y axis
    SDL_RenderLine(renderer, left, bottom, left, top);

    // Arrow heads
    int size = 8;
    // X arrow
    SDL_RenderLine(renderer, right, bottom, right - size, bottom - size);
    SDL_RenderLine(renderer, right, bottom, right - size, bottom + size);
    // Y arrow
    SDL_RenderLine(renderer, left, top, left - size, top + size);
    SDL_RenderLine(renderer, left, top, left + size, top + size);
}

static void drawAxisLabels(SDL_Renderer * renderer, TTF_Font * font, PlotArea * p,
                           const char *labelX, const char *labelY) {
    float left, right, top, bottom;
    innerBounds(p, &left, &right, &top, &bottom);
    drawText(renderer, font, labelX, right + 8, bottom - 10);               // X: "Minutes"
    drawText(renderer, font, labelY, left - 12, top - 20);                   // Y: "Cars currently parked"
}

static inline float mapYValueToPixel(float v, float vMin, float vMax, float top, float bottom) {
    if (vMax == vMin) return (top + bottom) * 0.5f;
    float t = (v - vMin) / (vMax - vMin);
    return bottom - t * (bottom - top); // invert (screen Y grows downward)
}

// Y ticks at 0, 1/4, 1/2, 3/4, 1; no tick at Y=0; no tick at arrowhead (top).
static void drawYTicksAndGrid(SDL_Renderer *renderer, TTF_Font *font, PlotArea *p, float yMin, float yMax) {
    float left, right, top, bottom;
    innerBounds(p, &left, &right, &top, &bottom);

    const float fractions[5] = {0.f, 0.25f, 0.5f, 0.75f, 1.f};
    const float eps = 1e-4f;

    for (int i = 0; i < 5; ++i) {
        float val = yMin + fractions[i] * (yMax - yMin);
        float y   = mapYValueToPixel(val, yMin, yMax, top, bottom);

        // Grid line (always draw)
        SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
        SDL_RenderLine(renderer, left, y, right, y);

        // Decide tick visibility:
        // - hide at Y=0 specifically
        // - hide at arrowhead (top => i==4)
        int showTick = 1;
        if (nearlyEqual(val, 0.0f, eps)) showTick = 0;
        if (i == 4) showTick = 0;

        if (showTick) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderLine(renderer, left - 6, y, left, y);
        }

        // Label (always show)
        char buf[64];
        snprintf(buf, sizeof(buf), "%.3g", val);
        drawText(renderer, font, buf, left - 46, y - 8);
    }
}

// X ticks at 0, 1/4, 1/2, 3/4, 1; bottom ticks; hide tick at arrowhead (right end)
static void drawXTicksAndGridMinutes(SDL_Renderer *renderer, TTF_Font *font, PlotArea *p, int sampleCount) {
    if (sampleCount <= 0) return;

    float left   = p->rect.x + p->padding;
    float bottom = p->rect.y + p->rect.h - p->padding;
    float right  = p->rect.x + p->rect.w - 20.0f;
    float top    = p->rect.y + 20.0f;

    float w = right - left;
    if (w <= 0) return;

    const float fractions[5] = {0.f, 0.25f, 0.5f, 0.75f, 1.f};
    int denom = (sampleCount > 1) ? (sampleCount - 1) : 1;

    for (int i = 0; i < 5; ++i) {
        float f = fractions[i];
        float x = left + f * w;

        // Grid
        SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
        SDL_RenderLine(renderer, x, top, x, bottom);

        // Tick at bottom (hide at arrowhead/right end)
        if (i != 4) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderLine(renderer, x, bottom, x, bottom - 6);
        }

        // Label (minutes = sample index since 1/min)
        int idx = (int)lroundf(f * (float)denom);
        float minutes = (float)idx;

        char buf[64];
        snprintf(buf, sizeof(buf), "%.3g", minutes);
        drawText(renderer, font, buf, x - 10, bottom + 14);
    }
}

// ================== Data Range & Resampling ==================
static void computeDataMinMax(const float *data, int count, float *outMin, float *outMax) {
    if (!data || count <= 0) { *outMin = 0.f; *outMax = 1.f; return; }
    float mn = data[0], mx = data[0];
    for (int i = 1; i < count; ++i) {
        if (data[i] < mn) mn = data[i];
        if (data[i] > mx) mx = data[i];
    }
    *outMin = mn; *outMax = mx;
}

static void computeYRange(const float *data, int count, float *outMin, float *outMax) {
    float mn, mx;
    computeDataMinMax(data, count, &mn, &mx);
    if (mn >= 0.0f) {
        *outMin = 0.0f;
        *outMax = (mx == 0.0f) ? 1.0f : mx;
    } else {
        if (mx == mn) mx = mn + 1.0f;
        float pad = 0.05f * (mx - mn);
        *outMin = mn - pad;
        *outMax = mx + pad;
    }
}

// Linear resampling to outCount samples uniformly over original index domain [0..count-1]
static float* resampleUniform(const float *data, int count, int outCount) {
    if (!data || count <= 0 || outCount <= 0) return NULL;
    float *out = (float*)malloc(sizeof(float) * (size_t)outCount);
    if (!out) return NULL;

    if (count == 1) {
        for (int j = 0; j < outCount; ++j) out[j] = data[0];
        return out;
    }

    for (int j = 0; j < outCount; ++j) {
        float t = (outCount == 1) ? 0.0f : (float)j / (float)(outCount - 1);
        float x = t * (float)(count - 1);
        int i0 = (int)floorf(x);
        int i1 = (i0 + 1 < count) ? (i0 + 1) : (count - 1);
        float a = x - (float)i0;
        out[j] = (1.0f - a) * data[i0] + a * data[i1];
    }
    return out;
}

// ================== Graph Drawing ==================
static void drawGraph(SDL_Renderer *renderer, PlotArea *p, float *data, int count, float yMin, float yMax) {
    if (!data || count <= 1) return;

    float left, right, top, bottom;
    innerBounds(p, &left, &right, &top, &bottom);

    float w = right - left;
    float h = bottom - top;
    if (w <= 0 || h <= 0) return;

    SDL_SetRenderDrawColor(renderer, 220, 60, 60, 255);

    for (int i = 0; i < count - 1; ++i) {
        float t0 = (float)i / (float)(count - 1);
        float t1 = (float)(i + 1) / (float)(count - 1);

        float x0 = left + t0 * w;
        float x1 = left + t1 * w;

        float v0 = data[i];
        float v1 = data[i + 1];

        float y0 = mapYValueToPixel(v0, yMin, yMax, top, bottom);
        float y1 = mapYValueToPixel(v1, yMin, yMax, top, bottom);

        SDL_RenderLine(renderer, x0, y0, x1, y1);
    }
}

// ================== Datasets: Random generation ==================
static float frand01(void) {
    return (float)rand() / (float)RAND_MAX;
}

static void freeDataset(Series *s) {
    if (s && s->v) {
        free(s->v);
        s->v = NULL;
        s->n = 0;
    }
}

// Create a dataset with random length and values [0, 100]
static void createRandomDataset(Series *s, int nMin, int nMax, float vMin, float vMax) {
    freeDataset(s);
    if (nMax < nMin) nMax = nMin;
    int n = nMin + (int)((nMax - nMin + 1) * frand01());
    if (n <= 0) n = nMin > 0 ? nMin : 1;

    s->v = (float*)malloc(sizeof(float) * (size_t)n);
    s->n = n;
    float range = vMax - vMin;
    for (int i = 0; i < n; ++i) {
        s->v[i] = vMin + frand01() * range;
    }
}

// ================== Layout ==================
static void computeLayout(int width, int height, LayoutMetrics *L, PlotArea *plot,
                          Button *leftBtn, Button *rightBtn, Button *closeBtn) {
    L->topBar      = TOP_BAR_H;
    L->leftBtnPad  = EDGE_MARGIN;
    L->rightBtnPad = EDGE_MARGIN;

    // Position side buttons at fixed 50x50, vertically centered in plotting region
    leftBtn->rect.w = rightBtn->rect.w = SIDE_BTN_W;
    leftBtn->rect.h = rightBtn->rect.h = SIDE_BTN_H;

    float plotTop    = L->topBar + EDGE_MARGIN;
    float plotHeight = height - L->topBar - 2.0f * EDGE_MARGIN;  // "full height" (minus top bar and 10px margins)
    float plotBottom = plotTop + plotHeight;

    // Side buttons X positions
    leftBtn->rect.x  = L->leftBtnPad;
    rightBtn->rect.x = width - L->rightBtnPad - rightBtn->rect.w;

    // Side buttons Y positions (centered vertically across the plot height)
    float centerY = plotTop + plotHeight * 0.5f;
    leftBtn->rect.y  = centerY - leftBtn->rect.h * 0.5f;
    rightBtn->rect.y = centerY - rightBtn->rect.h * 0.5f;

    // Plot rectangle uses full height, and leaves padding of 10 to the side buttons
    float plotLeft  = leftBtn->rect.x + leftBtn->rect.w + SIDE_PAD;
    float plotRight = rightBtn->rect.x - SIDE_PAD;
    float plotWidth = fmaxf(100.0f, plotRight - plotLeft);

    plot->rect.x = plotLeft;
    plot->rect.y = plotTop;
    plot->rect.w = plotWidth;
    plot->rect.h = plotHeight;

    // Close button (top-right corner inside top bar)
    closeBtn->rect.w = closeBtn->rect.h = CLOSE_SIZE;
    closeBtn->rect.x = width  - CLOSE_MARGIN - closeBtn->rect.w;
    closeBtn->rect.y = CLOSE_MARGIN;
}