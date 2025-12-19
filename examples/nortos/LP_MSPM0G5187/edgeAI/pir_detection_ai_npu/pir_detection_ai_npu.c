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

#include "npu.h"

/* Artifacts */
#include "model/tvmgen_default.h"

#include "feature_extract.h"

#define HUMAN_MOTION_INDEX          (0)
#define BACKGROUND_MOTION_INDEX     (1)
#define DOG_MOTION_INDEX            (2)

/* Defines the size of each buffer for ADC data acquisition */
#define ADC_BUFFER_SIZE         FE_FRAME_SIZE

/* Defines the number of buffer for ADC data acquisition */
#define NUM_BUFFER              (2)

/* Global Variable to store input to the Model */
int8_t if_map_int8[1][1][25][20];

/* Global variable to store Model output */
int8_t of_map[1][3] = {0,0,0};

/* Stores extracted features of a single window */
volatile q15_t featuresPerWindow[FEATURE_SIZE_PER_WINDOW];

/* ADC related Params */
/* Ping pong buffers for storing data from the ADC */
volatile q15_t ADC_buffers[NUM_BUFFER][ADC_BUFFER_SIZE];
volatile uint32_t dataIndex = 0;
volatile q15_t prodBufferIndex = 0,conBufferIndex = 0;

/* Flag to indicate whether the buffer is full */
volatile bool ADC_bufferState[NUM_BUFFER];

/* Stores the ADC result */
volatile uint32_t gADCresult;

/* Stores the data for each window */
q15_t window[32];

void postProcessing(void)
{
    /* Index with the highest value corresponds to the model's predicted class */
    uint8_t maxIndex = HUMAN_MOTION_INDEX;

    if(of_map[0][BACKGROUND_MOTION_INDEX] > of_map[0][maxIndex])
    {
        maxIndex = BACKGROUND_MOTION_INDEX;
    }

    if(of_map[0][DOG_MOTION_INDEX] > of_map[0][maxIndex])
    {
        maxIndex = DOG_MOTION_INDEX;
    }

    DL_GPIO_clearPins(LED_RED_PORT, LED_RED_PIN);
    DL_GPIO_clearPins(LED_BLUE_PORT, LED_BLUE_PIN);
    DL_GPIO_clearPins(LED_GREEN_PORT, LED_GREEN_PIN);

    /* Based on the model output, Turn on the corresponding LED */
    switch(maxIndex)
    {
        case HUMAN_MOTION_INDEX:
                    DL_GPIO_setPins(LED_RED_PORT,LED_RED_PIN);
                    break;

        case BACKGROUND_MOTION_INDEX:
                    DL_GPIO_setPins(LED_BLUE_PORT,LED_BLUE_PIN);
                    break;

        case DOG_MOTION_INDEX:
                    DL_GPIO_setPins(LED_GREEN_PORT,LED_GREEN_PIN);
                    break;

        default:
            break;
    }
}

int main(void)
{
    SYSCFG_DL_init();

    struct tvmgen_default_inputs tvm_if_map = {(void*) &if_map_int8[0]};
    struct tvmgen_default_outputs tvm_of_map = {(void*) &of_map[0]};

    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    DL_ADC12_startConversion(ADC12_0_INST);

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

    FE_init();

    while (1) {
        /* Check if the current ADC buffer is full and ready for processing */
        if(ADC_bufferState[conBufferIndex])
        {
            for(int i = 0 ; (i + WINDOW_SIZE) < FE_FRAME_SIZE ; i+= SLIDING_WINDOW_SIZE)
            {
                for(int j = 0; j < WINDOW_SIZE; j++)
                {
                    window[j] = ADC_buffers[conBufferIndex][i + j];
                }

                FE_process(window, &if_map_int8[0][0][i/SLIDING_WINDOW_SIZE], 0);
            }

            /* Setting the current ADC Buffer state to empty */
            ADC_bufferState[conBufferIndex] = 0;

            conBufferIndex = (conBufferIndex + 1) % NUM_BUFFER;

            /* Run Inference */
            tvmgen_default_run(&tvm_if_map, &tvm_of_map);

            while (!tvmgen_default_finished)
            {
                __NOP();
            }

            postProcessing();
        }
    }
}


void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            gADCresult = DL_ADC12_getMemResult(ADC12_0_INST, ADC12_0_ADCMEM_0);

            /* Check if whole ADC Buffer data is captured */
            if(dataIndex < ADC_BUFFER_SIZE)
            {
                /* Storing the data in Q15 format */
                ADC_buffers[prodBufferIndex][dataIndex++] = (int16_t)(gADCresult - 128);
            }

            if(dataIndex >= ADC_BUFFER_SIZE)
            {
                /* Set flag that the buffer is full */
                ADC_bufferState[prodBufferIndex] = true;

                prodBufferIndex = (prodBufferIndex + 1) % NUM_BUFFER;

                /* Reset the flag */
                ADC_bufferState[prodBufferIndex] = false;
                dataIndex = 0;
            }
            break;

        default:
            break;
    }
}
