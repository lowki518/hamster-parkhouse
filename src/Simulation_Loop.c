#include "../lib/queue_lib.h"
#include "../lib/car_lib.h"
#include "../lib/data_types.h"

/**
@brief The complete setup of the full simulation

@param[1] time_steps User defined number of time steps

@return void
*/
void do_simulation (t_Time time_steps) {
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