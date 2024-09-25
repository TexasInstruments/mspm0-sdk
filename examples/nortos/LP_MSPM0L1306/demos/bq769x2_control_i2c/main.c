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

#include "BQ769x2_protocol.h"

char enableReadFlag = 0;

unsigned int debug[20] = {0};

unsigned char I2Ctxbuff1[8] = {0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char I2Ctxbuff[8]  = {0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char I2Ctxbuff2[8] = {0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char I2Crxbuff[8]  = {0x00};
unsigned char I2Crxbuff1[32] = {0x00};

int main(void)
{
    /* Perform any initialization needed before using any board APIs */
    SYSCFG_DL_init();

    /* Set LED to indicate start of transfer */
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

    NVIC_EnableIRQ(I2C_0_INST_INT_IRQN);
    NVIC_EnableIRQ(GPIO_GRP_0_INT_IRQN);
    DL_SYSCTL_disableSleepOnExit();

    BQ769x2_Init();  // Configure all of the BQ769x2 register settings in simulation mode.
    delayUS(10000);

    while (1) {
        if (enableReadFlag == 1) {
            BQ769x2_ReadAlarmStatus();  // write 0x62, read 2 bytes.
            delayUS(10000);
            BQ769x2_ReadSafetyStatus();  // write 0x03, read 2 bytes. write 0x05, read 2 bytes. write 0x07, read 2 bytes.
            delayUS(10000);
            BQ769x2_ReadPFStatus();  // write 0x0B, read 2 bytes. write 0x0D, read 2 bytes. write 0x0F, read 2 bytes.
            delayUS(10000);
            BQ769x2_ReadAllVoltages();  // write 0x14, read 2 bytes. From 0x14 - 0x38
            delayUS(10000);
            BQ769x2_ReadCurrent();  // write 0x3A, read 2 bytes.
            delayUS(10000);
            BQ769x2_ReadAllTemperatures();  // write 0x70, read 2 bytes. write 0x72, read 2 bytes. write 0x74, read 2 bytes.
            delayUS(10000);
            BQ769x2_ReadPassQ();  // write 0x3E with 0x0076, read 0x40 with 32 bytes.
            delayUS(10000);
            enableReadFlag = 0;
        }
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        delay_cycles(16000000);
    }
}

void GROUP1_IRQHandler(void)
{
    /*
     * This IRQ is triggered when S2 is pressed on the LaunchPad. On every
     * switch press, the demo runs the BQ769x2 status read functions.
     */
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_GRP_0_INT_IIDX:
            switch (DL_GPIO_getPendingInterrupt(GPIO_GRP_0_PORT)) {
                case GPIO_GRP_0_PIN_0_IIDX:
                    enableReadFlag = 1;

                default:
                    break;
            }

            break;
        default:
            break;
    }
}
