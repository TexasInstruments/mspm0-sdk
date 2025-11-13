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

#include <ti/iqmath/include/IQmathLib.h>
#include <cstdio>
#include "ti_msp_dl_config.h"

#include "includes/EVConfig.h"

#include "modules/Adc.h"
#include "modules/ControlPilot.h"
#include "modules/ControlPilot_HAL.h"
#include "modules/FirmwareUpdater.h"
#include "modules/Gpio.h"
#include "modules/PlugLock.h"
#include "modules/PowerSwitch.h"
#include "modules/Rcd.h"
#include "modules/RemoteControlRX.h"
#include "modules/RemoteControlTX.h"

#include "pb/protobuf/tida010939.pb.h"

/* Temperature constants */
/* Used for temp calculation from ADC code. Expounded upon in TIDA-010939 design doc */
volatile _iq18 R_PT, C, numerator, square;
const _iq18 R_0      = _IQ18(1000);
const _iq17 R_PULLUP = _IQ17(10000);
const _iq18 TEMP_A   = _IQ24(-5.77 * 0.0000001);
const _iq18 TEMP_B   = _IQ18(0.0039083);
_iq18 A, B, denominator;

#define TEMP_SAMPLES 2
volatile uint16_t gADC1Temps[TEMP_SAMPLES];
float gTemps[TEMP_SAMPLES];

/* ADCs */
#define ADC0_SAMPLES 4
#define ADC0_FIFO_SAMPLES ADC0_SAMPLES >> 1
#define ADC1_SAMPLES 2

static Adc gADC0(ADC0);

/* PowerSwitch */
Gpio gRelayStatus(Relay_Check_PORT, Relay_Check_PIN_21_PIN);
Gpio gRelayWeldDetect(R_Mirror_Out_PORT, R_Mirror_Out_PIN_20_PIN);
static PowerSwitch gPowerSwitch(TIMA1, gRelayStatus, gRelayWeldDetect);

/* RCD */
Gpio gRcdReset(RCD_Reset_PORT, RCD_Reset_PIN_24_PIN);
Gpio gRcdnFault(RCD_nFault_PORT, RCD_nFault_PIN_27_PIN);
static Rcd gRcd(gRcdReset, gRcdnFault, &gPowerSwitch);

/* Plug Lock */
Gpio gPlugLockNSleep(Plug_Lock_nSleep_PORT, Plug_Lock_nSleep_PIN_18_PIN);
Gpio gPlugLockDirection(
    Plug_Lock_Direction_PORT, Plug_Lock_Direction_PIN_19_PIN);
Gpio gCHAdeMOPlugLockFeedback(
    Plug_Lock_CdM_FB_PORT, Plug_Lock_CdM_FB_PIN_14_PIN);
static PlugLock gPlugLock(PlugLock_INST, gPlugLockNSleep, gPlugLockDirection,
    gCHAdeMOPlugLockFeedback);

/* Control Pilot HAL */
Gpio gChargingEnable(EV_Charge_PORT, EV_Charge_PIN_3_PIN);
static ControlPilot_HAL gControlPilotHAL(
    CCS_CP_INST, gADC0, gPlugLock, &gChargingEnable);

/* Comms */
static MgmtLink gMgmtLink(UART0);
static RemoteControlTX gRemoteControlTX(gMgmtLink);

/* Control Pilot */
static ControlPilot gControlPilot(
    gControlPilotHAL, gRemoteControlTX, gPowerSwitch, gRcd);
static RemoteControlRX gRemoteControlRX(
    gControlPilot, gRemoteControlTX, gMgmtLink);

int main(void)
{
    /* Peripheral inits */
    SYSCFG_DL_init();

    /* Use MathACL to calculate temperature constants */
    A           = _IQ18mpyIQX(R_0, 18, TEMP_A, 24);
    B           = _IQ18mpy(R_0, TEMP_B);
    denominator = _IQ18mpy(_IQ18(2), A);

    /* Enable IRQs */
    NVIC_EnableIRQ(ADC0_INST_INT_IRQN);
    NVIC_EnableIRQ(ADC1_INST_INT_IRQN);
    NVIC_EnableIRQ(CCS_CP_INST_INT_IRQN);
    NVIC_EnableIRQ(CP_MeasureHI_Timer_INST_INT_IRQN);
    NVIC_EnableIRQ(CP_MeasureLO_Timer_INST_INT_IRQN);
    NVIC_EnableIRQ(PL_DRV_Timer_INST_INT_IRQN);
    NVIC_EnableIRQ(GPIOB_INT_IRQn);
    NVIC_EnableIRQ(UART0_INT_IRQn);

    //NVIC_EnableIRQ(UART1_INT_IRQn);
    //NVIC_EnableIRQ(I2C0_INT_IRQn);

    /* Begin Modules */

    gPowerSwitch.init();
    gControlPilotHAL.init();
    gMgmtLink.init();
    gControlPilot.init();
    gPlugLock.init();

    gRemoteControlRX.sendResetReason();

    while (1) {
        gControlPilot.main();
    }
}

#ifdef __cplusplus
extern "C" {
#endif

/* ADC interrupt handler for reading feedback values */
void ADC0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC0_INST)) {
        case DL_ADC12_IIDX_MEM3_RESULT_LOADED:
            gADC0.HAL_ADC_ConvCpltCallback(ADC0_INST);
            DL_ADC12_enableConversions(ADC0_INST);
            break;
        default:
            break;
    }
}

/* ADC interrupt for reading temperature */
void ADC1_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC1_INST)) {
        case DL_ADC12_IIDX_MEM1_RESULT_LOADED:
            gADC1Temps[0] =
                DL_ADC12_getMemResult(ADC1_INST, DL_ADC12_MEM_IDX_0);
            gADC1Temps[1] =
                DL_ADC12_getMemResult(ADC1_INST, DL_ADC12_MEM_IDX_1);

            /* Convert ADC codes to Celsius */
            R_PT = (_IQ5div(_IQ5mpyIQX(R_PULLUP, 17, _IQ18(gADC1Temps[0]), 18),
                       _IQ5(4095 - gADC1Temps[0])))
                   << 13;
            C         = R_0 - R_PT;
            square    = _IQ18sqrt(_IQ18mpy(B, B) - _IQ18mpy(4 * A, C));
            numerator = -B + square;
            gTemps[0] = _IQ18toF(_IQ18div(numerator, denominator));

            R_PT = (_IQ5div(_IQ5mpyIQX(R_PULLUP, 17, _IQ18(gADC1Temps[1]), 18),
                       _IQ5(4095 - gADC1Temps[1])))
                   << 13;
            C         = R_0 - R_PT;
            square    = _IQ18sqrt(_IQ18mpy(B, B) - _IQ18mpy(4 * A, C));
            numerator = -B + square;
            gTemps[1] = _IQ18toF(_IQ18div(numerator, denominator));

            /* Pass temps to control pilot for handling */
            gControlPilot.overtemp_check(&gTemps[0]);

            DL_ADC12_enableConversions(ADC1_INST);
            break;
        default:
            break;
    }
}

/* This timer signals whether CP feedback from EV is on the high/low (+12/9/6V / -12V) portion and sets ADC read accordingly */
void CCS_CP_INST_IRQHandler(void)
{
    switch (DL_Timer_getPendingInterrupt(CCS_CP_INST)) {
        case DL_TIMER_IIDX_LOAD:
            DL_Timer_startCounter(CP_MeasureHI_Timer_INST);
            break;
        case DL_TIMER_IIDX_CC0_UP:
            DL_Timer_startCounter(CP_MeasureLO_Timer_INST);
            break;
        default:
            break;
    }
}

/* Short delay to let feedback voltage settle before sampling */
void CP_MeasureHI_Timer_INST_IRQHandler(void)
{
    gADC0.triggerEvseCPHiISR();
}

void CP_MeasureLO_Timer_INST_IRQHandler(void)
{
    gADC0.triggerEvseCPLoISR();
}

void GROUP1_IRQHandler(void)
{
    switch (DL_GPIO_getPendingInterrupt(GPIOB)) {
        /* RCD nFault falling edge */
        case RCD_nFault_PIN_27_IIDX:
            gRcd.HAL_GPIO_EXTI_Callback(RCD_nFault_PIN_27_PIN);
            break;
        default:
            break;
    }
}

/* UART for EVerest --> MCU comms */
void UART0_IRQHandler(void)
{
    switch (DL_UART_getPendingInterrupt(UART0)) {
        case DL_UART_IIDX_DMA_DONE_RX:
            if (gMgmtLink.processByte()) gRemoteControlRX.processPacket();
            break;
        case DL_UART_IIDX_DMA_DONE_TX:
            gMgmtLink.TxDone();
            break;
        default:
            break;
    }
}

void PL_DRV_Timer_INST_IRQHandler(void)
{
    gPlugLock.motorOff();
}

#ifdef __cplusplus
}  // END EXTERN C
#endif
