//
//  Controlador.c
//  Lumiar-Final
//
//  Created by Victor Maraccini on 6/19/16.
//
//-

#include "Controlador.h"
//using namespace std;

#include "mraa.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

mraa_pwm_context pino_atuador = NULL; //led de iluminacao
mraa_pwm_context pino_atuador2 = NULL; // led de alerta de temperatura
mraa_platform_t platform;
//Cria uma variavel que representa uma porta analogica
mraa_aio_context analog_pin = NULL;

void* monitor() {
    for (;;) {
        if(estado == 1) {
            //Le os dados do sensor atraves da porta analogica instanciada
            pthread_mutex_lock(&valorLock);
            luminosidade = (mraa_aio_read(analog_pin))/10.f;
            pthread_mutex_unlock(&valorLock);
            usleep(5000);
        }
    }
    
}

void* lum_controller() {
    for(;;) {
        if(estado == 1) {
            //manual mode
            if(modo == 0) {
                mraa_pwm_write(pino_atuador, intensidade/100.f);
            }
            //automatic mode
            else if (modo == 1 && luminosidade < limiar_luminosidade) {
                mraa_pwm_write(pino_atuador, 1 - luminosidade/100.f);
            }
            else mraa_pwm_write(pino_atuador, 0);
        }
        else mraa_pwm_write(pino_atuador, 0);
        
        usleep(5000);
    }
}

void* temp_controller() {
    for(;;) {
        if(estado == 1) {
            if(temperatura > limiar_temperatura) {
                mraa_pwm_write(pino_atuador2, 100);
                usleep(3000);
                mraa_pwm_write(pino_atuador2, 0);
                usleep(3000);
            }
            else mraa_pwm_write(pino_atuador2, 0);
        }
        else mraa_pwm_write(pino_atuador2, 0);
    }
}

void main_controlador()
{
    platform = mraa_get_platform_type();
    
    switch (platform) {
        case MRAA_INTEL_GALILEO_GEN2:
            //onde 0 eh referente a porta Analagica alocada
            analog_pin = mraa_aio_init(0);
            pino_atuador = mraa_pwm_init(5);
            pino_atuador2 = mraa_pwm_init(6);
            mraa_pwm_enable(pino_atuador, 1);
            mraa_pwm_enable(pino_atuador2, 1);
            break ;
        default:
            fprintf(stderr, "Unsupported platform, exiting");
            return ;//MRAA_ERROR_INVALID_PLATFORM;
    }
    //Imprime uma mensagem de erro caso a inicializacao nao tenha acontecido
    if(analog_pin == NULL){
        fprintf(stderr, "MRAA couldn't initialize AIO, exiting");
        return ;//MRAA_ERROR_UNSPECIFIED;
    }
    
    pthread_t mon;
    pthread_t cont;
    pthread_t temp_cont;
    
    pthread_create(&mon, NULL, monitor, NULL);
    pthread_create(&cont, NULL, lum_controller, NULL);
    //pthread_create(&temp_cont, NULL, temp_controller, (void *)&args);
    
    pthread_join(mon, NULL);
    pthread_join(cont, NULL);
    //pthread_join(temp_cont, NULL);
    
    return ;//MRAA_SUCCESS;
    
}
