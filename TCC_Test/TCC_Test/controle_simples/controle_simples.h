//
// File: controle_simples.h
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
#ifndef RTW_HEADER_controle_simples_h_
#define RTW_HEADER_controle_simples_h_
#include <stddef.h>
#include <string.h>
#ifndef controle_simples_COMMON_INCLUDES_
# define controle_simples_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 // controle_simples_COMMON_INCLUDES_

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  (rtmGetTPtr((rtm))[0])
#endif

// Forward declaration for rtModel
typedef struct tag_RTM RT_MODEL;

// Block signals and states (auto storage) for system '<Root>'
typedef struct {
  real_T FilterCoefficient;            // '<S1>/Filter Coefficient'
  real_T IntegralGain;                 // '<S1>/Integral Gain'
} DW;

// Continuous states (auto storage)
typedef struct {
  real_T Integrator_CSTATE;            // '<S1>/Integrator'
  real_T Filter_CSTATE;                // '<S1>/Filter'
} X;

// State derivatives (auto storage)
typedef struct {
  real_T Integrator_CSTATE;            // '<S1>/Integrator'
  real_T Filter_CSTATE;                // '<S1>/Filter'
} XDot;

// State disabled
typedef struct {
  boolean_T Integrator_CSTATE;         // '<S1>/Integrator'
  boolean_T Filter_CSTATE;             // '<S1>/Filter'
} XDis;

#ifndef ODE3_INTG
#define ODE3_INTG

// ODE3 Integration Data
typedef struct {
  real_T *y;                           // output
  real_T *f[3];                        // derivatives
} ODE3_IntgData;

#endif

// External inputs (root inport signals with auto storage)
typedef struct {
  real_T dSensor;                      // '<Root>/dSensor'
  real_T vRel;                         // '<Root>/vRel'
  real_T vAbs;                         // '<Root>/vAbs'
  real_T vMax;                         // '<Root>/vMax'
} ExtU;

// External outputs (root outports fed by signals with auto storage)
typedef struct {
  real_T aRef;                         // '<Root>/aRef'
} ExtY;

// Real-time Model Data Structure
struct tag_RTM {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;

  //
  //  ModelData:
  //  The following substructure contains information regarding
  //  the data used in the model.

  struct {
    X *contStates;
    int_T *periodicContStateIndices;
    real_T *periodicContStateRanges;
    real_T *derivs;
    boolean_T *contStateDisabled;
    boolean_T zCCacheNeedsReset;
    boolean_T derivCacheNeedsReset;
    boolean_T blkStateChange;
    real_T odeY[2];
    real_T odeF[3][2];
    ODE3_IntgData intgData;
  } ModelData;

  //
  //  Sizes:
  //  The following substructure contains sizes information
  //  for many of the model attributes such as inputs, outputs,
  //  dwork, sample times, etc.

  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  //
  //  Timing:
  //  The following substructure contains information regarding
  //  the timing information for the model.

  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

#ifdef __cplusplus

extern "C" {

#endif

#ifdef __cplusplus

}
#endif

// Class declaration for model controle_simples
class controle_simplesModelClass {
  // public data and function members
 public:
  // External inputs
  ExtU rtU;

  // External outputs
  ExtY rtY;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // Constructor
  controle_simplesModelClass();

  // Destructor
  ~controle_simplesModelClass();

  // Real-Time Model get method
  RT_MODEL * getRTM();

  // private data and function members
 private:
  // Block signals and states
  DW rtDW;
  X rtX;                               // Block continuous states

  // Real-Time Model
  RT_MODEL rtM;

  // Continuous states update member function
  void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si );

  // Derivatives member function
  void controle_simples_derivatives();
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<Root>/To Workspace' : Unused code path elimination


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'controle_simples'
//  '<S1>'   : 'controle_simples/PID Controller'

#endif                                 // RTW_HEADER_controle_simples_h_

//
// File trailer for generated code.
//
// [EOF]
//
