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

/* Includes */
#include "npu.h"
#include <ti/driverlib/m0p/dl_interrupt.h>
#include "ti_msp_dl_config.h"
#include "feature_extract.h"

/* Artifacts */
#include "model/tvmgen_default.h"

/* Includes for DAP */
#include "uart_comm_target.h"
#include "dap_core.h"

/* Macro Definitions */
#define ADC_SAMPLE_SIZE (FE_FRAME_SIZE)
#define FE_FEATURE_SIZE (FE_FRAME_SIZE)
#define FE_FRAME_SKIP   (1U)

/* When FIFO is enabled 2 samples are compacted in a single word */
#define ADC_FIFO_SAMPLES (ADC_SAMPLE_SIZE / 2)
#define NUM_FRAME_CONNCAT (8)
#define INFERENCE_FRAME_THRESHOLD_FOR_ARC 5

/* Global Variables */

/* ADC Ping-Pong Buffers */
int16_t gADCSamplesPing[ADC_SAMPLE_SIZE];
int16_t gADCSamplesPong[ADC_SAMPLE_SIZE];

/* Global Variable to store input to the Model */
volatile int8_t gIF_Map[FE_FEATURE_SIZE];

/* Global variable to store Model output */
volatile int8_t gOF_Map[1][2] = {0, 0};

struct tvmgen_default_inputs tvm_if_map = { (void*) &gIF_Map[0] };
struct tvmgen_default_outputs tvm_of_map = { (void*) &gOF_Map[0] };

/* Buffer to hold newly extracted features in 8-bit integer format.*/
volatile int8_t gNewFeatures[FE_FEATURE_SIZE_PER_FRAME];

/* Buffer for storing intermediate samples during the feature extraction process */
volatile int16_t gNewFESamples[FE_FEATURE_SIZE_PER_FRAME];

/*Flag set by an ADC interrupt when new sample data is ready to be processed.*/
volatile bool gCheckADC;

/*Flag used to manage a ping-pong buffer system, typically toggled by an ISR*/
volatile bool gPing;

/*Circular buffer to store the last 8 predictions from the neural network.
  Each prediction is 0 (normal) or 1 (arc fault).*/
volatile uint8_t gPredVector[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/*Write pointer for the circular prediction buffer.*/
volatile uint8_t gPredPtr;

/* Running sum of the values in the 'pred' buffer, used for a voting-based
   arc fault detection algorithm.*/
volatile uint16_t gPredSum;

/* DAP Variables */
UART_Instance gUART;

/* Stores index of the detected class */
volatile int8_t detectedClass = -1;


/* Function to adjust sensitivity of post AI/ML inference tripping */
int trip_logic(int inf)
{
    /* determines at what point a prediction is considered arcing */
    int inf_thresh = 0;
    int frame_thresh = INFERENCE_FRAME_THRESHOLD_FOR_ARC;
    
    /* circular buffer for pred */
    gPredSum -= gPredVector[gPredPtr];
    if (inf >= inf_thresh)
    {
      gPredVector[gPredPtr] = 1;
      gPredSum += 1;
    }
    else
    {
      gPredVector[gPredPtr] = 0;
    }

    gPredPtr = (gPredPtr + 1) % 8;

    if (gPredSum > frame_thresh)
    {
      return 1;
    }
    else
    {
      return 0;
    }
    return 0;
}

/* Scales ADC samples and converts them to Q15 format */
void adc_q15_scale(int16_t *samples)
{
    int16_t shift = 2048;
    for(int i = 0; i < ADC_SAMPLE_SIZE; ++i)
	{
        samples[i] = ((samples[i] - shift) << 4); // 2^11 
    }
}

/**
 * main.c
 */
int main(void)
{
	/* Initialize local and global variables*/
	volatile int16_t *newSamples;
	gPing = true;
	gCheckADC = false;
	gPredPtr = 0;
	gPredSum = 0;

    /* Initialize the device for 80MHz operation from SYSPLL. */
    SYSCFG_DL_init();

	NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
    NVIC_EnableIRQ(DMA_INT_IRQn);
    UART_init(&gUART);


    /* Configure DMA source, destination and size */
	DL_DMA_setSrcAddr(DMA, DMA_CH2_CHAN_ID, (uint32_t)DL_ADC12_getFIFOAddress(ADC12_0_INST));
	DL_DMA_setDestAddr(DMA, DMA_CH2_CHAN_ID, (uint32_t)&gADCSamplesPing[0]);
	DL_DMA_setTransferSize(DMA, DMA_CH2_CHAN_ID, ADC_FIFO_SAMPLES);
	DL_DMA_enableChannel(DMA, DMA_CH2_CHAN_ID);

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

	/* Initialize the feature extaction library */
	FE_init();

    DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);

	/* The ADC is configured for Repeat Single Conversion,
     * so the ADC will continue until DL_ADC12_disableConversions() is called */
    DL_ADC12_startConversion(ADC12_0_INST);

	/* Enter execution loop - acquire data, pre-process, and run inference */
    while (1)
    {
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
            if(gPipelineConfig.mode == PIPELINE_MODE_SENSOR_INFERENCE)
            {
                int8_t modelOutput[3];
                modelOutput[0] = detectedClass;
                modelOutput[1] = (int8_t) gOF_Map[0][0] ;
                modelOutput[2] = (int8_t) gOF_Map[0][1] ;

                DAP_receivedDataResponse(&gUART,Resp_CMD_Channel_inferencing_value,3, modelOutput);
            }
        }

        /* start of half AC cycle */
        while (false == gCheckADC) 
		{
          	__WFE();
        }
        
		/* The data will be alternating between each buffer */
		/* Switch to send ADC Results to Pong Buffer */
        if (gPing == true) 
		{
			newSamples = gADCSamplesPing;
			DL_DMA_setDestAddr(DMA, DMA_CH2_CHAN_ID, (uint32_t)&gADCSamplesPong[0]);
			DL_DMA_setTransferSize(DMA, DMA_CH2_CHAN_ID, ADC_FIFO_SAMPLES);
			DL_DMA_enableChannel(DMA, DMA_CH2_CHAN_ID);
			DL_ADC12_enableDMA(ADC12_0_INST);

          	gPing = false;
        }
        /* Switch to send ADC Results to Ping Buffer */
        else 
		{
          	newSamples = gADCSamplesPong;
          	DL_DMA_setDestAddr(DMA, DMA_CH2_CHAN_ID, (uint32_t)&gADCSamplesPing[0]);
          	DL_DMA_setTransferSize(DMA, DMA_CH2_CHAN_ID, ADC_FIFO_SAMPLES);
          	DL_DMA_enableChannel(DMA, DMA_CH2_CHAN_ID);
          	DL_ADC12_enableDMA(ADC12_0_INST);

          	gPing = true;
        }
        gCheckADC = false;

        /* Convert unsigned ADC samples to signed q15 format */
		adc_q15_scale(newSamples);

		/* Run Feature extraction on the new captured samples */
		FE_process(newSamples, gNewFeatures,0);

		/* Shift the existing features by sliding window size and append new features */
		uint32_t numFeaturestoShift = ((FE_NUM_FRAME_CONCAT - FE_FRAME_SKIP) * (FE_FEATURE_SIZE_PER_FRAME));
        uint32_t index_FeatureShift = (FE_FRAME_SKIP) * (FE_FEATURE_SIZE_PER_FRAME);

		memmove(gIF_Map, gIF_Map + index_FeatureShift , numFeaturestoShift * sizeof(int8_t));
        memcpy(gIF_Map + numFeaturestoShift, gNewFeatures , sizeof(gNewFeatures));

        /* Copy the features to tvm_if_map */
		/* Run Inference on the trained model using NPU */
        tvmgen_default_run(&tvm_if_map, &tvm_of_map);

        while (!tvmgen_default_finished)
		{
            __WFE();
        }

        int norm= gOF_Map[0][1];
        int arc= gOF_Map[0][0];

        if(trip_logic(arc-norm))
        {
            detectedClass = 0;
            /* if an arc is detected set GPIO high */
            DL_GPIO_setPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
        }
        else
        {
            detectedClass = 1;
            /* if an arc is not detected set GPIO low */
            DL_GPIO_clearPins(GPIO_GRP_0_PORT, GPIO_GRP_0_PIN_0_PIN);
        }
	}

	return 0;
}

void ADC12_0_INST_IRQHandler(void)
{
	switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST))
	{
        case DL_ADC12_IIDX_DMA_DONE:
            gCheckADC = true;
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
