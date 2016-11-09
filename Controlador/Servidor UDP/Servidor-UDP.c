//
//  Servidor-UDP.c
//  Lumiar-Final
//
//  Created by Victor Maraccini on 6/19/16.
//
//

#include "Servidor-UDP.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

/*
 Comandos
 Consulta
 Retorna informações sobre os valores dos parâmetros e dos valores operacionais:
 Parâmetros:
 - Estado  PAR_ESTADO (“ligado” ou “desligado”)
 - Modo de operação  PAR_MODO (“manual” ou “automático”)
 - Intensidade do LED  <valor numérico>
 - Limiar para acendimento  <valor numérico>
 Valores:
 - Temperatura
 - Luminosidade
 - Intensidade do LED
 
 Alteração de parâmetros:
 Mudar estado:
 - PAR_ESTADO (ligar / desligar)
 Mudar modo de operação:
 - PAR_MODO (manual / automático)
 Mudar intensidade no modo manual:
 - PAR_INTENSIDADE <valor>
 Mudar limiar de luminosidade:
 - PAR_LIMIAR_LUMI (valor de 0 a 100)
 Mudar limiar de temperatura:
 - PAR_LIMIAR_TEMP (valor de 0 a 100)
 
 O valor informado pelo usuário deve ser validado pelo “Sistema Interativo” antes de aplicar no parâmetro
 */

#define PORTA_UDP 30000

#define RES_ERRO_STR            "ERRO"
#define RES_ERRO_ALTERAR_STR    "ERRO - Erro ao alterar valor"
#define RES_ERRO_COMANDO_STR    "ERRO - Comando desconhecido"
#define RES_ERRO_PARAM_STR      "ERRO - Parametro desconhecido"
#define RES_ERRO_CONS_STR       "ERRO - Consulta invalida"
#define RES_ERRO_VAL_STR        "ERRO - Valor desconhecido"
#define RES_ERRO_VAL_INV_STR    "ERRO - Valor invalido"

#define RES_OK_STR              "OK"

// Variaveis privadas
char udp_exit = 0;
int sd; //Socket descriptor
struct sockaddr_in serviceAddress;

pthread_mutex_t serverLock;

//Funcoes UDP
char abirSocketUDP();
char fecharSocketUDP();
ssize_t enviaMensagem(char *message, int socketDescriptor, struct sockaddr_in toAddress);
ssize_t recebeMensagem(char *buffer, int socketDescriptor, struct sockaddr_in *fromAddress);

//Funcoes do Servidor
char iniciarServidor();
char loop();
char executa(char *buffer, struct sockaddr_in fromAddr);

char consultaValor(VALOR valor, char *resposta);
char consultaParametro(PARAMETRO parametro, char *resposta);

void main_udp()
{
    iniciarServidor();
    
    while (!udp_exit) {
        loop();
    }
}

#pragma mark - Funcoes UDP

char abirSocketUDP()
{
    //Tenta abir o socket UDP
    sd = socket(PF_INET, SOCK_DGRAM, 0);
    
    //Verifica erros
    if (sd < 0)
        return ERRO_SOCKET;
    
    return OK;
}

char fecharSocketUDP()
{
    //Tenta fechar o socket
    int status = close(sd);
    if (status < 0)
        return ERRO_SOCKET;
    
    return OK;
}

ssize_t enviaMensagem(char *message, int socketDescriptor, struct sockaddr_in toAddress) {
    return sendto(socketDescriptor,
                  message,
                  strlen(message) + 1,
                  0,
                  (struct sockaddr *) &toAddress,
                  sizeof(toAddress));
}

ssize_t recebeMensagem(char *buffer, int socketDescriptor, struct sockaddr_in *fromAddress) {
    unsigned int size = sizeof(struct sockaddr_in);
    ssize_t s = recvfrom(socketDescriptor, buffer, BUFFER_LEN, 0, (struct sockaddr *) fromAddress, &size);
    return s;
}

#pragma mark - Funcoes do servidor

char iniciarServidor()
{
    if (abirSocketUDP())
        return ERRO_SOCKET;
    
    serviceAddress.sin_family = AF_INET;
    serviceAddress.sin_addr.s_addr = INADDR_ANY;
    serviceAddress.sin_port = htons(PORTA_UDP);
    
    //Binda a uma porta para escutar requisicoes
    if (bind(sd, (struct sockaddr *) &serviceAddress, sizeof(struct sockaddr_in)) != 0)
        return ERRO_BIND;
    
    return OK;
}

char loop()
{
    //Le mensagens
    char buffer[BUFFER_LEN];
    struct sockaddr_in client;
    
    if (recebeMensagem(buffer, sd, &client) < 0)
        return ERRO_RECEBER;
    
    //Executa comando
    executa(buffer, client);
    
    return OK;
}

char executa(char *buffer, struct sockaddr_in fromAddr)
{
    char status = OK;
    char comando[COM_LEN + 1];
    
    //Copia o comando
    memcpy(comando, &buffer[0], COM_LEN);
    comando[COM_LEN] = '\0';
    
    char resposta[BUFFER_LEN];
    
    if (strcmp(comando, ALTERA_COM_STR) == 0) {
        //Alteracao de parametro
        char paramBuff[PARAM_LEN + 1];
        memcpy(paramBuff, &buffer[COM_LEN], PARAM_LEN);
        paramBuff[PARAM_LEN] = '\0';
        
        int valor;
        PARAMETRO param = parametroDeString(paramBuff);
        sscanf(&buffer[COM_LEN + PARAM_LEN], "%d", &valor);
        
        char status = alteraParametro(param, valor);
        if (status == ERRO_VALOR_INVALIDO) {
            strcpy(resposta, RES_ERRO_VAL_INV_STR);
            status = ERRO_ALTERACAO;
        } else if (status != OK) {
            strcpy(resposta, RES_ERRO_ALTERAR_STR);
            status = ERRO_ALTERACAO;
        } else {
            strcpy(resposta, RES_OK_STR);
        }
        
    } else if (strcmp(comando, CONSULTA_COM_STR) == 0) {
        //Consulta
        PARAMETRO param = parametroDeString(&buffer[COM_LEN]);
        VALOR val = valorDeString(&buffer[COM_LEN]);
        
        if (param != PAR_DESCONHECIDO) {
            //Consulta de parametro
            if (consultaParametro(param, resposta) != OK) {
                strcpy(resposta, RES_ERRO_PARAM_STR);
                status = ERRO_CONSULTA;
            }
        } else if (val != VAL_DESCONHECIDO) {
            //Consulta de valor
            if (consultaValor(val, resposta) != OK) {
                strcpy(resposta, RES_ERRO_VAL_STR);
                status = ERRO_CONSULTA;
            }
        } else {
            strcpy(resposta, RES_ERRO_CONS_STR);
            status = ERRO_DESCONHECIDO;
        }
    } else {
        strcpy(resposta, RES_ERRO_COMANDO_STR);
        status = ERRO_DESCONHECIDO;
    }
    
    enviaMensagem(resposta, sd, fromAddr);
    
    return status;
}

#pragma mark - Funcoes especificas do Lumiar

char consultaValor(VALOR valor, char *resposta)
{
    switch (valor) {
        case VAL_INTENSIDADE:
            sprintf(resposta, "%d", intensidade);
            break;
        case VAL_LUMINOSIDADE:
            sprintf(resposta, "%d", luminosidade);
            break;
        case VAL_TEMPERATURA:
            sprintf(resposta, "%d", temperatura);
            break;
        default:
            return ERRO_DESCONHECIDO;
            break;
    }
    
    return OK;
}

char consultaParametro(PARAMETRO parametro, char *resposta)
{
    switch (parametro) {
        case PAR_MODO:
            sprintf(resposta, modo == 0 ? "manual" : "automatico");
            break;
        case PAR_ESTADO:
            sprintf(resposta, estado == 0 ? "desligado" : "ligado");
            break;
        case PAR_INTENSIDADE:
            sprintf(resposta, "%d", intensidade);
            break;
        case PAR_LIMIAR_LUMI:
            sprintf(resposta, "%d", limiar_luminosidade);
            break;
        case PAR_LIMIAR_TEMP:
            sprintf(resposta, "%d", limiar_temperatura);
            break;
        default:
            break;
    }
    return OK;
}