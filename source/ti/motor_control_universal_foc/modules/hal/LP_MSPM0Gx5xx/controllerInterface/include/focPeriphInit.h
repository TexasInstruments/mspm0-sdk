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

#ifdef FOC_GPIO_IN_PORT
/*! @brief GPIO port for direction pin */
#define FOC_GPIO_DIR_PORT           (FOC_GPIO_IN_PORT)
#else
/*! @brief GPIO port for direction pin */
#define FOC_GPIO_DIR_PORT           (FOC_GPIO_IN_DIR_PORT)
#endif

/*! @brief GPIO pin number for direction pin */
#define FOC_GPIO_DIR_PIN            (FOC_GPIO_IN_DIR_PIN)

#ifdef FOC_GPIO_IN_PORT
 /*! @brief GPIO port for brake pin */
 #define FOC_GPIO_BRAKE_PORT         (FOC_GPIO_IN_PORT)
#else
 /*! @brief GPIO port for brake pin */
 #define FOC_GPIO_BRAKE_PORT         (FOC_GPIO_IN_BRAKE_PORT)
#endif

/*! @brief GPIO pin number for brake pin */
#define FOC_GPIO_BRAKE_PIN          (FOC_GPIO_IN_BRAKE_PIN)

#ifdef FOC_GPIO_OUT_PORT
 /*! @brief GPIO port for nfault pin */
 #define FOC_GPIO_NFAULT_PORT         (FOC_GPIO_OUT_PORT)
#else
 /*! @brief GPIO port for nfault pin */
 #define FOC_GPIO_NFAULT_PORT         (FOC_GPIO_OUT_NFAULT_PORT)
#endif

/*! @brief GPIO pin number for nfault pin */
#define FOC_GPIO_NFAULT_PIN          (FOC_GPIO_OUT_NFAULT_PIN)

/*! @brief PWM instance */
#define FOC_PWMA0_INST            (PWMA0_INST)

 /*! @brief TIM instance */
 #define FOC_TIMA1_INST            (TIMA1_INST)

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
