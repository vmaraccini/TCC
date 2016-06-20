//
//  ContinentalStruct.h
//  Continental-Simulator
//
//  Created by Victor Maraccini on 6/16/16.
//  Copyright © 2016 Victor Maraccini. All rights reserved.
//

#ifndef ContinentalStruct_h
#define ContinentalStruct_h

#include <stdio.h>
#include "Common.h"

typedef enum {
    CAN_ARS308_ANGLE_STATUS_EXPANDED = 0,
    CAN_ARS308_ANGLE_STATUS_POINT,
    CAN_ARS308_ANGLE_STATUS_DIGITAL,
    ///No target, ignore.
    CAN_ARS308_ANGLE_STATUS_INVALID,
} CAN_ARS308_ANGLE_STATUS;

typedef enum {
    ///No target, ignore.
    CAN_ARS308_TARGET_TYPE_INVALID = 0,
    ///Incoming object (relative velocity < 0)
    CAN_ARS308_TARGET_TYPE_INCOMING,
    ///Stationary object
    CAN_ARS308_TARGET_TYPE_STATIONARY,
    ///Moving in the same direction
    CAN_ARS308_TARGET_TYPE_TRAFFIC,
} CAN_ARS308_TARGET_TYPE;

typedef struct CAN_ARS308_TARGET_STATUS { //Message ID 0x600
    ///Number of valid targets in near-range
    ///@discussion Range: 0 - 255, typical 1 to 96.
    unsigned char NoOfTargetsNear;
    ///Number of valid targets in near-range
    ///@discussion Range: 0 - 255, typical 1 to 96.
    unsigned char NoOfTargetsFar;
    unsigned char TCIV;
} CAN_ARS308_TARGET_STATUS;

/**Unified ARS Target structure Part 1
 @discussion For each cycle the status message and 96 message pairs 
 (CAN1_Target_1, CAN1_Target_2) for identified targets are sent. 
 The first 32 targets are from near range, the following 64 targets are from far range.*/
typedef struct CAN_ARS308_TARGET_1 { //Message ID 0x701
    ///Target number
    ///@discussion Range: 0 - 255, typical 1 to 96.
    unsigned char NoOfTarget_1;
    ///Target range standard deviation
    ///@discussion 0.1 m resolution, -10m offset, range: -10 to +15.5m
    unsigned char Tar_Dist_rms;
    ///Target angle standard deviation
    ///@discussion 0.1 deg resolution, range: 0 to 12.7 deg.
    unsigned char Tar_Ang_rms : 7;
    ///Target relative velocity standard deviation
    ///@discussion 0.03 m/s resolution, -5 m/s offset, range: -5 to 10.33 m/s
    unsigned int Tar_Vrel_rms : 9;
    ///Target relative velocity
    ///@discussion 0.03 m/s resolution, -25 m/s offset, range: -25 to 97.85 m/s
    unsigned int Tar_Vrel : 12;
    ///Target distance (range)
    ///@discussion 0.1 m resolution, range: 0 to 204.7m
    unsigned int Tar_Dist : 11;
} CAN_ARS308_TARGET_1;

/**Unified ARS Target structure Part 2
 @discussion For each cycle the status message and 96 message pairs
 (CAN1_Target_1, CAN1_Target_2) for identified targets are sent.
 The first 32 targets are from near range, the following 64 targets are from far range.*/
typedef struct CAN_ARS308_TARGET_2 { //Message ID 0x702
    ///Target number
    ///@discussion Range: 0 - 255, typical 1 to 96.
    unsigned char NoOfTarget_2;
    ///False alarm probability
    ///@discussion Resolution: 1%. Range: 0 to 127 %
    unsigned char Tar_PdH0 : 7;
    ///Target length
    ///@discussion Resolution: 0.1 m. Range: 0 to 51.1m
    unsigned int Tar_Length : 9;
    ///Target width
    ///@discussion Resolution: 0.1 m. Range: 0 to 51.1m
    unsigned int Tar_Width : 9;
    ///Target angle status
    ///@see CAN_ARS308_ANGLE_STATUS
    CAN_ARS308_ANGLE_STATUS Tar_Ang_stat : 2;
    ///Target type
    ///@see CAN_ARS308_TARGET_TYPE
    CAN_ARS308_TARGET_TYPE Tar_Type : 2;
    ///Target angle
    ///@discussion Resolution: 0.1 deg, -30 deg offset. Range: -30 to 72.3 deg
    unsigned int Tar_Ang : 10;
    ///Radar cross-section
    ///@discussion Resolution: 0.1 dBm^2, -50 dBm^2 offset. Range: -50 to 52.3 dBm^2
    unsigned int Tar_RCSValue : 10;
} CAN_ARS308_TARGET_2;

unsigned int ContinentalStructStatusToJSON(CAN_ARS308_TARGET_STATUS object, char *buffer);
unsigned int ContinentalStructTarget1ToJSON(CAN_ARS308_TARGET_1 object, char *buffer);
unsigned int ContinentalStructTarget2ToJSON(CAN_ARS308_TARGET_2 object, char *buffer);

#pragma mark - Helper functions

//Velocity
#define ContinentalVelocityFromInt(x) (-25 + 0.03f*x)
#define ContinentalIntFromVelocity(x) ((x+25)/.03f)

#define ContinentalVelocityRMSFromInt(x) (-5 + 0.03f*x)
#define ContinentalIntFromVelocityRMS(x) ((x+5)/.03f)

//Distance
#define ContinentalDistanceFromInt(x) 0.1f*x
#define ContinentalIntFromDistance(x) 10*x

#define ContinentalDistanceRMSFromInt(x) (-10 + 0.1f*x)
#define ContinentalIntFromDistanceRMS(x) 10*(x+10)

//Length
#define ContinentalLengthFromInt(x) ContinentalDistanceFromInt(x)
#define ContinentalIntFromLength(x) ContinentalIntFromDistance(x)

#define ContinentalWidthFromInt(x) ContinentalDistanceFromInt(x)
#define ContinentalIntFromWidth(x) ContinentalIntFromDistance(x)


//Angle
#define ContinentalAngleFromInt(x) (-30 + 0.1f*x)
#define ContinentalIntFromAngle(x) ((x + 30)/0.1f)

#define ContinentalAngleRMSFromInt(x) 0.1f*x
#define ContinentalIntFromAngleRMS(x) (x/0.1f)

//Probability
#define ContinentalProbabilityFromInt(x) (x/100.f)
#define ContinentalIntFromProbability(x) x*100

#endif /* ContinentalStruct_h */
