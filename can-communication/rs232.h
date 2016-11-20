
void SetupUART() {
   SYNC = 0; //Selecting Asynchronous Mode
   SPEN = 1; //Enables Serial Port
   CREN = 1; //Enables Continuous Reception
   TXEN = 1; //Enables Transmission
}

void UART_Write(char data) {
   while(!TRMT); //Waiting for Previous Data to Transmit completly
   TXREG = data; //Set transmit register; triggers transmission automatically.
}

char UART_TX_Empty() {
  return TRMT; //Returns Transmit Shift Status bit
}

void UART_Write_Text(char *text) {
  int i;
  for(i=0;text[i]!='\0';i++) //Expect a null-terminated string
    UART_Write(text[i]);
}

char UART_Read() {
  while(!RCIF); //Waits for Reception to complete
  CREN = 0;
  CREN = 1;
  return RCREG; //Returns the 8 bit data
}
