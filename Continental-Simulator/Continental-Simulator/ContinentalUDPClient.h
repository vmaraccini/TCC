//
//  ContinentalUDPClient.h
//  Continental-Simulator
//
//  Created by Victor Maraccini on 6/19/16.
//  Copyright © 2016 Victor Maraccini. All rights reserved.
//

#ifndef ContinentalUDPClient_h
#define ContinentalUDPClient_h

#include <stdio.h>
#include "Common.h"

//Constants
#define CLIENT_PORT_DISTANCE   25000
#define CLIENT_PORT_ANGLE      25001
#define CLIENT_PORT_VELOCITY   25002

extern unsigned char client_quit;

int broadcast();

#endif /* ContinentalUDPClient_h */
