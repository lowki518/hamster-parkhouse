#include "../../include/sim_lib.h"
#include "../../include/car_lib.h"
#include "../../include/config_lib.h"
#include "../../include/queue_lib.h"
#include "../../include/file_manager_lib.h"
#include "../../include/statistics_output_lib.h"
#include <stdlib.h>


/*
@brief Defines a new parking cell for car park

@return new cell
*/
t_Parking_Cell *new_cell() {
    t_Parking_Cell *new_cell = malloc(sizeof(*new_cell));
    if (!new_cell) {
        return NULL;
    }

    new_cell->pNext = NULL;
    new_cell->p_car_in_cell = NULL;
    return new_cell;
}

/*
@brief Initiates the car park with the declared amount of parking cells

@param[1] max_parking_cells The amount of parking cells of the car park

@return car park
*/
t_Car_Park *init_car_park(int max_parking_cells) {
    t_Car_Park *car_park = malloc(sizeof(car_park));
    if(!car_park) {
        return NULL;
    }
    
    car_park->max_parking_cells = max_parking_cells;

    // In the beginning all cells are free
    car_park->free_parking_cells = max_parking_cells;

    t_Parking_Cell *c_cell = new_cell();
    car_park->first_parking_cell = c_cell;

    for (int i = 0; i < max_parking_cells; i++) {
        c_cell->pNext = new_cell();
        c_cell = c_cell->pNext;
    }

    return car_park;
}

/*
@brief Clears the whole car park

@param[1] car_park A pointer to the car park

@return void
*/
void clear_car_park(t_Car_Park *car_park) {
    t_Parking_Cell *cell = car_park->first_parking_cell;

    while (cell != NULL) {
        unpark_car(cell, car_park);
        t_Parking_Cell *ctmp = cell->pNext;
        free(cell);
        cell = ctmp;
    }

    free(car_park);
}

/*
@brief Does the entire simulation and prits the data

@returns the number of the completed simulation
*/
int *start_simulation () {

    srand(random_seed);

    // allocates all the needed variables for the simulation
    unsigned int *car_id = malloc(sizeof(car_id));
    int *sim_nr = malloc(sizeof(sim_nr));
    t_Time *time = malloc(sizeof(time));
    t_Time *tot_parking_time = malloc(sizeof(tot_parking_time));
    int *full_house_steps = malloc(sizeof(full_house_steps));
    
    
    // if any allocation failed, abort
    if(!car_id || !sim_nr || !time || !tot_parking_time || !full_house_steps) {
        printf("Error allocating memory.\n");
        return NULL;
    }

    *car_id = 0;
    *sim_nr = get_new_file_number(output_path);
    *tot_parking_time = 0;
    *full_house_steps = 0;

    t_Car_Park *park = init_car_park(max_car_cells);
    t_Parking_Cell *cell = park->first_parking_cell;
    t_Queue *queue = init_queue();

    create_new_file_with_head_data(output_path, *sim_nr, simulation_time, max_car_cells, max_parking_time, car_probability, max_cars_per_ts, random_seed);
    print_head_data(*sim_nr, simulation_time, max_car_cells, max_parking_time, car_probability, max_cars_per_ts, random_seed);

    for (*time = 0; *time <= simulation_time; *(time)++) {

        // checks if any cars in the park need to be unparked
        unpark_cars_in_park(park, *time);

        // creates new cars
        for (int i = 0; i <= max_cars_per_ts; i++) {
            t_Car *new_car = car_arrives(car_probability, car_id, max_parking_time);
            if (new_car != NULL) {
                en_queue(queue, new_car);
                tot_parking_time += new_car->parking_time;
            }
        }

        // as long as there are free parking cells and cars in the queue -> park the cars
        while(park->free_parking_cells > 0 || queue->q_length > 0) {
            t_Car *car = de_queue(queue);
            park_car_in_park(car, park, *time);
        }
        
        // checks if the park is full and increases the counter
        if(park->free_parking_cells == 0) {
            *(full_house_steps)++;
        }

        // prints the data of the current timestep
        // TODO print the data in the console
        float avg_parking_time = (float) *tot_parking_time / (float) (*(car_id) + 1);
        Car_Brand brand = get_most_parked_brand(park);
        print_data_per_timestep(*time, (park->max_parking_cells - park->free_parking_cells), avg_parking_time, queue->q_length, *full_house_steps, *(car_id) + 1, brand);
        append_data_per_timestep(output_path, *sim_nr, *time, (park->max_parking_cells - park->free_parking_cells), avg_parking_time, queue->q_length, *full_house_steps, *(car_id) + 1, brand);
        
    }
    
    // clears the memory
    clear_queue(queue);
    clear_car_park(park);
    
    free(car_id);
    free(time);
    free(full_house_steps);
    free(tot_parking_time);

    return sim_nr;
}