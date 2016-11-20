//
//  UDP-Client.c
//  Controller
//
//  Created by Victor Gabriel Maraccini & Denis Isidoro de Franca
//  Copyright © 2016 TCC. All rights reserved.
//

#include "../Comum.h"
#include "UDP-Client.h"

#include "network.h"
#include <stdio.h>
#include <string.h>


//Socket descriptors
int clientSd_maxVelocity;
int clientSd_distance;

//Server addresses
struct sockaddr_in addr_maxVelocity;
struct sockaddr_in addr_distance;

//------------------ Functions ------------------

int main_udpMaxVelocity() {
    printf("ProDAV - Cliente UDP - Max speed\n\n");
    maxVelocity = 60;
    while (1);

    /*
    addr_maxVelocity = initializeClient(&clientSd_maxVelocity, 20001);
    usleep(1E3); //1ms

    char status = OK;
    while (status == OK) {
        status = read_maxVelocity();
    }

    closeClient(clientSd_maxVelocity);

    return OK;
    */
}

int main_udpDistance() {
    printf("ProDAV - Cliente UDP - Stereo\n\n");


    while (1);

    /*addr_distance = initializeClient(&clientSd_distance, 20002);
    usleep(1E3); //1ms

    char status = OK;
    while (status == OK) {
        status = read_distance();
    }

    closeClient(clientSd_distance);

    return OK;*/
}

#pragma mark - Read

char read_maxVelocity() {
    char buffer[BUFFER_LEN];
    VELOCITY_MSG msg;

    sendMessage("hello", clientSd_maxVelocity, addr_maxVelocity);
    char status = readMessage(buffer, clientSd_maxVelocity, addr_maxVelocity);
    if (status != OK) {
        printf("Error getting maximum velocity");
        return OK;
    } else {
        memcpy(&msg, buffer, 1);
        maxVelocity = msg.maxVelocity;
    }

    return OK;
}

char read_distance() {
    char buffer[BUFFER_LEN];
    STEREO_MSG msg;

    sendMessage("hello", clientSd_distance, addr_distance);
    char status = readMessage(buffer, clientSd_distance, addr_distance);
    if (status != OK) {
        printf("Error getting current distance");
        return OK;
    } else {
        memcpy(&msg, buffer, 4);
        leaderDistance = msg.distance;
        leaderVelocity = msg.velocity;

        printf("dist: %d, vel: %d\n", leaderDistance, leaderVelocity);
    }

    return OK;
}


