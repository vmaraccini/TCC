//
//  UDP-Client.h
//  Controller
//
//  Created by Victor Gabriel Maraccini & Denis Isidoro de Franca
//  Copyright © 2016 TCC. All rights reserved.
//

#ifndef Cliente_UDP_h
#define Cliente_UDP_h

#include <stdio.h>
#include "../Comum.h"

#define UDP_STEREO_PORT
#define UPD_MAXVELOCITY_PORT


int main_udpMaxVelocity();
int main_udpDistance();

char read_maxVelocity();
char read_distance();

typedef struct VELOCITY_MSG {
    int maxVelocity:1;
} VELOCITY_MSG;

typedef struct STEREO_MSG {
    int distance:2;
    int velocity:2;
} STEREO_MSG;

#endif /* Cliente_UDP_h */
