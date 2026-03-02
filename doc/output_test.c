/*
@brief Used for visualization of the planned statistics output
*/



#include <stdio.h>

int main() {

    

    char nb_steps_s[] = "Simualted Steps";
    char nb_cells_s[] = "Parking Spaces";
    char max_time_s[] = "Max. Time";
    char new_car_prob_s[] = "New Car Prob.";
    char max_new_cars_s[] = "Max. New Cars";
    char seed_s[] = "Seed";
    
    int simulation_number = 1;

    int nb_steps_i = 1000;
    int nb_cells_i = 400;
    int max_time_i = 30;
    float new_car_prob_f = 82.54f;
    int max_new_cars_i = 3;
    unsigned int seed_i = 892347;

    char curr_step_s[] = "Current Step";
    char curr_cars_s[] = "Parked Cars";
    char avg_time_s[] = "Avg. Time";
    char q_len_s[] = "Queue Length";
    char full_house_s[] = "Full Garage";
    char tot_sim_car_s[] = "Cars Simulated";
    char most_brand_s[] = "Most Brand";

    int curr_step_i[] =  {0,1,2,3,4,5,6,7,8,9};
    int curr_cars_i[] =  {0,1,3,5,7,6,8,9,9,11};
    float avg_time_f[] = {0,0,0,0,0,6,6,6,6,6};
    int q_len_i[] =      {0,0,0,0,0,0,0,0,0,0};
    int full_house_i[] = {0,0,0,0,0,0,0,0,0,0};
    int tot_sim_car_i[] = {0,1,3,5,7,7,9,10,0,12};
    char most_brand_sd[10][12] = {"BMW", "MERCEDES", "VOLVO", "OPEL", "DACIA",
        "FORD", "FIAT", "ALFA_ROMEO", "PORSCHE", "KIA"};




    printf("\n");
    printf("Simulation Nb. %i:\n", simulation_number);
    printf("\n");

    printf("|%15s|%15s|%15s|%15s|%15s|%10s|\n", nb_steps_s, nb_cells_s, max_time_s, new_car_prob_s, max_new_cars_s, seed_s);
    printf("|%15i|%15i|%15i|%14.2f%%|%15i|%10i|\n", nb_steps_i, nb_cells_i, max_time_i, new_car_prob_f, max_new_cars_i, seed_i);
    
    printf("\n");

    printf("|%15s|%15s|%15s|%15s|%15s|%15s|%15s|\n", curr_step_s, curr_cars_s, avg_time_s, q_len_s, full_house_s, tot_sim_car_s, most_brand_s);
    
    for (int i = 0; i < 10; i++) {
        printf("|%15i|%15i|%15.2f|%15i|%15i|%15i|%15s|\n", curr_step_i[i], curr_cars_i[i], avg_time_f[i], q_len_i[i], full_house_i[i], tot_sim_car_i[i], most_brand_sd[i]);
    }
    printf("\n");
    return 0;
}