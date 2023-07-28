/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!****************************************************************************
 *  @file       foc.h
 *  @brief      Sensorless FOC Motor Control Library FOC Module
 *
 *
 *  @anchor foc_h
 *  # Overview
 *
 *  APIs for FOC motor control
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_MODULE FOC_LIBRARY/FOC
 * @{
 */
#ifndef FOC_H
#define FOC_H

#include <stdint.h>
#include <stdbool.h>

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

#include "hal.h"

#include "rampgen.h"
#include "rmp_cntl.h"
#include "clarke.h"
#include "park.h"
#include "ipark.h"
#include "pi.h"
#include "cirlimit.h"
#include "volt_calc.h"
#include "esmo/esmopos.h"
#include "speed_est.h"
#include "svgen.h"
#include "pwmgen.h"
#include "motor_params.h"
#include "parameter.h"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
Following is the list of the Build Level choices.
------------------------------------------------------------------------------*/
/*! @brief Module check out (do not connect the motors) */
#define LEVEL1  1
/*! @brief Verify ADC, park/clarke, calibrate the offset */
#define LEVEL2  2
/*! @brief Verify closed current(torque) loop and PIDs and speed measurement */
#define LEVEL3  3
/*! @brief Verify speed estimation and rotor position est.*/                               
#define LEVEL4  4
/*! @brief Verify closed speed loop, speed PID and eSMO */
#define LEVEL5  5

/* Check if BUILDLEVEL is not defined */
#ifndef BUILDLEVEL
/*! @brief BUILDLEVEL is defined here if the user did not define it. */
#define   BUILDLEVEL LEVEL5
#endif

/*! @enum FOC_LOG_IDX */
typedef enum{
    /*! Index associated to LOG index 0 */
    FOC_LOG_IDX_0 = 0,
    /*! Index associated to LOG index 1 */
    FOC_LOG_IDX_1,
    /*! Index associated to LOG index 2 */
    FOC_LOG_IDX_2,
    /*! Index associated to LOG index 3 */
    FOC_LOG_IDX_3,
    /*! Index associated to LOG index 4 */
    FOC_LOG_IDX_4,
    /*! Index associated to LOG index 5 */
    FOC_LOG_IDX_5,
    /*! Total number of LOG index */
    FOC_LOG_IDX_MAX
}FOC_LOG_IDX;

/*!
 * @brief Size of the FOC variable logging buffer
 */
#define FOC_LOG_BUF_SIZE        (1024)

/*! @brief Define FOC structure */
typedef struct {
    /*!  Stores the FOC loop run counter */
    uint32_t isrTicker;
    /*!  Stores the Raw ADC readings of DC bus */
    uint32_t vdcAdc;
    /*!  Stores the Raw ADC readings of A phase */
    uint32_t iaAdc;
    /*!  Stores the Raw ADC readings of B phase */
    uint32_t ibAdc;
    /*!  Stores the A phase offset */
    uint32_t offsetA;
    /*!  Stores the B phase offset */
    uint32_t offsetB;
    /*!  Stores the DC bus voltage in pu */
    _iq vdc;
    /*!  Stores the A phase current in pu */
    _iq ia;
    /*!  Stores the B phase current in pu */
    _iq ib;
    /*!  Stores the Speed loop divider */
    uint16_t PISpdExecDivider;
    /*!  Stores the Speed loop execution count */
    uint16_t PISpdExecCount;
    /*!  Stores the foc parameters */
    float parameter[PARA_IDX_MAX];
    /*!  PWM channel set for A phase. @ref HAL_PWM_CHANNEL */
    HAL_PWM_CHANNEL pwmAHal;
    /*!  PWM channel set for B phase. @ref HAL_PWM_CHANNEL */
    HAL_PWM_CHANNEL pwmBHal;
    /*!  PWM channel set for C phase. @ref HAL_PWM_CHANNEL */
    HAL_PWM_CHANNEL pwmCHal;
} FOC_Instance;

/* Extern the foc modules */
extern PWMGEN_Instance pwmgen;
extern PI_Instance piSpd;
extern PI_Instance piId;
extern PI_Instance piIq;
extern CLARKE_Instance clarke;
extern PARK_Instance park;
extern CIRLMT_Instance cirlimit;
extern IPARK_Instance ipark;
extern SVGEN_Instance svgen;
extern RMPCNTL_Instance rc;
extern RAMPGEN_Instance rg;
extern PHASEVOLT_Instance phaseVolt;
extern SPDEST_Instance speedEst;
extern ESMO_Instance  esmo;

/* Extern the foc variables */
extern uint16_t lsw;
extern _iq idRef; 
extern _iq iqRef;
extern _iq speedRef;
extern _iq VqTesting;
extern _iq VdTesting;
extern bool enableFOC;
extern bool enablePWM;
extern _iq startCnt;

#if __ENABLE_LOG
/* Extern the log variables */
extern int32_t ccsLog[FOC_LOG_IDX_MAX][FOC_LOG_BUF_SIZE];
extern uint16_t ccsLogPtr[FOC_LOG_IDX_MAX];
#endif

/**
* @brief     Disables the PWM outputs
* @param[in] handle  A pointer to FOC instance
*/
void FOC_init(FOC_Instance *handle);

/**
* @brief     Sets the FOC parameter and updates the modules
* @param[in] handle   A pointer to FOC instance
* @param[in] paraIdx  Index of the parameter to set. One of @ref PARA_IDX
* @param[in] value    Value to be set
*/
void FOC_setPara(FOC_Instance *handle, PARA_IDX paraIdx, float value);

/**
* @brief     Disables FOC and PWM output
* @param[in] handle   A pointer to FOC instance
*/
void FOC_setFault(FOC_Instance *handle);

/**
* @brief     Enables PWM outputs
* @param[in] handle   A pointer to FOC instance
*/
void FOC_clearFault(FOC_Instance *handle);

#if __ENABLE_LOG
/**
* @brief     Adds variable to the log buffer
* @param[in] logVar   Index location to log the value. One of @ref FOC_LOG_IDX
* @param[in] value    Value to be log
*/
__STATIC_INLINE void FOC_addVarToLog(FOC_LOG_IDX logVar, int32_t value)
{
    ccsLog[logVar][ccsLogPtr[logVar]] = value;
    ccsLogPtr[logVar]+=1;
    if(ccsLogPtr[logVar] >= FOC_LOG_BUF_SIZE)
    {
        ccsLogPtr[logVar] = 0;
    }
}
#endif

/**
* @brief     Does the FOC motor control loop
* @param[in] handle  A pointer to FOC instance
*/
__STATIC_INLINE void FOC_run(FOC_Instance *handle)
{
    if(enableFOC == true)
    {
    if(!enablePWM)
    {
        enablePWM = true;
        HAL_PWMEnableChannel(handle->pwmAHal);
        HAL_PWMEnableChannel(handle->pwmBHal);
        HAL_PWMEnableChannel(handle->pwmCHal);
    }
/*================================ LEVEL 1 =====================================
    Level 1 checks the modules, should not connect the motor
==============================================================================*/
#if (BUILDLEVEL==LEVEL1)
        /*---------------------------------------------------------------------
            Connect inputs of the RMP module and run
        ----------------------------------------------------------------------*/
        if(lsw==0)rc.TargetValue = 0;
        else rc.TargetValue = speedRef;
        RC_run(&rc);
        /*----------------------------------------------------------------------
           Connect inputs of the RAMP GEN module and run
        ----------------------------------------------------------------------*/
        rg.Freq = rc.SetpointValue;
        RG_run(&rg);
        /*----------------------------------------------------------------------
            Connect inputs of the IPARK module and call run
        ----------------------------------------------------------------------*/
        ipark.Ds = VdTesting;
        ipark.Qs = VqTesting;
        ipark.Sine = _IQsinPU(rg.Out);
        ipark.Cosine = _IQcosPU(rg.Out);
        IPARK_run(&ipark);
        /*----------------------------------------------------------------------
            Connect inputs of the SVGEN module and call run
        ----------------------------------------------------------------------*/
        svgen.Ualpha = ipark.Alpha;
        svgen.Ubeta = ipark.Beta;
        SVGEN_run(&svgen);
        /*----------------------------------------------------------------------
            Connect inputs of the PWMGEN module and call run
        ----------------------------------------------------------------------*/
        pwmgen.MfuncC1 = svgen.Ta;
        pwmgen.MfuncC2 = svgen.Tb;
        pwmgen.MfuncC3 = svgen.Tc;
        PWMGEN_run(&pwmgen);

#if __ENABLE_LOG
        FOC_addVarToLog(FOC_LOG_IDX_0, svgen.Ta);
        FOC_addVarToLog(FOC_LOG_IDX_1, svgen.Tb);
        FOC_addVarToLog(FOC_LOG_IDX_2, svgen.Tc);
        FOC_addVarToLog(FOC_LOG_IDX_3, svgen.Tb - svgen.Tc);
        FOC_addVarToLog(FOC_LOG_IDX_4, rg.Out);
#endif
#endif /* (BUILDLEVEL==LEVEL1) */

/*================================ LEVEL 2 =====================================
    Level 2 verifies the analog-to-digital conversion, offset compensation,
    clarke/park transformations (CLARKE/PARK), phase voltage calculations
==============================================================================*/
#if (BUILDLEVEL==LEVEL2)
        /*---------------------------------------------------------------------
            Connect inputs of the RMP module and run
        ----------------------------------------------------------------------*/
        if(lsw==0)rc.TargetValue = 0;
        else rc.TargetValue = speedRef;
        RC_run(&rc);
        /*----------------------------------------------------------------------
           Connect inputs of the RAMP GEN module and run
        ----------------------------------------------------------------------*/
        rg.Freq = rc.SetpointValue;
        RG_run(&rg);
        /*----------------------------------------------------------------------
            Connect inputs of the CLARKE module and call run
        ----------------------------------------------------------------------*/
        clarke.As = handle->ia;
        clarke.Bs = handle->ib;
        CLARKE_run(&clarke);
        /*----------------------------------------------------------------------
            Connect inputs of the PARK module and call run
        ----------------------------------------------------------------------*/
        park.Alpha = clarke.Alpha;
        park.Beta = clarke.Beta;

        if(lsw==0) park.Angle = 0;
        else park.Angle = rg.Out;

        park.Sine = _IQsinPU(park.Angle);
        park.Cosine = _IQcosPU(park.Angle);
        PARK_run(&park);
        /*----------------------------------------------------------------------
            Connect inputs of the IPARK module and call run
        ----------------------------------------------------------------------*/
        ipark.Ds  = VdTesting;
        ipark.Qs  = VqTesting;
        ipark.Sine = park.Sine;
        ipark.Cosine = park.Cosine;
        IPARK_run(&ipark);
        /*----------------------------------------------------------------------
              Connect inputs of the PHASEVOLT module and call run
        ----------------------------------------------------------------------*/
        phaseVolt.DcBusVolt = handle->vdc;
        phaseVolt.MfuncV1 = svgen.Ta;
        phaseVolt.MfuncV2 = svgen.Tb;
        phaseVolt.MfuncV3 = svgen.Tc;
        PHASEVOLT_run(&phaseVolt);
        /*----------------------------------------------------------------------
            Connect inputs of the SVGEN module and call run
        ----------------------------------------------------------------------*/
        svgen.Ualpha = ipark.Alpha;
        svgen.Ubeta = ipark.Beta;
        SVGEN_run(&svgen);

#if __ENABLE_LOG
        FOC_addVarToLog(FOC_LOG_IDX_0, phaseVolt.Valpha);
        FOC_addVarToLog(FOC_LOG_IDX_1, phaseVolt.Vbeta);
        FOC_addVarToLog(FOC_LOG_IDX_2, rg.Out);
        FOC_addVarToLog(FOC_LOG_IDX_3, svgen.Ta);
        FOC_addVarToLog(FOC_LOG_IDX_4, clarke.Alpha);
        FOC_addVarToLog(FOC_LOG_IDX_5, clarke.Beta);
#endif
        /*----------------------------------------------------------------------
            Connect inputs of the PWMGEN module and call run
        ----------------------------------------------------------------------*/
        pwmgen.MfuncC1 = svgen.Ta;
        pwmgen.MfuncC2 = svgen.Tb;
        pwmgen.MfuncC3 = svgen.Tc;
        PWMGEN_run(&pwmgen);
#endif /* (BUILDLEVEL==LEVEL2) */

/*================================ LEVEL 3 =====================================
    Level 3 verifies the dq-axis current regulation performed by PID and speed
    measurement modules
==============================================================================*/
/* lsw=0: lock the rotor of the motor
   lsw=1: close the current loop */
#if (BUILDLEVEL==LEVEL3)
        /*---------------------------------------------------------------------
            Connect inputs of the RMP module and run
        ----------------------------------------------------------------------*/
        if(lsw==0)rc.TargetValue = 0;
        else rc.TargetValue = speedRef;
        RC_run(&rc);
        /*----------------------------------------------------------------------
           Connect inputs of the RAMP GEN module and run
        ----------------------------------------------------------------------*/
        rg.Freq = rc.SetpointValue;
        RG_run(&rg);
        /*----------------------------------------------------------------------
            Connect inputs of the CLARKE module and call run
        ----------------------------------------------------------------------*/
        clarke.As = handle->ia;
        clarke.Bs = handle->ib;
        CLARKE_run(&clarke);
        /*----------------------------------------------------------------------
            Connect inputs of the PARK module and call run
        ----------------------------------------------------------------------*/
        park.Alpha = clarke.Alpha;
        park.Beta = clarke.Beta;

        if(lsw==0) park.Angle = 0;
        else park.Angle = rg.Out;

        park.Sine = _IQsinPU(park.Angle);
        park.Cosine = _IQcosPU(park.Angle);
        PARK_run(&park);
        /*----------------------------------------------------------------------
              Connect inputs of the PI IQ module and call run
        ----------------------------------------------------------------------*/
        if(lsw==0) piIq.Ref = 0;
        else piIq.Ref = iqRef;
        piIq.Fbk = park.Qs;
        PI_run(&piIq);
        /*----------------------------------------------------------------------
              Connect inputs of the PI ID module and call run
        ----------------------------------------------------------------------*/
        if(lsw==0) piId.Ref = idRef;
        else piId.Ref = 0;
        piId.Fbk = park.Ds;
        PI_run(&piId);
        /*----------------------------------------------------------------------
              Connect inputs of the CIRLIMIT module and call run
        ----------------------------------------------------------------------*/
        cirlimit.inVd = piId.Out;
        cirlimit.inVq = piIq.Out;
        CIRLIMIT_run(&cirlimit);
        /*----------------------------------------------------------------------
            Connect inputs of the IPARK module and call run
        ----------------------------------------------------------------------*/
        ipark.Ds = cirlimit.outVd;
        ipark.Qs = cirlimit.outVq;
        ipark.Sine = park.Sine;
        ipark.Cosine = park.Cosine;
        IPARK_run(&ipark);
        /*----------------------------------------------------------------------
              Connect inputs of the PHASEVOLT module and call run
        ----------------------------------------------------------------------*/
        phaseVolt.DcBusVolt = handle->vdc;
        phaseVolt.MfuncV1 = svgen.Ta;
        phaseVolt.MfuncV2 = svgen.Tb;
        phaseVolt.MfuncV3 = svgen.Tc;
        PHASEVOLT_run(&phaseVolt);
        /*----------------------------------------------------------------------
            Connect inputs of the SVGEN module and call run
        ----------------------------------------------------------------------*/
        svgen.Ualpha = ipark.Alpha;
        svgen.Ubeta = ipark.Beta;
        SVGEN_run(&svgen);

#if __ENABLE_LOG
        FOC_addVarToLog(FOC_LOG_IDX_0, svgen.Ta);
        FOC_addVarToLog(FOC_LOG_IDX_1, rg.Out);
#endif
        /*----------------------------------------------------------------------
            Connect inputs of the PWMGEN module and call run
        ----------------------------------------------------------------------*/
        pwmgen.MfuncC1 = svgen.Ta;
        pwmgen.MfuncC2 = svgen.Tb;
        pwmgen.MfuncC3 = svgen.Tc;
        PWMGEN_run(&pwmgen);
#endif /* (BUILDLEVEL==LEVEL3) */

/*================================ LEVEL 4 =====================================
    Level 4 verifies the estimated rotor position and speed estimation
    performed by eSMO and speedEst modules, respectively.
==============================================================================*/
/* lsw=0: lock the rotor of the motor
   lsw=1: close the current loop */
#if (BUILDLEVEL==LEVEL4)
        /*---------------------------------------------------------------------
            Connect inputs of the RMP module and run
        ----------------------------------------------------------------------*/
        if(lsw==0)rc.TargetValue = 0;
        else rc.TargetValue = speedRef;
        RC_run(&rc);
        /*----------------------------------------------------------------------
           Connect inputs of the RAMP GEN module and run
        ----------------------------------------------------------------------*/
        rg.Freq = rc.SetpointValue;
        RG_run(&rg);
        /*----------------------------------------------------------------------
            Connect inputs of the CLARKE module and call run
        ----------------------------------------------------------------------*/
        clarke.As = handle->ia;
        clarke.Bs = handle->ib;
        CLARKE_run(&clarke);
        /*----------------------------------------------------------------------
            Connect inputs of the PARK module and call run
        ----------------------------------------------------------------------*/
        park.Alpha = clarke.Alpha;
        park.Beta = clarke.Beta;

        if(lsw==0) park.Angle = 0;
        else park.Angle = rg.Out;

        park.Sine = _IQsinPU(park.Angle);
        park.Cosine = _IQcosPU(park.Angle);
        PARK_run(&park);
        /*----------------------------------------------------------------------
              Connect inputs of the PI IQ module and call run
        ----------------------------------------------------------------------*/
        if(lsw==0) piIq.Ref = 0;
        else piIq.Ref = iqRef;
        piIq.Fbk = park.Qs;
        PI_run(&piIq);
        /*----------------------------------------------------------------------
              Connect inputs of the PI ID module and call run
        ----------------------------------------------------------------------*/
        if(lsw==0) piId.Ref = idRef;
        else piId.Ref = 0;
        piId.Fbk = park.Ds;
        PI_run(&piId);
        /*----------------------------------------------------------------------
              Connect inputs of the CIRLIMIT module and call run
        ----------------------------------------------------------------------*/
        cirlimit.inVd = piId.Out;
        cirlimit.inVq = piIq.Out;
        CIRLIMIT_run(&cirlimit);
        /*----------------------------------------------------------------------
            Connect inputs of the IPARK module and call run
        ----------------------------------------------------------------------*/
        ipark.Ds = cirlimit.outVd;
        ipark.Qs = cirlimit.outVq;
        ipark.Sine = park.Sine;
        ipark.Cosine = park.Cosine;
        IPARK_run(&ipark);
        /*----------------------------------------------------------------------
              Connect inputs of the PHASEVOLT module and call run
        ----------------------------------------------------------------------*/
        phaseVolt.DcBusVolt = handle->vdc;
        phaseVolt.MfuncV1 = svgen.Ta;
        phaseVolt.MfuncV2 = svgen.Tb;
        phaseVolt.MfuncV3 = svgen.Tc;
        PHASEVOLT_run(&phaseVolt);
        /*----------------------------------------------------------------------
              Connect inputs of the eSMO module and call run
        ----------------------------------------------------------------------*/
        /* 'startCnt' helps to smoothly change smoFreq during lsw 1->2 */
        if (lsw != 2)                   startCnt  = _IQ(0.001);
        else if (startCnt < _IQ(1.0))   startCnt += _IQ(0.0001);

        esmo.Ialpha   = clarke.Alpha;
        esmo.Ibeta    = clarke.Beta;
        esmo.Valpha   = phaseVolt.Valpha;
        esmo.Vbeta    = phaseVolt.Vbeta;
        esmo.runSpeed = _IQmpy(speedEst.EstimatedSpeed, startCnt);
        esmo.cmdSpeed = rc.SetpointValue;
        ESMO_run(&esmo);
        /*----------------------------------------------------------------------
            Connect inputs of the SPDEST module and call run
        ----------------------------------------------------------------------*/
        speedEst.EstimatedTheta = esmo.Theta;
        SPDEST_run(&speedEst);
        /*----------------------------------------------------------------------
            Connect inputs of the SVGEN module and call run
        ----------------------------------------------------------------------*/
        svgen.Ualpha = ipark.Alpha;
        svgen.Ubeta = ipark.Beta;
        SVGEN_run(&svgen);
        /*----------------------------------------------------------------------
            Connect inputs of the PWMGEN module and call run
        ----------------------------------------------------------------------*/
        pwmgen.MfuncC1 = svgen.Ta;
        pwmgen.MfuncC2 = svgen.Tb;
        pwmgen.MfuncC3 = svgen.Tc;
        PWMGEN_run(&pwmgen);
        
#if __ENABLE_LOG
        FOC_addVarToLog(FOC_LOG_IDX_0, esmo.Ialpha);
        FOC_addVarToLog(FOC_LOG_IDX_1, esmo.Ibeta);
        FOC_addVarToLog(FOC_LOG_IDX_2, esmo.Valpha);
        FOC_addVarToLog(FOC_LOG_IDX_3, esmo.Vbeta);
        FOC_addVarToLog(FOC_LOG_IDX_4, esmo.Theta);
        FOC_addVarToLog(FOC_LOG_IDX_5, rg.Out);
#endif
#endif /* (BUILDLEVEL==LEVEL4) */

/*================================ LEVEL 5 =====================================
    Level 5 verifies the speed regulator performed by PI module.
    The system speed loop is closed by using the estimated speed as a feedback
==============================================================================*/
/* lsw=0: lock the rotor of the motor
   lsw=1: close the current loop
   lsw=2: close the speed loop */
#if (BUILDLEVEL==LEVEL5)
        /*---------------------------------------------------------------------
            Connect inputs of the RMP module and run
        ----------------------------------------------------------------------*/
        if(lsw==0)rc.TargetValue = 0;
        else rc.TargetValue = speedRef;
        RC_run(&rc);
        /*----------------------------------------------------------------------
           Connect inputs of the RAMP GEN module and run
        ----------------------------------------------------------------------*/
        rg.Freq = rc.SetpointValue;
        RG_run(&rg);
        /*----------------------------------------------------------------------
            Connect inputs of the CLARKE module and call run
        ----------------------------------------------------------------------*/
        clarke.As = handle->ia;
        clarke.Bs = handle->ib;
        CLARKE_run(&clarke);
        /*----------------------------------------------------------------------
            Connect inputs of the PARK module and call run
        ----------------------------------------------------------------------*/
        park.Alpha = clarke.Alpha;
        park.Beta = clarke.Beta;

        if(lsw==0) park.Angle = 0;
        else if(lsw==1) park.Angle = rg.Out;
        else park.Angle =  esmo.Theta;

        park.Sine = _IQsinPU(park.Angle);
        park.Cosine = _IQcosPU(park.Angle);
        PARK_run(&park);
        /*----------------------------------------------------------------------
              Connect inputs of the PI SPEED module and call run
        ----------------------------------------------------------------------*/
        if (handle->PISpdExecCount >= handle->PISpdExecDivider)
        {
            piSpd.Ref = rc.SetpointValue;
            piSpd.Fbk = speedEst.EstimatedSpeed;
            PI_run(&piSpd);
            handle->PISpdExecCount = 1;
        }
        else
        {
            handle->PISpdExecCount++;
        }

        if(lsw==0 || lsw==1)    {piSpd.ui=0; piSpd.i1=0;}
        /*----------------------------------------------------------------------
              Connect inputs of the PI IQ module and call run
        ----------------------------------------------------------------------*/
        if(lsw==0) piIq.Ref = 0;
        else if(lsw==1) piIq.Ref = iqRef;
        else
        {
            piIq.Ref =  piSpd.Out;
        }
        piIq.Fbk = park.Qs;
        PI_run(&piIq);
        /*----------------------------------------------------------------------
              Connect inputs of the PI ID module and call run
        ----------------------------------------------------------------------*/
        if(lsw==0) piId.Ref = idRef;
        else piId.Ref = 0;
        piId.Fbk = park.Ds;
        PI_run(&piId);
        /*----------------------------------------------------------------------
              Connect inputs of the CIRLIMIT module and call run
        ----------------------------------------------------------------------*/
        cirlimit.inVd = piId.Out;
        cirlimit.inVq = piIq.Out;
        CIRLIMIT_run(&cirlimit);
        /*----------------------------------------------------------------------
            Connect inputs of the IPARK module and call run
        ----------------------------------------------------------------------*/
        ipark.Ds = cirlimit.outVd;
        ipark.Qs = cirlimit.outVq;
        ipark.Sine = park.Sine;
        ipark.Cosine = park.Cosine;
        IPARK_run(&ipark);
        /*----------------------------------------------------------------------
              Connect inputs of the PHASEVOLT module and call run
        ----------------------------------------------------------------------*/
        phaseVolt.DcBusVolt = handle->vdc;
        phaseVolt.MfuncV1 = svgen.Ta;
        phaseVolt.MfuncV2 = svgen.Tb;
        phaseVolt.MfuncV3 = svgen.Tc;
        PHASEVOLT_run(&phaseVolt);
        /*----------------------------------------------------------------------
              Connect inputs of the eSMO module and call run
        ----------------------------------------------------------------------*/
        /* 'startCnt' helps to smoothly change smoFreq during lsw 1->2 */
        if (lsw != 2)                   startCnt  = _IQ(0.001);
        else if (startCnt < _IQ(1.0))   startCnt += _IQ(0.0001);

        esmo.Ialpha   = clarke.Alpha;
        esmo.Ibeta    = clarke.Beta;
        esmo.Valpha   = phaseVolt.Valpha;
        esmo.Vbeta    = phaseVolt.Vbeta;
        esmo.runSpeed = _IQmpy(speedEst.EstimatedSpeed, startCnt);
        esmo.cmdSpeed = rc.SetpointValue;
        ESMO_run(&esmo);
        /*----------------------------------------------------------------------
            Connect inputs of the SPDEST module and call run
        ----------------------------------------------------------------------*/
        speedEst.EstimatedTheta = esmo.Theta;
        SPDEST_run(&speedEst);
        /*----------------------------------------------------------------------
            Connect inputs of the SVGEN module and call run
        ----------------------------------------------------------------------*/
        svgen.Ualpha = ipark.Alpha;
        svgen.Ubeta = ipark.Beta;
        SVGEN_run(&svgen);
        /*----------------------------------------------------------------------
            Connect inputs of the PWMGEN module and call run
        ----------------------------------------------------------------------*/
        pwmgen.MfuncC1 = svgen.Ta;
        pwmgen.MfuncC2 = svgen.Tb;
        pwmgen.MfuncC3 = svgen.Tc;
        PWMGEN_run(&pwmgen);
#endif /* (BUILDLEVEL==LEVEL5) */

        HAL_setPWMCcValue(handle->pwmAHal, pwmgen.dutyA);
        HAL_setPWMCcValue(handle->pwmBHal, pwmgen.dutyB);
        HAL_setPWMCcValue(handle->pwmCHal, pwmgen.dutyC);
    }
    else
    {
        /* Keep integral terms to zero for next startup */
        piId.i1 = 0;
        piIq.i1 = 0;
        piSpd.i1 = 0;

        rc.SetpointValue = 0;
        /* Reset angle to zero */
        rg.Angle = 0;

        /* Disabling the PWM channels */
        if(enablePWM)
        {
            enablePWM = false;
            HAL_PWMDisableChannel(handle->pwmAHal);
            HAL_PWMDisableChannel(handle->pwmBHal);
            HAL_PWMDisableChannel(handle->pwmCHal);
            HAL_setPWMCcValue(handle->pwmAHal, pwmgen.HalfPerMax);
            HAL_setPWMCcValue(handle->pwmBHal, pwmgen.HalfPerMax);
            HAL_setPWMCcValue(handle->pwmCHal, pwmgen.HalfPerMax);
        }
    }
}

/**
* @brief     Updates the adc values in the FOC instance
* @param[in] handle  A pointer to FOC instance
* @param[in] vdcAdc  ADC reading of dc bus
* @param[in] iaAdc   ADC reading from A phase
* @param[in] ibAdc   ADC reading from B phase
*/
__STATIC_INLINE void FOC_UpdateADCRaw(FOC_Instance *handle, uint16_t vdcAdc,
                                                 uint16_t iaAdc, uint16_t ibAdc)
{
    handle->vdcAdc = vdcAdc;
    handle->iaAdc = iaAdc;
    handle->ibAdc = ibAdc;
}

/**
* @brief     Runs the FOC tasks
* @param[in] handle  A pointer to FOC instance
* @param[in] vdcAdc  ADC reading of dc bus
* @param[in] iaAdc   ADC reading from A phase
* @param[in] ibAdc   ADC reading from B phase
*/
__STATIC_INLINE void FOC_loop(FOC_Instance *handle, uint16_t vdcAdc,
                                                 uint16_t iaAdc, uint16_t ibAdc)
{
    FOC_UpdateADCRaw(handle, vdcAdc, iaAdc, ibAdc);
    handle->isrTicker++;
    if (handle->isrTicker < 5000)
    {

    }
    else if (handle->isrTicker < 20000)
    {
        handle->offsetA = handle->iaAdc;
        handle->offsetB = handle->ibAdc;
    }
    else
    {
        handle->vdc = _IQ12toIQ(handle->vdcAdc);
        handle->ia  = _IQ11toIQ((int32_t)handle->iaAdc- handle->offsetA);
        handle->ib  = _IQ11toIQ((int32_t)handle->ibAdc- handle->offsetB);
        FOC_run(handle);
    }
}

#ifdef __cplusplus
}
#endif
#endif /* FOC_H */
/** @}*/
