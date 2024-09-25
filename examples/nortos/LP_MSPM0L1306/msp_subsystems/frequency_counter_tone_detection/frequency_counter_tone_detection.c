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

#include "ti_msp_dl_config.h"
#include <stdint.h>

/*========================== DEFINES ==================================*/

/* 
 * Based on required specifications, vary the value
 * between PERIOD_10ms, PERIOD_20ms, and PERIOD_50ms
 * to achieve desired frequency range.
 *
 * RANGES:
 * 10 ms: 100 Hz - 1 MHz
 * 20 ms:  50 Hz - 1 MHz
 * 50 ms:  20 Hz - 1 MHz
 *
 * Please reference [file name] for percent error
 */
#define CAPTURE_PERIOD (PERIOD_20ms)

/* 
 * Set the desired frequency range
 * NOTE: see [file name] to ensure proper capture period is set
 * for desired frequency range
*/  
#define LOWERBOUND (2000)
#define UPPERBOUND (10000)

/* 
 * The maximum amount of rising edge the Timer Compare
 * will read from the COMP. Used as a limit rather than
 * an actual fix value of counts
 */
#define MAX_COMPARE_COUNT 65000
#define PERIOD_100ms (PERIOD_TIMER_INST_LOAD_VALUE)

enum period { PERIOD_10ms = 328, PERIOD_20ms = 655, PERIOD_50ms = 1638 };

/*==================== FORWARD DECLARATIONS =========================*/
void getCOMPEdgeCount(void);
void startCOMPEdgeCount(void);
void stopCOMPEdgeCount(void);
void AddConfiguration_Periodic_Timer(void);

/*==================== LOCAL VARIABLES ==============================*/
/* Rising Edge Counter */
volatile uint16_t rEdgeRegister;
volatile uint32_t gFrequency;

/*======================== CODE =====================================*/
int main(void) {

  /* Power on GPIO, initialize pins as digital outputs */
  SYSCFG_DL_init();

  AddConfiguration_Periodic_Timer();

  /* Disabling the COMP */
  DL_COMP_disable(COMP_0_INST);

  /* Enable Period 100ms and Switch IRQs */
  NVIC_EnableIRQ(PERIOD_TIMER_INST_INT_IRQN);

  /* Starting Periodic timer (100ms default) */
  DL_TimerG_startCounter(PERIOD_TIMER_INST);

  /* Lowest possible power - TIMG0 */
  DL_SYSCTL_setPowerPolicySTANDBY1();

  /* Allow CPU to process after each wakeup event */
  DL_SYSCTL_disableSleepOnExit();

  double period;
  switch (CAPTURE_PERIOD) {
    case 1638:
        period = 0.05;
        break;
    case 655:
        period = 0.02;
        break;
    case 328:
        period = 0.01;
        break;
    default:
        break;
  }

  while (1) {
    __WFI();

    gFrequency = rEdgeRegister / period;

    if (gFrequency >= LOWERBOUND && gFrequency <= UPPERBOUND) {
      /* turn LED on when TRUE*/
      DL_GPIO_setPins(GPIOA, GPIO_ALERT_A1_PIN);
    } else {
      /* turn LED off when FALSE*/
      DL_GPIO_clearPins(GPIOA, GPIO_ALERT_A1_PIN);
    }
  }
}

/*
 * Generate 8ms capture timing based on shared periodic timer
 */
void AddConfiguration_Periodic_Timer(void) {
  DL_Timer_setCaptureCompareValue(PERIOD_TIMER_INST, CAPTURE_PERIOD,
                                  DL_TIMER_CC_0_INDEX);
  DL_Timer_setCaptureCompareCtl(PERIOD_TIMER_INST,
                                (uint32_t)DL_TIMER_INTERM_INT_ENABLED,
                                DL_TIMER_CC_ACOND_TIMCLK, DL_TIMER_CC_0_INDEX);
  DL_TimerG_enableInterrupt(
      PERIOD_TIMER_INST,
      (DL_TIMERG_INTERRUPT_ZERO_EVENT | DL_TIMERG_INTERRUPT_CC0_DN_EVENT));
}

void startCOMPEdgeCount(void) {

  DL_Timer_setTimerCount(COMPARE_0_INST, MAX_COMPARE_COUNT);

  DL_COMP_enable(COMP_0_INST);

  DL_TimerG_startCounter(COMPARE_0_INST);

  DL_SYSCTL_setPowerPolicySTOP1();
}

void stopCOMPEdgeCount(void) {
  DL_TimerG_stopCounter(COMPARE_0_INST);

  DL_COMP_disable(COMP_0_INST);

  rEdgeRegister =
      MAX_COMPARE_COUNT - DL_Timer_getTimerCount(COMPARE_0_INST) - 1;

  DL_SYSCTL_setPowerPolicySTANDBY1();
}

/* 
 * Period 100ms Timer IRQ
 * Waits for the timer's counter to reach an x amount of time
 * When the previous happens, the COMP is activated
 * Waits for the timer's counter to reach 0
 * When the previous happens, call startCOMPEdgeCount()
 */
void PERIOD_TIMER_INST_IRQHandler(void) {
  switch (DL_Timer_getPendingInterrupt(PERIOD_TIMER_INST)) {
  case DL_TIMER_IIDX_CC0_DN:
    startCOMPEdgeCount();
    break;
  case DL_TIMER_IIDX_ZERO:
    stopCOMPEdgeCount();

    break;
  default:
    break;
  }
}
