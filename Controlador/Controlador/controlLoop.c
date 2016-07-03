//
//  controlLoop.c
//  Controlador
//
//  Created by Victor Maraccini on 10/22/16.
//  Copyright © 2016 TCC. All rights reserved.
//

#include "controlLoop.h"

#define OPERATION_MODE_ACC 0
#define OPERATION_MODE_CC 1

#define OPERATION_MODE_HYSTERESIS_LEFT 0.1
#define OPERATION_MODE_HYSTERESIS_RIGHT 0.1

char currentMode = OPERATION_MODE_ACC;
//Variables

//Prototypes
double safeDistance(double currentVelocity);
double referenceVelocity(double leaderVelocity, double safeDistance, double currentDistance);
double pickVelocity(double constantDistanceVelocity, double desiredVelocity);

double controlLoop(double currentVelocity, double leaderVelocity, double desiredVelocity, double distance) {
    double desiredDistance = safeDistance(currentVelocity);
    double constantDistanceVelocity = referenceVelocity(leaderVelocity, desiredDistance, distance);
    return pickVelocity(constantDistanceVelocity, desiredVelocity) - currentVelocity;
}

double referenceVelocity(double leaderVelocity, double safeDistance, double currentDistance) {
    return leaderVelocity - 0.224 * (safeDistance - currentDistance);
}

double safeDistance(double currentVelocity) {
    return 2*currentVelocity + 1;
}

double pickVelocity(double constantDistanceVelocity, double desiredVelocity) {
    double difference = constantDistanceVelocity - desiredVelocity;
    if (currentMode == OPERATION_MODE_ACC && difference >= -OPERATION_MODE_HYSTERESIS_LEFT) {
        currentMode = OPERATION_MODE_CC;
    } else if (currentMode == OPERATION_MODE_CC && difference < OPERATION_MODE_HYSTERESIS_RIGHT) {
        currentMode = OPERATION_MODE_ACC;
    }
    printf("Current mode: %d", currentMode);
    return currentMode == OPERATION_MODE_ACC ? constantDistanceVelocity : desiredVelocity;
}
