#ifndef CLI_LIB_H
#define CLI_LIB_H

#include "data_types.h"
#include <stdint.h>

// Flag for running & starting simulation
extern int quit;
extern int start;
extern int file_nr;

// Structure for a mapping from string (command input) to a function
// with Command_Arg type argument
struct configure_map {
    char *config_name;
    int (*p_config_change)(char* arg);
};

struct print_map {
  char *print_name;
  void (*p_print_fun)();
};





/*
  @brief Read user input and error handling for bad input
  
  @param[1] p_input A pointer on input string (char *) 
 
  @return 0 if successful, otherwise 1
 */
int read_user_input(char * input);


/*
  @brief Handle user input commands
  
  @param[1] p_input A pointer on input string (char *) 
 
  @return 0 if successful, otherwise 1
 */
int handle_user_input(char * input);


/*
  @brief Print start menu (UI) and wait for user input

  @return void
 */
void start_menu();


/*
  @brief Print looped menu (UI) and wait for user input

  @return void
 */
void looped_menu();

/* 
  @brief Print command overview
 
  @return void
 */
void print_help();

/*
  @brief Print configs + value
 
  @return void
 */
void print_config();

#endif 