#include "../include/car_lib.h"
#include "../include/data_types.h"
#include "../include/sim_lib.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>



/*
@brief tests the car_arrives function
*/
void test_car_arrives() {
    unsigned int *id;
    *id = 0;

    t_Car *car = car_arrives(100.00f, id, 50);

    assert(car->id == 0);
    assert(car->parking_time <= 50);
    assert(0 <= car->brand <= 26); // 26 is the number of brands

    t_Car *car_2 = car_arrives(100.00f, id, 20);

    assert(car_2->id == 1);
    assert(car->parking_time <= 20);
    assert(0 <= car_2->brand <= 26); // 26 is the number of brands

    free(car);
    free(car_2);

}

/*
@brief tests the park_car function
*/
void test_park_car() {
    t_Car_Park *car_park = init_car_park(30);

    t_Parking_Cell *cell = car_park->first_parking_cell;

    t_Time time;

    unsigned int *id;
    *id = 0;

    t_Car *car = car_arrives(100.00f, id, 50);

    park_car(car, cell, time);

    assert(cell->p_car_in_cell == car);
    assert(car->start_parking_time == time);

    free(car);
    free(cell);
    free(car_park);
}

/*
@brief tests the check_parking_time function
*/
void test_check_parking_time() {
    unsigned int *id;
    *id = 0;

    t_Car *car = car_arrives(100.00f, id, 50);
    car->parking_time = 50;
    car->start_parking_time = 50;

    assert(check_parking_time(car, 100) == 1);
    assert(check_parking_time(car, 120) == 1);
    
    assert(check_parking_time(car, 51) == 0);

    free(car);
}

/*
@brief tests the unpark_car function
*/
void test_unpark_car() {
    t_Car_Park *car_park = init_car_park(30);
    t_Parking_Cell *cell = car_park->first_parking_cell;

    t_Time time;

    unsigned int *id;
    *id = 0;

    t_Car *car = car_arrives(100.00f, id, 50);

    park_car(car, cell, time);

    assert(cell->p_car_in_cell == NULL);

    free(car);
    free(cell);
    free(car_park);
}

/*
@brief tests the park_car_in_park function
*/
void test_park_car_in_park() {

}

/*
@brief tests the unpark_cars_in_park function
*/
void test_unpark_cars_in_park() {

}

/*
@brief tests the gest_most_parked_brand function
*/
void test_gest_most_parked_brand() {

}

/*
@brief tests the get_brand_by_number function
*/
void test_get_brand_by_number() {

}

/*
@brief tests the whole car_lib
*/
void test_car_lib() {
    test_car_arrives();
    test_park_car();
    test_check_parking_time();
    test_unpark_car();

    test_park_car_in_park();
    test_unpark_cars_in_park();
    test_gest_most_parked_brand();
    test_get_brand_by_number();
}