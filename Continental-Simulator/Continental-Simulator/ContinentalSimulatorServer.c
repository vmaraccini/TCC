//
//  ContinentalSimulatorServer.c
//  Continental-Simulator
//
//  Created by Victor Maraccini on 6/24/16.
//  Copyright © 2016 Victor Maraccini. All rights reserved.
//

#include "ContinentalSimulatorServer.h"
#include "ContinentalStruct.h"
#include "Common.h"

#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define VEL_LEN 1.5 //m
#define VEL_WIDTH 2 //m

int dsd, vsd, asd; //Socket descriptor

unsigned char server_quit = 0;

//------------------
// Socket
//------------------

///Opens the socket connections
int openUDPSockets();

///Populates a socket with a given port for UDP accesss
void populateAddress(struct sockaddr_in *address, int port);

///Closes a socked
int closeSocket();

///Gets a message from the server
int receive(void *buffer, int socketDescriptor);

//Receives the messages and updates internal state.
void serve();

int serveAndUpdate()
{
    if (openUDPSockets())
        return ERROR_OPENSOCKET;
    
    while (server_quit == 0) {
        //Starts the broadcast program
        serve();
    }
    
    //Closes the socket
    closeSocket();
    
    return OK;
}

void serve()
{
    receive((void*)&distance, dsd);
    receive((void*)&velocity, vsd);
    receive((void*)&angle, asd);
    
//    CAN_ARS308_TARGET_1 target1;
//    CAN_ARS308_TARGET_2 target2;
//    
//    CreateVehicle(distance,
//                  angle,
//                  velocity,
//                  VEL_WIDTH,
//                  VEL_LEN,
//                  &target1,
//                  &target2);
    
//    char buffer[512];
//    unsigned int len = 0;
//    len = ContinentalStructTarget1ToJSON(target1, buffer);
//    ContinentalStructTarget2ToJSON(target2, &buffer[len]);
//    printf("JSON: %s\n\n", buffer);
}

int openUDPSockets() {
    //Opens socket connection using UDP
    dsd = socket(PF_INET, SOCK_DGRAM, 0);
    vsd = socket(PF_INET, SOCK_DGRAM, 0);
    asd = socket(PF_INET, SOCK_DGRAM, 0);
    
    //Handles socket opening errors
    if (dsd < 0 ||
        vsd < 0 ||
        asd < 0) {
        perror("Error opening socket");
        return ERROR_OPENSOCKET;
    }
    
    struct sockaddr_in distanceAddress;
    struct sockaddr_in velocityAddress;
    struct sockaddr_in angleAddress;
    
    populateAddress(&distanceAddress, SERVER_PORT_DISTANCE);
    populateAddress(&velocityAddress, SERVER_PORT_VELOCITY);
    populateAddress(&angleAddress, SERVER_PORT_ANGLE);
    
    if (bind(dsd, (struct sockaddr *) &distanceAddress,
             sizeof(struct sockaddr_in)) != 0)
        return ERROR_BIND;
    
    if (bind(vsd, (struct sockaddr *) &velocityAddress,
             sizeof(struct sockaddr_in)) != 0)
        return ERROR_BIND;
    
    if (bind(asd, (struct sockaddr *) &angleAddress,
             sizeof(struct sockaddr_in)) != 0)
        return ERROR_BIND;
    
    
    return OK;
}

int receive(void *buffer, int socketDescriptor) {
    unsigned int size = sizeof(struct sockaddr_in);
    return recvfrom(socketDescriptor, buffer, sizeof(buffer), 0, (struct sockaddr *) NULL, &size) > 0;
}

void populateAddress(struct sockaddr_in *address, int port)
{
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(port);
}
