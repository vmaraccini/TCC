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

void CreateVehicle(float distance,
                   float angle,
                   float velocity,
                   float width,
                   float length,
                   CAN_ARS308_TARGET_1 *target1,
                   CAN_ARS308_TARGET_2 *target2);

unsigned long JSONArrayWithTarget1Objects(char *buffer, CAN_ARS308_TARGET_1 **target1Objects, unsigned int count);
unsigned long JSONArrayWithTarget2Objects(char *buffer, CAN_ARS308_TARGET_2 **target2Objects, unsigned int count);

int main(int argc, const char * argv[]) {

    CAN_ARS308_TARGET_1 vehicle1_t1;
    CAN_ARS308_TARGET_2 vehicle1_t2;
    CreateVehicle(40,
                  3,
                  5,
                  1.5,
                  2,
                  &vehicle1_t1,
                  &vehicle1_t2);
    
    CAN_ARS308_TARGET_1 vehicle2_t1;
    CAN_ARS308_TARGET_2 vehicle2_t2;
    
    CreateVehicle(50,
                  5,
                  1,
                  1.5,
                  2,
                  &vehicle2_t1,
                  &vehicle2_t2);
    
    char buffer1[3000];
    char buffer2[3000];
    CAN_ARS308_TARGET_1 *target1Objects[2] = {&vehicle1_t1, &vehicle2_t1};
    CAN_ARS308_TARGET_2 *target2Objects[2] = {&vehicle1_t2, &vehicle2_t2};
    
    int sd1, sd2;
    
    int stepUs = 30E3;
    float step = stepUs * 1E-6;
    
    float a = -6; //m/s^2
    
    for (int i = 0; i <= 300; i++) {
        float newDist = ContinentalDistanceFromInt(vehicle1_t1.Tar_Dist) + ContinentalVelocityFromInt(vehicle1_t1.Tar_Vrel) * step;
        float newVel = ContinentalVelocityFromInt(vehicle1_t1.Tar_Vrel) + a * step;
        newVel = newVel < -20 ? -20 : newVel;
        
        vehicle1_t1.Tar_Dist = ContinentalIntFromDistance(newDist);
        vehicle1_t1.Tar_Vrel = ContinentalIntFromVelocity(newVel);
        
        JSONArrayWithTarget1Objects(buffer1, target1Objects, 2);
        JSONArrayWithTarget2Objects(buffer2, target2Objects, 2);
        
        connectTo("127.0.0.1", 25000, &sd1); //Object1
        sendMessage(buffer1, sd1);
        
        usleep(1E3);
        connectTo("127.0.0.1", 25001, &sd2); //Object2
        sendMessage(buffer2, sd2);
        
        closeSocket(sd1);
        closeSocket(sd2);
        
        usleep(stepUs);
    }
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

unsigned long JSONArrayWithTarget1Objects(char *buffer, CAN_ARS308_TARGET_1 **target1Objects, unsigned int count)
{
    unsigned int len = 0;
    buffer[len++] = '[';
    for (unsigned int i = 0; i < count; i++) {
        CAN_ARS308_TARGET_1 target1 = *target1Objects[i];
        len += ContinentalStructTarget1ToJSON(target1, &buffer[len]);
        if (i < count - 1) {
            buffer[len++] = ',';
        }
    }
    buffer[len++] = ']';
    buffer[len++] = '\0';
    return len;
}

unsigned long JSONArrayWithTarget2Objects(char *buffer, CAN_ARS308_TARGET_2 **target2Objects, unsigned int count)
{
    unsigned int len = 0;
    buffer[len++] = '[';
    for (unsigned int i = 0; i < count; i++) {
        CAN_ARS308_TARGET_2 target2 = *target2Objects[i];
        len += ContinentalStructTarget2ToJSON(target2, &buffer[len]);
    }
    buffer[len++] = ']';
    buffer[len++] = '\0';
    return len;
}