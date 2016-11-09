//
//  Comum.h
//  Lumiar-Final
//
//  Created by Victor Maraccini on 6/19/16.
//
//

#ifndef Comum_h
#define Comum_h

#include <pthread.h>
#include <stdio.h>

#define BUFFER_LEN  100 //Buffer comum
#define PARAM_LEN   15  //Tamanho de um parametro
#define VAL_LEN     16  //Tamanho de um valor

#define PAR_ESTADO_STR          "PAR_ESTADO     "
#define PAR_MODO_STR            "PAR_MODO       "
#define PAR_INTENSIDADE_STR     "PAR_INTENSIDADE"
#define PAR_LIMIAR_LUMI_STR     "PAR_LIMIAR_LUMI"
#define PAR_LIMIAR_TEMP_STR     "PAR_LIMIAR_TEMP"

#define VAL_TEMPERATURA_STR     "VAL_TEMPERATURA "
#define VAL_LUMINOSIDADE_STR    "VAL_LUMINOSIDADE"
#define VAL_INTENSIDADE_STR     "VAL_INTENSIDADE "

typedef enum {
    PAR_ESTADO,
    PAR_MODO,
    PAR_INTENSIDADE,
    PAR_LIMIAR_LUMI,
    PAR_LIMIAR_TEMP,
    PAR_DESCONHECIDO
} PARAMETRO;

typedef enum {
    VAL_TEMPERATURA,
    VAL_LUMINOSIDADE,
    VAL_INTENSIDADE,
    VAL_DESCONHECIDO
} VALOR;

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

typedef enum {
    GET,
    POST,
    PUT,
    PATCH,
    DESCONHECIDO
} METODO;

typedef enum {
    R200,
    R404,
    JSON,
    IMG
} RESPOSTA;

//Declaracao de variaveis expostas globalmente

extern volatile int temperatura;
extern volatile int luminosidade;
extern volatile int intensidade;

extern volatile char estado; // 0 = Desligar / 1 = Ligar
extern volatile char modo; // 0 = Manual / Automatico

extern volatile int limiar_luminosidade; // 0 - 100
extern volatile int limiar_temperatura; // 0 - 100

//Variaveis de sincronizacao

extern pthread_mutex_t valorLock;
extern pthread_mutex_t paramLock;

//Funcoes auxiliares
char alteraParametro(PARAMETRO parametro, int valor);

PARAMETRO parametroDeString(char *buffer);
VALOR valorDeString(char *buffer);

char *stringDeParametro(PARAMETRO param);
char *stringDeValor(VALOR valor);

#endif /* Comum_h */
