/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
//*****************************************************************************
// Pulse oximeter demo
//
// Texas Instruments, Inc.
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include <ti/gui_composer/HAL.h>
#include <ti/gui_composer/IQMathLib/QmathLib.h>
#include "MSP_GUI/GUIComm_mpack.h"
#include "MSP_GUI/GUI_mpack.h"
#include "callbacks_mpack.h"
#include "math.h"
#include "string.h"
#include "ti_msp_dl_config.h"

#define ADC_SAMPLE_SIZE (256)  // Over-sampling factor
#define OVSMP_ARR_SIZE (61)    // Storage array size for over-sampled signal
#define NUM_BITS_OVSMP (4)     // Over-sample by how many bits
#define UART_TX_ARR_SIZE (37)  // 37 - TI GUI
#define BPM_ARR_SIZE (20)
#define SPO2_ARR_SIZE (20)
#define SHTDWN_LIGHT_THRSHLD (60000)
#define VREF_INT (2.5)
#define ADC12BIT_CODES (4096.0)
#define VDD_MONITORING_MLTPR (3)
#define LED_SETTLING_TIME \
    (32000000 * 0.000001 * 700)  // Allow LEDs and PD to settle for 700us

typedef enum {
    // Both LEDs are off */
    BOTH_LEDS_OFF = 0,
    // Red LED only is on */
    RED_LED_ON = 1,
    // IR LED only is on */
    IR_LED_ON = 2,
} LED_STATUS;
typedef enum {
    // Action that happens every second */
    EVERY_1_SEC = 125,
    // Action that happens every 1/5 second */
    EVERY_0s2_SEC = 25,
    // Action that happens every 6 seconds */
    EVERY_6_SEC = 30
} TIMER_ACTION_TIMES;
typedef enum {
    // Wait 2 seconds before rebooting */
    REBOOT_WAIT_TIME = 32000000 * 2,
    // Action that happens every 1/5 second */
    ADC_MEM_SWITCH_WAIT_TIME = (int) (32000000 * 0.0001 * 1),
    // ADC VDD Monitor wait time */
    ADC_VDD_MNTR_WAIT_TIME = (int) (32000000 * 0.001 * 1),
} PROCESS_WAIT_TIMES;

unsigned char RED_BRIGHTNESS =
    0xC9;  // DAC8 output 0x00(low) to 0xFF(high) for RED LED
unsigned char IR_BRIGHTNESS =
    0xC0;  // DAC8 output 0x00(low) to 0xFF(high) for IR LED
unsigned char OFF_BRIGHTNESS = 0xC0;  // DAC8 output 0x00 for LEDs off
unsigned char k =
    20;  // Signal low-pass moving-average filter (averaging "k" values)
float b = 0.01;  // IIR DC tracking filter coefficient

/* RAW data variables */
short ADCSamples[ADC_SAMPLE_SIZE];
int ADCOVSMP = 0;
char LEDon   = 0;
unsigned short OVSMParrRedtemp[OVSMP_ARR_SIZE];
unsigned short OVSMParrIRtemp[OVSMP_ARR_SIZE];
unsigned short OVSMParrRed[OVSMP_ARR_SIZE];
unsigned short OVSMParrIR[OVSMP_ARR_SIZE];

/* BPM Calculation variables */
int BPMtimeCtr                      = 0;
char maxFound                       = false;
unsigned short OVSMPmaxValRed       = 0;
unsigned short OVSMPmaxValIR        = 0;
int BPMmaxTime[2]                   = {0, 0};
unsigned short BPMarr[BPM_ARR_SIZE] = {100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
unsigned short BPM                  = 100;

/* SPO2 Calculation variables */
char minFound                         = false;
unsigned short OVSMPminValRed         = 0;
unsigned short X_Red                  = 0;
float Y_Red[2]                        = {0, 0};
unsigned short DC_Red                 = 0;
unsigned short AC_Red                 = 0;
unsigned short OVSMPminValIR          = 0;
unsigned short X_IR                   = 0;
float Y_IR[2]                         = {0, 0};
unsigned short DC_IR                  = 0;
unsigned short AC_IR                  = 0;
float RoR                             = 0;
unsigned short SpO2arr[SPO2_ARR_SIZE] = {99, 99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99};
unsigned short SpO2                   = 99;

/* Extra features variables */
unsigned short fingerIN = 0;
float BatV              = 0;

/* Function Declarations */
void toggleLEDs();
void DMAsetup();
void ADCrun();
void OVSMPcalc();
void BIOSIGcalc();
void BPMcalc();
void SpO2calc();

int main(void)
{
    /* Initialize peripherals */
    HAL_System_Init();

    /* Initialize GUI layer */
    GUI_Init();

    /* Initialize SYSCONFIG settings */
    SYSCFG_DL_init();

    /* SETUP INTERRUPTS AND UART */
    NVIC_EnableIRQ(T8_INST_INT_IRQN);
    NVIC_EnableIRQ(T1_INST_INT_IRQN);
    NVIC_EnableIRQ(B_INT_IRQN);
    NVIC_EnableIRQ(ADC_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(UART_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_INST_INT_IRQN);
    DL_UART_enable(UART_INST);

    /* Setup DMA for ADC */
    DL_DMA_setSrcAddr(DMA, DMA0_CHAN_ID,
        (uint32_t) DL_ADC12_getMemResultAddress(ADC_INST, DL_ADC12_MEM_IDX_0));
    DL_DMA_setDestAddr(DMA, DMA0_CHAN_ID, (uint32_t) &ADCSamples[0]);

    /* Clear LED pins */
    DL_GPIO_clearPins(P_PORT, P_R_PIN | P_IR_PIN);

    /* Wait for ACK from GUI */
    __WFI();

    while (1) {
    }
}
void T8_INST_IRQHandler(void)
{
    switch (DL_Timer_getPendingInterrupt(T8_INST)) {
        case DL_TIMER_IIDX_ZERO: {
            /* Run 1ms timer that red LED will stay on */
            DL_Timer_startCounter(T1_INST);

            /* Keep track of number of 8ms intervals */
            BPMtimeCtr++;

            /* Run functions for LED toggle and sampling */
            toggleLEDs();
            DMAsetup();
            ADCrun();

            break;
        }
        default:
            break;
    }
}

void T1_INST_IRQHandler(void)
{
    switch (DL_Timer_getPendingInterrupt(T1_INST)) {
        case DL_TIMER_IIDX_ZERO: {
            if (LEDon == RED_LED_ON) {
                /* Run 1ms timer that IR LED will stay on */
                DL_Timer_startCounter(T1_INST);

                /* Run functions for LED toggle and sampling */
                toggleLEDs();
                DMAsetup();
                ADCrun();
            } else if (LEDon == IR_LED_ON) {
                /* Turn off LEDs */
                toggleLEDs();

                /* TI GUI UART TX */
                GUIComm_sendUInt16("IR", 2, OVSMParrIR[OVSMP_ARR_SIZE - 1]);
                GUIComm_sendUInt16("Red", 3, OVSMParrRed[OVSMP_ARR_SIZE - 1]);
                GUIComm_sendUInt16("BPM", 3, BPM);
                GUIComm_sendUInt16("SpO2", 4, SpO2);

                /* Run calculations on data */
                BIOSIGcalc();

                /* Run SpO2 calculations every 1s */
                if (BPMtimeCtr % EVERY_1_SEC == 0) {
                    SpO2calc();
                }

                /* Get battery voltage every 1s */
                if (BPMtimeCtr % EVERY_1_SEC == 0) {
                    /* Switch to MEMCTL1 for internal supply measuring with INT VREF */
                    delay_cycles(
                        ADC_MEM_SWITCH_WAIT_TIME);  // wait 100us before switching
                    DL_ADC12_disableConversions(ADC_INST);
                    DL_ADC12_setStartAddress(
                        ADC_INST, DL_ADC12_SEQ_START_ADDR_01);
                    DL_ADC12_enableConversions(ADC_INST);

                    /* Make ADC measurement and convert to voltage (VDD monitoring channel 15 = VDD/3) */
                    DL_ADC12_startConversion(ADC_INST);
                    delay_cycles(
                        ADC_VDD_MNTR_WAIT_TIME);  // wait 1ms for conversion
                    BatV =
                        (DL_ADC12_getMemResult(ADC_INST, DL_ADC12_MEM_IDX_1) /
                            ADC12BIT_CODES * VREF_INT) *
                        VDD_MONITORING_MLTPR;

                    /* Switch back to MEMCTL0 for normal operation */
                    delay_cycles(
                        ADC_MEM_SWITCH_WAIT_TIME);  // wait 100us for calculation
                    DL_ADC12_disableConversions(ADC_INST);
                    DL_ADC12_setStartAddress(
                        ADC_INST, DL_ADC12_SEQ_START_ADDR_00);
                    DL_ADC12_enableConversions(ADC_INST);
                }

                /*
                 * Run FINGER IN TEST every 1/5s
                 * If sensor reading high (over 60000) (light transmitting to sensor directly)
                 * for more than 6 seconds (1/5 * 30), initiate timer shutdown
                */
                if (BPMtimeCtr % EVERY_0s2_SEC == 0) {
                    for (short i = 0; i < OVSMP_ARR_SIZE; i++) {
                        if (OVSMParrRed[i] < SHTDWN_LIGHT_THRSHLD) {
                            fingerIN = 0;
                            break;
                        }
                    }

                    fingerIN++;

                    if (fingerIN > EVERY_6_SEC) {
                        DL_Timer_stopCounter(T8_INST);
                        DL_Timer_stopCounter(T1_INST);
                        NVIC_DisableIRQ(T8_INST_INT_IRQN);
                        NVIC_DisableIRQ(T1_INST_INT_IRQN);
                        fingerIN = 0;
                    }
                }
            }
            break;
        }
        default:
            break;
    }
}

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case B_INT_IIDX:
            /* If Button is pressed (low), reset device */
            if (DL_GPIO_readPins(B_PORT, B_WAKE_PIN) == 0) {
                NVIC_DisableIRQ(B_INT_IRQN);
                DL_Timer_stopCounter(T8_INST);
                DL_Timer_stopCounter(T1_INST);
                NVIC_DisableIRQ(T8_INST_INT_IRQN);
                NVIC_DisableIRQ(T1_INST_INT_IRQN);
                for (short i = 0; i < BPM_ARR_SIZE; i++) {
                    BPMarr[i]  = 100;
                    SpO2arr[i] = 99;
                }
                BPM        = 100;
                SpO2       = 99;
                BPMtimeCtr = 0;
                delay_cycles(
                    REBOOT_WAIT_TIME);  // Wait 2 seconds before reboot
                DL_Timer_startCounter(T8_INST);
                NVIC_EnableIRQ(T8_INST_INT_IRQN);
                NVIC_EnableIRQ(T1_INST_INT_IRQN);
                NVIC_EnableIRQ(B_INT_IRQN);
            }
            break;
    }
}

void ADC_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC_INST)) {
        case DL_ADC12_IIDX_DMA_DONE:
            /* Calculate outcome of over-sampling and store information */
            if (LEDon != BOTH_LEDS_OFF) {
                OVSMPcalc();
            }
            /* Calculate VDD */
            else if (LEDon == BOTH_LEDS_OFF) {
            }
            break;
        default:
            break;
    }
}

void toggleLEDs()
{
    /* Turn on Red LED */
    if (LEDon == BOTH_LEDS_OFF) {
        DL_GPIO_setPins(P_PORT, P_R_PIN);
        DL_COMP_setDACCode0(DAC_INST, RED_BRIGHTNESS);
        LEDon = RED_LED_ON;
    }
    /* Turn on IR LED */
    else if (LEDon == RED_LED_ON) {
        DL_GPIO_clearPins(P_PORT, P_R_PIN);
        DL_GPIO_setPins(P_PORT, P_IR_PIN);
        DL_COMP_setDACCode0(DAC_INST, IR_BRIGHTNESS);
        LEDon = IR_LED_ON;
    }
    /* Turn off both LEDs */
    else if (LEDon == IR_LED_ON) {
        DL_GPIO_clearPins(P_PORT, P_IR_PIN);
        DL_COMP_setDACCode0(DAC_INST, OFF_BRIGHTNESS);
        LEDon = BOTH_LEDS_OFF;
    }
}

void DMAsetup()
{
    DL_DMA_setTransferSize(DMA, DMA0_CHAN_ID, ADC_SAMPLE_SIZE);
    DL_DMA_enableChannel(DMA, DMA0_CHAN_ID);
    DL_ADC12_enableDMA(ADC_INST);
}

void ADCrun()
{
    delay_cycles(LED_SETTLING_TIME);  // Allow LEDs and PD to settle for 700us
    DL_ADC12_startConversion(ADC_INST);
}

void OVSMPcalc()
{
    /* Over-sampling Calculation with Over-sampling size and denominator equation */
    int sampTot = 0;
    for (short i = 0; i < ADC_SAMPLE_SIZE; i++) {
        sampTot += ADCSamples[i];
    }
    ADCOVSMP = sampTot / pow(2, NUM_BITS_OVSMP);

    /* Save in RED or IR LED array depending on what is on */
    if (LEDon == RED_LED_ON) {
        /* Shift history and place new value at end */
        for (short i = 0; i < (OVSMP_ARR_SIZE - 1); i++) {
            OVSMParrRedtemp[i] = OVSMParrRedtemp[i + 1];
            OVSMParrRed[i]     = OVSMParrRed[i + 1];
        }
        OVSMParrRedtemp[OVSMP_ARR_SIZE - 1] = ADCOVSMP;

        /* Moving average filter with k */
        int movMeanTot = 0;
        for (short i = (OVSMP_ARR_SIZE - 1); i > ((OVSMP_ARR_SIZE - 1) - k);
             i--) {
            movMeanTot += OVSMParrRedtemp[i];
        }
        OVSMParrRed[OVSMP_ARR_SIZE - 1] = movMeanTot / k;
    } else if (LEDon == IR_LED_ON) {
        /* Shift history and place new value at end */
        for (short i = 0; i < (OVSMP_ARR_SIZE - 1); i++) {
            OVSMParrIRtemp[i] = OVSMParrIRtemp[i + 1];
            OVSMParrIR[i]     = OVSMParrIR[i + 1];
        }
        OVSMParrIRtemp[OVSMP_ARR_SIZE - 1] = ADCOVSMP;

        /* Moving average filter with k */
        int movMeanTot = 0;
        for (short i = (OVSMP_ARR_SIZE - 1); i > ((OVSMP_ARR_SIZE - 1) - k);
             i--) {
            movMeanTot += OVSMParrIRtemp[i];
        }
        OVSMParrIR[OVSMP_ARR_SIZE - 1] = movMeanTot / k;
    }
}

void BIOSIGcalc()
{
    /*
     * Find red peaks
     * Scan to see if center of array is max and mark it
     */
    for (short i = 0; i < OVSMP_ARR_SIZE; i++) {
        if (OVSMParrRed[i] > OVSMParrRed[(OVSMP_ARR_SIZE - 1) / 2]) {
            maxFound = false;
            break;
        } else {
            maxFound = true;
        }
    }

    if (maxFound) {
        /* Artificially increase max after saving to only capture first max in case of multiples */
        OVSMPmaxValRed = OVSMParrRed[(OVSMP_ARR_SIZE - 1) / 2]++;
        maxFound       = false;
    }

    /*
     * Find IR peaks
     * Scan to see if center of array is max and mark it
     */
    for (short i = 0; i < OVSMP_ARR_SIZE; i++) {
        if (OVSMParrIR[i] > OVSMParrIR[(OVSMP_ARR_SIZE - 1) / 2]) {
            maxFound = false;
            break;
        } else {
            maxFound = true;
        }
    }

    if (maxFound) {
        /* Artificially increase max after saving to only capture first max in case of multiples */
        OVSMPmaxValIR = OVSMParrIR[(OVSMP_ARR_SIZE - 1) / 2]++;
        maxFound      = false;

        /* Get time between max's for BPM time calculations --- IR ONLY */
        BPMmaxTime[0] = BPMmaxTime[1];
        BPMmaxTime[1] = BPMtimeCtr;
        BPMcalc();
    }

    /* Find RED valleys */
    /* Scan to see if center of array is min and mark it */
    for (short i = 0; i < OVSMP_ARR_SIZE; i++) {
        if (OVSMParrRed[i] < OVSMParrRed[(OVSMP_ARR_SIZE - 1) / 2]) {
            minFound = false;
            break;
        } else {
            minFound = true;
        }
    }

    if (minFound) {
        /* Artificially decrease min after saving to only capture first min in case of multiples */
        OVSMPminValRed = OVSMParrRed[(OVSMP_ARR_SIZE - 1) / 2]--;
        minFound       = false;
    }

    /*
     * Find IR valleys
     * Scan to see if center of array is min and mark it
     */
    for (short i = 0; i < OVSMP_ARR_SIZE; i++) {
        if (OVSMParrIR[i] < OVSMParrIR[(OVSMP_ARR_SIZE - 1) / 2]) {
            minFound = false;
            break;
        } else {
            minFound = true;
        }
    }

    if (minFound) {
        /* Artificially decrease min after saving to only capture first min in case of multiples */
        OVSMPminValIR = OVSMParrIR[(OVSMP_ARR_SIZE - 1) / 2]--;
        minFound      = false;
    }

    /* IIR Low Pass Filter for RED DC */
    X_Red    = OVSMParrRed[OVSMP_ARR_SIZE - 1];
    Y_Red[0] = Y_Red[1];
    Y_Red[1] = Y_Red[0] + b * (X_Red - Y_Red[0]);
    DC_Red   = Y_Red[1];

    /* IIR Low Pass Filter for IR DC */
    X_IR    = OVSMParrIR[OVSMP_ARR_SIZE - 1];
    Y_IR[0] = Y_IR[1];
    Y_IR[1] = Y_IR[0] + b * (X_IR - Y_IR[0]);
    DC_IR   = Y_IR[1];

    /* Calculate RED AC */
    AC_Red = OVSMPmaxValRed - OVSMPminValRed;

    /* Calculate IR AC */
    AC_IR = OVSMPmaxValIR - OVSMPminValIR;
}

void BPMcalc()
{
    /* Shift history and place new value at end */
    for (short i = 0; i < BPM_ARR_SIZE - 1; i++) {
        BPMarr[i] = BPMarr[i + 1];
    }

    /*
     * Calculate time between IR peaks and convert to BPM and save, excluding outliers
     * Equation for converting from 125Hz Counter to beats per minute
     */
    BPMarr[19] = 7500 / (BPMmaxTime[1] - BPMmaxTime[0]);
    if (BPMarr[19] > (BPM + 100) || BPMarr[19] < (BPM - 100)) {
        BPMarr[19] = BPM;
    }

    /* Maintain array for averaging samples */
    short BPMsum = 0;
    for (short i = 0; i < BPM_ARR_SIZE; i++) {
        BPMsum += BPMarr[i];
    }
    BPM = BPMsum / BPM_ARR_SIZE;
}

void SpO2calc()
{
    /* Calculate Ratio of Ratios (RoR) */
    RoR = (AC_Red / (float) DC_Red) / (float) (AC_IR / (float) DC_IR);

    /* Shift history left */
    for (short i = 0; i < SPO2_ARR_SIZE - 1; i++) {
        SpO2arr[i] = SpO2arr[i + 1];
    }

    /*
     * Calculate SpO2 + 3 offset and save, excluding outliers
     * Equation for RoR conversion to SpO2 based on data regression
     */
    SpO2arr[19] = -45.06 * RoR * RoR + 30.354 * RoR + 94.845 + 4;
    if (SpO2arr[19] > (SpO2 + 10) || SpO2arr[19] < (SpO2 - 10)) {
        SpO2arr[19] = SpO2;
    }

    /* Maintain array for averaging samples */
    short SpO2sum = 0;
    for (short i = 0; i < SPO2_ARR_SIZE; i++) {
        SpO2sum += SpO2arr[i];
    }
    SpO2 = SpO2sum / SPO2_ARR_SIZE;
}
