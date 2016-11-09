//
//  main.c
//  ClienteServidor
//
//  Created by Victor Gabriel Maraccini & Denis Isidoro de Franca
//  Copyright © 2016 Denisccini. All rights reserved.
//

//Error codes
#define ERROR_OPENSOCKET 1
#define ERROR_CONVERTIPV4ADDRESS 2
#define ERROR_CONNECT 2
#define ERROR_SEND 3
#define ERROR_RECEIVE 4
#define ERROR_CLOSE 5

#define EXIT 6

#include "Comum.h"

#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

//Socket descriptors
int client_sd_maxSpeed; 
int client_sd_distance;

//------------------ Functions ------------------

int iniciaCliente(int *client_sd, int port);
char finalizaCliente(int *client_sd);
char menu(int client_sd);

int conecta(char *serverIP, int serverPort, int socketDescriptor);

int enviaMensagem(char *message, int socketDescriptor);
int recebeMensagem(char *message, int socketDescriptor);

int main_maxSpeed(int argc, const char * argv[])
{
    printf("ProDAV - Cliente UDP - Max speed\n\n");
    
    iniciaCliente(&client_sd_maxSpeed, 3031);
    
    char status = OK;
    while (status == OK) {
        status = menu(&client_sd_maxSpeed);
    }
    
    finalizaCliente(client_sd_maxSpeed);
    
    return OK;
}

int main_distance(int argc, const char * argv[])
{
    printf("ProDAV - Cliente UDP - Distance\n\n");
    
    iniciaCliente(&client_sd_distance, 3032);
    
    char status = OK;
    while (status == OK) {
        status = menu(&client_sd_distance);
    }
    
    finalizaCliente(client_sd_distance);
    
    return OK;
}

int iniciaCliente(int *client_sd, int port)
{
    int status;
    
    //Opens socket connection using UDP
    client_sd = socket(PF_INET, SOCK_DGRAM, 0);
    
    //Handles socket opening errors
    if (client_sd < 0) {
        perror("Erro ao abrir socket");
        exit(ERROR_OPENSOCKET);
    }
    
    //printf("Entre com o IP do Lumiar: ");
    char ipString[BUFFER_LEN] = "127.0.0.1";
    //scanf("%s", ipString);
    
    //printf("Entre com a porta: ");
    //int port;
    scanf("%d", &port);
    
    status = conecta(ipString, port, client_sd);
    if (status != OK)
        exit(status);
    
    return OK;
}

char finalizaCliente(int *client_sd)
{
    int status = close(client_sd);
    if (status < 0)
        return ERROR_CLOSE;
    
    return OK;
}

#pragma mark - Menu

char menu(int client_sd)
{
    int comando;
    char mensagem[BUFFER_LEN];
    // mensage = ...
    
    /*
    int status = enviaMensagem(mensagem, client_sd);
    if (status != OK) {
        printf("Erro ao enviar mensagem.");
        return OK;
    }
    */
    
    status = recebeMensagem(mensagem, client_sd);
    if (status != OK) {
        printf("Erro ao enviar mensagem.");
        return OK;
    } else {
        //printf("Resposta do servidor: %s", mensagem);
        //maxSpeed = getValueFrom(mensagem)
    }
    
    return OK;
}

#pragma mark - Funcoes UDP

int conecta(char *serverIP, int serverPort, int socketDescriptor) {
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

int enviaMensagem(char *message, int socketDescriptor) {
    ssize_t len = send(socketDescriptor,
                       message,
                       strlen(message) + 1, //Add 1 to include \0
                       0);
    
    return len >= 0 ? OK : ERROR_SEND;
}

int recebeMensagem(char *buffer, int socketDescriptor) {
    ssize_t len = recv(socketDescriptor, buffer, BUFFER_LEN, 0);
    return len >= 0 ? OK : ERROR_RECEIVE;
}
