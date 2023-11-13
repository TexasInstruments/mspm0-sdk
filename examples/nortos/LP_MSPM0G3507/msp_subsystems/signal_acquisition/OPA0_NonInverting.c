/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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

#define OPA_0_INST                                                          OPA0
#define GPIO_OPA_0_IN1POS_PORT                                             GPIOA
#define GPIO_OPA_0_IN1POS_PIN                                     DL_GPIO_PIN_25
#define GPIO_OPA_0_IOMUX_IN1POS                                  (IOMUX_PINCM55)
#define GPIO_OPA_0_IOMUX_IN1POS_FUNC                IOMUX_PINCM55_PF_UNCONNECTED
#define GPIO_OPA_0_OUT_PORT                                                GPIOA
#define GPIO_OPA_0_OUT_PIN                                        DL_GPIO_PIN_22
#define GPIO_OPA_0_IOMUX_OUT                                     (IOMUX_PINCM47)
#define GPIO_OPA_0_IOMUX_OUT_FUNC                   IOMUX_PINCM47_PF_UNCONNECTED

/* Gain is setup = 2 */
static const DL_OPA_Config gOPA_0Config0 = {
    .pselChannel    = DL_OPA_PSEL_IN1_POS,
    .nselChannel    = DL_OPA_NSEL_RTAP,
    .mselChannel    = DL_OPA_MSEL_GND,
    .gain           = DL_OPA_GAIN_N1_P2,
    .outputPinState = DL_OPA_OUTPUT_PIN_ENABLED,
    .choppingMode   = DL_OPA_CHOPPING_MODE_STANDARD,
};


void OPA0_NonInverting()
{
    DL_OPA_disablePower(OPA_0_INST);
    DL_OPA_reset(OPA_0_INST);
    DL_OPA_enablePower(OPA_0_INST);

    DL_OPA_init(OPA_0_INST, (DL_OPA_Config *) &gOPA_0Config0);
    DL_OPA_enableRailToRailInput(OPA_0_INST);
    DL_OPA_setGainBandwidth(OPA_0_INST, DL_OPA_GBW_HIGH);
    DL_OPA_enable(OPA_0_INST);
}
