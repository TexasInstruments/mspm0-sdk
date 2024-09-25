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

/* OP1 = 0.5, 0.5 x 2^31 = 1,073,741,824 = 0x4000 0000 */
#define OP1_Q31 (0x40000000)
/* OP2 = 0.3, 0.3 x 2^31 = 644,245,094 = 0x2666 6666 */
#define OP2_Q31 (0x26666666)
/* OP3 = 15, 15 x 2^18 = 3,932,160 = 0x3C 0000 */
#define OP3_Q18 (0x003C0000)
/* OP4 = 4, 4 x 2^18 = 1,048,576 = 0x10 0000 */
#define OP4_Q18 (0x00100000)

const DL_MathACL_operationConfig gMpyConfig = {
    .opType      = DL_MATHACL_OP_TYPE_MPY_32,
    .opSign      = DL_MATHACL_OPSIGN_UNSIGNED,
    .iterations  = 1,
    .scaleFactor = 0,
    .qType       = DL_MATHACL_Q_TYPE_Q31};

const DL_MathACL_operationConfig gDivConfig = {
    .opType      = DL_MATHACL_OP_TYPE_DIV,
    .opSign      = DL_MATHACL_OPSIGN_UNSIGNED,
    .iterations  = 0,
    .scaleFactor = 1,
    .qType       = DL_MATHACL_Q_TYPE_Q18};


int main(void)
{
    volatile uint32_t mpyRes __attribute__((unused));
    volatile uint32_t divResultQuotient __attribute__((unused));
    volatile uint32_t divResultRemainder __attribute__((unused));

    SYSCFG_DL_init();

    /* Setup unsigned multiply operation, OP1 = 0.5, OP2 = 0.3 */
    DL_MathACL_startMpyOperation(MATHACL, &gMpyConfig, OP1_Q31, OP2_Q31);
    DL_MathACL_waitForOperation(MATHACL);

    /* Get result -- floating point Q31 equivalent of 0.15 = 0x1333 3333 */
    mpyRes = DL_MathACL_getResultOne(MATHACL);

    /* Divide result OP3 = 15 by OP4 = 4 */
    DL_MathACL_startDivOperation(MATHACL, &gDivConfig, OP3_Q18, OP4_Q18);
    DL_MathACL_waitForOperation(MATHACL);
    /* Get result -- floating point Q31 equivalent of 3.75 = 0xF 0000, remainder = 0 */
    divResultQuotient  = DL_MathACL_getResultOne(MATHACL);
    divResultRemainder = DL_MathACL_getResultTwo(MATHACL);

    /*
     * Stop at the break point to examine and verify the outputs:
     *   mpyRes = 0.15 (0x1333 3333)
     *   divResultQuotient = 3.75 (0x000F 0000)
     *   divResultRemainder = 0
     */

    DL_GPIO_clearPins(
        GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_TEST_PIN);
    __BKPT(0);

    while (1) {
        __WFI();
    }
}
