//
// File: ert_main.cpp
//
// Code generated for Simulink model 'controle_simples'.
//
// Model version                  : 1.108
// Simulink Coder version         : 8.10 (R2016a) 10-Feb-2016
// C/C++ source code generated on : Wed Aug 24 12:34:12 2016
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Mac OS X)
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#include <stddef.h>
#include <stdio.h>                     // This ert_main.c example uses printf/fflush 
#include "controle_simples.h"          // Model's header file
#include "rtwtypes.h"

static controle_simplesModelClass rtObj;// Instance of model class

//
// Associating rt_OneStep with a real-time clock or interrupt service routine
// is what makes the generated code "real-time".  The function rt_OneStep is
// always associated with the base rate of the model.  Subrates are managed
// by the base rate from inside the generated code.  Enabling/disabling
// interrupts and floating point context switches are target specific.  This
// example code indicates where these should take place relative to executing
// the generated code step function.  Overrun behavior should be tailored to
// your application needs.  This example simply sets an error status in the
// real-time model and returns from rt_OneStep.
//
void rt_OneStep(void);
void test();
void rt_OneStep(void)
{
  static boolean_T OverrunFlag = false;

  // Disable interrupts here

  // Check for overrun
  if (OverrunFlag) {
    rtmSetErrorStatus(rtObj.getRTM(), "Overrun");
    return;
  }

  OverrunFlag = true;

  // Save FPU context here (if necessary)
  // Re-enable timer or interrupt here
  // Set model inputs here

  // Step the model for base rate
  rtObj.step();

  // Get model outputs here

  // Indicate task complete
  OverrunFlag = false;

  // Disable interrupts here
  // Restore FPU context here (if necessary)
  // Enable interrupts here
}

//
// The example "main" function illustrates what is required by your
// application code to initialize, execute, and terminate the generated code.
// Attaching rt_OneStep to a real-time clock is target specific.  This example
// illustrates how you do this relative to initializing the model.
//
int_T main(int_T argc, const char *argv[])
{
  // Unused arguments
  (void)(argc);
  (void)(argv);

  // Initialize model
  rtObj.initialize();

  // Simulating the model step behavior (in non real-time) to
  //   simulate model behavior at stop time.

    test();
    
  // Disable rt_OneStep() here
  return 0;
}

void test() {
    rtObj.rtU.dSensor = 10; // m
    rtObj.rtU.vAbs = 5; // m/s
    rtObj.rtU.vRel = 2; // m/s
    rtObj.rtU.vMax = 10; // m/s
    
    rt_OneStep();
    
    while ((rtmGetErrorStatus(rtObj.getRTM()) == (NULL)) && !rtmGetStopRequested
           (rtObj.getRTM())) {
        rt_OneStep();
        printf("aRef: %f", rtObj.rtY.aRef);
    }
}

//
// File trailer for generated code.
//
// [EOF]
//
