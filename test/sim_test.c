#include "../include/sim_lib.h"
#include "../include/car_lib.h"
#include "../include/data_types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
@brief tests the new_cell function
*/
void test_new_cell() {
    t_Parking_Cell *cell = new_cell(); 

    assert(cell != NULL);
    assert(cell->pNext == NULL);
    assert(cell->p_car_in_cell == NULL);

    free(cell);
}

/*
@brief tests the init_car_park function
*/
void test_init_car_park() {
    t_Car_Park *car_park = init_car_park(30);

    assert(car_park != NULL);
    assert(car_park->max_parking_cells == 30);
    assert(car_park->free_parking_cells == 30);
    assert(car_park->first_parking_cell != NULL);

    // verify all cells are linked and empty
    t_Parking_Cell *cell = car_park->first_parking_cell;
    int count = 0;
    while (cell->pNext != NULL) { 
        assert(cell->p_car_in_cell == NULL);
        cell = cell->pNext;
        count++;
    }
    assert(count == 30); 

    clear_car_park(car_park); 
}

/*
@brief tests the clear_car_park function
*/
void test_clear_car_park() {
    t_Car_Park *car_park = init_car_park(20);

    clear_car_park(car_park);
}

/*
@brief tests clear_car_park with parked cars
*/
void test_clear_car_park_with_cars() {
    t_Car_Park *car_park = init_car_park(5);
    unsigned int id = 0;
    t_Time time = 0;

    // park some cars before clearing
    for (int i = 0; i < 3; i++) {
        t_Car *car = car_arrives(100.00f, &id, 50);
        park_car_in_park(car, car_park, time);
    }

    // should not crash even with cars still parked
    clear_car_park(car_park);
}

/*
@brief tests the entire sim_lib
*/
int main() {
    test_new_cell();
    test_init_car_park();
    test_clear_car_park();
    test_clear_car_park_with_cars();

    printf("All sim_lib tests passed!\n");

    return 0;
}
