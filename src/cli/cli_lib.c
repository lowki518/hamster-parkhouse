#include "../../include/cli_lib.h"
#include "../../include/config_lib.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

int quit = FALSE;
int start = FALSE;

// this function is just so I can try quitting.
void return_quit() {
    printf("Quitting...\n");
    quit = TRUE;
}

void return_start() {
    printf("Starting simulation...\n");
    quit = TRUE;
    start = TRUE;
}

// Map for print functions
static struct print_map print_table[] = {
    {"help", print_help},
    {"config", print_config},
    {"quit", return_quit},
    {"start", return_start}
};
// Map for configuration changer functions
struct configure_map config_table[] = {
    {"max_car_cells", change_max_car_cells},
    {"max_parking_time", change_max_parking_time},
    {"simulation_time", change_sim_time},
    {"car_probability", change_car_probability},
    {"random_seed", input_random_seed},
    {"output_path", change_output_path}, 
    {"max_cars_per_ts", change_max_cars_per_ts}
};


/*
  @brief Read user input and error handling for bad input
  
  @param[1] p_input A pointer on input string (char *) 
 
  @return 0 if successful, otherwise 1
 */
int read_user_input(char *p_input) {
    if (!fgets(p_input, 255, stdin)) {
        // Error handling - TODO
        printf("Invalid user imput\n");
        return 1;
    }
    return 0;
}

/*
  @brief Handle user input commands
  
  @param[1] p_input A pointer on input string (char *) 
 
  @return 0 if successful, otherwise 1
 */
int handle_user_input(char *p_input) {
    // Our commands are at most 2 words long. 
    // first: split input by space and remove all whitespace
    // TODO: Remove all whitespace
    char *first_arg = strtok(p_input, " ");
    char *second_arg = strtok(NULL, "");


    // Function pointers are fucking awesome lol
    // iterate first over table of print functions, then over table of config changers
    for (int i = 0; i < (sizeof(print_table) / sizeof(print_table[0])); i++) {
        if (!strcmp(first_arg, print_table[i].print_name)) {
            print_table[i].p_print_fun();
            return 0;
        }
    }
    for (int i = 0; i < (sizeof(config_table) / sizeof(config_table[0])); i++) {
        if (!strcmp(first_arg, config_table[i].config_name)) {
            if (config_table[i].p_config_change(second_arg)) {
                printf("Input value \" %s \" for config %s could not be parsed. \n"
                "No changes were made. Please try again.\n", second_arg, first_arg);
            }
            return 0;
        }
    }
    printf(" \"\"\" %s \"\"\" did not fit any known command. \n", first_arg);
    return -1;
}

/* 
  @brief Print command overview
 
  @return void
 */
void print_help() {
    // Print all the possible commands and usage
    // TODO: is everything complete?
    printf("How to use: \n"
    "> help         print help menu (this menu)\n"
    "> config       print all configs and their values\n"
    "> start        start the simulation with chosen configs\n"
    "> quit         quits this program\n"
    "To change configs: \n"
    "> $config_name $new_value\n"
    "Allowed types for configurations: \n"
    "   - max_car_cells     unsigned int, e.g. 100\n"
    "   - max_parking_time  unsigned int, e.g. 20\n"
    "   - simulation_time   unsigned int, minimum 4, e.g. 200\n"
    "   - car_probability   float, e.g. 0.5\n"
    "   - random_seed       int, e.g. 12308964\n"
    "   - output_path       string describing path, e.g. output/my_file_name\n"
    "   - max_cars_per_ts   unsigned int, e.g. 5\n"
    );
}

/*
  @brief Print configs + value
 
  @return void
 */
void print_config() {
    // print all name and values of config_variables in a nice fashion
    // need to know how to get to the values: global vars? or not?
    // TODO: how to get config vars? all complete?
    printf("Overview of all configurables: \n"
    "max_car_cells - defines how big the carpark is:\n"
    "value: %i\n"
    "max_parking_time - defines how long cars are allowed to stay \n"
    "value: %i\n"
    "simulation_time - how many simulation steps are calculated, minimum 4\n"
    "value: %i\n"
    "car_probability - probability for a car to appear each simulation step\n"
    "value: %.2f\n"
    "random_seed - seed for randomness. defaults to timestamp\n"
    "value: %i\n"
    "output_path - output path for result file(s)\n"
    "value: %s\n"
    "max_cars_per_ts - max amount of cars coming in per time step\n"
    "value: %i\n", 
    max_car_cells, max_parking_time, simulation_time, 
    car_probability, random_seed, output_path, max_cars_per_ts
    );
}


/*
  @brief Print start menu (UI) and wait for user input

  @return void
 */
void start_menu() {
    // Standard text visualisation
    printf("Hello, this will be our start menu.\n");
    print_help();
}


/*
  @brief Print looped menu (UI) and wait for user input

  @return void
 */
void looped_menu() {
    // read and handle user input
    char user_input[256]; // do we need to m/calloc?
    printf("Please enter your command: \n> ");
    read_user_input(user_input);
    handle_user_input(user_input);
}
