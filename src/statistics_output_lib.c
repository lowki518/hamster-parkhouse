#include "statistics_output_lib.h"

/*
@brief Rewrites the dataset with new data based of the index.

@param[1] datasetIndex An integer representing the index of the dataset to load.

@return A pointer 
*/
float *loadNewDataset(int datasetIndex, char *filepath) {
    /*
    File = OPEN File from *filepath

    datasetSize = READ dataset size from File
    float *data = ALLOCATE memory for datasetSize

    FOR i FROM 0 to datasetSize-1:
        data[i] = value from File based on datasetIndex and i
    
    return data;
    */

    return NULL; // Placeholder for actual implementation
}


/* 
@brief A simple linear interpolation function.

@param[1] a The start value.
@param[2] b The end value.
@param[3] t The interpolation factor, between 0.0f and 1.0f.

@return The interpolated value between a and b.
*/
float lerp(float a, float b, float t) {
    return a + (b - a) * t; 
}

/*
@brief Checks if a point (x, y) is inside a given rectangle.

@param[1] x The x-coordinate of the point.
@param[2] y The y-coordinate of the point.
@param[3] rect A pointer to the rectangle

@return 1 if the point is inside the rectangle, 0 otherwise.
*/
int isPointInsideRect(float x, float y, SDL_FRect * rect) {
    return (x >= rect->x && x <= rect->x + rect->w &&
            y >= rect->y && y <= rect->y + rect->h);
}


/*
@brief Calculates the inner bounds of a PlotArea, 
accounting for padding and saves it in the provided pointers.

@param[1] p A pointer to the PlotArea.
@param[2] left A pointer to a float where the left bound will be stored.
@param[3] right A pointer to a float where the right bound will be stored.
@param[4] top A pointer to a float where the top bound will be stored.
@param[5] bottom A pointer to a float where the bottom bound will be stored.

@return void
*/
void innerBounds(PlotArea *p, float *left, float *right, float *top, float *bottom) {
    *left   = p->rect.x + p->padding;
    *bottom = p->rect.y + p->rect.h - p->padding;
    *right  = p->rect.x + p->rect.w - 20.0f; // keep space for arrow
    *top    = p->rect.y - 20.0f;             // keep space for arrow
}


/*
@brief checks if two values are equal within a tolerance

@param[1] a The first value to compare.
@param[2] b The second value to compare.
@param[3] tol The tolerance within which the two values are considered equal.

@return 1 if the values are nearly equal within the specified tolerance, 0 otherwise.
*/
int nearlyEqual(float a, float b, float tol) {
    return fabsf(a - b) <= tol; //fabsf gets the absolute differenze
}


/*
@brief Updates the hover state of a button based on the current mouse position.

@param[1] btn A pointer to the button to update.
@param[2] mouseX The current x-coordinate of the mouse.
@param[3] mouseY The current y-coordinate of the mouse.

@return void
*/
void updateButtonHover(Button *btn, float mouseX, float mouseY) {
    btn->hovered = isPointInsideRect(mouseX, mouseY, &btn->rect);
}


/*
@brief updates the animation state of a button based on the time passed since the last update.

@param[1] btn A pointer to the button to update.
@param[2] dt The time in milliseconds since the last update.

@return void
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

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] rect A pointer to the rectangle to draw the glow around.
@param[3] intensity A float value between 0.0f and 1.0f representing the intensity of the glow effect.

@return void
*/
 void drawGlow(SDL_Renderer *renderer, SDL_FRect *rect, float intensity) {
    int layers = 6; //Multiple layers for a smoother animation 
    for (int i = 0; i < layers; i++) {
        float expand = (float) i * 4.0f;
        float alpha = (1.0f - (float)i / layers) * 80.0f * intensity;
        SDL_SetRenderDrawColor(renderer, 0, 200, 255, (Uint8)alpha); //Uint8 for safety
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

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] btn A pointer to the button to draw.

@return void
*/
void drawButton(SDL_Renderer *renderer, Button *btn) {
    drawGlow(renderer, &btn->rect, btn->hoverAnim);

    // Fill color (animated)
    SDL_Color base  = {0, 150, 255, 255};
    SDL_Color hover = {0, 200, 255, 255};
    float t = btn->hoverAnim;

    // Sets the color for the button based on the hover animation
    // Linear interpolation for smoother transition
    Uint8 r = (Uint8)lerp(base.r, hover.r, t); // Uint8 for safety
    Uint8 g = (Uint8)lerp(base.g, hover.g, t); // Uint8 for safety
    Uint8 b = (Uint8)lerp(base.b, hover.b, t); // Uint8 for safety
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    // Press Animation (scale down)
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

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] rect A pointer to the rectangle to draw the arrow in.
@param[3] direction An integer indicating the direction of the arrow (-1 for left, 1 for right).
@param[4] hoverAnim A float value between 0.0f and 1.0f representing the hover animation state.

@return void
*/
void drawArrow(SDL_Renderer *renderer, SDL_FRect *rect, int direction, float hoverAnim) {
    int cx = (int)(rect->x + rect->w / 2);
    int cy = (int)(rect->y + rect->h / 2);
    int size = (int)(rect->h * 0.33f + 3 * hoverAnim);

    // Draw the arrow using two lines forming a ">" or "<" shape in black
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

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] rect A pointer to  the rectangle to draw the "X" icon in.
@param[3] hoverAnim A float value between 0.0f and 1.0f representing the hover animation state.

@return void
*/
void drawXIcon(SDL_Renderer *renderer, SDL_FRect *rect, float hoverAnim) {
    // Gets the centre of the rectangle
    float cx = rect->x + rect->w * 0.5f;
    float cy = rect->y + rect->h * 0.5f;

    // Calculates the arm length of the "X" based on the rectangle size
    float arm = fminf(rect->w, rect->h) * -0.15f * hoverAnim;
    float x1 = cx - arm, y1 = cy - arm;
    float x2 = cx + arm, y2 = cy + arm;

    // Draw the "X" using two crossing lines in black
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderLine(renderer, x1, y1, x2, y2);
    SDL_RenderLine(renderer, x1, y2, x2, y1);
}

/*
@brief Handles events for navigation buttons.

@param[1] btn A pointer to the button to handle events for.
@param[2] event A pointer to the event to handle.

@return 1 if the button was clicked, 0 otherwise.
*/
int handleNavButtonEvent(Button *btn, SDL_Event *event) {
    int clicked = 0;
    // Checks if the button was pressed and released
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

@param[1] btn A pointer to the close button.
@param[2] event A pointer to the event to handle.
@param[3] running A pointer to the running state of the application, which will be set to 0 if the close button is clicked.

@return void
*/
void handleCloseButtonEvent(Button *btn, SDL_Event *event, int *running) {
    // Checks if the button was pressed and released
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

@param[1] path A string representing the file path to the font to open.
@param[2] size An integer representing the size of the font.

@return A pointer to the opened font, or NULL if the font could not be opened.
*/
TTF_Font * openFont(char *path, int size) {
    // Checks if TTF was initialized successfully
    if (TTF_Init() < 0) {
        printf("TTF init failed: %s", SDL_GetError());
        return NULL;
    }
    // Opens the font and checks if it was successful
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

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] font A pointer to the font to use for rendering the text.
@param[3] text A string representing the text to draw.
@param[4] x The x-coordinate where the text should be drawn.
@param[5] y The y-coordinate where the text should be drawn.

@return void
*/
void drawText(SDL_Renderer *renderer, TTF_Font *font, char *text, float x, float y) {
    // Sets the ssurface to draw the text on
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

    // Renders the text at a specific position
    SDL_FRect rec = {x, y, (float)surface->w, (float)surface->h};
    SDL_RenderTexture(renderer, texture, NULL, &rec);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}


/*
@brief Creates a new PlotArea with a white background.

@param[1] x The x-coordinate of the top-left corner of the PlotArea.
@param[2] y The y-coordinate of the top-left corner of the PlotArea.
@param[3] w The width of the PlotArea.
@param[4] h The height of the PlotArea.
@param[5] renderer A pointer to renderer to use for drawing the background.

@return A PlotArea structure representing the created PlotArea.
*/
PlotArea createPlotArea(float x, float y, float w, float h, SDL_Renderer * renderer) {
    PlotArea p;
    p.rect.x = x; p.rect.y = y; p.rect.w = w; p.rect.h = h;
    p.padding = PADDING;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &p.rect);
    return p;
}

/*
@brief Draws the background of the plot area.

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] p A pointer to the PlotArea to draw the background for.

@return void
*/
void drawPlotBackground(SDL_Renderer * renderer, PlotArea * p) {
    // Draws the background in white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &p->rect);
}

/*
@brief Draws the axes of the plot area with arrowheads.

This function was made using Copilot, as I didn't want to calculate the geometry.

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] p A pointer to the PlotArea to draw in.

@return void
*/
void drawAxesWithArrows(SDL_Renderer *renderer, PlotArea *p) {
    float left, right, top, bottom;
    innerBounds(p, &left, &right, &top, &bottom);

    // Draw axes in black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // X axis
    SDL_RenderLine(renderer, left, bottom, right, bottom);
    // Y axis
    SDL_RenderLine(renderer, left, bottom, left, top);

    // Arrow heads; size was tested
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

@param[1] renderer A pointer to the SDL_Renderer to use for drawing.
@param[2] font A pointer to the TTF_Font to use for rendering the text.
@param[3] p A pointer to the PlotArea structure representing the plot area to draw.
@param[4] labelX A string representing the label for the X axis.
@param[5] labelY A string representing the label for the Y axis.

@return void
*/
void drawAxisLabels(SDL_Renderer *renderer, TTF_Font *font, PlotArea *p, char *labelX, char *labelY) {
    float left, right, top, bottom;
    innerBounds(p, &left, &right, &top, &bottom);
    drawText(renderer, font, labelX, right + 8, bottom - 10);               // X: "Minutes"
    drawText(renderer, font, labelY, left - 12, top - 20);                   // Y: "Cars currently parked"
}


/*
@brief Maps a Y value to a pixel coordinate within the plot area, based on the specified Y range and plot dimensions.

@param[1] v The Y value to map.
@param[2] vMin The minimum Y value of the range.
@param[3] vMax The maximum Y value of the range.
@param[4] top The top pixel coordinate of the plot area.
@param[5] bottom The bottom pixel coordinate of the plot area.

@return The Y coordinate of the pixel
*/
float mapYValueToPixel(float v, float vMin, float vMax, float top, float bottom) {
    if (vMax == vMin) {
        return (top + bottom) * 0.5f;
    }

    // Calculates the relative position
    float t = (v - vMin) / (vMax - vMin); 

    // Maps the pixel to the absolute position
    return bottom - t * (bottom - top); 
}


/*
@brief Draws Y-axis ticks and grid lines on the plot area.

This function was done using Copilot, as it's just basic math and boring drawing. (I'm lazy)

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] font A pointer to the font to use for rendering the tick labels
@param[3] p A pointer to the PlotArea to draw in.
@param[4] yMin The minimum Y value of the range to determine tick positions.
@param[5] yMax The maximum Y value of the range to determine tick positions.

@return void
*/
void drawYTicksAndGrid(SDL_Renderer *renderer, TTF_Font *font, PlotArea *p, float yMin, float yMax) {
    // Gets the inner bounds of the plot area
    float left, right, top, bottom;
    innerBounds(p, &left, &right, &top, &bottom);

    float fractions[5] = {0.f, 0.25f, 0.5f, 0.75f, 1.f}; // Fractions for tick positions
    float tol = 1e-4f; // Tolerance for hiding the tick at Y=0

    for (int i = 0; i < 5; ++i) {
        // Calculates the Y value for the current tick
        float val = yMin + fractions[i] * (yMax - yMin);
        float y = mapYValueToPixel(val, yMin, yMax, top, bottom);

        // Grid line in grey
        SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
        SDL_RenderLine(renderer, left, y, right, y);

        // Decide tick visibility:
        // - hide at Y=0 specifically
        // - hide at arrowhead (top => i==4)
        int showTick = 1;
        if (nearlyEqual(val, 0.0f, tol)) showTick = 0;
        if (i == 4) showTick = 0;

        // Draws the ticks in black if there should be one
        if (showTick) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderLine(renderer, left - TICK_SIZE, y, left, y);
        }

        // Draws the labels for the ticks
        char buf[64];
        // Formats the tick value with 3 decimal places and saves it in a buffer
        snprintf(buf, sizeof(buf), "%.3f", val);
        drawText(renderer, font, buf, left - 46, y - TICK_SIZE + 2); // Numbers are tested for alignment
    }
}


/*
@brief Draws X-axis ticks and grid lines on the plot area, with labels representing minutes.

Basically copied from drawYTicksAndGrid.

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] font A pointer to the font to use for rendering the tick labels.
@param[3] p A pointer to the PlotArea to draw in.
@param[4] sampleCount The number of samples in the dataset, used to determine tick positions and labels.

@return void
*/
void drawXTicksAndGridMinutes(SDL_Renderer *renderer, TTF_Font *font, PlotArea *p, int sampleCount) {
    // Checks if there are any samples
    if (sampleCount <= 0) {
       return;
    }

    // Gets the inner bounds of the plot area
    float left, right, top, bottom;
    innerBounds(p, &left, &right, &top, &bottom);


    float fractions[5] = {0.f, 0.25f, 0.5f, 0.75f, 1.f}; // Fractions for tick positions
    int denom = sampleCount - 1; // Denominator for calculating tick labels (minutes)


    for (int i = 0; i < 5; ++i) {
        
        float x = left + fractions[i] * (right - left);

        // Draws the grid in grey
        SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
        SDL_RenderLine(renderer, x, top, x, bottom);

        // Tick at bottom (hide at arrowhead/right end)
        if (i != 4) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderLine(renderer, x, bottom, x, bottom - TICK_SIZE);
        }
    
        // Calculate the minutes for the tick label
        float minutes = fractions[i] * (float)denom;
        
        // Draws the labels for the ticks
        char buf[64];
        // Formats the tick value with 3 decimal places and saves it in a buffer
        snprintf(buf, sizeof(buf), "%.3f", minutes);
        drawText(renderer, font, buf, x - 10, bottom + TICK_SIZE + 6); // Numbers are tested for alignment
    }
}


/*
@brief Computes the minimum and maximum values in a dataset.
@param[1] data A pointer to the array of float values representing the dataset.
@param[2] count The number of elements in the dataset.
@param[3] outMin A pointer to a float where the minimum value will be stored.
@param[4] outMax A pointer to a float where the maximum value will be stored.

@return void
*/
void computeDataMinMax(float *data, int count, float *outMin, float *outMax) {
    
    // If the dataset is empty or NULL, set default min and max values
    if (!data || count <= 0) { 
        *outMin = 0.f; 
        *outMax = 1.f; 
        return; 
    }

    // Initialize min and max to the first element of the dataset
    float mn = data[0];
    float mx = data[0];

    // Iterate through the dataset to find the minimum and maximum values
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

@param[1] data A pointer to the array of float values representing the dataset.
@param[2] count The number of elements in the dataset.
@param[3] outMin A pointer to a float where the minimum Y value will be stored.
@param[4] outMax A pointer to a float where the maximum Y value will be stored.

@return void
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

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] p A pointer to the PlotArea to draw in
@param[3] data A pointer to the array of float values representing the dataset to graph.
@param[4] count The number of elements in the dataset.
@param[5] yMin The minimum Y value of the range to determine the vertical scaling of the graph.
@param[6] yMax The maximum Y value of the range to determine the vertical scaling of the graph.

@return void
*/
void drawGraph(SDL_Renderer *renderer, PlotArea *p, float *data, int count, float yMin, float yMax) {
    // For safety
    if (!data || count <= 1) {
        return;
    }

    // Gets the inner bounds of the plot area
    float left, right, top, bottom;
    innerBounds(p, &left, &right, &top, &bottom);

    // Calculates the width and height of the plotting area
    float w = right - left;
    float h = bottom - top;
    if (w <= 0 || h <= 0) {
        return;
    }

    // Draws the graph in red
    SDL_SetRenderDrawColor(renderer, 220, 60, 60, 255);

    // Iterates through the dataset and draws lines between consecutive points to form the graph
    for (int i = 0; i < count - 1; ++i) {
        float t0 = (float) i / (float) (count - 1);
        float t1 = (float) (i + 1) / (float) (count - 1);

        float x0 = left + t0 * w;
        float x1 = left + t1 * w;

        float val0 = data[i];
        float val1 = data[i + 1];

        float y0 = mapYValueToPixel(val0, yMin, yMax, top, bottom);
        float y1 = mapYValueToPixel(val1, yMin, yMax, top, bottom);

        // Draws a line between the two points (x0, y0) and (x1, y1)
        SDL_RenderLine(renderer, x0, y0, x1, y1);
    }
}


/*
@brief sets up the layout metrics and positions/sizes of the plot area and buttons based on the current window dimensions.

@param[1] width The current width of the window.
@param[2] height The current height of the window.
@param[3] L A pointer to the LayoutMetrics layout metrics will be stored.
@param[4] plot A pointer to where the PlotArea will be stored.
@param[5] leftBtn A pointer to the left button, where the computed position and size will be stored.
@param[6] rightBtn A pointer to the right button, where the computed position and size will be stored.
@param[7] closeBtn A pointer to the close button, where the computed position and size will be stored.

@return void
*/
void computeLayout(int width, int height, LayoutMetrics *L, PlotArea *plot, Button *leftBtn, Button *rightBtn, Button *closeBtn) {
    L->topBar      = TOP_BAR_H;
    L->leftBtnPad  = EDGE_MARGIN;
    L->rightBtnPad = EDGE_MARGIN;

    
    leftBtn->rect.w = rightBtn->rect.w = SIDE_BTN_SIZE;
    leftBtn->rect.h = rightBtn->rect.h = SIDE_BTN_SIZE;

    float plotTop    = L->topBar + EDGE_MARGIN;
    float plotHeight = height - L->topBar - 2.0f * EDGE_MARGIN;  // "full height" (minus top bar and 10px margins)
    float plotBottom = plotTop + plotHeight;

    leftBtn->rect.x  = L->leftBtnPad;
    rightBtn->rect.x = width - L->rightBtnPad - rightBtn->rect.w;

    float centerY = plotTop + plotHeight * 0.5f;
    leftBtn->rect.y  = centerY - leftBtn->rect.h * 0.5f;
    rightBtn->rect.y = centerY - rightBtn->rect.h * 0.5f;

    float plotLeft  = leftBtn->rect.x + leftBtn->rect.w + SIDE_PAD;
    float plotRight = rightBtn->rect.x - SIDE_PAD;
    float plotWidth = fmaxf(100.0f, plotRight - plotLeft);

    plot->rect.x = plotLeft;
    plot->rect.y = plotTop;
    plot->rect.w = plotWidth;
    plot->rect.h = plotHeight;

    closeBtn->rect.w = closeBtn->rect.h = CLOSE_SIZE;
    closeBtn->rect.x = width  - CLOSE_MARGIN - closeBtn->rect.w;
    closeBtn->rect.y = CLOSE_MARGIN;
}