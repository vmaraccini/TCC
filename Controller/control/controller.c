//
//  controller.c
//  Controller
//
//  Created by Victor Gabriel Maraccini & Denis Isidoro de Franca
//  Copyright © 2016 TCC. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "controller.h"

#define BUFFER_SIZE CONTROLLER_BUFFER_SIZE
#define BUFFER_TYPE CONTROLLER_BUFFER_TYPE

double inputBuffer[BUFFER_SIZE];
double outputBuffer[BUFFER_SIZE];

double inputCoeffs[BUFFER_SIZE] = {2.964, -2.908};
double outputCoeffs[BUFFER_SIZE] = {0.9601, 0, 0, 0, 0, 0.03921};

//Prototypes
void scalarProduct(BUFFER_TYPE *one, BUFFER_TYPE *other, BUFFER_TYPE *resultBuffer);
BUFFER_TYPE arraySum(BUFFER_TYPE *one);

void updateSample(BUFFER_TYPE newSample, BUFFER_TYPE* buffer);
BUFFER_TYPE calculateOutput(BUFFER_TYPE* input, BUFFER_TYPE* output);

void printArray(BUFFER_TYPE *array);

double controller(double newSample) {
    updateSample(newSample, inputBuffer);
    BUFFER_TYPE newOutput = calculateOutput(inputBuffer, outputBuffer);
    updateSample(newOutput, outputBuffer);
    
    return newOutput;
}

BUFFER_TYPE calculateOutput(BUFFER_TYPE* input, BUFFER_TYPE* output) {
    BUFFER_TYPE inputTemp[BUFFER_SIZE];
    BUFFER_TYPE outputTemp[BUFFER_SIZE];
    
    scalarProduct(input, inputCoeffs, inputTemp);
    scalarProduct(output, outputCoeffs, outputTemp);
    
    return arraySum(inputTemp) + arraySum(outputTemp);
}

void updateSample(BUFFER_TYPE newSample, BUFFER_TYPE* buffer) {
    BUFFER_TYPE temp[BUFFER_SIZE];
    memcpy(temp, buffer, BUFFER_SIZE * sizeof(BUFFER_TYPE));
    memcpy(&buffer[1], temp, (BUFFER_SIZE - 1) * sizeof(BUFFER_TYPE));
    buffer[0] = newSample;
}

void scalarProduct(BUFFER_TYPE *one, BUFFER_TYPE *other, BUFFER_TYPE *resultBuffer) {
    for (int i = 0; i < BUFFER_SIZE ; i++) {
        resultBuffer[i] = one[i] * other[i];
    }
}

BUFFER_TYPE arraySum(BUFFER_TYPE *one) {
    BUFFER_TYPE result = 0;
    for (int i = 0; i < BUFFER_SIZE ; i++) {
        result += one[i];
    }
    return result;
}

void printArray(BUFFER_TYPE *array) {
    printf("  ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%f, ", array[i]);
    }
    printf("\n");
}

