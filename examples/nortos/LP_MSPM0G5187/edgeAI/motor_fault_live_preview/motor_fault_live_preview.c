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

#include "model/tvmgen_default.h"
#include "ADXL355.h"
#include <feature_extract.h>

/* UART communication and DAP (Debug Access Port) core headers */
#include "uart_comm_target.h"
#include "dap_core.h"

/* ARM CMSIS-DSP Header files */
#include "arm_const_structs.h"
#include "arm_math.h"
#include "stdlib.h"

/** \brief Size of the class buffer */
#define SAMPLE_BUFFER_SIZE (10)

/** \brief Number of input channels */
#define INPUT_CHANNELS     (3)

/** \brief Uart handler for DAP operations */
UART_Instance gUART;

/** \brief Output buffer for live preview data */
uint8_t livePrev_Output[5];

/** \brief Detected class for live preview */
volatile uint8_t livePrev_detClass = 0;

/** \brief Model input buffer */
int8_t if_map[1][3][128][1];

/** \brief Model output buffer */
int8_t of_map[1][4];

/** \brief Data points of a single frame */
q15_t input_data_q15[FE_FRAME_SIZE];

/** \brief Extracted features of a single frame */
int8_t featuresPerFrame[FE_FEATURE_SIZE_PER_FRAME];

/** \brief Complete feature set for model input */
int8_t totalFeatures[FE_FEATURE_SIZE_PER_FRAME*FE_NUM_FRAME_CONCAT*INPUT_CHANNELS];

/** \brief Circular class buffer to determine which class LED has to glow */
uint8_t classBuffer[SAMPLE_BUFFER_SIZE];
/** \brief Index for the class buffer */
uint8_t classBufferIndex = 0;

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

/** \brief Feature extraction from the vibration data */
void motor_FE()
{
    for(int i=0;i< FE_NUM_FRAME_CONCAT ;i++)
    {
        for(int j = 0 ; j < FE_FRAME_SIZE ; j++)
        {
            input_data_q15[j] = dataBuff.xData[i*FE_FRAME_SIZE + j];
        }
        FE_process(input_data_q15, featuresPerFrame, 0);

        /* Append Extracted features */
        memcpy(&totalFeatures[0*(FE_NUM_FRAME_CONCAT*FE_FEATURE_SIZE_PER_FRAME) + (i*FE_FEATURE_SIZE_PER_FRAME)], featuresPerFrame, sizeof(featuresPerFrame));
    }

    for(int i=0;i< FE_NUM_FRAME_CONCAT ;i++)
    {
        for(int j = 0 ; j < FE_FRAME_SIZE ; j++)
        {
            input_data_q15[j] = dataBuff.yData[i*FE_FRAME_SIZE + j];
        }
        FE_process(input_data_q15, featuresPerFrame, 1);

        /* Append Extracted features */
        memcpy(&totalFeatures[1*(FE_NUM_FRAME_CONCAT*FE_FEATURE_SIZE_PER_FRAME) + (i*FE_FEATURE_SIZE_PER_FRAME)], featuresPerFrame, sizeof(featuresPerFrame));
    }

    for(int i=0;i< FE_NUM_FRAME_CONCAT ;i++)
    {
        for(int j = 0 ; j < FE_FRAME_SIZE ; j++)
        {
            input_data_q15[j] = dataBuff.zData[i*FE_FRAME_SIZE + j];
        }
        FE_process(input_data_q15, featuresPerFrame, 2);

        /* Append Extracted features */
        memcpy(&totalFeatures[2*(FE_NUM_FRAME_CONCAT*FE_FEATURE_SIZE_PER_FRAME) + (i*FE_FEATURE_SIZE_PER_FRAME)], featuresPerFrame, sizeof(featuresPerFrame));
    }

    int index=0;
    /* Store the extracted features in the if_map*/
    for(int i=0;i<INPUT_CHANNELS;i++)
    {
        for(int j=0;j<FE_NUM_FRAME_CONCAT*FE_FEATURE_SIZE_PER_FRAME;j++)
        {
            if_map[0][i][j][0] = totalFeatures[index];
            index++;
        }
    }

}

/** \brief Glow LED based on which class has the maximum frequency */
void glow_led()
{
    uint8_t freq[4] = {0, 0, 0, 0};

    /* Calculate the frequency of the classes */
    for(int i=0;i<SAMPLE_BUFFER_SIZE;i++)
    {
        if(classBuffer[i])
        {
            freq[classBuffer[i]-1]++;
        }
    }

    /* Find class with maximum frequency */
    uint8_t maximum_freq_class = 0;
    for(int i=1;i<4;i++){
        if(freq[maximum_freq_class]<freq[i]){
            maximum_freq_class = i;
        }
    }

    /* Store detected class */
    livePrev_detClass = maximum_freq_class;

    /* Run only if the buffer is completely filled */
    if(freq[0]+freq[1]+freq[2]+freq[3] == SAMPLE_BUFFER_SIZE)
    {
        /* Clear all LEDs first */
        DL_GPIO_clearPins(GPIO_LEDS_LED1_PORT, GPIO_LEDS_LED1_PIN);
        DL_GPIO_clearPins(GPIO_LEDS_LED2_PORT, GPIO_LEDS_LED2_PIN);
        DL_GPIO_clearPins(GPIO_LEDS_LED3_PORT, GPIO_LEDS_LED3_PIN);

        /* Set LED(s) based on detected class */
        switch(maximum_freq_class)
        {
            case 0:
                DL_GPIO_setPins(GPIO_LEDS_LED1_PORT, GPIO_LEDS_LED1_PIN);
                break;
            case 1:
                DL_GPIO_setPins(GPIO_LEDS_LED2_PORT, GPIO_LEDS_LED2_PIN);
                break;
            case 2:
                DL_GPIO_setPins(GPIO_LEDS_LED3_PORT, GPIO_LEDS_LED3_PIN);
                break;
            case 3:
                DL_GPIO_setPins(GPIO_LEDS_LED1_PORT, GPIO_LEDS_LED1_PIN);
                DL_GPIO_setPins(GPIO_LEDS_LED2_PORT, GPIO_LEDS_LED2_PIN);
                DL_GPIO_setPins(GPIO_LEDS_LED3_PORT, GPIO_LEDS_LED3_PIN);
                break;
            default:
                break;
        }
    }
}

/** \brief Append output class with maximum frequency to the buffer */
void add_output_class()
{
    /* Find class with maximum +ve value */
    uint8_t output_class = 0;
    for(int i=1;i<4;i++){
        if(of_map[0][output_class]<of_map[0][i]){
            output_class = i;
        }
    }

    classBuffer[classBufferIndex] = output_class + 1;
    classBufferIndex = (classBufferIndex + 1)%SAMPLE_BUFFER_SIZE;
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

    DL_GPIO_clearPins(GPIO_LEDS_LED1_PORT, GPIO_LEDS_LED1_PIN);
    DL_GPIO_clearPins(GPIO_LEDS_LED2_PORT, GPIO_LEDS_LED2_PIN);
    DL_GPIO_clearPins(GPIO_LEDS_LED3_PORT, GPIO_LEDS_LED3_PIN);

    /* Power up the neural processing unit (NPU) module.
     * Clear and enable the NPU interrupt.
     * NPU interrupts will be handled inside the NPU
     * library created during model compilation (outside of this project).
     * NOTE: In future releases, the NPU initialization calls made below
     * may be transitioned to SysConfig generated calls placed in
     * the auto-generatead ti_msp_dl_config.h/.c files, at which point
     * they will be handled inside of the SYSCFG_DL_init() call.
     */
    DL_NPU_reset(NPU);
    DL_NPU_enablePower(NPU);
    while (!(DL_SYSCTL_getStatus() & DL_SYSCTL_STATUS_NPU_READY));
    DL_NPU_clearInterruptStatus(NPU, DL_NPU_INTERRUPT_DONE);
    DL_NPU_enableInterrupt(NPU, DL_NPU_INTERRUPT_DONE);
    NVIC_EnableIRQ(NPU_INT_IRQn);

    struct tvmgen_default_inputs tvm_if_map = {(void*) &if_map[0]};
    struct tvmgen_default_outputs tvm_of_map = {(void*) &of_map[0]};

    /* Initialize ARM library for real FFT */
    FE_init();

    /* Sensor initialization */
    sensor_init();
    ADXL355_config(sensor);
    ADXL355_enableInterrupts(&sensor);
    NVIC_EnableIRQ(SENSOR_INT_IRQN);

    /* If write and read were successful, toggle LED */
    while (1)
    {
        /* DAP: Check for incoming UART commands */
        UART_checkForCommand(&gUART);

        if(gUART.RxStatus == UART_STATUS_PROCESS_FRAME)
        {
            DAP_processBuffer(&gUART);
            gUART.RxStatus = UART_STATUS_IDLE;
            DMA_RX_init(&gUART);
        }

        if(gStartStream)
        {
            if(gPipelineConfig.mode == PIPELINE_MODE_SENSOR_INFERENCE)
            {
                /* Prepare and send live preview output over UART */
                livePrev_Output[0] = livePrev_detClass;
                livePrev_Output[1] = (int8_t) of_map[0][0] ;
                livePrev_Output[2] = (int8_t) of_map[0][1] ;
                livePrev_Output[3] = (int8_t) of_map[0][2] ;
                livePrev_Output[4] = (int8_t) of_map[0][3] ;

                DAP_receivedDataResponse(&gUART, Resp_CMD_Channel_inferencing_value, 5, &livePrev_Output[0]);
            }
        }

        if(dataReady)
        {
            ADXL355_disableInterrupts(&sensor);

            /* Give data to feature extraction and model */
            motor_FE();

            /* Run Inference */
            tvmgen_default_run(&tvm_if_map, &tvm_of_map);

            while(!tvmgen_default_finished);

            /* Add the output class with maximum value to the classBuffer */
            add_output_class();

            /* Glow the respective led for the class with maximum frequency */
            glow_led();

            ADXL355_enableInterrupts(&sensor);

            NVIC_EnableIRQ(SENSOR_INT_IRQN);
            buffCount = 0;
            dataReady = 0;
        }
    }
}

/** \brief Interrupt handler for reading SPI data from the sensor */
void GROUP1_IRQHandler(void)
{
    switch (DL_GPIO_getPendingInterrupt(SENSOR_PORT))
    {
    case SENSOR_RDY_1_IIDX:
        if(sensor.sensorMode == SENSOR_MODE_COLLECT_DATA)
        {
            SPI_readByte(sensor, ADXL355_STATUS_ADDRESS);
            ADXL355_getData(sensor);
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

/** \brief DMA interrupt handler for UART/DAP data transfer */
void DMA_IRQHandler(void)
{
    switch (DL_DMA_getPendingInterrupt(DMA))
    {
        case DL_DMA_FULL_CH_EVENT_IIDX_EARLY_IRQ_DMACH1:
            DAP_getFrameLength(&gUART);
            gUART.RxStatus = UART_STATUS_RX_BUFFERING;
            break;

        case DL_DMA_EVENT_IIDX_DMACH0:
            gUART.TxStatus = UART_STATUS_IDLE;
            break;

        default:
            break;
    }
}
