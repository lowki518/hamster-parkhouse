// Definition of needed data types and structs. 


#ifndef DATA_TYPES_H
#define DATA_TYPES_H
// Boolean because wtf c
// maybe using stdbool.h instead but will stay at this for now. 
#define TRUE 1
#define FALSE 2


// Maybe an idea also for statistics? ^^
typedef enum {
    BMW, VW, SKODA, RENAULT, CITROEN, 
    TOYOTA, AUDI, MERCERDES, PEUGEOT, 
    MAYBACH, ALPINA, NISSAN, HONDA, 
    SAAB, VOLVO, OPEL, DACIA, FORD, 
    FIAT, ALFA_ROMEO, PORSCHE, KIA, 
    HYUNDAI, MAZDA, SEAT, SUBARU, SUZUKI,
} Car_Brand;


// time
// - time unit -> variable
// should it be a struct actually? 
 
typedef unsigned int t_Time;

typedef struct {
    int step_number; // I'm stuck
} Time_Unit;


// parking cell
// - boolean: free or not free
// - id in array for better administration? 
typedef struct {
    int is_free; // Is either TRUE or FALSE
    //?: unsigned int id;
} t_Parking_Cell;


// car park
// - # parking cells
// - Array of parking cells
// - maybe also # free parking cells? 
typedef struct {
    int max_parking_cells; // should be global variable or not? 
    // similarly max parking duration could be fitting here?
    t_Parking_Cell * administration;
} t_Car_Park;



// cars
// - id
// - car brand
// - parking time left
// - start time of parking
typedef struct {
    int id;
    int cell_index; // if not in parking_cell value should be -1
    Car_Brand brand; // should get its own enum
    Time_Unit parking_time_left;
    Time_Unit start_parking_time;
} t_Car;

//car node for queue
//sets pointer for next car
typedef struct {    
    t_Car * pt_Car;
    struct t_carnode *pNext; 
} t_carnode;


// queue
// length of queue
// sets pointer for first car
typedef struct {
    int t_qlength; 
    t_carnode *first_car; 
} queue;




#endif