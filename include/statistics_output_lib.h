#ifndef STATISTICS_OUTPUT_LIB_H
#define STATISTICS_OUTPUT_LIB_H


// Includes all needed Libraries
#include "../external/SDL/include/SDL3/SDL.h"
#include "../external/SDL_ttf/include/SDL3_ttf/SDL_ttf.h"
#include "file_manager_lib.h"
#include "data_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



// ================== Values for Layout and quick change ==================
// Size of Window
#define WIDTH             1200
#define HEIGHT            900

// Font
#define FONT_PATH         "/workspaces/hamster-parkhouse/external/Font/ARIAL.TTF"
#define FONT_SIZE         15

// Layout
#define TOP_BAR_H         44.0f   // reserved space for close button
#define SIDE_BTN_SIZE     50.0f   // size of the side buttons
#define SIDE_PAD          10.0f   // padding between plot and side buttons
#define EDGE_MARGIN       10.0f   // window margin to place side buttons
#define CLOSE_SIZE        28.0f   // size of the close button
#define CLOSE_MARGIN       8.0f   // margin of the close button from the top-right corner
#define PADDING           65.0f   // Padding for the plot area to make space for labels and ticks
#define TICK_SIZE          8.5f   // Length of the tick marks on the axes
#define ARROW_PADDING     20.0f   // Space for arrows
#define TOP_PADDING       25.0f   // Space fo the top label
#define SIDE_BTN_HEIGHT  440.0f   // The height of the side buttons

#define SPEED 8.0f // Speed of animations

// Button colors (base and hover, RGB)
#define BTN_BASE_R  166
#define BTN_BASE_G   27
#define BTN_BASE_B  255

#define BTN_HOVER_R 255
#define BTN_HOVER_G 16
#define BTN_HOVER_B 255
#define BTN_GLOW_SIZE 10.0f  // thickness of the glow border around buttons (in pixels)

// Graph color
#define GRAPH_COLOR_R 0
#define GRAPH_COLOR_G 0
#define GRAPH_COLOR_B 255

// The number of relevant data in statistics
#define DATASET_TYPE 5


//================== Structs ==================

// SDL_FRect is a basic rectangle with coordinates and the dimensions

typedef struct Button{
    SDL_FRect rect; 
    int hovered;
    int pressed;
    float hoverAnim; // 0.0f to 1.0f, used for glow effect
    float pressAnim; // 0.0f to 1.0f, used for scaling effect
} Button;


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
void print_head_data(int sim_nr, t_Time sim_duration, int parking_cells, t_Time max_parking_duration, 
                        float new_car_prob, int max_cars_arriving, unsigned int seed);


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
void print_data_per_timestep(t_Time timestep, int cars_parked, float avg_parking_time, 
                                int q_len, int full_house_steps, int tot_cars_simulated, Car_Brand most_brand);


/*
@brief Gets the number of simulated steps from the file

@param[1] file A pointer to the opened file

@return The number of simulated steps, or 0 if not found
*/
int get_simulation_length(FILE *file);


/*
@brief Rewrites the dataset array with values from a specific column in the file

@param[1] file A pointer to the opened file
@param[2] dataset_index The index of the dataset to load (0 = Parked Cars, 1 = Avg Time, ...)
@param[3] dataset A pointer to the float array to write the values into
@param[4] size The number of values to read, must match the allocated size of dataset

@return void
*/
void load_new_dataset(FILE* file, int dataset_index, float* dataset, int size);


/*
@brief Returns the name of a car brand by its enum value

@param[1] brand_numb The enum value of the car brand

@return A string containing the name of the car brand
*/
char* get_brand_by_number(Car_Brand brand_numb);

/*
@brief A simple linear interpolation function

@param[1] a The start value
@param[2] b The end value
@param[3] t The interpolation factor, between 0.0f and 1.0f

@return The interpolated value between a and b
*/
float lerp(float a, float b, float t);


/*
@brief Checks if a point (x, y) is inside a given rectangle

@param[1] x The x-coordinate of the point
@param[2] y The y-coordinate of the point
@param[3] rect A pointer to the rectangle

@return 1 if the point is inside the rectangle, 0 otherwise
*/
int is_point_inside_rect(float x, float y, SDL_FRect *rect);


/*
@brief Calculates the inner bounds of a plot rectangle, accounting for PADDING

@param[1] plot A pointer to the plot SDL_FRect
@param[2] left A pointer to a float where the left bound will be stored
@param[3] right A pointer to a float where the right bound will be stored
@param[4] top A pointer to a float where the top bound will be stored
@param[5] bottom A pointer to a float where the bottom bound will be stored

@return void
*/
void inner_bounds(SDL_FRect *plot, float *left, float *right, float *top, float *bottom);


/*
@brief Checks if two float values are equal within a given tolerance

@param[1] a The first value to compare
@param[2] b The second value to compare
@param[3] tol The tolerance within which the two values are considered equal

@return 1 if the values are nearly equal, 0 otherwise
*/
int nearly_equal(float a, float b, float tol);


/*
@brief Updates the hover state of a button based on the current mouse position

@param[1] btn A pointer to the button to update
@param[2] mouse_x The current x-coordinate of the mouse
@param[3] mouse_y The current y-coordinate of the mouse

@return void
*/
void update_button_hover(Button *btn, float mouse_x, float mouse_y);

/*
@brief Updates the animation state of a button based on the time passed since the last update

@param[1] btn A pointer to the button to update
@param[2] dt The time in seconds since the last update

@return void
*/
void update_button_animation(Button *btn, float dt);


/*
@brief Draws a glowing border effect around a given rectangle

@param[1] renderer A pointer to the renderer to use for drawing
@param[2] rect A pointer to the rectangle to draw the glow around
@param[3] intensity A float value between 0.0f and 1.0f representing the intensity of the glow

@return void
*/
void draw_glow(SDL_Renderer *renderer, SDL_FRect *rect, float intensity);


/*
@brief Draws a button with hover and press animations

@param[1] renderer A pointer to the renderer to use for drawing
@param[2] btn A pointer to the button to draw

@return void
*/
void draw_button(SDL_Renderer *renderer, Button *btn);


/*
@brief Draws an arrow icon inside a given rectangle

This function was made using Copilot, as its geometry is a bit tricky and I didn't want to do it manually

@param[1] renderer A pointer to the renderer to use for drawing
@param[2] rect A pointer to the rectangle to draw the arrow in
@param[3] direction An integer indicating the direction of the arrow (-1 for left, 1 for right)
@param[4] hover_anim A float value between 0.0f and 1.0f representing the hover animation state

@return void
*/
void draw_arrow(SDL_Renderer *renderer, SDL_FRect *rect, int direction, float hover_anim);


/*
@brief Draws a centered X icon inside a given rectangle

This function was made using Copilot, as its basic geometry and I wanted to save time

@param[1] renderer A pointer to the renderer to use for drawing
@param[2] rect A pointer to the rectangle to draw the X icon in
@param[3] hover_anim A float value between 0.0f and 1.0f representing the hover animation state

@return void
*/
void draw_x_icon(SDL_Renderer *renderer, SDL_FRect *rect, float hover_anim);


/*
@brief Handles mouse events for a navigation button and returns whether it was clicked

@param[1] btn A pointer to the button to handle events for
@param[2] event A pointer to the SDL event to handle

@return 1 if the button was clicked, 0 otherwise
*/
int handle_nav_button_event(Button *btn, SDL_Event *event);


/*
@brief Handles mouse events for the close button and sets running to FALSE if clicked

@param[1] btn A pointer to the close button
@param[2] event A pointer to the SDL event to handle
@param[3] running A pointer to the running state, set to FALSE if the button is clicked

@return void
*/
void handle_close_button_event(Button *btn, SDL_Event *event, int *running);


/*
@brief Opens a TTF font from a file path at a given size

@param[1] path A string representing the file path to the font
@param[2] size An integer representing the desired font size

@return A pointer to the opened TTF_Font, or NULL if loading failed
*/
TTF_Font *open_font(char *path, int size);


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
void draw_text(SDL_Renderer *renderer, TTF_Font *font, char *text, float x, float y);


/*
@brief Draws a solid white background covering the entire window

@param[1] renderer A pointer to the renderer to use for drawing

@return void
*/
void draw_background(SDL_Renderer *renderer);


/*
@brief Draws the X and Y axes of the plot area with arrowheads

This function was made using Copilot, as I didn't want to calculate the geometry

@param[1] renderer A pointer to the renderer to use for drawing
@param[2] plot A pointer to the plot SDL_FRect

@return void
*/
void draw_axes_with_arrows(SDL_Renderer *renderer, SDL_FRect *plot);


/*
@brief Draws the labels for the X and Y axes of the plot area

@param[1] renderer A pointer to the SDL_Renderer to use for drawing
@param[2] font A pointer to the TTF_Font to use for rendering
@param[3] plot A pointer to the plot SDL_FRect
@param[4] label_x A string representing the label for the X axis
@param[5] label_y A string representing the label for the Y axis

@return void
*/
void draw_axis_labels(SDL_Renderer *renderer, TTF_Font *font, SDL_FRect *plot, char *label_x, char *label_y);


/*
@brief Maps a data value to a pixel Y coordinate within the plot area

@param[1] v The value to map
@param[2] v_min The minimum value of the range
@param[3] v_max The maximum value of the range
@param[4] top The top pixel coordinate of the plot area
@param[5] bottom The bottom pixel coordinate of the plot area

@return The Y pixel coordinate corresponding to v
*/
float map_y_value_to_pixel(float v, float v_min, float v_max, float top, float bottom);


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
void draw_y_ticks_and_grid(SDL_Renderer *renderer, TTF_Font *font, SDL_FRect *plot, float y_min, float y_max);


/*
@brief Draws X-axis tick marks, grid lines and step labels on the plot area

@param[1] renderer A pointer to the renderer to use for drawing
@param[2] font A pointer to the font to use for rendering tick labels
@param[3] plot A pointer to the plot SDL_FRect
@param[4] sample_count The number of data samples, used to determine tick label values

@return void
*/
void draw_x_ticks_and_grid(SDL_Renderer *renderer, TTF_Font *font, SDL_FRect *plot, int sample_count);


/*
@brief Computes the minimum and maximum values in a float array

@param[1] data A pointer to the float array
@param[2] count The number of elements in the array
@param[3] out_min A pointer to a float where the minimum value will be stored
@param[4] out_max A pointer to a float where the maximum value will be stored

@return void
*/
void compute_data_min_max(float *data, int count, float *out_min, float *out_max);


/*
@brief Computes a suitable Y-axis range for a dataset, starting from 0 for non-negative data

@param[1] data A pointer to the float array
@param[2] count The number of elements in the array
@param[3] out_min A pointer to a float where the minimum Y value will be stored
@param[4] out_max A pointer to a float where the maximum Y value will be stored

@return void
*/
void compute_y_range(float *data, int count, float *out_min, float *out_max);


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
void draw_graph(SDL_Renderer *renderer, SDL_FRect *plot, float *data, int count, float y_min, float y_max);


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
                    Button *left_btn, Button *right_btn, Button *close_btn);


/*
@brief Opens the GUI window, loads simulation data from file and runs the main render loop

@param[1] path A string representing the directory path where the simulation file is located
@param[2] sim_number The simulation number used to identify the correct file

@return void
*/
void open_gui(const char* path, int sim_number);

#endif