#include "../../include/statistics_output_lib.h"

/*
@brief prints the simulation parameters.
*/
void print_head_data (int sim_nr, t_Time sim_duration, int parking_cells, t_Time max_parking_duration, float new_car_prob, int max_cars_arriving, unsigned int seed) {
    char sim_duration_s[] = "Simulated Steps";
    char parking_cells_s[] = "Parking Cells";
    char max_parking_duration_s[] = "Max. Parking Time";
    char new_car_prob_s[] = "New Car Prob.";
    char max_cars_arriving_s[] = "Max. New Cars";
    char seed_s[] = "Seed";
    
    char curr_step_s[] = "Current Step";
    char curr_cars_s[] = "Parked Cars";
    char avg_time_s[] = "Avg. Time";
    char q_len_s[] = "Queue Length";
    char full_house_s[] = "Full Garage";
    char tot_sim_car_s[] = "Cars Simulated";
    char most_brand_s[] = "Most Brand";


    printf("Simulation Nr. %i:\n", sim_nr);
    printf("\n");

    printf("|%15s|%15s|%19s|%15s|%15s|%15s|\n", sim_duration_s, parking_cells_s, max_parking_duration_s, new_car_prob_s, max_cars_arriving_s, seed_s);
    printf("|%15i|%15i|%19i|%14.2f%%|%15i|%15i|\n", sim_duration, parking_cells, max_parking_duration, new_car_prob, max_cars_arriving, seed);
    
    printf("\n\n");
    printf("|%15s|%15s|%19s|%15s|%15s|%15s|%15s|\n", curr_step_s, curr_cars_s, avg_time_s, q_len_s, full_house_s, tot_sim_car_s, most_brand_s);
}


/*
@brief prints the simulated data per timestep
*/
void print_data_per_timestep (t_Time timestep, int cars_parked, float avg_parking_time, int q_len, int full_house_steps, int tot_cars_simulated, Car_Brand most_brand) { 
    printf("|%15i|%15i|%15.2f|%15i|%15i|%15i|%15i|\n", timestep, cars_parked, avg_parking_time, q_len, full_house_steps, tot_cars_simulated, most_brand);
}


/*
@brief Rewrites the dataset with new data based on the index.
*/
float *loadNewDataset(int datasetIndex, char *filepath) {
    return NULL; // Placeholder for actual implementation
}


/* 
@brief A simple linear interpolation function.
*/
float lerp(float a, float b, float t) {
    return a + (b - a) * t; 
}


/*
@brief Checks if a point (x, y) is inside a given rectangle.
*/
int isPointInsideRect(float x, float y, SDL_FRect * rect) {
    return (x >= rect->x && x <= rect->x + rect->w &&
            y >= rect->y && y <= rect->y + rect->h);
}


/*
@brief Calculates the inner bounds of a plot rectangle,
accounting for PADDING and saves it in the provided pointers.
*/
void innerBounds(SDL_FRect *plot, float *left, float *right, float *top, float *bottom) {
    *left   = plot->x + PADDING;
    *bottom = plot->y + plot->h - PADDING;
    *right  = plot->x + plot->w - 20.0f; // keep space for arrow
    *top    = plot->y - 20.0f;            // keep space for arrow
}


/*
@brief checks if two values are equal within a tolerance
*/
int nearlyEqual(float a, float b, float tol) {
    return fabsf(a - b) <= tol;
}


/*
@brief Updates the hover state of a button based on the current mouse position.
*/
void updateButtonHover(Button *btn, float mouseX, float mouseY) {
    btn->hovered = isPointInsideRect(mouseX, mouseY, &btn->rect);
}


/*
@brief updates the animation state of a button based on the time passed since the last update.
*/
void updateButtonAnimation(Button *btn, float dt) {
    if(btn->hovered) {
        btn->hoverAnim += (1.0f - btn->hoverAnim) * SPEED * dt;
    } else {
        btn->hoverAnim += (0.0f - btn->hoverAnim) * SPEED * dt;
    }

    if(btn->pressed) {
        btn->pressAnim += (1.0f - btn->pressAnim) * SPEED * dt;
    } else {
        btn->pressAnim += (0.0f - btn->pressAnim) * SPEED * dt;
    }
}


/*
@brief Draws a cyan glowing & enlarging effect around a given rectangle.
*/
void drawGlow(SDL_Renderer *renderer, SDL_FRect *rect, float intensity) {
    int layers = 6;
    for (int i = 0; i < layers; i++) {
        float expand = (float) i * 4.0f;
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
@brief Draws a button with the renderer
*/
void drawButton(SDL_Renderer *renderer, Button *btn) {
    drawGlow(renderer, &btn->rect, btn->hoverAnim);

    SDL_Color base  = {0, 150, 255, 255};
    SDL_Color hover = {0, 200, 255, 255};
    float t = btn->hoverAnim;

    Uint8 r = (Uint8)lerp(base.r, hover.r, t);
    Uint8 g = (Uint8)lerp(base.g, hover.g, t);
    Uint8 b = (Uint8)lerp(base.b, hover.b, t);
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    float pressScale = 1.0f - 0.1f * btn->pressAnim;
    float w = btn->rect.w * pressScale;
    float h = btn->rect.h * pressScale;
    float x = btn->rect.x + (btn->rect.w - w) / 2.0f;
    float y = btn->rect.y + (btn->rect.h - h) / 2.0f;

    SDL_FRect rct = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rct);
}


/*
@brief Draws an arrow icon inside a given rectangle, with a specified direction and hover animation.

This function was made using Copilot, as its geometry is a bit tricky and I didn't want to do it manually.
*/
void drawArrow(SDL_Renderer *renderer, SDL_FRect *rect, int direction, float hoverAnim) {
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


/*
@brief Draws a centered "X" icon inside a given rectangle, with a hover animation effect.

This function was made using Copilot, as its basic geometry and I wanted to save time.
*/
void drawXIcon(SDL_Renderer *renderer, SDL_FRect *rect, float hoverAnim) {
    float cx = rect->x + rect->w * 0.5f;
    float cy = rect->y + rect->h * 0.5f;

    float arm = fminf(rect->w, rect->h) * -0.15f * hoverAnim;
    float x1 = cx - arm, y1 = cy - arm;
    float x2 = cx + arm, y2 = cy + arm;

    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderLine(renderer, x1, y1, x2, y2);
    SDL_RenderLine(renderer, x1, y2, x2, y1);
}


/*
@brief Handles events for navigation buttons.
*/
int handleNavButtonEvent(Button *btn, SDL_Event *event) {
    int clicked = 0;
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (btn->hovered && !btn->pressed) {
            btn->pressed = TRUE;
        } 
    } else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        if (btn->pressed && btn->hovered) {
            clicked = 1;
        }
        btn->pressed = FALSE;
    }
    return clicked;
}


/*
@brief Handles events for the close button.
*/
void handleCloseButtonEvent(Button *btn, SDL_Event *event, int *running) {
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (btn->hovered && !btn->pressed) btn->pressed = TRUE;
    }
    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        if (btn->pressed && btn->hovered) *running = FALSE;
        btn->pressed = FALSE;
    }
}


/*
@brief Opens a TTF font from a specified file path with a specified size.
*/
TTF_Font * openFont(char *path, int size) {
    if (TTF_Init() < 0) {
        printf("TTF init failed: %s", SDL_GetError());
        return NULL;
    }
    TTF_Font * font = TTF_OpenFont(path, size);
    if (!font) {
        printf("Failed to load font: %s", SDL_GetError());
        return NULL;
    }
    return font;
}


/*
@brief Draws text on the screen.

This function was done using Copilot, as I had no Idea how to use Text with SDL.
*/
void drawText(SDL_Renderer *renderer, TTF_Font *font, char *text, float x, float y) {
    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, 0, color);
    
    if (!surface) {
        printf("Failed to render text surface: %s", SDL_GetError());
        return;
    } 

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (!texture) { 
        SDL_DestroySurface(surface); 
        printf("Failed to create texture from surface: %s", SDL_GetError());
        return; 
    }

    SDL_FRect rec = {x, y, (float)surface->w, (float)surface->h};
    SDL_RenderTexture(renderer, texture, NULL, &rec);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}


/*
@brief Draws the background in white
*/
void drawBackground(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect rect = {0, 0, 1200, 900};
    SDL_RenderFillRect(renderer, &rect);
}


/*
@brief Draws the axes of the plot area with arrowheads.

This function was made using Copilot, as I didn't want to calculate the geometry.
*/
void drawAxesWithArrows(SDL_Renderer *renderer, SDL_FRect *plot) {
    float left, right, top, bottom;
    innerBounds(plot, &left, &right, &top, &bottom);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderLine(renderer, left, bottom, right, bottom); // X axis
    SDL_RenderLine(renderer, left, bottom, left,  top);    // Y axis

    int size = 8;
    // X arrow
    SDL_RenderLine(renderer, right, bottom, right - size, bottom - size);
    SDL_RenderLine(renderer, right, bottom, right - size, bottom + size);
    // Y arrow
    SDL_RenderLine(renderer, left, top, left - size, top + size);
    SDL_RenderLine(renderer, left, top, left + size, top + size);
}


/*
@brief Draws the labels for the X and Y axes of the plot area.
*/
void drawAxisLabels(SDL_Renderer *renderer, TTF_Font *font, SDL_FRect *plot, char *labelX, char *labelY) {
    float left, right, top, bottom;
    innerBounds(plot, &left, &right, &top, &bottom);
    drawText(renderer, font, labelX, right + 8,  bottom - 10); // X: "Minutes"
    drawText(renderer, font, labelY, left  - 12, top    - 20); // Y: dataset name
}


/*
@brief Maps a Y value to a pixel coordinate within the plot area.
*/
float mapYValueToPixel(float v, float vMin, float vMax, float top, float bottom) {
    if (vMax == vMin) {
        return (top + bottom) * 0.5f;
    }
    float t = (v - vMin) / (vMax - vMin);
    return bottom - t * (bottom - top);
}


/*
@brief Draws Y-axis ticks and grid lines on the plot area.

This function was done using Copilot, as it's just basic math and boring drawing. (I'm lazy)
*/
void drawYTicksAndGrid(SDL_Renderer *renderer, TTF_Font *font, SDL_FRect *plot, float yMin, float yMax) {
    float left, right, top, bottom;
    innerBounds(plot, &left, &right, &top, &bottom);

    float fractions[5] = {0.f, 0.25f, 0.5f, 0.75f, 1.f};
    float tol = 1e-4f;

    for (int i = 0; i < 5; ++i) {
        float val = yMin + fractions[i] * (yMax - yMin);
        float y = mapYValueToPixel(val, yMin, yMax, top, bottom);

        SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
        SDL_RenderLine(renderer, left, y, right, y);

        int showTick = 1;
        if (nearlyEqual(val, 0.0f, tol)) showTick = 0;
        if (i == 4) showTick = 0;

        if (showTick) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderLine(renderer, left - TICK_SIZE, y, left, y);
        }

        char buf[64];
        snprintf(buf, sizeof(buf), "%.3f", val);
        drawText(renderer, font, buf, left - 46, y - TICK_SIZE + 2);
    }
}


/*
@brief Draws X-axis ticks and grid lines on the plot area, with labels representing minutes.

Basically copied from drawYTicksAndGrid.
*/
void drawXTicksAndGridMinutes(SDL_Renderer *renderer, TTF_Font *font, SDL_FRect *plot, int sampleCount) {
    if (sampleCount <= 0) return;

    float left, right, top, bottom;
    innerBounds(plot, &left, &right, &top, &bottom);

    float fractions[5] = {0.f, 0.25f, 0.5f, 0.75f, 1.f};
    int denom = sampleCount - 1;

    for (int i = 0; i < 5; ++i) {
        float x = left + fractions[i] * (right - left);

        SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
        SDL_RenderLine(renderer, x, top, x, bottom);

        if (i != 4) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderLine(renderer, x, bottom, x, bottom - TICK_SIZE);
        }

        float minutes = fractions[i] * (float)denom;
        char buf[64];
        snprintf(buf, sizeof(buf), "%.3f", minutes);
        drawText(renderer, font, buf, x - 10, bottom + TICK_SIZE + 6);
    }
}


/*
@brief Computes the minimum and maximum values in a dataset.
*/
void computeDataMinMax(float *data, int count, float *outMin, float *outMax) {
    if (!data || count <= 0) { 
        *outMin = 0.f; 
        *outMax = 1.f; 
        return; 
    }

    float mn = data[0];
    float mx = data[0];

    for (int i = 1; i < count; ++i) {
        if (data[i] < mn) {
            mn = data[i];
        } else if (data[i] > mx) {
            mx = data[i];
        }
    }
    *outMin = mn; 
    *outMax = mx;
}


/*
@brief Computes the Y-axis range for a dataset.
*/
void computeYRange(float *data, int count, float *outMin, float *outMax) {
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


/*
@brief Draws a graph of the dataset on the plot area.
*/
void drawGraph(SDL_Renderer *renderer, SDL_FRect *plot, float *data, int count, float yMin, float yMax) {
    if (!data || count <= 1) return;

    float left, right, top, bottom;
    innerBounds(plot, &left, &right, &top, &bottom);

    float w = right - left;
    float h = bottom - top;
    if (w <= 0 || h <= 0) return;

    SDL_SetRenderDrawColor(renderer, 220, 60, 60, 255);

    for (int i = 0; i < count - 1; ++i) {
        float t0 = (float) i       / (float)(count - 1);
        float t1 = (float)(i + 1)  / (float)(count - 1);

        float x0 = left + t0 * w;
        float x1 = left + t1 * w;
        float y0 = mapYValueToPixel(data[i],     yMin, yMax, top, bottom);
        float y1 = mapYValueToPixel(data[i + 1], yMin, yMax, top, bottom);

        SDL_RenderLine(renderer, x0, y0, x1, y1);
    }
}


/*
@brief sets up the positions/sizes of the plot area and buttons.
*/
void computeLayout(int width, int height, SDL_FRect *plot,
                   Button *leftBtn, Button *rightBtn, Button *closeBtn) {
    leftBtn->rect.w = rightBtn->rect.w = SIDE_BTN_SIZE;
    leftBtn->rect.h = rightBtn->rect.h = SIDE_BTN_SIZE;

    float plotTop    = TOP_BAR_H + EDGE_MARGIN;
    float plotHeight = height - TOP_BAR_H - 2.0f * EDGE_MARGIN;

    leftBtn->rect.x  = EDGE_MARGIN;
    rightBtn->rect.x = width - EDGE_MARGIN - rightBtn->rect.w;

    float centerY = plotTop + plotHeight * 0.5f;
    leftBtn->rect.y  = centerY - leftBtn->rect.h * 0.5f;
    rightBtn->rect.y = centerY - rightBtn->rect.h * 0.5f;

    float plotLeft  = leftBtn->rect.x + leftBtn->rect.w + SIDE_PAD;
    float plotRight = rightBtn->rect.x - SIDE_PAD;

    plot->x = plotLeft;
    plot->y = plotTop;
    plot->w = fmaxf(100.0f, plotRight - plotLeft);
    plot->h = plotHeight;

    closeBtn->rect.w = closeBtn->rect.h = CLOSE_SIZE;
    closeBtn->rect.x = width  - CLOSE_MARGIN - closeBtn->rect.w;
    closeBtn->rect.y = CLOSE_MARGIN;
}