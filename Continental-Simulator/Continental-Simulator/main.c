//
//  main.c
//  Continental-Simulator
//
//  Created by Victor Maraccini on 6/16/16.
//  Copyright © 2016 Victor Maraccini. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ContinentalStruct.h"
#include "ContinentalUDPClient.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <math.h>

void CreateVehicle(float distance,
                   float angle,
                   float velocity,
                   float width,
                   float length,
                   CAN_ARS308_TARGET_1 *target1,
                   CAN_ARS308_TARGET_2 *target2);

unsigned long JSONArrayWithTarget1Objects(char *buffer, CAN_ARS308_TARGET_1 *target1Objects, unsigned int count);
unsigned long JSONArrayWithTarget2Objects(char *buffer, CAN_ARS308_TARGET_2 *target2Objects, unsigned int count);

#define IPMAXSTRSIZE    16
#define BUFFERSIZE      80
#define MSGMAXSIZE      512


//##########################################################
// main
//##########################################################

int main() {
    
    CAN_ARS308_TARGET_1 vehicle_t1[MAX_CARS];
    CAN_ARS308_TARGET_2 vehicle_t2[MAX_CARS];
    
    float distance, angle, velocity;
    float v_r, v_th, a_r, a_th;
    char buffer1[8000], buffer2[8000];
    int sd1, sd2;
    int count = -1;
    
    int                sd;                 // socket descriptor
    int                status;             // estado
    int                size;
    char               buffer[BUFFERSIZE]; // buffer temporario
    char               msg[MSGMAXSIZE];       // buffer temporario
    int                serverport;         // porta (formato host)
    struct sockaddr_in mysocketaddr;
    struct sockaddr_in fromaddr;
    struct hostent   * hostentryp;         // host entry (p/ traducao nome<->ip)
    struct servent   * serventryp;         // host entry (p/ traducao nome<->ip)
    
    mysocketaddr.sin_family = AF_INET;
    mysocketaddr.sin_addr.s_addr = INADDR_ANY;
    
    //####################
    // servico/porta
    //####################
    
    // Requisita nome do servico
    /*printf("Port: ");
     scanf("%s",buffer);
     
     // Verifica se string informada representa um inteiro
     serverport = atoi(buffer);*/
    serverport = 8081;
    if (serverport > 65535) {
        printf("Valor de porta invalido. \n");
        exit(1);
    }
    else if (serverport > 0)
        mysocketaddr.sin_port = htons((unsigned short int) serverport);
    else if (serverport <= 0) {
        // String informada nao e' um inteiro
        serventryp = getservbyname(buffer,"udp");
        if (serventryp != NULL) {
            mysocketaddr.sin_port = serventryp->s_port;
        }
        else {
            printf("Nome do servico (ou porta) invalido. \n");
            exit(1);
        }
    }
    
    //####################
    // imprime IP e porta
    //####################
    printf("Endereco IP do servidor = %s \n", inet_ntop(AF_INET,&mysocketaddr.sin_addr,buffer,BUFFERSIZE));
    printf("Porta do servidor = %hu \n", ntohs(mysocketaddr.sin_port));
    
    /****************************************************************/
    /* Socket(): Criacao do socket                                  */
    /****************************************************************/
    sd = socket(PF_INET,SOCK_DGRAM,0);
    if (sd < 0) {
        printf("Erro na criacao do socket. \n");
        perror("Descricao do erro");
        exit(1);
    }
    
    /****************************************************************/
    /* Bind(): associa o socket a um IP e a uma porta               */
    /****************************************************************/
    status = bind(sd,(struct sockaddr *)&mysocketaddr,sizeof(struct sockaddr_in));
    if (status < 0) {
        perror("Erro na chamada bind()");
        exit(1);
    }
    
    while (1) {
        
        size = sizeof(fromaddr);
        status = recvfrom(sd, msg, MSGMAXSIZE, 0, (struct sockaddr *)&fromaddr, &size);
        if (status < 0) {
            perror("ERRO no recebimento de datagramas UDP \n");
            exit(1);
        }
        
        char* line = strtok(strdup(buffer), "\n");
        while (line) {
            if (count < 0)
                sscanf(line, "%d", &count);
            else
                sscanf(line, "%f,%f,%f,%f,%f,%f", &distance, &angle, &v_r, &v_th, &a_r, &a_th);
            line  = strtok(NULL, "\n");
        }
        
        velocity = v_r * cos(v_th - angle);
        
        for (int i = 0; i < count; i++) {
            CreateVehicle(distance, angle, velocity, CAR_WIDTH, CAR_LENGTH, &vehicle_t1[i], &vehicle_t2[i]);
        }
        
        JSONArrayWithTarget1Objects(buffer1, vehicle_t1, count);
        JSONArrayWithTarget2Objects(buffer2, vehicle_t2, count);
        
        connectTo("127.0.0.1", 25000, &sd1); //Object1
        sendMessage(buffer1, sd1);
        
        usleep(1E3);
        
        connectTo("127.0.0.1", 25001, &sd2); //Object2
        sendMessage(buffer2, sd2);
        
        fflush(stdout);
        
    }
    
    closeSocket(sd1);
    closeSocket(sd2);
    
}


#pragma mark - Creation utilities

void CreateVehicle(float distance,
                   float angle,
                   float velocity,
                   float width,
                   float length,
                   CAN_ARS308_TARGET_1 *target1,
                   CAN_ARS308_TARGET_2 *target2)
{
    target1->Tar_Dist = ContinentalIntFromDistance(distance);
    target1->Tar_Vrel = ContinentalIntFromVelocity(velocity);
    target1->Tar_Dist_rms = ContinentalIntFromDistanceRMS(0);
    target1->Tar_Ang_rms = ContinentalIntFromAngleRMS(0);
    target1->Tar_Vrel_rms = ContinentalIntFromVelocityRMS(0);
    
    target2->Tar_Ang = ContinentalIntFromAngle(angle);
    target2->Tar_Width = ContinentalIntFromWidth(width);
    target2->Tar_Length = ContinentalIntFromLength(length);
    target2->Tar_Ang_stat = CAN_ARS308_ANGLE_STATUS_EXPANDED;
    target2->Tar_Type = velocity < 0 ? CAN_ARS308_TARGET_TYPE_INCOMING : CAN_ARS308_TARGET_TYPE_TRAFFIC;
}

#pragma mark - JSON utilities

unsigned long JSONArrayWithTarget1Objects(char *buffer, CAN_ARS308_TARGET_1 *target1Objects, unsigned int count)
{
    unsigned int len = 0;
    buffer[len++] = '[';
    for (unsigned int i = 0; i < count; i++) {
        CAN_ARS308_TARGET_1 target1 = target1Objects[i];
        len += ContinentalStructTarget1ToJSON(target1, &buffer[len]);
        if (i < count - 1) {
            buffer[len++] = ',';
        }
    }
    buffer[len++] = ']';
    buffer[len++] = '\0';
    return len;
}

unsigned long JSONArrayWithTarget2Objects(char *buffer, CAN_ARS308_TARGET_2 *target2Objects, unsigned int count)
{
    unsigned int len = 0;
    buffer[len++] = '[';
    for (unsigned int i = 0; i < count; i++) {
        CAN_ARS308_TARGET_2 target2 = target2Objects[i];
        len += ContinentalStructTarget2ToJSON(target2, &buffer[len]);
    }
    buffer[len++] = ']';
    buffer[len++] = '\0';
    return len;
}