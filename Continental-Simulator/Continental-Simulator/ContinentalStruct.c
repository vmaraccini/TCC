//
//  ContinentalStruct.c
//  Continental-Simulator
//
//  Created by Victor Maraccini on 6/16/16.
//  Copyright © 2016 Victor Maraccini. All rights reserved.
//

//Stats
#define NoOfTargetsNear_key "NoOfTargetsNear"
#define NoOfTargetsFar_key "NoOfTargetsFar"
#define TCIV_key "TCIV"

//Target part 1
#define NoOfTarget_1_key "NoOfTarget_1"
#define Tar_Dist_rms_key "Tar_Dist_rms"
#define Tar_Ang_rms_key "Tar_Ang_rms"
#define Tar_Vrel_rms_key "Tar_Vrel_rms"
#define Tar_Vrel_key "Tar_Vrel"
#define Tar_Dist_key "Tar_Dist"

//Target part 2
#define NoOfTarget_2_key "NoOfTarget_2"
#define Tar_PdH0_key "Tar_PdH0"
#define Tar_Length_key "Tar_Length"
#define Tar_Width_key "Tar_Width"
#define Tar_Ang_stat_key "Tar_Ang_stat"
#define Tar_Type_key "Tar_Type"
#define Tar_Ang_key "Tar_Ang"
#define Tar_RCSValue_key "Tar_RCSValue"


#include "ContinentalStruct.h"

#include <string.h>

unsigned int AddFloatToJSON(char *buffer, char *key, float value, char isLast);
unsigned int AddIntToJSON(char *buffer, char *key, unsigned int value, char isLast);
unsigned int AddToJSON(char *buffer, char *key, char *value, char isLast);

unsigned int ContinentalStructStatusToJSON(CAN_ARS308_TARGET_STATUS object, char *buffer)
{
    unsigned int len = 0;
    buffer[len++] = '{';
    len += AddIntToJSON(&buffer[len], NoOfTargetsNear_key, object.NoOfTargetsNear, 0);
    len += AddIntToJSON(&buffer[len], NoOfTargetsFar_key, object.NoOfTargetsFar, 0);
    len += AddIntToJSON(&buffer[len], TCIV_key, object.TCIV, 1);
    buffer[len++] = '}';
    buffer[len] = '\0';
    return len;
}

unsigned int ContinentalStructTarget1ToJSON(CAN_ARS308_TARGET_1 object, char *buffer)
{
    unsigned int len = 0;
    buffer[len++] = '{';
    len += AddIntToJSON(&buffer[len], NoOfTarget_1_key, object.NoOfTarget_1, 0);
    len += AddFloatToJSON(&buffer[len], Tar_Dist_rms_key, ContinentalDistanceRMSFromInt(object.Tar_Dist_rms), 0);
    len += AddFloatToJSON(&buffer[len], Tar_Ang_rms_key, ContinentalAngleRMSFromInt(object.Tar_Ang_rms), 0);
    len += AddFloatToJSON(&buffer[len], Tar_Vrel_rms_key, ContinentalVelocityRMSFromInt(object.Tar_Vrel_rms), 0);
    len += AddFloatToJSON(&buffer[len], Tar_Vrel_key, ContinentalVelocityFromInt(object.Tar_Vrel), 0);
    len += AddFloatToJSON(&buffer[len], Tar_Dist_key, ContinentalDistanceFromInt(object.Tar_Dist), 1);
    buffer[len++] = '}';
    buffer[len] = '\0';
    return len;
}

unsigned int ContinentalStructTarget2ToJSON(CAN_ARS308_TARGET_2 object, char *buffer)
{
    unsigned int len = 0;
    buffer[len++] = '{';
    len += AddIntToJSON(&buffer[len], NoOfTarget_2_key, object.NoOfTarget_2, 0);
    len += AddFloatToJSON(&buffer[len], Tar_PdH0_key, ContinentalProbabilityFromInt(object.Tar_PdH0), 0);
    len += AddFloatToJSON(&buffer[len], Tar_Length_key, ContinentalLengthFromInt(object.Tar_Length), 0);
    len += AddFloatToJSON(&buffer[len], Tar_Width_key, ContinentalWidthFromInt(object.Tar_Width), 0);
    len += AddIntToJSON(&buffer[len], Tar_Type_key, object.Tar_Type, 0);
    len += AddFloatToJSON(&buffer[len], Tar_Ang_key, ContinentalAngleFromInt(object.Tar_Ang), 0);
    len += AddIntToJSON(&buffer[len], Tar_RCSValue_key, object.Tar_RCSValue, 1);
    buffer[len++] = '}';
    buffer[len] = '\0';
    return len;
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

#pragma mark - Creation Helpers

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

