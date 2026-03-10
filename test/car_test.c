#include "../include/car_lib.h"
#include "../include/data_types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>




/*
@brief tests the car_arrives function
*/
void test_car_arrives() {
    g_id = 0;

    t_Car *car = car_arrives(100.00f, 50);

    assert(car->id == 0);
    assert(car->parking_time <= 50);

    free(car);

}


/*
@brief tests the check_parking_time function
*/
void test_check_parking_time() {

    t_Car *car = car_arrives(100.00f, 50);
    car->parking_time = 50;
    car->start_parking_time = 50;

    assert(check_parking_time(car, 100) == 1);
    assert(check_parking_time(car, 120) == 1);
    
    assert(check_parking_time(car, 51) == 0);

    free(car);
}