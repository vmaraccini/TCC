#include <regs_16f87x.h>

#define LINE0_ADDRESS 0X80
#define LINE1_ADDRESS 0XAF

  // ENTRADAS                               *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// As entradas devem ser associadas a nomes para facilitar a programação e
// futuras alterações do hardware.
#bit  C1   = portd.0                     // definições das chaves
#bit  C2   = portd.1                     // chave aberta  = 1
#bit  C3   = portd.2                     // chave fechada = 0
#bit  C4   = portd.3

///* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// *                               SAÍDAS                                *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// As saídas devem ser associadas a nomes para facilitar a programação e
// futuras alterações do hardware.
#bit  RS          = portb.5
#bit  E           = portd.4
#bit  DATA_PIN_7  = portd.3
#bit  DATA_PIN_6  = portd.2
#bit  DATA_PIN_5  = portd.1
#bit  DATA_PIN_4  = portd.0

//********************* LOW LEVEL FUNCTIONS *********************

void LcdCommand(char data) {
   RS = 0;                  // seleciona o envio de um comando

   TRISD = TRISD & 0xF0;      // configura pinos D0..D3 como saídas

   DATA_PIN_7 = (unsigned char)((data & 0x80)>>7);   //acerta dado
   DATA_PIN_6 = (unsigned char)((data & 0x40)>>6);   //no barramento
   DATA_PIN_5 = (unsigned char)((data & 0x20)>>5);
   DATA_PIN_4 = (unsigned char)((data & 0x10)>>4);
   
   E = 1;                  // envia pulso de enable
   delay_us(1);
   E = 0;
   
   DATA_PIN_7 = (unsigned char)((data & 0x08)>>3);   // acerta dado
   DATA_PIN_6 = (unsigned char)((data & 0x04)>>2);   // no barramento
   DATA_PIN_5 = (unsigned char)((data & 0x02)>>1);
   DATA_PIN_4 = (unsigned char)(data & 0x01);
   
   E = 1;                  // envia pulso de enable
   delay_us(1);
   E = 0;
   
   delay_us(40);
   TRISD = TRISD | 0x0F;      // configura pinos D0..D3 como entradas
}

void LcdClear(void) {
   LcdCommand(0x01);         // limpa lcd
   delay_ms(2);
}

void LcdInitialize(void) {
   delay_ms(100);            // espera 100 milisengundos
   TRISD = TRISD & 0xE0;      // configura pinos D0..D3 como saídas
   
   PORTD = 0x03;
  
   E = 1 ;                    // gera pulso no enable
   delay_us(1);            // espera 1 microsegundo
   E = 0;                    // desce o pino de enable
   
   delay_ms(5);            // espera 5 milisengundos
   
   E = 1 ;                  // gera pulso no enable
   delay_us(1);            // espera 1 microsegundo
   E = 0;                  // desce o pino de enable
   
   delay_us(100);            // espera 100 microsengundos
   
   E = 1 ;                  // gera pulso no enable
   delay_us(1);            // espera 1 microsegundo
   E = 0;                  // desce o pino de enable
   
   delay_us(40);            // espera 40 microsegundos
   
   PORTD = 0x02;
   
   E = 1 ;                  // gera pulso no enable
   delay_us(1);            // espera 1 microsegundo
   E = 0;                  // desce o pino de enable
   
   delay_us(40);            // espera 40 microsegundos
   
   LcdCommand(0x28);         // liga o display, sem cursor e sem blink (4 vias)
   
   LcdClear();            // limpa lcd
   
   LcdCommand(0x0C);         // display sem cursor
   
   LcdCommand(0x06);         // desloca cursor para a direita
}

void LcdWrite(char data)
{
   RS = 1;                  // seleciona o envio de um comando
   
   TRISD = TRISD & 0xF0;      // configura pinos D0..D3 como saídas
   
   DATA_PIN_7 = (char)((data & 0x80)>>7);   // acerta dado
   DATA_PIN_6 = (char)((data & 0x40)>>6);   // no barramento
   DATA_PIN_5 = (char)((data & 0x20)>>5);
   DATA_PIN_4 = (char)((data & 0x10)>>4);
   
   E = 1;            // envia pulso de enable
   delay_us(1);
   E = 0;
   
   DATA_PIN_7 = (char)((data & 0x08)>>3);   // acerta dado
   DATA_PIN_6 = (char)((data & 0x04)>>2);   // no barramento
   DATA_PIN_5 = (char)((data & 0x02)>>1);
   DATA_PIN_4 = (char)(data & 0x01);
   
   E = 1;            // envia pulso de enable
   delay_us(1);
   E = 0;
   
   delay_us(40);
   TRISD = TRISD | 0x0F;      // configura pinos D0..D3 como entradas
}

//********************* HIGH LEVEL FUNCTIONS *********************

void WriteLCD(char* msg, int length) {
   for(int i = 0; i < length; i++) {
      LcdWrite(msg[i]);
      delay_ms(5);
   }
}

void WriteLCDLine0(char* line) {
   LcdCommand(LINE0_ADDRESS);
   WriteLCD(line, 16);
}

void WriteLCDLine1(char* line) {
   LcdCommand(LINE1_ADDRESS);
   WriteLCD(line, 16);
}

char lcdBuffer[16];

void ClearLCDBuffer() {
   for (int i = 0; i < 16; i++) {
      lcdBuffer[i] = ' ';
   }
}

