#include "../include/car_lib.h"
#include "../include/data_types.h"
#include "../include/init_lib.h"
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
^

void test_park_car() {
    init_car_park();
    car_arrives();

    assert(cell->p_car_in_cell == car)

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