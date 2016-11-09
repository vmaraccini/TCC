//
//  main.c
//  ClienteServidor
//
//  Created by Victor Gabriel Maraccini & Denis Isidoro de Franca
//  Copyright © 2016 Denisccini. All rights reserved.
//

#include "Comum.h"
#include "Cliente-UDP.h"

#include "network.h"
#include <stdio.h>
#include <string.h>


//Socket descriptors
int clientSd_maxVelocity; 
int clientSd_distance;

//------------------ Functions ------------------

int main_udpMaxVelocity() {
    printf("ProDAV - Cliente UDP - Max speed\n\n");
    
    iniciaCliente(&clientSd_maxVelocity, 3031);
    
    char status = OK;
    while (status == OK) {
        status = read_maxVelocity();
    }
    
    finalizaCliente(clientSd_maxVelocity);
    
    return OK;
}

int main_udpDistance() {
    printf("ProDAV - Cliente UDP - Stereo\n\n");
    
    iniciaCliente(&clientSd_distance, 3032);
    
    char status = OK;
    while (status == OK) {
        status = read_distance();
    }
    
    finalizaCliente(clientSd_distance);
    
    return OK;
}

#pragma mark - Read

char read_maxVelocity() {
    char buffer[BUFFER_LEN];
    PLACA_MSG msg;
    
    char status = recebeMensagem(buffer, clientSd_maxVelocity);
    if (status != OK) {
        printf("Erro ao interpretar mensagem placa.");
        return OK;
    } else {
        memcpy(&msg, buffer, 4);
        maxVelocity = msg.maxVelocity;
    }
    
    return OK;
}

char read_distance() {
    char buffer[BUFFER_LEN];
    STEREO_MSG msg;
    
    char status = recebeMensagem(buffer, clientSd_distance);
    if (status != OK) {
        printf("Erro ao interpretar mensagem stereo.");
        return OK;
    } else {
        memcpy(&msg, buffer, 8);
        leaderDistance = msg.distance;
        leaderVelocity = msg.velocity;
    }
    
    return OK;
}


