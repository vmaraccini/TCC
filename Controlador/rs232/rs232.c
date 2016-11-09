#include "json.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

void sendData() {
    char mensagem[2];
    char* m = (char*)&pedal;
    memcpy(mensagem, m, 2);

    // TODO
    // foo(mensagem);
}

void main_rs232(){
    usleep(2E6);
    for (;;) {
        sendData()
        usleep(1E6);
    }
}