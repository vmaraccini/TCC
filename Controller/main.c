//
//  main.c
//  TCC

#include "Comum.h"

#include "control/controlador.h"
#include "udp/Cliente-UDP.h"
#include "json/json.h"
#include "rs232/rs232.h"
#include "semaphore.h"

#include <stdio.h>

//----------- Variables -----------
volatile char pedalByte;
volatile int maxVelocity;
volatile int leaderDistance;
volatile int leaderVelocity;
volatile int currentDistance;
volatile int currentVelocity;

//----------- Synchronization variables -----------
///Clear to send: Synchronizes control loop with UDP comm.
sem_t cts;

//----------- Threads -----------
pthread_t jsonSaveThread;
pthread_t udpMaxVelocityThread;
pthread_t udpDistanceThread;
pthread_t controladorThread;
pthread_t rs232Thread;

int main(int argc, const char * argv[]) {
    //Create threads
    pthread_create(&udpMaxVelocityThread, NULL, (void*) main_udpMaxVelocity, NULL);
    pthread_create(&udpDistanceThread, NULL, (void*) main_udpDistance, NULL);
    pthread_create(&controladorThread, NULL, (void*) main_controlador, NULL);
    pthread_create(&jsonSaveThread, NULL, (void*) main_json, NULL);
    pthread_create(&rs232Thread, NULL, (void*) main_rs232_read, NULL);
    pthread_create(&rs232Thread, NULL, (void*) main_rs232_write, NULL);
    
    //Join
    pthread_join(udpMaxVelocityThread, NULL);
    pthread_join(udpDistanceThread, NULL);
    pthread_join(controladorThread, NULL);
    pthread_join(jsonSaveThread, NULL);
    pthread_join(rs232Thread, NULL);
    
    return OK;
}
