//
//  main.c
//  Lumiar-Final
//
//  Created by Victor Maraccini on 6/19/16.
//
//

#include "Comum.h"

#include "Servidor UDP/Servidor-UDP.h"
#include "Servidor WEB/Servidor-WEB.h"
#include "Controlador/Controlador.h"

#include <stdio.h>

//Definicoes

//Variaveis (valores)

volatile int temperatura;
volatile int luminosidade;
volatile int intensidade;

//Parametros de controle

volatile char estado = 0; // 0 = Desligado / 1 = Ligado
volatile char modo = 0; // 0 = Manual / 1 = Automatico

volatile int limiar_luminosidade = 50; // 0 - 100
volatile int limiar_temperatura = 50; // 0 - 100

//Threads

pthread_t webThread;
pthread_t udpThread;
pthread_t controladorThread;

//Locks

pthread_mutex_t valorLock;
pthread_mutex_t paramLock;

int main(int argc, const char * argv[])
{
    //Criar threads
    pthread_create(&webThread, NULL, (void*) main_web, NULL);
    pthread_create(&udpThread, NULL, (void*) main_udp, NULL);
    pthread_create(&controladorThread, NULL, (void*) main_controlador, NULL);
    
    //Seguir as threads
    pthread_join(webThread, NULL);
    pthread_join(udpThread, NULL);
    pthread_join(controladorThread, NULL);
    
    return OK;
}
