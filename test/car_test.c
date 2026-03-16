#include "../include/car_lib.h"
#include "../include/data_types.h"
#include "../include/sim_lib.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
@brief tests the car_arrives function
*/
void test_car_arrives() {
    unsigned int id = 0;

    t_Car *car = car_arrives(100.00f, &id, 50);

    assert(car != NULL);
    assert(car->id == 0);
    assert(car->parking_time <= 50);
    assert(car->brand >= 0 && car->brand <= 26);

    t_Car *car_2 = car_arrives(100.00f, &id, 20);

    assert(car_2 != NULL);
    assert(car_2->id == 1);
    assert(car_2->parking_time <= 20);
    assert(car_2->brand >= 0 && car_2->brand <= 26);

    // 0% arrival chance -> always NULL
    t_Car *car_3 = car_arrives(0.00f, &id, 50);
    assert(car_3 == NULL);

    free(car);
    free(car_2);
}

/*
@brief tests the park_car function
*/
void test_park_car() {
    t_Car_Park *car_park = init_car_park(30);
    t_Parking_Cell *cell = car_park->first_parking_cell;
    t_Time time = 42;

    unsigned int id = 0;

    t_Car *car = car_arrives(100.00f, &id, 50);

    park_car(car, cell, time);

    assert(cell->p_car_in_cell == car);
    assert(car->start_parking_time == time);

    clear_car_park(car_park);
}

/*
@brief tests the check_parking_time function
*/
void test_check_parking_time() {
    unsigned int id = 0;

    t_Car *car = car_arrives(100.00f, &id, 50);
    car->parking_time = 50;
    car->start_parking_time = 50;

    // 50 + 50 = 100 <= 100 -> parking time expired
    assert(check_parking_time(car, 100) == 1);
    assert(check_parking_time(car, 120) == 1);

    // 50 + 50 = 100 > 51 -> parking time not yet expired
    assert(check_parking_time(car, 51) == 0);

    free(car);
}

/*
@brief tests the unpark_car function
*/
void test_unpark_car() {
    t_Car_Park *car_park = init_car_park(30);
    t_Parking_Cell *cell = car_park->first_parking_cell;
    t_Time time = 0;

    unsigned int id = 0;

    t_Car *car = car_arrives(100.00f, &id, 50);

    park_car(car, cell, time);
    // free_parking_cells was not decremented by park_car, only by park_car_in_park
    // so we just verify the cell is cleared after unpark
    unpark_car(cell, car_park);

    assert(cell->p_car_in_cell == NULL);
    // free_parking_cells should have been incremented by unpark_car
    assert(car_park->free_parking_cells == 31);

    clear_car_park(car_park);
}

/*
@brief tests the park_car_in_park function
*/
void test_park_car_in_park() {
    t_Car_Park *car_park = init_car_park(5);
    t_Time time = 0;
    unsigned int id = 0;

    // park one car and verify it ends up in the first cell
    t_Car *car = car_arrives(100.00f, &id, 50);
    park_car_in_park(car, car_park, time);

    assert(car_park->first_parking_cell->p_car_in_cell == car);

    // fill the remaining cells
    for (int i = 0; i < 4; i++) {
        t_Car *c = car_arrives(100.00f, &id, 50);
        park_car_in_park(c, car_park, time);
    }

    // all cells should be occupied
    t_Parking_Cell *cell = car_park->first_parking_cell;
    for (int i = 0; i < 5; i++) {
        assert(cell->p_car_in_cell != NULL);
        cell = cell->pNext;
    }

    clear_car_park(car_park);
}

/*
@brief tests the unpark_cars_in_park function
*/
void test_unpark_cars_in_park() {
    t_Car_Park *car_park = init_car_park(3);
    unsigned int id = 0;

    // car 1: parking time expired at t=100
    t_Car *car1 = car_arrives(100.00f, &id, 50);
    car1->parking_time = 10;
    park_car(car1, car_park->first_parking_cell, 0);

    // car 2: parking time not yet expired at t=100
    t_Car *car2 = car_arrives(100.00f, &id, 50);
    car2->parking_time = 200;
    park_car(car2, car_park->first_parking_cell->pNext, 0);

    unpark_cars_in_park(car_park, 100);

    // car1 should have been unparked
    assert(car_park->first_parking_cell->p_car_in_cell == NULL);
    // car2 should still be parked
    assert(car_park->first_parking_cell->pNext->p_car_in_cell == car2);

    clear_car_park(car_park);
}

/*
@brief tests the get_most_parked_brand function
*/
void test_get_most_parked_brand() {
    t_Car_Park *car_park = init_car_park(5);
    unsigned int id = 0;
    t_Time time = 0;

    // park 3x BMW (brand=0) and 1x VW (brand=1)
    for (int i = 0; i < 3; i++) {
        t_Car *car = car_arrives(100.00f, &id, 50);
        car->brand = 0; // BMW
        park_car_in_park(car, car_park, time);
    }
    t_Car *car_vw = car_arrives(100.00f, &id, 50);
    car_vw->brand = 1; // VW
    park_car_in_park(car_vw, car_park, time);

    assert(get_most_parked_brand(car_park) == 0); // BMW should win

    clear_car_park(car_park);
}

/*
@brief tests the get_brand_by_number function
*/
void test_get_brand_by_number() {
    assert(strcmp(get_brand_by_number(0),  "BMW")       == 0);
    assert(strcmp(get_brand_by_number(1),  "VW")        == 0);
    assert(strcmp(get_brand_by_number(7),  "MERCEDES")  == 0);
    assert(strcmp(get_brand_by_number(20), "PORSCHE")   == 0);
    assert(strcmp(get_brand_by_number(26), "SUZUKI")    == 0);
    assert(strcmp(get_brand_by_number(99), "")          == 0); // invalid value -> empty string
}

/*
@brief runs all car_lib tests
*/
int main() {
    test_car_arrives();
    test_park_car();
    test_check_parking_time();
    test_unpark_car();
    test_park_car_in_park();
    test_unpark_cars_in_park();
    test_get_most_parked_brand();
    test_get_brand_by_number();

    printf("All car_lib tests passed!\n");

    return 0;
}