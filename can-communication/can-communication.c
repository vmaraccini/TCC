#include <main.h>
#include "lcd.h"
#include "rs232.h"
#include <can-communication.h>

#use rs232(BAUD=9600, XMIT=PIN_C6, RCV=PIN_C7, PARITY=N, BITS=8)

CAN_200 reference;
CAN_201 enableControl;
CAN_590 message;

char pedal;

// Prototypes
void readCan();
void writeSerial();
void readSerial();
void writeCan();

void enableACC();

void printPedal(int pedal);

void main() {
   SetupCommunication();
   SetupUART();
   SetupCan();
   LCDInitialize();
   
   ClearLCDBuffer();
   sprintf(lcdBuffer, "PRODAV");
   WriteLCDLine0(&lcdBuffer);
   
   //Filter messages to read: 0x590
   CanFilter(0x590);
   
   //Initialize message structures
   CanStructInit(&reference);
   CanStructInit(&enableControl);
   
   for (;;) {
      enableACC();
      for (int i = 0; i < 3; i++) { readCan(); }
      writeSerial();
      readSerial();
      writeCan();
   }  
}

void readCan() {
   ReadMessage(&message);
   
   //Debug: Display velocity
   ClearLCDBuffer();
   unsigned long int v = 256*message.vel2 + message.vel1;
   float f = v / 100;
   sprintf(lcdBuffer, "%f", f);
   WriteLCDLine0(&lcdBuffer);
}

void writeSerial() {
   UART_Write(message.vel1);
   UART_Write(message.vel2);
}

void readSerial() {
   pedal = UART_Read();

   ClearLCDBuffer();
   sprintf(lcdBuffer, "PED: %d", pedal);
   WriteLCDLine1(&lcdBuffer);
}

void writeCan() {
   reference.pedal_simulado = pedal;
   CanSetSendAddress(0x200);
   SendCanFrame(&reference);
}

void enableACC() {
   enableControl.modo_pedal_sim = 1;
   enableControl.modo_operacao = 0;
   enableControl.ref_marcha_lenta = 0;
   CanSetSendAddress(0x201);
   SendCanFrame(&enableControl);
}

