#include "cli.h"
#include <stdio.h>


/**
 * @brief Read user input and error handling for bad input
 * 
 * @param[in] p_input pointer on input string (char *) 
 * @return 0 if successful, otherwise 1
 */
int read_user_input(char *p_input) {
    if (!scanf("%255[^\n]%*c", p_input)) {
        // Error handling - TODO
        printf("Error uwu");
        return 1;
    }
    
    return 0;
}

/**
 * @brief Handle user input commands
 * 
 * @param[in] p_input pointer on input string (char *) 
 * @return 0 if successful, otherwise 1
 */
int handle_user_input(char *p_input) {
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
    return 0;
}

/** 
 * @brief Print command overview
 */
void print_help() {
    // Print all the possible commands and usage
    // TODO: make nice looking menu for that
}

/**
 * @brief Print configs + value
 * 
 * @param p_<config_variable> Pointer to config variable 
 * ...
 */
void print_config() {
    // print all name and values of config_variables in a nice fashion
}


/**
 * @brief Print start menu (UI) and wait for user input
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