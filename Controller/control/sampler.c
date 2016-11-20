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
#include <time.h>

extern sem_t cts;

///Sampling time (seconds)
#define TS 0.1
#define MICROSECONDS_TO_SECONDS 1E6

void main_sampler() {
    //int sampleCount = 0;
    time_t ltime;
    ltime = time(NULL);

    struct timeval tv;
    struct timeval prevTv;

    char fileName[100];
    sprintf(fileName, "log-%lu.txt", ltime);
    FILE *log = fopen(fileName, "w");

    long stepCount = 0;
    leaderDistance = 75*100;
    leaderVelocity = (100*50/3.6);

    for (;;) {
        stepCount++;
        usleep(MICROSECONDS_TO_SECONDS * TS);

        //if (sampleCount > 45) { accEnabled = 1; }
        //else { sampleCount++; }

        leaderVelocity -= stepCount > 35*10 ? 30*TS : 0;
        if (leaderVelocity > 2600) { leaderVelocity = 2600; }
        else if (leaderVelocity < 1111) { leaderVelocity = 1111; }

        leaderDistance += (leaderVelocity - currentVelocity/3.6)*TS;

        double currentVelocity2 = (currentVelocity / 100.0) / 3.6;
        double leaderVelocity2 = leaderVelocity / 100.0;
        double maxVelocity2 = maxVelocity / 3.6;
        double leaderDistance2 = leaderDistance / 100.0;
        double output = controlLoop(currentVelocity2, leaderVelocity2, maxVelocity2, leaderDistance2);
        double pedal = controller(output);

        //Report a value from 0 to 255
        if (pedal*2.55 > 255) { pedalByte = 255; }
        else if (pedal*2.55 < 0) { pedalByte = 0; }
        else { pedalByte = pedal*2.55; }

        //Convert value to byte
        unsigned long long currTime = tv.tv_usec + tv.tv_sec * 1e6;
        unsigned long long diff = currTime - (prevTv.tv_usec + prevTv.tv_sec * 1e6);

        gettimeofday(&tv, NULL);

        ltime = time(NULL);
        printf("(%d,%d,%d,%d):%f,%d-%llu\n", currentVelocity, leaderVelocity, maxVelocity, leaderDistance, pedal, pedalByte, diff);
        fflush(stdin);

        fprintf(log, "%d,%d,%d,%d,%f,%d,%llu\n", currentVelocity, leaderVelocity, maxVelocity, leaderDistance, pedal, pedalByte, currTime);
        fflush(log);

        gettimeofday(&prevTv, NULL);
        //Signal that new values are available to broadcast
        sem_post(&cts);
    }
}

