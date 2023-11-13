/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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

/* Define the number of Rows - this is used to build memory table array */
#define NUMBER_OF_ROWS 3

/* Variable to define Mask value for DL_GPIO_writePinsVal function - bit wise OR of column pin values */
#define COL_MASK 0x38

/* Variable to define display period duration */
#define LED_DISPLAY_PERIOD 100 /* timer period = 5ms, so display period = 500ms */

/* Variable that tracks the current LED state of a single display period */
volatile uint32_t gLedState = 0;

/* Memory table array written to in order to update LEDs */
uint32_t gLedMemoryTable[NUMBER_OF_ROWS];

/* Function to update LED_MemoryTable */
void LED_updateTable(uint8_t rowNumber, uint8_t LEDs);

/* Enumeration for state machine to cycle through rows */
typedef enum {
    ROW_1,
    ROW_2,
    ROW_3
}rowNumber;

rowNumber rowState = ROW_1;

int main(void)
{
    /* Power on GPIO, initialize pins as digital outputs */
    SYSCFG_DL_init();


    /* Enable Timer interrupt to start LED cycle */
    NVIC_EnableIRQ(LED_STATE_INST_INT_IRQN);

    while(1){
        __WFI();
        /* Flash TI on repeat in half second increments */
        if (gLedState == LED_DISPLAY_PERIOD){ /* Display "T" for one display period */
            LED_updateTable(1, 0b111);
            LED_updateTable(2, 0b010);
            LED_updateTable(3, 0b010);
        } else if (gLedState == LED_DISPLAY_PERIOD*2){ /* Blank for one display period */
            LED_updateTable(1, 0b000);
            LED_updateTable(2, 0b000);
            LED_updateTable(3, 0b000);
        } else if (gLedState == LED_DISPLAY_PERIOD*3){ /* Display "I" for one display period */
            LED_updateTable(1, 0b111);
            LED_updateTable(2, 0b010);
            LED_updateTable(3, 0b111);
        } else if (gLedState == LED_DISPLAY_PERIOD*4){ /* Blank for one display period */
            LED_updateTable(1, 0b000);
            LED_updateTable(2, 0b000);
            LED_updateTable(3, 0b000);
        } else if (gLedState > LED_DISPLAY_PERIOD*4){ /* Reset gLedState and start over */
            gLedState = 0;
        }
    }
}

void LED_STATE_INST_IRQHandler(void) {
    switch (DL_TimerG_getPendingInterrupt(LED_STATE_INST)){
        case DL_TIMER_IIDX_ZERO:
            /* State machine to auto cycle from row 1 to row N and repeat */
            switch (rowState){
                case ROW_1:
                    /* Turn on ROW_1, Turn off ROW_3 */
                    DL_GPIO_clearPins(ROW_PORT, ROW_ROW_1_PIN);
                    DL_GPIO_setPins(ROW_PORT, ROW_ROW_3_PIN);

                    /* Set COLUMN values */
                    DL_GPIO_writePinsVal(COLUMN_PORT, COL_MASK, gLedMemoryTable[0]);
                    rowState = ROW_2;
                    break;
                case ROW_2:
                    /* Turn on ROW_2, Turn off ROW_1 */
                    DL_GPIO_clearPins(ROW_PORT, ROW_ROW_2_PIN);
                    DL_GPIO_setPins(ROW_PORT, ROW_ROW_1_PIN);

                    /* Set COLUMN values */
                    DL_GPIO_writePinsVal(COLUMN_PORT, COL_MASK, gLedMemoryTable[1]);
                    rowState = ROW_3;
                    break;
                case ROW_3:
                    /* Turn on ROW_3, Turn off ROW_2 */
                    DL_GPIO_clearPins(ROW_PORT, ROW_ROW_3_PIN);
                    DL_GPIO_setPins(ROW_PORT, ROW_ROW_2_PIN);

                    /* Set COLUMN values */
                    DL_GPIO_writePinsVal(COLUMN_PORT, COL_MASK, gLedMemoryTable[2]);
                    rowState = ROW_1;
                    break;
            }

            /* Increment LED_STATE */
            gLedState++;

            break;
        default:
            break;
    }
}

/* Write to memory table */
void LED_updateTable(uint8_t rowNumber, uint8_t LEDs){
    switch (rowNumber){
        case 1:
            gLedMemoryTable[0] = LEDs <<3;
            break;
        case 2:
            gLedMemoryTable[1] = LEDs <<3;
            break;
        case 3:
            gLedMemoryTable[2] = LEDs <<3;
            break;
        default:
            break;
    }
}
