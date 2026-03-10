#include "../include/file_manager_lib.h"
#include "../include/data_types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
@brief tests the new_file_number function
*/
void test_get_file_number() {

    assert(get_new_file_number("./data_test_1") == 3);
    assert(get_new_file_number("./data_test_2") == 1);

}

/*
@brief tests the create_new_file_with_head_data function
*/
void test_create_new_file_with_head_data (int new_file_number, t_Time sim_duration, int parking_cells, t_Time max_parking_duration, float new_car_prob, int max_cars_arriving, unsigned int seed) {
    create_new_file_with_head_data()
}
 