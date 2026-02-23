/* 
Messed around with SDL, to see if its usable for the project.
The Glow Effect and hovering was done with the help of copilot, since math is hard and I'm
not an expert with SDL


To run this version of gcc is needed: i686-w64-mingw32 and this command
gcc statistics_test.c -o test.exe -I "../external/SDL/i686-w64-mingw32/include" -L "../external/SDL/i686-w64-mingw32/lib" -lSDL3


*/

#include <SDL3/SDL.h>
#include <math.h>


// Window dimensions
#define WIDTH 800
#define HEIGHT 600

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
int isPointInsideRect(float x, float y, SDL_FRect *r) {
    return (x >= r->x && x <= r->x + r->w &&
            y >= r->y && y <= r->y + r->h);
}


/*
@brief Update button hover state based on mouse position
@param[1] pointer to the button
@param[2] x coordinate of the mouse
@param[3] y coordinate of the mouse
*/
void updateButtonHover(Button *btn, float mouseX, float mouseY) {
    btn->hovered = isPointInsideRect(mouseX, mouseY, &btn->rect);
}

/*
@brief Handle button press and release events
@param[1] pointer to the button
@param[2] pointer to the SDL event
@param[3] name of the button (for logging)
*/
void handleButtonEvent(Button *btn, SDL_Event *event, const char *name) {

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
void updateButtonAnimation(Button *btn, float dt) {

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
void drawGlow(SDL_Renderer *renderer, SDL_FRect *rect, float intensity) {

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
void drawButton(SDL_Renderer *renderer, Button *btn) {

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
void drawArrow(SDL_Renderer *renderer, SDL_FRect *rect, int direction, float hoverAnim) {

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



int main() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL init failed: %s", SDL_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Animated Buttons", WIDTH, HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    Button left = {{0,0,0,0}, FALSE, FALSE, 0.0f, 0.0f};
    Button right = {{0,0,0,0}, FALSE, FALSE, 0.0f, 0.0f};

    int running = TRUE;
    SDL_Event event;

    Uint64 last = 0;

    while (running) {

        // Delta time
        Uint64 now = SDL_GetTicks();
        float dt = (last == 0) ? 0.016f : (now - last) / 1000.0f;
        last = now;

        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        float scale = height / 600.0f;

        // Layout
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

        // Render
        SDL_SetRenderDrawColor(renderer, 20,20,20,255);
        SDL_RenderClear(renderer);

        drawButton(renderer, &left);
        drawButton(renderer, &right);

        drawArrow(renderer, &left.rect, -1, left.hoverAnim);
        drawArrow(renderer, &right.rect, 1, right.hoverAnim);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}