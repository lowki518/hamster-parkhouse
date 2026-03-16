/*
@brief Header File for simulation library
*/

#ifndef INIT_LIB_H
#define INIT_LIB_H

#include "data_types.h"
#include "car_lib.h"
#include <stdio.h>
#include <stdlib.h>


/*
@brief Defines a new parking cell for car park

@return new cell
*/
t_Parking_Cell *new_cell();



/*
@brief Initiates the car park with the declared amount of parking cells

@param[1] max_parking_cells The amount of parking cells of the car park

@return car park
*/
t_Car_Park *init_car_park(int max_parking_cells);

/*
@brief Clears the whole car park

@param[1] car_park A pointer to the car park

@return void
*/
void clear_car_park(t_Car_Park *car_park);


/*
@brief Does the entire simulation and prits the data

@returns the number of the completed simulation
*/
int *start_simulation ();

#endif