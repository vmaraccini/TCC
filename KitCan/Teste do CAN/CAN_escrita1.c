                    /*Bibliotecas*/

#include <16F877A.h>
#include <regs_16f87x.h>
#include <stdio.h>
#use delay (clock=20000000)

                    /* fus�veis*/

#fuses HS,NOWDT,PUT,BROWNOUT,NOLVP

  // ENTRADAS                               *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// As entradas devem ser associadas a nomes para facilitar a programa��o e
// futuras altera��es do hardware.
#bit	C1	= portd.0							// defini��es das chaves
#bit	C2	= portd.1							// chave aberta  = 1
#bit  	C3	= portd.2							// chave fechada = 0
#bit	C4	= portd.3

///* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// *                               SA�DAS                                *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// As sa�das devem ser associadas a nomes para facilitar a programa��o e
// futuras altera��es do hardware.
#bit  	RS    		= portb.5
#bit  	E	    	= portd.4
#bit 	DATA_PIN_7 	= portd.3
#bit 	DATA_PIN_6 	= portd.2
#bit 	DATA_PIN_5  = portd.1
#bit 	DATA_PIN_4 	= portd.0

///* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// *                               SUBROTINAS                            *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *					Rotina que envia um COMANDO para o LCD		         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void comando_lcd(char dado)
{
	RS = 0;						// seleciona o envio de um comando

	TRISD = TRISD & 0xF0;		// configura pinos D0..D3 como sa�das

    DATA_PIN_7 = (unsigned char)((dado & 0x80)>>7);	//acerta dado
	DATA_PIN_6 = (unsigned char)((dado & 0x40)>>6);	//no barramento
    DATA_PIN_5 = (unsigned char)((dado & 0x20)>>5);
	DATA_PIN_4 = (unsigned char)((dado & 0x10)>>4);

    E = 1;						// envia pulso de enable
    delay_us(1);
    E = 0;

    DATA_PIN_7 = (unsigned char)((dado & 0x08)>>3);	// acerta dado
    DATA_PIN_6 = (unsigned char)((dado & 0x04)>>2);	// no barramento
    DATA_PIN_5 = (unsigned char)((dado & 0x02)>>1);
    DATA_PIN_4 = (unsigned char)(dado & 0x01);

    E = 1;						// envia pulso de enable
    delay_us(1);
    E = 0;

    delay_us(40);

	TRISD = TRISD | 0x0F;		// configura pinos D0..D3 como entradas
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *				Rotina que envia um DADO a ser escrito no LCD	         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void escreve_lcd(char dado)
{
	RS = 1;						// seleciona o envio de um comando

	TRISD = TRISD & 0xF0;		// configura pinos D0..D3 como sa�das

    DATA_PIN_7 = (char)((dado & 0x80)>>7);	// acerta dado
	DATA_PIN_6 = (char)((dado & 0x40)>>6);	// no barramento
    DATA_PIN_5 = (char)((dado & 0x20)>>5);
	DATA_PIN_4 = (char)((dado & 0x10)>>4);

    E = 1;				// envia pulso de enable
    delay_us(1);
    E = 0;

    DATA_PIN_7 = (char)((dado & 0x08)>>3);	// acerta dado
    DATA_PIN_6 = (char)((dado & 0x04)>>2);	// no barramento
    DATA_PIN_5 = (char)((dado & 0x02)>>1);
    DATA_PIN_4 = (char)(dado & 0x01);

    E = 1;				// envia pulso de enable
    delay_us(1);
    E = 0;

    delay_us(40);

	TRISD = TRISD | 0x0F;		// configura pinos D0..D3 como entradas
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *						        Fun��o para limpar o LCD		         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void limpa_lcd(void)
{
	comando_lcd(0x01);			// limpa lcd
	delay_ms(2);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		     			Inicializa��o do Display de LCD			         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void inicializa_lcd(void)
{
    delay_ms(100);				// espera 100 milisengundos

 	TRISD = TRISD & 0xE0;		// configura pinos D0..D3 como sa�das

    PORTD = 0x03;

	E = 1 ;				        // gera pulso no enable
	delay_us(1);				// espera 1 microsegundo
	E = 0;				     	// desce o pino de enable

	delay_ms(5);				// espera 5 milisengundos

	E = 1 ;						// gera pulso no enable
	delay_us(1);				// espera 1 microsegundo
	E = 0;						// desce o pino de enable

	delay_us(100);				// espera 100 microsengundos

	E = 1 ;						// gera pulso no enable
	delay_us(1);				// espera 1 microsegundo
	E = 0;						// desce o pino de enable

	delay_us(40);				// espera 40 microsegundos

    PORTD = 0x02;

	E = 1 ;						// gera pulso no enable
	delay_us(1);				// espera 1 microsegundo
	E = 0;						// desce o pino de enable

	delay_us(40);				// espera 40 microsegundos

	comando_lcd(0x28);			// liga o display, sem cursor e sem blink (4 vias)

	limpa_lcd();				// limpa lcd

	comando_lcd(0x0C);			// display sem cursor

	comando_lcd(0x06);			// desloca cursor para a direita
}
                 /* declara��o das vari�veis*/
int data;

void can_reset()
{
   output_high (pin_e2); // apaga o led
   output_high (pin_b2); // Configura��o do Chip Select
   output_low(PIN_b2); // Configura��o do Chip Select
   output_high(PIN_b2); // Configura��o do Chip Select
   delay_us (10);
   output_low(PIN_b2); // Configura��o do Chip Select
   spi_write(0b11000000);
   output_high(PIN_b2); // Configura��o do Chip Select
   delay_us (10);
}
                  /*Configura��o da Escrita no Transiver*/

void write (int end, int dado)
{
   output_high (pin_b2); // Configura��o do Chip Select
   output_low(PIN_b2); // Configura��o do Chip Select
   output_high(PIN_b2); // Configura��o do Chip Select
   delay_us (10);
   output_low(PIN_b2); // Configura��o do Chip Select
   spi_write(0b00000010); // Manda a instru��o de escrita para o transiver
   spi_write(end); // Envia o endere�o no qual se deseja guardar o dado
   spi_write(dado); // Envia o dado
   output_high(PIN_b2); // Configura��o do Chip Select
   delay_us (10);
}

                 /*Configura��o da Leitura do Transiver*/

void read (int end)
{
   output_high (pin_b2); // Configura��o do Chip Select
   output_low(PIN_b2); // Configura��o do Chip Select
   output_high (pin_b2); // Configura��o do Chip Select
   delay_us (10);
   output_low(PIN_b2); // Configura��o do Chip Select
   spi_write(0b00000011); // Manda a instru��o de leitura para o transiver
   spi_write(end); // Envia o endere�o no qual deseja receber a informa��o
   data = spi_read(0);
   output_high (pin_b2);
   delay_us (10);
}

                 /* Configura��o do controlador CAN*/


void configuracao ()
{
   //Configura��o do modo de opera��o

   write (0x0F, 0b10000000); // CANCTRL, coloca em modo de configura��o
   delay_ms (100);

  // Configura��o dos modos de sincronismo

   write (0x28, 0b00000110); // CNF3, filtro desabilitado, clock out habilitado, ps2 6 tqs
   write (0x29, 0b10110011); // CNF2, progseg 6tqs, ps1 3 tqs
   write (0x2A, 0b11000000); // CNF1, sjw 4tqs    baud rate 125khz, osc 20mhz ->16tqs

   // Interrup��es

   write (0x2B, 0x10); //  interrup��es

   // Modo de funcionamento normal
   write (0x2C, 0x00); // Zera as interrup��es e libera a mensagem para ser enviada
   write (0x0F, 0x00); //CANCTRL, coloca em modo de trabalho
}


                  /* Inicializa��o do Hardware*/



void main ()
{
   int data;
   set_TRIS_B (0b11011011); // pinos RS e RB2(CS CAB) como sa�da
   set_TRIS_C (0b10010111);// RC6 como saida (TX232) RC3 e RC5 (SCK e SO CAN)
   setup_spi(SPI_MASTER | SPI_L_TO_H | SPI_XMIT_L_TO_H | SPI_CLK_DIV_16); // Configura a comunica��o SPI como Master, com uma atua��o na borda de subida e com uma divis�o de 16 no clock
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_32); // Define timer 0
   enable_interrupts(global|int_timer0); // Habilita interrup��es
   can_reset ();
   configuracao ();
   
   inicializa_lcd();
	comando_lcd(0x80);
   printf(escreve_lcd, "PROJETO FATEC");
   comando_lcd(0xAF);
   printf(escreve_lcd, "TESTE ESCRITA");
   delay_ms (100);

   while (true)
   {
       
   inicializa_lcd();
	comando_lcd(0x80);
   printf(escreve_lcd, "PROJETO FATEC");
   comando_lcd(0xAF);
   printf(escreve_lcd, "TESTE ESCRITA");
   delay_ms (100);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x01); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (2);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x01); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (5);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x01); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (10);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x01); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (3000);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x04); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (2);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x04); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (5);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x04); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (10);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x04); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (3000);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x10); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (2);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x10); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (5);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x10); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (10);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x10); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (3000);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x40); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (2);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x40); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (5);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x40); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (10);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x40); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (3000);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x03); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (2);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x03); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (5);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x03); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (10);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0x03); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (3000);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0xFF); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (2);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0xFF); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (5);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0xFF); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (10);
       write (0x50, 0x03); // Mesnagem de prioridade maior
       write (0x51, 0xA4); // ID da mensagem
       write (0x52, 0x40); // ID da mensagem
       write (0x55, 0x01); // Define o DLC para 1 byte (tamanho do dado enviado)
       write (0x56, 0xFF); // Endere�o onde a mesagem ser� guardada
		 write (0x50, 0b00001011); // Instru��o para enviar a mensagem
       write (0x50, 0b00000011);
		 write (0x2C , 0x00);
       delay_ms (3000);

   }
}
