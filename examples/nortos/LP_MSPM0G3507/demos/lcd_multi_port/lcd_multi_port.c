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

#define COM_LINE_TOTAL 4       // Total number of COM lines
#define SEGMENT_LINE_TOTAL 12  // Total number of segment lines
#define SEGMENT_LINE_LOWER_PORT_MASK \
    0x0003F000  // Pin mask for segments on Port A
#define SEGMENT_LINE_UPPER_PORT_MASK \
    0x00001F80                     // Pin mask for segments on Port B
#define SEGMENT_LINE_PORT_SPLIT 6  // Segment that splits Port B from Port A

void LCDWriteCharacter(uint8_t character, uint8_t position);
void clearLCDMemoryTable(void);

// Global variable that tracks current LCD state of a single display cycle
volatile uint8_t gLCDUpdatePeriod = 0;

/*
 * Arrays that are written to for screen updates
 * Lower is for Port A
 * Upper is for Port B
 * Array is 32 bit number representing a port by number of COM Lines starting with COM0
 */
uint32_t gLCDMemoryTableLower[COM_LINE_TOTAL];
uint32_t gLCDMemoryTableUpper[COM_LINE_TOTAL];

/*
 * Array that maps different characters to LCD map
 * Mapping assumes array as described above
 */
const uint8_t gNumberMap[11] = {
    /* 0     1     2     3     4     5     6     7     8     9     &  */
    0x5F, 0x06, 0x6B, 0x2F, 0x36, 0x3D, 0x7A, 0x07, 0x7F, 0x7F, 0x80};

/*
 * Map of different segments lines across a port
 * SEGMENT_LINE_LOWER_PORT_MASK must map these segments to port
 * All Port A segments (lower) must be listed before Port B segments (upper)
 */
const uint8_t gSegmentOffsetMap[SEGMENT_LINE_TOTAL] =
    {/*---------PORTA-------/---------PORTB-------*/
        /*SEG0 --------------5-/-6--------------SEG11*/
        12, 13, 14, 15, 16, 17, 7, 8, 9, 10, 11, 12};

int main(void)
{
    /* Power on GPIO, initialize pins as digital outputs */
    SYSCFG_DL_init();
    /* Initialize COM Pins to default state */
    DL_GPIO_enableHiZ(COM_LINES_COM0_IOMUX);
    DL_GPIO_enableHiZ(COM_LINES_COM1_IOMUX);
    DL_GPIO_enableHiZ(COM_LINES_COM2_IOMUX);
    DL_GPIO_enableHiZ(COM_LINES_COM3_IOMUX);
    DL_GPIO_setPins(COM_LINES_PORT, COM_LINES_COM0_PIN);
    DL_GPIO_setPins(COM_LINES_PORT, COM_LINES_COM1_PIN);
    DL_GPIO_setPins(COM_LINES_PORT, COM_LINES_COM2_PIN);
    DL_GPIO_setPins(COM_LINES_PORT, COM_LINES_COM3_PIN);

    /* Initial character writing for display */
    LCDWriteCharacter(2, 4);
    LCDWriteCharacter(5, 3);
    LCDWriteCharacter(2, 0);
    LCDWriteCharacter(2, 1);
    LCDWriteCharacter(5, 2);
    LCDWriteCharacter(2, 6);
    LCDWriteCharacter(5, 5);

    /* Enable Timer interrupt to start LCD cycle */
    NVIC_EnableIRQ(LCD_Update_Clock_INST_INT_IRQN);

    while (1) {
        /* Insert application here*/
    }
}

void LCD_Update_Clock_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(LCD_Update_Clock_INST)) {
        case DL_TIMER_IIDX_ZERO:
            /*
        * State machine for LCD update cycle
        * States correspond to timing diagram described in application note
        */
            switch (gLCDUpdatePeriod) {
                case 0: /* State t0 */
                    /* Update COM pin */
                    DL_GPIO_enableHiZ(COM_LINES_COM3_IOMUX);
                    DL_GPIO_setPins(COM_LINES_PORT, COM_LINES_COM3_PIN);
                    DL_GPIO_clearPins(COM_LINES_PORT, COM_LINES_COM0_PIN);
                    DL_GPIO_disableHiZ(COM_LINES_COM0_IOMUX);

                    /* Update Segment pins */
                    DL_GPIO_writePinsVal(SEGMENT_Lines_Lower_PORT,
                        SEGMENT_LINE_LOWER_PORT_MASK, gLCDMemoryTableLower[0]);
                    DL_GPIO_writePinsVal(SEGMENT_Lines_Upper_PORT,
                        SEGMENT_LINE_UPPER_PORT_MASK, gLCDMemoryTableUpper[0]);
                    break;
                case 1: /* State t1 */
                    /* Update COM pin */
                    DL_GPIO_setPins(COM_LINES_PORT, COM_LINES_COM0_PIN);

                    /* Toggle Segment pins */
                    DL_GPIO_togglePins(SEGMENT_Lines_Lower_PORT,
                        SEGMENT_LINE_LOWER_PORT_MASK);
                    DL_GPIO_togglePins(SEGMENT_Lines_Upper_PORT,
                        SEGMENT_LINE_UPPER_PORT_MASK);
                    break;
                case 2: /* State t2 */
                    /* Update COM pin */
                    DL_GPIO_enableHiZ(COM_LINES_COM0_IOMUX);
                    DL_GPIO_setPins(COM_LINES_PORT, COM_LINES_COM0_PIN);
                    DL_GPIO_clearPins(COM_LINES_PORT, COM_LINES_COM1_PIN);
                    DL_GPIO_disableHiZ(COM_LINES_COM1_IOMUX);

                    /* Update Segment pins */
                    DL_GPIO_writePinsVal(SEGMENT_Lines_Lower_PORT,
                        SEGMENT_LINE_LOWER_PORT_MASK, gLCDMemoryTableLower[1]);
                    DL_GPIO_writePinsVal(SEGMENT_Lines_Upper_PORT,
                        SEGMENT_LINE_UPPER_PORT_MASK, gLCDMemoryTableUpper[1]);
                    break;
                case 3: /* State t3 */
                    /* Update COM pin */
                    DL_GPIO_setPins(COM_LINES_PORT, COM_LINES_COM1_PIN);

                    /* Toggle Segment pins */
                    DL_GPIO_togglePins(SEGMENT_Lines_Lower_PORT,
                        SEGMENT_LINE_LOWER_PORT_MASK);
                    DL_GPIO_togglePins(SEGMENT_Lines_Upper_PORT,
                        SEGMENT_LINE_UPPER_PORT_MASK);
                    break;
                case 4: /* State t4 */
                    /* Update COM pin */
                    DL_GPIO_enableHiZ(COM_LINES_COM1_IOMUX);
                    DL_GPIO_setPins(COM_LINES_PORT, COM_LINES_COM1_PIN);
                    DL_GPIO_clearPins(COM_LINES_PORT, COM_LINES_COM2_PIN);
                    DL_GPIO_disableHiZ(COM_LINES_COM2_IOMUX);

                    /* Update Segment pins */
                    DL_GPIO_writePinsVal(SEGMENT_Lines_Lower_PORT,
                        SEGMENT_LINE_LOWER_PORT_MASK, gLCDMemoryTableLower[2]);
                    DL_GPIO_writePinsVal(SEGMENT_Lines_Upper_PORT,
                        SEGMENT_LINE_UPPER_PORT_MASK, gLCDMemoryTableUpper[2]);
                    break;
                case 5: /* State t5 */
                    /* Update COM pin */
                    DL_GPIO_setPins(COM_LINES_PORT, COM_LINES_COM2_PIN);

                    /* Toggle Segment pins */
                    DL_GPIO_togglePins(SEGMENT_Lines_Lower_PORT,
                        SEGMENT_LINE_LOWER_PORT_MASK);
                    DL_GPIO_togglePins(SEGMENT_Lines_Upper_PORT,
                        SEGMENT_LINE_UPPER_PORT_MASK);
                    break;
                case 6: /* State t6 */
                    /* Update COM pin */
                    DL_GPIO_enableHiZ(COM_LINES_COM2_IOMUX);
                    DL_GPIO_setPins(COM_LINES_PORT, COM_LINES_COM2_PIN);
                    DL_GPIO_clearPins(COM_LINES_PORT, COM_LINES_COM3_PIN);
                    DL_GPIO_disableHiZ(COM_LINES_COM3_IOMUX);

                    /* Update Segment pins */
                    DL_GPIO_writePinsVal(SEGMENT_Lines_Lower_PORT,
                        SEGMENT_LINE_LOWER_PORT_MASK, gLCDMemoryTableLower[3]);
                    DL_GPIO_writePinsVal(SEGMENT_Lines_Upper_PORT,
                        SEGMENT_LINE_UPPER_PORT_MASK, gLCDMemoryTableUpper[3]);
                    break;
                case 7: /* State t7 */
                    /* Update COM pin */
                    DL_GPIO_setPins(COM_LINES_PORT, COM_LINES_COM3_PIN);

                    /* Toggle Segment pins */
                    DL_GPIO_togglePins(SEGMENT_Lines_Lower_PORT,
                        SEGMENT_LINE_LOWER_PORT_MASK);
                    DL_GPIO_togglePins(SEGMENT_Lines_Upper_PORT,
                        SEGMENT_LINE_UPPER_PORT_MASK);

                    break;
            }
            /* Check for end of LCD update cycle */
            gLCDUpdatePeriod++;
            if (gLCDUpdatePeriod > 7) {
                gLCDUpdatePeriod = 0;
            }
            break;
        default:
            break;
    }
}

/**
 *  @brief      Writes characters to LCD map memory table
 *
 *  @param[in]  character       Index to gNumberMap[] to chose character to write
 *  @param[in]  position  Index to gSegmentOffsetMap[] for digit to segment translation
 *
 *  This function assumes segments are grouped together in pairs and next to each other in a port
 */
void LCDWriteCharacter(uint8_t character, uint8_t position)
{
    uint8_t upperNibble, lowerNibble, scratch, tempCount;
    uint8_t index = 0;

    upperNibble = (gNumberMap[character] & 0xF0) >> 4;
    lowerNibble = gNumberMap[character] & 0x0F;
    /* Determine Segment Port */
    if (position < SEGMENT_LINE_PORT_SPLIT / 2) {
        /* Lower Segment Port */
        for (tempCount = 0; tempCount < COM_LINE_TOTAL; tempCount++) {
            scratch = ((upperNibble >> tempCount) & 0x01) << 1;
            scratch |= (lowerNibble >> tempCount) & 0x01;
            gLCDMemoryTableLower[tempCount] &=
                ~(0x2 << gSegmentOffsetMap[position * 2]);
            gLCDMemoryTableLower[tempCount] |=
                scratch << gSegmentOffsetMap[position * 2];
        }
    } else { /* Upper Segment Port */
        for (tempCount = 0; tempCount < COM_LINE_TOTAL; tempCount++) {
            scratch = ((upperNibble >> tempCount) & 0x01) << 1;
            scratch |= (lowerNibble >> tempCount) & 0x01;
            gLCDMemoryTableUpper[tempCount] &=
                ~(0x2 << gSegmentOffsetMap[position * 2]);
            gLCDMemoryTableUpper[tempCount] |=
                scratch << gSegmentOffsetMap[position * 2];
        }
    }
}

/**
 * @brief Clears LCD map memory table
 *
 * This function clears the LCD memory map array.
*/
void clearLCDMemoryTable(void)
{
    for (int i = 0; i <= COM_LINE_TOTAL; i++) {
        gLCDMemoryTableLower[i] = 0;
        gLCDMemoryTableUpper[i] = 0;
    }
}
