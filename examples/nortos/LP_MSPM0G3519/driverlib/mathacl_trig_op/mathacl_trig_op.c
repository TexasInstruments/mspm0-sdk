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
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, plaintext, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"

const DL_MathACL_operationConfig gSinOpConfig = {
    .opType      = DL_MATHACL_OP_TYPE_SINCOS,
    .opSign      = DL_MATHACL_OPSIGN_UNSIGNED,
    .iterations  = 10,
    .scaleFactor = 0,
    .qType       = DL_MATHACL_Q_TYPE_Q31};

int main(void)
{
    volatile uint32_t op1Cos __attribute__((unused));
    volatile uint32_t op1Sin __attribute__((unused));
    volatile uint32_t op2Cos __attribute__((unused));
    volatile uint32_t op2Sin __attribute__((unused));
    volatile uint32_t op3Cos __attribute__((unused));
    volatile uint32_t op3Sin __attribute__((unused));

    /*
     *                            Operation 3
     *                               0, 1
     *                                |
     *                                |      ^ Operation 2 -- .707, .707
     *                                |
     *                                |
     *                      ----------|---------- 1, 0     Operation 1
     */

    SYSCFG_DL_init();

    /* Original setup operation, plus first operation with OP1 = 0 degrees */
    /* Cos(0) = 1, Sin(0) = 0 */
    DL_MathACL_startSinCosOperation(MATHACL, &gSinOpConfig, 0);
    DL_MathACL_waitForOperation(MATHACL);
    op1Cos = DL_MathACL_getResultOne(MATHACL);
    op1Sin = DL_MathACL_getResultTwo(MATHACL);

    /* Re-use existing configuration with new OP OP1 = 45 degrees */
    /* Cos(45) = ~0.707, Sin(45) = ~0.707 */
    DL_MathACL_startSinCosOperation(MATHACL, &gSinOpConfig, 0x20000000);
    DL_MathACL_waitForOperation(MATHACL);
    op2Cos = DL_MathACL_getResultOne(MATHACL);
    op2Sin = DL_MathACL_getResultTwo(MATHACL);

    /* Re-use existing configuration with new OP OP3 = 90 degrees */
    /* Cos(90) = 0, Sin(95) = 1 */
    DL_MathACL_startSinCosOperation(MATHACL, &gSinOpConfig, 0x40000000);
    DL_MathACL_waitForOperation(MATHACL);
    op3Cos = DL_MathACL_getResultOne(MATHACL);
    op3Sin = DL_MathACL_getResultTwo(MATHACL);

    /*
     * Stop at the break point to examine and verify the outputs:
     *   op1Cos = should be 1 (0x7FFFFFFF)
     *   op1Sin = 0
     *   op2Cos = 0.707 (0x5A89E374)
     *   op2Sin = 0.707 (0x5A7B0D41)
     *   op3Cos = 0
     *   op3Sin = should be 1 (0x7FFFFFFF)
     */

    DL_GPIO_clearPins(
        GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);

    __BKPT(0);

    while (1) {
        __WFI();
    }
}
