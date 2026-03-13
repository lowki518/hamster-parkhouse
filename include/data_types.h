// Definition of needed data types and structs. 


#ifndef DATA_TYPES_H
#define DATA_TYPES_H


// Boolean Values
#define TRUE 1

#define FALSE 0



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


// Car struct
// Unique ID
// Brand of the car
// How long the car will park
// When the car started parking
typedef struct {
    int id;
    Car_Brand brand; 
    t_Time parking_time;
    t_Time start_parking_time;
} t_Car;

// Parking_Cell Struct
// Pointer to the car in the cell
// Pointer to next cell
typedef struct Parking_Cell{
    t_Car *p_car_in_cell;
    struct Parking_Cell *pNext;
} t_Parking_Cell;

// Car_Park Struct
// Number of parking cells
// Number of free parking cells
// Pointer to first parking cell 
typedef struct {
    int max_parking_cells; 
    int free_parking_cells;
    t_Parking_Cell *first_parking_cell;
} t_Car_Park;



// Car_Node Struct
// Pointer to cointained car
// Pointer to next Node
typedef struct t_Car_Node {    
    t_Car *pCar;
    struct t_Car_Node *pNext; 
} t_Car_Node;


// Queue struct
// Length of queue
// Pointer to first node
// Pointer to last node
typedef struct {
    int q_length; 
    t_Car_Node *p_first_pos;
    t_Car_Node *p_last_pos;
} t_Queue;



#endif