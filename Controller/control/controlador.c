#include <stdio.h>
#include "controlador.h"
#include "controller.h"
#include "controlLoop.h"
#include "vehicle.h"

#define INITIAL_VELOCITY 14 / 3.6 //14 km/h is the smallest velocity allowed by 3rd gear
#define MAX_VELOCITY 60 / 3.6

void main_controlador() {
    
    for (;;) {
        
        // car.distance = Distância do carro da frente (m)
        // car.velocity = velocidade atual em relação ao chão (m/s);
        // leaderVelocity: Velocidade relativa do carro da frente (m/s)
        // maxVelocity: Velocidade da via em relação ao chão (m/s)
        
        car.distance = leaderDistance/100;
        
        double output = controlLoop(car.velocity, leaderVelocity/100, maxVelocity/100, car.distance);
        double pedal = controller(output);

        updateCar(pedal);

        currentDistance = car.distance;
        currentVelocity = car.velocity;
        
        pedalByte = (pedal*100) * 0xFF;
    }
}

