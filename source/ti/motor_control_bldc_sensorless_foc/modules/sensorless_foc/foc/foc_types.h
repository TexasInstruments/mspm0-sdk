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
 *  @file       foc_types.h
 *  @brief      Sensorless FOC Motor Control Library FOC TYPES Module
 *
 *
 *  @anchor foc_types_h
 *  # Overview
 *
 *  FOC types defines
 *
 *  <hr>
 ******************************************************************************/
/** @defgroup FOC_TYPES FOC_LIBRARY/FOC_TYPES
 * @{
 */
#ifndef FOC_TYPES_H
#define FOC_TYPES_H

#include "hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief 32 bit highest value is the reserved value */
#define _RESERVED    (uint32_t)0xFFFFFFFFFU

/*! @brief Define FOC HAL structure */
typedef struct{
    /*! PWM channel set for A phase. @ref HAL_PWM_CHANNEL */
    HAL_PWM_CHANNEL pwmAHal;
    /*! PWM channel set for B phase. @ref HAL_PWM_CHANNEL */
    HAL_PWM_CHANNEL pwmBHal;
    /*! PWM channel set for C phase. @ref HAL_PWM_CHANNEL */
    HAL_PWM_CHANNEL pwmCHal;
    /*! ADC channel set for A phase. @ref HAL_ADC_CHANNEL */
    HAL_ADC_CHANNEL isena;
    /*! ADC channel set for B phase. @ref HAL_ADC_CHANNEL */
    HAL_ADC_CHANNEL isenb;
    /*! ADC channel set for C phase. @ref HAL_ADC_CHANNEL */
    HAL_ADC_CHANNEL isenc;
    /*! PWM channel set for ADC trigger. @ref HAL_PWM_CHANNEL */
    HAL_PWM_CHANNEL adcTrigHal;
    /*! COMP channel set for A phase comparator. @ref HAL_COMP_CHANNEL */
    HAL_COMP_CHANNEL isenAComp;
    /*! COMP channel set for B phase comparator. @ref HAL_COMP_CHANNEL */
    HAL_COMP_CHANNEL isenBComp;
    /*! COMP channel set for C phase comparator. @ref HAL_COMP_CHANNEL */
    HAL_COMP_CHANNEL isenCComp;
    /*! Capture channel set for IPD timer. @ref HAL_CAPTURE_CHANNEL */
    HAL_CAPTURE_CHANNEL ipdTimer;
} FOC_HAL;

/*!
 * @enum MOTOR
 * @brief MOTOR state machine state
 */
typedef enum{
    /*! Idle state used to get ready to start */
    MOTOR_IDLE = 0,
    /*! Motor in standby */
    MOTOR_STANDBY,
    /*! Motor aligned to a phase */
    MOTOR_ALIGN,
    /*! Motor in slow first cycle */
    MOTOR_SLOW_FIRST_CYC,
    /*! Motor in ipd routine */
    MOTOR_IPD,
    /*! Motor openloop rampup */
    MOTOR_RAMPUP,
    /*! Motor in transit to closed loop */
    MOTOR_TRANSIT,
    /*! Motor running in closed loop */
    MOTOR_RUN,
    /*! Motor stop state */
    MOTOR_STOP,
    /*! Motor fault state */
    MOTOR_FAULT,
    /*! Reserved value, should not be set */
    MOTOR_RESERVED = _RESERVED,
} MOTOR;

/*! @brief Define fault status bits */
typedef struct{
   /*! External fault flag */
    uint32_t extFaultIn:1;
   /*! Undervoltage fault flag */
   uint32_t uvlo:1;
   /*! Overvoltage fault flag */
   uint32_t ovlo:1;
   /*! Overcurrent fault flag */
   uint32_t ocp:1;
} FAULT_STATUS_BITS;

/*! @brief Define fault status */
typedef union{
    /*! 32 bit fault status */
    uint32_t all;
    /*! Fault status bitwise */
    FAULT_STATUS_BITS bits;
} FAULT_STATUS;

/*! @enum FOC_DIRECTION */
typedef enum{
    /*! Phase direction clockwise */
    FOC_DIRECTION_CW = 0,
    /*! Phase direction counter clockwise */
    FOC_DIRECTION_CCW,
    /*! Reserved value, should not be set */
    FOC_DIRECTION_RESERVED = _RESERVED,
} FOC_DIRECTION;

/*! @enum FOC_DISCL */
typedef enum{
    /*! Closed loop is enabled */
    FOC_DISCL_ENABLE = 0,
    /*! Closed loop is disabled */
    FOC_DISCL_DISABLE,
    /*! Reserved value, should not be set */
    FOC_DISCL_RESERVED = _RESERVED,
} FOC_DISCL;

/*! @enum FOC_OUTER_LOOP */
typedef enum{
    /*! Outer loop is current */
    FOC_OUTER_LOOP_CURRENT = 0,
    /*! Outer loop is speed */
    FOC_OUTER_LOOP_SPEED,
    /*! Reserved value, should not be set */
    FOC_OUTER_LOOP_RESERVED = _RESERVED,
} FOC_OUTER_LOOP;

/*! @enum MOTOR_STATE */
typedef enum{
    /*! Stop the motor */
    MOTOR_STATE_STOP = 0,
    /*! Start motor */
    MOTOR_STATE_START
} MOTOR_STATE;

/*! @enum FOC_CMD */
typedef enum{
    /*! Speedref command */
    FOC_CMD_SPEEDREF = 0,
    /*! Id reference command */
    FOC_CMD_IDREF,
    /*! Iq reference command */
    FOC_CMD_IQREF,
} FOC_CMD;

/*! @enum FOC_STARTUP */
typedef enum{
    /*! Align and go startup */
    FOC_STARTUP_ALIGN = 0,
    /*! Slow first cycle startup */
    FOC_STARTUP_SLOW_FIRST,
    /*! IPD startup */
    FOC_STARTUP_IPD,
    /*! Reserved value, should not be set */
    FOC_STARTUP_RESERVED = _RESERVED,
} FOC_STARTUP;

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
} FOC_LOG_IDX;

/*! @enum FOC_PWM_FREQ */
typedef enum {
    /*! PWM frequency 10000 */
    FOC_PWM_FREQ_10000 = 0,
    /*! PWM frequency 15000 */
    FOC_PWM_FREQ_15000,
    /*! PWM frequency 20000 */
    FOC_PWM_FREQ_20000,
    /*! PWM frequency 25000 */
    FOC_PWM_FREQ_25000,
    /*! PWM frequency 30000 */
    FOC_PWM_FREQ_30000,
    /*! PWM frequency 35000 */
    FOC_PWM_FREQ_35000,
    /*! PWM frequency 40000 */
    FOC_PWM_FREQ_40000,
    /*! PWM frequency 45000 */
    FOC_PWM_FREQ_45000,
    /*! PWM frequency 50000 */
    FOC_PWM_FREQ_50000,
    /*! Max values of pwm frequency available */
    FOC_PWM_FREQ_MAX,
    /*! Reserved value, should not be set */
    FOC_PWM_FREQ_RESERVED = _RESERVED,
} FOC_PWM_FREQ;

/*! @enum FOC_IPD_THRES_COUNT */
typedef enum{
    /*! IPD threshold count 1 */
    FOC_IPD_THRES_COUNT_1 = 1,
    /*! IPD threshold count 2 */
    FOC_IPD_THRES_COUNT_2 = 2,
    /*! IPD threshold count 3 */
    FOC_IPD_THRES_COUNT_3 = 3,
    /*! IPD threshold count 4 */
    FOC_IPD_THRES_COUNT_4 = 4,
    /*! IPD threshold count 5 */
    FOC_IPD_THRES_COUNT_5 = 5,
    /*! IPD threshold count 6 */
    FOC_IPD_THRES_COUNT_6 = 6,
    /*! IPD threshold count 7 */
    FOC_IPD_THRES_COUNT_7 = 7,
    /*! IPD threshold count 8 */
    FOC_IPD_THRES_COUNT_8 = 8,
    /*! IPD threshold count 9 */
    FOC_IPD_THRES_COUNT_9 = 9,
    /*! IPD threshold count 10 */
    FOC_IPD_THRES_COUNT_10 = 10,
    /*! IPD threshold count 11 */
    FOC_IPD_THRES_COUNT_11 = 11,
    /*! IPD threshold count 12 */
    FOC_IPD_THRES_COUNT_12 = 12,
    /*! IPD threshold count 13 */
    FOC_IPD_THRES_COUNT_13 = 13,
    /*! IPD threshold count 14 */
    FOC_IPD_THRES_COUNT_14 = 14,
    /*! IPD threshold count 15 */
    FOC_IPD_THRES_COUNT_15 = 15,
    /*! IPD threshold count 16 */
    FOC_IPD_THRES_COUNT_16 = 16,
    /*! IPD threshold count 17 */
    FOC_IPD_THRES_COUNT_17 = 17,
    /*! IPD threshold count 18 */
    FOC_IPD_THRES_COUNT_18 = 18,
    /*! IPD threshold count 19 */
    FOC_IPD_THRES_COUNT_19 = 19,
    /*! IPD threshold count 20 */
    FOC_IPD_THRES_COUNT_20 = 20,
    /*! IPD threshold count 21 */
    FOC_IPD_THRES_COUNT_21 = 21,
    /*! IPD threshold count 22 */
    FOC_IPD_THRES_COUNT_22 = 22,
    /*! IPD threshold count 23 */
    FOC_IPD_THRES_COUNT_23 = 23,
    /*! IPD threshold count 24 */
    FOC_IPD_THRES_COUNT_24 = 24,
    /*! IPD threshold count 25 */
    FOC_IPD_THRES_COUNT_25 = 25,
    /*! IPD threshold count 26 */
    FOC_IPD_THRES_COUNT_26 = 26,
    /*! IPD threshold count 27 */
    FOC_IPD_THRES_COUNT_27 = 27,
    /*! IPD threshold count 28 */
    FOC_IPD_THRES_COUNT_28 = 28,
    /*! IPD threshold count 29 */
    FOC_IPD_THRES_COUNT_29 = 29,
    /*! IPD threshold count 30 */
    FOC_IPD_THRES_COUNT_30 = 30,
    /*! IPD threshold count 31 */
    FOC_IPD_THRES_COUNT_31 = 31,
    /*! IPD threshold count 32 */
    FOC_IPD_THRES_COUNT_32 = 32,
    /*! IPD threshold count 33 */
    FOC_IPD_THRES_COUNT_33 = 33,
    /*! IPD threshold count 34 */
    FOC_IPD_THRES_COUNT_34 = 34,
    /*! IPD threshold count 35 */
    FOC_IPD_THRES_COUNT_35 = 35,
    /*! IPD threshold count 36 */
    FOC_IPD_THRES_COUNT_36 = 36,
    /*! IPD threshold count 37 */
    FOC_IPD_THRES_COUNT_37 = 37,
    /*! IPD threshold count 38 */
    FOC_IPD_THRES_COUNT_38 = 38,
    /*! IPD threshold count 39 */
    FOC_IPD_THRES_COUNT_39 = 39,
    /*! IPD threshold count 40 */
    FOC_IPD_THRES_COUNT_40 = 40,
    /*! IPD threshold count 41 */
    FOC_IPD_THRES_COUNT_41 = 41,
    /*! IPD threshold count 42 */
    FOC_IPD_THRES_COUNT_42 = 42,
    /*! IPD threshold count 43 */
    FOC_IPD_THRES_COUNT_43 = 43,
    /*! IPD threshold count 44 */
    FOC_IPD_THRES_COUNT_44 = 44,
    /*! IPD threshold count 45 */
    FOC_IPD_THRES_COUNT_45 = 45,
    /*! IPD threshold count 46 */
    FOC_IPD_THRES_COUNT_46 = 46,
    /*! IPD threshold count 47 */
    FOC_IPD_THRES_COUNT_47 = 47,
    /*! IPD threshold count 48 */
    FOC_IPD_THRES_COUNT_48 = 48,
    /*! IPD threshold count 49 */
    FOC_IPD_THRES_COUNT_49 = 49,
    /*! IPD threshold count 50 */
    FOC_IPD_THRES_COUNT_50 = 50,
    /*! IPD threshold count 51 */
    FOC_IPD_THRES_COUNT_51 = 51,
    /*! IPD threshold count 52 */
    FOC_IPD_THRES_COUNT_52 = 52,
    /*! IPD threshold count 53 */
    FOC_IPD_THRES_COUNT_53 = 53,
    /*! IPD threshold count 54 */
    FOC_IPD_THRES_COUNT_54 = 54,
    /*! IPD threshold count 55 */
    FOC_IPD_THRES_COUNT_55 = 55,
    /*! IPD threshold count 56 */
    FOC_IPD_THRES_COUNT_56 = 56,
    /*! IPD threshold count 57 */
    FOC_IPD_THRES_COUNT_57 = 57,
    /*! IPD threshold count 58 */
    FOC_IPD_THRES_COUNT_58 = 58,
    /*! IPD threshold count 59 */
    FOC_IPD_THRES_COUNT_59 = 59,
    /*! IPD threshold count 60 */
    FOC_IPD_THRES_COUNT_60 = 60,
    /*! IPD threshold count 61 */
    FOC_IPD_THRES_COUNT_61 = 61,
    /*! IPD threshold count 62 */
    FOC_IPD_THRES_COUNT_62 = 62,
    /*! IPD threshold count 63 */
    FOC_IPD_THRES_COUNT_63 = 63,
    /*! IPD threshold count 64 */
    FOC_IPD_THRES_COUNT_64 = 64,
    /*! IPD threshold count 65 */
    FOC_IPD_THRES_COUNT_65 = 65,
    /*! IPD threshold count 66 */
    FOC_IPD_THRES_COUNT_66 = 66,
    /*! IPD threshold count 67 */
    FOC_IPD_THRES_COUNT_67 = 67,
    /*! IPD threshold count 68 */
    FOC_IPD_THRES_COUNT_68 = 68,
    /*! IPD threshold count 69 */
    FOC_IPD_THRES_COUNT_69 = 69,
    /*! IPD threshold count 70 */
    FOC_IPD_THRES_COUNT_70 = 70,
    /*! IPD threshold count 71 */
    FOC_IPD_THRES_COUNT_71 = 71,
    /*! IPD threshold count 72 */
    FOC_IPD_THRES_COUNT_72 = 72,
    /*! IPD threshold count 73 */
    FOC_IPD_THRES_COUNT_73 = 73,
    /*! IPD threshold count 74 */
    FOC_IPD_THRES_COUNT_74 = 74,
    /*! IPD threshold count 75 */
    FOC_IPD_THRES_COUNT_75 = 75,
    /*! IPD threshold count 76 */
    FOC_IPD_THRES_COUNT_76 = 76,
    /*! IPD threshold count 77 */
    FOC_IPD_THRES_COUNT_77 = 77,
    /*! IPD threshold count 78 */
    FOC_IPD_THRES_COUNT_78 = 78,
    /*! IPD threshold count 79 */
    FOC_IPD_THRES_COUNT_79 = 79,
    /*! IPD threshold count 80 */
    FOC_IPD_THRES_COUNT_80 = 80,
    /*! IPD threshold count 81 */
    FOC_IPD_THRES_COUNT_81 = 81,
    /*! IPD threshold count 82 */
    FOC_IPD_THRES_COUNT_82 = 82,
    /*! IPD threshold count 83 */
    FOC_IPD_THRES_COUNT_83 = 83,
    /*! IPD threshold count 84 */
    FOC_IPD_THRES_COUNT_84 = 84,
    /*! IPD threshold count 85 */
    FOC_IPD_THRES_COUNT_85 = 85,
    /*! IPD threshold count 86 */
    FOC_IPD_THRES_COUNT_86 = 86,
    /*! IPD threshold count 87 */
    FOC_IPD_THRES_COUNT_87 = 87,
    /*! IPD threshold count 88 */
    FOC_IPD_THRES_COUNT_88 = 88,
    /*! IPD threshold count 89 */
    FOC_IPD_THRES_COUNT_89 = 89,
    /*! IPD threshold count 90 */
    FOC_IPD_THRES_COUNT_90 = 90,
    /*! IPD threshold count 91 */
    FOC_IPD_THRES_COUNT_91 = 91,
    /*! IPD threshold count 92 */
    FOC_IPD_THRES_COUNT_92 = 92,
    /*! IPD threshold count 93 */
    FOC_IPD_THRES_COUNT_93 = 93,
    /*! IPD threshold count 94 */
    FOC_IPD_THRES_COUNT_94 = 94,
    /*! IPD threshold count 95 */
    FOC_IPD_THRES_COUNT_95 = 95,
    /*! IPD threshold count 96 */
    FOC_IPD_THRES_COUNT_96 = 96,
    /*! IPD threshold count 97 */
    FOC_IPD_THRES_COUNT_97 = 97,
    /*! IPD threshold count 98 */
    FOC_IPD_THRES_COUNT_98 = 98,
    /*! IPD threshold count 99 */
    FOC_IPD_THRES_COUNT_99 = 99,
    /*! IPD threshold count 100 */
    FOC_IPD_THRES_COUNT_100 = 100,
    /*! Maximum values in threshold count */
    FOC_IPD_THRES_COUNT_MAX = 101,
    /*! Reserved value, should not be set */
    FOC_IPD_THRES_COUNT_RESERVED = _RESERVED,
} FOC_IPD_THRES_COUNT;

/*! @enum FOC_IPD_FREQ */
typedef enum {
    /*! IPD frequency 50 */
    FOC_IPD_FREQ_50 = 0,
    /*! IPD frequency 100 */
    FOC_IPD_FREQ_100,
    /*! IPD frequency 250 */
    FOC_IPD_FREQ_250,
    /*! IPD frequency 500 */
    FOC_IPD_FREQ_500,
    /*! IPD frequency 1000 */
    FOC_IPD_FREQ_1000,
    /*! IPD frequency 2000 */
    FOC_IPD_FREQ_2000,
    /*! IPD frequency 4000 */
    FOC_IPD_FREQ_4000,
    /*! IPD frequency 8000 */
    FOC_IPD_FREQ_8000,
    /*! IPD frequency 10000 */
    FOC_IPD_FREQ_10000,
    /*! Max values of IPD frequency available */
    FOC_IPD_FREQ_MAX,
    /*! Reserved value, should not be set */
    FOC_IPD_FREQ_RESERVED = _RESERVED,
} FOC_IPD_FREQ;

/*! @enum FOC_SPD_DIV */
typedef enum{
    /*! Speed loop execution divider is 1 */
    FOC_SPD_DIV_1 = 1,
    /*! Speed loop execution divider is 2  */
    FOC_SPD_DIV_2 = 2,
    /*! Speed loop execution divider is 5 */
    FOC_SPD_DIV_5 = 5,
    /*! Speed loop execution divider is 10 */
    FOC_SPD_DIV_10 = 10,
    /*! Speed loop execution divider is 15  */
    FOC_SPD_DIV_15 = 15,
    /*! Speed loop execution divider is 20  */
    FOC_SPD_DIV_20 = 20,
    /*! Reserved value, should not be set */
    FOC_SPD_DIV_RESERVED = _RESERVED,
} FOC_SPD_DIV;

#ifdef __cplusplus
}
#endif
#endif /* FOC_TYPES_H */
/** @}*/
