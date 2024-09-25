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

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "BQ79616.h"
#include "UART.h"
#include "ti_msp_dl_config.h"

/* ARRAYS (MUST place out of loop so not re-allocated every iteration) */
/* hold all 16 vcell*_hi/lo values */
BYTE response_frame[(16 * 2 + 6) * TOTALBOARDS];
/* Switch interrupt flag */
volatile bool gTXMsg;
DL_MCAN_TxBufElement txMsg0;

void CAN_Msg_Init(DL_MCAN_TxBufElement* CANMsg);

int main(void)
{
    SYSCFG_DL_init();

    int i, currentBoard;
    /* define for CAN output format */
    uint16_t cunt, cunto;

    /* Initialize message0 to transmit. */
    CAN_Msg_Init(&txMsg0);

    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(MCAN0_INST))
        ;

    BQ79616_Wake();
    /* write register to start wake tone */
    WriteReg(0, CONTROL1, 0x20, 1, FRMWRT_SGL_W);
    /**
     * wake tone duration is ~1.6ms per board
     * + 10ms per board for each device to wake up from shutdown = 11.6ms per 616 board
     * (rounded to 12ms since variable used is an integer)
     */
    delay_cycles(CLKper * 1000 * 12 * TOTALBOARDS);
    //ReadReg(1, CONTROL1, response_frame, 1, 0, FRMWRT_SGL_R);

    AutoAddress();
    /**
     * 4ms total required after shutdown to wake transition for AFE settling time,
     * this is for top device only
     */
    delay_cycles(CLKper * 1000 * 4);
    /* MASK CUST_CRC SO CONFIG CHANGES DON'T FLAG A FAULT */
    WriteReg(0, FAULT_MSK2, 0x40, 1, FRMWRT_ALL_W);
    /* CLEAR ALL FAULTS */
    ResetAllFaults(0, FRMWRT_ALL_W);

    /* set up the main ADC */
    /* set all cells to active */
    WriteReg(0, ACTIVE_CELL, 0x0A, 1, FRMWRT_ALL_W);
    /* 26Hz LPF_Vcell (38ms average) */
    WriteReg(0, ADC_CONF1, 0x02, 1, FRMWRT_ALL_W);
    /* continuous run, LPF enabled and MAIN_GO */
    WriteReg(0, ADC_CTRL1, 0x0E, 1, FRMWRT_ALL_W);
    /* initial delay to allow LPF to average for 38ms (26Hz LPF setting used) */
    delay_cycles(CLKper * 1000 * 38 + 5 * CLKper * TOTALBOARDS);

    while (1) {
        if (gTXMsg == true) {
            /********************************************************************************
             * READ CELL VOLTAGES
             */
            /* clear out the array so we know we have fresh data every loop  */
            memset(response_frame, 0, sizeof(response_frame));
            /* wait single round robin cycle time + reclocking delay for each device, every loop */
            delay_cycles(32 * (192 + 5 * TOTALBOARDS));
            /* read all the values (HI/LO for each cell = 32 total) */
            ReadReg(0, VCELL16_HI, response_frame, 16 * 2, 0, FRMWRT_ALL_R);

            /* NOTE: printf function is not guaranteed to work on all system. */
            /* format and print the resultant response frame */
            /* start with a newline to add some extra spacing between loop */
            printf("\n");

            for (currentBoard = 0; currentBoard < TOTALBOARDS;
                 currentBoard++) {
                /* response frame actually starts with top of stack, so currentBoard is actually inverted from what it should be */
                printf("BOARD %d:\t", TOTALBOARDS - currentBoard);

                /* go through each byte in the current board (32 bytes = 16 cells * 2 bytes each) */
                for (i = 0; i < 32; i += 2) {
                    /* each board responds with 32 data bytes + 6 header bytes */
                    /* so need to find the start of each board by doing that * currentBoard */
                    int boardByteStart = (16 * 2 + 6) * currentBoard;

                    /* convert the two individual bytes of each cell into a single 16 bit data item (by bit shifting) */
                    uint16_t rawData =
                        (response_frame[boardByteStart + i + 4] << 8) |
                        response_frame[boardByteStart + i + 5];

                    /* cast as a signed 16 bit data item, and multiply by 190.73uV to get an actual voltage */
                    float cellVoltage = ((int16_t) rawData) * 0.00019073;

                    /**
                     * print the voltages - it is (32-i)/2 because cells start from 16 down to 1
                     * and there are 2 bytes per cell (i value is twice the cell number)
                     */
                    printf("Cell %d = %f\t", (32 - i) / 2, cellVoltage);
                }
                /* newline per board */
                printf("\n");
            }  //end of currentBoard loop
            /* transmit the massage with CAN */
            for (currentBoard = 0; currentBoard < TOTALBOARDS;
                 currentBoard++) {
                int boardByteStart = (16 * 2 + 6) * currentBoard;
                for (cunt = 0; cunt < 4; cunt++) {
                    for (cunto = 0; cunto < 8; cunto++) {
                        txMsg0.data[cunto] =
                            response_frame[boardByteStart + cunto + cunt * 8 +
                                           4];
                    }
                    delay_cycles(320000);
                    DL_MCAN_writeMsgRam(
                        MCAN0_INST, DL_MCAN_MEM_TYPE_BUF, 0, &txMsg0);
                    DL_MCAN_TXBufAddReq(MCAN0_INST, 0);
                }
            }  //end of currentBoard
        }      //end of (gTXMsg == true)
    }          //end of main while
}  //end of main

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_SWITCHES_INT_IIDX:
            switch (DL_GPIO_getPendingInterrupt(GPIO_SWITCHES_PORT)) {
                case DL_GPIO_IIDX_DIO21:
                    gTXMsg = true;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void CAN_Msg_Init(DL_MCAN_TxBufElement* CANMsg)
{
    /* Identifier Value. */
    CANMsg->id = ((uint32_t)(0x4)) << 18U;
    /* Transmit data frame. */
    CANMsg->rtr = 0U;
    /* 11-bit standard identifier. */
    CANMsg->xtd = 0U;
    /* ESI bit in CAN FD format depends only on error passive flag. */
    CANMsg->esi = 0U;
    /* Transmitting 4 bytes. */
    CANMsg->dlc = 8U;
    /* CAN FD frames transmitted with bit rate switching. */
    CANMsg->brs = 1U;
    /* Frame transmitted in CAN FD format. */
    CANMsg->fdf = 0U;
    /* Store Tx events. */
    CANMsg->efc = 1U;
    /* Message Marker. */
    CANMsg->mm = 0xAAU;
    /* Data bytes. */
    CANMsg->data[0] = 0x01;
}
