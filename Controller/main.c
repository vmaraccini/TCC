//
//  main.c
//  TCC

#include "Comum.h"

#include "control/sampler.h"
#include "udp/UDP-Client.h"
#include "json/json.h"
#include "rs232/rs232.h"
#include "semaphore.h"

#include <stdio.h>

//----------- Variables -----------
///Single byte representing the pedal position (0-100%) Unit: None
volatile char pedalByte;
///Single byte representing the maximum allowed velocity (0-255) Unit: km/h
volatile char maxVelocity;
///32-bit Integer representing the leader distance (0-2^16) Unit: cm
///@discussion: A distance of 2^16 means no detection.
volatile int leaderDistance;
///32-bit Integer representing the leader velocity. Unit: cm/s
///@discussion: A distance of 2^16 means no detection.
volatile int leaderVelocity;
///16-bit Integer representing the vehicle's current velocity (0-2^16) Unit: dm/h
volatile int currentVelocity;

//----------- Synchronization variables -----------
///Clear to send: Synchronizes control loop with RS232 comm.
sem_t cts;

//----------- Threads -----------
pthread_t jsonSaveThread;
pthread_t udpMaxVelocityThread;
pthread_t udpDistanceThread;
pthread_t controllerThread;
pthread_t rs232Thread;

int main(int argc, const char * argv[]) {
    //Create threads
    pthread_create(&udpMaxVelocityThread, NULL, (void*) main_udpMaxVelocity, NULL);
    pthread_create(&udpDistanceThread, NULL, (void*) main_udpDistance, NULL);
    pthread_create(&controllerThread, NULL, (void*) main_sampler, NULL);
    pthread_create(&jsonSaveThread, NULL, (void*) main_json, NULL);
    pthread_create(&rs232Thread, NULL, (void*) main_rs232, NULL);
    
    //Join
    pthread_join(udpMaxVelocityThread, NULL);
    pthread_join(udpDistanceThread, NULL);
    pthread_join(controllerThread, NULL);
    pthread_join(jsonSaveThread, NULL);
    pthread_join(rs232Thread, NULL);
    
    return OK;
}
