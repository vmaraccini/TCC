#include <stdio.h>
#include "controlador.h"
#include "controller.h"
#include "controlLoop.h"
#include "vehicle.h"

#define INITIAL_VELOCITY 14 / 3.6 //14 km/h is the smallest velocity allowed by 3rd gear
#define MAX_VELOCITY 60 / 3.6

int main_controlador() {

    for (;;) {
        car.distance = leaderDistance;
        car.velocity = leaderVelocity;

        double output = controlLoop(car.velocity, leaderVelocity, maxVelocity, car.distance);
        double pedal = controller(output);

        updateCar(pedal);

        printf("--------------\n");
        printf("Current output: %f\n", pedal);
        printf("Current car distance: %f\n", car.distance);
        printf("Current car velocity: %f\n", car.velocity);
        printf("Leader car velocity: %f\n", leaderVelocity);
        printf("--------------\n");

        currentDistance = car.distance;
        currentVelocity = car.velocity;
    }
}

