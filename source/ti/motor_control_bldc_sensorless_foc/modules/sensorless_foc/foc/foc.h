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

#include "iqmath_rts.h"

#include "hal.h"
#include "user.h"
#include "foc_types.h"

#include "ipd.h"
#include "rampgen.h"
#include "rmp_cntl.h"
#include "clarke.h"
#include "park.h"
#include "ipark.h"
#include "pi.h"
#include "volt_calc.h"
#include "estimator.h"
#include "svgen.h"
#include "pwmgen.h"
#include "motor_params.h"
#include "parameter.h"
#include "transit.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Base address of the saved parameters in FLASH */
#define FLASH_PARAMS_BASE_ADDRESS HAL_MEMORY_FLASH_PARAMS_BASE_ADDRESS

/*! @brief Base address of the saved parameters in SRAM */
#define SRAM_PARAMS_BASE_ADDRESS HAL_MEMORY_SRAM_PARAMS_BASE_ADDRESS

/*! @brief Memory Signature for write configurations flag */
#define FOC_WRITE_FLAG_SIGNATURE        (0x789ABCDE)

/*! @brief Seed value for the CRC calculation */
#define FOC_CRC_SEED                    (0xFFFFFFFF)

/*! @brief Size of the write flag in bytes */
#define FOC_WRITE_FLAG_SIZE_BYTES       (4)

/*! @brief Size of the CRC in bytes */
#define FOC_CRC_SIZE_BYTES              (4)

/*! @brief Access key for flash */
#define FOC_FLASH_ACCESSKEY             (0xA)

/*! @brief Right shift to convert bytes to words */
#define BYTES_TO_WORD_RSHIFT            (2)

/*! @brief Frequency of the foc control loop */
#define FOC_CONTROL_LOOP_FREQ_HZ        (1000)

/*! @brief Calibration samples in power of 2 */
#define CALIB_SAMPLES_2_POW             (10)

/*! @brief Calibration samples to average */
#define CALIB_AVG_SAMPLES               (1 << CALIB_SAMPLES_2_POW)

/*! @brief Limit for the resultant voltage vector is 0.866 of vdc */
#define FOC_PIIDQ_LMT_IQ                _IQ(0.866)

/*! @brief Size of the FOC variable logging buffer */
#define FOC_LOG_BUF_SIZE                (1024)

/*! @brief Uservar update rate in milliseconds */
#define FOC_USERVAR_UPDATE_MS           (200)

/*! @brief 12 bit offset to 8 bit shift factor */
#define FOC_12_TO_8_BIT_OFFSET_SHIFT    (4)

/** @brief Computing voltage scale factor */
#define USER_DEFAULT_VOLTAGE_SF                 (HAL_ADC_DEFAULT_REF_VOLTAGE * \
                                                (USER_DEFAULT_FOC_VOLT_RATIO))

/** @brief Computing current scale factor */
#define USER_DEFAULT_CURRENT_SF                 (HAL_ADC_DEFAULT_REF_VOLTAGE/ \
                    (2 * USER_DEFAULT_DRV_RSHUNT * USER_DEFAULT_BASE_CSA_GAIN))

/*! @brief Define command structure */
typedef struct {
    /*! State of the motor. @ref MOTOR_STATE */
    MOTOR_STATE motorState;
    /*! Iq PI controller reference used in closed loop
     for the outer current loop */
    _iq IqRef;
    /*! Id PI controller reference used in closed loop
     for the outer current loop */
    _iq IdRef;
    /*! Speed PI controller reference used in closed loop
     for the outer speed loop */
    _iq speedRef;
    /*! Outer loop for foc. @ref FOC_OUTER_LOOP */
    FOC_OUTER_LOOP outerLoop;
    /*! Selection of direction of the motor */
    FOC_DIRECTION directionReversal;
    /*! Selection of enabling closed loop */
    FOC_DISCL disCL;
} COMMAND;

/*! @brief Define align startup instance */
typedef struct {
    /*! Align counter */
    uint32_t cnt;
    /*! Align time set by user used for comparing with the align counter */
    uint32_t alignTime;
    /*! Motor phase current in the align state */
    _iq alignCurr;
} ALIGN_Instance;

/*! @brief Define rampup instance */
typedef struct {
    /*! Rampup current in Ampere */
    _iq rampupCurr;
    /*! Rampup rate converted to Hz per control loop cycles */
    _iq rate;
    /*! Rampup target in Hz */
    _iq target;
    /*! Output ramp */
    _iq rampOut;
} RAMPUP_Instance;

/*! @brief Define control structure */
typedef struct {
    /*! Align startup instance */
    ALIGN_Instance align;
    /*! Rampup instance */
    RAMPUP_Instance rampup;
    /*! Transit instance */
    TRANSIT_Instance transit;
    /*! Startup method selection */
    FOC_STARTUP startupMethod;
    /*! Slow first cycle frequency */
    _iq slowFirstCycFreq;
    /*! Slow first cycle complete flag */
    bool slowFirstCycCmplt;
} CONTROL;

/*! @brief Define FOC structure */
typedef struct {
    /*! Stores the FOC loop run counter */
    uint32_t isrTicker;
    /*! Stores the foc parameters */
    uint32_t parameter[PARA_IDX_MAX];
    /*! Stores the state of the motor */
    MOTOR state;
    /*! Stores the fault status of the motor */
    FAULT_STATUS faultStatus;
    /*! Stores the command variables */
    COMMAND cmd;
    /*! Stores the control variables */
    CONTROL control;
    /*! Stores the Raw ADC readings of DC bus */
    uint32_t vdcAdc;
    /*! Stores the Raw ADC readings of A phase */
    uint32_t iaAdc;
    /*! Stores the Raw ADC readings of B phase */
    uint32_t ibAdc;
    /*! Stores the Raw ADC readings of C phase */
    uint32_t icAdc;
    /*! Flag to show if offset calibration is complete */
    uint8_t offsetCalib;
    /*! Stores the A phase offset */
    uint32_t offsetA;
    /*! Stores the B phase offset */
    uint32_t offsetB;
    /*! Stores the C phase offset */
    uint32_t offsetC;
    /*! Stores the DC bus voltage */
    _iq vdc;
    /*! Stores the A phase current*/
    _iq ia;
    /*! Stores the B phase current*/
    _iq ib;
    /*! Stores the Over voltage limit */
    _iq overVoltageLimit;
    /*! Stores the Under voltage limit */
    _iq underVoltageLimit;
    /*! Stores the Over current limit */
    _iq overCurrentLimit;
    /*! Stores the Speed loop divider */
    FOC_SPD_DIV PISpdExecDivider;
    /*! Stores the Speed loop execution count */
    uint32_t PISpdExecCount;
    /*! Flag used to enable or disable the PWM channels */
    bool enablePWM;
    /*! Status of the PWM channels */
    bool enablePWMStatus;
    /*! Gain divider */
    uint8_t csaDiv;
    #if defined(USER_DEFAULT_ENABLE_GAIN_CHANGE) \
    || defined(USER_DEFAULT_ENABLE_FAULT_INPUT_CHECK)
    void * drv_handle;
    #endif
    /*! HAL instance of FOC */
    FOC_HAL hal;
    /*! ADC trigger point */
    uint32_t adcTrigVal;
    /*! Voltage scale factor */
    _iq voltageSF;
    /*! Current scale factor */
    _iq currentSF;
    /*! Flag used to run the userVar update */
    bool runUserVarUpdate;
    /*! Flag for initial user var loading complete */
    bool userVarsLoadCmplt;
} FOC_Instance;

/* Extern the foc modules */
extern IPD_Instance ipd;
extern PWMGEN_Instance pwmgen;
extern PI_Instance piSpd;
extern PI_Instance piId;
extern PI_Instance piIq;
extern CLARKE_Instance clarke;
extern PARK_Instance park;
extern IPARK_Instance ipark;
extern SVGEN_Instance svgen;
extern RMPCNTL_Instance rc;
extern RAMPGEN_Instance rg;
extern PHASEVOLT_Instance phaseVolt;
extern EST_Instance  est;

/* Extern variables */
extern uint32_t offsetCalibcnt;
extern uint32_t calibSumA;
extern uint32_t calibSumB;
extern uint32_t calibSumC;
extern uint32_t pAngle;
extern uint8_t userVarUpdateCounter;

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
* @brief     Loads the saved variables from flash to sram if the crc is correct
*/
void FOC_initFLASH(void);

/**
* @brief     Sets the values of foc vars as user vars + 1, it is used to later 
* update the foc vars with user vars while update parameters
* @param[in] userParamsFoc  A pointer to user parameters used by foc
* @param[in] userParams     A pointer to user parameters set by user
* @param[in] size           Size of variables to init in bytes
*/
void FOC_initUserParamsFoc(USER_PARAMS *userParamsFoc, USER_PARAMS *userParams, uint32_t size);

/**
* @brief     Copies from source parameters to destination parameters
* @param[in] dstParams  Pointer to destination parameters
* @param[in] srcParams  Pointer to source parameters
* @param[in] size       Size of variables to init in bytes
*/
void FOC_loadSavedParams(USER_PARAMS *dstParams, const USER_PARAMS *srcParams, uint32_t size);
/**
* @brief     Erases and writes the variables to flash
* @param[in] params Pointer to the data to be written to flash
* @param[in] size   Size of variables to init in bytes
*/
void FOC_writeFLASH(USER_PARAMS *params, uint32_t size);

/**
* @brief     Sets the FOC parameter and updates the modules
* @param[in] handle   A pointer to FOC instance
* @param[in] paraIdx  Index of the parameter to set.
* USER parameters of @ref PARA_IDX
* @param[in] value    Value to be set
*/
void FOC_setPara(FOC_Instance *handle, PARA_IDX paraIdx, int32_t value);

/**
* @brief     Sets the state machine to fault state and disables PWM output
* @param[in] handle   A pointer to FOC instance
*/
void FOC_setFault(FOC_Instance *handle);

/**
* @brief     Clears all the recorded faults
* @param[in] handle   A pointer to FOC instance
*/
void FOC_clearFault(FOC_Instance *handle);

/**
 * @brief     Gets the status of the PWM channels
* @param[in] handle   A pointer to FOC instance
 * @return    Returns the status of the pwm channels
 * 
 * @retval     true  The PWM channels are enabled
 * @retval     false The PWM channels are disabled
 */
__STATIC_INLINE bool FOC_getPwmEnableStatus(FOC_Instance *handle)
{
  return (handle->enablePWMStatus);
}

/**
 * @brief     Sets the status of the PWM channels
* @param[in] handle   A pointer to FOC instance
* @param[in] enablePWMState   PWM state to be set
 */
__STATIC_INLINE void FOC_setPwmEnableStatus(FOC_Instance *handle,
                                                            bool enablePWMState)
{
  handle->enablePWMStatus = enablePWMState;
}

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
* @brief     Calculated the limits for the PI IQ integral outputs
* @param[in] handle  A pointer to FOC instance
* @return    Returns the PI iq limit
*/
__STATIC_INLINE _iq FOC_getPIIQLMT(FOC_Instance *handle)
{
    _iq vdc = _IQtoIQ14(_IQmpy(handle->vdc, FOC_PIIDQ_LMT_IQ));
    _iq piOut = _IQtoIQ14(piId.out);
    return (_IQ14toIQ(_IQ14sqrt(_IQ14mpy(vdc, vdc) - _IQ14mpy(piOut, piOut))));
}

/**
* @brief     Set the PWM channel to trigger ADC
* @param[in] handle  A pointer to FOC instance
*/
__STATIC_INLINE void FOC_setADCTrig(FOC_Instance *handle)
{
    HAL_setPWMCcValue(handle->hal.adcTrigHal, handle->adcTrigVal);
}

/**
* @brief     Does the process of updating the foc variables with the userVar
* @param[in] handle  A pointer to FOC instance
* @param[in] userVar  A pointer to user variables
*/
void FOC_paramsUpdateProcess(FOC_Instance *handle, USER_VAR *userVar);

/**
* @brief     Checks the fault conditions
* @param[in] handle  A pointer to FOC instance
*/
__STATIC_INLINE void FOC_runMonitor(FOC_Instance *handle)
{
    bool fault = false;

    /* check the external fault input */
#ifdef USER_DEFAULT_ENABLE_FAULT_INPUT_CHECK
    if(FAULT_INPUT_STATUS((DRV_HANDLE *)handle->drv_handle))
    {
        fault = true;
        handle->faultStatus.bits.extFaultIn = true;
    }
#endif

    /* check the fault in phase currents */
    if(_IQabs(handle->ia) > handle->overCurrentLimit ||
     _IQabs(handle->ib) > handle->overCurrentLimit ||
     _IQabs(handle->ia + handle->ib) > handle->overCurrentLimit)
    {
        fault = true;
        handle->faultStatus.bits.ocp = true;
    }

    /* check the fault in bus voltage */
    if(handle->vdc > handle->overVoltageLimit)
    {
        fault = true;
        handle->faultStatus.bits.ovlo = true;
    }

    if(handle->vdc < handle->underVoltageLimit)
    {
        fault = true;
        handle->faultStatus.bits.uvlo = true;
    }

    if(fault == true)
    {
        FOC_setFault(handle);
    }
}

/**
* @brief     FOC motor control loop
* @param[in] handle  A pointer to FOC instance
*/
__STATIC_INLINE void FOC_CONTROL_run(FOC_Instance *handle)
{
    COMMAND *cmd = &handle->cmd;
    CONTROL *control = &handle->control;

    if(handle->cmd.motorState == MOTOR_STATE_STOP &&
    ((handle->state != MOTOR_STOP) &&
    (handle->state != MOTOR_IDLE) &&
    (handle->state != MOTOR_FAULT) &&
    (handle->state != MOTOR_STANDBY)))
    {
        handle->state = MOTOR_STOP;
    }

    /* state machine is run after the calibration, also the initial adc sampling
     * should be complete before the control loop runs*/
    if(handle->offsetCalib)
    {
        switch(handle->state)
        {
            case MOTOR_IDLE:
                handle->enablePWM = 0;
                TRANSIT_reset(&handle->control.transit);
                control->align.cnt = 0;
                control->rampup.rampOut = 0;
                control->slowFirstCycCmplt = 0;
                IPD_reset(&ipd);
                RC_reset(&rc);
                RG_reset(&rg);
                CLARKE_reset(&clarke);
                PARK_reset(&park);
                IPARK_reset(&ipark);
                EST_reset(&est);
                PI_reset(&piSpd);
                PI_reset(&piId);
                PI_reset(&piIq);
                handle->state = MOTOR_STANDBY;
                break;
            case MOTOR_STANDBY:
                if(handle->cmd.motorState == MOTOR_STATE_START)
                {
                    if(control->startupMethod == FOC_STARTUP_ALIGN)
                    {
                        handle->state = MOTOR_ALIGN;
                    }
                    else if(control->startupMethod == FOC_STARTUP_SLOW_FIRST)
                    {
                        handle->state = MOTOR_SLOW_FIRST_CYC;
                    }
                    else if(control->startupMethod == FOC_STARTUP_IPD)
                    {
                        handle->state = MOTOR_IPD;
                    }
                    /* reset the pwm enable checks */
                    handle->enablePWM = 0;
                    FOC_setPwmEnableStatus(handle, 1);
                }
                break;
            case MOTOR_ALIGN:
                handle->enablePWM = 1;
                if(control->align.cnt >= control->align.alignTime)
                {
                handle->state = MOTOR_RAMPUP;

                }
                else
                {
                    piIq.ref = control->align.alignCurr;
                    control->align.cnt++;
                }
                break;
            case MOTOR_SLOW_FIRST_CYC:
                if(control->slowFirstCycCmplt)
                {
                    handle->state = MOTOR_RAMPUP;
                }
                else
                {
                    handle->enablePWM = 1;
                    rg.Freq = handle->control.slowFirstCycFreq;
                    piIq.ref = control->align.alignCurr;
                }
                break;
            case MOTOR_IPD:
                if(ipd.ipdState == IPD_CMPLT)
                {
                    if(handle->cmd.directionReversal)
                    {
                        rg.Angle = ANGLE_MATH_2PIp3_PU - ipd.outAngle;
                    }
                    else
                    {
                        rg.Angle = ipd.outAngle;
                    }
                    handle->state = MOTOR_RAMPUP;
                }
                break;
            case MOTOR_RAMPUP:
                handle->enablePWM = 1;
                piIq.ref = control->rampup.rampupCurr;
                control->rampup.rampOut += control->rampup.rate;
                if(control->rampup.rampOut >= control->rampup.target)
                {
                    control->rampup.rampOut = control->rampup.target;
                    if(!handle->cmd.disCL)
                    {
                        handle->state = MOTOR_TRANSIT;
                        rc.SetpointValue = control->rampup.target;
                        piSpd.outI = piIq.fbk>>2;
                    }
                }
                break;
            case MOTOR_TRANSIT:
                if(control->transit.state == TRANSIT_STATE_COMPLETE)
                {
                    handle->state = MOTOR_RUN;
                }
                break;
            case MOTOR_RUN:
                /* This is expected to be empty */
                break;
            case MOTOR_STOP:
                handle->cmd.motorState = MOTOR_STATE_STOP;
                handle->enablePWM = 0;
                handle->state = MOTOR_IDLE;
                break;
            case MOTOR_FAULT:
                handle->cmd.motorState = MOTOR_STATE_STOP;
                handle->enablePWM = 0;
                if(handle->faultStatus.all == 0)
                {
                    /* When the user clears the fault the state goes to idle */
                    handle->state = MOTOR_IDLE;
                }
                break;
            default:
                /* This is expected to be empty */
                break;
        }

        /* Check for fault conditions */
        FOC_runMonitor(handle);
    }
    
    /* sets the update flag for the user variables */
    if(++userVarUpdateCounter >= FOC_USERVAR_UPDATE_MS)
    {
        userVarUpdateCounter = 0;
        handle->runUserVarUpdate = 1;
    }
}

/**
* @brief     Runs FOC loop
* @param[in] handle  A pointer to FOC instance
*/
__STATIC_INLINE void FOC_run(FOC_Instance *handle)
{
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
    park.Sine = _IQ24toIQ(ANGLE_getSine(park.Angle));
    park.Cosine = _IQ24toIQ(ANGLE_getSine(park.Angle + ANGLE_MATH_PIp2_PU));
    PARK_run(&park);
    
    if(handle->state == MOTOR_RUN || handle->state == MOTOR_TRANSIT)
    {
        if(handle->cmd.outerLoop == FOC_OUTER_LOOP_SPEED)
        {
            rc.TargetValue = handle->cmd.speedRef;
            RC_run(&rc);
            if (handle->PISpdExecCount >= handle->PISpdExecDivider)
            {
                piSpd.ref = rc.SetpointValue;
                piSpd.fbk = est.freq;
                /* max output of pi speed is the max measurable current */
                piSpd.outMax =  handle->currentSF>>handle->csaDiv;
                piSpd.outMin = -piSpd.outMax;
                piSpd.outIMax = piSpd.outMax;
                piSpd.outIMin = -piSpd.outIMax;
                PI_run(&piSpd);
                piIq.ref = piSpd.out;
                piId.ref = 0;
                handle->PISpdExecCount = 1;
            }
            else
            {
                handle->PISpdExecCount++;
            }
        }
        else
        {
            piIq.ref = handle->cmd.IqRef;
            piId.ref = handle->cmd.IdRef;
        }
    }

    switch (handle->state)
    {
        case MOTOR_ALIGN:
            park.Angle = 0;
            rg.Angle = 0;
            break;
        case MOTOR_SLOW_FIRST_CYC:
            pAngle = rg.Out;
            RG_run(&rg);
            if(rg.Out < pAngle)
            {
                handle->control.slowFirstCycCmplt = 1;
            }
            park.Angle = rg.Out;
            break;
        case MOTOR_IPD:
            IPD_run(&ipd, &handle->hal, 
                                handle->offsetA>>FOC_12_TO_8_BIT_OFFSET_SHIFT,
                                handle->offsetB>>FOC_12_TO_8_BIT_OFFSET_SHIFT, 
                                handle->offsetC>>FOC_12_TO_8_BIT_OFFSET_SHIFT);
            break;
        case MOTOR_RAMPUP:
            rg.Freq = handle->control.rampup.rampOut;
            RG_run(&rg);
            park.Angle = rg.Out;
            break;
        case MOTOR_TRANSIT:
            rg.Freq = handle->control.rampup.rampOut;
            RG_run(&rg);
            park.Angle = TRANSIT_run(&handle->control.transit, est.Theta,
                                                                        rg.Out);
            break;
        case MOTOR_RUN:
            park.Angle = est.Theta;
            break;
        default:
            break;
    }

    switch(handle->state)
    {
        case MOTOR_IPD:
            break;
        case MOTOR_ALIGN:
        case MOTOR_SLOW_FIRST_CYC:
        case MOTOR_RAMPUP:
        case MOTOR_TRANSIT:
        case MOTOR_RUN:
            /*------------------------------------------------------------------
                Connect inputs of the PI ID module and call run
            ------------------------------------------------------------------*/
            piId.fbk = park.Ds;
            piId.outIMax = _IQmpy(handle->vdc, FOC_PIIDQ_LMT_IQ);
            piId.outIMin = -piId.outIMax;
            piId.outMax = piId.outIMax;
            piId.outMin = -piId.outMax;
            PI_run(&piId);
            /*------------------------------------------------------------------
                Connect inputs of the PI IQ module and call run
            ------------------------------------------------------------------*/
            piIq.fbk = park.Qs;
            piIq.outIMax = FOC_getPIIQLMT(handle);
            piIq.outIMin = -piIq.outIMax;
            piIq.outMax = piIq.outIMax;
            piIq.outMin = -piIq.outMax;
            PI_run(&piIq);
            /*------------------------------------------------------------------
                Connect inputs of the PHASEVOLT module and call run
            ------------------------------------------------------------------*/
            phaseVolt.DcBusVolt = handle->vdc;
            phaseVolt.MfuncV1 = svgen.Ta;
            phaseVolt.MfuncV2 = svgen.Tb;
            phaseVolt.MfuncV3 = svgen.Tc;
            PHASEVOLT_run(&phaseVolt);
            /*------------------------------------------------------------------
                Connect inputs of the est module and call run
            ------------------------------------------------------------------*/
            est.Ialpha   = clarke.Alpha;
            est.Ibeta    = clarke.Beta;
            est.Valpha   = phaseVolt.Valpha;
            est.Vbeta    = phaseVolt.Vbeta;
            EST_run(&est);
            /*------------------------------------------------------------------
                Connect inputs of the IPARK module and call run
            ------------------------------------------------------------------*/
            ipark.Ds = _IQdiv(piId.out, handle->vdc);
            ipark.Qs = _IQdiv(piIq.out, handle->vdc);
            ipark.Sine = park.Sine;
            ipark.Cosine = park.Cosine;
            IPARK_run(&ipark);
            /*------------------------------------------------------------------
                Connect inputs of the SVGEN module and call run
            ------------------------------------------------------------------*/
            svgen.Ualpha = ipark.Alpha;
            svgen.Ubeta = ipark.Beta;
            SVGEN_run(&svgen);
            /*------------------------------------------------------------------
                Connect inputs of the PWMGEN module and call run
            ------------------------------------------------------------------*/
            if(handle->cmd.directionReversal)
            {
                pwmgen.MfuncC1 = svgen.Tb;
                pwmgen.MfuncC2 = svgen.Ta;
            }
            else
            {
                pwmgen.MfuncC1 = svgen.Ta;
                pwmgen.MfuncC2 = svgen.Tb;
            }
            pwmgen.MfuncC3 = svgen.Tc;
            PWMGEN_run(&pwmgen);

            HAL_setPWMCcValue(handle->hal.pwmAHal, pwmgen.dutyA);
            HAL_setPWMCcValue(handle->hal.pwmBHal, pwmgen.dutyB);
            HAL_setPWMCcValue(handle->hal.pwmCHal, pwmgen.dutyC);
            
        case MOTOR_IDLE:
        case MOTOR_STOP:
        case MOTOR_STANDBY:
            /* pwm control checks to */
            if(handle->enablePWM)
            {
                if(!FOC_getPwmEnableStatus(handle))
                {
                    /* Enable the PWM channels */
                    HAL_PWMEnableChannel(handle->hal.pwmAHal);
                    HAL_PWMEnableChannel(handle->hal.pwmBHal);
                    HAL_PWMEnableChannel(handle->hal.pwmCHal);
                    FOC_setPwmEnableStatus(handle, 1);
                }
            }
            else
            {
                if(FOC_getPwmEnableStatus(handle))
                {
                    /* Disable the PWM channels */
                    HAL_PWMDisableChannel(handle->hal.pwmAHal);
                    HAL_PWMDisableChannel(handle->hal.pwmBHal);
                    HAL_PWMDisableChannel(handle->hal.pwmCHal);
                    FOC_setPwmEnableStatus(handle, 0);
                }
            }
            break;
        default:
            /* This is expected to be empty */
            break;
    }
}

/**
* @brief     Updates the adc values in the FOC instance
* @param[in] handle  A pointer to FOC instance
* @param[in] vdcAdc  ADC reading of dc bus
* @param[in] iaAdc   ADC reading from A phase
* @param[in] ibAdc   ADC reading from B phase
* @param[in] icAdc   ADC reading from C phase
*/
__STATIC_INLINE void FOC_UpdateADCRaw(FOC_Instance *handle, uint16_t vdcAdc,
                                 uint16_t iaAdc, uint16_t ibAdc, uint16_t icAdc)
{
    handle->vdcAdc = vdcAdc;
    handle->iaAdc = iaAdc;
    handle->ibAdc = ibAdc;
    handle->icAdc = icAdc;
}

/**
* @brief     Runs the FOC tasks
* @param[in] handle  A pointer to FOC instance
* @param[in] vdcAdc  12 bit ADC reading of dc bus
* @param[in] iaAdc   12 bit ADC reading from A phase
* @param[in] ibAdc   12 bit ADC reading from B phase
* @param[in] icAdc   12 bit ADC reading from C phase
*/
__STATIC_INLINE void FOC_loop(FOC_Instance *handle, uint16_t vdcAdc,
                                 uint16_t iaAdc, uint16_t ibAdc, uint16_t icAdc)
{
    _iq iaTemp, ibTemp;
    FOC_UpdateADCRaw(handle, vdcAdc, iaAdc, ibAdc, icAdc);
    handle->isrTicker++;

    /* Set the voltages and currents */
    handle->vdc = _IQmpy(_IQ12toIQ(handle->vdcAdc), handle->voltageSF);
    iaTemp = _IQmpy(_IQ11toIQ((int32_t)handle->iaAdc- handle->offsetA),
                                         handle->currentSF)>>handle->csaDiv;
    ibTemp = _IQmpy(_IQ11toIQ((int32_t)handle->ibAdc- handle->offsetB),
                                         handle->currentSF)>>handle->csaDiv;

    if(handle->cmd.directionReversal)
    {
        handle->ia  = ibTemp;
        handle->ib  = iaTemp;
    }
    else
    {
        handle->ia  = iaTemp;
        handle->ib  = ibTemp;
    }

    if(handle->offsetCalib == 0 && (handle->userVarsLoadCmplt) )
    {
        HAL_setPWMCcValue(handle->hal.pwmAHal, pwmgen.HalfPerMax<<1);
        HAL_setPWMCcValue(handle->hal.pwmBHal, pwmgen.HalfPerMax<<1);
        HAL_setPWMCcValue(handle->hal.pwmCHal, pwmgen.HalfPerMax<<1);
        HAL_PWMEnableChannel(handle->hal.pwmAHal);
        HAL_PWMEnableChannel(handle->hal.pwmBHal);
        HAL_PWMEnableChannel(handle->hal.pwmCHal);
        offsetCalibcnt++;
        calibSumA += handle->iaAdc;
        calibSumB += handle->ibAdc;
        calibSumC += handle->icAdc;
        if(offsetCalibcnt >= CALIB_AVG_SAMPLES )
        {
            calibSumA >>= CALIB_SAMPLES_2_POW;
            calibSumB >>= CALIB_SAMPLES_2_POW;
            calibSumC >>= CALIB_SAMPLES_2_POW;
            handle->offsetA = calibSumA;
            handle->offsetB = calibSumB;
            handle->offsetC = calibSumC;
            handle->offsetCalib = 1;
            HAL_PWMDisableChannel(handle->hal.pwmAHal);
            HAL_PWMDisableChannel(handle->hal.pwmBHal);
            HAL_PWMDisableChannel(handle->hal.pwmCHal);
        }
    }
    else
    {
        /* Run the foc loop */
        FOC_run(handle);
    }

    /* Set the adc trigger point */
    FOC_setADCTrig(handle);
}

#ifdef __cplusplus
}
#endif
#endif /* FOC_H */
/** @}*/
