//
//  sampler.c
//  Controller
//
//  Created by Victor Gabriel Maraccini & Denis Isidoro de Franca
//  Copyright © 2016 TCC. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include "sampler.h"
#include "controller.h"
#include "controlLoop.h"
#include "semaphore.h"

extern sem_t cts;

///Sampling time (seconds)
#define TS 0.1
#define MICROSECONDS_TO_SECONDS 1E6

void main_sampler() {
    
    for (;;) {
        usleep(MICROSECONDS_TO_SECONDS * TS);
        
        double output = controlLoop(currentVelocity, leaderVelocity/100, maxVelocity/100, leaderDistance/100);
        double pedal = controller(output);
     
        //Convert value to byte
        pedalByte = (int)(pedal*100) & 0xFF;
        
        //Signal that new values are available to broadcast
        sem_post(&cts);
    }
}

