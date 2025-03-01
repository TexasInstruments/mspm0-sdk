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
 *  @file       measure.h
 *  @brief      HAL interface for FOC Module
 *
 * 
 *  @anchor measure_h
 *  # Overview
 *
 *  HAL Apis used for measurement from ADC
 *
 *  <hr>
 ******************************************************************************/

#ifndef MEASURE_H
#define MEASURE_H

#include "stdint.h"
#include "mcLib.h"
#include "iqNum.h"
#include "main.h"

/*! @brief Current offset averaging factor 2^n */
#define HAL_CURRENT_OFFSET_MEAS_BITS    5
/*! @brief Current offset averaging counts */
#define HAL_CURRENT_OFFSET_MEAS_COUNT   (1 << HAL_CURRENT_OFFSET_MEAS_BITS)

/*! @brief Inverse Vdc scaling */
#define INV_VDC_IQ_SCALING          25

/*! @brief Voltage gain types */
typedef enum
{
    /*! Phase voltage gain of Max Volt / 4 */
    VOLTAGE_GAIN_VMAX_BY_4 = 2,
    /*! Phase voltage gain of Max Volt / 2 */
    VOLTAGE_GAIN_VMAX_BY_2 = 1,
    /*! Phase voltage gain of Max Volt / 1 */
    VOLTAGE_GAIN_VMAX_BY_1 = 0,
}VOLTAGE_GAIN_TYPES;

/*! @brief HAL measurement types */
typedef enum
{
    /*! Measure Phase Currents */
    MEASURE_PHASE_CURRENT = 0,
    /*! Measure Phase Voltages */
    MEASURE_PHASE_VOLTAGE = 1,
}HAL_MEASUREMENT_TYPES;

/*! @brief HAL measurement types */
typedef enum
{
    /*! Default invalid */
    CURRENT_INVALID = 0,
    /*! Reconstructed three phase current with ignoring phase having max duty */
    CURRENT_THREE_SHUNT_DYNAMIC = 1,
    /*! Three phase current with A and C sampled simultaneously / ABC on single ADC if simultaneous sampling disabled */
    CURRENT_THREE_SHUNT_AB_C = 2,
    /*! Three phase current with A and B sampled simultaneously */
    CURRENT_THREE_SHUNT_A_BC = 3,
    /*! Two phase current with A and B sampled simultaneously / AB on single ADC if simultaneous sampling disabled */
    CURRENT_TWO_SHUNT_A_B = 4,
    /*! Two phase current with A and C sampled simultaneously / AC on single ADC if simultaneous sampling disabled */
    CURRENT_TWO_SHUNT_A_C = 5,
    /*! Two phase current with B and C sampled simultaneously / BC on single ADC if simultaneous sampling disabled*/
    CURRENT_TWO_SHUNT_B_C = 6,
    /*! DC bus current reconstructing phase currents */
    CURRENT_SINGLE_SHUNT = 7,
}CURRENT_SHUNT_TYPES;

/*! @brief ADC sampling ratio types */
typedef enum
{
    /*! Sample every PWM */
    ADC_SAMPLE_EVERY_PWM = 0,
    /*! Sample every 2 PWM */
    ADC_SAMPLE_EVERY_2_PWM = 1,
    /*! Sample every 3 PWM */
    ADC_SAMPLE_EVERY_3_PWM = 2,
    /*! Sample every 4 PWM */
    ADC_SAMPLE_EVERY_4_PWM = 3,
    /*! Sample every 5 PWM */
    ADC_SAMPLE_EVERY_5_PWM = 4,
    /*! Sample every 6 PWM */
    ADC_SAMPLE_EVERY_6_PWM = 5,
    /*! Sample every 7 PWM */
    ADC_SAMPLE_EVERY_7_PWM = 6,
    /*! Sample every 8 PWM */
    ADC_SAMPLE_EVERY_8_PWM = 7,
    /*! Sample every 9 PWM */
    ADC_SAMPLE_EVERY_9_PWM = 8,
    /*! Sample every 10 PWM */
    ADC_SAMPLE_EVERY_10_PWM = 9,
    /*! Sample every 11 PWM */
    ADC_SAMPLE_EVERY_11_PWM = 10,
    /*! Sample every 12 PWM */
    ADC_SAMPLE_EVERY_12_PWM = 11,
    /*! Sample every 13 PWM */
    ADC_SAMPLE_EVERY_13_PWM = 12,
    /*! Sample every 14 PWM */
    ADC_SAMPLE_EVERY_14_PWM = 13,
    /*! Sample every 15 PWM */
    ADC_SAMPLE_EVERY_15_PWM = 14,
    /*! Sample every 16 PWM */
    ADC_SAMPLE_EVERY_16_PWM = 15,
}ADC_SAMPLING_RATIO_TYPES;

/*! @brief Current offset structure */
typedef struct
{
    int16_t
        /*! A-phase current offset */
        iaOffset,
        /*! B-phase on ADC0 current offset */
        ibADC0Offset,
        /*! B-phase on ADC1 current offset */
        ibADC1Offset,
        /*! C-phase current offset */
        icOffset,
        /*! DC current offset First Sample */
        i1Offset,
        /*! DC current offset Second Sample */
        i2Offset;
}CURRENT_OFFSET_T;

/*! @brief Offset measurement states */
typedef enum
{
    /*! Offset init state */
    OFFSET_INIT = 0,
    /*! Compute state */
    COMPUTE_OFFSET = 1,
    /*! Offset complete state */
    OFFSET_COMPLETE = 2,
}OFFSET_MEASUREMENT_STATES;

/*! @brief measureCurrentFlags structure */
typedef struct
{
    _Bool
        /*! Three shunt dynamic sampling */
        threeShuntDynamic: 1,
        /*! Three shunt dynamic sampling set by user */
        threeShuntDynamicSet: 1,
        /*! Offset measurement status */
        offsetAB_CStatus: 1,
        /*! Offset measurement at initial gain status */
        offsetGainStatus: 1,
        /*! Offset measurement status */
        offsetStatus: 1;
}measureCurrentFlags;

/*! @brief Measure current flag */
typedef union
{
    /*! measure current flags */
    measureCurrentFlags b;
    /*! Block Access */
    uint16_t w;
}MEASURE_CURRENT_FLAGS_T;

/*! @brief Measure current structure */
typedef struct
{
    /*! Offset measurement states */
    OFFSET_MEASUREMENT_STATES
        state;

    MC_ABC_T
        /*! Three Phase Current */
        iabc,
        /*! Three Phase Raw Current */
        iabcRaw;

    MC_SECTOR_TYPES
        /*! Previous previous Voltage Sector */
        *pPrevPrevSector;

    CURRENT_OFFSET_T
        /*! Current offset at peak current of IMax/1 */
        offsetGainIMaxBy1,
        /*! Current offset */
        *pOffset;

    int32_t
        /*! A-phase current offset */
        iaSum,
        /*! B-phase on ADC0 current offset */
        ibADC0Sum,
        /*! B-phase on ADC1 current offset */
        ibADC1Sum,
        /*! C-phase current offset */
        icSum,
        /*! DC current sum of first sample */
        i1Sum,
        /*! DC current sum of second sample */
        i2Sum,
        /*! Current magnitude squared */
        iMagSqr;

    int32_t
        /*! First current sample */
        iFirstSample,
        /*! Second current sample */
        iSecondSample;

    CURRENT_SHUNT_TYPES
        /*! Current shunt configuration  to sense set by user */
        currentShuntSet,
        /*! Current Shunt configuration to sense */
        currentShunt;

    const MC_PHASE_TYPES
        /*! Maximum duty phase: sample other two currents */
        *pMaxDutyPhase;

    MC_PHASE_TYPES
        /*! Previous sample of maximum duty phase */
        prevMaxDutyPhase;

    int16_t
        /*! Offset counter */
        offsetMeasCount;

    int32_t 
        /*! Filtered source current or bus current */
        *pSourceCurrentFilt;

    MEASURE_CURRENT_FLAGS_T
        /*! Measure current flags */
        flags;             

}HAL_MEASURE_CURRENT_T;

/*! @brief Measure voltage structure */
typedef struct
{
    VOLTAGE_GAIN_TYPES
        /*! Voltage Gain */
        voltageGain,
        /*! Voltage Gain set by user */
        voltageGainSet;

    int32_t
        /*! DC bus voltage */
        vdc,
        /*! Filtered DC bus voltage */
        vdcFilt,
        /*! Filtered Inverse of DC bus voltage */
        invVdcFilt;

    MC_ABC_T
        /*! Three Phase Phase Voltages */
        vabc,
        /*! Three Phase Raw Phase Voltages */
        vabcRaw;

}HAL_MEASURE_VOLTAGE_T;

/*! @brief Measure motor inputs structure */
typedef struct
{
    ADC_SAMPLING_RATIO_TYPES
        /*! Ratio of PWM to ADC Sampling frequency */
        adcSamplingRatio;   

    int32_t
        /*! Normalized Sampling Time */
        deltaT,
        /*! PWM Period */
        pwmPeriod,
        /*! Propagation Delay To trigger */
        propagationDelay;

    HAL_MEASUREMENT_TYPES
        /*! selection of Phase Current or Phase voltage measured */
        measureQuantity;

    HAL_MEASURE_CURRENT_T
        /*! Sensed Currents */
        current;

    HAL_MEASURE_VOLTAGE_T
        /*! Sensed Voltages */
        voltage;
}HAL_MEASURE_MOTOR_INPUTS_T;

/*! @brief Extern motor inputs */
extern HAL_MEASURE_MOTOR_INPUTS_T *g_pMotorInputs;

/**
 * @brief     Initializes motor iputs
 * @param[in] pMotorInputs   Pointer to motor input
 */
void inputsInit(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs);

/**
 * @brief     Measures offset of current input
 * @param[in] pMotorInputs   Pointer to motor input
 */
void measureCurrentOffset(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs);

/**
 * @brief     Status of offset measurement
 * @param[in] pMotorInputs   Pointer to motor input
 * @return    Returns the status of offset measurement
 * 
 * @retval     true  Offset measurement completed
 * @retval     false Offset measurement not completed
 */
inline _Bool getMeasureOffsetStatus(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs)
{
    return pMotorInputs->current.flags.b.offsetStatus;
}

#ifdef __cplusplus
}
#endif
#endif /* MEASURE_H */
