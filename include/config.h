#ifndef CONFIG
#define CONFIG
#include "data_types.h"
#include <sys/types.h>
#include <stdint.h>


/*
 @brief Change global variable g_simulation_time

 @param[1] t_Time simulation_time The paramater to be changed

 @return -1 for error
*/
int change_sim_time(char * new_simulation_time);


/*
@brief change max_car_cell field inside car_park

@param[1] max_park_cells An integer of max. parking cells
@param[2] t_Car_Park A pointer to the car park

@return -1 for error
*/
int change_max_car_cells(char * new_max_park_cells);

/*
@brief Changing the max. parking time

@param[1] max_parking_time Describes the max. parking time for the cars
@param[2] t_Car_Park A pointer to the car park

@return -1 for error
*/
int change_max_parking_time(char * new_max_parking_time);

/*
@brief Changes the probability of car arrival

@param[1] car_probability A float for car arrival

@return -1 for error
*/
int change_car_probability(char * new_car_probability);

/*
@brief Changes the randomness seed

@param[1] seed An input of the randomness seed

@return -1 for error
*/
int input_random_seed(char * new_seed); 

#endif