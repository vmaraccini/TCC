//
//  network.h
//  Controller
//
//  Created by Victor Gabriel Maraccini & Denis Isidoro de Franca
//  Copyright © 2016 TCC. All rights reserved.
//

#ifndef network_h
#define network_h

//Error codes
#define ERROR_OPENSOCKET 1
#define ERROR_CONVERTIPV4ADDRESS 2
#define ERROR_CONNECT 2
#define ERROR_SEND 3
#define ERROR_RECEIVE 4
#define ERROR_CLOSE 5

#define EXIT 6

#include <stdio.h>

int initializeClient(int *client_sd, int port);
char closeClient(int client_sd);
char menu(int client_sd);

int openConnection(char *serverIP, int serverPort, int socketDescriptor);

int sendMessage(char *message, int socketDescriptor);
int getMessage(char *message, int socketDescriptor);


#endif /* network_h */
