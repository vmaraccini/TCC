#include <main.h>
#include "lcd.h"
#include <can-communication.h>

//Prototypes
void ReadLoop();
void WriteLoop();

void PrintNoData() {
   ClearLCDBuffer();
   sprintf(lcdBuffer, "NO DATA");
   WriteLCDLine1(&lcdBuffer);
}

void PrintVelocity(unsigned long int velocity, Can_ID id) {
   ClearLCDBuffer();
   sprintf(lcdBuffer, "VEL: %lu|%lu", velocity, id);
   WriteLCDLine1(&lcdBuffer);
}

void main() {
   SetupCommunication();
   SetupCan();
   //SetupTimers();
   LCDInitialize();
   
   ClearLCDBuffer();
   sprintf(lcdBuffer, "HELLO WORLD");
   WriteLCDLine0(&lcdBuffer);
   
   delay_ms(500);
   
   ReadLoop();
}

void ReadLoop() {
   CAN_590 message;
   Can_Id id;
   
   ClearLCDBuffer();
   sprintf(lcdBuffer, "RX NODE");
   WriteLCDLine0(&lcdBuffer);
   
   while (1) {
      if (ReadMessage(0x590, &message, &id)) {
         PrintVelocity(message.velocidade, id);
      }
      
      delay_ms(10);
   }
}

void WriteLoop() {
   CAN_590 message;
   
   ClearLCDBuffer();
   sprintf(lcdBuffer, "TX NODE");
   WriteLCDLine0(&lcdBuffer);
   
   message.velocidade = 100;
   CanSetSendAddress(0x590);
   
   delay_ms(50);
      
   while (1) {
      SendCanFrame(&message);
      
      PrintVelocity(message.velocidade, 0);
      delay_ms(100);
      message.velocidade += 1;
      if (message.velocidade > 500) message.velocidade = 0;
   }
}

