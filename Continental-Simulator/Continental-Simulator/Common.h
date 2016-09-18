//
//  Common.h
//  Continental-Simulator
//
//  Created by Victor Maraccini on 6/19/16.
//  Copyright © 2016 Victor Maraccini. All rights reserved.
//

#ifndef Common_h
#define Common_h

#include <stdio.h>

#define BUFFER_LEN 100
#define VALBUFFSIZE 50

typedef enum {
    OK = 0,
    ERROR_OPENSOCKET,
    ERROR_CONVERTIPV4ADDRESS,
    ERROR_CONNECT,
    ERROR_SEND,
    ERROR_RECEIVE,
    ERROR_CLOSE,
    ERROR_BIND
} ERROR;

volatile extern double distance, velocity, angle;

//JSON Functions
unsigned int AddFloatToJSON(char *buffer, char *key, float value, char isLast);
unsigned int AddIntToJSON(char *buffer, char *key, unsigned int value, char isLast);
unsigned int AddToJSON(char *buffer, char *key, char *value, char isLast);

#endif /* Common_h */
