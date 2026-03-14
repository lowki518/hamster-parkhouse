#include "../../include/data_types.h"
#include "../../include/file_manager_lib.h"
#include "../../include/car_lib.h"
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>

/*
@brief Gets the number (Simulation_X.txt) of the last file in results.

@param[1] folder The path to the folder to search in

@return Returns the number of the last file
*/
int get_new_file_number(char *folder) {
    
    DIR *dir = opendir(*folder);  //go to folder

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

@param[1] path The path to the folder to save the file in
@param[2] new_file_number The number of the File to write.
@param[3] sim_duration The total amount of time steps done.
@param[4] parking_cells The amount of parking_spaces.
@param[5] max_parking_duration The maximum parking time for the cars.
@param[6] new_car_prob The probability of a new car arriving (in percent).
@param[7] max_cars_arriving How many cars can arrive in a simulation step.
@param[8] seed The seed used to generate randomness within this simulation.

@return void
*/
void create_new_file_with_head_data (char *path,int new_file_number, t_Time sim_duration, int parking_cells, t_Time max_parking_duration, float new_car_prob, int max_cars_arriving, unsigned int seed) {
    char filename[60];

    sprintf(filename, "%sSimulation_%d.txt", *path, new_file_number);

    FILE *fptr = fopen(filename, "w");
    if (fptr == NULL) {
        printf("Error creating file.\n");
        return NULL;
    }   
    
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


    fprintf(fptr, "\n");
    fprintf(fptr, "Simulation Nr. %i:\n", new_file_number);
    fprintf(fptr, "\n");

    fprintf(fptr, "|%15s|%15s|%19s|%15s|%15s|%15s|\n", sim_duration_s, parking_cells_s, max_parking_duration_s, new_car_prob_s, max_cars_arriving_s, seed_s);
    fprintf(fptr, "|%15i|%15i|%19i|%14.2f%%|%15i|%15i|\n", sim_duration, parking_cells, max_parking_duration, new_car_prob, max_cars_arriving, seed);
    
    fprintf(fptr, "\n\n");
    fprintf(fptr, "|%15s|%15s|%19s|%15s|%15s|%15s|%15s|\n", curr_step_s, curr_cars_s, avg_time_s, q_len_s, full_house_s, tot_sim_car_s, most_brand_s);
    
    fclose(fptr);
}

/*
@brief Appends the data for every simulated step to the file.

@param[1] path The path to the directory of the file
@param[2] new_file_number The number of the File to write in.
@param[3] timestep The current timestep.
@param[4] cars_parked The amount of cars currently parked.
@param[5] avg_parking_time The average parking time of the cars.
@param[6] q_len The current length of the queue.
@param[7] full_house_steps The amount of steps the parking garage was full.
@param[8] tot_cars_simulated The total amount of cars that was simulated.
@param[9] most_brand The car brand that parked the most.

@return void
*/
void append_data_per_timestep (char *path, int new_file_number, t_Time timestep, int cars_parked, float avg_parking_time, int q_len, int full_house_steps, int tot_cars_simulated, Car_Brand most_brand) {
    char filename[60];

    sprintf(filename, "%sSimulation_%d.txt", *path, new_file_number);

    FILE *fptr = fopen(filename, "a");     
    if (fptr == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }

    fprintf(fptr, "|%15i|%15i|%15.2f|%15i|%15i|%15i|%15s|\n", timestep, cars_parked, avg_parking_time, q_len, full_house_steps, tot_cars_simulated, get_brand_by_number(most_brand));
    
    fclose(fptr);
}


/*
@brief opens simulation from path with file number to read

@param[1] path The path to the file
@param[2] file_number The number of the simulation

@return the pointer to the file stream
*/
FILE *open_file_r(const char* path, int file_number) {
    char filename[60];

    sprintf(filename, "%sSimulation_%d.txt", path, file_number);

    FILE *fptr = fopen(filename, "r");     
    if (fptr == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }

    return fptr;
}

/*
@brief opens simulation from path with file number to append

@param[1] path The path to the file
@param[2] file_number The number of the simulation

@return the pointer to the file stream
*/
FILE *open_file_a(const char* path, int file_number) {
    char filename[60];

    sprintf(filename, "%sSimulation_%d.txt", path, file_number);

    FILE *fptr = fopen(filename, "a");     
    if (fptr == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }

    return fptr;
}