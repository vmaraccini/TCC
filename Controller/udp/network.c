//
//  network.c
//  Controller
//
//  Created by Victor Gabriel Maraccini & Denis Isidoro de Franca
//  Copyright © 2016 TCC. All rights reserved.
//

#include "Comum.h"
#include "network.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int openConnection(char *serverIP, int serverPort, int socketDescriptor) {
    int status;
    
    //Creates the server descriptor
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    
    //Converts IP address from string
    status = inet_pton(AF_INET, serverIP, &serverAddress.sin_addr);
    if (status <= 0)
        return ERROR_CONVERTIPV4ADDRESS;
    
    //Creates the connection
    status = connect(socketDescriptor,
                     (struct sockaddr*)&serverAddress,
                     sizeof(serverAddress));
    if (status < 0)
        return ERROR_CONNECT;
    
    return OK;
}

int sendMessage(char *message, int socketDescriptor) {
    ssize_t len = send(socketDescriptor,
                       message,
                       strlen(message) + 1, //Add 1 to include \0
                       0);
    
    return len >= 0 ? OK : ERROR_SEND;
}

int getMessage(char *buffer, int socketDescriptor) {
    ssize_t len = recv(socketDescriptor, buffer, BUFFER_LEN, 0);
    return len >= 0 ? OK : ERROR_RECEIVE;
}

int initializeClient(int *client_sd, int port) {
    int status;
    
    //Opens socket connection using UDP
    *client_sd = socket(PF_INET, SOCK_DGRAM, 0);
    
    //Handles socket opening errors
    if (client_sd < 0) {
        perror("Erro ao abrir socket");
        exit(ERROR_OPENSOCKET);
    }
    
    status = openConnection("127.0.0.1", port, *client_sd);
    if (status != OK)
        exit(status);
    
    return OK;
}

char closeClient(int client_sd) {
    int status = close(client_sd);
    if (status < 0)
        return ERROR_CLOSE;
    
    return OK;
}
