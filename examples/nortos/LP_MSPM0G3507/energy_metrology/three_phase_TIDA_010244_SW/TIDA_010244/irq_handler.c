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

//*****************************************************************************
// the includes
//*****************************************************************************

#include "TIDA_010244.h"

/*! @brief Defines AMC1 data is ready  */
#define AMC1_DATA_READY       0x10
/*! @brief Defines AMC2 data is ready  */
#define AMC2_DATA_READY       0x20
/*! @brief Defines AMC3 data is ready  */
#define AMC3_DATA_READY       0x40
/*! @brief Defines AMC3 data is ready  */
#define AMC4_DATA_READY       0x80
/*! @brief Defines all ADC data is ready */
#define ADC_DATA_READY        (AMC1_DATA_READY |AMC2_DATA_READY |AMC3_DATA_READY |AMC4_DATA_READY)
/*! @brief Defines waiting for ADC data  */
#define ADC_DATA_WAIT         0x00
/*!
 * @brief Define when testing with lookup table
 *        Undefine when testing with actual input
 */
#undef DEBUG

#ifdef DEBUG
/*! @brief Defines square root of 2 */
#define SQRT2                (1.414213562373095)
/*! @brief Defines voltage scale factor */
#define V_SCALING            (1585.2)
/*! @brief Defines current scale factor */
#define I_SCALING            (125)
/*! @brief Defines RMS voltage input */
#define V_RMS                (120)
/*! @brief Defines RMS current input */
#define I_RMS                (10)
/*! @brief Defines samples per cycle */
#define SAMPLES_PER_CYCLE    (80)
/*! @brief Defines 2 * PI value */
#define TWO_PI               (6.2831853)
/*! @brief Defines voltage start index  */
#define VOLTAGE_START_INDEX_DS    (0)
/*! @brief Defines current start index  */
#define CURRENT_START_INDEX_DS    (0)
/*! @brief Defines phase increment  */
#define PHASE_INCREMENT           (27)
/*! @brief array to store the indexes of voltage    */
uint32_t voltageIndex[3] = {0,0,0};
/*! @brief array to store the indexes of current    */
uint32_t currentIndex[3] = {0,0,0};
/*! @brief Store the index of neutral current    */
uint32_t neutralIndex = 0;
/*! @brief Array to store voltage debug data    */
_iq30 voltage[SAMPLES_PER_CYCLE];
/*! @brief Array to store current debug data    */
_iq30 current[SAMPLES_PER_CYCLE];
#endif

/*! @enum DMA_WAIT */
typedef enum
{
    /*! @brief DMA is free  */
    DMA_WAIT_NONE = 0x00,
    /*! @brief DMA is waiting for AMC1 interrupt */
    DMA_WAIT_AMC1,
    /*! @brief DMA is waiting for AMC2 interrupt */
    DMA_WAIT_AMC2,
    /*! @brief DMA is waiting for AMC3 interrupt */
    DMA_WAIT_AMC3,
    /*! @brief DMA is waiting for AMC4 interrupt */
    DMA_WAIT_AMC4
}DMA_WAIT;

/*! @enum PENDING_GPIO_INTERRUPT */
typedef enum
{
    /*! @brief No GPIO interrupts are pending   */
    PENDING_GPIO_INTERRUPT_NONE = 0x00,
    /*! @brief AMC1 gpio interrupt is pending    */
    PENDING_GPIO_INTERRUPT_AMC1 = 0x01,
    /*! @brief AMC2 gpio interrupt is pending    */
    PENDING_GPIO_INTERRUPT_AMC2 = 0x02,
    /*! @brief AMC3 gpio interrupt is pending    */
    PENDING_GPIO_INTERRUPT_AMC3 = 0x04,
    /*! @brief AMC3 gpio interrupt is pending    */
    PENDING_GPIO_INTERRUPT_AMC4 = 0x08
}PENDING_GPIO_INTERRUPT;

/*! @brief Stores adc status    */
uint8_t adcDataStatus = ADC_DATA_WAIT;
/*! @brief Stores DMA wait status   */
DMA_WAIT dmaWait = DMA_WAIT_NONE;
/*! @brief Stores pending gpio interrupts   */
uint8_t pendingGPIOInterrupt = PENDING_GPIO_INTERRUPT_NONE;
/*! @brief uart communication stage */
volatile int8_t uartStage = 0;

/*!
 * @brief Debug init
 *        initializes the lookup table
 */
void debugInit(void)
{
#ifdef DEBUG
    for(int i = PHASE_ONE; i < MAX_PHASES; i++)
    {
        voltageIndex[i] = VOLTAGE_START_INDEX_DS + (i * PHASE_INCREMENT);
        currentIndex[i] = CURRENT_START_INDEX_DS + (i * PHASE_INCREMENT);
    }
    neutralIndex = CURRENT_START_INDEX_DS;

    for(int i = 0; i < SAMPLES_PER_CYCLE; i++)
    {
        _iq23 t = _IQ23mpy(_IQ23div(_IQ23(i),_IQ23(SAMPLES_PER_CYCLE)), _IQ23(TWO_PI));

        voltage[i] = _IQ30mpy(_IQ30((V_RMS*SQRT2)/V_SCALING), _IQ23toIQ30(_IQ23sin(t)));
        current[i] = _IQ30mpy(_IQ30(I_RMS*SQRT2/I_SCALING), _IQ23toIQ30(_IQ23sin(t)));
    }
#endif
}

/*! @brief SPI Handler  */
void GEN_SPI_0_INST_IRQHandler(void)
{
    switch (DL_SPI_getPendingInterrupt(GEN_SPI_0_INST))
    {
    case DL_SPI_IIDX_DMA_DONE_TX:

        break;
    case DL_SPI_IIDX_TX_EMPTY:

        break;
    case DL_SPI_IIDX_DMA_DONE_RX:

        break;
    default:
        break;
    }
}

/*!
 * @brief get debug data
 * @param[in] workingData The metrology instance
 */
void get_debugData(metrologyData *workingData)
{
#ifdef DEBUG
    if (voltageIndex[0] == 160)
    {
        voltageIndex[0] = 0;
    }
    if (voltageIndex[1] == 160)
    {
        voltageIndex[1] = 0;
    }
    if (voltageIndex[2] == 160)
    {
        voltageIndex[2] = 0;
    }
    if (currentIndex[0] == 160)
    {
        currentIndex[0] = 0;
    }
    if (currentIndex[1] == 160)
    {
        currentIndex[1] = 0;
    }
    if (currentIndex[2] == 160)
    {
        currentIndex[2] = 0;
    }

    workingData->rawVoltageData[PHASE_ONE]      = _IQ30toIQ23(voltage[voltageIndex[0]]);
    workingData->rawVoltageData[PHASE_TWO]      = _IQ30toIQ23(voltage[voltageIndex[1]]);
    workingData->rawVoltageData[PHASE_THERE]    = _IQ30toIQ23(voltage[voltageIndex[2]]);
    workingData->rawCurrentData[PHASE_ONE]      = _IQ30toIQ23(current[currentIndex[0]]);
    workingData->rawCurrentData[PHASE_TWO]      = _IQ30toIQ23(current[currentIndex[1]]);
    workingData->rawCurrentData[PHASE_THERE]    = _IQ30toIQ23(current[currentIndex[2]]);
    workingData->rawNeutralData                 = _IQ30toIQ23(current[currentIndex[0]]) +
                                                  _IQ30toIQ23(current[currentIndex[1]]) +
                                                  _IQ30toIQ23(current[currentIndex[2]]);

    voltageIndex[0]++;
    currentIndex[0]++;
    voltageIndex[1]++;
    currentIndex[1]++;
    voltageIndex[2]++;
    currentIndex[2]++;
#endif
}

/*!
 * @brief get ADC data
 * @param[in] workingData  The metrology instance
 * @param[in] amcHandle1   The AMC1 instance
 * @param[in] amcHandle2   The AMC2 instance
 * @param[in] amcHandle3   The AMC3 instance
 * @param[in] amcHandle4   The AMC4 instance
 */
void get_sampleData(metrologyData *workingData,volatile AMC_Instance *amcHandle1, volatile AMC_Instance *amcHandle2, volatile AMC_Instance *amcHandle3, volatile AMC_Instance *amcHandle4)
{
    /*
     * In case a reset occurred on any of the AMC
     * Disable the interrupts, initialize the AMC,
     * Enable the interrupts.
     */
    if(amcHandle1->channelData.response & STATUS_RESET_MASK)
    {
        gTidaHandle.adcStatus = ADC_INIT;
        NVIC_DisableIRQ(UART_0_INST_INT_IRQN);
        NVIC_DisableIRQ(GEN_GPIO_IN_INT_IRQN);
        AMC131M03_init(amcHandle1);
    }
    else
    {
        workingData->rawCurrentData[PHASE_ONE]      = amcHandle1->channelData.chan0;
        workingData->rawVoltageData[PHASE_ONE]      = amcHandle1->channelData.chan1;
    }
    if(amcHandle2->channelData.response & STATUS_RESET_MASK)
    {
        gTidaHandle.adcStatus = ADC_INIT;
        NVIC_DisableIRQ(UART_0_INST_INT_IRQN);
        NVIC_DisableIRQ(GEN_GPIO_IN_INT_IRQN);
        AMC131M03_init(amcHandle2);
    }
    else
    {
        workingData->rawCurrentData[PHASE_TWO]      = amcHandle2->channelData.chan0;
        workingData->rawVoltageData[PHASE_TWO]      = amcHandle2->channelData.chan1;
    }
    if(amcHandle3->channelData.response & STATUS_RESET_MASK)
    {
        gTidaHandle.adcStatus = ADC_INIT;
        NVIC_DisableIRQ(UART_0_INST_INT_IRQN);
        NVIC_DisableIRQ(GEN_GPIO_IN_INT_IRQN);
        AMC131M03_init(amcHandle3);
    }
    else
    {
        workingData->rawCurrentData[PHASE_THERE]    = amcHandle3->channelData.chan0;
        workingData->rawVoltageData[PHASE_THERE]    = amcHandle3->channelData.chan1;
    }
    if(amcHandle4->channelData.response & STATUS_RESET_MASK)
    {
        gTidaHandle.adcStatus = ADC_INIT;
        NVIC_DisableIRQ(UART_0_INST_INT_IRQN);
        NVIC_DisableIRQ(GEN_GPIO_IN_INT_IRQN);
        AMC131M03_init(amcHandle4);
    }
    else
    {
        workingData->rawNeutralData                 = amcHandle4->channelData.chan0;
    }

    if(amcHandle1->channelData.response & STATUS_RESET_MASK ||
       amcHandle2->channelData.response & STATUS_RESET_MASK ||
       amcHandle3->channelData.response & STATUS_RESET_MASK ||
       amcHandle4->channelData.response & STATUS_RESET_MASK)
    {
        TIDA_startDataCollection(&gTidaHandle);
        NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
        NVIC_EnableIRQ(GEN_GPIO_IN_INT_IRQN);
    }
}

/*! @brief GPIO Handler */
void GROUP1_IRQHandler(void)
{
    uint32_t interruptStatus;

    interruptStatus = HAL_getGPIOEnabledInterruptStatus(gAMCHandle1.ready);
    if((interruptStatus & gpioInputPin[gAMCHandle1.ready].pin) == gpioInputPin[gAMCHandle1.ready].pin)
    {
        switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1))
        {
            case GEN_GPIO_IN_INT_IIDX:
                if(dmaWait == DMA_WAIT_NONE)
                {
                    dmaWait = DMA_WAIT_AMC1;
                    AMC_startDMADataTransfer(&gAMCHandle1);
                }
                else
                {
                    pendingGPIOInterrupt |= PENDING_GPIO_INTERRUPT_AMC1;
                }
                break;
            default:
                /* Do Nothing */
                break;
        }
        HAL_clearGPIOInterruptStatus(gAMCHandle1.ready);
    }

    interruptStatus = HAL_getGPIOEnabledInterruptStatus(gAMCHandle2.ready);
    if((interruptStatus & gpioInputPin[gAMCHandle2.ready].pin) == gpioInputPin[gAMCHandle2.ready].pin)
    {
        switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1))
        {
            case GEN_GPIO_IN_INT_IIDX:
                if(dmaWait == DMA_WAIT_NONE)
                {
                    dmaWait = DMA_WAIT_AMC2;
                    AMC_startDMADataTransfer(&gAMCHandle2);
                }
                else
                {
                    pendingGPIOInterrupt |= PENDING_GPIO_INTERRUPT_AMC2;
                }
                break;
            default:
                /* Do Nothing */
                break;
        }
        HAL_clearGPIOInterruptStatus(gAMCHandle2.ready);
    }

    interruptStatus = HAL_getGPIOEnabledInterruptStatus(gAMCHandle3.ready);
    if((interruptStatus & gpioInputPin[gAMCHandle3.ready].pin) == gpioInputPin[gAMCHandle3.ready].pin)
    {
        switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1))
        {
            case GEN_GPIO_IN_INT_IIDX:
                if(dmaWait == DMA_WAIT_NONE)
                {
                    dmaWait = DMA_WAIT_AMC3;
                    AMC_startDMADataTransfer(&gAMCHandle3);
                }
                else
                {
                    pendingGPIOInterrupt |= PENDING_GPIO_INTERRUPT_AMC3;
                }
                break;
            default:
                /* Do Nothing */
                break;
        }
        HAL_clearGPIOInterruptStatus(gAMCHandle3.ready);
    }

    interruptStatus = HAL_getGPIOEnabledInterruptStatus(gAMCHandle4.ready);
    if((interruptStatus & gpioInputPin[gAMCHandle4.ready].pin) == gpioInputPin[gAMCHandle4.ready].pin)
    {
        switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1))
        {
            case GEN_GPIO_IN_INT_IIDX:
                if(dmaWait == DMA_WAIT_NONE)
                {
                    dmaWait = DMA_WAIT_AMC4;
                    AMC_startDMADataTransfer(&gAMCHandle4);
                }
                else
                {
                    pendingGPIOInterrupt |= PENDING_GPIO_INTERRUPT_AMC4;
                }
                break;
            default:
                /* Do Nothing */
                break;
        }
        HAL_clearGPIOInterruptStatus(gAMCHandle4.ready);
    }
}

/*! @brief DMA Handler  */
void DMA_IRQHandler(void)
{
    if(gTidaHandle.adcStatus == ADC_READY)
    {
        switch(DL_DMA_getPendingInterrupt(DMA))
        {
        case DL_DMA_EVENT_IIDX_DMACH2:
           if(uartStage == 0)
           {
               int8_t len = gDLT645.rxMsg.buf.uint8[DLT645_PREAMBLE_BYTES + DLT645_MESSAGE_HEADER_BYTES - 1];
               uartStage = 1;
               DL_DMA_setTransferSize(DMA,DMA_CH2_CHAN_ID,len + DLT645_MESSAGE_TRAILER_BYTES);
               DL_DMA_enableChannel(DMA, DMA_CH2_CHAN_ID);
           }
           else if(uartStage == 1)
           {
               uartStage = 2;
               for(int i = DLT645_PREAMBLE_BYTES;i < MAX_SERIAL_MESSAGE_LEN;i++)
               {
                   gDLT645.rxMsg.buf.uint8[i-DLT645_PREAMBLE_BYTES] = gDLT645.rxMsg.buf.uint8[i];
               }
               DLT645_UARTRxByte(&gDLT645);
           }
           else
           {
               /* Do Nothing    */
           }
            break;
        case DL_DMA_EVENT_IIDX_DMACH3:
            DLT645_UARTRxdmaInit(&gDLT645);
            uartStage = 0;
            break;
        case DL_DMA_EVENT_IIDX_DMACH1:
            switch(dmaWait)
            {
            case DMA_WAIT_AMC1:
                adcDataStatus |= AMC1_DATA_READY;
                break;
            case DMA_WAIT_AMC2:
                adcDataStatus |= AMC2_DATA_READY;
                break;
            case DMA_WAIT_AMC3:
                adcDataStatus |= AMC3_DATA_READY;
                break;
            case DMA_WAIT_AMC4:
                adcDataStatus |= AMC4_DATA_READY;
                break;
            default:
                /* Incorrect DMA interrupt  */
                break;
            }

            if(pendingGPIOInterrupt & PENDING_GPIO_INTERRUPT_AMC1)
            {
                dmaWait = DMA_WAIT_AMC1;
                AMC_startDMADataTransfer(&gAMCHandle1);
                pendingGPIOInterrupt &= ~PENDING_GPIO_INTERRUPT_AMC1;
            }
            else if(pendingGPIOInterrupt & PENDING_GPIO_INTERRUPT_AMC2)
            {
                dmaWait = DMA_WAIT_AMC2;
                AMC_startDMADataTransfer(&gAMCHandle2);
                pendingGPIOInterrupt &= ~PENDING_GPIO_INTERRUPT_AMC2;
            }
            else if(pendingGPIOInterrupt & PENDING_GPIO_INTERRUPT_AMC3)
            {
                dmaWait = DMA_WAIT_AMC3;
                AMC_startDMADataTransfer(&gAMCHandle3);
                pendingGPIOInterrupt &= ~PENDING_GPIO_INTERRUPT_AMC3;
            }
            else if(pendingGPIOInterrupt & PENDING_GPIO_INTERRUPT_AMC4)
            {
                dmaWait = DMA_WAIT_AMC4;
                AMC_startDMADataTransfer(&gAMCHandle4);
                pendingGPIOInterrupt &= ~PENDING_GPIO_INTERRUPT_AMC4;
            }
            else
            {
                dmaWait = DMA_WAIT_NONE;
                pendingGPIOInterrupt = PENDING_GPIO_INTERRUPT_NONE;
            }
            if(adcDataStatus == ADC_DATA_READY)
            {
                AMC_getChannelData(&gAMCHandle1);
                AMC_getChannelData(&gAMCHandle2);
                AMC_getChannelData(&gAMCHandle3);
                AMC_getChannelData(&gAMCHandle4);
        #ifdef DEBUG
                get_debugData(&gMetrologyworkingData);
        #else
                get_sampleData(&gMetrologyworkingData,&gAMCHandle1, &gAMCHandle2, &gAMCHandle3, &gAMCHandle4);
        #endif
                Metrology_perSampleProcessing(&gMetrologyworkingData);
                Metrology_perSampleEnergyPulseProcessing(&gMetrologyworkingData);
                adcDataStatus = ADC_DATA_WAIT;
            }
            break;
        default:
            /* Incorrect DMA interrupt  */
            break;
        }
    }
    else
    {
        switch(DL_DMA_getPendingInterrupt(DMA))
        {
        case DL_DMA_EVENT_IIDX_DMACH1:
            if(gAMCHandle1.DMAWait == AMC_DMA_WAITING)
            {
                gAMCHandle1.DMAStatus = AMC_DMA_TRANSFER_DONE;
                gAMCHandle1.DMAWait = AMC_DMA_IDLE;
            }
            else if(gAMCHandle2.DMAWait == AMC_DMA_WAITING)
            {
                gAMCHandle2.DMAStatus = AMC_DMA_TRANSFER_DONE;
                gAMCHandle2.DMAWait = AMC_DMA_IDLE;
            }
            else if(gAMCHandle3.DMAWait == AMC_DMA_WAITING)
            {
                gAMCHandle3.DMAStatus = AMC_DMA_TRANSFER_DONE;
                gAMCHandle3.DMAWait = AMC_DMA_IDLE;
            }
            else if(gAMCHandle4.DMAWait == AMC_DMA_WAITING)
            {
                gAMCHandle4.DMAStatus = AMC_DMA_TRANSFER_DONE;
                gAMCHandle4.DMAWait = AMC_DMA_IDLE;
            }
            else
            {
                /* Incorrect DMA interrupt  */
            }
            break;
        default:
            /* Incorrect DMA interrupt  */
            break;
        }
    }
}


