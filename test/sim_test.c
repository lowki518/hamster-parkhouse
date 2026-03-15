#include "../include/sim_lib.h"
#include "../include/data_types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
@brief tests the init_new_cell function
*/
void test_init_new_cell() {
    t_Parking_Cell *new_cell = init_new_cell();

    assert(new_cell->pNext == NULL);
    assert(new_cell->p_car_in_cell == NULL);
  
    free(new_cell);
}

/*
@brief tests the init_car_parl function
*/
void test_init_car_park() {
    t_Car_Park *car_park = init_car_park(30);

    assert(car_park->max_parking_cells == 30);
    assert(car_park->first_parking_cell != NULL);
    assert(car_park->free_parking_cells == 30);

    t_Parking_Cell *cell = car_park->first_parking_cell;

    while (cell != NULL) {
        t_Parking_Cell *ctmp = cell->pNext;
        free(cell);
        cell = ctmp;
    }

    free(car_park);
}

/*
@brief tests the clear_car_park function
It makes no sense to do multiple unit tests here, as the park is cleared
*/
void test_clear_car_park() {
    t_Car_Park *car_park = init_car_park(20);

    clear_car_park(car_park);

    assert(car_park == NULL);
}