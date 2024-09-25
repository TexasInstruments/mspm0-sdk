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
 *  @file       focPeriphInit.h
 *  @brief      HAL interface for FOC Module
 *
 * 
 *  @anchor focPeriphInit_h
 *  # Overview
 *
 *  HAL Apis for peripheral initializations
 *
 *  <hr>
 ******************************************************************************/

#ifndef FOC_RUN_CONFIG_H
#define FOC_RUN_CONFIG_H

#include "stdint.h"

/* Including all the sysconfig generated defines */
#include "ti_msp_dl_config.h"

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif

/*! @brief GPIO port for direction pin */
#define FOC_GPIO_DIR_PORT           (FOC_GPIO_IN_PORT)

/*! @brief GPIO pin number for direction pin */
#define FOC_GPIO_DIR_PIN            (FOC_GPIO_IN_DIR_PIN)

/*! @brief GPIO port for brake pin */
#define FOC_GPIO_BRAKE_PORT         (FOC_GPIO_IN_PORT)

/*! @brief GPIO pin number for brake pin */
#define FOC_GPIO_BRAKE_PIN          (FOC_GPIO_IN_BRAKE_PIN)

/*! @brief GPIO port for nfault pin */
#define FOC_GPIO_NFAULT_PORT         (FOC_GPIO_OUT_PORT)

/*! @brief GPIO pin number for nfault pin */
#define FOC_GPIO_NFAULT_PIN          (FOC_GPIO_OUT_NFAULT_PIN)

/*! @brief Timer instance used for IPD */
#define FOC_CAPTURE_IPD_INST            (CAPTURE_INST)
/*! @brief Capture index used for IPD */
#define FOC_CAPTURE_IPD_IDX             (DL_TIMER_CC_0_INDEX)
/*! @brief Load value for IPD timer */
#define FOC_CAPTURE_IPD_LOAD_VALUE      (CAPTURE_INST_LOAD_VALUE)
/*! @brief IRQ number for IPD timer */
#define FOC_CAPTURE_IRQN                (CAPTURE_INST_INT_IRQN)

/*! @brief Comparator instance for U phase current */
#define FOC_COMP_U_INST             (COMP_U_INST)
/*! @brief Comparator publisher channel for U phase current */
#define FOC_COMP_U_PUB_CH           (COMP_U_INST_PUB_CH)
/*! @brief Comparator instance for V phase current */
#define FOC_COMP_V_INST             (COMP_V_INST)
/*! @brief Comparator publisher channel for V phase current */
#define FOC_COMP_V_PUB_CH           (COMP_V_INST_PUB_CH)
/*! @brief Comparator instance for W phase current */
#define FOC_COMP_W_INST             (COMP_W_INST)
/*! @brief Comparator publisher channel for W phase current */
#define FOC_COMP_W_PUB_CH           (COMP_W_INST_PUB_CH)

/*! @brief PWM instance */
#define FOC_PWMA0_INST            (PWMA0_INST)

/*! @brief PWM U phase index */
#define FOC_PWMA0_U_IDX           (GPIO_PWMA0_C2_IDX)
/*! @brief PWM V phase index */
#define FOC_PWMA0_V_IDX           (GPIO_PWMA0_C3_IDX)
/*! @brief PWM W phase index */
#define FOC_PWMA0_W_IDX           (GPIO_PWMA0_C1_IDX)
/*! @brief PWM Index used for ADC trigger */
#define FOC_PWMA0_ADC_TRIG_IDX    (GPIO_PWMA0_C0_IDX)

/*! @brief Define for ADC trigger up event */
#define FOC_PWMA0_ADC_TRIG_UP_EVENT     (DL_TIMERA_EVENT_CC0_UP_EVENT)
/*! @brief Define for ADC trigger down event */
#define FOC_PWMA0_ADC_TRIG_DN_EVENT     (DL_TIMERA_EVENT_CC0_DN_EVENT)
/*! @brief Define for ADC trigger load event */
#define FOC_PWMA0_ADC_TRIG_LOAD_EVENT   (DL_TIMER_EVENT_LOAD_EVENT)

/*! @brief Timer instance used as system timer */
#define FOC_TIMER_INST            (FOC_TIMG0_INST)

/*! @brief Event channel used by PWM to trigger ADCs */
#define ADC_PWM_TRIG_CH         (12)

/**
 * @brief     FOC peripheral initialization
 */
void focPeriphInit(void);

#ifdef __cplusplus
}
#endif
#endif /* FOC_RUN_CONFIG_H */
