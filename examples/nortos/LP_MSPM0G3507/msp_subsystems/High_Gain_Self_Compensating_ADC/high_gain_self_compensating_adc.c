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
//////////////////////////////////////////////////////////////////////////////

#include "ti_msp_dl_config.h"
#include "dac_init.h"
#include "remove_dc_voltage.h"
#include "cascade_input_voltage.h"
#include "saturation_control.h"
#include "customizable_features.h"

/**** Global Variable Local Declarations **************************************/

/* ADC & Timer Flags initialization */
volatile bool gCheckADC_0 =false, gCheckADC_1 =false; 
volatile bool gTIMERA_1_Timeout =false;  

/**** Global Variable Extern Declarations **************************************/
extern uint16_t gOPA1_output;          

/**** Local Function Prototypes ***********************************************/
static inline void sendUARTData (void);

/**** Local Variables ***********************************************/

/* Storing Cascade Gained Input Voltages */
uint32_t cascade_gained_input_voltage;
uint32_t Cascade_Gained_Input_Data_buffer[DATALOGGING_LENGTH] = {0};
unsigned int Data_pointer = 0;

/* Data for UART to transmit */
uint8_t txPacket[UART_PACKET_SIZE] = {};

 int main(void){
/**** Beginning of hardware initialization ***********************************/

    /* Initialize Hardware */
    SYSCFG_DL_init();

    /* Initialize interrupts and exceptions via the NVIC */
    NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
    NVIC_EnableIRQ(ADC12_1_INST_INT_IRQN); 
    
/**** End of hardware initialization *****************************************/

/**** Beginning of software initialization ***********************************/

    /* Initialize DAC */
    dac_init();
/**** End of software initialization *****************************************/

    /* Start TimerA */
    DL_TimerA_startCounter(TIMER_1_INST);

    while(1){
        /* Self-Compensating Functions */     
        remove_dc_voltage();                                                     
        cascade_gained_input_voltage = cascade_input_voltage(gOPA1_output);       
        saturation_control(gOPA1_output);                                         
        
        /* Transmit Cascade Gained Data over UART */
        sendUARTData();

        /* Increment pointer but not looping */
        Data_pointer += 1;
        if(Data_pointer == DATALOGGING_LENGTH){Data_pointer = DATALOGGING_LENGTH-1;}  

        /* TIMERA_1 time delay for sampling interval:  50ms */            
        while (false == gTIMERA_1_Timeout) {
               __WFI();
        }
        gTIMERA_1_Timeout = false;
    }
}

static inline void sendUARTData (void){
        Cascade_Gained_Input_Data_buffer[Data_pointer] = cascade_gained_input_voltage;
        txPacket[0] = Cascade_Gained_Input_Data_buffer[Data_pointer];// Lower Order byte
        txPacket[1] = Cascade_Gained_Input_Data_buffer[Data_pointer] >> 8;
        txPacket[2] = Cascade_Gained_Input_Data_buffer[Data_pointer] >> 16;
        txPacket[3] = Cascade_Gained_Input_Data_buffer[Data_pointer] >> 24; // Higher Order Byte

        /*Fills TX FIFO with data and transmits the data*/
        DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], UART_PACKET_SIZE);

        /*Wait until all bytes have been transmitted and the TX FIFO is empty*/
        while (DL_UART_Main_isBusy(UART_0_INST))
            ;
}

void TIMER_1_INST_IRQHandler(void)
{
    switch (DL_TimerA_getPendingInterrupt(TIMER_1_INST)) {
        case DL_TIMER_IIDX_ZERO:
            gTIMERA_1_Timeout = true;
            break;
        default:
            break;
    }
}

void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            gCheckADC_0 = true;
           DL_ADC12_disableConversions(ADC12_0_INST);
            break;
        default:
            break;
    }
}

void ADC12_1_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_1_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            gCheckADC_1 = true;
            DL_ADC12_disableConversions(ADC12_1_INST);
            break;
        default:
            break;
    }
}