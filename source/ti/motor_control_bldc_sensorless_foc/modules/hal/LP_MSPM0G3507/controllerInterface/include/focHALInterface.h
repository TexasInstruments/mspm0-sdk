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
 *  @file       focHALInterface.h
 *  @brief      HAL interface for FOC Module
 *
 * 
 *  @anchor focHALInterface_h
 *  # Overview
 *
 *  HAL Apis used by FOC motor control
 *
 *  <hr>
 ******************************************************************************/

#ifndef FOC_HAL_INTERFACE_H
#define FOC_HAL_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Including all the sysconfig generated defines */
#include "ti_msp_dl_config.h"

/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>

/* Include the mathacl optimized IQmath */
#include "IQmath_mathacl.h"

#include "focPeriphInit.h"

#include "stdint.h"
#include "measure.h"
#include "mcLib.h"
#include "main.h"
#include "iqNum.h"
#include "appInterface.h"

/*! @brief Minimum floating voltage for phase in per unit */
#define MIN_FLOAT_VOLTAGE   (0.1)

/*! @brief Maximum floating voltage for phase in per unit */
#define MAX_FLOAT_VOLTAGE   (0.9)

/*!
 * @brief MCU clock frequency
 */
#define HAL_MCU_CLK_SPEED_HZ                (80000000)

/*!
 * @brief Cycles for 1 us delay
 */
#define HAL_CYCLES_TO_1_US_DELAY            (HAL_MCU_CLK_SPEED_HZ/1000000)

/*!
 * @brief Gpio high state
 */
#define HAL_GPIO_HIGH            (1)

/*!
 * @brief Gpio low state
 */
#define HAL_GPIO_LOW             (0)

/*! @brief VOLTAGE_VECTOR */
typedef enum
{
    /*! IPD vector AC */
    VOLTAGE_VECTOR_AC = 0,
    /*! IPD vector CA */
    VOLTAGE_VECTOR_CA,
    /*! IPD vector BC */
    VOLTAGE_VECTOR_BC,
    /*! IPD vector CB */
    VOLTAGE_VECTOR_CB,
    /*! IPD vector BA */
    VOLTAGE_VECTOR_BA,
    /*! IPD vector AB */
    VOLTAGE_VECTOR_AB,
    /*! Max number of IPD vectors */
    VOLTAGE_VECTOR_MAX
}VOLTAGE_VECTOR;

/**
 * @brief     Read Motor inputs
 * @param[in] pMotorInputs   Pointer to motor input
 */
void HAL_ReadMotorInputs(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs);

/**
 * @brief     Set Phase current channels
 * @param[in] pMotorInputs   Pointer to motor input
 */
void HAL_SetPhaseCurrentChannels(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs);

/**
 * @brief     Set ADC channel to index
 * @param[in] adc12     Pointer to ADC instance
 * @param[in] idx       ADC index
 * @param[in] chansel   channel to set in index
 */
void HAL_setADCIdxChannel(ADC12_Regs *adc12, DL_ADC12_MEM_IDX idx, uint32_t chansel);

/**
 * @brief     Set Phase voltage channels in ADC
 * @param[in] pMotorInputs   Pointer to motor input
 */
void HAL_SetPhaseVoltageChannels(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs);

/**
 * @brief     Read Phase voltages from ADCs
 * @param[in] pMotorInputs   Pointer to motor input
 */
void HAL_ReadPhaseVoltages(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs);

/**
 * @brief     Read phase currents
 * @param[in] pMotorInputs   Pointer to motor input
 */
void HAL_ReadCurrents(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs);

/**
 * @brief     Select two shunt if duty saturates in any phases
 * @param[in] pMotorInputs   Pointer to motor input
 */
void HAL_SelectShuntMeasure(HAL_MEASURE_MOTOR_INPUTS_T *pMotorInputs);

/**
 * @brief     Enable PWMs
 */
void HAL_EnablePWM(void);

/**
 * @brief     Disable PWMs and all outputs are forced low
 */
void HAL_DisablePWM(void);

/**
 * @brief     Turns all low side PWM outputs high
 */
void HAL_EnableLowSideBrake(void);

/**
 * @brief     Set PWM dutycycles
 * @param[in] pABC      Pointer to dutycycle
 * @param[in] FullDuty   Full dutycycle
 */
void HAL_UpdateDuty(const MC_ABC_T *pABC, const int32_t FullDuty);

/**
 * @brief     Set PWM Frequency
 * @param[in] pwmPeriod    PWM period to set
 * @param[in] propagationDelay   Propagation delay
 */
void HAL_SetPWMFrequency(const int32_t pwmPeriod,
                         const int32_t propagationDelay);

/**
 * @brief     Set ADC sampling frequency
 * @param[in] samplingRatio    ADC sampling ratio
 */
void HAL_SetADCSamplingFrequency(const ADC_SAMPLING_RATIO_TYPES samplingRatio);

/**
 * @brief     Set System timer frequency
 * @param[in] systemPeriod    Timer period
 */
void HAL_SetSystemFrequency(const int32_t systemPeriod);

/**
 * @brief     Get phase floating status
 * @param[in] pVoltage    pointer to voltage structure
 * @return    Returns the state of phases floating
 * 
 * @retval     true  Phases floating
 * @retval     false Phases not floating
 */
_Bool HAL_GetPhasesFloatingStatus(const HAL_MEASURE_VOLTAGE_T *pVoltage);

/**
 * @brief     Sets Nfault pin low
 */
void HAL_ClearNFault(void);

/**
 * @brief     Sets Nfault pin high
 */
void HAL_SetNFault(void);

/**
 * @brief     Gets the latched fault status
 * @return    Returns latched Fault status
 */
uint32_t HAL_getRawHVdieFaultStatus(void);

/**
 * @brief     Gets the fault status
 * @return    Returns Fault status
 */
uint32_t HAL_getRawFaultStatus(void);

/**
 * @brief     Clears the fault status in pwm
 */
void HAL_clearFaultStatus(void);

/**
 * @brief     Starts gate driver offset calibration
 */
void HAL_setGateDriveOffsetCalib(void);

/**
 * @brief     Stops gate driver offset calibration
 */
void HAL_resetGateDriveOffsetCalib(void);

/**
 * @brief     Set PWMs to zero
 */
void HAL_pwmCountZero(void);

/**
 * @brief     Blocks for the specified delay time
 * @param[in] num_us   Delay in us
 */
void HAL_delayMicroSeconds(uint32_t num_us);

/**
 * @brief     Disables all the interrupts
 */
void HAL_DisableINT(void);

/**
 * @brief     Triggers a POR reset
 */
void HAL_resetPOR(void);

/**
 * @brief     Get the status of the Direction pin
 * @return    Returns GPIO state of Direction pin
 * 
 * @retval     true  Direction pin is high
 * @retval     false Direction pin is low
 */
_Bool HAL_getDirPinStatus(void);

/**
 * @brief     Get the status of the brake pin
 * @return    Returns GPIO state of brake pin

 * 
 * @retval     true  brake pin is high
 * @retval     false brake pin is low
 */
_Bool HAL_getBrakePinStatus(void);

/**
 * @brief     Force the PWM out to high and complement out to low
 * @param[in] CCIdx   A PWM CC index.
 */
__STATIC_INLINE void HAL_PWMForceHigh(DL_TIMER_CC_INDEX CCIdx)
{
    DL_Timer_overrideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_HIGH,
                          DL_TIMER_FORCE_CMPL_OUT_LOW, CCIdx);
}

/**
 * @brief     Force the PWM out to low and complement out to high
 * @param[in] CCIdx   A PWM CC index.
 */
__STATIC_INLINE void HAL_PWMForceLow(DL_TIMER_CC_INDEX CCIdx)
{
    DL_Timer_overrideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_LOW,
                          DL_TIMER_FORCE_CMPL_OUT_HIGH, CCIdx);
}

/**
 * @brief     Disable forcing the PWM out and complement out
 * @param[in] CCIdx   A PWM CC index.
 */
__STATIC_INLINE void HAL_PWMForceDisable(DL_TIMER_CC_INDEX CCIdx)
{
    DL_Timer_overrideCCPOut(FOC_PWMA0_INST, DL_TIMER_FORCE_OUT_DISABLED,
                          DL_TIMER_FORCE_CMPL_OUT_DISABLED, CCIdx);
}

/**
 * @brief   Enables the IPD capture timer
 */
void HAL_IPDEnableISR(void);

/**
 * @brief   Disabled the IPD capture timer
 */
void HAL_IPDDisableISR(void);

/**
 * @brief   Returns the time captured
 * @return    Returns the captured counter value
 */
uint16_t HAL_IPDGetTime(void);

/**
 * @brief     Initializes comparators
 */
void HAL_COMPInit(void);

/**
 * @brief     Set IPD Pulse configuration
 * @param[in] vector voltage vector to set
 * @param[in] CompVal Comparator threshold value
 */
void HAL_setIPDPulseConfig(VOLTAGE_VECTOR vector, uint8_t CompVal);

/**
 * @brief     Set IPD Pulse action for T1 event
 * @param[in] vector Current voltage vector
 * @param[in] CompVal Comparator threshold value
 * @param[in] fastDecayEnable Fast decay enabled
 */
void HAL_IPDT1Action(VOLTAGE_VECTOR vector, uint8_t CompVal, uint8_t fastDecayEnable);

#ifdef __cplusplus
}
#endif
#endif /* FOC_HAL_INTERFACE_H */
