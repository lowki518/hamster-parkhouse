/*
@brief Library with all the main functions for the statistics output and GUIIIIIIII
*/

#include "../../include/statistics_output_lib.h"
#include "../../include/car_lib.h"
#include "../../include/file_manager_lib.h"
#include "../../include/data_types.h"


/*
@brief Prints the simulation parameters

@param[1] sim_nr The number of the simulation
@param[2] sim_duration The total amount of time steps done
@param[3] parking_cells The amount of parking spaces
@param[4] max_parking_duration The maximum parking time for the cars
@param[5] new_car_prob The probability of a new car arriving (in percent)
@param[6] max_cars_arriving How many cars can arrive in a simulation step
@param[7] seed The seed used to generate randomness within this simulation

@return void
*/
void print_head_data(int sim_nr, t_Time sim_duration, int parking_cells, t_Time max_parking_duration, float new_car_prob, int max_cars_arriving, unsigned int seed) {
    char sim_duration_s[]         = "Simulated Steps";
    char parking_cells_s[]        = "Parking Cells";
    char max_parking_duration_s[] = "Max. Parking Time";
    char new_car_prob_s[]         = "New Car Prob.";
    char max_cars_arriving_s[]    = "Max. New Cars";
    char seed_s[]                 = "Seed";

    char curr_step_s[]            = "Current Step";
    char curr_cars_s[]            = "Parked Cars";
    char avg_time_s[]             = "Avg. Time";
    char q_len_s[]                = "Queue Length";
    char full_house_s[]           = "Full Garage";
    char tot_sim_car_s[]          = "Cars Simulated";
    char most_brand_s[]           = "Most Brand";

    printf("Simulation Nr. %i:\n", sim_nr);
    printf("\n");
    printf("|%15s|%15s|%19s|%15s|%15s|%15s|\n", sim_duration_s, parking_cells_s, max_parking_duration_s, new_car_prob_s, max_cars_arriving_s, seed_s);
    printf("|%15i|%15i|%19i|%14.2f%%|%15i|%15i|\n", sim_duration, parking_cells, max_parking_duration, new_car_prob, max_cars_arriving, seed);
    printf("\n\n");
    printf("|%15s|%15s|%15s|%15s|%15s|%15s|%15s|\n", 
        curr_step_s, curr_cars_s, avg_time_s, q_len_s, full_house_s, tot_sim_car_s, most_brand_s);
    
    
}


/*
@brief Prints the simulated data for a single timestep

@param[1] timestep The current timestep
@param[2] cars_parked The amount of cars currently parked
@param[3] avg_parking_time The average parking time of the cars
@param[4] q_len The current length of the queue
@param[5] full_house_steps The amount of steps the parking garage was full
@param[6] tot_cars_simulated The total amount of cars that was simulated
@param[7] most_brand The car brand that parked the most

@return void
*/
void print_data_per_timestep(t_Time timestep, int cars_parked, float avg_parking_time, int q_len, int full_house_steps, int tot_cars_simulated, Car_Brand most_brand) {
    printf("|%15i|%15i|%15.2f|%15i|%15i|%15i|%15s|\n", timestep, cars_parked, avg_parking_time, q_len, full_house_steps, tot_cars_simulated, get_brand_by_number(most_brand));
}


/*
@brief Gets the number of simulated steps from the file

@param[1] file A pointer to the opened file

@return The number of simulated steps, or 0 if not found
*/
int get_simulation_length(FILE *file) {
    char line[116];
    int simulated_steps = 0;

    rewind(file);

    // search for the header line containing "Simulated Steps"
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Simulated Steps")) {
            // the next line contains the actual values
            if (fgets(line, sizeof(line), file)) {
                // skip leading '|' and spaces to reach the first number
                char *ptr = line;
                while (*ptr == '|' || *ptr == ' ') ptr++;
                sscanf(ptr, "%d", &simulated_steps);
            }
            break;
        }
    }

    return simulated_steps;
}


/*
@brief Rewrites the dataset array with values from a specific column in the file

@param[1] file A pointer to the opened file
@param[2] dataset_index The index of the dataset to load (0 = Parked Cars, 1 = Avg Time, ...)
@param[3] dataset A pointer to the float array to write the values into
@param[4] size The number of values to read, must match the allocated size of dataset

@return void
*/
void load_new_dataset(FILE* file, int dataset_index, float* dataset, int size) {
    char line[116];
    rewind(file);

    // offset by 2 to skip "Current Step" column and account for leading '|'
    int col_index = dataset_index + 2;

    // skip lines until the data header "Current Step" is found
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Current Step")) {
            break;
        }
    }

    int count = 0;

    while (count < size && fgets(line, sizeof(line), file)) {

        // skip empty lines
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') {
            continue;
        } 

        char tmp[116];
        strncpy(tmp, line, sizeof(tmp));
        tmp[sizeof(tmp) - 1] = '\0';

        char *token;
        char *rest = tmp;
        int col = 0;
        float val = 0.0f;
        int found = 0;

        while ((token = strtok_r(rest, "|", &rest))) {
            // skip empty tokens caused by leading '|'
            if (token[0] == '\0' || token[0] == '\n' || token[0] == '\r') {
                continue;
            }

            col++;

            if (col == col_index) {
                // skip leading spaces before parsing the value
                while (*token == ' ') token++;
                if (sscanf(token, "%f", &val) == 1) {
                    found = 1;
                }
                break;
            }
        }

        if (found) {
            dataset[count] = val;
            count++;
        }
    }
}


/*
@brief A simple linear interpolation function

@param[1] a The start value
@param[2] b The end value
@param[3] t The interpolation factor, between 0.0f and 1.0f

@return The interpolated value between a and b
*/
float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}


/*
@brief Checks if a point (x, y) is inside a given rectangle

@param[1] x The x-coordinate of the point
@param[2] y The y-coordinate of the point
@param[3] rect A pointer to the rectangle

@return 1 if the point is inside the rectangle, 0 otherwise
*/
int is_point_inside_rect(float x, float y, SDL_FRect *rect) {
    return (x >= rect->x && x <= rect->x + rect->w &&
            y >= rect->y && y <= rect->y + rect->h);
}


/*
@brief Calculates the inner bounds of a plot rectangle, accounting for PADDING

@param[1] plot A pointer to the plot SDL_FRect
@param[2] left A pointer to a float where the left bound will be stored
@param[3] right A pointer to a float where the right bound will be stored
@param[4] top A pointer to a float where the top bound will be stored
@param[5] bottom A pointer to a float where the bottom bound will be stored

@return void
*/
void inner_bounds(SDL_FRect *plot, float *left, float *right, float *top, float *bottom) {
    *left   = plot->x + PADDING;
    *bottom = plot->y + plot->h - PADDING;
    *right  = plot->x + plot->w - ARROW_PADDING;  // keep space for arrow
    *top    = plot->y - ARROW_PADDING;            // keep space for arrow
}


/*
@brief Checks if two float values are equal within a given tolerance

@param[1] a The first value to compare
@param[2] b The second value to compare
@param[3] tol The tolerance within which the two values are considered equal

@return 1 if the values are nearly equal, 0 otherwise
*/
int nearly_equal(float a, float b, float tol) {
    return fabsf(a - b) <= tol;
}


/*
@brief Updates the hover state of a button based on the current mouse position

@param[1] btn A pointer to the button to update
@param[2] mouse_x The current x-coordinate of the mouse
@param[3] mouse_y The current y-coordinate of the mouse

@return void
*/
void update_button_hover(Button *btn, float mouse_x, float mouse_y) {
    btn->hovered = is_point_inside_rect(mouse_x, mouse_y, &btn->rect);
}


/*
@brief Updates the animation state of a button based on the time passed since the last update

@param[1] btn A pointer to the button to update
@param[2] dt The time in seconds since the last update

@return void
*/
void update_button_animation(Button *btn, float dt) {
    // smoothly interpolate hoverAnim toward 1 when hovered, toward 0 otherwise
    if (btn->hovered) {
        btn->hoverAnim += (1.0f - btn->hoverAnim) * SPEED * dt;
    } else {
        btn->hoverAnim += (0.0f - btn->hoverAnim) * SPEED * dt;
    }

    // smoothly interpolate pressAnim toward 1 when pressed, toward 0 otherwise
    if (btn->pressed) {
        btn->pressAnim += (1.0f - btn->pressAnim) * SPEED * dt;
    } else {
        btn->pressAnim += (0.0f - btn->pressAnim) * SPEED * dt;
    }
}


/*
@brief Draws a glowing border effect around a given rectangle

@param[1] renderer A pointer to the renderer to use for drawing
@param[2] rect A pointer to the rectangle to draw the glow around
@param[3] intensity A float value between 0.0f and 1.0f representing the intensity of the glow

@return void
*/
void draw_glow(SDL_Renderer *renderer, SDL_FRect *rect, float intensity) {
    // scale alpha by intensity so the glow fades in with hover animation
    Uint8 alpha = (Uint8)(255.0f * intensity);
    SDL_SetRenderDrawColor(renderer, BTN_HOVER_R, BTN_HOVER_G, BTN_HOVER_B, alpha);
    SDL_FRect border = {
        rect->x - BTN_GLOW_SIZE,
        rect->y - BTN_GLOW_SIZE,
        rect->w + BTN_GLOW_SIZE * 2,
        rect->h + BTN_GLOW_SIZE * 2
    };
    SDL_RenderFillRect(renderer, &border);
}


/*
@brief Draws a button with hover and press animations

@param[1] renderer A pointer to the renderer to use for drawing
@param[2] btn A pointer to the button to draw

@return void
*/
void draw_button(SDL_Renderer *renderer, Button *btn) {
    draw_glow(renderer, &btn->rect, btn->hoverAnim);

    SDL_Color base  = {BTN_BASE_R,  BTN_BASE_G,  BTN_BASE_B,  255};
    SDL_Color hover = {BTN_HOVER_R, BTN_HOVER_G, BTN_HOVER_B, 255};

    // interpolate color between base and hover based on hover animation state
    Uint8 r = (Uint8)lerp(base.r, hover.r, btn->hoverAnim);
    Uint8 g = (Uint8)lerp(base.g, hover.g, btn->hoverAnim);
    Uint8 b = (Uint8)lerp(base.b, hover.b, btn->hoverAnim);
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    // shrink the button slightly when pressed
    float press_scale = 1.0f - 0.1f * btn->pressAnim;
    float w = btn->rect.w * press_scale;
    float h = btn->rect.h * press_scale;
    float x = btn->rect.x + (btn->rect.w - w) / 2.0f;
    float y = btn->rect.y + (btn->rect.h - h) / 2.0f;

    SDL_FRect rct = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rct);
}


/*
@brief Draws an arrow icon inside a given rectangle

This function was made using Copilot, as its geometry is a bit tricky and I didn't want to do it manually

@param[1] renderer A pointer to the renderer to use for drawing
@param[2] rect A pointer to the rectangle to draw the arrow in
@param[3] direction An integer indicating the direction of the arrow (-1 for left, 1 for right)
@param[4] hover_anim A float value between 0.0f and 1.0f representing the hover animation state

@return void
*/
void draw_arrow(SDL_Renderer *renderer, SDL_FRect *rect, int direction, float hover_anim) {
    int cx   = (int)(rect->x + rect->w / 2);
    int cy   = (int)(rect->y + rect->h / 2);
    int size = (int)(rect->h * 0.33f + 3 * hover_anim);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // black
    if (direction == -1) {
        SDL_RenderLine(renderer, cx + size, cy - size, cx - size, cy);
        SDL_RenderLine(renderer, cx - size, cy,        cx + size, cy + size);
    } else {
        SDL_RenderLine(renderer, cx - size, cy - size, cx + size, cy);
        SDL_RenderLine(renderer, cx + size, cy,        cx - size, cy + size);
    }
}


/*
@brief Draws a centered X icon inside a given rectangle

This function was made using Copilot, as its basic geometry and I wanted to save time

@param[1] renderer A pointer to the renderer to use for drawing
@param[2] rect A pointer to the rectangle to draw the X icon in
@param[3] hover_anim A float value between 0.0f and 1.0f representing the hover animation state

@return void
*/
void draw_x_icon(SDL_Renderer *renderer, SDL_FRect *rect, float hover_anim) {
    hover_anim; // icon is always fully visible, hover_anim kept for API compatibility

    float cx  = rect->x + rect->w * 0.5f;
    float cy  = rect->y + rect->h * 0.5f;
    float arm = fminf(rect->w, rect->h) * 0.3f;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //black
    SDL_RenderLine(renderer, cx - arm, cy - arm, cx + arm, cy + arm);
    SDL_RenderLine(renderer, cx - arm, cy + arm, cx + arm, cy - arm);
}


/*
@brief Handles mouse events for a navigation button and returns whether it was clicked

@param[1] btn A pointer to the button to handle events for
@param[2] event A pointer to the SDL event to handle

@return 1 if the button was clicked, 0 otherwise
*/
int handle_nav_button_event(Button *btn, SDL_Event *event) {
    int clicked = 0;
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (btn->hovered && !btn->pressed) {
            btn->pressed = TRUE;
        }
    } else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        // only count as clicked if mouse was released while still hovering
        if (btn->pressed && btn->hovered) {
            clicked = 1;
        }
        btn->pressed = FALSE;
    }
    return clicked;
}


/*
@brief Handles mouse events for the close button and sets running to FALSE if clicked

@param[1] btn A pointer to the close button
@param[2] event A pointer to the SDL event to handle
@param[3] running A pointer to the running state, set to FALSE if the button is clicked

@return void
*/
void handle_close_button_event(Button *btn, SDL_Event *event, int *running) {
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        if (btn->hovered && !btn->pressed) {
            btn->pressed = TRUE;
        }
    }
    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        if (btn->pressed && btn->hovered) {
            *running = FALSE;
            btn->pressed = FALSE;
        }
    }
}


/*
@brief Opens a TTF font from a file path at a given size

@param[1] path A string representing the file path to the font
@param[2] size An integer representing the desired font size

@return A pointer to the opened TTF_Font, or NULL if loading failed
*/
TTF_Font *open_font(char *path, int size) {
    if (TTF_Init() < 0) {
        printf("TTF init failed: %s", SDL_GetError());
        return NULL;
    }

    TTF_Font *font = TTF_OpenFont(path, size);
    if (!font) {
        printf("Failed to load font: %s", SDL_GetError());
        return NULL;
    }
    return font;
}


/*
@brief Draws a string of text at a given position on the screen

This function was done using Copilot, as I had no idea how to use text with SDL

@param[1] renderer A pointer to the renderer to use for drawing
@param[2] font A pointer to the font to use for rendering
@param[3] text A string representing the text to draw
@param[4] x The x-coordinate where the text should be drawn
@param[5] y The y-coordinate where the text should be drawn

@return void
*/
void draw_text(SDL_Renderer *renderer, TTF_Font *font, char *text, float x, float y) {
    SDL_Color color   = {0, 0, 0, 255};
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
@brief Draws a solid white background covering the entire window

@param[1] renderer A pointer to the renderer to use for drawing

@return void
*/
void draw_background(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
    SDL_FRect rect = {0, 0, WIDTH, HEIGHT};
    SDL_RenderFillRect(renderer, &rect);
}


/*
@brief Draws the X and Y axes of the plot area with arrowheads

This function was made using Copilot, as I didn't want to calculate the geometry

@param[1] renderer A pointer to the renderer to use for drawing
@param[2] plot A pointer to the plot SDL_FRect

@return void
*/
void draw_axes_with_arrows(SDL_Renderer *renderer, SDL_FRect *plot) {
    float left, right, top, bottom;
    inner_bounds(plot, &left, &right, &top, &bottom);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
    SDL_RenderLine(renderer, left, bottom, right, bottom); // X axis
    SDL_RenderLine(renderer, left, bottom, left,  top);    // Y axis

    int size = 8;
    // X arrowhead
    SDL_RenderLine(renderer, right, bottom, right - size, bottom - size);
    SDL_RenderLine(renderer, right, bottom, right - size, bottom + size);
    // Y arrowhead
    SDL_RenderLine(renderer, left, top, left - size, top + size);
    SDL_RenderLine(renderer, left, top, left + size, top + size);
}


/*
@brief Draws the labels for the X and Y axes of the plot area

@param[1] renderer A pointer to the SDL_Renderer to use for drawing
@param[2] font A pointer to the TTF_Font to use for rendering
@param[3] plot A pointer to the plot SDL_FRect
@param[4] label_x A string representing the label for the X axis
@param[5] label_y A string representing the label for the Y axis

@return void
*/
void draw_axis_labels(SDL_Renderer *renderer, TTF_Font *font, SDL_FRect *plot, char *label_x, char *label_y) {
    float left, right, top, bottom;
    inner_bounds(plot, &left, &right, &top, &bottom);

    draw_text(renderer, font, label_x, right + 8, bottom - 10);

    // center the Y label above the plot, approximate half-width using char count * 4
    float plot_center_x = left + (right - left) / 2.0f;
    draw_text(renderer, font, label_y, plot_center_x - (strlen(label_y) * 4.0f), top - TOP_PADDING);
}


/*
@brief Maps a data value to a pixel Y coordinate within the plot area

@param[1] v The value to map
@param[2] v_min The minimum value of the range
@param[3] v_max The maximum value of the range
@param[4] top The top pixel coordinate of the plot area
@param[5] bottom The bottom pixel coordinate of the plot area

@return The Y pixel coordinate corresponding to v
*/
float map_y_value_to_pixel(float v, float v_min, float v_max, float top, float bottom) {
    if (v_max == v_min) {
        return (top + bottom) * 0.5f;
    }
    float t = (v - v_min) / (v_max - v_min);
    return bottom - t * (bottom - top);
}


/*
@brief Draws Y-axis tick marks, grid lines and value labels on the plot area

This function was done using Copilot, as it's just basic math and boring drawing

@param[1] renderer A pointer to the renderer to use for drawing
@param[2] font A pointer to the font to use for rendering tick labels
@param[3] plot A pointer to the plot SDL_FRect
@param[4] y_min The minimum Y value of the data range
@param[5] y_max The maximum Y value of the data range

@return void
*/
void draw_y_ticks_and_grid(SDL_Renderer *renderer, TTF_Font *font, SDL_FRect *plot, float y_min, float y_max) {
    float left, right, top, bottom;
    inner_bounds(plot, &left, &right, &top, &bottom);

    float fractions[5] = {0.f, 0.25f, 0.5f, 0.75f, 1.0f}; // the interval of the grid lines
    float tol = 1e-4f;

    // fixed right anchor so all Y labels are right-aligned to the same x position
    float label_right_anchor = left - 8;

    for (int i = 0; i < 5; ++i) {
        float val = y_min + fractions[i] * (y_max - y_min);
        float y   = map_y_value_to_pixel(val, y_min, y_max, top, bottom);

        // draw horizontal grid line
        SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
        SDL_RenderLine(renderer, left, y, right, y);

        // skip tick at zero and at the top edge
        int show_tick = 1;
        if (nearly_equal(val, 0.0f, tol)) {
            show_tick = 0;
        }

        if (i == 4) {
            show_tick = 0;
        }

        if (show_tick) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderLine(renderer, left - TICK_SIZE, y, left, y);
        }

        char buf[64];
        snprintf(buf, sizeof(buf), "%.2f", val);

        // right-align label by subtracting approximate text width from anchor
        float text_width = strlen(buf) * 7.0f;
        draw_text(renderer, font, buf, label_right_anchor - text_width - SIDE_PAD, y - TICK_SIZE + 2);
    }
}


/*
@brief Draws X-axis tick marks, grid lines and step labels on the plot area

@param[1] renderer A pointer to the renderer to use for drawing
@param[2] font A pointer to the font to use for rendering tick labels
@param[3] plot A pointer to the plot SDL_FRect
@param[4] sample_count The number of data samples, used to determine tick label values

@return void
*/
void draw_x_ticks_and_grid(SDL_Renderer *renderer, TTF_Font *font, SDL_FRect *plot, int sample_count) {
    if (sample_count <= 0) {
        return;
    }

    float left, right, top, bottom;
    inner_bounds(plot, &left, &right, &top, &bottom);

    float fractions[5] = {0.f, 0.25f, 0.5f, 0.75f, 1.0f}; // the interval of the grid lines
    int denom = sample_count;

    for (int i = 0; i < 5; ++i) {
        float x = left + fractions[i] * (right - left);

        // draw vertical grid line
        SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
        SDL_RenderLine(renderer, x, top, x, bottom);

        // skip tick at the rightmost edge
        if (i != 4) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderLine(renderer, x, bottom, x, bottom - TICK_SIZE);
        }

        char buf[64];
        snprintf(buf, sizeof(buf), "%.2f", fractions[i] * (float)denom);
        draw_text(renderer, font, buf, x - 10, bottom + TICK_SIZE + 6);
    }
}


/*
@brief Computes a suitable Y-axis range for a dataset, starting from 0 for non-negative data

@param[1] data A pointer to the float array
@param[2] count The number of elements in the array
@param[3] out_min A pointer to a float where the minimum Y value will be stored
@param[4] out_max A pointer to a float where the maximum Y value will be stored

@return void
*/
void compute_y_range(float *data, int count, float *out_min, float *out_max) {
    if (!data || count <= 0) {
        *out_min = 0.0f;
        *out_max = 1.0f;
        return;
    }

    // find min and max directly here
    float mn = data[0];
    float mx = data[0];
    for (int i = 1; i < count; ++i) {
        if      (data[i] < mn) mn = data[i];
        else if (data[i] > mx) mx = data[i];
    }

    if (mn >= 0.0f) {
        *out_min = 0.0f;
        *out_max = (mx == 0.0f) ? 1.0f : mx;
    } else {
        if (mx == mn) mx = mn + 1.0f;
        float pad = 0.05f * (mx - mn);
        *out_min = mn - pad;
        *out_max = mx + pad;
    }
}


/*
@brief Draws a line graph of a float dataset onto the plot area

@param[1] renderer A pointer to the renderer to use for drawing
@param[2] plot A pointer to the plot SDL_FRect
@param[3] data A pointer to the float array containing the dataset
@param[4] count The number of elements in the dataset
@param[5] y_min The minimum Y value used for scaling
@param[6] y_max The maximum Y value used for scaling

@return void
*/
void draw_graph(SDL_Renderer *renderer, SDL_FRect *plot, float *data, int count, float y_min, float y_max) {
    if (!data || count <= 1) {
        return;
    }

    float left, right, top, bottom;
    inner_bounds(plot, &left, &right, &top, &bottom);

    float w = right - left;
    float h = bottom - top;
    if (w <= 0 || h <= 0) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, GRAPH_COLOR_R, GRAPH_COLOR_G, GRAPH_COLOR_B, 255);

    // connect each consecutive pair of data points with a line segment
    for (int i = 0; i < count - 1; ++i) {
        float t0 = (float) i      / (float)(count - 1);
        float t1 = (float)(i + 1) / (float)(count - 1);

        float x0 = left + t0 * w;
        float x1 = left + t1 * w;
        float y0 = map_y_value_to_pixel(data[i],     y_min, y_max, top, bottom);
        float y1 = map_y_value_to_pixel(data[i + 1], y_min, y_max, top, bottom);

        SDL_RenderLine(renderer, x0, y0, x1, y1);
    }
}


/*
@brief Computes the positions and sizes of the plot area and all buttons

@param[1] width The current width of the window
@param[2] height The current height of the window
@param[3] plot A pointer to the SDL_FRect where the plot area dimensions will be stored
@param[4] left_btn A pointer to the left navigation button
@param[5] right_btn A pointer to the right navigation button
@param[6] close_btn A pointer to the close button

@return void
*/
void compute_layout(int width, int height, SDL_FRect *plot,
                    Button *left_btn, Button *right_btn, Button *close_btn) {
    left_btn->rect.w = right_btn->rect.w = SIDE_BTN_SIZE;
    left_btn->rect.h = right_btn->rect.h = SIDE_BTN_SIZE;

    float plot_top    = TOP_BAR_H + EDGE_MARGIN;
    float plot_height = height - TOP_BAR_H - 2.0f * EDGE_MARGIN;

    left_btn->rect.x  = EDGE_MARGIN;
    right_btn->rect.x = width - EDGE_MARGIN - right_btn->rect.w;

    // vertically center the nav buttons within the plot area
    left_btn->rect.y  = HEIGHT - SIDE_BTN_HEIGHT;
    right_btn->rect.y = HEIGHT - SIDE_BTN_HEIGHT;

    float plot_left  = left_btn->rect.x  + left_btn->rect.w  + SIDE_PAD;
    float plot_right = right_btn->rect.x - SIDE_PAD;

    plot->x = plot_left;
    plot->y = plot_top;
    plot->w = fmaxf(100.0f, plot_right - plot_left);
    plot->h = plot_height;

    close_btn->rect.w = close_btn->rect.h = CLOSE_SIZE;
    close_btn->rect.x = width  - CLOSE_MARGIN - close_btn->rect.w;
    close_btn->rect.y = CLOSE_MARGIN;
}


/*
@brief Opens the GUI window, loads simulation data from file and runs the main render loop

@param[1] path A string representing the directory path where the simulation file is located
@param[2] sim_number The simulation number used to identify the correct file

@return void
*/
void open_gui(const char* path, int sim_number) {

    FILE* data_file = open_file_r(path, sim_number);

    // load initial dataset
    int current_dataset = 0;
    int size            = get_simulation_length(data_file);
    float *dataset      = calloc(size, sizeof(float));
    load_new_dataset(data_file, current_dataset, dataset, size);

    char dataset_name[DATASET_TYPE][32] = {
        "Parked Cars", "Avg. Time", "Queue Length", "Full Garage", "Cars Simulated"
    };

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL init failed: %s", SDL_GetError());
        return;
    }

    SDL_Window   *window   = SDL_CreateWindow("Hamster-Parkhouse-Output", WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    SDL_FRect     plot     = {0};

    TTF_Font *font = open_font(FONT_PATH, FONT_SIZE);

    if (!font) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    Button left_btn  = {{0,0,0,0}, FALSE, FALSE, 0.0f, 0.0f};
    Button right_btn = {{0,0,0,0}, FALSE, FALSE, 0.0f, 0.0f};
    Button close_btn = {{0,0,0,0}, FALSE, FALSE, 0.0f, 0.0f};

    float y_min = 0.0f, y_max = 1.0f;

    int running = TRUE;
    SDL_Event event;
    int last = 0;

    while (running) {
        int now = SDL_GetTicks();
        float dt = 0.016f; // fallback delta time assuming 60 FPS

        if (last != 0) {
            dt = (float)(now - last) / 1000.0f;
        }
        last = now;

        compute_layout(WIDTH, HEIGHT, &plot, &left_btn, &right_btn, &close_btn);

        float mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        update_button_hover(&left_btn,  mouse_x, mouse_y);
        update_button_hover(&right_btn, mouse_x, mouse_y);
        update_button_hover(&close_btn, mouse_x, mouse_y);

        int left_clicked = 0, right_clicked = 0;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = FALSE;
            }

            // accumulate click state across multiple events in one frame
            left_clicked  |= handle_nav_button_event(&left_btn,  &event);
            right_clicked |= handle_nav_button_event(&right_btn, &event);

            if (left_clicked) {
                current_dataset--;
                if (current_dataset < 0) {
                    current_dataset = 4;
                }
                load_new_dataset(data_file, current_dataset, dataset, size);
            }

            if (right_clicked) {
                current_dataset++;
                if (current_dataset > 4) {
                    current_dataset = 0;
                }
                load_new_dataset(data_file, current_dataset, dataset, size);
            }

            handle_close_button_event(&close_btn, &event, &running);

            if (event.type == SDL_EVENT_KEY_UP && event.key.key == SDLK_ESCAPE) {
                running = FALSE;
            }
        }

        update_button_animation(&left_btn,  dt);
        update_button_animation(&right_btn, dt);
        update_button_animation(&close_btn, dt);

        compute_y_range(dataset, size, &y_min, &y_max);

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        draw_background(renderer);

        draw_y_ticks_and_grid(renderer, font, &plot, y_min, y_max);
        draw_x_ticks_and_grid(renderer, font, &plot, size);

        draw_axes_with_arrows(renderer, &plot);
        draw_axis_labels(renderer, font, &plot, "Minutes", dataset_name[current_dataset]);

        draw_graph(renderer, &plot, dataset, size, y_min, y_max);

        draw_button(renderer, &left_btn);
        draw_button(renderer, &right_btn);
        draw_arrow(renderer, &left_btn.rect,  -1, left_btn.hoverAnim);
        draw_arrow(renderer, &right_btn.rect,  1, right_btn.hoverAnim);

        draw_button(renderer, &close_btn);
        draw_x_icon(renderer, &close_btn.rect, close_btn.hoverAnim);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    free(dataset);
    fclose(data_file);

    SDL_Quit();
}