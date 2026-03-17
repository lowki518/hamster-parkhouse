/*
@brief Header file for the Car library
*/
#ifndef CAR_LIB_H
#define CAR_LIB_H

#include "data_types.h"

/*
@brief parks a car in an allocated parking cell

@param[1] car A pointer to the car that needs to be parked
@param[2] cell A pointer to the parking cell where the car should be parked
@param[3] time The current time

@return  void
*/
void park_car (t_Car *car, t_Parking_Cell *cell, t_Time time);


/*
@brief Unparks a car from its allocated parking cell

@param[1] cell A pointer to the cell of the car
@param[2] park A pointer to the car park

@return void
*/
void unpark_car(t_Parking_Cell *cell, t_Car_Park *park);

/*
@brief parks the car in the park and sets the parking time

@param[1] car A pointer to the car to park
@param[2] park A pointer to the car park
@param[3] time The current time

@return void
*/
void park_car_in_park (t_Car *car, t_Car_Park *park, t_Time time);

/*
@brief unparks all cars that need to unparked at the current time

@param[1] park A pointer to the car park
@param[2] time The current time

@return void
*/
void unpark_cars_in_park (t_Car_Park *park, t_Time time);

/*
@brief Generates the arrival of a car

The generating of a new car is based on the users choice of arrival chances in percentage.

@param[1] percentage The users choice of arrival chances
@param[2] id A pointer to the ID for the car
@param[3] max_parking The maximum allowed parking time

@return  the car or not
*/
t_Car * car_arrives (float percentage, unsigned int *id, t_Time max_parking);

/*
@brief Checks if parking time of a car is over 

@param[1] car The car to check
@param[2] time The time of the parked car to check

@return 1 if a car needs to leave, 0 if it needs to stay
*/
int check_parking_time(t_Car *car, t_Time time);

/*
@brief gets the current most parked brand

@param[1] park A pointer to the car park

@return the most parked brand
*/
Car_Brand get_most_parked_brand(t_Car_Park *park);

/*
@brief returns the name of the brand by its enum code

@param[1] brand_numb The number of the brand

@return the name of the brand
*/
char* get_brand_by_number(Car_Brand brand_numb);

#endif