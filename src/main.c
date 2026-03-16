
#include "../include/cli_lib.h"
#include "../include/data_types.h"
#include "../include/sim_lib.h"

#include <stdio.h>


int main() {
    // Opens CLI start Menu
    start_menu();

    while (!quit) {
        looped_menu();
        if (start) {
            int *sim_nr = start_simulation();
            start = FALSE;
        }
        if (file_nr == -1) {
            printf("Please input your shit in here @Ben you mofo ÒnÓ\n");
        }
    }
}