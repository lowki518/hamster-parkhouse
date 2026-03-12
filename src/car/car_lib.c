#include "../../include/car_lib.h"
#include "../../include/data_types.h"
#include <stdio.h>
#include <stdlib.h>

/*
@brief parks a car in an allocated parking cell

@param[1] car A pointer to the car that needs to be parked
@param[2] cell A pointer to the parking cell where the car should be parked
@param[3] time The current time

@return  void
*/
void park_car (t_Car *car, t_Parking_Cell *cell, t_Time time) {
    cell->p_car_in_cell = car;
    car->start_parking_time = time;
}


/*
@brief Unparks a car from its allocated parking cell

@param[1] cell A pointer to the cell of the car
@param[2] park A pointer to the car park

@return void
*/
void unpark_car(t_Parking_Cell *cell, t_Car_Park *park) {

    free(cell->p_car_in_cell);
    cell->p_car_in_cell = NULL;

    park->free_parking_cells++;

}

/*
@brief unparks all cars that need to be unparked based on the time

@param[1] car A pointer to the car to park
@param[2] park A pointer to the car park
@param[3] time The current time

@return void
*/
void park_car_in_park (t_Car *car, t_Car_Park *park, t_Time time) {
    t_Parking_Cell *cell = park->first_parking_cell;

    for (int i = 0; i < park->max_parking_cells; i++) {
        if(cell->p_car_in_cell == NULL) {
            park_car(car, cell, time);
            return;
        }
        cell = cell->pNext;
    }
    
}

/*
@brief unparks all cars that need to unparked at the current time

@param[1] park A pointer to the car park
@param[2] time The current time

@return void
*/
void unpark_cars_in_park (t_Car_Park *park, t_Time time) {
    t_Parking_Cell *cell = park->first_parking_cell;

    for (int i = 0; i < park->max_parking_cells; i++) {
        t_Car *car = cell->p_car_in_cell;
        if(car != NULL) {
            if(check_parking_time(car, time)) {
                unpark_car(cell, park);
            }
        }
        cell = cell->pNext;
    }
}


/*
@brief Generates the arrival of a car

The generating of a new car is based on the users choice of arrival chances in percentage.

@param[1] percentage The users choice of arrival chances
@param[2] id A pointer to the ID for the car
@param[3] max_parking The maximum allowed parking time

@return the car or not
*/
t_Car * car_arrives (float percentage, unsigned int *id, t_Time max_parking) {
    
    //generates random number between 0 and 10000 abd converts percentage to int
    int rand_v = rand() % 10001;
    if (rand_v <= (int) (percentage*100)) {
        t_Car *new_car = malloc(sizeof(t_Car));
        if(!new_car) {
            return NULL;
        }

        new_car->id = *id;
        (*id)++;

        // generates a random value between 0 and max_parking
        new_car->parking_time = rand() % (max_parking + 1);

        // generates a random value between 0 and 26 (Number of car brands)
        // -> generates a random brand for the car
        new_car->brand = rand() % (27);

        return new_car;
    }
    return NULL;
}

/*
@brief Checks if parking time of a car is over 

@param[1] car The car to check
@param[2] time The time of the parked car to check

@return 1 if a car needs to leave, 0 if it needs to stay
*/
int check_parking_time(t_Car *car, t_Time time) {
    if(car->parking_time + car->start_parking_time <= time) {
        return TRUE;
    }
    return FALSE;
}

/*
@brief gets the current most parked brand

@param[1] park A pointer to the car park

@return the most parked brand
*/
Car_Brand get_most_parked_brand(t_Car_Park *park) {
    t_Parking_Cell *cell = park->first_parking_cell;
    int *brand = calloc(27, sizeof (brand)); //allocates memory for 26 brands

    while (cell != NULL) {
        //increases the value for each brand if the car is of the brand type
        t_Car *car = cell->p_car_in_cell;
        if(car) {
            *(brand + car->brand) += 1;
        }
        cell = cell->pNext;
    }

    // gets the index of the highest value 
    int max_index = 0;
    for (int i = 0; i < 27; i++) {
        if (brand[i] > brand[max_index]) {
            max_index = i;
        }
    }


    free(brand);

    return max_index;

}

/*
@brief returns the name of the brand by its enum code

@param[1] brand_numb The number of the brand

@return the name of the brand
*/
char* get_brand_by_number(Car_Brand brand_numb) {

    switch (brand_numb) {

        case 0:
            return "BMW";
        case 1:
            return "VW";
        case 2:
            return "SKODA";
        case 3:
            return "RENAULT";
        case 4:
            return "CITROEN";
        case 5:
            return "TOYOTA";
        case 6:
            return "AUDI";
        case 7:
            return "MERCEDES";
        case 8:
            return "PEUGEOT";
        case 9:
            return "MAYBACH";
        case 10:
            return "ALPINA";
        case 11:
            return "NISSAN";
        case 12:
            return "HONDA";
        case 13:
            return "SAAB";
        case 14:
            return "VOLVO";
        case 15:
            return "OPEL";
        case 16:
            return "DACIA";
        case 17:
            return "FORD";
        case 18:
            return "FIAT";
        case 19:
            return "ALFA ROMEO";
        case 20:
            return "PORSCHE";
        case 21:
            return "KIA";
        case 22:
            return "HYUNDAI";
        case 23:
            return "MAZDA";
        case 24:
            return "SEAT";
        case 25:
            return "SUBARU";
        case 26:
            return "SUZUKI";
    
        default:
            return "";
    
    }
}