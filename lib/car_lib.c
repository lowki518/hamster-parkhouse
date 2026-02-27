#include "car_lib.h"
#include <stdio.h>

/*
@brief parks a car in an allocated parking cell

@param[1] car A pointer to the car that needs to be parked
@param[2] cell A pointer to the parking cell where the car should be parked

@return  void
*/
void park_car (t_Car *car, t_Parking_Cell *cell) {
    /*
        *car = *cell //car parks in cell
    */
}


/*
@brief Unparks a car from its allocated parking cell

@param[1] car A pointer to the car that should be unparked
@param[2] cell A pointer to the cell of the car

@return void
*/
void unpark_car(t_Car *car, t_Parking_Cell *cell) {
    /*
        car leaves
        *cell = free
        car gets free/deleted
    */
}


/*
@brief Generates the arrival of a car

The generating of a new car is based on the users choice of arrival chances in percentage.

@param[1] percentage The users choice of arrival chances
@param[2] lastID The last ID given to a car

@return  the car or not
*/
t_Car * car_arrives (float percentage, int *lastID, t_Time max_parking) {
    /*
        FOR (rand(0-100) < percentage) DO
            generate new car = new_car
            new_car get one more ID 
            new_car gets a max parking time assigned
        END FOR
    */    
}

/*
@brief Checks if parking time of a car is over 

@param[1] car The car to check
@param[2] time The time of the parked car to check

@return wether a car needs to leave or not
*/
int check_parking_time(t_Car *car, t_Time time) {
    /*
        IF car parking time over THEN
            car leaves
        END IF 
    */
}