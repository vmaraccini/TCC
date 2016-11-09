//
//  Servidor-WEB.h
//  Lumiar-Final
//
//  Created by Victor Maraccini on 6/19/16.
//
//

#ifndef Servidor_WEB_h
#define Servidor_WEB_h

#include "SizeConstants.h"
#include "../Comum.h"

//todo: MOVE THESE GUYS TO ____HELPER.H ACCORDINGLY 
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define BASE_DIR        "/web"

void main_web();
void perdaconexao();

#endif /* Servidor_WEB_h */
