/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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
#include "ti_msp_dl_config.h"

#ifndef COMP_HAL_H_
#define COMP_HAL_H_

//COMP_DACCODE0 set by Sysconfig                                    (39)
#define COMP_DACCODE1                                               (77)
#define COMP_DACCODE2                                               (116)

//IN0+ already defined w/ SysConfig

//IN0-
#define GPIO_COMP_IN0N_PORT                                         (GPIOB)
#define GPIO_COMP_IN0N_PIN                                          (DL_GPIO_PIN_22)
#define GPIO_COMP_IOMUX_IN0N                                        (IOMUX_PINCM50)
#define GPIO_COMP_IOMUX_IN0N_FUNC                                   (IOMUX_PINCM50_PF_UNCONNECTED)

//IN1-
#define GPIO_COMP_IN1N_PORT                                         (GPIOA)
#define GPIO_COMP_IN1N_PIN                                          (DL_GPIO_PIN_17)
#define GPIO_COMP_IOMUX_IN1N                                        (IOMUX_PINCM39)
#define GPIO_COMP_IOMUX_IN1N_FUNC                                   (IOMUX_PINCM39_PF_UNCONNECTED)

//function definitions
void update_comp_configA (void);
void update_comp_configB (void);
void update_comp_configC (void);

#endif /* COMP_HAL_H_ */
