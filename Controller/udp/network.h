//
//  network.h
//  Controller
//
//  Created by Victor Maraccini on 11/9/16.
//  Copyright © 2016 Denis. All rights reserved.
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

int iniciaCliente(int *client_sd, int port);
char finalizaCliente(int client_sd);
char menu(int client_sd);

int conecta(char *serverIP, int serverPort, int socketDescriptor);

int enviaMensagem(char *message, int socketDescriptor);
int recebeMensagem(char *message, int socketDescriptor);


#endif /* network_h */
