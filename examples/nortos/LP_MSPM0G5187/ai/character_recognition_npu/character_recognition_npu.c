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


/*
 * @brief Define to 1 when a compiled model is added to the project.
 */
#define MODEL_AVAILABLE (1)
#if MODEL_AVAILABLE == (0)
#warning A compiled model must be added to this project - see the README file.
#endif


#include <ti/npu/npu.h>
#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <string.h>

#if MODEL_AVAILABLE == (1)
#include "model/tvmgen_default.h"
#endif

#define INPUT_FEATURE_SIZE 784      /* 28x28 size of image is transfered as linear 1D array*/

#define NUM_CLASSES 10      /* 10 is the number of output digit classes from 0-9"*/

#define CPU_CLK_FREQ_KHZ 80000      /*CPU clock frequency in KHz using it to log the inference time */

/* Index for the byte buffer */
volatile uint8_t byteIndex;

/* Index for the float buffer */
volatile uint32_t nIndex;

/* Output feature map array */
volatile float of_map[1][10];

/* Input feature map array */
volatile float uartRxBuffer[INPUT_FEATURE_SIZE];

/* Timer reading variable for inference start time and end time */
volatile uint32_t time1;
volatile uint32_t time2;

/* Timer reading variable for for full processing start time and end time */
volatile uint32_t time3;
volatile uint32_t time4;

/**
 * @brief A union for converting between a 32-bit float and a 4-byte array.
 *
 * This is used for serializing and deserializing floating-point numbers
 * for communication over UART.
 */
typedef union {
    float floatValue;        /**< 32-bit floating-point representation. */
    uint8_t byteValue[sizeof(float)]; /**< 4-byte array representation for serialization. */
} FloatConverter;

/**
 * @enum UART_Status
 * @brief Enumeration for tracking the state of the UART communication.
 */
typedef enum
{
    /*! UART is idle, waiting for a new frame. */
    UART_STATUS_IDLE = 0,
    /*! Currently receiving a frame of data via UART. */
    UART_STATUS_RX_BUFFERING,
    /*! A full frame has been received and is ready for processing. */
    UART_STATUS_PROCESS_FRAME,
    /*! The inference result is being transmitted via UART. */
    UART_STATUS_TX_TRANSMITTING,
} UART_Status;

/* Volatile union for converting received UART bytes to a float. */
volatile FloatConverter rxFloatData;

/* Volatile union for converting a float to bytes for UART transmission. */
volatile FloatConverter txFloatData;

/* volatile variable to track the current state of the UART communication */
volatile UART_Status status;

/**
 * @brief Executes the inference model on the input data.
 *
 * This function prepares the input data, runs the TVM-generated model,
 * and sets the UART status to indicate that the results are ready to be sent.
 * It also records the start and end times for the inference process using SysTick.
 */
void runInference(){

    /* 
     * Clear and enable the NPU interrupt.
     */
    while (!(DL_SYSCTL_getStatus() & DL_SYSCTL_STATUS_NPU_READY));
    DL_NPU_clearInterruptStatus(NPU, DL_NPU_INTERRUPT_DONE);
    DL_NPU_enableInterrupt(NPU, DL_NPU_INTERRUPT_DONE);
    NVIC_EnableIRQ(NPU_INT_IRQn);

    /*
     * Enter execution loop - acquire data and run inference
     */
    struct tvmgen_default_inputs tvm_if_map = {(void *) &uartRxBuffer[0]};
    struct tvmgen_default_outputs tvm_of_map = {(void *) &of_map[0]};

    time1 = SysTick->VAL;
    tvmgen_default_run(&tvm_if_map,&tvm_of_map);
    time2 = SysTick->VAL;

    while(!tvmgen_default_finished);

    status = UART_STATUS_TX_TRANSMITTING;

    DL_NPU_disableInterrupt(NPU, DL_NPU_INTERRUPT_DONE);

    return;
}


/**
 * @brief Executes the inference model on the input data.
 *
 * This function prepares the input data, runs the TVM-generated model,
 * and sets the UART status to indicate that the results are ready to be sent.
 * It also records the start and end times for the inference process using SysTick.
 */
void uart_send(){
    for (int i = 0; i < NUM_CLASSES ; i++) {
        txFloatData.floatValue = of_map[0][i];

        for(int j=0;j<4;j++){

            DL_UART_Main_clearInterruptStatus( UART_0_INST, DL_UART_MAIN_INTERRUPT_EOT_DONE);

            /* Waits until FIFO has space to fill the data */
            while (DL_UART_Main_isTXFIFOFull(UART_0_INST)) {
            }
            DL_UART_Main_transmitData(UART_0_INST, txFloatData.byteValue[j]);

        }
    }

    txFloatData.floatValue = (float)(time1-time2)/(CPU_CLK_FREQ_KHZ);
    for(int j=0;j<4;j++){

        DL_UART_Main_clearInterruptStatus( UART_0_INST, DL_UART_MAIN_INTERRUPT_EOT_DONE);

        /* Waits until FIFO has space to fill the data */
        while (DL_UART_Main_isTXFIFOFull(UART_0_INST)) {
        }
        DL_UART_Main_transmitData(UART_0_INST, txFloatData.byteValue[j]);

    }

    txFloatData.floatValue = (float)(time3-time4)/(CPU_CLK_FREQ_KHZ);
    for(int j=0;j<4;j++){
        DL_UART_Main_clearInterruptStatus( UART_0_INST, DL_UART_MAIN_INTERRUPT_EOT_DONE);

        /* Waits until FIFO has space to fill the data */
        while (DL_UART_Main_isTXFIFOFull(UART_0_INST)) {
        }
        DL_UART_Main_transmitData(UART_0_INST, txFloatData.byteValue[j]);

    }

    status = UART_STATUS_IDLE;

    return ;
}

int main(void)
{

    SYSCFG_DL_init();

    /* Power up the neural processing unit (NPU) module. */
    DL_NPU_reset(NPU);
    DL_NPU_enablePower(NPU);

    /* Clear and enable the UART interrupt. */
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);


    /* Set the SysTick timer period to its maximum value. */
    uint32_t systick_period = 0x00FFFFFF;


    status = UART_STATUS_IDLE;
    byteIndex = 0;
    nIndex =0;
    while(1){

        if(status == UART_STATUS_PROCESS_FRAME){
            DL_SYSTICK_init(systick_period);
            DL_SYSTICK_enable();
            time3 = SysTick->VAL;
            /* Run the inference function. */
            runInference();
            time4 = SysTick->VAL;
        }

        /* If the inference is complete, transmit the results. */
        if(status == UART_STATUS_TX_TRANSMITTING){
            uart_send();
        }

    }

    return 0;

}


/**
 * @brief UART interrupt handler for receiving data.
 *
 * This handler reads incoming bytes, assembles them into 32-bit floats,
 * and fills the input buffer. When the buffer is full, it signals the
 * main loop to start processing the frame.
 */
void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:

            if(status == UART_STATUS_IDLE){
                status = UART_STATUS_RX_BUFFERING;
            }

            DL_UART_Main_clearInterruptStatus(UART_0_INST,DL_UART_MAIN_INTERRUPT_RX);

            /* Read the received byte and store it for float conversion. */
            rxFloatData.byteValue[byteIndex++] = DL_UART_Main_receiveData(UART_0_INST);

            /* If 4 bytes have been received, convert them to a float and store it. */
            if (byteIndex >= 4) {
                uartRxBuffer[nIndex] = rxFloatData.floatValue;
                byteIndex = 0;
                nIndex++;
            }

            /* If the input buffer is full (784 floats), signal to process the frame. */
            if(nIndex == INPUT_FEATURE_SIZE){
                status = UART_STATUS_PROCESS_FRAME;
                nIndex = 0;
            }

            break;
        default:
            break;
    }
}
