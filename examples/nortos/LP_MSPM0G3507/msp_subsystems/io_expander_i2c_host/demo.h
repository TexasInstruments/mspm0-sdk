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
#ifndef DEMO_DEMO_H_
#define DEMO_DEMO_H_

#include "i2c_controller.h"

/*======================== DEFINES/MACROS ===================================*/
#define MSPM0L1306_TARGET       (0)
#define MSPM0G3507_TARGET       (1)

/* User - Select the target MSPM0 IO expander configuration. */
#define TARGET  (MSPM0G3507_TARGET)

#if TARGET==MSPM0L1306_TARGET

/*===========================================================================*/
/*               DEMO HEADER FOR MSPM0L1306 TARGET                           */
/*===========================================================================*/

/* These defines match the target's SYSCONFIG generated pin assignments */
#define LED_RGB_BLUE                            (DL_GPIO_PIN_27)    // 0x08000000
#define LED_RGB_RED                             (DL_GPIO_PIN_26)    // 0x04000000
#define IRQ_OUT                                 (DL_GPIO_PIN_23)    // 0x00800000
#define SW_S1                                   (DL_GPIO_PIN_18)    // 0x00004000
#define SW_S2                                   (DL_GPIO_PIN_14)    // 0x00000400
#define LED_RGB_GREEN                           (DL_GPIO_PIN_13)    // 0x00002000
#define USER_OUT                                (DL_GPIO_PIN_12)    // 0x00001000
#define USER_IN                                 (DL_GPIO_PIN_7)     // 0x00000080


/* Define Number of expander IO ports on TARGET device */
#define IOX_NUM_OF_PORTS                        (1)

#elif TARGET==MSPM0G3507_TARGET
/*===========================================================================*/
/*               DEMO HEADER FOR MSPM0G3507 TARGET                           */
/*===========================================================================*/

/* These defines match the target's SYSCONFIG generated pin assignments */
#define LED_RGB_GREEN                           (DL_GPIO_PIN_27)    // 0x08000000
#define LED_RGB_RED                             (DL_GPIO_PIN_26)    // 0x04000000
#define LED_RGB_BLUE                            (DL_GPIO_PIN_22)    // 0x00400000
#define SW_S3                                   (DL_GPIO_PIN_21)    // 0x00200000
#define USER_OUT                                (DL_GPIO_PIN_4)     // 0x00000010
#define IRQ_OUT                                 (DL_GPIO_PIN_0)     // 0x00000001
#define SW_S2                                   (DL_GPIO_PIN_18)    // 0x00004000
#define USER_IN                                 (DL_GPIO_PIN_7)     // 0x00002000


/* Number of expander IO ports on TARGET device */
#define IOX_NUM_OF_PORTS                        (2)

#endif

/*============================ EXTERNS =====================================*/
extern void demoIOx(void);

#endif /* DEMO_DEMO_H_ */
