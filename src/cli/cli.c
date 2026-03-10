//#include "cli.h"
#include "cli.h"
#include <string.h>
#include <stdio.h>
#include "../include/config.h"



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


    // Switch case does not work for strings, only ints (or chars).
    // Thus we use if else construction with strcmp.
    // Hashtable might also be possible but might also be overkill. 
    if (!strcmp(first_arg, "quit")) {
        // TODO: Implement Quitting
        printf("Quitting");
        return 0;
    } else if (!strcmp(first_arg, "start")) {
        // TODO: Implement Starting simulation
        printf("Start simulation");
        return 0;
    } else if (!strcmp(first_arg, "help")) {
        print_help();
        return 0;
    }
    printf("Input was invalid. Please try again. ");
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

/* 
  @brief Print command overview
 
  @return void
 */

void print_help() {
    // Print all the possible commands and usage
    // TODO: make nice looking menu for that
    printf("Hello, I am under da water. Please help.");
}

/*
  @brief Print configs + value
 
  @param[1] p_<config_variable> A pointer to config variable 
  ...

  @return void
 */
void print_config() {
    // print all name and values of config_variables in a nice fashion
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