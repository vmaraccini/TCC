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

int main_json(){
    
    FILE* f;

    usleep(2E6);

    for (;;) {
        
        f = fopen("../web/data.json", "w");

        if (f == NULL) {
            printf("Error opening file!\n");
            return 1;
        }

        /* print some text */
        fprintf(f, "{\"current_speed\":%d,\"leader_distance\":%d,\"leader_speed\":%d,\"max_speed\":%d,\"pedal\":%d}",
                currentVelocity, leaderDistance, leaderVelocity, maxVelocity, pedalByte);
        
        fclose(f);

        usleep(1E6);

    }

    return 0;

}
