/*
* Copyright (c) 2024, Texas Instruments Incorporated
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

#include "dali_gpio_comm.h"

#define IS_IN_RANGE(data, min, max) ((data) >= (min) && (data) <= (max))

/* Function Declarations */
/**
 * @brief Enable Collision Detection Mechanism
 */
static void DALI_TX_enableCollisionDetection(void);

/**
 * @brief Reset DALI Tx 
 */
static void DALI_TX_ResetTx(void);

/**
 * @brief Check if Bit received in Rx is similar to Tx  
 */
static bool DALI_TX_checkForCollision(void);

/**
 * @brief Check if Timing of the bit Received is within tolerance range  
 */
static bool DALI_TX_checkTiming(void);

/**
 * @brief define procedure to handle Collision
 */
static void DALI_TX_handleCollision(void);

/**
 * @brief decode received bits from DALI Rx and returns true if it is a proper frame
 */
static bool DALI_RX_decodeFrame(void);

void DALI_TX_enableTimer_Output(void)
{
    DL_GPIO_initPeripheralInputFunction(DALI_TX_IOMUX,DALI_TX_TIMER_CAP_FUNC);

    DL_Timer_setCCPDirection(TIMER_DALI_TX_INST, DALI_TX_CC_OUTPUT);

    IOMUX->SECCFG.PINCM[DALI_TX_IOMUX] |= IOMUX_PINCM_INV_ENABLE;
}

static void DALI_TX_enableCollisionDetection(void)
{
    dali_gpio.Tx.isCDetectionActive = true;

    /* Configure CC Channel 1 for checking Collision Detection */
    DL_Timer_setCaptureCompareValue(TIMER_DALI_TX_INST, DALI_TX_DELAY_COLLISION_DETECTION, DALI_TX_CD_CC_INDEX);
    DL_Timer_clearInterruptStatus(TIMER_DALI_TX_INST , DALI_TX_INTR_CD_CC_UP_EVENT);
    DL_Timer_enableInterrupt(TIMER_DALI_TX_INST , DALI_TX_INTR_CD_CC_UP_EVENT );
}

static void DALI_TX_ResetTx(void)
{
    DL_Timer_stopCounter(TIMER_DALI_TX_INST);
    DL_Timer_disableInterrupt(TIMER_DALI_TX_INST , DALI_TX_INTR_CD_CC_UP_EVENT );
    DL_Timer_setCaptureCompareAction(TIMER_DALI_TX_INST,DL_TIMER_CC_LACT_DISABLED,DALI_TX_CC_INDEX);
    
    dali_gpio.Tx.tIndex = 0;
    dali_gpio.Tx.isCDetectionActive = false;
}

static bool DALI_TX_checkForCollision(void)
{
    if(dali_gpio.Rx.currBit != dali_gpio.Tx.currBit)
    {
        return true;
    }

    return false;
}

static bool DALI_TX_checkTiming(void)
{
    for(int i = 0 ; i < dali_gpio.Rx.captIndex; i++)
    {
        if((!IS_IN_RANGE(dali_gpio.Rx.captBitTimings[i],DALI_TX_CD_HALFBIT_MIN_CYC,DALI_TX_CD_HALFBIT_MAX_CYC))
                &&
           (!IS_IN_RANGE(dali_gpio.Rx.captBitTimings[i],DALI_TX_CD_BIT_MIN_CYC,DALI_TX_CD_BIT_MAX_CYC)))
        {
            /* Timing of the signal has been violated */
            return false;
        }
    }

    return true;
}

static void DALI_TX_handleCollision(void)
{
    if(DALI_TX_checkTiming())
    {
        /* Collision Avoidance */
        DALI_TX_ResetTx();
        dali_gpio.Tx.status = TX_COLLISION_AVOIDANCE;
    }

    else
    {
        /* Collision Recovery */
        dali_gpio.Tx.status = TX_COLLISION_BREAK;

        dali_gpio.Tx.isCDetectionActive = false;

        DL_TimerA_stopCounter(TIMER_DALI_TX_INST);
        DL_TimerA_setTimerCount(TIMER_DALI_TX_INST,0);

        DL_TimerA_setLoadValue(TIMER_DALI_TX_INST,TRANSMITTER_CR_BREAK_TIME*TIMER_DALI_TX_CLK_MHZ);

        DL_Timer_setCaptureCompareAction(TIMER_DALI_TX_INST, DL_TIMER_CC_LACT_DISABLED, DALI_TX_CC_INDEX);
        DL_Timer_overrideCCPOut(TIMER_DALI_TX_INST, DL_TIMER_FORCE_OUT_HIGH, DL_TIMER_FORCE_CMPL_OUT_HIGH, DALI_TX_CC_INDEX);

        DL_TimerA_clearInterruptStatus(TIMER_DALI_TX_INST, DL_TIMER_INTERRUPT_LOAD_EVENT);
        DL_TimerA_enableInterrupt(TIMER_DALI_TX_INST, DL_TIMER_INTERRUPT_LOAD_EVENT);

        DL_TimerA_startCounter(TIMER_DALI_TX_INST);
    }
}

static bool DALI_RX_decodeFrame(void)
{
    uint8_t halfBits[100] = {0}, halfBitIndex= 0;

    /* Check if start bit is proper */
    if(!(dali_gpio.Rx.captBits[0] == 1 && dali_gpio.Rx.captBits[1] == 0))
        return false;


    for(int i=0 ; i < dali_gpio.Rx.captIndex ; i++)
    {
        if(i > 0 && (dali_gpio.Rx.captBits[i] == dali_gpio.Rx.captBits[i-1]))
            return false;

        if(IS_IN_RANGE(dali_gpio.Rx.captBitTimings[i], DALI_RX_HALFBIT_MIN_CYC,DALI_RX_HALFBIT_MAX_CYC))
        {
            halfBits[halfBitIndex++] = dali_gpio.Rx.captBits[i];
        }

        else if(IS_IN_RANGE(dali_gpio.Rx.captBitTimings[i], DALI_RX_BIT_MIN_CYC,DALI_RX_BIT_MAX_CYC))
        {
            halfBits[halfBitIndex++] = dali_gpio.Rx.captBits[i];
            halfBits[halfBitIndex++] = dali_gpio.Rx.captBits[i];
        }

        else
        {
            return false;
        }
    }

    /* Last bit is not counted if it is high ,so incrementing the index if it is odd*/
    if(halfBitIndex % 2 == 1)
    {
        halfBitIndex++;
    }

    /* Check if it contains all the bits (start bit and 8 bit frames )*/
    if((halfBitIndex - 2) % 16  != 0)
    {
        return false;
    }

    /* Check for Send Twice commands */
    uint32_t prevRxData = 0;
    for(int i = 0; i < dali_gpio.Rx.dataSize; i++)
    {
        prevRxData = prevRxData << 8;
        prevRxData |= dali_gpio.Rx.dataArr[i];
    }


    dali_gpio.Rx.dataSize = (halfBitIndex - 2) / 16;

    /* Decode Rx Data */
    uint32_t RxData = 0;

    for(int i = 2 ; i < halfBitIndex; i += 2)
    {
       if(halfBits[i] == 0 && halfBits[i + 1] == 1)
       {
           RxData = (RxData << 1) | 0 ;
       }

       else
       {
           RxData = (RxData << 1) | 1 ;
       }
    }



    /* Check if command follows Send Twice Command conditions */
    dali_gpio.Rx.isSendTwiceCmd = false;

    if( (prevRxData == RxData)
        &&
        IS_IN_RANGE( DL_TimerG_getTimerCount(SETTLING_TIMER_INST)*SETTLING_TIMER_TICK, \
                    SEND_TWICE_SETTLING_MIN , SEND_TWICE_SETTLING_MAX))
    {
        dali_gpio.Rx.isSendTwiceCmd = true;
    }

    for(int i = dali_gpio.Rx.dataSize - 1; i >= 0 ; i--)
    {
        dali_gpio.Rx.dataArr[i] = RxData & 0xFF;
        RxData = RxData >> 8;
    }


    return true;
}

void DALI_TX_transmitFrame(void)
{
    dali_gpio.Tx.tBitsCount = 0;

    dali_gpio.Tx.transmitBits[dali_gpio.Tx.tBitsCount++] = 1;
    dali_gpio.Tx.transmitBits[dali_gpio.Tx.tBitsCount++] = 0;

    for(int j = 0; j < dali_gpio.Tx.dataSize ; j++)
    {
        for(int i = 7; i >= 0 ;i--)
        {
            if(dali_gpio.Tx.dataArr[j] & (1 << i))
            {
                dali_gpio.Tx.transmitBits[dali_gpio.Tx.tBitsCount++] = 1;
                dali_gpio.Tx.transmitBits[dali_gpio.Tx.tBitsCount++] = 0;
            }

            else
            {
                dali_gpio.Tx.transmitBits[dali_gpio.Tx.tBitsCount++] = 0;
                dali_gpio.Tx.transmitBits[dali_gpio.Tx.tBitsCount++] = 1;  
            }
        }
    }

    /* Stop Condition */
    dali_gpio.Tx.transmitBits[dali_gpio.Tx.tBitsCount++] = 0;

    dali_gpio.Tx.status = TX_TRANSMIT_INPROGRESS;

    /* First rising edge is not detected in ISR (Rx.currBit is not updated), so setting init value as 1 to Pass Collision Detection */
    dali_gpio.Rx.currBit = 1;

    DL_Timer_startCounter(TIMER_DALI_TX_INST);

    while(dali_gpio.Tx.status == TX_TRANSMIT_INPROGRESS)
    {};

    if(dali_gpio.Tx.status == TX_COLLISION_DETECTED)
    {
        DALI_TX_handleCollision();

        while((dali_gpio.Tx.status != TX_COLLISION_AVOIDANCE)
              && (dali_gpio.Tx.status != TX_COLLISION_RECOVER)
              && (dali_gpio.Tx.status != TX_IDLE))
        {};

        if(dali_gpio.Tx.status == TX_COLLISION_RECOVER)
        {
            dali_gpio.Tx.settlingTime = TRANSMITTER_CR_RECOVERY_TIME; 
        } 
    }

    return ;
}


void TIMER_DALI_TX_INST_IRQHandler(void)
{
    switch (DL_Timer_getPendingInterrupt(TIMER_DALI_TX_INST)) {

    case DL_TIMER_IIDX_ZERO:
        if(dali_gpio.Tx.status == TX_TRANSMIT_INPROGRESS)
        {
            if(dali_gpio.Tx.tIndex == dali_gpio.Tx.tBitsCount)
            {
                dali_gpio.Tx.status = TX_TRANSMIT_SUCCESS;
                DALI_TX_ResetTx();
                break;
            }

            /* Check if Tx is driving the line high or low */
            if(dali_gpio.Tx.tIndex > 0 )
            {
                dali_gpio.Tx.currBit = dali_gpio.Tx.transmitBits[dali_gpio.Tx.tIndex - 1];

                /* Enable Collision Detection */
                DALI_TX_enableCollisionDetection();
            }

            if(dali_gpio.Tx.transmitBits[dali_gpio.Tx.tIndex++] & 0x1)
            {
                DL_Timer_setCaptureCompareAction(TIMER_DALI_TX_INST,DL_TIMER_CC_LACT_CCP_HIGH,DALI_TX_CC_INDEX);
            }
            else
            {
                DL_Timer_setCaptureCompareAction(TIMER_DALI_TX_INST,DL_TIMER_CC_LACT_CCP_LOW,DALI_TX_CC_INDEX);
            }

        }
        break;

    case DL_TIMER_IIDX_CC1_UP:
        if(dali_gpio.Tx.isCDetectionActive)
        {
            bool isCollisionDetected = DALI_TX_checkForCollision();

            /* Check if collision is detected while transmitting Frame */
            if(dali_gpio.Tx.status == TX_TRANSMIT_INPROGRESS  && isCollisionDetected)
            {
                DALI_TX_ResetTx();
                dali_gpio.Tx.status = TX_COLLISION_DETECTED;
            }

            
            if(dali_gpio.Tx.status == TX_COLLISION_BREAK)
            {
                /* Check if bus is still in active state, after the break time */
                if(isCollisionDetected)
                {
                    DALI_TX_ResetTx();
                    dali_gpio.Tx.status = TX_COLLISION_AVOIDANCE;
                }

                else
                {
                    DALI_TX_ResetTx();
                    dali_gpio.Tx.status = TX_COLLISION_RECOVER;
                }
            }
        }
        break;

    case DL_TIMER_IIDX_LOAD:
        if(dali_gpio.Tx.status == TX_COLLISION_BREAK)
        {
            DL_TimerA_stopCounter(TIMER_DALI_TX_INST);

            /* Reset Load Value to Tx Half Bit time */
            DL_TimerA_setLoadValue(TIMER_DALI_TX_INST,TIMER_DALI_TX_INST_LOAD_VALUE);

            DL_TimerA_disableInterrupt(TIMER_DALI_TX_INST, DL_TIMER_INTERRUPT_LOAD_EVENT);
            DL_Timer_overrideCCPOut(TIMER_DALI_TX_INST, DL_TIMER_FORCE_OUT_DISABLED, DL_TIMER_FORCE_CMPL_OUT_DISABLED, DALI_TX_CC_INDEX);
            
            dali_gpio.Tx.currBit = 0;

            /* Enable Collision Detection to check if bus is in idle state for reduced settling time */
            DALI_TX_enableCollisionDetection();

            DL_TimerA_startCounter(TIMER_DALI_TX_INST);
        }
        break;

    default:
        break;
    }
}

void TIMER_DALI_RX_INST_IRQHandler(void)
{
    switch (DL_Timer_getPendingInterrupt(TIMER_DALI_RX_INST))
    {

        /* Rising Edge */
        case DL_TIMER_IIDX_CC1_UP:
            /* Reset Timer Counter */
            DL_Timer_setTimerCount(TIMER_DALI_RX_INST, 0);

            dali_gpio.Rx.isActive = true;
            dali_gpio.Rx.currBit = 1;

            /* Additional check for collision detection */
            if(dali_gpio.Tx.isCDetectionActive && (dali_gpio.Rx.currBit != dali_gpio.Tx.currBit))
            {
                if(dali_gpio.Tx.status == TX_TRANSMIT_INPROGRESS)
                {
                    DALI_TX_ResetTx();
                    dali_gpio.Tx.status = TX_COLLISION_DETECTED;
                }
            }

            if(dali_gpio.Rx.captIndex < 100)
            {
                dali_gpio.Rx.captBits[dali_gpio.Rx.captIndex] = 0;
                dali_gpio.Rx.captBitTimings[dali_gpio.Rx.captIndex++] = DL_TimerG_getCaptureCompareValue(TIMER_DALI_RX_INST,DL_TIMER_CC_1_INDEX) \
                                        - DL_TimerG_getCaptureCompareValue(TIMER_DALI_RX_INST,DL_TIMER_CC_0_INDEX);
            }

            break;

        /* Falling Edge */
        case DL_TIMER_IIDX_CC0_UP:
            /* Set Trigger for Rising Edge */
            DL_TimerG_setCaptureCompareCtl(TIMER_DALI_RX_INST,
            DL_TIMER_CC_MODE_CAPTURE, (DL_TIMER_CC_ZCOND_NONE | DL_TIMER_CC_ACOND_TIMCLK | DL_TIMER_CC_CCOND_TRIG_RISE),
            DL_TIMER_CC_1_INDEX);

            dali_gpio.Rx.isActive = true;
            dali_gpio.Rx.currBit = 0;

            /* Additional check for collision detection */
            if(dali_gpio.Tx.isCDetectionActive && (dali_gpio.Rx.currBit != dali_gpio.Tx.currBit))
            {
                if(dali_gpio.Tx.status == TX_TRANSMIT_INPROGRESS)
                {
                    DALI_TX_ResetTx();
                    dali_gpio.Tx.status = TX_COLLISION_DETECTED;
                }
            }

            if(dali_gpio.Rx.captIndex < 100)
            {

                dali_gpio.Rx.captBits[dali_gpio.Rx.captIndex] = 1;
                dali_gpio.Rx.captBitTimings[dali_gpio.Rx.captIndex++] = DL_TimerG_getCaptureCompareValue(TIMER_DALI_RX_INST,DL_TIMER_CC_0_INDEX);
            }

            break;

        /* Stop Condition */
        case DL_TIMER_IIDX_LOAD:
            DL_TimerG_stopCounter(TIMER_DALI_RX_INST);

            /* Decode captured bits only if the signal is received from another device */
            if(dali_gpio.Tx.status == TX_IDLE)
            {   
                /* Verify if it's a proper frame */
                if(DALI_RX_decodeFrame())
                {
                    /* set a Flag indicating new command is received */
                    gReceive = true;

                }
                
                /* Handle Incorrect Rx Msg */
                else
                {
                    dali_gpio.Rx.dataArr[0] = 0xFF;
                    dali_gpio.Rx.dataArr[1] = 0xFF;
                    dali_gpio.Rx.dataArr[2] = 0xFF;
                    dali_gpio.Rx.dataArr[3] = 0xFF;
                }
            }

            /* Reset Settling Timer Counter */
            DL_Timer_setTimerCount(SETTLING_TIMER_INST, 0);
            DL_Timer_startCounter(SETTLING_TIMER_INST);

            /* Set bus status as Idle */
            dali_gpio.Rx.isActive = false;
            dali_gpio.Tx.status = TX_IDLE;

            dali_gpio.Rx.captIndex = 0;

            /* Re-Configure ZCOND to start the timer for Rising edge */
            DL_TimerG_setCaptureCompareCtl(TIMER_DALI_RX_INST,
            DL_TIMER_CC_MODE_CAPTURE, (DL_TIMER_CC_ZCOND_TRIG_RISE | DL_TIMER_CC_ACOND_TIMCLK | DL_TIMER_CC_CCOND_TRIG_RISE),
            DL_TIMER_CC_1_INDEX);
        
            break;

        default:
            break;
    }
}
