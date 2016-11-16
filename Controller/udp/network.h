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

#include <netinet/in.h>
#include <stdio.h>

struct sockaddr_in initializeClient(int *client_sd, int port);
char closeClient(int client_sd);
char menu(int client_sd);

struct sockaddr_in openConnection(char *serverIP, int serverPort, int socketDescriptor);

int sendMessage(char *message, int socketDescriptor, struct sockaddr_in serveraddr);
int readMessage(char *message, int socketDescriptor, struct sockaddr_in serveraddr);


#endif /* network_h */
