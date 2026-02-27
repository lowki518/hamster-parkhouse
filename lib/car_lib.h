#ifndef CAR_LIB_H
#define CAR_LIB

#include "data_types.h"

t_Car *new_car ();

void *park_car (t_Car *car, t_Parking_Cell *cell);



t_Car * car_arrives (float percentage);

#endif