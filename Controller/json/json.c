#include "json.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

void main_json(){

    usleep(2E6);
    FILE *f = fopen("data.json", "w");

    for (;;) {

        if (f == NULL){
            printf("Error opening file!\n");
            exit(1);
        }

        /* print some text */
//        fprintf(f, "max = %f\n", maxSpeed);

        usleep(1E6);

    }

    fclose(f);

}
