#include "car_lib.h"
#include "../data_types.h"
#include <stdio.h>

/*
@brief parks a car in an allocated parking cell

@param[1] car A pointer to the car that needs to be parked
@param[2] cell A pointer to the parking cell where the car should be parked

@return  void
*/
void park_car (t_Car *car, t_Parking_Cell *cell) {
    car->cell_index = cell[i];
}


/*
@brief Unparks a car from its allocated parking cell

@param[1] car A pointer to the car that should be unparked
@param[2] cell A pointer to the cell of the car

@return void
*/
void unpark_car(t_Car *car, t_Parking_Cell *cell) {
    // car leaves

    cell->is_free = TRUE;  // cell is emptied
    
    free (car);

}


/*
@brief Generates the arrival of a car

The generating of a new car is based on the users choice of arrival chances in percentage.

@param[1] percentage The users choice of arrival chances
@param[2] lastID The last ID given to a car

@return  the car or not
*/
t_Car * car_arrives (float percentage, int *lastID, t_Time max_parking) {
    if (((rand()%(10000+1))/100) < percentage) {
        t_Car *new_car = malloc(sizeof(t_Car));

        new_car->id = g_id;
        g_id =g_id++;

        new_car->parking_time = rand()%(max_parking + 1);
    }
}

/*
@brief Checks if parking time of a car is over 

@param[1] car The car to check
@param[2] time The time of the parked car to check

@return wether a car needs to leave or not
*/
int check_parking_time(t_Car *car, t_Time time) {
    if(car->parking_time + car->start_parking_time <= time) {
        return TRUE;
    }
    return FALSE;
}

