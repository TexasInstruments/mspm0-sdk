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
#include "TIDA_010940.h"

/*! @brief Defines ADS01 data is ready  */
#define ADS_DATA_1_READY     (0x01)
/*! @brief Defines all ADS data is ready */
#define ADC_DATA_READY       ADS_DATA_1_READY
/*! @brief Defines waiting for ADC data  */
#define ADC_DATA_WAIT        (0x00)
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
#define SAMPLES_PER_CYCLE    (160)
/*! @brief Defines 2 * PI value */
#define TWO_PI               (6.2831853)
/*! @brief Defines voltage start index  */
#define VOLTAGE_START_INDEX_DS    (0)
/*! @brief Defines current start index  */
#define CURRENT_START_INDEX_DS    (0)
/*! @brief array to store the indexes of voltage    */
uint32_t voltageIndex = 0;
/*! @brief array to store the indexes of current    */
uint32_t currentIndex = 0;
/*! @brief Store the index of neutral current    */
uint32_t neutralIndex = 0;
/*! @brief Array to store voltage debug data    */
_iq30 voltage[SAMPLES_PER_CYCLE];
/*! @brief Array to store current debug data    */
_iq30 current[SAMPLES_PER_CYCLE];
#endif
/*! @brief uart communication stage */
volatile int8_t uartStage = 0;
/*! @brief Stores Ads status    */
uint8_t adsDataStatus = ADC_DATA_WAIT;

/*!
 * @brief Debug init
 *        initializes the lookup table
 */
void debugInit(void)
{
#ifdef DEBUG
    voltageIndex = VOLTAGE_START_INDEX_DS;
    currentIndex = CURRENT_START_INDEX_DS;
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
    if (voltageIndex == 160)
    {
        voltageIndex = 0;
    }
    if (currentIndex == 160)
    {
        currentIndex = 0;
    }
    if (neutralIndex == 160)
    {
        neutralIndex = 0;
    }

    workingData->rawVoltageData[PHASE_ONE]      = _IQ30toIQ23(voltage[voltageIndex]);
    workingData->rawCurrentData[PHASE_ONE]      = _IQ30toIQ23(current[currentIndex]);
    workingData->rawNeutralData                 = _IQ30toIQ23(current[neutralIndex]);

    voltageIndex++;
    currentIndex++;
    neutralIndex++;
#endif
}

/*!
 * @brief get ADS data
 * @param[in] workingData The metrology instance
 * @param[in] adsHandle   The ADS instance
 */
void get_adsData(metrologyData *workingData, volatile ADS_Instance *adsHandle)
{
    if(adsHandle->channelData.response & STATUS_RESET_MASK)
    {
        /*
         * In case a reset occurred on ADS
         * Disable the interrupts, initialize the ADS,
         * Enable the interrupts.
         */
        gTidaHandle.adcStatus = ADC_INIT;
        NVIC_DisableIRQ(GEN_UART_0_INST_INT_IRQN);
        NVIC_DisableIRQ(GEN_GPIO_IN_INT_IRQN);

        ADS131M03_init(adsHandle);

        TIDA_startDataCollection(&gTidaHandle);
        NVIC_EnableIRQ(GEN_UART_0_INST_INT_IRQN);
        NVIC_EnableIRQ(GEN_GPIO_IN_INT_IRQN);
    }
    else
    {
    #ifdef SINGLE_PHASE_SUPPORT
        workingData->rawVoltageData[PHASE_ONE]      = adsHandle->channelData.chan2;
        workingData->rawCurrentData[PHASE_ONE]      = adsHandle->channelData.chan0;
    #endif
    }
}

/*! @brief GPIO Handler */
void GROUP1_IRQHandler(void)
{
    uint32_t interruptStatus;
    interruptStatus = HAL_getGPIOEnabledInterruptStatus(gADSHandle.ready);
    if((interruptStatus & gpioInputPin[gADSHandle.ready].pin) == gpioInputPin[gADSHandle.ready].pin)
    {
        switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1))
        {
            case GEN_GPIO_GROUP_A_IIDX:
                ADS_startDMADataTransfer(&gADSHandle);
                break;
            default:
                /* Do Nothing */
                break;
        }
        HAL_clearGPIOInterruptStatus(gADSHandle.ready);
    }

    interruptStatus = DL_GPIO_getEnabledInterruptStatus(GPIO_GRP_0_MENU_SW_PORT, GPIO_GRP_0_MENU_SW_PIN);

    if((interruptStatus & GPIO_GRP_0_MENU_SW_PIN) == GPIO_GRP_0_MENU_SW_PIN)
    {
        switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1))
        {
            case GEN_GPIO_GROUP_A_IIDX:
                TIDA_displayOnLCD(&gLCDHandle);
                break;
            default:
                /* Do Nothing */
                break;
        }
        DL_GPIO_clearInterruptStatus(GPIO_GRP_0_MENU_SW_PORT, GPIO_GRP_0_MENU_SW_PIN);
    }
}

/*! @brief DMA Handler  */
void DMA_IRQHandler(void)
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
        if(gTidaHandle.adcStatus == ADC_READY)
        {
        adsDataStatus = ADC_DATA_READY;
        }
        else
        {
            if(gADSHandle.DMAWait == ADS_DMA_WAITING)
            {
                gADSHandle.DMAStatus = ADS_DMA_TRANSFER_DONE;
                gADSHandle.DMAWait = ADS_DMA_IDLE;
            }
            else
            {
                /* Incorrect DMA interrupt  */
            }
        }
        break;
    default:
        break;
    }
    if(adsDataStatus == ADC_DATA_READY)
    {
        ADS_getChannelData(&gADSHandle);
#ifdef DEBUG
        get_debugData(&gMetrologyworkingData);
#else
        get_adsData(&gMetrologyworkingData, &gADSHandle);
#endif
        Metrology_perSampleProcessing(&gMetrologyworkingData);
        Metrology_perSampleEnergyPulseProcessing(&gMetrologyworkingData);
        adsDataStatus = ADC_DATA_WAIT;
    }
}

void TIMER_0_INST_IRQHandler(void)
{
    TIDA_displayOnLCD(&gLCDHandle);
}

