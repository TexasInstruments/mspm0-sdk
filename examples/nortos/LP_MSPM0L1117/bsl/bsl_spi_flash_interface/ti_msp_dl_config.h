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

#define POWER_STARTUP_DELAY                                                (16)

/* GPIO configuration for SPI */
#define BSL_SPI (SPI0)
#define BSL_SPI_INTERRUPT_NUM (SPI0_INT_IRQn)

#define BSL_SPI_GPIO_IOMUX_SCLK (IOMUX_PINCM16)
#define BSL_SPI_SCLK_FUNCTION (IOMUX_PINCM16_PF_SPI0_SCLK)

#define BSL_SPI_GPIO_IOMUX_PICO (IOMUX_PINCM15)
#define BSL_SPI_PICO_FUNCTION (IOMUX_PINCM15_PF_SPI0_PICO)

#define BSL_SPI_GPIO_IOMUX_POCI (IOMUX_PINCM14)
#define BSL_SPI_POCI_FUNCTION (IOMUX_PINCM14_PF_SPI0_POCI)

#define BSL_SPI_GPIO_IOMUX_CS (IOMUX_PINCM19)
#define BSL_SPI_CS_FUNCTION (IOMUX_PINCM19_PF_SPI0_CS0)

#define GPIO_SPI_0_PICO_PORT                                              GPIOB
#define GPIO_SPI_0_PICO_PIN                                       DL_GPIO_PIN_2

#define GPIO_SPI_0_POCI_PORT                                              GPIOA
#define GPIO_SPI_0_POCI_PIN                                       DL_GPIO_PIN_7

#define GPIO_SPI_0_SCLK_PORT                                              GPIOB
#define GPIO_SPI_0_SCLK_PIN                                       DL_GPIO_PIN_3
#define GPIO_SPI_0_CS_PORT                                                GPIOA
#define GPIO_SPI_0_CS_PIN                                         DL_GPIO_PIN_8

#define BSL_SPI_CLOCK_SOURCE (DL_SPI_CLOCK_BUSCLK)

#define BSL_CRC (CRCP0)
#define BSL_CRCP_SEED ((uint32_t) 0xFFFFFFFFU)




#endif /* ti_msp_dl_config_h */
