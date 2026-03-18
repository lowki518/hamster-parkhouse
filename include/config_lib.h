/*
@brief Header file for the Config library
*/

#ifndef CONFIG_LIB_H
#define CONFIG_LIB_H
#include "data_types.h"
#include <sys/types.h>
#include <stdint.h>



// ------- global config variable declarations --------- //
extern unsigned int max_car_cells;
extern t_Time max_parking_time;
extern t_Time simulation_time;
extern float car_probability;
extern unsigned int random_seed;
extern char output_path[256];
extern unsigned int max_cars_per_ts;

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

/*
@brief Changes the output path for output files

@param[1] new_path An input of the new output path

@return -1 for error
*/
int change_output_path(char * new_path);

/*
@brief Changes the randomness seed

@param[1] new_max_cars An input of the new amount of max cars

@return -1 for error
*/
int change_max_cars_per_ts(char * new_max_cars);

#endif