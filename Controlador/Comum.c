//
//  Comum.c
//  Lumiar-Final
//
//  Created by Victor Maraccini on 6/19/16.
//
//

#include "Comum.h"

#include <string.h>

#pragma mark - Valores e parametros

char alteraParametro(PARAMETRO parametro, int valor)
{
    char status = OK;
    
    pthread_mutex_lock(&paramLock);
    switch (parametro) {
        case PAR_ESTADO:
            if (valor != 0 && valor != 1) {
                return ERRO_VALOR_INVALIDO;
            }
            estado = valor;
            break;
        case PAR_MODO:
            if (valor != 0 && valor != 1) {
                return ERRO_VALOR_INVALIDO;
            }
            modo = valor;
            break;
        case PAR_INTENSIDADE:
            if (valor < 0 || valor > 100) {
                return ERRO_VALOR_INVALIDO;
            }
            intensidade = valor;
            break;
        case PAR_LIMIAR_LUMI:
            if (valor < 0 || valor > 100) {
                return ERRO_VALOR_INVALIDO;
            }
            limiar_luminosidade = valor;
            break;
        case PAR_LIMIAR_TEMP:
            if (valor < 0 || valor > 100) {
                return ERRO_VALOR_INVALIDO;
            }
            limiar_temperatura = valor;
            break;
        default:
            status = ERRO_PARAMETRO_DESCONHECIDO;
            break;
    }
    pthread_mutex_unlock(&paramLock);
    
    return status;
}

PARAMETRO parametroDeString(char *buffer)
{
    if (strstr(PAR_ESTADO_STR, buffer) != NULL) {
        return PAR_ESTADO;
    }
    if (strstr(PAR_MODO_STR, buffer) != NULL) {
        return PAR_MODO;
    }
    if (strstr(PAR_INTENSIDADE_STR, buffer) != NULL) {
        return PAR_INTENSIDADE;
    }
    if (strstr(PAR_LIMIAR_LUMI_STR, buffer) != NULL) {
        return PAR_LIMIAR_LUMI;
    }
    if (strstr(PAR_LIMIAR_TEMP_STR, buffer) != NULL) {
        return PAR_LIMIAR_TEMP;
    }

    return PAR_DESCONHECIDO;
}

VALOR valorDeString(char *buffer)
{
    if (strcmp(buffer, VAL_TEMPERATURA_STR) == 0) {
        return VAL_TEMPERATURA;
    }
    if (strcmp(buffer, VAL_LUMINOSIDADE_STR) == 0) {
        return VAL_LUMINOSIDADE;
    }
    if (strcmp(buffer, VAL_INTENSIDADE_STR) == 0) {
        return VAL_INTENSIDADE;
    }
    
    return VAL_DESCONHECIDO;
}

char *stringDeParametro(PARAMETRO param)
{
    switch (param) {
        case PAR_ESTADO:
            return PAR_ESTADO_STR;
            break;
        case PAR_MODO:
            return PAR_MODO_STR;
            break;
        case PAR_INTENSIDADE:
            return PAR_INTENSIDADE_STR;
            break;
        case PAR_LIMIAR_LUMI:
            return PAR_LIMIAR_LUMI_STR;
            break;
        case PAR_LIMIAR_TEMP:
            return PAR_LIMIAR_TEMP_STR;
            break;
        default:
            return NULL;
            break;
    }
}

char *stringDeValor(VALOR valor)
{
    switch (valor) {
        case VAL_TEMPERATURA:
            return VAL_TEMPERATURA_STR;
            break;
        case VAL_LUMINOSIDADE:
            return VAL_LUMINOSIDADE_STR;
            break;
        case VAL_INTENSIDADE:
            return VAL_INTENSIDADE_STR;
            break;
        default:
            return NULL;
            break;
    }
}