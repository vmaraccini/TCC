//
//  rs232.c
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
#include <sys/time.h>
#include <stdio.h>
#include "termios.h"
#include "fcntl.h"

#include "semaphore.h"

extern sem_t cts;

void configureUART(int* fdes, int mode);

int uart_fd = -1;
char buffer[2];

#if DEBUG
struct timeval tv;
#endif

///Reads the pedal byte over UART
void sendData() {
    write(uart_fd, (char *)&pedalByte, 1);
}

///Reads 2 bytes off UART and populates currentVelocity
void readData() {
    if (read(uart_fd, buffer, 2) == 2) {
        currentVelocity = buffer[0] + (buffer[1] << 8);
    }
    
#if DEBUG
    struct timeval temp;
    gettimeofday(&temp, NULL);
    unsigned long delta = 1E6*temp.tv_sec + temp.tv_usec - 1E6*tv.tv_sec - tv.tv_usec;
    tv = temp;
    printf("Time between reads: %lu", delta);
#endif
    
    //Flush after read to discard dangling bytes
    tcflush(uart_fd, TCIOFLUSH);
}

///Configures UART for communication
void configureUART(int* fdes, int mode) {
    //Open UART in blocking read/write mode
    *fdes = open("/dev/ttyUSB0", mode);
    if (*fdes == -1) {
        //ERROR - CAN'T OPEN SERIAL PORT
        perror("Error - Unable to open UART\n");
    }
    
    //Configure UART
    struct termios options;
    tcgetattr(*fdes, &options);
    cfsetospeed(&options, (speed_t)B9600);
    cfsetispeed(&options, (speed_t)B9600);
    
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_cflag &= ~CRTSCTS; //Disable flow control
    options.c_cflag &= ~PARENB; //Disable parity
    options.c_cc[VMIN] = 0; //Pure time read. Wait for VTIME else return 0
    options.c_cc[VTIME] = 5; //0.5 sec read timeout
    options.c_iflag &= ~(IXON | IXOFF | IXANY); //Disable flow control options
    
    tcflush(*fdes, TCIFLUSH);
    tcsetattr(*fdes, TCSANOW, &options);
}

///Master RS232 communication
///@discussion Sends one pedal byte (0-100% from 0-255) and receives 2 velocity bytes from UART.
void main_rs232() {
    configureUART(&uart_fd, O_RDWR | O_NOCTTY);
    
    for (;;) {
        //Wait for new data
        sem_wait(&cts);
        sendData();
        readData();
    }
}
