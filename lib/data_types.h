// Definition of needed data types and structs. 


#ifndef DATA_TYPES_H
#define DATA_TYPES_H


// Boolean Values
#define TRUE 1
#define FALSE 2


// Used for Statistics
typedef enum {
    BMW, VW, SKODA, RENAULT, CITROEN, 
    TOYOTA, AUDI, MERCERDES, PEUGEOT, 
    MAYBACH, ALPINA, NISSAN, HONDA, 
    SAAB, VOLVO, OPEL, DACIA, FORD, 
    FIAT, ALFA_ROMEO, PORSCHE, KIA, 
    HYUNDAI, MAZDA, SEAT, SUBARU, SUZUKI
} Car_Brand;


// Time as unsigned int for better overview
typedef unsigned int t_Time;


// Car_Park Struct
// Number of parking cells
// Number of free parking cells
// Maximum parking duration
// Pointer to first parking cell 
typedef struct {
    int max_parking_cells; 
    int free_parking_cells;
    t_Time max_parking_duration; 
    t_Parking_Cell *first_parking_cell;
} t_Car_Park;


// Car struct
// Unique ID
// Cell index
// Brand of the car
// How long the car will park
// When the car started parking
typedef struct {
    int id;
    int cell_index; // should be -1 when car is in queue
    Car_Brand brand; 
    Time_Unit parking_time;
    Time_Unit start_parking_time;
} t_Car;


// Car_Node Struct
// Pointer to cointained car
// Pointer to next Node
typedef struct {    
    t_Car *pCar;
    struct t_carnode *pNext; 
} t_Car_Node;


// Queue struct
// Length of queue
// Pointer to first node
typedef struct {
    int q_length; 
    t_carnode *first_pos; 
} t_Queue;


// Parking_Cell Struct
// (Boolean) Is the cell free
// Pointer to the car in the cell
typedef struct {
    int is_free;
    t_Car *car_in_cell;
} t_Parking_Cell;


#endif