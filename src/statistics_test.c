/* 
Messed around with SDL, to see if its usable for the project.
The Glow Effect and hovering was done with the help of copilot, since math is hard and I'm
not an expert with SDL.
Code based of Tutorials from Programming Rainbow and Christopher Medina.

To run this version of gcc is needed: i686-w64-mingw32 and this command
gcc statistics_test.c -o test.exe -I "../external/SDL/i686-w64-mingw32/include" -I "../external/SDL3_ttf/i686-w64-mingw32/include" -L "../external/SDL/i686-w64-mingw32/lib"  -L "../external/SDL3_ttf/i686-w64-mingw32/lib"  -lSDL3 -lSDL3_ttf

*/

#include <SDL3/SDL.h>
#include <math.h>
#include <SDL3_ttf/SDL_ttf.h>


// Window dimensions
#define WIDTH 1000
#define HEIGHT 1000

// Boolean values
#define TRUE 1
#define FALSE 0

//Button struct
typedef struct {
    SDL_FRect rect;

    int hovered;
    int pressed;

    float hoverAnim;
    float pressAnim;

} Button;

//Plot struct
typedef struct {
    SDL_FRect rect;   
    float padding;    
} PlotArea;

/*
@brief Create a white plot area with given position and size
@param[1] x coordinate of the plot area
@param[2] y coordinate of the plot area
@param[3] size of the plot area (width and height)
@param[4] pointer to the SDL renderer (for drawing)
@return initialized PlotArea struct
*/
PlotArea createPlotArea(float x, float y, float size, SDL_Renderer * renderer) {
    PlotArea p;

    p.rect.x = x;
    p.rect.y = y;
    p.rect.w = size;
    p.rect.h = size;

    p.padding = 60.0f;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &p.rect);

    return p;
}

/*
@brief Draw positive X and Y axes inside the plot area
@param[1] pointer to the SDL renderer
@param[2] pointer to the plot area
*/
void drawAxes(SDL_Renderer * renderer, PlotArea * p) {


    float left   = p->rect.x + p->padding;
    float bottom = p->rect.y + p->rect.h - p->padding;

    float right  = p->rect.x + p->rect.w - 20.0f;
    float top    = p->rect.y + 20.0f;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderLine(renderer, (int)left, (int)bottom, (int)right, (int)bottom);

    SDL_RenderLine(renderer, (int)left, (int)bottom, (int)left, (int)top);
}

/*
@brief Draw an arrow head at the end of the axis
@param[1] pointer to the SDL renderer
@param[2] x coordinate of the arrow head
@param[3] y coordinate of the arrow head
@param[4] direction of the arrow (0 for X axis, 1 for Y axis)
*/
void drawArrowHead(SDL_Renderer * renderer, float x, float y, int dir) {

    int size = 8;

    if (!dir) {
        SDL_RenderLine(renderer, x, y, x - size, y - size);
        SDL_RenderLine(renderer, x, y, x - size, y + size);
    } else {
        SDL_RenderLine(renderer, x, y, x - size, y + size);
        SDL_RenderLine(renderer, x, y, x + size, y + size);
    }
}

/*
@brief Draw text at a given position using SDL_ttf
@param[1] pointer to the SDL renderer
@param[2] pointer to the TTF font
@param[3] text string to draw
@param[4] x coordinate of the text
@param[5] y coordinate of the text
*/
void drawText(SDL_Renderer * renderer, TTF_Font * font, char * text, float x, float y) {

    SDL_Color color = {0, 0, 0, 255};

    SDL_Surface *surface = TTF_RenderText_Blended(font, text, 0, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FRect dst = {x, y, surface->w, surface->h};

    SDL_RenderTexture(renderer, texture, NULL, &dst);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}


/*
@brief Draw axis labels "X" and "Y" at the ends of the axes
@param[1] pointer to the SDL renderer
@param[2] pointer to the TTF font
@param[3] pointer to the plot area
*/
void drawAxisLabels(SDL_Renderer * renderer, TTF_Font * font, PlotArea * p, char labelX[], char labelY[]) {

    float left   = p->rect.x + p->padding;
    float bottom = p->rect.y + p->rect.h - p->padding + 15.0f;

    float right  = p->rect.x + p->rect.w - 15.0f;
    float top    = p->rect.y + 20.0f;

    drawText(renderer, font, labelX, right + 5, bottom - 10);

    drawText(renderer, font, labelY, left - 10, top - 20);
}

/*
@brief Draw axes with arrow heads at the ends
@param[1] pointer to the SDL renderer
@param[2] pointer to the plot area
*/
void drawAxesWithArrows(SDL_Renderer *renderer, PlotArea *p) {

    float left   = p->rect.x + p->padding;
    float bottom = p->rect.y + p->rect.h - p->padding;

    float right  = p->rect.x + p->rect.w - 20.0f;
    float top    = p->rect.y + 20.0f;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderLine(renderer, left, bottom, right, bottom);
    SDL_RenderLine(renderer, left, bottom, left, top);
    
    drawArrowHead(renderer, right, bottom, 0);
    drawArrowHead(renderer, left, top, 1);
}

/*
@brief Draw white plot background
@param[1] pointer to the SDL renderer
@param[2] pointer to the plot area
*/
void drawPlotBackground(SDL_Renderer * renderer, PlotArea * p) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &p->rect);
}


/*
@brief Linear interpolation between two values
@param[1] start value
@param[2] end value
@param[3] interpolation factor (0.0 to 1.0)
@return interpolated value
*/
float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

/*
@brief Check if a point is inside a rectangle
@param[1] x coordinate of the point
@param[2] y coordinate of the point
@param[3] pointer to the rectangle
@return 1 if inside, 0 otherwise
*/
int isPointInsideRect(float x, float y, SDL_FRect * rect) {
    return (x >= rect->x && x <= rect->x + rect->w &&
            y >= rect->y && y <= rect->y + rect->h);
}

/*
@brief Update button hover state based on mouse position
@param[1] pointer to the button
@param[2] x coordinate of the mouse
@param[3] y coordinate of the mouse
*/
void updateButtonHover(Button * btn, float mouseX, float mouseY) {
    btn->hovered = isPointInsideRect(mouseX, mouseY, &btn->rect);
}

/*
@brief Handle button press and release events
@param[1] pointer to the button
@param[2] pointer to the SDL event
@param[3] name of the button (for logging)
*/
void handleButtonEvent(Button * btn, SDL_Event * event, char * name) {

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (btn->hovered && !btn->pressed) {
            btn->pressed = TRUE;
        }
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        if (btn->pressed && btn->hovered) {
            SDL_Log("%s BUTTON CLICKED", name);
        }
        btn->pressed = FALSE;
    }
}

/*
@brief Update button animation states
@param[1] pointer to the button
@param[2] delta time since last update
*/
void updateButtonAnimation(Button * btn, float dt) {

    float speed = 10.0f;

    //sets target value 1 if hovered/pressed is true, 0 otherwise
    float targetHover = btn->hovered ? 1.0f : 0.0f;
    float targetPress = btn->pressed ? 1.0f : 0.0f;

    btn->hoverAnim += (targetHover - btn->hoverAnim) * speed * dt;
    btn->pressAnim += (targetPress - btn->pressAnim) * speed * dt;
}

/*
@brief Draw a glowing effect around the button
@param[1] pointer to the SDL renderer
@param[2] pointer to the button rectangle
@param[3] intensity of the glow (0.0 to 1.0)
*/
void drawGlow(SDL_Renderer * renderer, SDL_FRect * rect, float intensity) {

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

/*
@brief Draw a button with glow and animation effects
@param[1] pointer to the SDL renderer
@param[2] pointer to the button
*/
void drawButton(SDL_Renderer * renderer, Button * btn) {

    // Glow
    drawGlow(renderer, &btn->rect, btn->hoverAnim);

    // Color animation
    SDL_Color base = {0, 150, 255, 255};
    SDL_Color hover = {0, 200, 255, 255};

    float t = btn->hoverAnim;

    Uint8 r = (Uint8)lerp(base.r, hover.r, t);
    Uint8 g = (Uint8)lerp(base.g, hover.g, t);
    Uint8 b = (Uint8)lerp(base.b, hover.b, t);

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    // Press animation (shrink)
    float pressScale = 1.0f - 0.1f * btn->pressAnim;

    float w = btn->rect.w * pressScale;
    float h = btn->rect.h * pressScale;

    float x = btn->rect.x + (btn->rect.w - w) / 2.0f;
    float y = btn->rect.y + (btn->rect.h - h) / 2.0f;

    SDL_FRect rct = {x, y, w, h};

    SDL_RenderFillRect(renderer, &rct);
}

/*
@brief Draw an arrow inside the button indicating direction
@param[1] pointer to the SDL renderer
@param[2] pointer to the button rectangle
@param[3] direction of the arrow (-1 for left, 1 for right)
@param[4] intensity of the hover animation (0.0 to 1.0)
*/
void drawArrow(SDL_Renderer * renderer, SDL_FRect * rect, int direction, float hoverAnim) {

    int cx = (int)(rect->x + rect->w / 2);
    int cy = (int)(rect->y + rect->h / 2);

    int size = (int)(10 + 4 * hoverAnim);

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);

    if (direction == -1) {
        SDL_RenderLine(renderer, cx + size, cy - size, cx - size, cy);
        SDL_RenderLine(renderer, cx - size, cy, cx + size, cy + size);
    } else {
        SDL_RenderLine(renderer, cx - size, cy - size, cx + size, cy);
        SDL_RenderLine(renderer, cx + size, cy, cx - size, cy + size);
    }
}

TTF_Font * openFont(const char * path, int size) {
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


int main() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL init failed: %s", SDL_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Animated Buttons", WIDTH, HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    PlotArea plot = createPlotArea(100, 50, 600, renderer);

    TTF_Font * font = openFont("../external/Font/ARIAL.TTF", 12);

    Button left = {{0,0,0,0}, FALSE, FALSE, 0.0f, 0.0f};
    Button right = {{0,0,0,0}, FALSE, FALSE, 0.0f, 0.0f};

    int running = TRUE;
    SDL_Event event;

    Uint64 last = 0;

    while (running) {

        Uint64 now = SDL_GetTicks();
        float dt = (last == 0) ? 0.016f : (now - last) / 1000.0f;
        last = now;

        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        float scale = height / 1000.0f;

        left.rect = (SDL_FRect){20.0f * scale, height/2.0f - 25.0f*scale, 100.0f*scale, 50.0f*scale};
        right.rect = (SDL_FRect){width - 120.0f*scale, height/2.0f - 25.0f*scale, 100.0f*scale, 50.0f*scale};

        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        updateButtonHover(&left, mouseX, mouseY);
        updateButtonHover(&right, mouseX, mouseY);

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_EVENT_QUIT)
                running = FALSE;

            handleButtonEvent(&left, &event, "LEFT");
            handleButtonEvent(&right, &event, "RIGHT");
        }

        updateButtonAnimation(&left, dt);
        updateButtonAnimation(&right, dt);

        SDL_SetRenderDrawColor(renderer, 20,20,20,255);
        SDL_RenderClear(renderer);

        drawButton(renderer, &left);
        drawButton(renderer, &right);

        drawArrow(renderer, &left.rect, -1, left.hoverAnim);
        drawArrow(renderer, &right.rect, 1, right.hoverAnim);

        drawPlotBackground(renderer, &plot);
        drawAxesWithArrows(renderer, &plot);
        drawAxisLabels(renderer, font, &plot, "X", "Y");

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}