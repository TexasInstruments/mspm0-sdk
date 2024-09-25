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

uint8_t flag = 0;

int main(void)
{
    SYSCFG_DL_init();

    NVIC_SetPriority(TIMER_0_INST_INT_IRQN, 1);
    NVIC_SetPriority(WWDT0_INT_IRQN, 0);

    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    NVIC_EnableIRQ(WWDT0_INT_IRQN);

    DL_TimerG_startCounter(TIMER_0_INST);

    while (1)
        ;
}

/*
 * Using __attribute__((interrupt, fully_populate_jump_tables)) in TI CLANG
 * compiler can eliminate non-deterministic control flow by using fully
 * populated jump tables to implement switch statements.
 *
 * NOTE: This attribute may negatively impact code size depending on the size
 * of the jump table
 */

#if defined(__TI_COMPILER_VERSION__)
__attribute__((interrupt, fully_populate_jump_tables))
#endif
void TIMER_0_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_0_INST)) {
        case DL_TIMERG_IIDX_ZERO:
            if (flag == 0) {
                DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
            }
            break;
        case DL_TIMERG_IIDX_LOAD:
        case DL_TIMERG_IIDX_CC0_DN:
        case DL_TIMERG_IIDX_CC1_DN:
        case DL_TIMERG_IIDX_CC0_UP:
        case DL_TIMERG_IIDX_CC1_UP:
        case DL_TIMERG_IIDX_OVERFLOW:
        default:
            break;
    }
}

#if defined(__TI_COMPILER_VERSION__)
__attribute__((interrupt, fully_populate_jump_tables))
#endif
void GROUP0_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_0)) {
        case DL_INTERRUPT_GROUP0_IIDX_WWDT0:
            switch (DL_WWDT_getPendingInterrupt(WWDT0)) {
                case DL_WWDT_IIDX_INTERVAL_TIMER:
                    switch (flag) {
                        case 0:
                            DL_GPIO_clearPins(
                                GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
                            flag = 1;
                            break;
                        case 1:
                            flag = 0;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
        case DL_INTERRUPT_GROUP0_IIDX_WWDT1:
        case DL_INTERRUPT_GROUP0_IIDX_DEBUGSS:
        case DL_INTERRUPT_GROUP0_IIDX_FLASH:
        case DL_INTERRUPT_GROUP0_IIDX_WUC_FSUB0:
        case DL_INTERRUPT_GROUP0_IIDX_WUC_FSUB1:
        case DL_INTERRUPT_GROUP0_IIDX_SYSCTL:
        default:
            break;
    }
}
