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

int client_sd; //Socket descriptor

//------------------ Functions ------------------

int iniciaCliente();
char finalizaCliente();

int conecta(char *serverIP, int serverPort, int socketDescriptor);

int enviaMensagem(char *message, int socketDescriptor);

int recebeMensagem(char *message, int socketDescriptor);

char menu();

int main(int argc, const char * argv[])
{
    printf("Projeto Lumiar - Controlador UDP\n\n");
    
    iniciaCliente();
    
    char status = OK;
    while (status == OK) {
        status = menu();
    }
    
    finalizaCliente();
    
    return OK;
}

int iniciaCliente()
{
    int status;
    
    //Opens socket connection using UDP
    client_sd = socket(PF_INET, SOCK_DGRAM, 0);
    
    //Handles socket opening errors
    if (client_sd < 0) {
        perror("Erro ao abrir socket");
        exit(ERROR_OPENSOCKET);
    }
    
    printf("Entre com o IP do Lumiar: ");
    char ipString[BUFFER_LEN];
    scanf("%s", ipString);
    
    printf("Entre com a porta: ");
    int port;
    scanf("%d", &port);
    
    status = conecta(ipString, port, client_sd);
    if (status != OK)
        exit(status);
    
    return OK;
}

char finalizaCliente()
{
    int status = close(client_sd);
    if (status < 0)
        return ERROR_CLOSE;
    
    return OK;
}

#pragma mark - Menu

char menu()
{
    printf("\n----- Menu Principal -----\n");
    printf("1 - Consultar\n");
    printf("2 - Alterar\n");
    printf("3 - Sair\n");
    printf("--------------------------\n");
    
    int comando;
    char mensagem[BUFFER_LEN];
    scanf("%d", &comando);
    
    switch (comando) {
        case 1: {
            printf("1 - Parametro\n");
            printf("2 - Valor\n");
            printf("3 - Voltar\n");
            printf("--------------------------\n");
            
            scanf("%d", &comando);
            if (comando == 3)
                return OK;
            
            char *consulta;
            if (comando == 1) {
                printf("1 - Estado\n");
                printf("2 - Modo\n");
                printf("3 - Intensidade\n");
                printf("4 - Limiar de luminosidade\n");
                printf("5 - Limiar de temperatura\n");
                printf("--------------------------\n");
                
                scanf("%d", &comando);
                switch (comando) {
                    case 1:
                        consulta = PAR_ESTADO_STR;
                        break;
                    case 2:
                        consulta = PAR_MODO_STR;
                        break;
                    case 3:
                        consulta = PAR_INTENSIDADE_STR;
                        break;
                    case 4:
                        consulta = PAR_LIMIAR_LUMI_STR;
                        break;
                    case 5:
                        consulta = PAR_LIMIAR_TEMP_STR;
                        break;
                    default:
                        printf("Comando inválido");
                        return OK;
                        break;
                }
                
            } else if (comando == 2) {
                printf("1 - Temperatura\n");
                printf("2 - Luminosidade\n");
                printf("3 - Intensidade\n");
                printf("--------------------------\n");
                
                scanf("%d", &comando);
                switch (comando) {
                    case 1:
                        consulta = VAL_TEMPERATURA_STR;
                        break;
                    case 2:
                        consulta = VAL_LUMINOSIDADE_STR;
                        break;
                    case 3:
                        consulta = VAL_INTENSIDADE_STR;
                        break;
                    default:
                        printf("Comando inválido");
                        return OK;
                        break;
                }
                
            } else {
                printf("Comando inválido");
                return OK;
            }
            
            sprintf(mensagem, "CONS%s", consulta);
            
        }
            break;
        case 2:
            printf("1 - Estado\n");
            printf("2 - Modo\n");
            printf("3 - Intensidade\n");
            printf("4 - Limiar de luminosidade\n");
            printf("5 - Limiar de temperatura\n");
            printf("--------------------------\n");
            
            scanf("%d", &comando);
            char *parametro;
            switch (comando) {
                case 1:
                    parametro = PAR_ESTADO_STR;
                    break;
                case 2:
                    parametro = PAR_MODO_STR;
                    break;
                case 3:
                    parametro = PAR_INTENSIDADE_STR;
                    break;
                case 4:
                    parametro = PAR_LIMIAR_LUMI_STR;
                    break;
                case 5:
                    parametro = PAR_LIMIAR_TEMP_STR;
                    break;
                default:
                    printf("Comando inválido");
                    return OK;
                    break;
            }
            
            int valor;
            printf("Entre com o valor: ");
            scanf("%d", &valor);
            
            sprintf(mensagem, "ALTE%s%d", parametro, valor);
            
            break;
            case 3:
            return EXIT;
            
        default:
            printf("Comando inválido");
            return OK;
            break;
    }
    
    int status = enviaMensagem(mensagem, client_sd);
    if (status != OK) {
        printf("Erro ao enviar mensagem.");
        return OK;
    }
    
    status = recebeMensagem(mensagem, client_sd);
    if (status != OK) {
        printf("Erro ao enviar mensagem.");
        return OK;
    } else {
        printf("Resposta do servidor: %s", mensagem);
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
