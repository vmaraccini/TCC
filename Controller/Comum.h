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

extern char pedalByte;
//extern char accEnabled;
extern int maxVelocity;
extern int leaderDistance;
extern int leaderVelocity;
extern int currentVelocity;

#endif /* Comum_h */
