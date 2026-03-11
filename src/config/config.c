#include "../../include/data_types.h"
#include "../../include/config.h"
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>

// ------- global config variable declarations --------- //
// can add default values also btw
uint8_t max_car_cells;
t_Time max_parking_time;
uint8_t simulation_time;
float car_probability;
int random_seed;
char output_path[256];


/*
 @brief Change global variable g_simulation_time

 @param[1] t_Time simulation_time The paramater to be changed

 @return -1 for error
*/
int change_sim_time(char* new_simulation_time) {
    uint8_t new_value = (t_Time)atoi(new_simulation_time);
    if (!new_value) {
        return -1;
    }
    simulation_time = new_value;
    return 0;
}


/*
@brief change max_car_cell field inside car_park

@param[1] max_park_cells An integer of max. parking cells
@param[2] t_Car_Park A pointer to the car park

@return -1 for error
*/
int change_max_car_cells(char * new_max_car_cells){
    uint8_t new_value = (uint8_t)atoi(new_max_car_cells);
    if (!new_value) {
        return -1;
    }
    max_car_cells = new_value;
    return 0;
}

/*
@brief Changing the max. parking time

@param[1] max_parking_time Describes the max. parking time for the cars
@param[2] t_Car_Park A pointer to the car park

@return -1 for error
*/
int change_max_parking_time(char * new_max_parking_time) {
    uint8_t new_value = (t_Time)atoi(new_max_parking_time);
    if (!new_value) {
        return -1;
    }
    max_parking_time = new_value;
    return 0;
}

/*
@brief Changes the probability of car arrival

@param[1] car_probability A float for car arrival

@return -1 for error
*/
int change_car_probability(char * new_car_probability) {
    float new_value = atof(new_car_probability);
    if (!new_value) {
        return -1;
    }
    car_probability = new_value;
    return 0;
}

/*
@brief Changes the randomness seed

@param[1] seed An input of the randomness seed

@return -1 for error
*/
int input_random_seed(char * new_seed) {
    // might also just typecast? then it will always be okay? 
    uint8_t new_value = atoi(new_seed);
    if (!new_value) {
        return -1;
    }
    random_seed = new_value;
    return 0;
}
