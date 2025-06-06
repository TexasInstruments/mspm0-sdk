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
#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Application Major FW Version */
#define APP_MAJOR_VER  (0x01)
/*! @brief Application Minor FW Version */
#define APP_MINOR_VER  (0x01)
/*! @brief Application Patch FW Version */
#define APP_PATCH_VER  (0x01)
/*! @brief Application FW Version */
#define APP_FW_VERSION (APP_MAJOR_VER << 16) + (APP_MINOR_VER<<8) + (APP_PATCH_VER)

/*! @brief Sqrt(3) in float */
#define SQRT_3              	1.732050807568877

/* Application Memory Address Base
 * User should ensure that any new variables
 * added to below structures shouldn't
 * overflow into other sections of memory*/

/*! @brief FLASH shadow address base
*    1KBytes of SRAM Memory space reserved for storing
*    the Tuning Configurations that can be flashed
*    */
#define APP_USER_INP_BASE            0x20200000UL

/*! @brief User CTRL Inputs address base */
#define APP_USER_CTRL_BASE          0x20200400UL

/*! @brief User Status address base */
#define APP_USER_STATUS_BASE        0x20200430UL

/*! @brief FW Version address base */
#define APP_SHADOW_END             0x20200F00UL

/*! @brief FW Version address base */
#define APP_FW_VER_BASE             0x20200FFCUL

#ifdef DRV8329

/*! @brief DRV8329 has non inverting isense */
#define _NONINVERT_ISEN
/*! @brief IPD feature is enabled */
#define __IPD_ENABLE
/*! @brief DRV8329 propagation delay */
#define DRIVER_PROPAGATION_DELAY_nS                164
/*! @brief DRV8329 minimum on time(rise time + settling time) */
#define DRIVER_MIN_ON_TIME_nS                      1300
/*! @brief DC voltage base value */
#define DC_VOLTAGE_BASE                             76.04
/*! @brief Full scale readable current used as current base value,
calculated using (FULL Scale Voltage(3.3) - CSA_REF(3.3/8) )/(Rsense(20mOhm) * CSA Gain(20)) */
#define FULL_SCALE_CURRENT_BASE                    7.218
/*! @brief Current shunt configuration */
#define __CURRENT_SINGLE_SHUNT
/*! @brief Enable dynamic current shunt changing */
#define DYNAMIC_CURRENT_SHUNT_CONFIG_EN             FALSE

#elif defined DRV8323

/*! @brief DRV8323 has inverting isense */
#define _INVERT_ISEN
/*! @brief IPD feature is enabled */
#define __IPD_ENABLE
/*! @brief DRV8323 propagation delay */
#define DRIVER_PROPAGATION_DELAY_nS                100
/*! @brief DRV8323 minimum on time(rise time + settling time) */
#define DRIVER_MIN_ON_TIME_nS                      4000
/*! @brief DC voltage base value */
#define DC_VOLTAGE_BASE                             57.42
/*! @brief Full scale readable current used as current base value,
calculated using (FULL Scale Voltage(3.3)/2*(Rsense(7mOhm) * CSA Gain(20)) */
#define FULL_SCALE_CURRENT_BASE                    11.785
/*! @brief Current shunt configuration */
#define __CURRENT_THREE_SHUNT_DYNAMIC
/*! @brief Enable dynamic current shunt changing */
#define DYNAMIC_CURRENT_SHUNT_CONFIG_EN             TRUE

#elif defined DRV8316

/*! @brief DRV8316 has inverting isense */
#define _INVERT_ISEN
/*! @brief IPD feature is enabled */
#define __IPD_ENABLE
/*! @brief DRV8323 propagation delay */
#define DRIVER_PROPAGATION_DELAY_nS                100
/*! @brief DRV8316 minimum on time (rise time + settling time) */
#define DRIVER_MIN_ON_TIME_nS                      1000
/*! @brief DC voltage base value */
#define DC_VOLTAGE_BASE                             44.59
/*! @brief Full scale readable current used as current base value,
calculated using (FULL Scale Voltage(3.3)/2* CSA Gain(0.15(v/A))) */
#define FULL_SCALE_CURRENT_BASE                     11.0
/*! @brief Current shunt configuration */
#define __CURRENT_THREE_SHUNT_DYNAMIC
/*! @brief Enable dynamic current shunt changing */
#define DYNAMIC_CURRENT_SHUNT_CONFIG_EN             TRUE

#elif defined TIDA010250

/*! @brief TIDA010250 has inverting isense */
#define _INVERT_ISEN
/*! @brief IPD feature is enabled */
#define __IPD_ENABLE
/*! @brief TIDA010250 propagation delay */
#define DRIVER_PROPAGATION_DELAY_nS                500
/*! @brief TIDA010250 minimum on time (rise time + settling time) */
#define DRIVER_MIN_ON_TIME_nS                      8000
/*! @brief DC voltage base value */
#define DC_VOLTAGE_BASE                             448.0
/*! @brief Full scale readable current used as current base value,
calculated using (FULL Scale Voltage(3.3)/2* CSA Gain) */
#define FULL_SCALE_CURRENT_BASE                     8.25
/*! @brief Current shunt configuration */
#define __CURRENT_THREE_SHUNT_DYNAMIC
/*! @brief Enable dynamic current shunt changing */
#define DYNAMIC_CURRENT_SHUNT_CONFIG_EN             FALSE//TRUE

#elif defined TIDA010265

/*! @brief TIDA010265 has non inverting isense */
#define _NONINVERT_ISEN
/*! @brief IPD feature is enabled */
#define __IPD_ENABLE
/*! @brief TIDA010265 propagation delay */
#define DRIVER_PROPAGATION_DELAY_nS                100
/*! @brief TIDA010265 minimum on time (rise time + settling time) */
#define DRIVER_MIN_ON_TIME_nS                      2000
/*! @brief DC voltage base value */
#define DC_VOLTAGE_BASE                             404.129
/*! @brief Full scale readable current used as current base value,
calculated using (FULL Scale Voltage(3.3)/2* CSA Gain) */
#define FULL_SCALE_CURRENT_BASE                     7.919
/*! @brief Current shunt configuration */
#define __CURRENT_TWO_SHUNT_B_C_N
#define __CURRENT_SINGLE_SHUNT
/*! @brief Enable dynamic current shunt changing */
#define DYNAMIC_CURRENT_SHUNT_CONFIG_EN             FALSE

#endif

/*! @brief Total time taken for ADC to sample all currents */
#define ADC_SAMPLING_TIME_ns                        1800

/*! @brief MOTOR base Speed */
#define MOTOR_MAX_SPEED                             (200.0)
/*! @brief MOTOR base voltage */
#define MOTOR_VOLTAGE_BASE                          DC_VOLTAGE_BASE / SQRT_3

/*! @brief Hysteresis for under voltage */
#define UNDER_VOLTAGE_HYSTERESIS_VOLTS              (1.0)

/*! @brief Hysteresis for over voltage */
#define OVER_VOLTAGE_HYSTERESIS_VOLTS               (1.0)

/*! @brief Hysteresis for under voltage in perunit */
#define UNDER_VOLTAGE_HYSTERESIS_PU                 (UNDER_VOLTAGE_HYSTERESIS_VOLTS/DC_VOLTAGE_BASE) /* % of DC Bus Voltage */

/*! @brief Hysteresis for over voltage in perunit */
#define OVER_VOLTAGE_HYSTERESIS_PU                  (OVER_VOLTAGE_HYSTERESIS_VOLTS/DC_VOLTAGE_BASE) /* % of DC Bus Voltage */

/*! @brief Discontinous SVM minimum dutycycle in PWM timer counts */
#define DISCONTINUOUS_MIN_DUTY_COUNTS               10 

/* System setup */

/*! @brief Frequency of system interrupt used for running the low priority tasks in kHz  */
#define SYSTEM_EXECUTION_FREQ_KHZ                   1

/*! @brief Calculated system timer execution rate */
#define SYSTEM_EXECUTION_RATE               1.0f/(SYSTEM_EXECUTION_FREQ_KHZ*1000.0f)

/*! @brief CPU clock frequency in MHz */
#define CPU_FREQUENCY_MHZ                           80UL

/*! @brief System timer frequency in MHz */
#define SYSTEM_TIMER_FREQUENCY_MHZ                  40UL

/* IPD Parameters */
/*! @brief PWM frequency set when IPD in state */
#define IPD_EXECUTION_FREQUENCY_KHZ                 50UL
/*! @brief PWM timer period when IPD in state */
#define IPD_PWM_PERIOD                              (CPU_FREQUENCY_MHZ \
                                                     * 1000UL) /               \
                                                     IPD_EXECUTION_FREQUENCY_KHZ

/* Bit position for faults in fault status register for external communication and control */

/* Controller fault reporting */
/*! @brief IPD clock time out fault index */
#define IPD_CLOCK_TIMEOUT_FAULT_INDEX   0x10000000

/*! @brief IPD decay time fault index */
#define IPD_DECAY_TIME_FAULT_INDEX      0x08000000

/*! @brief BUS current limit fault index */
#define BUS_CURRENT_LIMIT_INDEX     0x04000000

/*! @brief Abnormal speed fault index */
#define ABN_SPEED_FAULT_INDEX       0x00800000

/*! @brief Abnormal backemf fault index */
#define ABN_BEMF_FAULT_INDEX        0x00400000

/*! @brief No motor fault index */
#define NO_MOTOR_FAULT_INDEX        0x00200000

/*! @brief Motor lock fault index */
#define MTR_LOCK_FAULT_INDEX        0x00F00000

/*! @brief Motor lock fault status */
#define MTR_LOCK_FAULT_STATUS       0x00100000

/*! @brief Hardware lock Ilimit fault index */
#define HW_LOCK_ILIMIT_FAULT_INDEX  0x00040000

/*! @brief Under voltage fault index */
#define UNDER_VOLTAGE_FAULT_INDEX   0x00020000

/*! @brief Over voltage fault index */
#define OVER_VOLTAGE_FAULT_INDEX    0x00010000



/* shunt configurations applied below */

#ifdef __CURRENT_THREE_SHUNT_DYNAMIC
    /*! @brief Define current shunt config */
    #define CURRENT_SHUNT_CONFIG                        CURRENT_THREE_SHUNT_DYNAMIC
#endif

#ifdef __CURRENT_THREE_SHUNT_AB_C
    /*! @brief Define current shunt config */
    #define CURRENT_SHUNT_CONFIG                        CURRENT_THREE_SHUNT_AB_C
#endif

#ifdef __CURRENT_THREE_SHUNT_A_BC
    /*! @brief Define current shunt config */
    #define CURRENT_SHUNT_CONFIG                        CURRENT_THREE_SHUNT_A_BC
#endif

#ifdef __CURRENT_TWO_SHUNT_A_B
    /*! @brief Define current shunt config */
    #define CURRENT_SHUNT_CONFIG                        CURRENT_TWO_SHUNT_A_B
#endif

#ifdef __CURRENT_TWO_SHUNT_A_C
    /*! @brief Define current shunt config */
    #define CURRENT_SHUNT_CONFIG                        CURRENT_TWO_SHUNT_A_C
#endif

#ifdef __CURRENT_TWO_SHUNT_B_C
    /*! @brief Define current shunt config */
    #define CURRENT_SHUNT_CONFIG                        CURRENT_TWO_SHUNT_B_C
#endif

#ifdef __CURRENT_SINGLE_SHUNT
    /*! @brief Define current shunt config */
    #define CURRENT_SHUNT_CONFIG                        CURRENT_SINGLE_SHUNT
#endif

#ifdef __cplusplus
}
#endif
#endif /* MAIN_H */
