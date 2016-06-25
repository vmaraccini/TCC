//
//  Common.c
//  Continental-Simulator
//
//  Created by Victor Maraccini on 6/19/16.
//  Copyright © 2016 Victor Maraccini. All rights reserved.
//

#include "Common.h"
#include <string.h>

#pragma mark - JSON helper

unsigned int AddFloatToJSON(char *buffer, char *key, float value, char isLast)
{
    char valString[VALBUFFSIZE];
    sprintf(valString, "%f", value);
    return AddToJSON(buffer, key, valString, isLast);
}

unsigned int AddIntToJSON(char *buffer, char *key, unsigned int value, char isLast)
{
    char valString[VALBUFFSIZE];
    sprintf(valString, "%d", value);
    return AddToJSON(buffer, key, valString, isLast);
}

unsigned int AddToJSON(char *buffer, char *key, char *value, char isLast)
{
    unsigned len = 0;
    
    //"key": value (as ASCII number)
    buffer[len++] = '"';
    strcpy(&buffer[len], key);
    len += strlen(key);
    buffer[len++] = '"';
    buffer[len++] = ':';
    buffer[len++] = ' ';
    strcpy(&buffer[len], value);
    len += strlen(value);
    
    if (!isLast) {
        buffer[len++] = ',';
    }
    
    return len;
}