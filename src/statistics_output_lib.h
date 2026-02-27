#ifndef STATISTICS_OUTPUT_LIB_H
#define STATISTICS_OUTPUT_LIB_H


// Includes all needed Libraries
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// ================== Values for Layout and quick change ==================
// Size of Window
#define WIDTH             1500
#define HEIGHT            1000

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

#define SPEED 10.0f // Speed of animations

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

typedef struct {
    SDL_FRect rect;
    float padding; // inner padding for axes/labels
} PlotArea;

typedef struct {
    float topBar;
    float leftBtnPad;   // window-left padding for left button
    float rightBtnPad;  // window-right padding for right button
} LayoutMetrics;

/*
@brief Rewrites the dataset with new data based of the index and the Filepath.

@param[1] datasetIndex An integer representing the index of the dataset to load.
@param[2] filepath A pointer to a string representing the path to the file containing the new data.

@return A pointer 
*/
float *loadNewDataset(int datasetIndex, char *filepath);


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
@brief Calculates the inner bounds of a PlotArea, 
accounting for padding and saves it in the provided pointers.

@param[1] p A pointer to the PlotArea.
@param[2] left A pointer to a float where the left bound will be stored.
@param[3] right A pointer to a float where the right bound will be stored.
@param[4] top A pointer to a float where the top bound will be stored.
@param[5] bottom A pointer to a float where the bottom bound will be stored.

@return void
*/
void innerBounds(PlotArea *p, float *left, float *right, float *top, float *bottom);


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
@brief Creates a new PlotArea with a white background.

@param[1] x The x-coordinate of the top-left corner of the PlotArea.
@param[2] y The y-coordinate of the top-left corner of the PlotArea.
@param[3] w The width of the PlotArea.
@param[4] h The height of the PlotArea.
@param[5] renderer A pointer to renderer to use for drawing the background.

@return A PlotArea structure representing the created PlotArea.
*/
PlotArea createPlotArea(float x, float y, float w, float h, SDL_Renderer *renderer);


/*
@brief Draws the background of the plot area.

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] p A pointer to the PlotArea to draw the background for.

@return void
*/
void drawPlotBackground(SDL_Renderer *renderer, PlotArea *p);


/*
@brief Draws the axes of the plot area with arrowheads.

This function was made using Copilot, as I didn't want to calculate the geometry.

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] p A pointer to the PlotArea to draw in.

@return void
*/
void drawAxesWithArrows(SDL_Renderer *renderer, PlotArea *p);


/*
@brief Draws the labels for the X and Y axes of the plot area.

@param[1] renderer A pointer to the SDL_Renderer to use for drawing.
@param[2] font A pointer to the TTF_Font to use for rendering the text.
@param[3] p A pointer to the PlotArea structure representing the plot area to draw.
@param[4] labelX A string representing the label for the X axis.
@param[5] labelY A string representing the label for the Y axis.

@return void
*/
void drawAxisLabels(SDL_Renderer *renderer, TTF_Font *font, PlotArea *p, char *labelX, char *labelY);


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
@param[3] p A pointer to the PlotArea to draw in.
@param[4] yMin The minimum Y value of the range to determine tick positions.
@param[5] yMax The maximum Y value of the range to determine tick positions.

@return void
*/
void drawYTicksAndGrid(SDL_Renderer *renderer, TTF_Font *font, PlotArea *p, float yMin, float yMax);

/*
@brief Draws X-axis ticks and grid lines on the plot area, with labels representing minutes.

Basically copied from drawYTicksAndGrid.

@param[1] renderer A pointer to the renderer to use for drawing.
@param[2] font A pointer to the font to use for rendering the tick labels.
@param[3] p A pointer to the PlotArea to draw in.
@param[4] sampleCount The number of samples in the dataset, used to determine tick positions and labels.

@return void
*/
void drawXTicksAndGridMinutes(SDL_Renderer *renderer, TTF_Font *font, PlotArea *p, int sampleCount);


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
@param[2] p A pointer to the PlotArea to draw in
@param[3] data A pointer to the array of float values representing the dataset to graph.
@param[4] count The number of elements in the dataset.
@param[5] yMin The minimum Y value of the range to determine the vertical scaling of the graph.
@param[6] yMax The maximum Y value of the range to determine the vertical scaling of the graph.

@return void
*/
void drawGraph(SDL_Renderer *renderer, PlotArea *p, float *data, int count, float yMin, float yMax);


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
void computeLayout(int width, int height, LayoutMetrics *L, PlotArea *plot,
                          Button *leftBtn, Button *rightBtn, Button *closeBtn);

#endif