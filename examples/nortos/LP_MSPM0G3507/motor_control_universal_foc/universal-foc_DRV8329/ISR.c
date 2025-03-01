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

 #include "iqTrig.h"
/* Include the IQmath Library */
#include <ti/iqmath/include/IQmathLib.h>
#include <string.h>
#include "focHALInterface.h"
#include "focPeriphInit.h"
#include "main.h"
#include "mcLib.h"
#include "ISR.h"
#include "appDefs.h"
/*Communication Interface for the application*/
#include "uart_comm.h"
#include "gateDriver.h"
#include "appUserInputsConfig.h"

#define EBPAST_MOTOR
/*! @brief FW Version register for reference and code maintenance */
uint32_t *pAppFWVersion = (uint32_t*)APP_FW_VER_BASE;

/*! @brief User eeprom input pointer */
USER_INPUT_INTERFACE_T *pUserInputRegs = (USER_INPUT_INTERFACE_T*)APP_USER_INP_BASE;

/*! @brief User ram input variables pointer */
USER_CTRL_INTERFACE_T *pUserCtrlRegs = (USER_CTRL_INTERFACE_T*)(APP_USER_CTRL_BASE);

/*! @brief User eeprom input pointer */
USER_STATUS_INTERFACE_T *pUserStatusRegs = (USER_STATUS_INTERFACE_T*)APP_USER_STATUS_BASE;

/*! @brief UART Instance pointer */
UART_Instance *pUART = (UART_Instance*)(APP_USER_STATUS_BASE + sizeof(USER_STATUS_INTERFACE_T));

/*! @brief Gate driver config pointer */
HV_DIE_EEPROM_INTERFACE_T *pGateDriverConfig = (HV_DIE_EEPROM_INTERFACE_T*)(APP_USER_STATUS_BASE + sizeof(USER_STATUS_INTERFACE_T)+ sizeof(UART_Instance));

/*! @brief Application interface pointer */
APP_INTERFACE_T *g_pAppInterface = (APP_INTERFACE_T *)(APP_USER_STATUS_BASE + sizeof(USER_STATUS_INTERFACE_T)+ sizeof(UART_Instance)+ sizeof(HV_DIE_EEPROM_INTERFACE_T));

/*! @brief Motor control application pointer */
UNIVERSAL_FOC_APPLICATION_T *g_pMC_App  = (void*)(APP_USER_STATUS_BASE + sizeof(USER_STATUS_INTERFACE_T)+ sizeof(UART_Instance)+ sizeof(HV_DIE_EEPROM_INTERFACE_T)+ sizeof(APP_INTERFACE_T));

/*! @brief Motor inputs pointer */
HAL_MEASURE_MOTOR_INPUTS_T *g_pMotorInputs;

/*! @brief Data written to DAC */
int32_t dacWriteData = 0;

/**
 * @brief Set default motor parameters
 */
static void setUserDefaultMotorParameters(void);

#ifndef FOC_ISR_ADC1
/* ADC0 ISR, used to run the FOC algorithm */
void ADC0_INST_IRQHandler(void)
{
    DL_ADC12_IIDX pendIrq = DL_ADC12_getPendingInterrupt(ADC0_INST);
    switch(pendIrq)
    {
        case FOC_ADC_MEM_RES_LOAD:
            FOC_ADC_ISR();
            break;
        default:
            break;
    }
}
#endif

#ifdef FOC_ISR_ADC1
/* ADC1 ISR, used to run the FOC algorithm */
void ADC1_INST_IRQHandler(void)
{
    DL_ADC12_IIDX pendIrq = DL_ADC12_getPendingInterrupt(ADC1_INST);
    switch(pendIrq)
    {
        case FOC_ADC_MEM_RES_LOAD:
            FOC_ADC_ISR();
            break;
        default:
            break;
    }
}
#endif

/* DMA ISR, used for UART communication */
void DMA_IRQHandler(void)
{
    switch (DL_DMA_getPendingInterrupt(DMA))
    {
        case DL_DMA_FULL_CH_EVENT_IIDX_EARLY_IRQ_DMACH0:
            UART_getFrameLength(pUART);
            pUART->status = UART_STATUS_RX_BUFFERING;
            break;

        case DL_DMA_EVENT_IIDX_DMACH1:
            DMA_RX_init(pUART);
            break;
        default:
            break;
    }
}

/* Handles DAC output, Handles the motor inputs and runs the MC application */
void FOC_ADC_ISR(void)
{

    HAL_ReadMotorInputs(g_pMotorInputs);

    applicationRun(g_pMC_App);

    HAL_SelectShuntMeasure(g_pMotorInputs);

    /* Data Sequencer Conversion has occurred.
     * Current information is available
     * Execute FOC and any other test algorithms */

    if(pUserCtrlRegs->dacCtrl.dacEn != 0)
    {
        if(pUserCtrlRegs->dacCtrl.dacScalingFactor != 0)
        {
            dacWriteData = _IQmpy_mathacl(
                        *((uint32_t *)pUserCtrlRegs->dacCtrl.dacOutAddr),
                         pUserCtrlRegs->dacCtrl.dacScalingFactor);
            /* Adding offset value to half of reference voltage */
            dacWriteData = _IQtoIQ12((dacWriteData >> 1) + _IQ(0.5));
        }
        else
        {
            dacWriteData = *((uint32_t *)pUserCtrlRegs->dacCtrl.dacOutAddr);
            if((pUserCtrlRegs->dacCtrl.dacShift)>=0)
            {
                dacWriteData <<= pUserCtrlRegs->dacCtrl.dacShift;
            }
            else
            {
                dacWriteData >>= (-1*pUserCtrlRegs->dacCtrl.dacShift);
            }
        }
        DL_DAC12_output12(DAC0, dacWriteData);
    }
}

/* Timer ISR, used for running the low priority tasks */
void FOC_TIMG0_INST_IRQHandler()
{
    switch (DL_TimerG_getPendingInterrupt(FOC_TIMG0_INST))
    {
        case DL_TIMER_IIDX_LOAD:
            /* Set the speed reference */
            g_pAppInterface->userInputs.speedInput = _IQ15toIQ(pUserCtrlRegs->
                        speedCtrl.b.speedInput);

            /* Execute the low priority timed house-keeping tasks */
            applicationLowPriorityRun(g_pMC_App);
            break;
        default:
            break;
    }
}

#ifdef __IPD_ENABLE
/* Capture ISR, used for capture IPD time */
void CAPTURE_INST_IRQHandler()
{
    switch (DL_TimerA_getPendingInterrupt(CAPTURE_INST))
    {
        case DL_TIMER_IIDX_CC0_DN:
            applicationIPDCurrThresh(g_pMC_App);
            break;
        case DL_TIMER_IIDX_ZERO:
            applicationIPDOverFlow(g_pMC_App);
            break;
        default:
            break;
    }
}
#endif

void appConfig(void)
{
    gateDriverInit();
    gateDriverConfig();
    updateConfigsInit();
    applicationConfig(g_pMC_App);
    UART_init(pUART);
}

void userInputsInit(void)
{
    setUserDefaultMotorParameters();
    appUserInputsInit(&(g_pAppInterface->userInputs));

    pUserCtrlRegs->algoDebugCtrl1.b.clearFlt = TRUE;
}
void appInputReset(void)
{
    *pAppFWVersion = APP_FW_VERSION;

    if((APP_USER_INP_BASE + sizeof(USER_INPUT_INTERFACE_T)) > APP_USER_CTRL_BASE)
    {
        /* User input Register limit exceeded */
        while(1); /* Stop in Infinite loop */
    }

    if((APP_USER_CTRL_BASE + sizeof(USER_CTRL_INTERFACE_T)) > APP_USER_STATUS_BASE)
    {
        /* User status Register limit exceeded */
        while(1); /* Stop in Infinite loop */
    }

    if(((uint32_t)g_pMC_App + getMCAppSize()) > APP_SHADOW_END)
    {
        /* MC App size limit exceeded */
        while(1); /* Stop in Infinite loop */
    }

    memset(g_pAppInterface, 0, sizeof(APP_INTERFACE_T));

    memset (pUserCtrlRegs, 0, sizeof(USER_CTRL_INTERFACE_T));

    g_pAppInterface->userInputs.debugFlags.b.updatedData = FALSE;

    g_pAppInterface->flags.b.shadowToAlgorithmSet = TRUE;
    g_pAppInterface->flags.b.hvDieConfigSet = TRUE;
    g_pAppInterface->flags.b.faultsResponseSet = TRUE;
}

static void setUserDefaultMotorParameters(void)
{
    pUserInputRegs->isdCfg.w         = 0x0C440018;
    pUserInputRegs->mtrStartUp1.w    = 0x09949400;
    pUserInputRegs->mtrStartUp2.w    = 0x53026006;

    pUserInputRegs->closeLoop1.w     = 0x03C26F24;
    pUserInputRegs->fieldCtrl.w      = 0x00;


    pUserInputRegs->faultCfg1.w      = 0x00007A8C;
    pUserInputRegs->faultCfg2.w      = 0x024C0000;

    pUserInputRegs->miscAlgo.w       = 0x0000F3C5;

    pUserInputRegs->pinCfg.w         = 0x000E50E2;
    pUserInputRegs->periphCfg1.w     = 0x06003742;

    pUserInputRegs->systemParams.kSlide = 0.1;

#ifdef DRV8323
    pGateDriverConfig->gateDrvCfg1.w  = 0x0141919F;
    pGateDriverConfig->gateDrvCfg2.w  = 0x0FDFF800;
#endif

#ifdef DRV8316
    pGateDriverConfig->gateDrvCfg1.w = 0x1C400100;
    pGateDriverConfig->gateDrvCfg2.w = 0x00000000;
#endif

#ifdef DRV8329
    pUserInputRegs->periphCfg1.b.mcuDeadTime = 10;
#endif

#ifdef NIDEC_MOTOR
    pUserInputRegs->isdCfg.w         = 0x4C44000C;
    pUserInputRegs->closeLoop2.w     = 0xB9900C90;
    pUserInputRegs->.systemParams.mtrResist        = 415;
    pUserInputRegs->systemParams.mtrInductance    = 345;
    pUserInputRegs->systemParams.mtrBemfConst     = 90;
    pUserInputRegs->systemParams.speedLoopKp      = 0.0539;
    pUserInputRegs->systemParams.speedLoopKi    = 0.036;
    pUserInputRegs->systemParams.currLoopKp     = 3.42;
    pUserInputRegs->systemParams.currLoopKi     = 3678;
    pUserInputRegs->systemParams.maxMotorSpeed = MOTOR_MAX_SPEED;
    pUserInputRegs->systemParams.maxMotorPower     = 25;

#elif defined NIDEC_PUMP_MOTOR
    pUserInputRegs->isdCfg.w         = 0x4644000A;
    pUserInputRegs->closeLoop2.w     = 0xB9900C90;
    pUserInputRegs->systemParams.mtrResist        = 1850;
    pUserInputRegs->systemParams.mtrInductance    = 1650;
    pUserInputRegs->systemParams.mtrBemfConst     = 220;
    pUserInputRegs->systemParams.speedLoopKp      = 0.01;
    pUserInputRegs->systemParams.speedLoopKi    = 0.0005;
    pUserInputRegs->systemParams.currLoopKp     = 0.1;
    pUserInputRegs->systemParams.currLoopKi     = 10;
    pUserInputRegs->systemParams.maxMotorSpeed = 180;
    pUserInputRegs->systemParams.maxMotorPower     = 25;

#elif defined DAIKIN_MOTOR
    pUserInputRegs->isdCfg.w         = 0x4644000A;
    pUserInputRegs->closeLoop2.w     = 0xB9900C90;
    pUserInputRegs->systemParams.mtrResist        = 78000;
    pUserInputRegs->systemParams.mtrInductance    = 197000;
    pUserInputRegs->systemParams.mtrBemfConst     = 12800;
    pUserInputRegs->systemParams.speedLoopKp      = 0.0539;
    pUserInputRegs->systemParams.speedLoopKi    = 0.036;
    pUserInputRegs->systemParams.currLoopKp     = 3.42;
    pUserInputRegs->systemParams.currLoopKi     = 3678;
    pUserInputRegs->systemParams.maxMotorSpeed = 100;
    pUserInputRegs->systemParams.maxMotorPower     = 60;

#elif defined EBPAST_MOTOR
    pUserInputRegs->isdCfg.w         = 0x6644040A;
    pUserInputRegs->closeLoop2.w     = 0xB9900C90;
    pUserInputRegs->systemParams.mtrResist        = 900;
    pUserInputRegs->systemParams.mtrInductance    = 1265;
    pUserInputRegs->systemParams.mtrBemfConst     = 400;
    pUserInputRegs->systemParams.speedLoopKp      = 0.01;
    pUserInputRegs->systemParams.speedLoopKi    = 0.05;
    pUserInputRegs->systemParams.currLoopKp     = 1;
    pUserInputRegs->systemParams.currLoopKi     = 500;
    pUserInputRegs->systemParams.maxMotorSpeed = 200;
    pUserInputRegs->systemParams.fluxWeakeningKp     = 1.0;
    pUserInputRegs->systemParams.fluxWeakeningKi     = 500;
    pUserInputRegs->systemParams.maxMotorPower     = 25;
#else

    pUserInputRegs->isdCfg.w         = 0x4644000A;
    pUserInputRegs->closeLoop2.w     = 0xB9900C90;
    pUserInputRegs->systemParams.mtrResist        = 590;
    pUserInputRegs->systemParams.mtrInductance    = 550;
    pUserInputRegs->systemParams.mtrBemfConst     = 290;
    pUserInputRegs->systemParams.speedLoopKp      = 0.0539;
    pUserInputRegs->systemParams.speedLoopKi    = 0.036;
    pUserInputRegs->systemParams.currLoopKp     = 3.42;
    pUserInputRegs->systemParams.currLoopKi     = 3678;
    pUserInputRegs->systemParams.fluxWeakeningKp     = 1.0;
    pUserInputRegs->systemParams.fluxWeakeningKi     = 500;
    pUserInputRegs->systemParams.maxMotorSpeed = MOTOR_MAX_SPEED;
    pUserInputRegs->systemParams.maxMotorPower     = 15;

#endif

    pUserInputRegs->systemParams.voltageBase = MOTOR_VOLTAGE_BASE;

    pUserInputRegs->systemParams.currentBase = FULL_SCALE_CURRENT_BASE;

    pUserCtrlRegs->speedCtrl.b.speedInput = 0;
    pUserCtrlRegs->algoDebugCtrl1.b.closeLoopDis = 0;
    pUserInputRegs->faultCfg2.b.abnSpdFaultEn = 0;
    pUserInputRegs->faultCfg2.b.lowBemfFaultEn = 0;

    /* DAC output settings */
    pUserCtrlRegs->dacCtrl.dacScalingFactor = _IQ(1.0);

    pUserCtrlRegs->dacCtrl.dacOutAddr = 0x20200CA4;

    pUserCtrlRegs->dacCtrl.dacShift = 0;

    pUserCtrlRegs->dacCtrl.dacEn = 1;

    /* Enable status updates*/
    
    pUserCtrlRegs->algoDebugCtrl2.b.updateSysParams = 1;
    pUserCtrlRegs->algoDebugCtrl2.b.statusUpdateEn = 1;
}
