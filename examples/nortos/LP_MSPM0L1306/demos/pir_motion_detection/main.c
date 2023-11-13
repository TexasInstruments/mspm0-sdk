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

#include "remove_pir_dc_voltage.h"
#include "cascade_input_voltage.h"
#include "motion_detection.h"
#include "saturation_control.h"
#include "despiking_derippling.h"
#include "bsp.h"

/* Buffers for Datalogging of PIR Signal*/

uint32_t PIR_Despiked_Data_buffer[PIR_DATALOGGING_LENGTH] = {0};

uint32_t PIR_Cascade_Gained_Input_Data_buffer[PIR_DATALOGGING_LENGTH] = {0};

uint32_t PIR_Inst_Detn_th_val[PIR_DATALOGGING_LENGTH] = {0};

uint32_t PIR_abs_inst_dev_value[PIR_DATALOGGING_LENGTH] = {0};

uint32_t PIR_motion_detection_logging[PIR_DATALOGGING_LENGTH] = {0};


unsigned int PIR_Data_pointer = 0;

/**** Global Variable Local Declarations **************************************/
//

volatile bool gCheckADC_0,gCheckADC_1;
volatile bool gTIMERG_1_Timeout,gTIMERG_2_Timeout,gTIMERG_3_Timeout;

/* Data for UART to transmit */
// UART FIFO size maximum 4 Byte
#define UART_PACKET_SIZE (4)
uint8_t txPacket[UART_PACKET_SIZE] = {};


/**** Global Variable Extern Declarations **************************************/
//

extern uint16_t DAC8_DATA;
extern uint16_t PWM_DAC5_DATA;
extern uint16_t v_out2_value;           // ADC0.8
extern uint32_t v_out1_value;

/**** Local Function Prototypes ***********************************************/
SYSCONFIG_WEAK void SYSCFG_DL_COMP_0_config(void);

int main(void)
{
    uint32_t i;
    uint32_t gained_cascade_input_voltage;
    uint32_t depeak_input_voltage;

/**** Beginning of hardware initialization ***********************************/

    // Initialize GPIOs and Clock System
    // Initialize AFE ,Vref ,ADC and SAC
    SYSCFG_DL_init();

    DL_COMP_setDACCode0(COMP_0_INST, DAC8_DATA);
    DL_COMP_enable(COMP_0_INST);

    //Initialize interrupts and exceptions via the NVIC.
    NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_2_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_3_INST_INT_IRQN);
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    // Start PWM DAC
    DL_TimerG_startCounter(PWM_0_INST);

    gCheckADC_0 = false;
    gTIMERG_1_Timeout = false;
    gTIMERG_2_Timeout = false;

    /* Confirm VREF has settled before triggering ADC12 conversion */
    while (DL_VREF_CTL1_READY_NOTRDY == DL_VREF_getStatus(VREF))
        ;

    /**** End of hardware initialization *****************************************/

    Red_LED_on;
    Green_LED_off;

    /* Enter Sleep0 */
    //TIMERG_3 time delay:  6s
    for (i=0;i<3;i++) {

        Green_LED_on;

        DL_TimerG_startCounter(TIMER_3_INST);

        DL_SYSCTL_setPowerPolicyRUN0SLEEP0();

        while (false == gTIMERG_3_Timeout) {
              __WFI();
        }

        gTIMERG_3_Timeout = false;

        DL_TimerG_startCounter(TIMER_3_INST);

        DL_SYSCTL_setPowerPolicyRUN0SLEEP0();

        while (false == gTIMERG_3_Timeout) {
              __WFI();
        }

        gTIMERG_3_Timeout = false;

    }

    //DL_TimerG_disablePower(TIMER_3_INST);
    /**** Start of software initialization ***************************************/

    Green_LED_on;
    Red_LED_off;

    remove_pir_dc_voltage();
    Red_LED_on;
    cascade_input_voltage();
    out_sig_sat_ctrl(v_out2_value);

    /**** End of software initialization *****************************************/
    txPacket[0] = PIR_Despiked_Data_buffer[PIR_Data_pointer];// Lower Order byte
    txPacket[1] = PIR_Despiked_Data_buffer[PIR_Data_pointer] >> 8;
    txPacket[2] = PIR_Despiked_Data_buffer[PIR_Data_pointer] >> 16;
    txPacket[3] = PIR_Despiked_Data_buffer[PIR_Data_pointer] >> 24; // Higher Order Byte

    // Fills TX FIFO with data and transmits the data
    DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], UART_PACKET_SIZE);

    // Wait until all bytes have been transmitted and the TX FIFO is empty
    while (DL_UART_Main_isBusy(UART_0_INST))
        ;

    txPacket[0] = PIR_Inst_Detn_th_val[PIR_Data_pointer];// Lower Order byte
    txPacket[1] = PIR_Inst_Detn_th_val[PIR_Data_pointer] >> 8;
    txPacket[2] = PIR_Inst_Detn_th_val[PIR_Data_pointer] >> 16;
    txPacket[3] = PIR_Inst_Detn_th_val[PIR_Data_pointer] >> 24; // Higher Order Byte

    // Fills TX FIFO with data and transmits the data
    DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], UART_PACKET_SIZE);

    // Wait until all bytes have been transmitted and the TX FIFO is empty
    while (DL_UART_Main_isBusy(UART_0_INST))
        ;

    txPacket[0] = PIR_abs_inst_dev_value[PIR_Data_pointer];// Lower Order byte
    txPacket[1] = PIR_abs_inst_dev_value[PIR_Data_pointer] >> 8;
    txPacket[2] = PIR_abs_inst_dev_value[PIR_Data_pointer] >> 16;
    txPacket[3] = PIR_abs_inst_dev_value[PIR_Data_pointer] >> 24; // Higher Order Byte

    // Fills TX FIFO with data and transmits the data
    DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], UART_PACKET_SIZE);

    // Wait until all bytes have been transmitted and the TX FIFO is empty
    while (DL_UART_Main_isBusy(UART_0_INST))
        ;


    txPacket[0] = PWM_DAC5_DATA;// Lower Order byte
    txPacket[1] = PWM_DAC5_DATA >> 8;
    txPacket[2] = PWM_DAC5_DATA >> 16;
    txPacket[3] = PWM_DAC5_DATA >> 24; // Higher Order Byte

    // Fills TX FIFO with data and transmits the data
    DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], UART_PACKET_SIZE);

    // Wait until all bytes have been transmitted and the TX FIFO is empty
    while (DL_UART_Main_isBusy(UART_0_INST))
        ;

    txPacket[0] = v_out1_value;// Lower Order byte
    txPacket[1] = v_out1_value >> 8;
    txPacket[2] = v_out1_value >> 16;
    txPacket[3] = v_out1_value >> 24; // Higher Order Byte

    // Fills TX FIFO with data and transmits the data
    DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], UART_PACKET_SIZE);

    // Wait until all bytes have been transmitted and the TX FIFO is empty
    while (DL_UART_Main_isBusy(UART_0_INST))
        ;


    txPacket[0] = DAC8_DATA;// Lower Order byte
    txPacket[1] = DAC8_DATA >> 8;
    txPacket[2] = DAC8_DATA >> 16;
    txPacket[3] = DAC8_DATA >> 24; // Higher Order Byte

    // Fills TX FIFO with data and transmits the data
    DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], UART_PACKET_SIZE);

    // Wait until all bytes have been transmitted and the TX FIFO is empty
    while (DL_UART_Main_isBusy(UART_0_INST))
        ;

    txPacket[0] = v_out2_value;// Lower Order byte
    txPacket[1] = v_out2_value >> 8;
    txPacket[2] = v_out2_value >> 16;
    txPacket[3] = v_out2_value >> 24; // Higher Order Byte

    // Fills TX FIFO with data and transmits the data
    DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], UART_PACKET_SIZE);

    // Wait until all bytes have been transmitted and the TX FIFO is empty
    while (DL_UART_Main_isBusy(UART_0_INST))
        ;


    /* Shut down power of all analog peripherals and disable all all analog peripherals module */
    DL_VREF_reset(VREF);
    DL_OPA_reset(OPA_0_INST);
    DL_OPA_reset(OPA_1_INST);

    DL_TimerG_stopCounter(PWM_0_INST);

    DL_VREF_disablePower(VREF);
    DL_COMP_disable(COMP_0_INST);
    DL_COMP_disablePower(COMP_0_INST);
    DL_OPA_disablePower(OPA_0_INST);
    DL_OPA_disablePower(OPA_1_INST);
    DL_GPAMP_disable();
    DL_TimerG_disablePower(PWM_0_INST);
    DL_TimerG_disablePower(TIMER_2_INST);

    DL_TimerG_startCounter(TIMER_1_INST);

    Red_LED_off;
    Green_LED_off;

    while(1) {

        //TIMERG_1 time delay:  50ms
        while (false == gTIMERG_1_Timeout) {
               __WFI();
           }

        gTIMERG_1_Timeout = false;

        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        /*
         * power up PWM-DAC
         */
        DL_TimerG_enablePower(PWM_0_INST);
        DL_TimerG_startCounter(PWM_0_INST);
        DL_GPAMP_enable();

        /* Power up all analog peripherals and enable all all analog peripherals module, before PIR signal procedure */
        DL_VREF_enablePower(VREF);
        SYSCFG_DL_VREF_init();
        DL_COMP_enablePower(COMP_0_INST);
        DL_COMP_setDACCode0(COMP_0_INST, DAC8_DATA);
        DL_COMP_enable(COMP_0_INST);
        DL_OPA_enablePower(OPA_0_INST);
        DL_OPA_enablePower(OPA_1_INST);
        SYSCFG_DL_OPA_0_init();
        SYSCFG_DL_OPA_1_init();
        DL_TimerG_enablePower(TIMER_2_INST);
        DL_ADC12_enablePower(ADC12_0_INST);

        /* Enter Sleep0 */
        DL_TimerG_startCounter(TIMER_2_INST);

        DL_SYSCTL_setPowerPolicyRUN0SLEEP0();
        //TIMERG_2 time delay:  40us
        while (false == gTIMERG_2_Timeout) {
              __WFI();
        }

        gTIMERG_2_Timeout = false;
        
        remove_pir_dc_voltage();
        
        /* Shut down power of all analog peripherals and disable all all analog peripherals module */
        DL_TimerG_stopCounter(PWM_0_INST);
        DL_TimerG_disablePower(PWM_0_INST);

        //DL_GPIO_disablePower(GPIOA);
        DL_ADC12_disablePower(ADC12_0_INST);
        DL_VREF_disablePower(VREF);
        DL_COMP_disable(COMP_0_INST);
        DL_COMP_disablePower(COMP_0_INST);
        DL_OPA_disablePower(OPA_0_INST);
        DL_OPA_disablePower(OPA_1_INST);
        DL_GPAMP_disable();
        DL_TimerG_disablePower(TIMER_2_INST);

        DL_VREF_reset(VREF);
        DL_OPA_reset(OPA_0_INST);
        DL_OPA_reset(OPA_1_INST);

        gained_cascade_input_voltage = cascade_input_voltage();

        PIR_Cascade_Gained_Input_Data_buffer[PIR_Data_pointer] = gained_cascade_input_voltage;  // Just testing the datalogging

        out_sig_sat_ctrl(v_out2_value);


        depeak_input_voltage = despiking_derippling_calc(gained_cascade_input_voltage);

        PIR_Despiked_Data_buffer[PIR_Data_pointer] = depeak_input_voltage;

        signal_analysis(depeak_input_voltage);

        txPacket[0] = PIR_Despiked_Data_buffer[PIR_Data_pointer];// Lower Order byte
        txPacket[1] = PIR_Despiked_Data_buffer[PIR_Data_pointer] >> 8;
        txPacket[2] = PIR_Despiked_Data_buffer[PIR_Data_pointer] >> 16;
        txPacket[3] = PIR_Despiked_Data_buffer[PIR_Data_pointer] >> 24; // Higher Order Byte

        // Fills TX FIFO with data and transmits the data
        DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], UART_PACKET_SIZE);

        // Wait until all bytes have been transmitted and the TX FIFO is empty
        while (DL_UART_Main_isBusy(UART_0_INST))
            ;

        txPacket[0] = PIR_Inst_Detn_th_val[PIR_Data_pointer];// Lower Order byte
        txPacket[1] = PIR_Inst_Detn_th_val[PIR_Data_pointer] >> 8;
        txPacket[2] = PIR_Inst_Detn_th_val[PIR_Data_pointer] >> 16;
        txPacket[3] = PIR_Inst_Detn_th_val[PIR_Data_pointer] >> 24; // Higher Order Byte

        // Fills TX FIFO with data and transmits the data
        DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], UART_PACKET_SIZE);

        // Wait until all bytes have been transmitted and the TX FIFO is empty
        while (DL_UART_Main_isBusy(UART_0_INST))
            ;

        txPacket[0] = PIR_abs_inst_dev_value[PIR_Data_pointer];// Lower Order byte
        txPacket[1] = PIR_abs_inst_dev_value[PIR_Data_pointer] >> 8;
        txPacket[2] = PIR_abs_inst_dev_value[PIR_Data_pointer] >> 16;
        txPacket[3] = PIR_abs_inst_dev_value[PIR_Data_pointer] >> 24; // Higher Order Byte

        // Fills TX FIFO with data and transmits the data
        DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], UART_PACKET_SIZE);

        // Wait until all bytes have been transmitted and the TX FIFO is empty
        while (DL_UART_Main_isBusy(UART_0_INST))
            ;

        txPacket[0] = PWM_DAC5_DATA;// Lower Order byte
        txPacket[1] = PWM_DAC5_DATA >> 8;
        txPacket[2] = PWM_DAC5_DATA >> 16;
        txPacket[3] = PWM_DAC5_DATA >> 24; // Higher Order Byte

        // Fills TX FIFO with data and transmits the data
        DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], UART_PACKET_SIZE);

        // Wait until all bytes have been transmitted and the TX FIFO is empty
        while (DL_UART_Main_isBusy(UART_0_INST))
            ;

        txPacket[0] = v_out1_value;// Lower Order byte
        txPacket[1] = v_out1_value >> 8;
        txPacket[2] = v_out1_value >> 16;
        txPacket[3] = v_out1_value >> 24; // Higher Order Byte

        // Fills TX FIFO with data and transmits the data
        DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], UART_PACKET_SIZE);

        // Wait until all bytes have been transmitted and the TX FIFO is empty
        while (DL_UART_Main_isBusy(UART_0_INST))
            ;

        txPacket[0] = DAC8_DATA;// Lower Order byte
        txPacket[1] = DAC8_DATA >> 8;
        txPacket[2] = DAC8_DATA >> 16;
        txPacket[3] = DAC8_DATA >> 24; // Higher Order Byte

        // Fills TX FIFO with data and transmits the data
        DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], UART_PACKET_SIZE);

        // Wait until all bytes have been transmitted and the TX FIFO is empty
        while (DL_UART_Main_isBusy(UART_0_INST))
            ;

        txPacket[0] = v_out2_value;// Lower Order byte
        txPacket[1] = v_out2_value >> 8;
        txPacket[2] = v_out2_value >> 16;
        txPacket[3] = v_out2_value >> 24; // Higher Order Byte

        // Fills TX FIFO with data and transmits the data
        DL_UART_Main_fillTXFIFO(UART_0_INST, &txPacket[0], UART_PACKET_SIZE);

        // Wait until all bytes have been transmitted and the TX FIFO is empty
        while (DL_UART_Main_isBusy(UART_0_INST))
            ;

        PIR_Data_pointer += 1;
        if(PIR_Data_pointer == PIR_DATALOGGING_LENGTH){PIR_Data_pointer = PIR_DATALOGGING_LENGTH-1;}  // Not Looping

    }
}



/* COMP_0 Initialization */
static const DL_COMP_Config gCOMP_0Config = {
    .channelEnable = DL_COMP_ENABLE_CHANNEL_NONE,
    .mode          = DL_COMP_MODE_FAST,
    .negChannel    = DL_COMP_IMSEL_CHANNEL_0,
    .posChannel    = DL_COMP_IPSEL_CHANNEL_0,
    .hysteresis    = DL_COMP_HYSTERESIS_NONE,
    .polarity      = DL_COMP_POLARITY_NON_INV
};
static const DL_COMP_RefVoltageConfig gCOMP_0VRefConfig = {
    .mode           = DL_COMP_REF_MODE_STATIC,
    .source         = DL_COMP_REF_SOURCE_VDDA_DAC,
    .terminalSelect = DL_COMP_REF_TERMINAL_SELECT_NEG,
    .controlSelect  = DL_COMP_DAC_CONTROL_SW,
    .inputSelect    = DL_COMP_DAC_INPUT_DACCODE0
};

SYSCONFIG_WEAK void SYSCFG_DL_COMP_0_config(void)
{
    DL_COMP_init(COMP_0_INST, (DL_COMP_Config *) &gCOMP_0Config);
    DL_COMP_refVoltageInit(COMP_0_INST, (DL_COMP_RefVoltageConfig *) &gCOMP_0VRefConfig);
    DL_COMP_setDACCode0(COMP_0_INST, DAC8_DATA);

    DL_COMP_enable(COMP_0_INST);

}

void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM2_RESULT_LOADED:
            gCheckADC_0 = true;
            DL_ADC12_disableConversions(ADC12_0_INST);
            break;
        default:
            break;
    }

}


void TIMER_1_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_1_INST)) {
        case DL_TIMER_IIDX_ZERO:
            gTIMERG_1_Timeout = true;

            break;
        default:
            break;
    }
}


void TIMER_2_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_2_INST)) {
        case DL_TIMER_IIDX_ZERO:
            gTIMERG_2_Timeout = true;

            break;
        default:
            break;
    }
}


void TIMER_3_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(TIMER_3_INST)) {
        case DL_TIMER_IIDX_ZERO:
            DL_TimerG_stopCounter(TIMER_3_INST);

            gTIMERG_3_Timeout = true;

            Green_LED_off;

            break;
        default:
            break;
    }
}
