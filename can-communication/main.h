#include <16F877A.h>
#device ADC=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=20000000)

void SetupCommunication() {
   set_tris_a(0xff);
   set_tris_d(0x00);
   set_tris_e(0x00);
   set_TRIS_B (0b11011011); // pinos RS e RB2(CS CAB) como sa�da
   set_TRIS_C (0b10010111); // RC6 como saida (TX232) RC3 e RC5 (SCK e SO CAN)
   setup_spi(SPI_MASTER | SPI_L_TO_H | SPI_XMIT_L_TO_H | SPI_CLK_DIV_16); // Configura a comunica��o SPI como Master, com uma atua��o na borda de subida e com uma divis�o de 16 no clock
   enable_interrupts(global|int_timer0); // Habilita interrup��es
}

void SetupTimers() {
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_32); // Define timer 0
}

