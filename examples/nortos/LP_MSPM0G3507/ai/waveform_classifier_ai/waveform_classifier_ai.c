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

/* Artifacts */
#include "model/tvmgen_default.h"

#include <ti/feature_extract/feature_extract.h>


#define SAWTOOTH_WAVE_INDEX     (0)
#define SINE_WAVE_INDEX         (1)
#define SQUARE_WAVE_INDEX       (2)

/* Defines the number of frames to skip in each inference step (Sliding window) */
#define SLIDING_WINDOW_SIZE     (4)

/* Defines the size of each buffer for ADC data acquisition */
#define ADC_BUFFER_SIZE         ((SLIDING_WINDOW_SIZE)*(FE_FRAME_SIZE))

/* Defines the number of buffer for ADC data acquisition */
#define NUM_BUFFER              (2)

/* Global Variable to store input to the Model */
float if_map[1][128][1][1] ;
/* Global variable to store Model output */
float of_map[1][3]={0,0,0};

/* Stores extracted features of a single frame */
volatile q15_t featuresPerFrame[FE_FEATURE_SIZE_PER_FRAME];

/* Stores features for all frames within the current sliding window */
volatile q15_t newFeatures[(SLIDING_WINDOW_SIZE * FE_FEATURE_SIZE_PER_FRAME)];

/* Stores the complete feature set for model input */
volatile q15_t totalFeatures[FE_FEATURE_SIZE_PER_FRAME*FE_NUM_FRAME_CONCAT];

/* ADC related Params */
/* Ping pong buffers for storing data from the ADC */
volatile q15_t ADC_buffers[NUM_BUFFER][ADC_BUFFER_SIZE];
volatile uint32_t dataIndex = 0;
volatile q15_t prodBufferIndex = 0,conBufferIndex = 0;

/* Flag to indicate whether the buffer is full */
volatile bool ADC_bufferState[NUM_BUFFER];

volatile uint32_t gADCresult;

void postProcessing(void)
{
    /* Index with the highest value corresponds to the model's predicted class */
    uint8_t maxIndex = SAWTOOTH_WAVE_INDEX;

    if(of_map[0][SINE_WAVE_INDEX] > of_map[0][maxIndex])
    {
        maxIndex = SINE_WAVE_INDEX;
    }

    if(of_map[0][SQUARE_WAVE_INDEX] > of_map[0][maxIndex])
    {
        maxIndex = SQUARE_WAVE_INDEX;
    }

    DL_GPIO_clearPins(LED_PORT, LED_RED_PIN | LED_BLUE_PIN | LED_GREEN_PIN);

    /* Based on the model output, Turn on the corresponding LED */
    switch(maxIndex)
    {
        case SAWTOOTH_WAVE_INDEX:
                    DL_GPIO_setPins(LED_PORT,LED_RED_PIN);
                    break;

        case SINE_WAVE_INDEX:
                    DL_GPIO_setPins(LED_PORT,LED_BLUE_PIN);
                    break;

        case SQUARE_WAVE_INDEX:
                    DL_GPIO_setPins(LED_PORT,LED_GREEN_PIN);
                    break;

        default:
            break;
    }
}

int main(void)
{
    SYSCFG_DL_init();

    struct tvmgen_default_inputs tvm_if_map = {(void*) &if_map[0]};
    struct tvmgen_default_outputs tvm_of_map = {(void*) &of_map[0]};

    FE_init();

    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    DL_ADC12_startConversion(ADC12_0_INST);

    while (1) {

        /* Check if the current ADC buffer is full and ready for processing */
        if(ADC_bufferState[conBufferIndex])
        {
            /* Transfer the curr ADC Buffer */
            for(int j=0; j < SLIDING_WINDOW_SIZE ; j++)
            {
                /* Start address of each frame */
                q15_t* framePtr = &ADC_buffers[conBufferIndex][j*(FE_FRAME_SIZE)];

                /* Extract Features from the corresponding frame */
                FE_process(framePtr,featuresPerFrame);

                /* Append Extracted features */
                memcpy(&newFeatures[(j * FE_FEATURE_SIZE_PER_FRAME)], featuresPerFrame, sizeof(featuresPerFrame));
            }

            /* Setting the current ADC Buffer state to empty */
            ADC_bufferState[conBufferIndex] = 0;

            conBufferIndex = (conBufferIndex + 1) % NUM_BUFFER;

            /* Calculate the number of features that need to be shifted to the left */
            uint32_t featuresToRetain = (FE_NUM_FRAME_CONCAT - SLIDING_WINDOW_SIZE) * (FE_FEATURE_SIZE_PER_FRAME);

            /* Calculate the starting index for the data being shifted to the left*/
            uint32_t shiftIndex = (SLIDING_WINDOW_SIZE) * (FE_FEATURE_SIZE_PER_FRAME);

            /* Shift existing features to create space for new features */
            memmove(totalFeatures, totalFeatures + shiftIndex, featuresToRetain * sizeof(q15_t));

            /* Append the new features */
            memcpy(totalFeatures + featuresToRetain, newFeatures, sizeof(newFeatures));

            /* Copy the features to tvm_if_map */
            for(int i = 0 ; i < (FE_FEATURE_SIZE_PER_FRAME*FE_NUM_FRAME_CONCAT) ; i++)
            {
                if_map[0][i][0][0] = totalFeatures[i];
            }
            
            /* Run Inference */
            tvmgen_default_run(&tvm_if_map, &tvm_of_map);

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
                ADC_buffers[prodBufferIndex][dataIndex++] = (gADCresult - 2048) << 4;
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
