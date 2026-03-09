#include "../data_types.h"
#include "file_manager_lib.h"
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>

/*
@brief Gets the number (Simulation_X.txt) of the last file in results.

@param[1] folder A pointer to the folder to check

@return Returns the number of the last file
*/
int get_new_file_number(const char *folder) {
    const char *folder = "../data";
    
    DIR *dir = opendir(folder);  //go to folder

    struct dirent *past_sim;  //pointer to files in folder
    int max_file_nr = 0;

    if (dir == NULL) {
        printf("The given directory could not be opened");
        return -1;
    }

    while ((past_sim = readdir(dir)) != NULL) {     //goes through all files in "data"
        int file_nr;
        
        if (sscanf(past_sim->d_name, "Simulation_%d.txt", &file_nr) == 1) {     //saves biggest nr of filenames
            if (file_nr > max_file_nr) {
                max_file_nr = file_nr;
            }
        }
    }

    closedir(dir);
    int new_file_number = max_file_nr + 1;
    return new_file_number;
}


/*
@brief Creates the new file for saving the simulation data with the sim. parameters.

@param[1] new_file_number The number of the File to write.
@param[2] sim_duration The total amount of time steps done.
@param[3] parking_cells The amount of parking_spaces.
@param[4] max_parking_duration The maximum parking time for the cars.
@param[5] new_car_prob The probability of a new car arriving (in percent).
@param[6] max_cars_arriving How many cars can arrive in a simulation step.
@param[7] seed The seed used to generate randomness within this simulation.

@return void
*/
void create_new_file_with_head_data (int new_file_number, t_Time sim_duration, int parking_cells, t_Time max_parking_duration, float new_car_prob, int max_cars_arriving, unsigned int seed) {
    char filename[50];

    sprintf(filename, "Simulation_%d.txt", new_file_number);

    FILE *fptr;                 //can I use the same fptr?
    fptr = fopen(filename, "w");
    if (fptr == NULL) {
        printf("Error creating file");
        return 1;
    }
    fclose(fptr);    
    
    char sim_duration_s[] = "Simulated Steps";
    char parking_cells_s[] = "Parking Cells";
    char max_parking_duration_s[] = "Max. Parking Time";
    char new_car_prob_s[] = "New Car Prob.";
    char max_cars_arriving_s[] = "Max. New Cars";
    char seed_s[] = "Seed";
    
    char curr_step_s[] = "Current Step";
    char curr_cars_s[] = "Parked Cars";
    char avg_time_s[] = "Avg. Time";
    char q_len_s[] = "Queue Length";
    char full_house_s[] = "Full Garage";
    char tot_sim_car_s[] = "Cars Simulated";
    char most_brand_s[] = "Most Brand";

    fptr = fopen(filename, "w");

    fprintf(fptr, "\n");
    fprintf(fptr, "Simulation Nr. %i:\n", new_file_number);
    fprintf(fptr, "\n");

    fprintf(fptr, "|%15s|%15s|%19s|%15s|%15s|%15s|\n", sim_duration_s, parking_cells_s, max_parking_duration_s, new_car_prob_s, max_cars_arriving_s, seed_s);
    fprintf(fptr, "|%15i|%15i|%19i|%14.2f%%|%15i|%15i|\n", sim_duration, parking_cells, max_parking_duration, new_car_prob, max_cars_arriving, seed);
    
    fprintf(fptr, "\n");

    fprintf(fptr, "|%15s|%15s|%19s|%15s|%15s|%15s|%15s|\n", curr_step_s, curr_cars_s, avg_time_s, q_len_s, full_house_s, tot_sim_car_s, most_brand_s);

    fclose(fptr);
    /*
    file = CREATE NEW FILE BY NUMBER
        WRITE HEADER data IN file
    CLOSE file
    */
}

/*
@brief Appends the data for every simulated step to the file.

@param[1] new_file_number The number of the File to write in.
@param[2] timestep The current timestep.
@param[3] cars_parked The amount of cars currently parked.
@param[4] avg_parking_time The average parking time of the cars.
@param[5] q_len The current length of the queue.
@param[6] full_house_steps The amount of steps the parking garage was full.
@param[7] tot_cars_simulated The total amount of cars that was simulated.
@param[8] most_brand The car brand that parked the most.

@return void
*/
void append_data_per_timestep (int new_file_number, t_Time timestep, int cars_parked, float avg_parking_time, int q_len, int full_house_steps, int tot_cars_simulated, Car_Brand most_brand) {
    FILE *fptr;

    fptr = fopen(filename, "a");     //still unnsure on how to pass the filename along to other functions
    
    
    fprintf(fptr, "|%15i|%15i|%15.2f|%15i|%15i|%15i|%15s|\n", timestep, cars_parked, avg_parking_time, q_len, full_house_steps, tot_cars_simulated, most_brand);
    fprintf(fptr, "\n");

    fclose(fptr);

    /*
    file = OPEN FILE BY NUMBER
        PRINT IN FILE : DATA
    CLOSE FILE
    */
}


/*
@brief writes the new config data into the config file.

@param[1] parking_cells The amount of parking cells.
@param[2] max_parking_duration The max. amount of timesteps a car can park.
@param[3] sim_duration The duration of the simulation.
@param[4] new_car_prob The probability of a new car arriving.
@param[5] max_cars_arriving The maximum amount of cars arriving in one step.
@param[6] seed The seed used to generate the randomness within the simulation.

@return void
*/
void write_config_data (int parking_cells, t_Time max_parking_duration, t_Time sim_duration, float new_car_prob, int max_cars_arriving, unsigned int seed) {
    char sim_duration_s[] = "Simulated Steps";
    char parking_cells_s[] = "Parking Cells";
    char max_parking_duration_s[] = "Max. Parking Time";
    char new_car_prob_s[] = "New Car Prob.";
    char max_cars_arriving_s[] = "Max. New Cars";
    char seed_s[] = "Seed";
    
    FILE *fptr; 
    
    fptr = fopen("config.txt", "w");      //edit file name and dir


    fprintf(fptr, "\n");
    fprintf(fptr, "Configured data:\n");
    fprintf(fptr, "\n");

    fprintf(fptr, "|%15s|%15s|%19s|%15s|%15s|%15s|\n", sim_duration_s, parking_cells_s, max_parking_duration_s, new_car_prob_s, max_cars_arriving_s, seed_s);
    fprintf(fptr, "|%15i|%15i|%19i|%14.2f%%|%15i|%15i|\n", sim_duration, parking_cells, max_parking_duration, new_car_prob, max_cars_arriving, seed);

    fclose(fptr);

    /*
    file = OPEN CONFIG FILE
        FOR DATA in File                        //why do we need a FOR loop? can't we just write in the file (automaticlaly deletes the old stuff anyways)
            DATA (in File) = NEW DATA
        END FOR
    CLOSE file
    */
}


/*
@brief Reads the config file

@param[1] parking_cells A pointer to where the amount of parking cells will be stored.
@param[2] max_parking_duration A pointer to where the maximum parking duration will be stored.
@param[3] sim_duration A pointer to where the duration of the simulation will be stored.
@param[4] new_car_prob A pointer to where the probability of a car arriving will be stored.
@param[5] max_cars_arriving A pointer to where the maximum amount of cars arriving in one step will be stored.
@param[6] seed A pointer to where the seed will be stored.

@return void
*/
void read_config_data (int *parking_cells, t_Time *max_parking_duration, t_Time *sim_duration, float *new_car_prob, int *max_cars_arriving, unsigned int *seed) {

    /*
    file = OPEN CONFIG FILE
        FOR DATA in File
            *data = DATA FROM FILE
        END FOR
    close file
    */

 }