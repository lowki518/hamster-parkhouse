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
After running the simulation we return back to the start menu. float

