#include "../../include/car_lib.h"
#include "../../include/data_types.h"
#include <stdio.h>

/*
@brief parks a car in an allocated parking cell

@param[1] car A pointer to the car that needs to be parked
@param[2] cell A pointer to the parking cell where the car should be parked
@param[3] time The current time

@return  void
*/
void park_car (t_Car *car, t_Parking_Cell *cell, t_Time time) {
    cell->car_in_cell = car;
    car->start_parking_time = time;
}


/*
@brief Unparks a car from its allocated parking cell

@param[1] cell A pointer to the cell of the car

@return void
*/
void unpark_car(t_Parking_Cell *cell) {

    free(cell->car_in_cell);

    cell->car_in_cell = NULL;

}


/*
@brief Generates the arrival of a car

The generating of a new car is based on the users choice of arrival chances in percentage.

@param[1] percentage The users choice of arrival chances
@param[2] id A pointer to the ID for the car
@param[3] max_parking The maximum allowed parking time

@return  the car or not
*/
t_Car * car_arrives (float percentage, unsigned int *id, t_Time max_parking) {
    
    //generates random number between 0 and 10000 abd converts percentage to int
    int rand = rand() % 10001;
    if (rand <= (int) (percentage*100)) {
        t_Car *new_car = malloc(sizeof(t_Car));
        if(!t_Car) {
            return NULL;
        }

        new_car->id = *id;
        (*id)++;

        new_car->parking_time = rand()%(max_parking + 1);

        return new_car;
    }
    return NULL;
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

