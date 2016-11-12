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
   
   delay_ms(500);
   
   //Filter messages to read: 0x590
   CanFilter(0x590);
   
   //Initialize message structures
   CanStructInit(&reference);
   CanStructInit(&enableControl);
   
   enableACC();
   
   for (;;) {
      readCan();
      writeSerial();
      readSerial();
      writeCan();
   }
}

void readCan() {
   message.velocidade += 1; //Stubbed
   //ReadMessage(0x590, &message, &id)
}

void writeSerial() {
   int msg0 = message.velocidade & 0xFF;
   int msg1 = (message.velocidade >> 8) & 0xFF;
   UART_Write(msg0);
   UART_Write(msg1);
}

void readSerial() {
   pedal = UART_Read();
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

