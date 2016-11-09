/////////////////////////////////////////////////////////////////////////
////                         EX_CAN_CCS_B.C                          ////
////                                                                 ////
//// Example of CCS's MCP2510 CAN library.  This example was tested  ////
//// using MCP250xxx CCS's CAN Prototype board.                      ////
////                                                                 ////
//// This example provides the firmware for the B node in CCS's      ////
//// CAN prototyping board.  Node B responds to CAN ID 0x202         ////
//// by setting the 3 LEDs to the value transmitted by Node A.       ////
//// Node B also repsonds to requests from CAN ID 0x201 by           ////
//// transmitting an A/D reading.                                    ////
////                                                                 ////
//// Using a serial port, this example also shows all traffic on the ////
//// CAN bus as received by the MCP2510.                             ////
////                                                                 ////
//// For more documentation on the MPC2510 CAN library, see          ////
//// can-mcp2510.c                                                   ////
////                                                                 ////
//// For more documentation on the CCS Can Prototype board see       ////
//// ex_can_ccs_a.c                                                  ////
////                                                                 ////
////  Jumpers:                                                       ////
////     PCM,PCH    pin C7 to RS232 RX, pin C6 to RS232 TX           ////
////                                                                 ////
////  This example will work with the PCM compiler.                  ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2003 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

//#include <16F877A.h>
//#fuses HS,NOPROTECT,NOLVP,NOWDT
//#use delay(clock=2500000)
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#define CAN_DO_DEBUG TRUE
#define BUFFER_SIZE 8

#include "pic16f877a.h"
#include "mcp2515b.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MIN(x, y) x > y ? y : x
#define MAX(x, y) x > y ? x : y

#define RESPOND_TO_ID_AD   0x201
#define RESPOND_TO_ID_LED  0x202

//Function prototypes
void sendECUMessage(char *buffer, char bufferSize, int id);
void sendThrottleReference(char reference);

//PIC-specific routines
void setupTimers();

//Definitions
#define ID_THROTTLE_REFERENCE 0X200

int main() {
    int buffer[BUFFER_SIZE];
    int i;
    
    for(i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }
    
    setupTimers();
    can_init();
}

void setupTimers() {
    setup_timer_2(T2_DIV_BY_16, 195, 16);
    enable_interrupts(int_timer2);
}

#int_timer2
void isr() {
    
}

//Prefixes 'ECU' and sends the rest of the payload.
void sendECUMessage(char *buffer, char bufferSize, int id) {
    char *result = "ECU\0\0\0\0\0";
    
    int payloadSize = MIN(bufferSize, 5);
    memcpy(&result[3], buffer, payloadSize);
    can_putd(id, result, 8, 1, 0, 0);
}

//Sends pedal reference padded with zeros at the end.
void sendThrottleReference(char reference) {
    sendECUMessage(&reference, 1, ID_THROTTLE_REFERENCE);
}
