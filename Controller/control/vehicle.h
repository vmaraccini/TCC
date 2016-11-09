//
//  vehicle.h
//  Controlador
//
//  Created by Victor Maraccini on 10/22/16.
//  Copyright © 2016 TCC. All rights reserved.
//

#ifndef vehicle_h
#define vehicle_h

#include <stdio.h>

typedef struct CAR {
    double distance;
    double velocity;
} Car;

extern Car car;

void updateCar(double reference);

#endif /* vehicle_h */
