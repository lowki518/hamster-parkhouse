#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/statistics_output_lib.h"



#define TMP_DIR  "/workspaces/hamster-parkhouse/test/tmp"
#define TMP_DATA "/workspaces/hamster-parkhouse/test/tmp/test_file.txt"
#define TEST_TOL 0.0001


void test_nearly_equal() {
    // values within tolerance
    assert(nearly_equal(1.0f, 1.00005f, TEST_TOL) == 1);

    // values equal
    assert(nearly_equal(5.0f, 5.0f, TEST_TOL) == 1);

    // values outside tolerance
    assert(nearly_equal(1.0f, 2.0f, TEST_TOL) == 0);

    printf("test_nearly_equal passed\n");
}


void test_lerp() {
    // midpoint between 0 and 10 should be 5
    assert(nearly_equal(lerp(0.0f, 10.0f, 0.5f), 5.0f, TEST_TOL));

    // t=0 should return a     a    b     t
    assert(nearly_equal(lerp(3.0f, 9.0f, 0.0f), 3.0f, TEST_TOL));

    // t=1 should return b    a     b     t
    assert(nearly_equal(lerp(3.0f, 9.0f, 1.0f), 9.0f, TEST_TOL));

    // negative values
    assert(nearly_equal(lerp(-10.0f, 10.0f, 0.5f), 0.0f, TEST_TOL));

    // negative values with positive output
    assert(nearly_equal(lerp(-10.0f, 10.0f, 0.75f), 5.0f, TEST_TOL));

    // negative values with negative output
    assert(nearly_equal(lerp(-10.0f, 10.0f, 0.25f), -5.0f, TEST_TOL));

    printf("test_lerp passed\n");
}


void test_is_point_inside_rect() {
    //                  x      y    width   height
    SDL_FRect rect = {10.0f, 10.0f, 100.0f, 50.0f};

    // point clearly inside
    assert(is_point_inside_rect(50.0f, 30.0f, &rect) == 1);

    // point on the left edge
    assert(is_point_inside_rect(10.0f, 30.0f, &rect) == 1);

    // point on the right edge
    assert(is_point_inside_rect(110.0f, 30.0f, &rect) == 1);

    // point on the top edge
    assert(is_point_inside_rect(50.0f, 60.0f, &rect) == 1);

    // point on the bottom edge
    assert(is_point_inside_rect(50.0f, 10.0f, &rect) == 1);

    // point clearly outside (left)
    assert(is_point_inside_rect(0.0f, 30.0f, &rect) == 0);

    // point clearly outside (right)
    assert(is_point_inside_rect(200.0f, 30.0f, &rect) == 0);

    // point clearly outside (top)
    assert(is_point_inside_rect(50.0f, 5.0f, &rect) == 0);

    // point clearly outside (bottom)
    assert(is_point_inside_rect(50.0f, 100.0f, &rect) == 0);

    printf("test_is_point_inside_rect passed\n");
}


void test_map_y_value_to_pixel() {
    float top    = 0.0f;
    float bottom = 100.0f;

    // minimum value should map to bottom
    assert(nearly_equal(map_y_value_to_pixel(0.0f,   0.0f, 100.0f, top, bottom), 100.0f, TEST_TOL));

    // maximum value should map to top
    assert(nearly_equal(map_y_value_to_pixel(100.0f, 0.0f, 100.0f, top, bottom), 0.0f,   TEST_TOL));

    // midpoint value should map to the middle
    assert(nearly_equal(map_y_value_to_pixel(50.0f,  0.0f, 100.0f, top, bottom), 50.0f,  TEST_TOL));

    // when v_min == v_max, should return the midpoint between top and bottom
    assert(nearly_equal(map_y_value_to_pixel(5.0f,   5.0f,   5.0f, top, bottom), 50.0f,  TEST_TOL));

    printf("test_map_y_value_to_pixel passed\n");
}


void test_compute_y_range() {
    float out_min, out_max;

    // non-negative data: min should be 0, max should be the largest value
    float pos[] = {1.0f, 5.0f, 3.0f};
    compute_y_range(pos, 3, &out_min, &out_max);
    assert(nearly_equal(out_min, 0.0f, TEST_TOL));
    assert(nearly_equal(out_max, 5.0f, TEST_TOL));

    // all zeros: max should fall back to 1
    float zeros[] = {0.0f, 0.0f, 0.0f};
    compute_y_range(zeros, 3, &out_min, &out_max);
    assert(nearly_equal(out_min, 0.0f, TEST_TOL));
    assert(nearly_equal(out_max, 1.0f, TEST_TOL));

    // negative data: should add 5% padding
    float neg[] = {-10.0f, -2.0f};
    compute_y_range(neg, 2, &out_min, &out_max);
    float expected_pad = 0.05f * 8.0f; // (max - min) = 8
    assert(nearly_equal(out_min, -10.0f - expected_pad, TEST_TOL));
    assert(nearly_equal(out_max,  -2.0f + expected_pad, TEST_TOL));

    printf("test_compute_y_range passed\n");
}


void test_get_simulation_length() {
    FILE *file = fopen(TMP_DATA, "w");
    assert(file != NULL);
    fprintf(file, "Simulation Nr. 1:\n\n");
    fprintf(file, "|Simulated Steps|  Parking Cells|\n");
    fprintf(file, "|             42|            100|\n");
    fclose(file);

    file = fopen(TMP_DATA, "r");
    assert(file != NULL);

    int length = get_simulation_length(file);
    assert(length == 42);

    fclose(file);
    remove(TMP_DATA); // delete temporary test file
    printf("test_get_simulation_length passed\n");
}


void test_load_new_dataset() {
    FILE *file = fopen(TMP_DATA, "w");
    assert(file != NULL);
    fprintf(file, "Simulation Nr. 1:\n\n");
    fprintf(file, "|Simulated Steps|\n");
    fprintf(file, "|              3|\n\n\n");
    fprintf(file, "|   Current Step|  Parked Cars|    Avg. Time|\n");
    fprintf(file, "|              0|            5|         1.50|\n");
    fprintf(file, "|              1|           10|         2.50|\n");
    fprintf(file, "|              2|           15|         3.50|\n");
    fclose(file);

    file = fopen(TMP_DATA, "r");
    assert(file != NULL);

    float dataset[] = {0,0,0};

    // dataset_index 0 = Parked Cars (col 2 after offset)
    load_new_dataset(file, 0, dataset, 3);
    assert(nearly_equal(dataset[0],  5.0f, TEST_TOL));
    assert(nearly_equal(dataset[1], 10.0f, TEST_TOL));
    assert(nearly_equal(dataset[2], 15.0f, TEST_TOL));

    // dataset_index 1 = Avg. Time (col 3 after offset)
    load_new_dataset(file, 1, dataset, 3);
    assert(nearly_equal(dataset[0], 1.50f, TEST_TOL));
    assert(nearly_equal(dataset[1], 2.50f, TEST_TOL));
    assert(nearly_equal(dataset[2], 3.50f, TEST_TOL));

    fclose(file);
    remove(TMP_DATA); // delete temporary test file
    printf("test_load_new_dataset passed\n");
}


void test_inner_bounds() {
    SDL_FRect plot = {100.0f, 50.0f, 800.0f, 600.0f};
    float left, right, top, bottom;

    inner_bounds(&plot, &left, &right, &top, &bottom);

    assert(nearly_equal(left, plot.x + PADDING, TEST_TOL));
    assert(nearly_equal(bottom, plot.y + plot.h - PADDING, TEST_TOL));
    assert(nearly_equal(right, plot.x + plot.w - ARROW_PADDING, TEST_TOL));
    assert(nearly_equal(top, plot.y - ARROW_PADDING, TEST_TOL));

    printf("test_inner_bounds passed\n");
}


int main() {
    printf("Running gui tests...\n\n");

    // create temporary directory for test files
    mkdir(TMP_DIR, 0777); //0777 is for the rights to create a folder and more

    test_nearly_equal();
    test_lerp();
    test_is_point_inside_rect();
    test_map_y_value_to_pixel();
    test_compute_y_range();
    test_get_simulation_length();
    test_load_new_dataset();
    test_inner_bounds();

    // remove temporary directory after all tests
    rmdir(TMP_DIR);

    printf("\ngui tests passed\n");
    return 0;
}