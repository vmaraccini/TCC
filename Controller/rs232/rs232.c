#include "json.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

void sendData() {
    // TODO
    // foo(pedal);
}

void main_rs232(){
    usleep(2E6);
    for (;;) {
        sendData();
        usleep(1E4);
    }
}
