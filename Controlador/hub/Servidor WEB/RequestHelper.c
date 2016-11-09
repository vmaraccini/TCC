//
//  RequestHelper.c
//  Lumiar-Final
//
//  Created by Denis on 21/06/2016.
//
//

#include "RequestHelper.h"
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void show40x (STATUS status, char* msg) {
    switch (status) {
        case ERRO_METODO_DESCONHECIDO:
            strcpy(msg, "Metodo nao conhecido");
            break;
        case ERRO_METODO_INVALIDO:
            strcpy(msg, "Metodo invalido");
            break;
        case ERRO_ARQUIVO:
            strcpy(msg, "Arquivo nao encontrado");
            break;
        case ERRO_CAMINHO:
            strcpy(msg, "Falha no endereco");
            break;
        default:
            strcpy(msg, "Erro desconhecido");
            break;
    }
}

void replaceWithCurrentValues (char* msg) {
    char buffer[MSGSIZE];
    strcpy(buffer, msg);
    sprintf(msg, buffer, modo, estado, luminosidade, intensidade, limiar_luminosidade);
}

RESPOSTA interpret(char* buffer, char* msg) {
    
    METODO method = methodFromRequest(buffer);
    STATUS status = method == DESCONHECIDO ? ERRO_METODO_DESCONHECIDO : OK;
    IF_ERROR_THEN_40X(R404)
    
    char path[BUFFERSIZE];
    status = pathFromRequest(buffer, path);
    
    if (method == GET) {
        if (strcmp(path, "/current.json") == 0) {
            sprintf(msg,
                    "{\"modo\": %d, \"estado\": %d, \"luminosidade\": %d, \"intensidade\": %d, \"limiar\": %d}",
                    modo, estado, luminosidade, intensidade, limiar_luminosidade);
            return JSON;
        } else if (strncmp(path, "/update.html", 12) == 0) {
            update(buffer, msg);
        } else {
            char newPath[BUFSIZ];
            char pwd[BUFSIZ];
            getCurrentDir(pwd, BUFSIZ);
            composePath(pwd, path, newPath);
            
            if (strcmp(&newPath[strlen(newPath) - 3], "jpg") == 0) {
                strcpy(msg, newPath);
                return IMG;
            }
            
            status = browseAndRead(newPath, msg);
            if (strncmp(path, "/", 1) == 0 ||
                strncmp(path, "/index.html", 11) == 0) {
                replaceWithCurrentValues(msg);
            }
        }
    } else {
        status = ERRO_METODO_INVALIDO;
    }
    
    IF_ERROR_THEN_40X(R404)
    return R200;
    
}

METODO methodFromRequest(char* buffer) {
    if (strncmp("GET", buffer, 3) == 0)
        return GET;
    else if (strncmp("POST", buffer, 4) == 0)
        return POST;
    else if (strncmp("PUT", buffer, 3) == 0)
        return PUT;
    else if (strncmp("PATCH", buffer, 5) == 0)
        return PATCH;
    else
        return DESCONHECIDO;
}

STATUS pathFromRequest(char* buffer, char *result) {
    long firstSpace = strstr(buffer, " ") - buffer;
    
    long nextSpace = strstr(&buffer[firstSpace + 1], " ") - &buffer[firstSpace];
    long questionMark = strstr(&buffer[firstSpace + 1], "?") - &buffer[firstSpace];
    
    if (nextSpace < 0 && questionMark < 0) {
        return ERRO_CAMINHO;
    }
    
    long min = questionMark > nextSpace ? nextSpace : questionMark;
    size_t end = questionMark < 0 ? nextSpace : min;
    
    strncpy(result, &buffer[firstSpace + 1], end - 1);
    result[end - 1] = '\0';
    return OK;
}

long getNextKeyAndValue(char *buffer, char* key, int* value, long offset) {
    if (offset < 0) {
        offset = strstr(buffer, "?") - buffer;
    }
    
    if (offset >= strlen(buffer)) {
        return -1;
    }
    
    char *keySubstr = &buffer[offset + 1];
    
    //Try to find equal sign
    long equalSign = strstr(keySubstr, "=") - keySubstr;
    if (equalSign < 0) {
        return -1;
    }
    
    strncpy(key, keySubstr, equalSign);
    key[equalSign] = '\0';
    
    char *valueSubstr = &keySubstr[equalSign + 1];
    //Find equal sign
    char *andPos = strstr(keySubstr, "&");
    char *spacePos = strstr(keySubstr, " ");
    long and = andPos != NULL ? andPos - valueSubstr : -1;
    long space = spacePos != NULL ? spacePos - valueSubstr : -1;
    
    long end = and < 0 ? space : and;
    if (end < 0) {
        end = strlen(valueSubstr - 1);
        if (end >= BUFSIZ) {
            end = BUFSIZ - 1;
        }
    }
    
    char scan[BUFSIZ];
    strncpy(scan, valueSubstr, end);
    scan[end] = '\0';
    
    sscanf(scan, "%d", value);
    
    long valueOffset = valueSubstr - buffer;
    return end + valueOffset;
}

int isDirectory(FILE* fd) {
    int fn = fileno(fd);
    struct stat path_stat;
    fstat(fn, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

STATUS browseAndRead(char* path, char* msg) {
    
    int status = OK;
    
    long lSize;
    size_t result;
    
    FILE* fd = fopen(path, "r");
    if (fd != NULL && isDirectory(fd)) {
        char newPath[BUFSIZ];
        sprintf(newPath, "%sindex.html", path);
        
        fd = fopen(newPath, "r");
        if (fd == NULL) {
            status = ERRO_CAMINHO;
            IF_ERROR_THEN_40X(status)
        }
    } else if (fd == NULL) {
        return ERRO_ARQUIVO;
    }
    
    // obtain file size:
    fseek(fd, 0, SEEK_END);
    lSize = ftell(fd);
    rewind(fd);
    
    // copy the file into the buffer:
    result = fread(msg, 1, lSize, fd);
    if (result != lSize) {
        return ERRO_ARQUIVO;
    }
    
    // terminate
    fclose(fd);
    
    return OK;
}

char interpretKeyAndValue(char* key, int value) {
    PARAMETRO param = parametroDeString(key);
    if (param != PAR_DESCONHECIDO) {
        return alteraParametro(param, value);
    }
    return ERRO_PARAMETRO_DESCONHECIDO;
}

void update (char* buffer, char* msg) {
    
    char key[BUFFERSIZE];
    int value = 0;
    long pos = -1;
    
    char line[BUFSIZ];
    sscanf(buffer, "GET %s", line);
    
    char status = OK, statusTemp = OK;
    
    do {
        pos = getNextKeyAndValue(line, key, &value, pos);
        statusTemp = interpretKeyAndValue(key, value);
        if (statusTemp != OK) {
            status = statusTemp;
        }
    } while (pos >= 0);
    
    sprintf(msg, "%s%s",
           (status == OK ? "Dados alterados com sucesso" : "Nem todos os dados foram alterados com sucesso"),
           "<script>window.setTimeout(function(){window.location.href = '..';},5000);</script>");
    
}
