#include "json.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include "semaphore.h"

extern sem_t cts;

void sendData() {
    
}

void main_rs232(){
    for (;;) {
        //Wait for new data
        sem_wait(&cts);
        sendData();
    }
}
