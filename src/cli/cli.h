#ifndef CLI 
#define CLI

/**
 * @brief Read user input and error handling for bad input
 * 
 * @param[in] p_input pointer on input string (char *) 
 * @return 0 if successful, otherwise 1
 */
int read_user_input(char * input);


/**
 * @brief Handle user input commands
 * 
 * @param[in] p_input pointer on input string (char *) 
 * @return 0 if successful, otherwise 1
 */
int handle_user_input(char * input);


/** 
 * @brief Print command overview
 */
void start_menu();


/**
 * @brief Print configs + value
 * 
 * @param p_<config_variable> Pointer to config variable 
 * ...
 */
void print_help();


/**
 * @brief Print start menu (UI) and wait for user input
 */
void print_config();





#endif 