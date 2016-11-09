//
//  main.c
//  TCC

#include "Comum.h"

#include "Servidor UDP/Servidor-UDP.h"
#include "Controlador/Controlador.h"

#include <stdio.h>

//Definicoes

//Variaveis (valores)

volatile int pedal;
volatile int maxSpeed;
volatile int distance;

//Threads

pthread_t jsonSaveThread;
pthread_t udpMaxSpeedThread;
pthread_t udpDistanceThread;
pthread_t controladorThread;

//Locks

pthread_mutex_t valorLock;
pthread_mutex_t paramLock;

int main(int argc, const char * argv[])
{
    //Criar threads
    pthread_create(&udpMaxSpeedThread, NULL, (void*) main_udpMaxSpeed, NULL);
    pthread_create(&udpDistanceThread, NULL, (void*) main_udpDistance, NULL);
    pthread_create(&controladorThread, NULL, (void*) main_controlador, NULL);
    pthread_create(&jsonSaveThread, NULL, (void*) main_jsonSave, NULL);
    
    //Seguir as threads
    pthread_join(udpMaxSpeedThread, NULL);
    pthread_join(udpDistanceThread, NULL);
    pthread_join(controladorThread, NULL);
    pthread_join(jsonSaveThread, NULL);
    
    return OK;
}
