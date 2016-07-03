//
//  main.c
//  Controlador
//
//  Created by Victor Maraccini on 10/22/16.
//  Copyright © 2016 TCC. All rights reserved.
//

#include <stdio.h>
#include "controller.h"
#include "controlLoop.h"
#include "vehicle.h"

#define INITIAL_VELOCITY 14 / 3.6 //14 km/h is the smallest velocity allowed by 3rd gear
#define LEADER_VELOCITY 50 / 3.6
#define LEADER_DISTANCE 20
#define MAX_VELOCITY 60 / 3.6

typedef struct DATA {
    double carVelocity;
    double leaderVelocity;
    double distance;
} Data;

int main(int argc, const char * argv[]) {
    
    car.distance = LEADER_DISTANCE;
    
    for (int i = 0; i < 10000; i++) {
        double output = controlLoop(car.velocity, LEADER_VELOCITY, MAX_VELOCITY, car.distance);
        double pedal = controller(output);
        updateCar(pedal);
        printf("--------------\n");
        printf("Current output: %f\n", pedal);
        printf("Current car distance: %f\n", car.distance);
        printf("Current car velocity: %f\n", car.velocity);
        printf("Leader car velocity: %f\n", LEADER_VELOCITY);
        printf("--------------\n");
    }
}

