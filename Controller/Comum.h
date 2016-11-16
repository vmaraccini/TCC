#ifndef Comum_h
#define Comum_h

#include <pthread.h>
#include <stdio.h>

#define BUFFER_LEN 8
#define BUFSIZE 8

typedef enum {
    OK = 0,
    ERRO_PARAMETRO_DESCONHECIDO,
    ERRO_VALOR_DESCONHECIDO,
    ERRO_VALOR_INVALIDO,
    ERRO_SOCKET,
    ERRO_BIND,
    ERRO_RECEBER,
    ERRO_ALTERACAO,
    ERRO_CONSULTA,
    ERRO_DESCONHECIDO,
    
    ERRO_METODO_DESCONHECIDO,
    ERRO_METODO_INVALIDO,
    ERRO_ARQUIVO,
    ERRO_CAMINHO
    
} STATUS;

extern volatile char pedalByte;
extern volatile char maxVelocity;
extern volatile int leaderDistance;
extern volatile int leaderVelocity;
extern volatile int currentVelocity;

#endif /* Comum_h */
