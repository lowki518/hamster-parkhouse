// Definition of needed data types and structs. 

// Boolean because wtf c
typedef enum Boolean {
    True = 1,
    False = 0,
} Boolean;


// Maybe an idea also for statistics? ^^
typedef enum Car_Type {
    BMW, VW, SKODA, RENAULT, CITRÖN, TOYOTA,
} Car_Type;


// time
// - time unit -> variable
// - max parking time
typedef struct Time_Unit {
    float step_number; // I'm stuck
} Time_Unit;

// parking cell
// - boolean: free or not free
// - id in array for better administration? 
// Usable both as fixed time point or as time point difference
typedef struct Parking_Cell {
    Boolean is_free; // boolean in the future
    //?: unsigned int id;
} Parking_Cell;


// car park
// - # parking cells
// - Array of parking cells
// - maybe also # free parking cells? 
typedef struct Carpark {
    int max_parking_cells;
    Parking_Cell * administration;
} Carpark;



// cars
// - id
// - car type
// - parking time left
// - start time of parking
typedef struct Car {
    int id;
    Car_Type type; // should get its own enum
    Time_Unit parking_time_left;
    Time_Unit start_parking_time;
} Car;