//#include "cli.h"
#include "cli.h"
#include <string.h>
#include <stdio.h>
//#include "../include/config.h"

// This probably should be put into the cli.h file, but doing that later. 
// since I'm using t_Time it will not compile either way because of course... >:(
void print_quit() {
    printf("quit");
}

static struct print_map print_table[3] = {
    {"help", print_help},
    {"config", print_config},
    {"quit", print_quit}
};


/*
  @brief Read user input and error handling for bad input
  
  @param[1] p_input A pointer on input string (char *) 
 
  @return 0 if successful, otherwise 1
 */
int read_user_input(char *p_input) {
    if (!scanf("%255[^\n]%*c", p_input)) {
        // Error handling - TODO
        printf("Error");
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
    /*
    for (int i = 0; i < (sizeof(config_table) / sizeof(config_table[0])); i++) {
        if (!strcmp(first_arg, config_table[i].config_name)) {
            config_table[i].p_config_fun(second_arg);
            return 0;
        }
    }
    */
    printf(" \"\"\" %s \"\"\" did not fit any known command. ", first_arg);
    return 1;
    /*
    input_array = split input by "space"    

    switch (input_array[0])
        case <var_name>: // for each var_name own case
            if typecheck input_array[1]:
                change_<var>(input_array[1])
            else:
                error wrong type
        case "exit", "q", "quit":
            exit program 
        case "help", "h":
            print help_menu
        case "configs": 
            print config data
        default: 
            print "Did not understand", help menu
    */
}

/* 
  @brief Print command overview
 
  @return void
 */

void print_help() {
    // Print all the possible commands and usage
    // TODO: make nice looking menu for that
    printf("How to use: \n"
    "> help         print help menu (this menu)\n"
    "> config       print all configs and their values\n"
    "> start        start the simulation with chosen configs\n"
    "> ...\n"
    "To change configs: \n"
    "> $config_name $new_value\n"
    "Allowed types for configurations: \n"
    "   - max_car_cells     unsigned int, e.g. 100\n"
    "   - max_parking_time  unsigned int, e.g. 20\n"
    "   - simulation_time   unsigned int, e.g. 200\n"
    "   - car_probability   float, e.g. 0.5\n"
    "   - random_seed       int, e.g. 12308964\n"
    "   - output_path       string describing path, e.g. output/my_file_name\n"
    );

}

/*
  @brief Print configs + value
 
  @param[1] p_<config_variable> A pointer to config variable 
  ...

  @return void
 */
void print_config() {
    // print all name and values of config_variables in a nice fashion
    // need to know how to get to the values: global vars? or not?
    printf("All names and values in a nice fashion");
}

/*
  @brief Print start menu (UI) and wait for user input

  @return void
 */

void start_menu() {
    // Standard text visualisation
    printf("Hello, this will be our start menu. ");
    printf("\nPlease input one of the following commands: \n");
    printf("tbd \n");
    printf("> ");
    
    // read and handle user input
    char *user_input; // do we need to m/calloc?
    read_user_input(user_input);
}

int main() {
    char input[256];
    read_user_input(input);
    handle_user_input(input);
    return 0;
}