
#include "../include/car_lib.h"
#include "../include/cli_lib.h"
#include "../include/config_lib.h"
#include "../include/data_types.h"
#include "../include/file_manager_lib.h"
#include "../include/sim_lib.h"
#include "../include/queue_lib.h"
#include "../include/statistics_output_lib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main() {
    // Opens CLI start Menu
    start_menu();

    // Keeps menu open, until simulation is started or quit
    while (!quit) {
        looped_menu();
    }

    // If user wants to quit -> don't start simulation and exit
    if(!start) {
        return 0;
    }

    int *sim_nr = start_simulation();

}