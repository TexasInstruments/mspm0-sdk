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
 *  @file       user.h
 *  @brief      Sensorless FOC Motor Control Library USER Module
 *
 * 
 *  @anchor user_h
 *  # Overview
 *
 *  flash parameter defines
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_USER_USER FOC_USER/USER
 * @{
 */
#ifndef USER_H
#define USER_H

#include <stdint.h>
#include "foc_types.h"
#include "motor_params.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BSXL8323RS_REVA
#include "drv8323rs.h"
#include "param_drv8323rs.h"
#endif

/** @brief DRV CSA gain when gain setting is 0 */
#define USER_DEFAULT_BASE_CSA_GAIN              (drvGain[0])

/*! @enum FOC_CSA_GAIN */
typedef enum {
#ifdef DRV_CSA_GAIN_MAP_0
    /*! FOC CSA gain setting 0 */
    FOC_CSA_GAIN_0 = DRV_CSA_GAIN_MAP_0,
#endif
#ifdef DRV_CSA_GAIN_MAP_1
    /*! FOC CSA gain setting 1 */
    FOC_CSA_GAIN_1 = DRV_CSA_GAIN_MAP_1,
#endif
#ifdef DRV_CSA_GAIN_MAP_2
    /*! FOC CSA gain setting 2 */
    FOC_CSA_GAIN_2 = DRV_CSA_GAIN_MAP_2,
#endif
#ifdef DRV_CSA_GAIN_MAP_3
    /*! FOC CSA gain setting 3 */
    FOC_CSA_GAIN_3 = DRV_CSA_GAIN_MAP_3,
#endif
    /*! Max number of CSA GAIN */
    FOC_CSA_GAIN_MAX,
    /*! Reserved value, should not be set */
    FOC_CSA_GAIN_RESERVED = _RESERVED,
} FOC_CSA_GAIN;

/*! @brief Define user params */
typedef struct {
    /*! Stator resistance */
    uint32_t rs;
    /*! Stator inductance */
    uint32_t ls;
    /*! Motor backemf constant in  V/Hz */
    uint32_t ke;
    /*! Motor max frequency */
    uint32_t maxFreq;
    /*! PWM frequency selection. One of @ref FOC_PWM_FREQ */
    FOC_PWM_FREQ PWMFreq;
    /*! Deadband in ns */
    uint32_t deadband;
    /*! CSA gain selection. One of @ref FOC_CSA_GAIN */
    FOC_CSA_GAIN CSAGain;
    /*! Outer loop selection. One of @ref FOC_OUTER_LOOP */
    FOC_OUTER_LOOP outerLoop;
    /*! CW CCW selection. One of @ref FOC_DIRECTION */
    FOC_DIRECTION directionReversal;
    /*! Startup method selection. One of @ref FOC_STARTUP */
    FOC_STARTUP startupMethod;
    /*! Slow first cycle frequency */
    uint32_t slowCycFreq;
    /*! Align or slow first cycle current */
    uint32_t alignCur;
    /*! Align time */
    uint32_t alignTime;
    /*! IPD current threshold counts. One of @ref FOC_IPD_THRES_COUNT */
    FOC_IPD_THRES_COUNT IPDCurrThresh;
    /*! IPD pulse frequency. One of @ref FOC_IPD_FREQ */
    FOC_IPD_FREQ IPDFreq;
    /*! Rampup current */
    uint32_t rampupCur;
    /*! Rampup rate in Hz/s */
    uint32_t rampupRate;
    /*! Rampup target */
    uint32_t rampupTarget;
    /*! Ramprate for speed reference */
    uint32_t speedRefRampRate;
    /*! Proportional gain for PI speed */
    uint32_t piSpdKp;
    /*! Integral gain for PI speed */
    uint32_t piSpdKi;
    /*! PI speed execution divider */
    FOC_SPD_DIV piSpdDiv;
    /*! Proportional gain for PI Iq */
    uint32_t piIqKp;
    /*! Integral gain for PI Iq */
    uint32_t piIqKi;
    /*! Proportional gain for PI Id */
    uint32_t piIdKp;
    /*! Integral gain for PI Id */
    uint32_t piIdKi;
    /*! Over voltage limit */
    uint32_t overVoltageLimit;
    /*! Under voltage limit */
    uint32_t underVoltageLimit;
    /*! Over current limit */
    uint32_t overCurrentLimit;
    /*! Write configuration flag */
    uint32_t writeFlag;
    /*! CRC value */
    uint32_t crc;
} USER_PARAMS;

/*! @brief Define control register 1 */
typedef struct {
    /*! Clear fault flag */
    uint32_t clearFault:1;
    /*! Flash access key */
    uint32_t flashAccessKey:4;
    /*! Flash write flag */
    uint32_t flashWrite:1;
    /*! Flash read saved parameters flag */
    uint32_t flashRead:1;
    /*! Flash read default parameters flag */
    uint32_t defaultRead:1;
    /*! Disable closed loop flag */
    uint32_t disCL:1;
    /*! Motor enable */
    uint32_t enableMotor:1;
} CNTRL_REG_1;

/*! @brief Define all control registers */
typedef struct {
    /*! Control register 1 */
    CNTRL_REG_1 cntrl1;
    /*! Speed reference */
    uint32_t speedRef;
    /*! Q axis current reference */
    int32_t iqRef;
    /*! D axis current reference */
    int32_t idRef;
}CNTRL_REGS;

/*! @brief Define Status registers */
typedef struct {
    /*! Fault status */
    FAULT_STATUS faultStatus;
    /*! State machine state */
    MOTOR motorState;
    /*! DC bus voltage */
    int32_t vdc;
    /*! Estimated speed */
    int32_t speedFbk;
    /*! Reference speed */
    int32_t speedRef;
    /*! Q axis current */
    int32_t iqCurr;
    /*! Reference Q axis current */
    int32_t iqCurrRef;
    /*! D axis current */
    int32_t idCurr;
    /*! Reference D axis current */
    int32_t idCurrRef;
}STATUS_REGS;

/*! @brief Define user variables structure */
typedef struct {
    /*! Control registers  */
    CNTRL_REGS controlRegs;
    /*! Status registers  */
    STATUS_REGS statusRegs;
    /*! User parameters  */
    USER_PARAMS userParams;
} USER_VAR;

/* Extern parameter */

/** @brief Parameters at reserved area of SRAM ,this is modified by user */
extern USER_VAR userVar;

/** @brief Parameters used by foc */
extern USER_PARAMS userParamsFoc;

/** @brief Default parameters in FLASH */
extern const USER_PARAMS defaultParams;

/** @brief Parameters at reserved area of FLASH */
extern const USER_PARAMS savedParams;

#ifdef __cplusplus
}
#endif
#endif /* USER_H */
/** @}*/
