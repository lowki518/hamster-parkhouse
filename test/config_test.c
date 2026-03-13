#include "../include/config_lib.h"
#include <assert.h>
#include <string.h>

/* 
@brief test change_sim_time function
*/
void test_change_sim_time() {
    assert(change_sim_time("13") == 0);
    assert(simulation_time == 13);
    assert(change_sim_time("invalid") != 0);
}

/* 
@brief test change_max_car_cells function
*/
void test_change_max_car_cells() {
    assert(change_max_car_cells("23") == 0);
    assert(max_car_cells == 23);
    assert(change_max_car_cells("invalid") != 0);
}

/* 
@brief test change_max_parking_time function
*/
void test_change_max_parking_time() {
    assert(change_max_parking_time("123") == 0);
    assert(max_parking_time == 123);
    assert(change_max_parking_time("invalid") != 0);
}

/* 
@brief test change_car_probability function
*/
void test_change_car_probability() {
    // Does this work? since floating point comparison might be 
    // a little fucked
    assert(change_car_probability("14.6") == 0);
    assert((int)car_probability == (int)14.6);
    assert(change_car_probability("invalid") != 0);
}

/* 
@brief test input_random_seed function
*/
void test_input_random_seed() {
    assert(input_random_seed("1234") == 0);
    assert(random_seed == 1234);
    assert(input_random_seed("invalid") != 0);
}

/* 
@brief test change_output_path function
*/
void test_change_output_path() {
    assert(change_output_path("output_stuff/"));
    assert(!strcmp(output_path, "output_stuff/"));
}

/* 
@brief test change_max_cars_per_ts function
*/
void test_change_max_cars_per_ts() {
    assert(change_max_cars_per_ts("123") == 0);
    assert(max_cars_per_ts == 123);
    assert(change_max_cars_per_ts("invalid") != 0);
}

int main() {
    test_input_random_seed();
    test_change_sim_time();
    test_change_output_path();
    test_change_max_cars_per_ts();
    test_change_max_parking_time();
    test_change_car_probability();
    test_change_max_car_cells();
}