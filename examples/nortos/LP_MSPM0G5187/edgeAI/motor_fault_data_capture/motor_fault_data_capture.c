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

#include "ti_msp_dl_config.h"
#include "ADXL355.h"

/* UART communication and DAP (Debug Access Port) core headers */
#include "uart_comm_target.h"
#include "dap_core.h"

#include "stdlib.h"

/** \brief Uart handler for DAP operations */
UART_Instance gUART;

/** \brief Number of vibration data to be transmitted */
volatile uint32_t gSendVibrationData = 0;

/** \brief Size of a single data point */
uint32_t dataPointSize = 3;

/** \brief Sensor configuration structure */
sensorStruct sensor;

/** \brief Initialises the sensor */
void sensor_init()
{
    sensor.spiReg      = SPI_0_INST;      /* SPI instance for sensor */
    sensor.csPort      = SPI_PORT;        /* Chip select port */
    sensor.csPin       = SPI_CS_0_PIN;    /* Chip select pin */
    sensor.readyPort   = SENSOR_PORT;     /* Ready port for sensor */
    sensor.readyPin    = SENSOR_RDY_1_PIN;/* Ready pin for sensor */
    sensor.sensorMode  = SENSOR_MODE_IDLE;/* Set sensor to idle mode */

    DL_GPIO_disableInterrupt(SENSOR_PORT, SENSOR_RDY_1_PIN); /* Disable sensor interrupt initially */
}

/** \brief Main function */
int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(DMA_INT_IRQn);

    UART_init(&gUART);

    DL_DMA_clearInterruptStatus(DMA, DL_DMA_FULL_CH_INTERRUPT_EARLY_CHANNEL1);
    DL_DMA_enableInterrupt(DMA, DL_DMA_FULL_CH_INTERRUPT_EARLY_CHANNEL1);
    DL_DMA_Full_Ch_setEarlyInterruptThreshold(DMA, DMA_CH0_CHAN_ID, DL_DMA_EARLY_INTERRUPT_THRESHOLD_64);

    DL_GPIO_clearInterruptStatus(SENSOR_PORT, SENSOR_RDY_1_PIN);
    DL_GPIO_disableInterrupt(SENSOR_PORT, SENSOR_RDY_1_PIN);

    /* Sensor initialization */
    sensor_init();
    ADXL355_config(sensor);
    ADXL355_enableInterrupts(&sensor);
    NVIC_EnableIRQ(SENSOR_INT_IRQN);

    while (1)
    {
        /* DAP: Check for incoming UART commands */
        UART_checkForCommand(&gUART);

        if(gUART.RxStatus == UART_STATUS_PROCESS_FRAME)
        {
            /* Process incoming DAP frame then reset status and prepare RX */
            DAP_processBuffer(&gUART);
            gUART.RxStatus = UART_STATUS_IDLE;
            DMA_RX_init(&gUART);
        }

        /* Calculates the payload and sends it over UART */
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
                uint32_t payloadLen = dataPointSize*(gProperties[0]->value.u16)*getDataLen(gProperties[0]->type);

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
                gSendVibrationData = gProperties[0]->value.u32;
            }
        }
    }
}

/** \brief DMA interrupt handler for UART/DAP data transfer */
void DMA_IRQHandler(void)
{
    switch (DL_DMA_getPendingInterrupt(DMA))
    {
        case DL_DMA_FULL_CH_EVENT_IIDX_EARLY_IRQ_DMACH1:
            /* Early DMA threshold reached: prepare to determine frame length */
            DAP_getFrameLength(&gUART);
            gUART.RxStatus = UART_STATUS_RX_BUFFERING;
            break;

        case DL_DMA_EVENT_IIDX_DMACH0:
            /* RX DMA finished for channel 0 - mark UART idle */
            gUART.TxStatus = UART_STATUS_IDLE;
            break;

        default:
            /* Unknown/unused DMA event */
            break;
    }
}

/** \brief GPIO interrupt handler for sensor ready events */
void GROUP1_IRQHandler(void)
{
    switch (DL_GPIO_getPendingInterrupt(SENSOR_PORT))
    {
        case SENSOR_RDY_1_IIDX:
            if(sensor.sensorMode == SENSOR_MODE_COLLECT_DATA)
            {
                /* Read status to acknowledge sensor and fetch the new sample */
                SPI_readByte(sensor, ADXL355_STATUS_ADDRESS);
                ADXL355_getData(sensor);

                int16_t xComp = (int16_t)xData;
                int16_t yComp = (int16_t)yData;
                int16_t zComp = (int16_t)zData;

                /* Send data over UART */
                if(gSendVibrationData > 0)
                {
                    DL_UART_transmitDataBlocking(UART_DAP_INST, (xComp >> 8) & 0xFF);
                    DL_UART_transmitDataBlocking(UART_DAP_INST, xComp & 0xFF);
                    DL_UART_transmitDataBlocking(UART_DAP_INST, (yComp >> 8) & 0xFF);
                    DL_UART_transmitDataBlocking(UART_DAP_INST, yComp & 0xFF);
                    DL_UART_transmitDataBlocking(UART_DAP_INST, (zComp >> 8) & 0xFF);
                    DL_UART_transmitDataBlocking(UART_DAP_INST, zComp & 0xFF);
                    
                    gSendVibrationData--;

                    /* Send Frame End Byte after sending last data */
                    if(gSendVibrationData == 0)
                    {
                        DL_UART_transmitDataBlocking(UART_DAP_INST, FRAME_END_BYTE);
                        gUART.TxStatus = UART_STATUS_IDLE;
                    }
                }
            }
            else
            {
                /* Incorrect interrupt  */
            }
            break;
        default:
            break;
    }
}
