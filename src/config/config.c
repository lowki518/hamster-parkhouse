#include "../types/data_types.h"
#include <sys/types.h>
#include <stdint.h>


/*
Change global variable g_simulation_time
*/
int change_sim_time(t_Time simulation_time);


/*
change max_car_cell field inside car_park
*/
int change_max_car_cells(uint8_t max_car_cells, t_Car_Park * car_park);

/*

*/
int change_max_parking_time(t_Time max_parking_time, t_Car_Park * car_park);


int change_car_probability(float car_probability);

// wtf this ugly I believe this ain't right with uint8
int input_random_seed(uint8_t seed); 
