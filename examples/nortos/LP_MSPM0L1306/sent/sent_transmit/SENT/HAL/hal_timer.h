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
 *  @file       hal_timer.h
 *  @brief      SENT HAL TIMER header file
 ******************************************************************************/

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>

/* clang-format off */
/* Defines for PWM_0 */
#define PWM_0_INST                                                         TIMG1
#define PWM_0_INST_IRQHandler                                   TIMG1_IRQHandler
#define PWM_0_INST_INT_IRQN                                     (TIMG1_INT_IRQn)
#define PWM_0_INST_CLK_FREQ                                              4000000

#define PWM_0_CHAN_SEL                                     (DL_TIMER_CC_1_INDEX)
#define PWM_0_CHAN_OUT                                     (DL_TIMER_CC1_OUTPUT)
/* clang-format on */

/* Initializes timer to run at 4 MHz*/
void HAL_Timer_init(uint16_t load, uint16_t capture);
void HAL_Timer_start(void);
void HAL_Timer_updateLoadVal(uint16_t loadVal);
