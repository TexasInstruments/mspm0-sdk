/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
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
 *  ============ ti_drivers_config.h =============
 *  Configured MSPM0 Driver module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */

/* clang-format off */

#define CONFIG_GPIO_LED_0               (0)
#define CONFIG_GPIO_LED_0_IOMUX         (IOMUX_PINCM1)

#define CONFIG_GPIO_LED_1               (54)
#define CONFIG_GPIO_LED_1_IOMUX         (IOMUX_PINCM50)

#define CONFIG_GPIO_BUTTON_0            (53)
#define CONFIG_GPIO_BUTTON_0_IOMUX      (IOMUX_PINCM49) /* PB21 - Button S2 */
#define CONFIG_GPIO_BUTTON_1            (18)
#define CONFIG_GPIO_BUTTON_1_IOMUX      (IOMUX_PINCM40) /* PA18 - Button S1 */
/* Uncomment these lines if there's only 1 switch on the device */
//#define CONFIG_GPIO_BUTTON_1            (53)
//#define CONFIG_GPIO_BUTTON_1_IOMUX      (IOMUX_PINCM49)

#define CONFIG_GPIO_LED_ON              (1)
#define CONFIG_GPIO_LED_OFF             (0)

/* clang-format on */
