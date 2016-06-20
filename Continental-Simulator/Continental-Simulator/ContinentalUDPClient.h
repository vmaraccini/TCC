//
//  ContinentalUDPClient.h
//  Continental-Simulator
//
//  Created by Victor Maraccini on 6/19/16.
//  Copyright © 2016 Victor Maraccini. All rights reserved.
//

#ifndef ContinentalUDPClient_h
#define ContinentalUDPClient_h

#include <stdio.h>
#include "Common.h"

#define BUFFER_LEN 300

typedef enum {
    ERROR_OPENSOCKET = 1,
    ERROR_CONVERTIPV4ADDRESS,
    ERROR_CONNECT,
    ERROR_SEND,
    ERROR_RECEIVE,
    ERROR_CLOSE,
    ERROR_BIND,
    ERROR_ECHO,
} CLIENT_ERROR;

int connectTo(char *serverIP, int serverPort, int *socketDescriptor);
int closeSocket (int sd);
int sendMessage(char *message, int socketDescriptor);
int receiveMessage(char *buffer, int socketDescriptor);

#endif /* ContinentalUDPClient_h */
