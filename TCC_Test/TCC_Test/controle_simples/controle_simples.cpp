//
// File: controle_simples.cpp
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

#include "controle_simples.h"
#define NumBitsPerChar                 8U

// Private macros used by the generated code to access rtModel
#ifndef rtmIsMajorTimeStep
# define rtmIsMajorTimeStep(rtm)       (((rtm)->Timing.simTimeStep) == MAJOR_TIME_STEP)
#endif

#ifndef rtmIsMinorTimeStep
# define rtmIsMinorTimeStep(rtm)       (((rtm)->Timing.simTimeStep) == MINOR_TIME_STEP)
#endif

#ifndef rtmGetTPtr
# define rtmGetTPtr(rtm)               ((rtm)->Timing.t)
#endif

#ifndef rtmSetTPtr
# define rtmSetTPtr(rtm, val)          ((rtm)->Timing.t = (val))
#endif

// private model entry point functions
extern void controle_simples_derivatives();
extern "C" {
  extern real_T rtInf;
  extern real_T rtMinusInf;
  extern real_T rtNaN;
  extern real32_T rtInfF;
  extern real32_T rtMinusInfF;
  extern real32_T rtNaNF;
  extern void rt_InitInfAndNaN(size_t realSize);
  extern boolean_T rtIsInf(real_T value);
  extern boolean_T rtIsInfF(real32_T value);
  extern boolean_T rtIsNaN(real_T value);
  extern boolean_T rtIsNaNF(real32_T value);
  typedef struct {
    struct {
      uint32_T wordH;
      uint32_T wordL;
    } words;
  } BigEndianIEEEDouble;

  typedef struct {
    struct {
      uint32_T wordL;
      uint32_T wordH;
    } words;
  } LittleEndianIEEEDouble;

  typedef struct {
    union {
      real32_T wordLreal;
      uint32_T wordLuint;
    } wordL;
  } IEEESingle;
}                                      // extern "C"
  extern "C"
{
  real_T rtInf;
  real_T rtMinusInf;
  real_T rtNaN;
  real32_T rtInfF;
  real32_T rtMinusInfF;
  real32_T rtNaNF;
}

extern "C" {
  extern real_T rtGetInf(void);
  extern real32_T rtGetInfF(void);
  extern real_T rtGetMinusInf(void);
  extern real32_T rtGetMinusInfF(void);
}                                      // extern "C"
  extern "C"
{
  extern real_T rtGetNaN(void);
  extern real32_T rtGetNaNF(void);
}                                      // extern "C"

extern "C" {
  //
  // Initialize the rtInf, rtMinusInf, and rtNaN needed by the
  // generated code. NaN is initialized as non-signaling. Assumes IEEE.
  //
  void rt_InitInfAndNaN(size_t realSize)
  {
    (void) (realSize);
    rtNaN = rtGetNaN();
    rtNaNF = rtGetNaNF();
    rtInf = rtGetInf();
    rtInfF = rtGetInfF();
    rtMinusInf = rtGetMinusInf();
    rtMinusInfF = rtGetMinusInfF();
  }

  // Test if value is infinite
  boolean_T rtIsInf(real_T value)
  {
    return (boolean_T)((value==rtInf || value==rtMinusInf) ? 1U : 0U);
  }

  // Test if single-precision value is infinite
  boolean_T rtIsInfF(real32_T value)
  {
    return (boolean_T)(((value)==rtInfF || (value)==rtMinusInfF) ? 1U : 0U);
  }

  // Test if value is not a number
  boolean_T rtIsNaN(real_T value)
  {
    return (boolean_T)((value!=value) ? 1U : 0U);
  }

  // Test if single-precision value is not a number
  boolean_T rtIsNaNF(real32_T value)
  {
    return (boolean_T)(((value!=value) ? 1U : 0U));
  }
}
  extern "C"
{
  //
  // Initialize rtInf needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  real_T rtGetInf(void)
  {
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T inf = 0.0;
    if (bitsPerReal == 32U) {
      inf = rtGetInfF();
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0x7FF00000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      inf = tmpVal.fltVal;
    }

    return inf;
  }

  //
  // Initialize rtInfF needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  real32_T rtGetInfF(void)
  {
    IEEESingle infF;
    infF.wordL.wordLuint = 0x7F800000U;
    return infF.wordL.wordLreal;
  }

  //
  // Initialize rtMinusInf needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  real_T rtGetMinusInf(void)
  {
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T minf = 0.0;
    if (bitsPerReal == 32U) {
      minf = rtGetMinusInfF();
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0xFFF00000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      minf = tmpVal.fltVal;
    }

    return minf;
  }

  //
  // Initialize rtMinusInfF needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  real32_T rtGetMinusInfF(void)
  {
    IEEESingle minfF;
    minfF.wordL.wordLuint = 0xFF800000U;
    return minfF.wordL.wordLreal;
  }
}

extern "C" {
  //
  // Initialize rtNaN needed by the generated code.
  // NaN is initialized as non-signaling. Assumes IEEE.
  //
  real_T rtGetNaN(void)
  {
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T nan = 0.0;
    if (bitsPerReal == 32U) {
      nan = rtGetNaNF();
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0xFFF80000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      nan = tmpVal.fltVal;
    }

    return nan;
  }

  //
  // Initialize rtNaNF needed by the generated code.
  // NaN is initialized as non-signaling. Assumes IEEE.
  //
  real32_T rtGetNaNF(void)
  {
    IEEESingle nanF = { { 0 } };

    nanF.wordL.wordLuint = 0xFFC00000U;
    return nanF.wordL.wordLreal;
  }
}
//
// This function updates continuous states using the ODE3 fixed-step
// solver algorithm
//
  void controle_simplesModelClass::rt_ertODEUpdateContinuousStates(RTWSolverInfo
  *si )
{
  // Solver Matrices
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = (ODE3_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 2;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  // Save the state values at time t in y, we'll use x as ynew.
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  // Assumes that rtsiSetT and ModelOutputs are up-to-date
  // f0 = f(t,y)
  rtsiSetdX(si, f0);
  controle_simples_derivatives();

  // f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*));
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  this->step();
  controle_simples_derivatives();

  // f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*));
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  this->step();
  controle_simples_derivatives();

  // tnew = t + hA(3);
  // ynew = y + f*hB(:,3);
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

// Model step function
void controle_simplesModelClass::step()
{
  real_T rtb_vRef;
  real_T u0;
  if (rtmIsMajorTimeStep((&rtM))) {
    // set solver stop time
    rtsiSetSolverStopTime(&(&rtM)->solverInfo,(((&rtM)->Timing.clockTick0+1)*
      (&rtM)->Timing.stepSize0));
  }                                    // end MajorTimeStep

  // Update absolute time of base rate at minor time step
  if (rtmIsMinorTimeStep((&rtM))) {
    (&rtM)->Timing.t[0] = rtsiGetT(&(&rtM)->solverInfo);
  }

  // Gain: '<Root>/Tempo de reacao' incorporates:
  //   Inport: '<Root>/vAbs'
  //   Inport: '<Root>/vRel'
  //   Sum: '<Root>/Sum1'
  rtb_vRef = (rtU.vAbs                 //@>17
              + rtU.vRel)              //@>9
    * 1.5;

  // MinMax: '<Root>/MinMax'
  if ((5.0 >= rtb_vRef) || rtIsNaN(rtb_vRef)) {
    rtb_vRef = 5.0;
  }

  // Sum: '<Root>/Sum' incorporates:
  //   Gain: '<Root>/Gain'
  //   Inport: '<Root>/dSensor'
  //   Inport: '<Root>/vRel'
  //   MinMax: '<Root>/MinMax'
  rtb_vRef = (rtb_vRef                 //@>14,4
              - rtU.dSensor)           //@>4
    - 1.5                              //@>23
    * rtU.vRel;

  // Gain: '<S1>/Filter Coefficient' incorporates:
  //   Gain: '<S1>/Derivative Gain'
  //   Integrator: '<S1>/Filter'
  //   Sum: '<S1>/SumD'
  rtDW.FilterCoefficient = (-0.931121700721534//@>6:1668
    * rtb_vRef                         //@>6:1672
    - rtX.Filter_CSTATE)               //@>6:1671
    * 1.02327765217534;

  // Sum: '<S1>/Sum' incorporates:
  //   Gain: '<S1>/Proportional Gain'
  //   Integrator: '<S1>/Integrator'
  u0 = (-3.87280472505017              //@>6:1666
        * rtb_vRef                     //@>6:1665
        + rtX.Integrator_CSTATE)       //@>6:1665
    + rtDW.FilterCoefficient;

  // Saturate: '<S1>/Saturate'
  if (u0 > 100.0) {
    // Outport: '<Root>/aRef'
    rtY.aRef = 100.0;
  } else if (u0 < -100.0) {
    // Outport: '<Root>/aRef'
    rtY.aRef = -100.0;
  } else {
    // Outport: '<Root>/aRef'
    rtY.aRef = u0;
  }

  // End of Saturate: '<S1>/Saturate'

  // Gain: '<S1>/Integral Gain'
  rtDW.IntegralGain = -1.31628778505579//@>6:1667
    * rtb_vRef;
  if (rtmIsMajorTimeStep((&rtM))) {
    rt_ertODEUpdateContinuousStates(&(&rtM)->solverInfo);

    // Update absolute time for base rate
    // The "clockTick0" counts the number of times the code of this task has
    //  been executed. The absolute time is the multiplication of "clockTick0"
    //  and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
    //  overflow during the application lifespan selected.
    ++(&rtM)->Timing.clockTick0;
    (&rtM)->Timing.t[0] = rtsiGetSolverStopTime(&(&rtM)->solverInfo);

    {
      // Update absolute timer for sample time: [0.2s, 0.0s]
      // The "clockTick1" counts the number of times the code of this task has
      //  been executed. The resolution of this integer timer is 0.2, which is the step size
      //  of the task. Size of "clockTick1" ensures timer will not overflow during the
      //  application lifespan selected.
      (&rtM)->Timing.clockTick1++;
    }
  }                                    // end MajorTimeStep
}

// Derivatives for root system: '<Root>'
void controle_simplesModelClass::controle_simples_derivatives()
{
  XDot *_rtXdot;
  _rtXdot = ((XDot *) (&rtM)->ModelData.derivs);

  // Derivatives for Integrator: '<S1>/Integrator'
  _rtXdot->Integrator_CSTATE = rtDW.IntegralGain;

  // Derivatives for Integrator: '<S1>/Filter'
  _rtXdot->Filter_CSTATE = rtDW.FilterCoefficient;
}

// Model initialize function
void controle_simplesModelClass::initialize()
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));

  {
    // Setup solver object
    rtsiSetSimTimeStepPtr(&(&rtM)->solverInfo, &(&rtM)->Timing.simTimeStep);
    rtsiSetTPtr(&(&rtM)->solverInfo, &rtmGetTPtr((&rtM)));
    rtsiSetStepSizePtr(&(&rtM)->solverInfo, &(&rtM)->Timing.stepSize0);
    rtsiSetdXPtr(&(&rtM)->solverInfo, &(&rtM)->ModelData.derivs);
    rtsiSetContStatesPtr(&(&rtM)->solverInfo, (real_T **) &(&rtM)
                         ->ModelData.contStates);
    rtsiSetNumContStatesPtr(&(&rtM)->solverInfo, &(&rtM)->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&(&rtM)->solverInfo, &(&rtM)
      ->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&(&rtM)->solverInfo, &(&rtM)
      ->ModelData.periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&(&rtM)->solverInfo, &(&rtM)
      ->ModelData.periodicContStateRanges);
    rtsiSetErrorStatusPtr(&(&rtM)->solverInfo, (&rtmGetErrorStatus((&rtM))));
    rtsiSetRTModelPtr(&(&rtM)->solverInfo, (&rtM));
  }

  rtsiSetSimTimeStep(&(&rtM)->solverInfo, MAJOR_TIME_STEP);
  (&rtM)->ModelData.intgData.y = (&rtM)->ModelData.odeY;
  (&rtM)->ModelData.intgData.f[0] = (&rtM)->ModelData.odeF[0];
  (&rtM)->ModelData.intgData.f[1] = (&rtM)->ModelData.odeF[1];
  (&rtM)->ModelData.intgData.f[2] = (&rtM)->ModelData.odeF[2];
  (&rtM)->ModelData.contStates = ((X *) &rtX);
  rtsiSetSolverData(&(&rtM)->solverInfo, (void *)&(&rtM)->ModelData.intgData);
  rtsiSetSolverName(&(&rtM)->solverInfo,"ode3");
  rtmSetTPtr((&rtM), &(&rtM)->Timing.tArray[0]);
  (&rtM)->Timing.stepSize0 = 0.2;

  // InitializeConditions for Integrator: '<S1>/Integrator'
  rtX.Integrator_CSTATE = 0.0;

  // InitializeConditions for Integrator: '<S1>/Filter'
  rtX.Filter_CSTATE = 0.0;
}

// Constructor
controle_simplesModelClass::controle_simplesModelClass()
{
}

// Destructor
controle_simplesModelClass::~controle_simplesModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL * controle_simplesModelClass::getRTM()
{
  return (&rtM);
}

//
// File trailer for generated code.
//
// [EOF]
//
