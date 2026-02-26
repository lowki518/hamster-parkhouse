# IO Module
The car park simulation (cps) is configurable via the following CLI:

## Start Menu
After starting the program configurations can be made via command line. 
From the menu of variables to be changed (shown with their respective data types) one can enter the variable name and the new value. E.g: (TODO)
There is a default state that can be reverted by entering 
$ variable_name default


The following variables can be configured (with type):
- max_car_cells (unsigned int)
- max_parking_time in steps (unsigned int)
- simulation_time in time_steps (unsigned int)
- car_probability (float)
- random_seed (int)


## How will it work? 
Depending on how the whole program is planned. 
If we use a while-loop for everything we can stop the loop by waiting for user input. 
Then we parse the input and either start the simulation or change something in the configurations. 
After running the simulation we return back to the start menu. 

After some research (Mistral AI lol) I found the straightforward REPL-idea (Read-Eval-Print Loop)
for a nice interactive command-line session. 
How it could look: 
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
    }
    return 0;
}


```
Remember this is just some pseudocode written in under 5 minutes, NOT working c code. 
However it should provide a good example of how it could look like for a nice session like approach of our program as CLI. 
It should also be pretty maintainable and scalable as we just need to configure the switch_case block in handle_user_input.
