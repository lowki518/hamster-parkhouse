#ifndef CAR_LIB_H
#define CAR_LIB_H

#include "data_types.h"

/*
@brief parks a car in an allocated parking cell

@param[1] car A pointer to the car that needs to be parked
@param[2] cell A pointer to the parking cell where the car should be parked

@return  void
*/
void park_car (t_Car *car, t_Parking_Cell *cell);


/*
@brief Unparks a car from its allocated parking cell

@param[1] car A pointer to the car that should be unparked
@param[2] cell A pointer to the cell of the car

@return void
*/
void unpark_car(t_Car *car, t_Parking_Cell *cell);


/*
@brief Generates the arrival of a car

The generating of a new car is based on the users choice of arrival chances in percentage.

@param[1] percentage The users choice of arrival chances
@param[2] lastID The last ID given to a car

@return  the car or not
*/
t_Car * car_arrives (float percentage, int *lastID);

/*
@brief Checks if parking time of a car is over 

@param[1] car The car to check
@param[2] time The time of the parked car to check

@return wether a car needs to leave or not
*/
int check_parking_time(t_Car *car, t_Time time);


#endif