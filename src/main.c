#include <stdio.h>
#include <string.h>
#include "types/data_types.h"

// Global variables:
t_Time g_simulation_time;
float g_car_probability;
int rand_seed(); // should be uint8 in the end!


// remove to main menu module
void handle_user_input(char * input);



int main() {
    char input[256];
    printf("Hello to our program");

    while (1) {
        printf("> "); // start of user input line
        scanf("%255[^\n]%*c", input);
        handle_user_input(input);
        // Also break condition for while loop in handle_user_input() needed
    }
    return 0;
}