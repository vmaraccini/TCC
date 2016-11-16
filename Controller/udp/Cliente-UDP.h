//
//  Cliente-UDP.h
//  Lumiar-Final
//
//  Created by Victor Maraccini on 6/19/16.
//
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

typedef struct PLACA_MSG {
   int maxVelocity;
} PLACA_MSG;

typedef struct STEREO_MSG {
   int distance;
   int velocity;
} STEREO_MSG;

#endif /* Cliente_UDP_h */
