/*
 * Copyright (c) 2021, Texas Instruments Incorporated - http://www.ti.com
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

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0L111X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

//#define CONFIG_MSPM0L111X
#define CONFIG_MSPM0G518X

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */

/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)

#define I2S_INST                                                         (I2S0)
#define I2S_INST_IRQn                                           (I2S0_INT_IRQn)
#define I2S_INST_IRQHandler                                   (I2S0_IRQHandler)

/* Use PB10 on LP */
#define GPIO_I2S_IOMUX_WCLK                                     (IOMUX_PINCM27)
#define GPIO_I2S_IOMUX_WCLK_FUNC                   (IOMUX_PINCM27_PF_I2S0_WCLK)
/* Use PA9 on LP */
#define GPIO_I2S_IOMUX_BCLK                                     (IOMUX_PINCM20)
#define GPIO_I2S_IOMUX_BCLK_FUNC                   (IOMUX_PINCM20_PF_I2S0_BCLK)
/* Use PA13 on LP */
#define GPIO_I2S_IOMUX_AD0                                      (IOMUX_PINCM35)
#define GPIO_I2S_IOMUX_AD0_FUNC                     (IOMUX_PINCM35_PF_I2S0_AD0)
/* Use PB13 on LP */
#define GPIO_I2S_IOMUX_AD1                                      (IOMUX_PINCM30)
#define GPIO_I2S_IOMUX_AD1_FUNC                     (IOMUX_PINCM30_PF_I2S0_AD1)

/* Defines for DMA_CH0 */
#define DMA_CH0_CHAN_ID                                                     (0)
#define I2S_INST_DMA_TRIGGER_0                               (DMA_I2S0_TX_TRIG)

__STATIC_INLINE void DL_GPIO_inena_Function(
    uint32_t pincmIndex, uint32_t inena_function)
{
    IOMUX->SECCFG.PINCM[pincmIndex] |= (inena_function|IOMUX_PINCM_PIPD_ENABLE);
}

/* clang-format on */

void SYSCFG_DL_init();
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_DMA_init(void);
void SYSCFG_DL_I2S_init(void);

#endif /* ti_msp_dl_config_h */
