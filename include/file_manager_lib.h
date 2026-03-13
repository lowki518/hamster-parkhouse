#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "data_types.h"


/*
@brief Gets the number (Simulation_X.txt) of the last file in results.

@param[1] folder The path to the folder to search in

@return Returns the number of the last file
*/
int get_new_file_number (char *folder);


/*
@brief Creates the new file for saving the simulation data with the sim. parameters.

@param[1] path The path to the folder to save the file in
@param[2] new_file_number The number of the File to write.
@param[3] sim_duration The total amount of time steps done.
@param[4] parking_cells The amount of parking_spaces.
@param[5] max_parking_duration The maximum parking time for the cars.
@param[6] new_car_prob The probability of a new car arriving (in percent).
@param[7] max_cars_arriving How many cars can arrive in a simulation step.
@param[8] seed The seed used to generate randomness within this simulation.

@return void
*/
void create_new_file_with_head_data (char *path, int new_file_number, t_Time sim_duration, int parking_cells, t_Time max_parking_duration, float new_car_prob, int max_cars_arriving, unsigned int seed);


/*
@brief Appends the data for every simulated step to the file.

@param[1] path The path to the directory of the file
@param[2] new_file_number The number of the File to write in.
@param[3] timestep The current timestep.
@param[4] cars_parked The amount of cars currently parked.
@param[5] avg_parking_time The average parking time of the cars.
@param[6] q_len The current length of the queue.
@param[7] full_house_steps The amount of steps the parking garage was full.
@param[8] tot_cars_simulated The total amount of cars that was simulated.
@param[9] most_brand The car brand that parked the most.

@return void
*/
void append_data_per_timestep (char *path, int new_file_number, t_Time timestep, int cars_parked, float avg_parking_time, int q_len, int full_house_steps, int tot_cars_simulated, Car_Brand most_brand);


/*

@brief writes the new config data into the config file.

@param[1] parking_cells The amount of parking cells.
@param[2] max_parking_duration The max. amount of timesteps a car can park.
@param[3] sim_duration The duration of the simulation.
@param[4] new_car_prob The probability of a new car arriving.
@param[5] max_cars_arriving The maximum amount of cars arriving in one step.
@param[6] seed The seed used to generate the randomness within the simulation.


void write_config_data (int parking_cells, t_Time max_parking_duration, t_Time sim_duration, float new_car_prob, int max_cars_arriving, unsigned int seed);



@brief Reads the config file

@param[1] parking_cells A pointer to where the amount of parking cells will be stored.
@param[2] max_parking_duration A pointer to where the maximum parking duration will be stored.
@param[3] sim_duration A pointer to where the duration of the simulation will be stored.
@param[4] new_car_prob A pointer to where the probability of a car arriving will be stored.
@param[5] max_cars_arriving A pointer to where the maximum amount of cars arriving in one step will be stored.
@param[6] seed A pointer to where the seed will be stored.

@return void

void read_config_data (int *parking_cells, t_Time *max_parking_duration, t_Time *sim_duration, float *new_car_prob, int *max_cars_arriving, unsigned int *seed);

*/
#endif