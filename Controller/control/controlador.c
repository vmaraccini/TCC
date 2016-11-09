#include <stdio.h>
#include <unistd.h>
#include "controlador.h"
#include "controller.h"
#include "controlLoop.h"
#include "semaphore.h"

extern sem_t cts;

#define INITIAL_VELOCITY 14 / 3.6 //14 km/h is the smallest velocity allowed by 3rd gear
#define MAX_VELOCITY 60 / 3.6

#define TS 0.1 //Sampling time (seconds)
#define MICROSECONDS_TO_SECONDS 1E6

void main_controlador() {
    
    for (;;) {
        usleep(MICROSECONDS_TO_SECONDS * TS);
        
        // car.distance = Distância do carro da frente (m)
        // car.velocity = velocidade atual em relação ao chão (m/s);
        // leaderVelocity: Velocidade relativa do carro da frente (m/s)
        // maxVelocity: Velocidade da via em relação ao chão (m/s)
        
        double output = controlLoop(currentVelocity, leaderVelocity/100, maxVelocity/100, leaderDistance/100);
        double pedal = controller(output);
     
        //Report a value from 0 to 255
        pedalByte = (int)(pedal*100) & 0xFF;
        
        //Signal that new values are available to broadcast
        sem_post(&cts);
    }
}

