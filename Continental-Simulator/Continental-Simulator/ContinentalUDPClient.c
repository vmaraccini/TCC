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

int initClient (int *sd) {
    
    //Opens socket connection using UDP
    *sd = socket(PF_INET, SOCK_DGRAM, 0);
    
    //Handles socket opening errors
    if (*sd < 0) {
        exit(ERROR_OPENSOCKET);
    }
    
    return OK;
}

int closeSocket (int sd) {
    int status = close(sd);
    if (status < 0)
        exit(ERROR_CLOSE);
    return status;
}

int connectTo(char *serverIP, int serverPort, int *socketDescriptor) {
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

int sendMessage(char *message, int socketDescriptor) {
    return (send(socketDescriptor,
                 message,
                 strlen(message) + 1, //Add 1 to include \0
                 0) >= 0) ? OK : ERROR_SEND;
}

int receiveMessage(char *buffer, int socketDescriptor) {
    unsigned int status = recv(socketDescriptor, buffer, BUFFER_LEN, 0) >= 0 ? OK : ERROR_RECEIVE;
    return status;
}
