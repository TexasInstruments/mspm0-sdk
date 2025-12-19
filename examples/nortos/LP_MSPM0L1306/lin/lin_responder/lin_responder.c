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
#include <stdio.h>
#include <string.h>
#include "lin_config.h"
#include "ti_msp_dl_config.h"

extern volatile LIN_STATE gStateMachine;
volatile LIN_Sync_Bits gBitTimes[LIN_RESPONDER_SYNC_CYCLES];
uint8_t gResponderRXBuffer[LIN_DATA_MAX_BUFFER_SIZE]      = {0};
uint8_t gResponderTXPacket[LIN_DATA_MAX_BUFFER_SIZE]      = {0};

/* Variables for autobaud feature */

volatile uint16_t gLinResponseLapseVar = LIN_RESPONSE_LAPSE;
volatile uint32_t gLin0TbitWidthVar    = LIN_0_TBIT_WIDTH;
volatile uint16_t gPrevBaudRate        = LIN_0_BAUD_RATE;
volatile uint16_t gCurrBaudRate        = LIN_0_BAUD_RATE;
volatile uint16_t gSumOfTbits = 0;
volatile bool gAutoBaudRateDone = false;
volatile uint16_t gNumCyclesPos = 0;
volatile uint16_t gNumCyclesNeg = 0;
volatile uint16_t gNumCyclesErrorCount = 0;
volatile uint32_t gAutoBaudRateTbitThreshold = 2;

void LIN_processMessage_Rx(void);


LIN_table_record_t responderMessageTable[LIN_RESPONDER_NUM_MSGS] = {
    /* Important, connection number matches the order of this table */
    {
        0x08, /* PID 0x08 */
        5,
    },
    {
        0x49, /* PID 0x49 */
        5,
    },
    {
        0x0D, /* PID 0x0D */
        5,
    },
    {0x39, /* PID 0x39 */
        8, LIN_processMessage_Rx},
    {0xBA, /* PID 0x3A */
        8, LIN_processMessage_Rx},
    {0xFB, /* PID 0x3B */
        8, LIN_processMessage_Rx},
};



int main(void)
{
    SYSCFG_DL_init();

    /* Enable transceiver */
    DL_GPIO_setPins(GPIO_LIN_ENABLE_PORT, GPIO_LIN_ENABLE_USER_LIN_ENABLE_PIN);

    NVIC_ClearPendingIRQ(LIN_0_INST_INT_IRQN);
    NVIC_EnableIRQ(LIN_0_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    
    DL_SYSCTL_enableSleepOnExit();
     
    while (1) {
        __WFI();
    }
}


void LIN_0_INST_IRQHandler(void)
{
     
    uint16_t counterVal       = 0;
    uint8_t data              = 0;
    uint16_t averageBitTime   = 0;
    uint16_t measuredBaudRate = 0;
    
        /* Detect LIN Break Field larger than 9.5 bit time. */
    switch (DL_UART_Extend_getPendingInterrupt(LIN_0_INST)) {
        case DL_UART_IIDX_LIN_FALLING_EDGE:
            /* Signals the start of the break field. */
            if (gStateMachine == LIN_STATE_WAIT_FOR_BREAK) {
                gStateMachine = LIN_STATE_BREAK_FIELD;

                /*Clear old data from last wrong PID transmit in RX buffer*/
                DL_UART_Extend_receiveData(LIN_0_INST);

                /*Enable LINC1 to detect rising edge of break field*/
                DL_UART_enableLINRisingEdgeCapture(LIN_0_INST);
                DL_UART_Extend_clearInterruptStatus(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RXD_POS_EDGE);
                DL_UART_Extend_enableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RXD_POS_EDGE);
               
            }
            break;

        /* Rising Edge Detection Interrupt on UARTxRXD. */
        case DL_UART_EXTEND_IIDX_RXD_POS_EDGE:
        if (gStateMachine == LIN_STATE_BREAK_FIELD)
        {
            
            /* Judge whether LIN Break Field is shorter than 20 bit time. */
            counterVal = DL_UART_Extend_getLINRisingEdgeCaptureValue(LIN_0_INST);
            /* Validation check of the length of the break field. */
            if (counterVal < (gLin0TbitWidthVar * 20) &&
                counterVal >(gLin0TbitWidthVar * 9.5))
            {

                /*Enable interrupt to detect SYNC*/
                DL_UART_Extend_enableLINSyncFieldValidationCounterControl(LIN_0_INST);
                DL_UART_Extend_disableLINCounterCompareMatch(LIN_0_INST);
                DL_UART_Extend_disableLINCountWhileLow(LIN_0_INST);
                DL_UART_Extend_disableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RX);
                DL_UART_Extend_clearInterruptStatus(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RXD_NEG_EDGE);
                DL_UART_Extend_enableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RXD_NEG_EDGE);
                gStateMachine = LIN_STATE_SYNC_FIELD_NEG_EDGE;
                gAutoBaudRateDone=false;
                
            }
            else{
                gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
                DL_UART_Extend_disableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RXD_POS_EDGE);
            }
            DL_UART_Extend_disableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_LIN_RISING_EDGE);
            
        }
        else if(gStateMachine==LIN_STATE_SYNC_FIELD_POS_EDGE){
        if(gAutoBaudRateDone == true){
            DL_UART_Extend_enableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RX);
            DL_UART_Extend_disableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_LINC0_MATCH);
            DL_UART_Extend_disableLINCounter(LIN_0_INST);
            DL_UART_disableLINRisingEdgeCapture(LIN_0_INST);
            DL_UART_Extend_disableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RXD_NEG_EDGE);
            DL_UART_Extend_disableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RXD_POS_EDGE);
        }
        else{
            gNumCyclesPos++;
            gStateMachine = LIN_STATE_SYNC_FIELD_NEG_EDGE;
        }
        }
       
            break;

        /* Negative Edge Detection Interrupt on UARTxRXD. */
        case DL_UART_EXTEND_IIDX_RXD_NEG_EDGE:

      if (gStateMachine == LIN_STATE_SYNC_FIELD_NEG_EDGE) {

          /* Capture the timing at falling edge (contains 2 Tbit timing) */
          gBitTimes[gNumCyclesNeg].negEdge = DL_UART_Extend_getLINFallingEdgeCaptureValue(LIN_0_INST);

         if(gNumCyclesPos > 0){

          /* Ensure positive edge occurred before this negative edge */
          if ( gNumCyclesNeg == (gNumCyclesPos - 1)) {

              /* Accumulate timing for average calculation */
              gSumOfTbits += gBitTimes[gNumCyclesNeg].negEdge;

              /* Simple 5% tolerance check on the 2-Tbit timing */
              if (!(gBitTimes[gNumCyclesNeg].negEdge > ((gLin0TbitWidthVar * 2 * 95) / 100) &&
                    gBitTimes[gNumCyclesNeg].negEdge < ((gLin0TbitWidthVar * 2 * 105) / 100))) {
                  gNumCyclesErrorCount++;
              }
              gNumCyclesNeg++;
          }
          else{
            gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
          }
         }

              /* Check if all 4 sync segments completed */
              if (gNumCyclesNeg == LIN_RESPONDER_SYNC_CYCLES) {

                  /* Determine if auto baud rate adjustment is needed */
                  if (gNumCyclesErrorCount >= gAutoBaudRateTbitThreshold && AUTO_BAUD_ENABLED) {

                      /* Calculate average bit time (divide by 2 since neg edge has 2 Tbit) */
                      averageBitTime = (gSumOfTbits / LIN_RESPONDER_SYNC_CYCLES) / 2;
                      measuredBaudRate = LIN_0_INST_FREQUENCY / averageBitTime;
                    

                      /* Update Baudrate */
                      DL_UART_disable(LIN_0_INST);
                      delay_cycles(AUTO_BAUD_CONFIG_DELAY);
                      DL_UART_disableFIFOs(LIN_0_INST);

                      gLinResponseLapseVar = LIN_0_INST_FREQUENCY / (2 * measuredBaudRate);
                      gLin0TbitWidthVar = averageBitTime;

                      DL_UART_configBaudRate(LIN_0_INST, LIN_0_INST_FREQUENCY, measuredBaudRate);
                      DL_UART_Extend_setLINCounterCompareValue(LIN_0_INST,
                          gLin0TbitWidthVar * LIN_0_TBIT_COUNTER_COEFFICIENT);

                      DL_UART_Extend_enable(LIN_0_INST);

                      /* Update baud rate tracking */
                      gPrevBaudRate = gCurrBaudRate;
                      gCurrBaudRate = measuredBaudRate;
                  }
                  else{
                    gPrevBaudRate = gCurrBaudRate;
                  }

                  /* Mark auto baud as done and reset counters */
                  gAutoBaudRateDone = true;
                  gNumCyclesNeg = 0;
                  gNumCyclesPos = 0;
                  gNumCyclesErrorCount = 0;
                  gSumOfTbits = 0;
                

                  /* Stay in sync field state - pos edge will handle the transition */
                  gStateMachine = LIN_STATE_SYNC_FIELD_POS_EDGE;
                 DL_UART_Extend_disableInterrupt(LIN_0_INST, DL_UART_EXTEND_INTERRUPT_RXD_NEG_EDGE);
                 
              } else {
                  /* More sync segments to process */
                  gStateMachine = LIN_STATE_SYNC_FIELD_POS_EDGE;
              }

          } else {
              /* Sequence error - reset and wait for break */
              gStateMachine = LIN_STATE_WAIT_FOR_BREAK;
              gNumCyclesPos = 0;
              gNumCyclesNeg = 0;
              gNumCyclesErrorCount = 0;
              gSumOfTbits = 0;
              gAutoBaudRateDone = false;
          }
      
      break;
      
        case DL_UART_EXTEND_IIDX_RX:
            /* Determine if SYNC byte was received */
         if(gStateMachine == LIN_STATE_SYNC_FIELD_POS_EDGE)
         {
            data = DL_UART_Extend_receiveData(LIN_0_INST);
            gStateMachine = LIN_STATE_PID_FIELD;
         }
         else if (gStateMachine == LIN_STATE_PID_FIELD ) {

                data = DL_UART_Extend_receiveData(LIN_0_INST);
                LIN_Responder_receivePID(LIN_0_INST, data, gResponderTXPacket, responderMessageTable);
            }
                /* Signals the data field */
            else if (gStateMachine == LIN_STATE_DATA_FIELD) {
                data = DL_UART_Extend_receiveData(LIN_0_INST);
                DL_Timer_stopCounter(TIMER_0_INST);
                DL_Timer_setLoadValue(TIMER_0_INST, TIMEOUT);
                LIN_Responder_receiveMessage(data, gResponderRXBuffer, responderMessageTable);
               
                
            }
              /* Flushes any data that comes after an invalid PID*/
           else if (gStateMachine == LIN_STATE_WAIT_FOR_BREAK) {
                data = DL_UART_Extend_receiveData(LIN_0_INST);
            }
            break;

#ifdef Transmit_INT
        case DL_UART_EXTEND_IIDX_TX:
            LIN_Responder_transmitMessage(LIN_0_INST, gResponderTXPacket, responderMessageTable);
            break;
#endif

        default:
            break;
    }
}


void LIN_processMessage_Rx(void)
{
    memcpy(&gResponderTXPacket[0], &gResponderRXBuffer[0], 0x08);
    /* Toggle LED1 with RX packet */
    DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
}

void LIN_processMessage_Tx(void)
{
    /* Toggle LED2 with TX packet */
    DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);
}

