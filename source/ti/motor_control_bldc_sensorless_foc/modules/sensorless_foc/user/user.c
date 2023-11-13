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
#include "user.h"
#include "hal.h"

/** @brief IQ value of stator resistance */
#define USER_DEFAULT_FOC_RS_IQ             _IQ(USER_DEFAULT_FOC_RS)

/** @brief IQ value of stator inductance */
#define USER_DEFAULT_FOC_LS_IQ             _IQ(USER_DEFAULT_FOC_LS)

/** @brief IQ value of motor back emf constant */
#define USER_DEFAULT_FOC_KE_IQ             _IQ(MOTOR_PARA_KE)

/** @brief IQ motor slow first cycle frequency */
#define USER_DEFAULT_FOC_SLOW_FIRST_CYC_FREQ_IQ \
                                    _IQ(USER_DEFAULT_FOC_SLOW_FIRST_CYC_FREQ)

/** @brief IQ value of align current */
#define USER_DEFAULT_FOC_ALIGN_CURRENT_IQ   _IQ(USER_DEFAULT_FOC_ALIGN_CURRENT)

/** @brief IQ value of align time in seconds */
#define USER_DEFAULT_FOC_ALIGN_TIME_IQ      _IQ(USER_DEFAULT_FOC_ALIGN_TIME)

/** @brief IQ value of ipd advance angle */
#define USER_DEFAULT_FOC_IPD_ADV_ANGLE_IQ   _IQ(USER_DEFAULT_FOC_IPD_ADV_ANGLE)

/** @brief IQ value of ipd voltage inject */
#define USER_DEFAULT_FOC_IPD_VOLT_INJECT_IQ \
                                        _IQ(USER_DEFAULT_FOC_IPD_VOLT_INJECT)

/** @brief IQ value of rampup current */
#define USER_DEFAULT_FOC_RAMPUP_CURRENT_IQ _IQ(USER_DEFAULT_FOC_RAMPUP_CURRENT)

/** @brief IQ value of rampup rate in Hz/s */
#define USER_DEFAULT_FOC_RAMPUP_RATE_IQ    _IQ(USER_DEFAULT_FOC_RAMPUP_RATE)

/** @brief IQ value of ramp target in Hz/s */
#define USER_DEFAULT_FOC_RAMPUP_TARGET_IQ  _IQ(USER_DEFAULT_FOC_RAMPUP_TARGET)

/** @brief IQ value of speed reference ramp rate in Hz/s */
#define USER_DEFAULT_FOC_SPEED_REF_RAMP_RATE_IQ  \
                                    _IQ(USER_DEFAULT_FOC_SPEED_REF_RAMP_RATE)

/** @brief IQ value of KP for PI controller for speed */
#define USER_DEFAULT_FOC_PISPD_KP_IQ       _IQ(USER_DEFAULT_FOC_PISPD_KP)

/** @brief IQ value of KI for PI controller for speed */
#define USER_DEFAULT_FOC_PISPD_KI_IQ       _IQ(USER_DEFAULT_FOC_PISPD_KI)

/** @brief IQ value of KP for PI controller for Iq */
#define USER_DEFAULT_FOC_PIIQ_KP_IQ        _IQ(USER_DEFAULT_FOC_PIIQ_KP)

/** @brief IQ value of KI for PI controller for Iq */
#define USER_DEFAULT_FOC_PIIQ_KI_IQ        _IQ(USER_DEFAULT_FOC_PIIQ_KI)

/** @brief IQ value of KP for PI controller for Id */
#define USER_DEFAULT_FOC_PIID_KP_IQ        _IQ(USER_DEFAULT_FOC_PIID_KP)

/** @brief IQ value of KI for PI controller for Id */
#define USER_DEFAULT_FOC_PIID_KI_IQ        _IQ(USER_DEFAULT_FOC_PIID_KI)

/** @brief IQ value of over voltage limit */
#define USER_DEFAULT_FOC_OVER_VOLT_IQ       _IQ(USER_DEFAULT_FOC_OVER_VOLT)

/** @brief IQ value of under voltage limit */
#define USER_DEFAULT_FOC_UNDER_VOLT_IQ      _IQ(USER_DEFAULT_FOC_UNDER_VOLT)

/** @brief IQ value of over current limit */
#define USER_DEFAULT_FOC_OVER_CURRENT_IQ    _IQ(USER_DEFAULT_FOC_OVER_CURRENT)

/** @brief Parameters at reserved area of SRAM ,this is modified by user */
USER_VAR userVar __attribute__((section(".motorConfigSram")));

/** @brief Parameters used by foc */
USER_PARAMS userParamsFoc;

/** @brief Parameters at reserved area of FLASH */
__attribute__((retain))
__attribute__((section(".motorConfigFlash")))
const USER_PARAMS savedParams;

/** @brief Default parameters in FLASH */
__attribute__((retain))
const USER_PARAMS defaultParams  =
{
    .rs = USER_DEFAULT_FOC_RS_IQ,
    .ls = USER_DEFAULT_FOC_LS_IQ,
    .ke = USER_DEFAULT_FOC_KE_IQ,
    .maxFreq = USER_DEFAULT_FOC_MAX_FREQ,
    .PWMFreq = USER_DEFAULT_FOC_PWMFREQ,
    .deadband = USER_DEFAULT_FOC_DEADBAND,
    .CSAGain = USER_DEFAULT_CSA_GAIN,
    .outerLoop = USER_DEFAULT_FOC_OUTER_LOOP,
    .directionReversal = USER_DEFAULT_FOC_DIRECTION_REVERSAL,
    .startupMethod = USER_DEFAULT_FOC_STARTUP_METHOD,
    .slowCycFreq = USER_DEFAULT_FOC_SLOW_FIRST_CYC_FREQ_IQ,
    .alignCur = USER_DEFAULT_FOC_ALIGN_CURRENT_IQ,
    .alignTime = USER_DEFAULT_FOC_ALIGN_TIME_IQ,
    .IPDCurrThresh = USER_DEFAULT_IPD_THRESHOLD_COUNT,
    .IPDFreq = USER_DEFAULT_FOC_IPD_FREQ,
    .rampupCur = USER_DEFAULT_FOC_RAMPUP_CURRENT_IQ,
    .rampupRate = USER_DEFAULT_FOC_RAMPUP_RATE_IQ,
    .rampupTarget = USER_DEFAULT_FOC_RAMPUP_TARGET_IQ,
    .speedRefRampRate = USER_DEFAULT_FOC_SPEED_REF_RAMP_RATE_IQ,
    .piSpdKp = USER_DEFAULT_FOC_PISPD_KP_IQ,
    .piSpdKi = USER_DEFAULT_FOC_PISPD_KI_IQ,
    .piSpdDiv = USER_DEFAULT_FOC_PISPD_DIV,
    .piIdKp = USER_DEFAULT_FOC_PIIQ_KP_IQ,
    .piIdKi = USER_DEFAULT_FOC_PIIQ_KI_IQ,
    .piIqKp = USER_DEFAULT_FOC_PIID_KP_IQ,
    .piIqKi = USER_DEFAULT_FOC_PIID_KI_IQ,
    .overVoltageLimit = USER_DEFAULT_FOC_OVER_VOLT_IQ,
    .underVoltageLimit = USER_DEFAULT_FOC_UNDER_VOLT_IQ,
    .overCurrentLimit = USER_DEFAULT_FOC_OVER_CURRENT_IQ,
};
