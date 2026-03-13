#include "../include/cli.h"
#include "../include/config.h"
#include <assert.h>

/*
@brief tests handle_user_input function
*/
void test_handle_user_input() {
    // test inputs
    char change_car_cells_config[] = "max_car_cells 20";
    char print_input[] = "help";
    char print_input_still_works[] = "help this does not change the ouput";
    char error_input[] = "this should give an error";

    // Successfully changing one configurable
    assert(handle_user_input(change_car_cells_config) == 0);
    assert(max_car_cells == 20);

    // Successfully running print inputs
    assert(handle_user_input(print_input) == 0);
    assert(handle_user_input(print_input_still_works) == 0);

    // Get error code - TODO: right error code
    assert(handle_user_input(error_input) != 0);
}