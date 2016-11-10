#include <main.h>
#include "lcd.h"
#include <can-communication.h>

#use rs232(BAUD=19200, XMIT=PIN_C6, RCV=PIN_C7, PARITY=N, BITS=8)

CAN_200 reference;
CAN_201 enableControl;
CAN_470 message;
Can_Id id;
char buffer[2];
char pedal;

// Prototypes
void readCan();
void writeSerial();
void readSerial();
void writeCan();

void main() {
   SetupCommunication();
   SetupCan();
   LCDInitialize();
   
   ClearLCDBuffer();
   sprintf(lcdBuffer, "PRODAV");
   WriteLCDLine0(&lcdBuffer);
   
   delay_ms(500);
   
   for (;;) {
      readCan();
      writeSerial();
      readSerial();
      writeCan();
   }
}

void readCan() {
   ClearLCDBuffer();
   sprintf(lcdBuffer, "READ CAN");
   WriteLCDLine0(&lcdBuffer);
   
   if (ReadMessage(0x590, &message, &id)) {
      ClearLCDBuffer();
      sprintf(lcdBuffer, "VEL: %lu", message.velocidade);
      WriteLCDLine1(&lcdBuffer);
   }
}

void writeSerial() {
   ClearLCDBuffer();
   sprintf(lcdBuffer, "WRITE SERIAL");
   WriteLCDLine0(&lcdBuffer);

   sprintf(buffer, "%lu", message.velocidade);
   putc(buffer);
}

void readSerial() {
   ClearLCDBuffer();
   sprintf(lcdBuffer, "READ SERIAL");
   WriteLCDLine0(&lcdBuffer);

   pedal = getc();

   ClearLCDBuffer();
   sprintf(lcdBuffer, "PED: %d", pedal);
   WriteLCDLine1(&lcdBuffer);
}

void writeCan() {
   ClearLCDBuffer();
   sprintf(lcdBuffer, "WRITE CAN");
   WriteLCDLine0(&lcdBuffer);
   
   CanStructInit(&reference);
   reference.pedal_simulado = 5;
   
   CanStructInit(&enableControl);
   enableControl.modo_pedal_sim = 1;
   enableControl.modo_operacao = 0;
   enableControl.ref_marcha_lenta = 0;
   
   delay_ms(30);
   reference.pedal_simulado = pedal;
   CanSetSendAddress(0x200);
   SendCanFrame(&reference);
   PrintPedal(pedal, 0x200);
   
   delay_ms(70);
   CanSetSendAddress(0x201);
   SendCanFrame(&enableControl);
}

