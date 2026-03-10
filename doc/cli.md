# CLI - Commands

## change <variable>
Possible to change all variables by name:
- max_car_cells (unsigned int)
- max_parking_time in steps (unsigned int)
- simulation_time in time_steps (unsigned int)
- car_probability (float)
- random_seed (int)
- output path for results
- TODO: any vars missing? 
```bash
> <variable> <new_value>
```

## start simulation
```bash
> start
```

## exit program
```bash
> exit
> q
> quit
```

## help
print overview over all commands and usage
-> start menu will start with 'help' command run

## print configs
```bash
> configs
```
List config names and values in nice format. 
Needed to read them. 

# CLI - How it looks:
Once you start the program it will look like this: 
```
Welcome to our car park simulation. 
How to use: 
  > command1     Explanation
  > command2     ...
  > ...
Change configs: 
  > $config_name $new_value
  Allowed types for configurations: 
    - config_name(s): input_type
    - ...
...
Please enter your input: 
> 
```
To get a list of all configs you will have to run config command. 
Otherwise it should be very straightforward since we don't plan on having too many commands.
We will however not clear the terminal after each command/specific commands (except maybe "clear" command. To be discussed though)
 

# How it will work: 
We use the REPL (Read-Eval-Print Loop) strategy as it gives the user a nice interactive command line session feeling. 

The user interacts with us via writing commands. We have to read the input and parse the commands in a safe way. 

Then we evaluate the command, e.g. change some config setting or start the simulation. 
There we have to also check for invalid input commands or wrong type of config variable. 

After that we print a feedback from each command, e.g. "Command ran with exit code X" or "Successfully changed variable <xyz> to value 123". 

A first pseudocode draft of the loop: 
```c
#include <stdio.h>

void handle_user_input(input) {
    switch (input) {
        case "xyz":
            do_smth()
        case "...":
            do_smth_else()
        case "exit", "q", "quit", ...:
            exit_program()
        case "start":
            start_simulation()
        default: 
            whatever()
    }
}



int main() {
    char input[256];
    printf("Hello to our program");

    while (true) {
        printf("> "); // start of user input line
        read_user_input(); // however to do this
        handle_user_input(); // however to do this
        // Also break condition for while loop in handle_user_input() needed
        give_feedback(errno);
    }
    return 0;
}
```
Change of plans: I found out that c has function pointers. 

Since switch case doesn't work for strings (only for ints and chars which are kinda the same thing) we have to choose another way. 

For that we build up a table of key-value pairs with key as string-type and value as function-type. 

We save these pairs in an array and for each input we check each pair until we find a match and run the specified function. 

It had to be split into print functions with void return type and config changers with return type int, because different return types and argument types are just dumb to implement. We will first iterate over the print table and then over the config changer table. 

## How do we get to config values?
Since we have to be able to first of all change the configs (in configs.c) and also print them we have to have a way of reading them. 

This has to be discussed as it is not clear to me. 

I see two possibilities: 

1. We define getter and setter functions for each variable, in which we have to give over a pointer to the variable or
2. We define the variables/structs as global variables so that we can directly change them. 