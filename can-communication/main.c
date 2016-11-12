#include <main.h>
#include "can_mcp2515.h"
#include "regs_16f87x.h"

#zero_RAM

// ENTRADAS*
#bit   C1   = portd.0                     // definições das chaves
#bit   C2   = portd.1                     // chave aberta  = 1
#bit     C3   = portd.2                     // chave fechada = 0
#bit   C4   = portd.3

// SAIDAS
#bit     RS          = portb.5
#bit     E          = portd.4
#bit    DATA_PIN_7    = portd.3
#bit    DATA_PIN_6    = portd.2
#bit    DATA_PIN_5  = portd.1
#bit    DATA_PIN_4    = portd.0

void Reiniciar_CAN() //Reiniciar CS
{
   output_high (PIN_B2); 
   output_low(PIN_B2); 
   output_high(PIN_B2); 
   delay_us (10);
   output_low(PIN_B2); 
   spi_write(0b11000000);
   output_high(PIN_B2); 
   delay_us (10);
}

void CS_high(void) //CS em nível 1
{
   output_high (PIN_B2); 
   output_low(PIN_B2);  
   output_high(PIN_B2);  
   delay_us (10);
   output_low(PIN_B2);
}

void CS_low(void) //CS em nível 0
{
   output_high(PIN_B2);
   delay_us(10);
}

void escrita(int add, int dado) //Comando escrita da comunicação SPI
{
   CS_high();
   spi_write(0b00000010); // Comando escrita
   spi_write(add); // Endereço da comunicação SPI
   spi_write(dado); //Dado a transmitir
   CS_low();
}

void leitura(int add) //Comando leitura da comunicação SPI
{
   CS_high();
   spi_write(0b00000011); // Comando leitura
   spi_write(add); // Endereço da comunicação SPI   
}

void Configuracao_CAN() //Configuração dos protocolos de transimssão CAN
{
   escrita(0x0f, 0b10000000); //CANCTRL - modo de configuração
   delay_ms(100);           
   escrita(0x28, 0x02); //CNF3      
   escrita(0x29, 0xA0); //CNF2       
   escrita(0x2a, 0x03); //CNF1      
   escrita(0x60, 0b01100100); //Mascaras e filtros ligados (com roll over)
   escrita(0x70, 0b01100000); //Mascaras e filtros ligados
   
   //Filtrar mensagem desejada: 0x470
   escrita(0x00, 0b10001110);
   escrita(0x01, 0b00000000);
   
   //Mascaras em 1
   escrita(0x20, 0xFF);
   escrita(0x21, 0xFF);
   
   escrita(0x2b, 0b00000001); //Interrupção 1
   escrita(0x2C, 0b00000000); //Interrupção 2
   escrita(0x0F, 0b00000000); //CANCTRL - modo de start   
}

void comando_lcd(char dado) {
   RS = 0;                  // seleciona o envio de um comando

   TRISD = TRISD & 0xF0;      // configura pinos D0..D3 como saídas

    DATA_PIN_7 = (unsigned char)((dado & 0x80)>>7);   //acerta dado
   DATA_PIN_6 = (unsigned char)((dado & 0x40)>>6);   //no barramento
    DATA_PIN_5 = (unsigned char)((dado & 0x20)>>5);
   DATA_PIN_4 = (unsigned char)((dado & 0x10)>>4);

    E = 1;                  // envia pulso de enable
    delay_us(1);
    E = 0;

    DATA_PIN_7 = (unsigned char)((dado & 0x08)>>3);   // acerta dado
    DATA_PIN_6 = (unsigned char)((dado & 0x04)>>2);   // no barramento
    DATA_PIN_5 = (unsigned char)((dado & 0x02)>>1);
    DATA_PIN_4 = (unsigned char)(dado & 0x01);

    E = 1;                  // envia pulso de enable
    delay_us(1);
    E = 0;

    delay_us(40);

   TRISD = TRISD | 0x0F;      // configura pinos D0..D3 como entradas
}

void limpa_lcd(void) {
   comando_lcd(0x01);         // limpa lcd
   delay_ms(2);
}

void inicializa_lcd(void) {
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

   comando_lcd(0x28);         // liga o display, sem cursor e sem blink (4 vias)

   limpa_lcd();            // limpa lcd

   comando_lcd(0x0C);         // display sem cursor

   comando_lcd(0x06);         // desloca cursor para a direita
}

void escreve_lcd(char dado) {
   RS = 1;                  // seleciona o envio de um comando

   TRISD = TRISD & 0xF0;      // configura pinos D0..D3 como saídas

    DATA_PIN_7 = (char)((dado & 0x80)>>7);   // acerta dado
   DATA_PIN_6 = (char)((dado & 0x40)>>6);   // no barramento
    DATA_PIN_5 = (char)((dado & 0x20)>>5);
   DATA_PIN_4 = (char)((dado & 0x10)>>4);

    E = 1;            // envia pulso de enable
    delay_us(1);
    E = 0;

    DATA_PIN_7 = (char)((dado & 0x08)>>3);   // acerta dado
    DATA_PIN_6 = (char)((dado & 0x04)>>2);   // no barramento
    DATA_PIN_5 = (char)((dado & 0x02)>>1);
    DATA_PIN_4 = (char)(dado & 0x01);

    E = 1;            // envia pulso de enable
    delay_us(1);
    E = 0;

    delay_us(40);

   TRISD = TRISD | 0x0F;      // configura pinos D0..D3 como entradas
}

void escreve_mensagem(char* mensagem, int length) {
   for (int i = 0; i < length; i++) {
      escreve_lcd(mensagem[i]);
      delay_ms(10);
   }
}

void SetupSPI() {
   set_tris_a(0xff);
   set_tris_b(0xdb);
   set_tris_c(0xb7); 
   set_tris_d(0x00);
   set_tris_e(0x00);
  
   setup_spi(SPI_MASTER | SPI_L_TO_H | SPI_XMIT_L_TO_H | SPI_CLK_DIV_16); //Comunicação SPI como Master, borda de subida e divisão de 16 no clock original
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_32); 
   //enable_interrupts(global|int_timer0);
   output_high (PIN_B2);   
}

void main() //Função principal do programa
{
   SetupSPI();
   
   CanReset();
   CanConfig();
   //CanFilter(0x280);
   
   inicializa_lcd();
   
   comando_lcd(0x80);
   
   char buffer[5] = "teste";
   escreve_mensagem(buffer, 5);
   
   delay_ms(500);
   
   int data = 0;
   long int id = 0;
   
   while (true)
   {
       delay_ms(50);
       if(CanKbhit()) {
        
         id = CanRead(0x61) << 3;
         id |= ((CanRead(0x62) & 0b11100000) >> 5);
         
         char buffer[16];
         comando_lcd(0x80);
         sprintf(buffer, "ID:%d%d%d", (int)((id & 0xF00) >> 16), (int)((id & 0xF0) >> 8), (int)(id & 0xF));
         escreve_mensagem(buffer, 16); 
         
         data = CanRead(0x66);
         comando_lcd(0xAF);
          for (int filtro = 0; filtro < 8; filtro++) {
            escreve_lcd(data & filtro ? '1' : '0');
            delay_ms(20);
          }
          
       } else {
         char buffer[10] = "NODATA";
         escreve_mensagem(buffer, 6);
       }
   }
}



