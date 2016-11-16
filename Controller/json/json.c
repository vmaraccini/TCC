//
//  json.c
//  Controller
//
//  Created by Victor Gabriel Maraccini & Denis Isidoro de Franca
//  Copyright © 2016 TCC. All rights reserved.
//

#include "json.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

void main_json(){
    
    FILE* f;

    usleep(2E6);
    //FILE *f = fopen("data.json", "w");

    for (;;) {
        
        f = fopen("../web/data.json", "w");

        if (f == NULL) {
            printf("Error opening file!\n");
            exit(1);
        }

        /* print some text */
        fprintf(f, "{\"current_speed\":%d,\"leader_distance\":%d,\"leader_speed\":%d,\"max_speed\":%d,\"pedal\":%d}",
                currentVelocity, leaderDistance, leaderVelocity, maxVelocity, pedalByte);
        
        fclose(f);

        usleep(1E6);

    }

    //fclose(f);

}
