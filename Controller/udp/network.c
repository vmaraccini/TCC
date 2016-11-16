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

struct sockaddr_in openConnection(char *hostname, int serverPort, int socketDescriptor) {
    struct sockaddr_in serveraddr;
    struct hostent *server;
    
    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }
    
    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(serverPort);
    
    return serveraddr;
}

int sendMessage(char *message, int socketDescriptor, struct sockaddr_in serveraddr) {
    int serverlen = sizeof(serveraddr);
    ssize_t len = sendto(socketDescriptor, message, strlen(message) + 1, 0, (struct sockaddr *) &serveraddr, serverlen);
    return len >= 0 ? OK : ERROR_SEND;
}

int readMessage(char *buffer, int socketDescriptor, struct sockaddr_in serveraddr) {
    socklen_t serverlen = sizeof(serveraddr);
    ssize_t len = recvfrom(socketDescriptor, buffer, strlen(buffer), 0, (struct sockaddr *) &serveraddr, &serverlen);
    return len >= 0 ? OK : ERROR_RECEIVE;
}

struct sockaddr_in initializeClient(int *client_sd, int port) {
    //Opens socket connection using UDP
    *client_sd = socket(PF_INET, SOCK_DGRAM, 0);
    
    //Handles socket opening errors
    if (client_sd < 0) {
        perror("Erro ao abrir socket");
        exit(ERROR_OPENSOCKET);
    }
    
    return openConnection("localhost", port, *client_sd);
}

char closeClient(int client_sd) {
    int status = close(client_sd);
    if (status < 0)
        return ERROR_CLOSE;
    
    return OK;
}
