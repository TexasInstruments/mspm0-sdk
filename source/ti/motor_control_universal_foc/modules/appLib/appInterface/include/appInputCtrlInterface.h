/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
 *  @file       appInputCtrlInterface.h
 *  @brief      Application Defines Module
 *
 * 
 *  @anchor appInputCtrlInterface_h
 *  # Overview
 *
 *  defines the macros for application layer
 *
 *  <hr>
 ******************************************************************************/

#ifndef APPINPUTCTRLINTERFACE_H
#define APPINPUTCTRLINTERFACE_H

#include "stdint.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief userInputISD structure */
typedef struct
{
    uint32_t
    /*! Reserved 1 */
    Reserved1:              9,
    /*! BEMF threshold to detect if motor is stationary  */
    statDetectThr:          3,
    /*! Hi-Z time  */
    hiZTime:                4,
    /*! Brake time  */
    brkTime:                4,
    /*! Brake configuration for time based or current based braking */
    brkConfig:              1,
    /*! Minimum speed threshold to resynchronize to close loop (% of
    MAX_SPEED) */
    fwDrvResyncThr:         4,
    /*! Resynchronization enable */
    resyncEn:               1,
    /*! Reverse drive enable */
    rvsDrEn:                1,
    /*! Hi-Z enable */
    hiZEn:                  1,
    /*! Brake enable */
    brakeEn:                1,
    /*! ISD enable */
    isdEn:                  1,
    /*! Reserved */
    Reserved2:              2;
}userInputISD;

/*! @brief ISD Config register */
typedef union
{
    /*! Bitwise access */
    userInputISD b;
    /*! Block access */
    uint32_t w;
}USER_INPUT_ISDCFG_T;

/*! @brief userInputMotorStartUp1 structure */
typedef struct
{
    uint32_t
    /*! Reserved 1  */
    Reserved1:               1,
    /*! Open loop current limit configuration */
    olILimitCfg:            1,
    /*! Number of times IPD is repeatedly executed */
    ipdRepeat:              2,
    /*! IPD advance angle */
    ipdAdvAngle:            2,
    /*! Reserved 2 */
    Reserved2:               1,
    /*! IPD current threshold */
    ipdCurrThresh:          7,
    /*! IPD clock frequency */
    ipdClkFreq:             3,
    /*! Align or slow first cycle current limit */
    alignOrSlowCurrLimit:   5,
    /*! Align time */
    alignTime:              4,
    /*! Align, slow first cycle and open loop current ramp rate */
    alignSlowRampRate:      4,
    /*! Motor start-up method */
    mtrStartUpOption:       2;
}userInputMotorStartUp1;

/*! @brief START_UP1_CONFIG register */
typedef union
{
    /*! Bitwise access */
    userInputMotorStartUp1 b;
    /*! Block access */
    uint32_t w;
}USER_INPUT_MTR_STARTUP1_T;

/*! @brief userInputMotorStartUp2 structure */
typedef struct
{
    uint32_t
    /*! Ramp rate for reducing difference between estimated theta and open
    loop theta */
    thetaErrRampRate:   3,
    /*! First cycle frequency in open loop for align, double align and IPD
    startup options */
    FirstCycFreqSel:    1,
    /*! Frequency of first cycle in close loop startup (% of MAX_SPEED) */
    slowFirstCycFreq:   4,
    /*! Align angle */
    alignAngle:         5,
    /*! Open to close loop handoff threshold (% of MAX_SPEED) */
    olClHandOffThr:     5,
    /*! Reserved 1 */
    Reserved1:      1,
    /*! Open loop acceleration coefficient A2 */
    olAcc2:             4,
    /*! Open loop acceleration coefficient A1 */
    olAcc1:             4,
    /*! Open loop current limit */
    olILimit:           5;
}userInputMotorStartUp2;

/*! @brief START_UP2_CONFIG register */
typedef union
{
    /*! Bitwise access */
    userInputMotorStartUp2 b;
    /*! Block access */
    uint32_t w;
}USER_INPUT_MTR_STARTUP2_T;

/*! @brief userInputCloseLoop1 structure */
typedef struct
{
    uint32_t
    /*! Reserved 1*/
    Reserved1:           1,
    /*! Reserved 2*/
    Reserved2:           1,
    /*! AVS enable */
    avsEn:                  1,
    /*! PWM modulation */
    pwmMode:                1,
    /*! Output PWM switching frequency */
    pwmFreqOut:             4,
    /*! Closed loop deceleration. This register is used only if AVS is disabled
    and CL_DEC_CONFIG is set to '0' */
    clDec:                  5,
    /*! Closed loop deceleration configuration */
    clDecCfg:               1,
    /*! Closed loop acceleration */
    clAcc:                  5,
    /*! Overmodulation enable */
    overModEnable:          1,
    /*! Motor stop options */
    mtrStopOption:          2,
    /*! Maximum current reference in torque PI loop */
    iLimit:                 5,
    /*! Reduces the ADC sampling rate to half value, so that PWM freq can be high */
    highFreqFOCEn:          1,
    /*! Control mode */
    controlMode:            2,
    /*! Reserved 3 */
    Reserved3:               2;
}userInputCloseLoop1;

/*! @brief CLOSELOOP1_CONFIG register */
typedef union
{
    /*! Bitwise access */
    userInputCloseLoop1 b;
    /*! Block access */
    uint32_t w;
}USER_INPUT_CLOSE_LOOP1_T;

/*! @brief userInputCloseLoop2 structure */
typedef struct
{
    uint32_t
    /*! Reserved 1*/
    Reserved1:           14,
    /*! Lead angle for voltage input */
    leadAngle:			5,
    /*! Brake current threshold */
    brkCurrThr:         5,
    /*! Speed threshold for BRAKE pin and motor stop options (low-side
    braking or high-side braking or align braking) (% of MAX_SPEED) */
    brkSpeedThr:        4,
    /*! Speed threshold for active spin down (% of MAX_SPEED) */
    actSpinThr:         4;
}userInputCloseLoop2;

/*! @brief CLOSELOOP2_CONFIG register */
typedef union
{
    /*! Bitwise access */
    userInputCloseLoop2 b;
    /*! Block access */
    uint32_t w;
}USER_INPUT_CLOSE_LOOP2_T;

/*! @brief userInputCloseLoop2 structure */
typedef struct
{
    uint32_t
    /*! Enable Flux Weakening */
    fluxWeakeningEn:            1,
    /*! Flux Weakening Current Ratio */
    fluxWeakCurrRatio:          3,
    /*! Flux Weakening Modulation Index Reference */
    fluxWeakeningReference:     2,
    /*! Enable Maximum Torque Per Amp */
    mtpaEnable:                 1,
    /*! Reserved 1 */
    Reserved1:                   25;
}userInputFieldCtrl;

/*! @brief userInputFieldCtrl structure */
typedef union
{
    /*! Bitwise access */
    userInputFieldCtrl b;
    /*! Block access */
    uint32_t    w;
}USER_INPUT_FIELD_CTRL_T;
/*! @brief userInputFaultCfg1 structure */
typedef struct
{
    uint32_t
    /*! Motor Lock Mode */
    mtrLckMode:         2,
    /*! Lock detection retry time */
    lockRetry:          4,
    /*! Reserved 1 */
    Reserved1:           26;
}userInputFaultCfg1;

/*! @brief FAULT_CONFIG1 register */
typedef union
{
    /*! Bitwise access */
    userInputFaultCfg1 b;
    /*! Block access */
    uint32_t w;
}USER_INPUT_FAULT_CFG1_T;

/*! @brief userInputFaultCfg2 structure */
typedef struct
{
    uint32_t
    /*! Overvoltage fault mode */
    maxVmMode:          1,
    /*! Maximum voltage for running motor */
    maxVmMtr:           3,
    /*! Undervoltage fault mode */
    minVmMode:          1,
    /*! Minimum voltage for running motor */
    minVmMtr:           3,
    /*! Reserved 2 */
    Reserved2:          5,
    /*! No motor lock threshold */
    noMtrThr:           5,
    /*! Abnormal BEMF lock threshold (% of expected BEMF) */
    abnBemfThr:         3,
    /*! Abnormal speed lock threshold (% of MAX_SPEED) */
    lockAbnSpeed:       3,
    /*! Lock 3 : No motor enable */
    noMtrFaultEn:            1,
    /*! Lock 2 : Abnormal BEMF enable */
    lowBemfFaultEn:            1,
    /*! Lock 1 : Abnormal speed enable */
    abnSpdFaultEn:            1,
    /*! Reserved 1*/
    Reserved1:           5;
}userInputFaultCfg2;

/*! @brief FAULT_CONFIG2 register */
typedef union
{
    /*! Bitwise access */
    userInputFaultCfg2 b;
    /*! Block access */
    uint32_t w;
}USER_INPUT_FAULT_CFG2_T;

/*! @brief userInputMiscAlgo1 structure */
typedef struct
{
    uint32_t
    /*! Reserved 1 */
    Reserved1:     3,
    /*! Persistence time for declaring motor is stopped after applying brake */
    brkCurrPersist:     2,
    /*! Reserved 2 */
    Reserved2:           5,
    /*! Persistence time for declaring motor is running */
    isdRunTime:         2,
    /*! Persistence time for declaring motor has stopped */
    isdStopTime:        2,
    /*! Reserved 3 */
    Reserved3:          2,
    /*! Close loop acceleration when estimator is not yet fully aligned */
    clSlowAcc:          4,
    /*! Maximum number of timer overflows allowed while IPD */
    ipdMaxOverflow:     2,
    /*! Reserved 4*/
    Reserved4:           10;
}userInputMiscAlgo;

/*! @brief FAULT_CONFIG2 register */
typedef union
{
    /*! Bitwise access */
    userInputMiscAlgo b;
    /*! Block access */
    uint32_t w;
}USER_INPUT_MISC_ALGO_T;

/*! @brief userInputPinCfg structure */
typedef struct
{
    uint32_t
        /*! Brake pin override */
        brakeInp:           2,
        /*! Brake pin mode */
        brakePinMode:       1,
        /*! Reserved 1*/
        Reserved1:          16,
        /*! Bus voltage filter enable */
        vdcFiltDis:         1,
        /*! Reserved 2*/
        Reserved2:           12;
}userInputPinCfg;

/*! @brief PIN_CONFIG register */
typedef union
{
    /*! Bitwise access */
    userInputPinCfg b;
    /*! Block access */
    uint32_t w;
}USER_INPUT_PIN_CFG_T;

/*! @brief userInputPeriCfg1 structure */
typedef struct
{
    uint32_t
    /*! Response to change of DIR pin status */
    dirChangeMode:             1,
    /*! DIR pin override */
    dirInput:                   2,
    /*! Bus current limit enable */
    busCurrLimitEnable:         1,
    /*! Bus current limit */
    busCurrLimit:               5,
    /*! deadtime for PWM outputs */
    mcuDeadTime:                6,
    /*! Reserved */
    Reserved1:                   17;
}userInputPeriCfg1;

/*! @brief PERI_CONFIG1 register */
typedef union
{
    /*! Bitwise access */
    userInputPeriCfg1 b;
    /*! Block access */
    uint32_t w;
}USER_INPUT_PERI_CFG1_T;

/*! @brief SYSTEM_CONFIGURATIONS*/
typedef struct
{
    /*! motor resistance */
    uint32_t                            mtrResist;
    /*! motor inductance */
    uint32_t                            mtrInductance;
    /*! motor inductance saliency percentage */
    float                               mtrSaliency;
    /*! motor backemf constant */
    uint32_t                            mtrBemfConst;
    /*! Maximum DC Bus Max Voltage in mV*/
    float                               voltageBase;
    /*! Maximum DC Bus Current in mA*/
    float                               currentBase;
    /*! Maximum Motor speed in mHz*/
    float                               maxMotorSpeed;
    /*! Max Motor Power in W */
    uint32_t                            maxMotorPower;
    /*! Speed Loop Kp */
    float                               speedLoopKp;
    /*! Speed Loop Ki */
    float                               speedLoopKi;
    /*! Current Loop Kp */
    float                               currLoopKp;
    /*! Current Loop Ki */
    float                               currLoopKi;
    /*! Integral Controller Gain for  Flux Weakening */
    float                               fluxWeakeningKi;
    /*! Proportional Control Gain for  Flux Weakening */
    float                               fluxWeakeningKp;
    /*! SMO Sliding Mode Gain */
    float                               kSlide;

}SYSTEM_PARAMETERS_T;

/*! @brief DEVICE_CONFIG1 register */
typedef struct
{
    /*! SYSTEM PARAMS */
    SYSTEM_PARAMETERS_T             systemParams;
    /*! isdCfg config */
    USER_INPUT_ISDCFG_T             isdCfg;
    /*! startup 1 */
    USER_INPUT_MTR_STARTUP1_T       mtrStartUp1;
    /*! startup 2 */
    USER_INPUT_MTR_STARTUP2_T       mtrStartUp2;
    /*! close loop 1 */
    USER_INPUT_CLOSE_LOOP1_T        closeLoop1;
    /*! close loop 2 */
    USER_INPUT_CLOSE_LOOP2_T        closeLoop2;
    /*! Field Control */
    USER_INPUT_FIELD_CTRL_T         fieldCtrl;
    /*! fault config */
    USER_INPUT_FAULT_CFG1_T         faultCfg1;
    /*! fault config */
    USER_INPUT_FAULT_CFG2_T         faultCfg2;
    /*! int algo 1 */
    USER_INPUT_MISC_ALGO_T          miscAlgo;
    /*! pin config */
    USER_INPUT_PIN_CFG_T            pinCfg;
    /*! peripheral config 2 */
    USER_INPUT_PERI_CFG1_T          periphCfg1;
    /*! Reserved */
} USER_INPUT_INTERFACE_T;

/*! @brief ramSpeedCtrl structure */
typedef struct
{
    uint32_t
        /*! Speed Input */
        speedInput:           15,
        /*! Reserved */
        reserved:             17;
}ramSpeedCtrl;

/*! @brief SPEED_CTRL register */
typedef union
{
    /*! Bit wise access */
    ramSpeedCtrl b;    
    /*! Block access */
    uint32_t w;
}RAM_SPEED_CTRL_T;

/*! @brief ramAlgoDebugCtrl1 structure */
typedef struct
{
    uint32_t
    /*! Reserved */
    Reserved1:                   10,
    /*! Force align angle state source select */
    forceAlignAngleSrcSelect:   1,
    /*! Force ISD enable */
    forceISDEn:                 1,
    /*! Force IPD enable */
    forceIPDEn:                 1,
    /*! Force slow first cycle enable */
    forceSlowCycleFirstCycleEn: 1,
    /*! Force align state enable */
    forceAlignEn:               1,
    /*! Disable closed loop */
    closeLoopDis:               1,
    /*! Reserved */
    Reserved2:                   6,
    /*! Align angle used during forced align state */
    forcedAlignAngle:           9,
    /*! Clears all faults */
    clearFlt:                   1;
}ramAlgoDebugCtrl1;

/*! @brief ALGO_DEBUG_1 register */
typedef union
{
    /*! Bit wise access */
    ramAlgoDebugCtrl1 b;
    /*! Block access */
    uint32_t w;
}RAM_ALGO_DEBUG_1_T;

/*! @brief ramAlgoDebugCtrl2 structure */
typedef struct
{
    uint32_t
    /*! Reserved 1 */
    Reserved1:              6,
    /*! Vq applied when current loop and speed loop are disabled */
    forceVQCurrLoopDis:    10,
    /*! Vd applied when current loop and speed loop are disabled */
    forceVDCurrLoopDis:    10,
    /*! Current loop and speed loop are disabled */
    currLoopDis:           1,
    /*! statusUpdate Flag */
    statusUpdateEn:        1,
    /*! Update configurations Flag */
    updateConfigs:         1,
    /*! Update System Parameters Dynamically Flag */
    updateSysParams:       1,
    /*! Reserved 2*/
    Reserved2:             2;
}ramAlgoDebugCtrl2;

/*! @brief ALGO_DEBUG_2 register */
typedef union
{
    /*! Bit wise access */
    ramAlgoDebugCtrl2 b;
    /*! Block access */
    uint32_t w;
}RAM_ALGO_DEBUG_2_T;

/*! @brief ramAlgoDebugCtrl2 structure */
typedef struct
{
    uint32_t
    /*! Id reference used when Flux Mode is Enabled */
    fluxModeReference:          10,
    /*! Reserved */
    reserved1:             22;
}ramAlgoDebugCtrl3;

/*! @brief ALGO_DEBUG_2 register */
typedef union
{
    /*! Bit wise access */
    ramAlgoDebugCtrl3 b;
    /*! Block access */
    uint32_t w;
}RAM_ALGO_DEBUG_3_T;


/*! @brief DAC_CNTRL register */
typedef struct
{
    /*! DAC enable */
    uint16_t dacEn;
    /*! Unipolar ouput Left or right shift, used if dacScalingFactor is 0 */
    int16_t dacShift;
    /*! Dac scale factor in iq 27 */
    int32_t dacScalingFactor;
    /*! Variable address to output through DAC */
    uint32_t dacOutAddr;
}RAM_DAC_CNTRL_T;

/*! @brief DEVICE_CONFIG1 register */
typedef struct
{
    /*! Speed control */
    RAM_SPEED_CTRL_T    speedCtrl;
    /*! Algo debug 1 */
    RAM_ALGO_DEBUG_1_T  algoDebugCtrl1;
    /*! Algo debug 2 */
    RAM_ALGO_DEBUG_2_T  algoDebugCtrl2;
    /*! Algo debug 3 */
    RAM_ALGO_DEBUG_3_T  algoDebugCtrl3;
    /*! DAC control */
    RAM_DAC_CNTRL_T  dacCtrl;
}USER_CTRL_INTERFACE_T;

/* Extern parameter */

/** @brief pointer to eeprom base */
extern USER_INPUT_INTERFACE_T *pUserInputRegs;

/** @brief pointer to ram variables */
extern USER_CTRL_INTERFACE_T *pUserCtrlRegs;

#ifdef __cplusplus
}
#endif
#endif /* APPINPUTCTRLINTERFACE_H */
