typedef unsigned long int Can_Id;

//******************** BASE ROUTINES ********************

int CanKbhit(void) {
   int teste;
   output_high (PIN_B2); // Configuração do Chip Select
   output_low(PIN_B2); // Configuração do Chip Select
   output_high (PIN_B2); // Configuração do Chip Select
   delay_us (10);
   output_low(PIN_B2); // Configuração do Chip Select
   spi_write(0b00000011); // Manda a instrução de leitura para o transceiver
   spi_write(0x2C); // Envia o endereço no qual deseja receber a informação
   teste = spi_read(0); //informa se chegou um dado no receptor, registrador CANINTF
   output_high(PIN_B2);
   delay_us (10);
   return teste&0x01;
}

void CanWrite (int end, int dado) {
   output_high (PIN_B2); // Configuração do Chip Select
   output_low(PIN_B2); // Configuração do Chip Select
   output_high(PIN_B2); // Configuração do Chip Select
   delay_us (10);
   output_low(PIN_B2); // Configuração do Chip Select
   spi_write(0b00000010); // Manda a instrução de escrita para o transceiver
   spi_write(end); // Envia o endereço no qual se deseja guardar o dado
   spi_write(dado); // Envia o dado
   output_high(PIN_B2); // Configuração do Chip Select
   delay_us (10);
}

void CanFilter (Can_Id id) {

   CanWrite (0x0F, 0b10000000); // CANCTRL, coloca em modo de configuração

   int mask_on = 0;
   if (id == 0) mask_on |= 0b01100000;

   //Setar filtros
   CanWrite(0x60, 0b00000000 | mask_on);
   CanWrite(0x70, 0b00000000 | mask_on);
   
    //Mascaras em 1
   CanWrite(0x20, 0xFF);
   //CanWrite(0x21, 0xFF);
   
   unsigned char IDR0_filter = 0; // Contem a mascara para o registrador IDR0
   unsigned char IDR1_filter = 0; // Contem a mascara para o registrador IDR1
   IDR0_filter |= (unsigned char)((id >> 3) & 0xFF); // ID10 a ID3
   IDR1_filter |= (unsigned char)((id & 0b0000000000000111) << 5); // ID2 a ID0 + (IDE = 0)
   
   //Filtrar mensagem desejada
   CanWrite(0x00, IDR0_filter);
   CanWrite(0x01, IDR1_filter);
   
   CanWrite (0x0F, 0x00); // CANCTRL, coloca em modo de trabalho
}

void CanReset(void) {
   output_high (PIN_B2); // Configuração do Chip Select
   output_low(PIN_B2); // Configuração do Chip Select
   output_high(PIN_B2); // Configuração do Chip Select
   delay_us (10);
   output_low(PIN_B2); // Configuração do Chip Select
   spi_write(0b11000000);
   output_high(PIN_B2); // Configuração do Chip Select
   delay_us (10);
}
              
//******************** RX ROUTINES ********************

int CanRead (int end) {
   int data;
   CanWrite (0x2C, 0x00); // Habilita a leitura novamente zerando o CANINTF
   output_high (PIN_B2); // Configuração do Chip Select
   output_low(PIN_B2); // Configuração do Chip Select
   output_high (PIN_B2); // Configuração do Chip Select
   delay_us (10);
   output_low(PIN_B2); // Configuração do Chip Select
   spi_write(0b00000011); // Manda a instrução de leitura para o transceiver
   spi_write(end); // Envia o endereço no qual deseja receber a informação
   data = spi_read(0);
   output_high (PIN_B2);
   delay_us (10);
   return data;
}

void CanConfig (void) {
   //Configuração do modo de operação
   CanWrite (0x0F, 0b10000000); // CANCTRL, coloca em modo de configuração
   delay_ms (100);

   //Configuração dos modos de sincronismo
   CanWrite (0x28, 0x02); // CNF3, filtro desabilitado, clock out habilitado, ps2 3 tqs
   CanWrite (0x29, 0xA0); // CNF2, progseg 1tqs, ps5 3 tqs
   CanWrite (0x2A, 0x01); // CNF1, sjw 1tqs    baud rate 500khz, osc 20mhz ->10tqs
   
   // Interrupções
   CanWrite (0x2B, 0x01); //  interrupção ligada
   CanWrite (0x2C, 0x00); // Habilita a leitura

   // Modo de funcionamento normal
   CanWrite (0x0F, 0x00); // CANCTRL, coloca em modo de trabalho
   
   //Seta DLC da Mensagem de Transmissão 8 bytes
   CanWrite (0x35,0x08);
}

//******************** TX ROUTINES ********************

void CanSetSendAddress(unsigned long int address) {
   // Set message id
   int idHigh = (unsigned char)((address >> 3) & 0xFF);
   int idLow = (unsigned char)((address & 0b0000000000000111) << 5);
   
   CanWrite(0x31, idHigh);
   CanWrite(0x32, idLow);
}

void CanSend(void) {
   CanWrite (0x30, 0b00001000); // Instrução para enviar a mensagem, TXrequest
   CanWrite (0x30, 0b00000000); // Instrução para não enviar mais mensagem
   CanWrite (0x2C, 0x00); // Habilita a leitura novamente zerando o CANINTF
}

//Sends a char[8] message
void SendCanFrame(char* frame) {
   for (int i = 0; i < 8; i++) {
      CanWrite(0x36 + i, frame[i]);
   }
   CanSend();
}

