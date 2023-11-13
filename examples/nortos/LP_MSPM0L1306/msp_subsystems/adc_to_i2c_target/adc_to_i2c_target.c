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

/* Maximum size of TX packet */
#define I2C_TX_MAX_PACKET_SIZE (16)

/* Maximum size of RX packet */
#define I2C_RX_MAX_PACKET_SIZE (16)

/* Commands for the ADC sampling */
#define SAMPLE_ADC (0x01)
#define SEND_RESULTS (0x02)
#define SAMPLE_SEND_RESULTS (0x03)

/* Data sent to Controller in response to Read transfer, default set to 0xAA */
uint8_t gTxPacket[I2C_TX_MAX_PACKET_SIZE] = {0xAA};

/* Counters for TX length and bytes sent */
uint32_t gTxLen, gTxCount;

/* Data received from Controller during a Write transfer */
uint8_t gRxPacket[I2C_RX_MAX_PACKET_SIZE];

/* Counters for TX length and bytes sent */
uint32_t gRxLen, gRxCount, gCommandCount;

/* Boolean to know when a stop command was issued */
bool gStopReceived = false;

/* ADC variables:
 * gADCResult is the adc result taken from the Memory Register
 * NumberOfADCSamples is the count of total ADC samples to keep track for the gTxResult buffer
 */

uint16_t gADCResult;
uint8_t gNumberOfADCSamples = 0;
bool gCheckADC = false;

/* Process Command will take the command from the I2C Rx and parse which command is to be issued */
void processCommand(uint8_t cmd);

/* ADC Process will start the ADC and separate the ADC result into 8 bits to place into the Tx Packet buffer. */
void ADCProcess();

int main(void)
{
    SYSCFG_DL_init();

    /* Set LED to indicate start of transfer */
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

   /* Reset variables for the Tx, these are used in the I2C ISR */
    gTxCount = 0;
    gTxLen   = 0;
    DL_I2C_enableInterrupt(I2C_INST, DL_I2C_INTERRUPT_TARGET_TXFIFO_TRIGGER);

    /* Initialize variables to receive data inside RX ISR */
    gRxCount = 0;
    gRxLen   = I2C_RX_MAX_PACKET_SIZE;

    NVIC_EnableIRQ(I2C_INST_INT_IRQN);
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);


    while (1)
    {
        /* Sleep until the stop command is given from the I2C Controller */
        while(gStopReceived == false)
            __WFE();
        gStopReceived = false;

        /* Process given commands */
        for(uint16_t i = 0; i < gCommandCount; i++){
            processCommand(gRxPacket[i]);
        }
        gCommandCount = 0;
    }
}

/* Process the command given */
void processCommand(uint8_t cmd){

    switch(cmd){
        /* Sample the ADC */
        case SAMPLE_ADC:
            ADCProcess();
            break;
        /* Add 2 to the Tx Length, this equates to one more full result added to the TX transmission */
        case SEND_RESULTS:
            gTxLen += 2;
            break;
        /* Sample the ADC and increment the Tx Length. */
        case SAMPLE_SEND_RESULTS:
            ADCProcess();
            gTxLen += 2;
            break;
        default:
            break;
    }
}

/* Start the ADC process of sampling and storing into the TxPacket Buffer */
void ADCProcess(){

    /* Start the ADC Sampling */

    DL_ADC12_startConversion(ADC12_0_INST);

    /* Sleep until the ADC sampling and conversion is finished, then reset the boolean */
    while(gCheckADC == false){
        __WFI();
    }
    gCheckADC = false;

    /* Get the ADC result and place the upper byte and lower byte into the TxPacket, upper byte first */
    gADCResult = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
    gTxPacket[gNumberOfADCSamples*2] = (uint8_t)((gADCResult >> 8) & 0xFF);
    gTxPacket[gNumberOfADCSamples*2 + 1] = (uint8_t)(gADCResult & 0xFF);

    /* Make sure the number of ADC samples does not go over the TxPacket size, if it does next sample will overwrite */
    gNumberOfADCSamples++;
    if(gNumberOfADCSamples*2 + 1 > I2C_TX_MAX_PACKET_SIZE)
    {
        gNumberOfADCSamples = 0;
    }
    DL_ADC12_enableConversions(ADC12_0_INST);

}


void ADC12_0_INST_IRQHandler(void) {
  switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
  case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
    gCheckADC = true;
    break;
  default:
    break;
  }
}


void I2C_INST_IRQHandler(void)
{
    static bool commandReceived = false;

    switch (DL_I2C_getPendingInterrupt(I2C_INST)) {
        case DL_I2C_IIDX_TARGET_START:
            /* Initialize RX or TX after Start condition is received */
            gTxCount = 0;
            gRxCount = 0;
            commandReceived   = false;
            /* Flush TX FIFO to refill it */
            DL_I2C_flushTargetTXFIFO(I2C_INST);
            break;
        case DL_I2C_IIDX_TARGET_RXFIFO_TRIGGER:
            /* Store received commands in buffer */
            commandReceived = true;
            while (DL_I2C_isTargetRXFIFOEmpty(I2C_INST) != true) {
                if (gRxCount < gRxLen) {
                    gRxPacket[gRxCount++] = DL_I2C_receiveTargetData(I2C_INST);
                } else {
                    /* Prevent overflow and just ignore data */
                    DL_I2C_receiveTargetData(I2C_INST);
                }
            }
            break;
        case DL_I2C_IIDX_TARGET_TXFIFO_TRIGGER:
            /* Fill TX FIFO with ADC Results */
            if (gTxCount < gTxLen) {
                gTxCount += DL_I2C_fillTargetTXFIFO(
                    I2C_INST, &gTxPacket[gTxCount], (gTxLen - gTxCount));
            } else {
                /*
                 * Fill FIFO with 0xAA if more data is requested than
                 * expected gTxLen
                 */
                while (DL_I2C_transmitTargetDataCheck(I2C_INST, 0xAA) != false)
                    ;
            }
            break;
        case DL_I2C_IIDX_TARGET_STOP:
            /* If commands were received, set the command count to RxCount and reset fifo */
            if (commandReceived == true) {
                gCommandCount = gRxCount;
                commandReceived = false;
                DL_I2C_flushTargetTXFIFO(I2C_INST);
            }

            /* Set Stop Received to true to allow the main loop to proceed */
            gStopReceived = true;
            /* Toggle LED to indicate successful RX or TX */
            DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
            break;
        default:
            break;
    }
}
