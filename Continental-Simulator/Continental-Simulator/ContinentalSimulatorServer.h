//
//  ContinentalSimulatorServer.h
//  Continental-Simulator
//
//  Created by Victor Maraccini on 6/24/16.
//  Copyright © 2016 Victor Maraccini. All rights reserved.
//

#ifndef ContinentalSimulatorServer_h
#define ContinentalSimulatorServer_h

#include <stdio.h>


//Constants
#define SERVER_PORT_DISTANCE    25010
#define SERVER_PORT_VELOCITY    25011
#define SERVER_PORT_ANGLE       25012

/**
 Serves at porst PORT_DISTANCE, PORT_VELOCITY and PORT_ANGLE
 and updates internal states to send to Matlab.
 */
int serveAndUpdate();

extern unsigned char server_quit;

#endif /* ContinentalSimulatorServer_h */
