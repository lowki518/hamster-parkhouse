#ifndef CLI 
#define CLI

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
  @brief Print command overview
 
  @return void
 */
void print_help();

/*
  @brief Print configs + value
 
  @param[1] p_<config_variable> A pointer to config variable 
  ...

  @return void
 */
void print_config();

#endif 