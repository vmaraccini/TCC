#include <main.h>
#include "lcd.h"
#include <can-communication.h>

#use rs232(BAUD=19200, XMIT=PIN_C6, RCV=PIN_C7, PARITY=N, BITS=8)

//Prototypes
void ReadLoop();

int pedal = 0;
int mode = 0; // 0 manual, 1 auto




void sendPedal(int pedal) {
   CAN_200 message;
   message.pedal_simulado = pedal;
   CanSetSendAddress(0x200);
   delay_ms(50);
   SendCanFrame(&message);
}

void setPedal(int p) {
   pedal = p;
}

void calculatePedal() {
   // todo
   pedal = 42;
}




#int_rda
void serial()
{
   char dado = 0;
   disable_interrupts(int_rda);
   dado = getc();
   
   switch (dado) {
      case 'a':
         setPedal(10);
      break;
      case 'b':
         setPedal(60);
      break;
      case 'c':
         setPedal(120);
      break;
      case 'd':
         setPedal(160);
      break;
      case 'm':
         mode = !mode;
      break;
   }

   clear_interrupt(int_rda);
   enable_interrupts(int_rda);
}



#int_timer0
void timer0()
{
   if (mode == 1) {
      pedal = calculatePedal();
   }
   sprintf(lcdBuffer, "PED: %lu|%d", pedal, mode);
   WriteLCDLine1(&lcdBuffer);
   sendPedal(pedal)
}





void PrintNoData() {
   ClearLCDBuffer();
   sprintf(lcdBuffer, "NO DATA");
   WriteLCDLine1(&lcdBuffer);
}

void PrintVelocity(unsigned long int velocity, Can_ID id) {
   ClearLCDBuffer();
   sprintf(lcdBuffer, "VEL: %lu|%lu", velocity, id);
   WriteLCDLine0(&lcdBuffer);
}

void main() {
   SetupCommunication();
   SetupCan();
   //SetupTimers();
   LCDInitialize();
   
   ClearLCDBuffer();
   sprintf(lcdBuffer, "ProDAV");
   WriteLCDLine0(&lcdBuffer);
   delay_ms(50);
   sprintf(lcdBuffer, "v20161103");
   WriteLCDLine1(&lcdBuffer);
   
   delay_ms(2500);
   
   ReadLoop();
}

void ReadLoop() {
   CAN_590 message;
   Can_Id id;
   
   ClearLCDBuffer();
   
   while (1) {
      if (ReadMessage(0x590, &message, &id)) {
         PrintVelocity(message.velocidade, id);
      }
      delay_ms(10);
   }
}

