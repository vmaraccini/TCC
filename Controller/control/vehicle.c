//
//  vehicle.c
//  Controlador
//
//  Created by Victor Maraccini on 10/22/16.
//  Copyright © 2016 TCC. All rights reserved.
//

#include "vehicle.h"

Car car;

#define TS 0.1

double previousVelocity = 0 / 3.6;
double leaderVelocity = 50 / 3.6;

#define DRAG .1

void updateCar(double reference) {
    double newVelocity = previousVelocity * 0.981 + 0.01323 * reference;
    car.velocity = newVelocity - DRAG;
    previousVelocity = car.velocity;
    car.distance += (leaderVelocity - car.velocity) * TS;
}
