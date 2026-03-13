#ifndef STATISTICS_OUTPUT_LIB_H
#define STATISTICS_OUTPUT_LIB_H


// Includes all needed Libraries
#include "../external/SDL/include/SDL3/SDL.h"
#include "../external/SDL_ttf/include/SDL3_ttf/SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "data_types.h"


// ================== Values for Layout and quick change ==================
// Size of Window
#define WIDTH             1200
#define HEIGHT            900

// Font
#define FONT_PATH         "../external/Font/ARIAL.TTF"
#define FONT_SIZE         12

// Layout
#define TOP_BAR_H         44.0f   // reserved space for close button
#define SIDE_BTN_SIZE     50.0f   // size of the side buttons
#define SIDE_PAD          10.0f   // padding between plot and side buttons
#define EDGE_MARGIN       10.0f   // window margin to place side buttons
#define CLOSE_SIZE        28.0f   // size of the close button
#define CLOSE_MARGIN       8.0f   // margin of the close button from the top-right corner
#define PADDING           60.0f   // Padding for the plot area to make space for labels and ticks
#define TICK_SIZE          6.0f   // Length of the tick marks on the axes
#define ARROW_PADDING     20.0f   // Space for arrows

#define SPEED 10.0f // Speed of animations

// Button colors (base and hover, RGB)
#define BTN_BASE_R  166
#define BTN_BASE_G   27
#define BTN_BASE_B  255
#define BTN_HOVER_R 255
#define BTN_HOVER_G 16
#define BTN_HOVER_B 255
#define BTN_GLOW_SIZE 10.0f  // thickness of the glow border around buttons (in pixels)

// Booleans
#define TRUE 1
#define FALSE 0


//================== Structs ==================

// SDL_FRect is a basic rectangle with coordinates and the dimensions

typedef struct {
    SDL_FRect rect; 
    int hovered;
    int pressed;
    float hoverAnim; // 0.0f to 1.0f, used for glow effect
    float pressAnim; // 0.0f to 1.0f, used for scaling effect
} Button;


/*
@brief prints the simulation parameters.

@param[1] sim_nr The number of the simulation.
@param[2] sim_duration The total amount of time steps done.
@param[3] parking_cells The amount of parking_spaces.
@param[4] max_parking_duration The maximum parking time for the cars.
@param[5] new_car_prob The probability of a new car arriving (in percent).
@param[6] max_cars_arriving How many cars can arrive in a simulation step.
@param[7] seed The seed used to generate randomness within this simulation.

@return void
*/
void print_head_data (int sim_nr, t_Time sim_duration, int parking_cells, t_Time max_parking_duration, float new_car_prob, int max_cars_arriving, unsigned int seed);


/*
@brief prints the simulated data per timestep

@param[1] timestep The current timestep.
@param[2] cars_parked The amount of cars currently parked.
@param[3] avg_parking_time The average parking time of the cars.
@param[4] q_len The current length of the queue.
@param[5] full_house_steps The amount of steps the parking garage was full.
@param[6] tot_cars_simulated The total amount of cars that was simulated.
@param[7] most_brand The car brand that parked the most.

@return void
*/
void print_data_per_timestep (t_Time timestep, int cars_parked, float avg_parking_time, int q_len, int full_house_steps, int tot_cars_simulated, Car_Brand most_brand);

/*
@brief gets the length of the Simulation

@param[1] file A pointer to the opened file

@return the simulation length
*/
int get_simulation_length(FILE *file);


/*
@brief Rewrites the dataset with new data based on the index

@param[1] file A pointer to the opened file
@param[2] dataset_index The index of the data to load

@return void
*/
void load_new_dataset(FILE* file, int dataset_index, float* dataset);

/*
@brief returns the name of the brand by its enum code

@param[1] brand_numb The number of the brand

@return the name of the brand
*/
char* get_brand_by_number(int brand_numb);


/* 
@brief A simple linear interpolation function.

@param[1] a The start value.
@param[2] b The end value.
@param[3] t The interpolation factor, between 0.0f and 1.0f.

@return The interpolated value between a and b.
*/
float lerp(float a, float b, float t);


/*
@brief Checks if a point (x, y) is inside a given rectangle.

@param[1] x The x-coordinate of the point.
@param[2] y The y-coordinate of the point.
@param[3] rect A pointer to the rectangle

@return 1 if the point is inside the rectangle, 0 otherwise.
*/
int isPointInsideRect(float x, float y, SDL_FRect * rect);


/*
@brief Calculates the inner bounds of a plot rectangle,
accounting for PADDING and saves it in the provided pointers.

@param[1] plot A pointer to the plot SDL_FRect.
@param[2] left A pointer to a float where the left bound will be stored.
@param[3] right A pointer to a float where the right bound will be stored.
@param[4] top A pointer to a float where the top bound will be stored.
@param[5] bottom A pointer to a float where the bottom bound will be stored.

@return void
*/
void innerBounds(SDL_FRect *plot, float *left, float *right, float *top, float *bottom);


/*
@brief checks if two values are equal within a tolerance

@param[1] a The first value to compare.
@param[2] b The second value to compare.
@param[3] tol The tolerance within which the two values are considered equal.

@return 1 if the values are nearly equal within the specified tolerance, 0 otherwise.
*/
int nearlyEqual(float a, float b, float tol);


/*
@brief Updates the hover state of a button based on the current mouse position.

@param[1] btn A pointer to the button to update.
@param[2] mouseX The current x-coordinate of the mouse.
@param[3] mouseY The current y-coordinate of the mouse.

@return void
*/
void updateButtonHover(Button *btn, float mouseX, float mouseY);


/*
@brief updates the animation state of a button based on the time passed since the last update.

@param[1] btn A pointer to the button to update.
@param[2] dt The time in milliseconds since the last update.

@return void
*/
void updateButtonAnimation(Button *btn, float dt);


/*
@brief Draws a cyan glowing & enlarging effect around a given rectangle.

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] rect A pointer to the rectangle to draw the glow around.
@param[3] intensity A float value between 0.0f and 1.0f representing the intensity of the glow effect.

@return void
*/
void drawGlow(SDL_Renderer *renderer,  SDL_FRect *rect, float intensity);


/*
@brief Draws a button with the renderer

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] btn A pointer to the button to draw.

@return void
*/
void drawButton(SDL_Renderer *renderer, Button *btn);


/*
@brief Draws an arrow icon inside a given rectangle, with a specified direction and hover animation.

This function was made using Copilot, as its geometry is a bit tricky and I didn't want to do it manually.

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] rect A pointer to the rectangle to draw the arrow in.
@param[3] direction An integer indicating the direction of the arrow (-1 for left, 1 for right).
@param[4] hoverAnim A float value between 0.0f and 1.0f representing the hover animation state.

@return void
*/
void drawArrow(SDL_Renderer * renderer, SDL_FRect * rect, int direction, float hoverAnim);


/*
@brief Draws a centered "X" icon inside a given rectangle, with a hover animation effect.

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] rect A pointer to  the rectangle to draw the "X" icon in.
@param[3] hoverAnim A float value between 0.0f and 1.0f representing the hover animation state.

@return void
*/
void drawXIcon(SDL_Renderer *renderer, SDL_FRect *rect, float hoverAnim);


/*
@brief Handles events for navigation buttons.

@param[1] btn A pointer to the button to handle events for.
@param[2] event A pointer to the event to handle.

@return 1 if the button was clicked, 0 otherwise.
*/
int handleNavButtonEvent(Button * btn, SDL_Event * event);


/*
@brief Handles events for the close button.

@param[1] btn A pointer to the close button.
@param[2] event A pointer to the event to handle.
@param[3] running A pointer to the running state of the application, which will be set to 0 if the close button is clicked.

@return void
*/
void handleCloseButtonEvent(Button *btn, SDL_Event *event, int *running);


/*
@brief Opens a TTF font from a specified file path with a specified size.

@param[1] path A string representing the file path to the font to open.
@param[2] size An integer representing the size of the font.

@return A pointer to the opened font, or NULL if the font could not be opened.
*/
TTF_Font * openFont(char *path, int size);


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
void drawText(SDL_Renderer *renderer, TTF_Font *font, char *text, float x, float y);


/*
@brief Draws the background in white

@param[1] renderer A pointer to the renderer to use for drawing.

@return void
*/
void drawBackground(SDL_Renderer *renderer);


/*
@brief Draws the axes of the plot area with arrowheads.

This function was made using Copilot, as I didn't want to calculate the geometry.

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] plot A pointer to the plot SDL_FRect to draw in.

@return void
*/
void drawAxesWithArrows(SDL_Renderer *renderer, SDL_FRect *plot);


/*
@brief Draws the labels for the X and Y axes of the plot area.

@param[1] renderer A pointer to the SDL_Renderer to use for drawing.
@param[2] font A pointer to the TTF_Font to use for rendering the text.
@param[3] plot A pointer to the plot SDL_FRect to draw.
@param[4] labelX A string representing the label for the X axis.
@param[5] labelY A string representing the label for the Y axis.

@return void
*/
void drawAxisLabels(SDL_Renderer *renderer, TTF_Font *font, SDL_FRect *plot, char *labelX, char *labelY);


/*
@brief Maps a Y value to a pixel coordinate within the plot area, based on the specified Y range and plot dimensions.

@param[1] v The Y value to map.
@param[2] vMin The minimum Y value of the range.
@param[3] vMax The maximum Y value of the range.
@param[4] top The top pixel coordinate of the plot area.
@param[5] bottom The bottom pixel coordinate of the plot area.

@return The Y coordinate of the pixel
*/
float mapYValueToPixel(float v, float vMin, float vMax, float top, float bottom);


/*
@brief Draws Y-axis ticks and grid lines on the plot area.

This function was done using Copilot, as it's just basic math and boring drawing. (I'm lazy)

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] font A pointer to the font to use for rendering the tick labels
@param[3] plot A pointer to the plot SDL_FRect to draw in.
@param[4] yMin The minimum Y value of the range to determine tick positions.
@param[5] yMax The maximum Y value of the range to determine tick positions.

@return void
*/
void drawYTicksAndGrid(SDL_Renderer *renderer, TTF_Font *font, SDL_FRect *plot, float yMin, float yMax);

/*
@brief Draws X-axis ticks and grid lines on the plot area, with labels representing minutes.

Basically copied from drawYTicksAndGrid.

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] font A pointer to the font to use for rendering the tick labels.
@param[3] plot A pointer to the plot SDL_FRect to draw in.
@param[4] sampleCount The number of samples in the dataset, used to determine tick positions and labels.

@return void
*/
void drawXTicksAndGridMinutes(SDL_Renderer *renderer, TTF_Font *font, SDL_FRect *plot, int sampleCount);


/*
@brief Computes the minimum and maximum values in a dataset.
@param[1] data A pointer to the array of float values representing the dataset.
@param[2] count The number of elements in the dataset.
@param[3] outMin A pointer to a float where the minimum value will be stored.
@param[4] outMax A pointer to a float where the maximum value will be stored.

@return void
*/
void computeDataMinMax(float *data, int count, float *outMin, float *outMax);


/*
@brief Computes the Y-axis range for a dataset.

@param[1] data A pointer to the array of float values representing the dataset.
@param[2] count The number of elements in the dataset.
@param[3] outMin A pointer to a float where the minimum Y value will be stored.
@param[4] outMax A pointer to a float where the maximum Y value will be stored.

@return void
*/
void computeYRange(float *data, int count, float *outMin, float *outMax);


/*
@brief Draws a graph of the dataset on the plot area.

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] plot A pointer to the plot SDL_FRect to draw in.
@param[3] data A pointer to the array of float values representing the dataset to graph.
@param[4] count The number of elements in the dataset.
@param[5] yMin The minimum Y value of the range to determine the vertical scaling of the graph.
@param[6] yMax The maximum Y value of the range to determine the vertical scaling of the graph.

@return void
*/
void drawGraph(SDL_Renderer *renderer, SDL_FRect *plot, float *data, int count, float yMin, float yMax);


/*
@brief sets up the positions/sizes of the plot area and buttons based on the current window dimensions.

@param[1] width The current width of the window.
@param[2] height The current height of the window.
@param[3] plot A pointer to the SDL_FRect where the plot area will be stored.
@param[4] leftBtn A pointer to the left button, where the computed position and size will be stored.
@param[5] rightBtn A pointer to the right button, where the computed position and size will be stored.
@param[6] closeBtn A pointer to the close button, where the computed position and size will be stored.

@return void
*/
void computeLayout(int width, int height, SDL_FRect *plot,
                          Button *leftBtn, Button *rightBtn, Button *closeBtn);

#endif