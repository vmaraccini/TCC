#include "Comum.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include "termios.h"
#include "semaphore.h"

extern sem_t cts;

//Function prototypes
void configureUART(int* fdes, int mode);
void sendData(char* message, ssize_t length);
int readData(char* buffer, int length);

//Internal variables
int uart_read_fd = -1;
int uart_write_fd = -1;

void main_rs232_write(){
    configureUART(&uart_write_fd, O_WRONLY | O_NOCTTY | O_NDELAY);

    for (;;) {
        //Wait for new data
        sem_wait(&cts);
        sendData("1", 1);
    }
}

void main_rs232_read() {
    configureUART(&uart_read_fd, O_RDONLY | O_NOCTTY);
    
    char buffer[2];
    for (;;) {
        //Performs blocking read
        if (readData(buffer, 2) < 0) {
            
        }
    }
}

int readData(char* buffer, int length) {
    ssize_t rx_length = read(uart_read_fd, buffer, length);
    if (rx_length < length) {
        return -1;
    }
    return 0;
}

void sendData(char* message, ssize_t length) {
    if (uart_read_fd != -1) {
        ssize_t count = write(uart_read_fd, message, length);
        if (count < 0) { perror("UART TX error\n"); }
    }
}

void configureUART(int* fdes, int mode) {
    //Open UART in blocking read/write mode
    *fdes = open("/dev/ttyAMA0", mode);
    
    if (*fdes == -1) {
        //ERROR - CAN'T OPEN SERIAL PORT
        perror("Error - Unable to open UART\n");
    }
    
    //Configure UART
    struct termios options;
    tcgetattr(*fdes, &options);
    options.c_cflag = B19200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(*fdes, TCIFLUSH);
    tcsetattr(*fdes, TCSANOW, &options);
}
