//
//  ContinentalUDPClient.c
//  Continental-Simulator
//
//  Created by Victor Maraccini on 6/19/16.
//  Copyright © 2016 Victor Maraccini. All rights reserved.
//

#include "ContinentalUDPClient.h"

#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>

unsigned char client_quit = 0;

char *broadcastAddress = "192.168.1.150";

int initClient (int *sd);
int closeSocket (int sd);

int connectTo(char *serverIP, int serverPort, int *socketDescriptor);
int sendDouble(double val, int socketDescriptor);

int broadcast()
{
    int dsd, asd, vsd;
    initClient(&dsd);
    initClient(&asd);
    initClient(&vsd);
    
    connectTo(broadcastAddress, CLIENT_PORT_DISTANCE, &dsd);
    connectTo(broadcastAddress, CLIENT_PORT_ANGLE, &asd);
    connectTo(broadcastAddress, CLIENT_PORT_VELOCITY, &vsd);
    
    while (client_quit == 0) {
        sendDouble(distance, dsd);
        sendDouble(velocity, vsd);
        sendDouble(angle, asd);
        
        usleep(30E3);
    }
    
    closeSocket(dsd);
    closeSocket(vsd);
    closeSocket(asd);
    
    return OK;
}

#pragma mark - Socket

int initClient (int *sd)
{
    //Opens socket connection using UDP
    *sd = socket(PF_INET, SOCK_DGRAM, 0);
    
    //Handles socket opening errors
    if (*sd < 0) {
        exit(ERROR_OPENSOCKET);
    }
    
    return OK;
}

int closeSocket (int sd)
{
    int status = close(sd);
    if (status < 0)
        exit(ERROR_CLOSE);
    return status;
}

#pragma mark - Messages

int connectTo(char *serverIP, int serverPort, int *socketDescriptor)
{
    int status;
    initClient(socketDescriptor);
    
    //Creates the server descriptor
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    
    //Converts IP address from string
    status = inet_pton(AF_INET, serverIP, &serverAddress.sin_addr);
    if (status <= 0)
        return ERROR_CONVERTIPV4ADDRESS;
    
    //Creates the connection
    status = connect(*socketDescriptor, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
    if (status < 0)
        return ERROR_CONNECT;
    
    return OK;
}

int sendDouble(double val, int socketDescriptor)
{
    ssize_t len =  send(socketDescriptor, &val, sizeof(double), 0);
    return len > 0 ? OK : ERROR_SEND;
}
