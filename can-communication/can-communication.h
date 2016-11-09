#include <can_mcp2515.h>

typedef struct CAN_MESSAGE {
   char d0;
   char d1;
   char d2;
   char d3;
   char d4;
   char d5;
   char d6;
   char d7;
} CAN_MESSAGE;

typedef struct CAN_590 {
   unsigned long int ref_torque; // ref torque: 0 - 100 %. fator: 0.01
   unsigned long int velocidade; // fator: 0.1
   int marcha; // 0: neutro - 5
   int reserved:2;
   int pedal_freio:1;
   int pedal_embreagem:1; // 1: press, 0: solto
   int modo_cruzeiro:1;
   int modo_pedal_simulado:1;
   int ref_cruzeiro; // fator: 1
   int erro; // 0: sem erro
} CAN_590;

typedef struct CAN_470 {
   int seta_esquerda:1;
   int seta_direita:1;
   int reserved0:1;
   int pisca_alerta:1;
   int reserved1:1;
   int luz_re:1;
   int reserved2:2;
   int porta_motorista:1;
   int porta_passageiro:1;
   int porta_TE:1;
   int porta_TD:1;
   int capo:1;
   int porta_malas:1;
   int reserved3:2;
   int backlight_painel;
   char ignore[5];
} CAN_470;

typedef struct CAN_SIGNED {
   char e;
   char c;
   char u;
   char reserved[5];
} CAN_SIGNED;

typedef struct CAN_200 {
   char e;
   char c;
   char u;
   int pedal_simulado;
   char reserved[4];
} CAN_200;

typedef struct CAN_201 {
   char e;
   char c;
   char u;
   int modo_pedal_sim; // 1: liga, 0: desliga
   int modo_operacao; // 1: econom, 0: normal
   int ref_marcha_lenta; // 0 - 11; 0 = 800, MLenta = 600 + (dado - 1)*50
} CAN_201;
 
void CanStructInit(CAN_SIGNED* val) {
   val->e = 'E';
   val->c = 'C';
   val->u = 'U';
}

//Blocking function that reads a single message with a given id.
#define READ_TIMEOUT 10000 //us
#define READ_POLLING 100 //us
#define TIMEOUT_COUNT READ_TIMEOUT / READ_POLLING
int ReadMessage(Can_Id id, CAN_MESSAGE* buffer, Can_Id *recvId) {
   //Enable hardware filter
   CanFilter(id);
   
   //Wait for message
   /*unsigned long timeout = 0;
   while (!CanKbhit() && timeout <= TIMEOUT_COUNT) {  
      delay_us(READ_POLLING); 
      timeout++;
   };
   
   if (timeout >= TIMEOUT_COUNT) return 0;
   */
   
   if (!CanKbhit()) return 0;
   
   //Read 8 bytes
   char temp[8];
   for (int i = 0; i < 8; i++) {
      temp[i] = CanRead(0x66 + i);
   }
   
   //Save read address, if needed
   if (recvId > 0) {
      *recvId = 0;
      *recvId |= (unsigned long)(CanRead(0x61)) << 3;
      *recvId |= (CanRead(0x62) & 0b11100000) >> 5;
   }
   
   memcpy(buffer, temp, 8);
   
   return 1;
}

void SetupCan() {
   CanReset();
   CanConfig();
}

