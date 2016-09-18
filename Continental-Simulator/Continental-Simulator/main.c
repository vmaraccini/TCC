//
//  main.c
//  Continental-Simulator
//
//  Created by Victor Maraccini on 6/16/16.
//  Copyright © 2016 Victor Maraccini. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <pthread/pthread.h>

#include "ContinentalSimulatorServer.h"
#include "ContinentalUDPClient.h"

volatile double distance, velocity, angle = 0;

int main(int argc, const char * argv[]) {
    
    pthread_t serverThread;
    pthread_t clientThread;
    
    pthread_create(&serverThread, NULL, (void*)serveAndUpdate, 0);
    pthread_create(&clientThread, NULL, (void*)broadcast, 0);
    
    pthread_join(serverThread, NULL);
    pthread_join(clientThread, NULL);
    
}