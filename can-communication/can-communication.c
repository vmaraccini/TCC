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

void PrintPedal(unsigned long int velocity, Can_ID id) {
   ClearLCDBuffer();
   sprintf(lcdBuffer, "PED: %lu|%lu", velocity, id);
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
   
   WriteLoop();
}

void ReadLoop() {
   CAN_470 message;
   Can_Id id;
   
   ClearLCDBuffer();
   sprintf(lcdBuffer, "RX NODE");
   WriteLCDLine0(&lcdBuffer);
   
   while (1) {
      if (ReadMessage(0x470, &message, &id)) {
         //PrintVelocity(message.seta_esquerda, id);
      }
      
      delay_ms(10);
   }
}

void WriteLoop() {
   CAN_200 reference;
   CAN_201 enableControl;
   
   ClearLCDBuffer();
   sprintf(lcdBuffer, "TX NODE");
   WriteLCDLine0(&lcdBuffer);
   
   CanStructInit(&reference);
   reference.pedal_simulado = 5;
   
   CanStructInit(&enableControl);
   enableControl.modo_pedal_sim = 1;
   enableControl.modo_operacao = 0;
   enableControl.ref_marcha_lenta = 0;
   
   delay_ms(50);
      
   int pedal = 0;
   while (1) {
      pedal = pedal + 1;
      if (pedal >= 40) pedal = 0;
      reference.pedal_simulado = pedal;
      CanSetSendAddress(0x200);
      SendCanFrame(&reference);
      PrintPedal(pedal, 0x200);
      
      delay_ms(150);
      CanSetSendAddress(0x201);
      SendCanFrame(&enableControl);
      delay_ms(150);
   }
}

