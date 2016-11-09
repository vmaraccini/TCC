//
//  Cliente-UDP.h
//  Lumiar-Final
//
//  Created by Victor Maraccini on 6/19/16.
//
//

#ifndef Cliente_UDP_h
#define Cliente_UDP_h

#include <stdio.h>
#include "../Comum.h"


int main_udpMaxVelocity();
int main_udpDistance();

char read_maxVelocity();
char read_distance();

int iniciaCliente(int *client_sd, int port);
char finalizaCliente(int client_sd);
char menu(int client_sd);

int conecta(char *serverIP, int serverPort, int socketDescriptor);

int enviaMensagem(char *message, int socketDescriptor);
int recebeMensagem(char *message, int socketDescriptor);

typedef struct PLACA_MSG {
   int maxVelocity;
} PLACA_MSG;

typedef struct STEREO_MSG {
   int distance;
   int velocity;
} STEREO_MSG;

#endif /* Cliente_UDP_h */
