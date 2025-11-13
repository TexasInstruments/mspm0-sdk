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
#include "feature_extract.h"

/* ARM CMSIS-DSP Header files */
#include "arm_const_structs.h"
#include "arm_math.h"
#include "stdlib.h"

/** \brief Size of the class buffer */
#define SAMPLE_BUFFER_SIZE (10)

/** \brief Number of input channels */
#define INPUT_CHANNELS     (3)

/** \brief Model input buffer */
float if_map[1][384][1][1];

/** \brief Model output buffer */
float of_map[1][4]={0,0,0,0};

/** \brief Data points of a single frame */
q15_t input_data_q15[FE_FRAME_SIZE];

/** \brief Extracted features of a single frame */
q15_t featuresPerFrame[FE_FEATURE_SIZE_PER_FRAME];

/** \brief Complete feature set for model input */
q15_t totalFeatures[FE_FEATURE_SIZE_PER_FRAME*FE_NUM_FRAME_CONCAT*INPUT_CHANNELS];

/** \brief Circular class buffer to determine which class LED has to glow */
uint8_t class_buffer[SAMPLE_BUFFER_SIZE];
/** \brief Index for the class buffer */
uint8_t class_buffer_index = 0;

/** \brief Sensor configuration structure */
sensorStruct sensor;

/** \brief Initialises the sensor */
void sensor_init()
{
    sensor.spiReg      = SPI_0_INST;
    sensor.csPort      = SPI_PORT;
    sensor.csPin       = SPI_CS_0_PIN;
    sensor.readyPort   = SENSOR_PORT;
    sensor.readyPin    = SENSOR_RDY_1_PIN;
    sensor.sensorMode  = SENSOR_MODE_IDLE;

    DL_GPIO_disableInterrupt(SENSOR_PORT, SENSOR_RDY_1_PIN);
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
        FE_process(input_data_q15, featuresPerFrame);

        /* Append Extracted features */
        memcpy(&totalFeatures[0*(FE_NUM_FRAME_CONCAT*FE_FEATURE_SIZE_PER_FRAME) + (i*FE_FEATURE_SIZE_PER_FRAME)], featuresPerFrame, sizeof(featuresPerFrame));
    }

    for(int i=0;i< FE_NUM_FRAME_CONCAT ;i++)
    {
        for(int j = 0 ; j < FE_FRAME_SIZE ; j++)
        {
            input_data_q15[j] = dataBuff.yData[i*FE_FRAME_SIZE + j];
        }
        FE_process(input_data_q15, featuresPerFrame);

        /* Append Extracted features */
        memcpy(&totalFeatures[1*(FE_NUM_FRAME_CONCAT*FE_FEATURE_SIZE_PER_FRAME) + (i*FE_FEATURE_SIZE_PER_FRAME)], featuresPerFrame, sizeof(featuresPerFrame));
    }

    for(int i=0;i< FE_NUM_FRAME_CONCAT ;i++)
    {
        for(int j = 0 ; j < FE_FRAME_SIZE ; j++)
        {
            input_data_q15[j] = dataBuff.zData[i*FE_FRAME_SIZE + j];
        }
        FE_process(input_data_q15, featuresPerFrame);

        /* Append Extracted features */
        memcpy(&totalFeatures[2*(FE_NUM_FRAME_CONCAT*FE_FEATURE_SIZE_PER_FRAME) + (i*FE_FEATURE_SIZE_PER_FRAME)], featuresPerFrame, sizeof(featuresPerFrame));
    }

    /* Store the extracted features in the if_map*/
    for(int i=0;i<FE_NUM_FRAME_CONCAT*FE_FEATURE_SIZE_PER_FRAME*INPUT_CHANNELS;i++)
    {
        if_map[0][i][0][0] = totalFeatures[i];
    }

}

/** \brief Glow LED based on which class has the maximum frequency */
void glow_led()
{
    uint8_t freq[4] = {0, 0, 0, 0};

    /* Calculate the frequency of the classes */
    for(int i=0;i<SAMPLE_BUFFER_SIZE;i++)
    {
        if(class_buffer[i])
        {
            freq[class_buffer[i]-1]++;
        }
    }

    /* Find class with maximum frequency */
    uint8_t maximum_freq_class = 0;
    for(int i=1;i<4;i++){
        if(freq[maximum_freq_class]<freq[i]){
            maximum_freq_class = i;
        }
    }

    /* Run only if the buffer is completely filled */
    if(freq[0]+freq[1]+freq[2]+freq[3] == SAMPLE_BUFFER_SIZE)
    {
        DL_GPIO_clearPins(GPIO_LEDS_LED1_PORT, GPIO_LEDS_LED1_PIN);
        DL_GPIO_clearPins(GPIO_LEDS_LED2_PORT, GPIO_LEDS_LED2_PIN);
        DL_GPIO_clearPins(GPIO_LEDS_LED3_PORT, GPIO_LEDS_LED3_PIN);

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

    class_buffer[class_buffer_index] = output_class + 1;
    class_buffer_index = (class_buffer_index + 1)%SAMPLE_BUFFER_SIZE;
}

/* Main function */
int main(void)
{
    SYSCFG_DL_init();

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
        if(dataReady)
        {
            ADXL355_disableInterrupts(&sensor);

            /* Give data to feature extraction and model */
            motor_FE();

            /* Run Inference */
            tvmgen_default_run(&tvm_if_map, &tvm_of_map);

            while(!tvmgen_default_finished);

            /* Add the output class with maximum value to the class_buffer */
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

/* Interrupt handler for reading SPI data from the sensor */
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
