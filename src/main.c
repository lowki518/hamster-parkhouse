
#include "../include/cli_lib.h"
#include "../include/data_types.h"
#include "../include/config_lib.h"
#include "../include/sim_lib.h"
#include "../include/statistics_output_lib.h"

#include <stdio.h>


int main() {
    // Opens CLI start Menu
    start_menu();


    while (!quit) {
        looped_menu();
        if (start) {
            int *sim_nr = start_simulation();
            if(sim_nr != NULL) {
                open_gui(output_path, *sim_nr);
            }
            free(sim_nr);
            start = FALSE;

        }
        if (file_nr != -1) {
            open_gui(output_path, file_nr);

            file_nr = -1;
        }
    }

}