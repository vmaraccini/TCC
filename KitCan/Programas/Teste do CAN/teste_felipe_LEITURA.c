
#include <16F877A.h>
#include <regs_16f87x.h>
#include <stdio.h>
#use delay(clock=20000000)

#fuses NOLVP, NOWDT, NOPROTECT, hs

/* declaração das variáveis*/

int ignicao,led, teste;
int cont, b, i, data, choice;
unsigned long int temp, temp_a, timer_a;

// ENTRADAS                               *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// As entradas devem ser associadas a nomes para facilitar a programação e
// futuras alterações do hardware.
#bit	C1	= portd.0							// definições das chaves
#bit	C2	= portd.1							// chave aberta  = 1
#bit  	C3	= portd.2							// chave fechada = 0
#bit	C4	= portd.3

///* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// *                               SAÍDAS                                *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// As saídas devem ser associadas a nomes para facilitar a programação e
// futuras alterações do hardware.
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

	TRISD = TRISD & 0xF0;		// configura pinos D0..D3 como saídas

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

	TRISD = TRISD & 0xF0;		// configura pinos D0..D3 como saídas

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
 *						        Função para limpar o LCD		         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void limpa_lcd(void)
{
	comando_lcd(0x01);			// limpa lcd
	delay_ms(2);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *		     			Inicialização do Display de LCD			         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void inicializa_lcd(void)
{
    delay_ms(100);				// espera 100 milisengundos

 	TRISD = TRISD & 0xE0;		// configura pinos D0..D3 como saídas

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


void can_reset()
{
   output_high (pin_b2); // Configuração do Chip Select
   output_low(PIN_b2); // Configuração do Chip Select
   output_high(PIN_b2); // Configuração do Chip Select
   delay_us (10);
   output_low(PIN_b2); // Configuração do Chip Select
   spi_write(0b11000000);
   output_high(PIN_b2); // Configuração do Chip Select
   delay_us (10);
}

/*Configuração da Escrita no Transiver*/

void write (int end, int dado)
{
   output_high (pin_b2); // Configuração do Chip Select
   output_low(PIN_b2); // Configuração do Chip Select
   output_high(PIN_b2); // Configuração do Chip Select
   delay_us (10);
   output_low(PIN_b2); // Configuração do Chip Select
   spi_write(0b00000010); // Manda a instrução de escrita para o transiver
   spi_write(end); // Envia o endereço no qual se deseja guardar o dado
   spi_write(dado); // Envia o dado
   output_high(PIN_b2); // Configuração do Chip Select
   delay_us (10);
}

                 /*Configuração da Leitura do Transiver*/

/* Configuração do controlador CAN*/


void configuracao ()
{
   //Configuração do modo de operação

    write (0x0F, 0b10000000); // CANCTRL, coloca em modo de configuração
    delay_ms (100);

  // Configuração dos modos de sincronismo

   write (0x28, 0b00000110); // CNF3, filtro desabilitado, clock out habilitado, ps2 6 tqs
   write (0x29, 0b10110011); // CNF2, progseg 6tqs, ps1 3 tqs
   write (0x2A, 0b11000000); // CNF1, sjw 4tqs    baud rate 125khz, osc 20mhz ->16tqs

   // Configuração das máscaras e filtros

   write (0x60, 0b01100100); //mascaras e filtros desligados, com roll over
   write (0x70, 0b00000000); //mascaras e filtros desligados

   // Interrupções

   write (0x2B, 0x01); //  interrupção ligada
   write (0x2C, 0x00) ; // Habilita a leitura

   // Modo de funcionamento normal

   write (0x0F, 0x00); //CANCTRL, coloca em modo de trabalho, clkout enable, aborta trasmissões pendentes
}


#define IGN          PIN_d0
#define SETA         PIN_e1
#define LANTERNA     PIN_e0
#define FAROL_BAIXO  PIN_C0
#define FAROL_ALTO   PIN_e2

#define bit_0 0x01
#define bit_1 0x04
#define bit_2 0x10
#define bit_3 0x40
#define bit_4 0x03


// Tratamento do timer 0

#int_timer0
void trata_timer(void)
{
   set_timer0( 99 + get_timer0() ); // Começa a contagem do timer0 com 99 para que quando ocorra o estouro, o tempo seja igual a 1 ms
   if (temp) // Se tempo for igual 1 faça
   {
      temp--; // decrementa tempo
   }
   if (temp_a) // Se tempo_a for igual 1 faça
   {
      temp_a--; // decrementa tempo_a
   }
   if (timer_a) // Se timer_a for igual 1 faça
   {
      timer_a--; // decrementa timer_a
   }
}

// Inicialização do Hardware

void init_hw(void)
{
   set_TRIS_B (0b11011011); // pinos RS e RB2(CS CAB) como saída
   set_TRIS_C (0b10010111);// RC6 como saida (TX232) RC3 e RC5 (SCK e SO CAN)
   setup_spi(SPI_MASTER | SPI_L_TO_H | SPI_XMIT_L_TO_H | SPI_CLK_DIV_16); // Configura a comunicação SPI como Master, com uma atuação na borda de subida e com uma divisão de 16 no clock
   output_high (pin_b2);
   setup_ADC_ports(NO_ANALOGS); // Sem portas analógicas
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_32); // Define timer 0
   enable_interrupts(global|int_timer0); // Habilita interrupções
   temp_a = 0; // Atribui 0 a variável timer_a
   set_timer0 (0); // seta timer0 com 0
   timer_a = 0;
   led = 0;
   cont = 0;
   choice = 0;
}

//Função da Lanterna

void lanterna1 ()
{
   output_low (LANTERNA); // Coloca o estado de Lanterna em 1
}

// Farol Baixo

void farol_baixo1 ()
{
      output_low(FAROL_BAIXO); // Coloca o estado de farol_baixo em 1
}

// Farol Alto

void farol_alto1 ()
{
   output_low (FAROL_ALTO); // Coloca o estado de farol_alto em 1
}

void main(void)
{
   init_hw (); // Chama a função init_hw
   inicializa_lcd(); // Limpa LCD para escrever
	comando_lcd(0x80);   // configura a tela inicial
   printf(escreve_lcd, "PROJETO FATEC"  );
   comando_lcd(0xAF);   // configura a tela inicial
   printf(escreve_lcd, "ILUMINACAO"  );
   delay_ms (100);
   ignicao = input(IGN); // Pega o valor que está em IGN e coloca em ignicao
   can_reset (); // reseta a rede CAN para que seja possível tranferir dados por ela
 	configuracao (); // Cofiguração dos buffers de sincronismo, interrupção, máscaras e filtros
   while (1)
     {

         output_high (pin_b2); // Configuração do Chip Select
         output_low(PIN_b2); // Configuração do Chip Select
         output_high (pin_b2); // Configuração do Chip Select
         delay_us (10);
         output_low(PIN_b2); // Configuração do Chip Select
         spi_write(0b00000011); // Manda a instrução de leitura para o transiver
         spi_write(0x2C); // Envia o endereço no qual deseja receber a informação
         teste = spi_read(0);
         output_high(PIN_b2);
         delay_us (10);



         write (0x2C, 0x00); // Habilita a leitura novamente zerando o CANINTF

		   if (teste) // Se data for igual a zero, faça
         {
            write (0x2C, 0x00); // Habilita a leitura novamente zerando o CANINTF
            write (0x0C, 0x00);
            data = 0;
            output_high (pin_b2); // Configuração do Chip Select
            output_low(PIN_b2); // Configuração do Chip Select
            output_high (pin_b2); // Configuração do Chip Select
            delay_us (10);
            output_low(PIN_b2); // Configuração do Chip Select
            spi_write(0b00000011); // Manda a instrução de leitura para o transiver
            spi_write(0x66); // Envia o endereço no qual deseja receber a informação
            data = spi_read(0);
            output_high(PIN_b2);
            delay_us (10);
            write (0x2C, 0x00); // Habilita a leitura novamente zerando o CANINTF

		      timer_a = 3000; // Atribui 2000 a variável timer_a
            cont = data; // Atribui o valor de data a cont
	      }
         if (!timer_a && !teste) // Se timer_a não for igual ou maior que um e data for maior ou igual a um, faça
         {
            cont = 0x00; // Iguala cont a 0x00
         }

         switch (choice)
         {
            case 0:

            if (cont & bit_0) // Se cont & bit_0 for igual ou maior a um, faça
            {
                 if (!temp_a) // Se temp_a for zero
                  {
                     temp_a = 1000; // Atribui 1000 temp_a
                     led = !led; // Inverte o estado de led
                     output_bit (SETA,led); // Coloca em SETA o valor carregado em led
                  }
            }
      		if(~cont & bit_0) // Faz a função lógica "e", entre o inverso de cont e o bit_0, se a função for verdadeira faça
      		{
      		    output_high (SETA); // Desliga seta
      		}

            choice = choice + 1 ;
            break;

            case 1:

            if (cont & bit_1) // Faz a função lógica "e", entre cont e bit_1, se a função for verdadeira faça
            {
   		 	  lanterna1(); // Chama a função lanterna1
   		   }

   		   if (~cont & bit_1)//Faz a função lógica "e", entre o inverso de cont e o bit_1, se a função for verdadeira faça
   		   {
   		      output_high (LANTERNA); // Desliga Lanterna
   		   }

            choice = choice + 1 ;
            break;

            case 2:

            if (cont & bit_2) // Se cont & bit_2 for igual ou maior a um, faça
            {
               farol_baixo1 ();// chama a função farol_baixo1
            }
   		   if (~ cont & bit_2) // Se cont & bit_2 for igual ou maior a um, faça
            {
               output_high (FAROL_BAIXO); // Joga FAROL_BAIXO para 0
            }

            choice = choice + 1 ;
            break;

            case 3:

            if (cont & bit_3) // Se cont & bit_3 for igual ou maior a um, faça
            {
               farol_alto1 (); // chama a função farol_alto
            }
    		   if (~ cont & bit_3) // Se cont & bit_3 for igual ou maior a um, faça
            {
               output_high (FAROL_ALTO); // Joga FAROL_ALTO para 0
            }

            choice = choice + 1 ;
            break;

            case 4:

            if (cont & bit_4) // Se cont & bit_3 for igual ou maior a um, faça
            {
               if (!temp_a) // Se temp_a for zero
               {
                    temp_a = 500; // Atribui 500 temp_a
                    led=!led; // Inverte o estado de led
                    output_bit (SETA,led); // Coloca em SETA o valor carregado em led
               }
   		   }
            if(~cont & bit_0)
   		   {
   		 	  output_high (SETA);
            }

            choice = 0;
            break;
        }
    }
}


