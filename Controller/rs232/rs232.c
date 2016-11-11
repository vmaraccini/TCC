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
void sendData(void* message, ssize_t length);
int readData(char* buffer, int length);

//Internal variables
int uart_fd = -1;

void main_rs232(){
    configureUART(&uart_fd, O_RDWR | O_NOCTTY | O_NDELAY);

    for (int i = 48; i < 90; i++) {
        //Wait for new data
        sem_wait(&cts);
        sendData(&i, 1);
        if (i == 90) {i = 48;}
    }
}

int readData(char* buffer, int length) {
    ssize_t rx_length = read(uart_fd, buffer, length);
    if (rx_length < length) {
        return -1;
    }
    return 0;
}

void sendData(void* message, ssize_t length) {
    if (uart_fd != -1) {
        ssize_t count = write(uart_fd, (void *)message, length);
        if (count < 0) { perror("UART TX error\n"); }
    }
}

void configureUART(int* fdes, int mode) {
    //Open UART in blocking read/write mode
    *fdes = open("/dev/tty.usbserial", mode);
    
    if (*fdes == -1) {
        //ERROR - CAN'T OPEN SERIAL PORT
        perror("Error - Unable to open UART\n");
    }
    
    //Configure UART
    struct termios tty;
    tcgetattr(*fdes, &tty);
    
    cfsetospeed (&tty, B9600);
    cfsetispeed (&tty, B9600);
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars

    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
    
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
    
    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    tcflush(*fdes, TCIFLUSH);
    tcsetattr(*fdes, TCSANOW, &tty);
}
