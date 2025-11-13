/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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

#include <ti/driverlib/m0p/dl_interrupt.h>
#include "ti_msp_dl_config.h"

/* Includes for DAP */
#include "uart_comm_target.h"
#include "dap_core.h"

/*! ADC Offset */
#define ADC_OFFSET      (2048U)

/* Global Variable to store ADC result */
volatile uint32_t gADCresult;

/* DAP Variables */
UART_Instance gUART;

/* Number of ADC data to send */
volatile uint32_t gSendAdcData = 0;

int main(void)
 {
    SYSCFG_DL_init();

    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    /* DAP INIT */
    NVIC_EnableIRQ(DMA_INT_IRQn);
    UART_init(&gUART);

    /* Start ADC conversion */
    DL_ADC12_startConversion(ADC12_0_INST);

    while (1) {

        /* DAP */
        UART_checkForCommand(&gUART);

        if(gUART.RxStatus == UART_STATUS_PROCESS_FRAME)
        {
            DAP_processBuffer(&gUART);
            gUART.RxStatus = UART_STATUS_IDLE;
            DMA_RX_init(&gUART);
        }

        if(gStartStream)
        {
            if(gPipelineConfig.mode == PIPELINE_MODE_DATA_ACQUISITION)
            {
                /* Wait if UART is already transmitting data */
               while(gUART.TxStatus == UART_STATUS_TX_TRANSMITTING)
               {};

               /* Send start byte */
               DL_UART_transmitDataBlocking(UART_DAP_INST,FRAME_START_BYTE);
               /* Send channel number */
               DL_UART_transmitDataBlocking(UART_DAP_INST, Serial_Resp_receiveData | Resp_CMD_Channel_sensor_signal);
               uint32_t payloadLen = (gProperties[0]->value.u16)*getDataLen(gProperties[0]->type);

               /* Send Payload length */               
               if(IN_RANGE(payloadLen, PAYLOAD_FIELD_1B_MIN_LEN, PAYLOAD_FIELD_1B_MAX_LEN)) 
               {
                   DL_UART_transmitDataBlocking(UART_DAP_INST, (payloadLen) & 0xFF);
               }

               else if(IN_RANGE(payloadLen, PAYLOAD_FIELD_2B_MIN_LEN, PAYLOAD_FIELD_2B_MAX_LEN))
               {
                   uint16_t dapLen = PAYLOAD_FIELD_2B_OFFSET + payloadLen;
                   DL_UART_transmitDataBlocking(UART_DAP_INST, (dapLen >> 8) & 0xFF);
                   DL_UART_transmitDataBlocking(UART_DAP_INST, (dapLen) & 0xFF);
               }

               else if(IN_RANGE(payloadLen, PAYLOAD_FIELD_3B_MIN_LEN, PAYLOAD_FIELD_3B_MAX_LEN))
               {
                   uint32_t dapLen = PAYLOAD_FIELD_3B_OFFSET + payloadLen;
                   DL_UART_transmitDataBlocking(UART_DAP_INST, (dapLen >> 16) & 0xFF);
                   DL_UART_transmitDataBlocking(UART_DAP_INST, (dapLen >> 8) & 0xFF);
                   DL_UART_transmitDataBlocking(UART_DAP_INST, (dapLen) & 0xFF);
               }

               gStartStream = false;

               gUART.TxStatus = UART_STATUS_TX_TRANSMITTING;
               gSendAdcData = gProperties[0]->value.u32;
            }
        }
    }
}

void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            {
                gADCresult = DL_ADC12_getMemResult(ADC12_0_INST, ADC12_0_ADCMEM_0);

                /* Converts 12 bit ADC result to signed 16-bit Q15 format */
                int16_t dataCaptured_q15 = (gADCresult - ADC_OFFSET) << 4;
            
                if(gSendAdcData > 0)
                {                
                    DL_UART_transmitDataBlocking(UART_DAP_INST, (dataCaptured_q15 >> 8) & 0xFF);
                    DL_UART_transmitDataBlocking(UART_DAP_INST, dataCaptured_q15 & 0xFF);
                    gSendAdcData--;

                    /* Send Frame End Byte after sending last data */
                    if(gSendAdcData == 0)
                    {
                        /* Send End Byte */
                        DL_UART_transmitDataBlocking(UART_DAP_INST, FRAME_END_BYTE);
                        gUART.TxStatus = UART_STATUS_IDLE;
                    }
                }
            }
            break;
            
        default:
            break;
    }
}

void DMA_IRQHandler(void)
{
    switch (DL_DMA_getPendingInterrupt(DMA))
    {
        case DL_DMA_FULL_CH_EVENT_IIDX_EARLY_IRQ_DMACH0:
            DAP_getFrameLength(&gUART);
            gUART.RxStatus = UART_STATUS_RX_BUFFERING;
            break;

        case DL_DMA_EVENT_IIDX_DMACH1:
            gUART.TxStatus = UART_STATUS_IDLE;
            break;

        default:
            break;
    }
}
