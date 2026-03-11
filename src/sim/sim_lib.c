#include "../../include/sim_lib.h"
#include "../../include/config_lib.h"
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
        unpark_car(cell);
        t_Parking_Cell *ctmp = cell->pNext;
        free(cell);
        cell = ctmp;
    }

    free(car_park);
}

/*
@brief The complete setup of the full simulation

@param[1] time_steps User defined number of time steps (length of sim)

@return void
*/
void start_simulation (t_Time time_steps) {

    t_Car_Park *park = init_car_park(max_car_cells);
    t_Parking_Cell *cell = park->first_parking_cell;
    
    for (int i = 0; i <= time_steps; i++) {
        
    }
    

    /*
    print_simulation_data()

        FOR i FROM 0 To time_steps {

            FOR EVERY parking_cell {
                IF CELL IS FREE -> continue

                IF IS PARKING TIME OVER? THEN
                    car_leaves()
                    CELL = FREE
            }

            IF CAR IS WAITING IN QUEUE THEN
                check_free_parking_cells() -> car_parks()

            IF A NEW CAR ARRIVES THEN
                check_free_parking_cells() -> car_parks()
                ELSE go_into_queue()

            print_timestep_data()
            save_timestep_data()
        }

        visualize_total_data()
    */
}