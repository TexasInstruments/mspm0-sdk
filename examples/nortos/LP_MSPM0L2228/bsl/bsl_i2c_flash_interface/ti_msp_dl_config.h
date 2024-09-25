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
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

/* GPIO configuration - I2C */
#define BSL_GPIO_I2C_SDA_PIN (0U)
#define BSL_GPIO_I2C_SCL_PIN (1U)
#define BSL_GPIO_I2C_SDA (3U)
#define BSL_GPIO_I2C_SCL (3U)

/* Definitions for I2C */
#define BSL_I2C (I2C0)
#define BSL_I2C_INTERRUPT_NUM (I2C0_INT_IRQn)
#define BSL_I2C_CLOCK_SOURCE (DL_I2C_CLOCK_BUSCLK)
#define BSL_I2C_TARGET_ADDRESS ((uint32_t) 0x00000048)

/* Definitions for CRC */
#define BSL_CRC (CRCP0)
#define BSL_CRCP_SEED ((uint32_t) 0xFFFFFFFFU)

#endif /* ti_msp_dl_config_h */
