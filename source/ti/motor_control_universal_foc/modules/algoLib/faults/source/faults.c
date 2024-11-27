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

#include "appInputCtrlInterface.h"
#include "faults.h"
#include "focHALInterface.h"
#include "application.h"
#include "gateDriver.h"
#include "main.h"
#include "ISR.h"
#include "services.h"

/* Registers used for storing fault configurations */

uint32_t gateDriverRawFaultStatus = 0; /* Raw status register for faults */
uint32_t controllerRawFaultStatus = 0;
uint32_t controllerFaultActionLatched = 0;

/* Report all controller faults */
uint32_t controllernFaultReport = CONTROLLER_FAULT_REPORT_DEFAULT;

/* No action on :
 * 1. OTW_MCE
 * 2. Bus current limit
 * 3. Speed loop saturation
 * 4. Current loop saturation */
uint32_t controllerFaultAction = CONTROLLER_FAULT_ACTION_DEFAULT;

void faultServiceInit(FAULT_SERVICE_T *pFault)
{
    pFault->count = 0;
    pFault->flags.b.retryStatus = FALSE;
    pFault->flags.b.clearFault = FALSE;
    pFault->flags.b.actionTaken = FALSE;
}

void faultServiceRun(void *g_pMC_App)
{
    UNIVERSAL_FOC_APPLICATION_T *pMC_App = (UNIVERSAL_FOC_APPLICATION_T*) g_pMC_App;
    FAULT_SERVICE_T *pFault = &pMC_App->faultService;
    if(!pFault->flags.b.actionTaken)
    {
        HAL_DisablePWM();
        pFault->pLoad->state = LOAD_STOP;
        update_FOC_faultStatus(pMC_App);
    }

    pFault->flags.b.actionTaken = TRUE;
}

void faultServiceLowPriorityRun(void *g_pMC_App)
{
    UNIVERSAL_FOC_APPLICATION_T *pMC_App = (UNIVERSAL_FOC_APPLICATION_T*) g_pMC_App;
    FAULT_SERVICE_T *pFault = &pMC_App->faultService;

    pFault->flags.b.retryEnable =
                ((pMC_App->sourceLimits.overCurrent.flags.b.status &&
                        (pFault->flags.b.hardwareOverCurrentRetryEnable)) ||
                 ((pMC_App->foc.stallDetect.noMotorStall.flags.b.status) &&
                         (pFault->flags.b.noMotorRetryEnable)) ||
                 ((pMC_App->foc.stallDetect.abnormalSpeedStall.flags.b.status) &&
                         (pFault->flags.b.abnormalSpeedRetryEnable)) ||
                 ((pMC_App->foc.stallDetect.lowBemfStall.flags.b.status) &&
                         (pFault->flags.b.lowBemfRetryEnable)));

    if(pFault->flags.b.retryEnable)
    {
        if(pFault->count < pFault->countMax)
        {
            pFault->count++;
            pFault->flags.b.retryStatus = FALSE;
        }
        else
        {
            pFault->flags.b.retryStatus = TRUE;
        }
    }
}

/* derive the fault status form gate drive fault status and controller fault
 * status and construct 32 bit Raw faults status register
 * */
void update_FOC_faultStatus(void *g_pMC_App)
{
    UNIVERSAL_FOC_APPLICATION_T *pMC_App = (UNIVERSAL_FOC_APPLICATION_T*) g_pMC_App;
    FAULT_SERVICE_T *pFault = &pMC_App->faultService;
    USER_STATUS_INTERFACE_T *pUserOutputs = (pMC_App->pAppInterface->pUserOutputs);

    /* Update Controller Faults Raw Fault Status */
    controllerRawFaultStatus = 0;

    /* Hardware Over current */
    if(pMC_App->sourceLimits.overCurrent.flags.b.status)
    {
        controllerRawFaultStatus |= HW_LOCK_ILIMIT_FAULT_INDEX;
    }

    /* NO_MOTOR fault */
    if(pMC_App->foc.stallDetect.noMotorStall.flags.b.status)
    {
        controllerRawFaultStatus |= NO_MOTOR_FAULT_INDEX;
        controllerRawFaultStatus |= MTR_LOCK_FAULT_STATUS;
    }

   /*case ABNORMAL_SPEED_STALL*/
    if(pMC_App->foc.stallDetect.abnormalSpeedStall.flags.b.status)
    {
        controllerRawFaultStatus |= ABN_SPEED_FAULT_INDEX;
        controllerRawFaultStatus |= MTR_LOCK_FAULT_STATUS;
    }

    /* Abnormal_BEMF fault */
    if(pMC_App->foc.stallDetect.lowBemfStall.flags.b.status)
    {
        controllerRawFaultStatus |= ABN_BEMF_FAULT_INDEX;
        controllerRawFaultStatus |= MTR_LOCK_FAULT_STATUS;
    }

    /* Load Stalls */
    if(pFault->pLoad->ipd.faultStatus != IPD_NO_FAULT)
    {
        if(pFault->pLoad->ipd.faultStatus == IPD_FAULT_CLOCK_TIMEOUT)
        {
            controllerRawFaultStatus |= IPD_CLOCK_TIMEOUT_FAULT_INDEX;
        }
        else
        {
            controllerRawFaultStatus |= IPD_DECAY_TIME_FAULT_INDEX;
        }
    }

    /* Source Current Limit */

    if(((getCurrentLimitTransition(&pMC_App->sourceLimits.sourceCurrentLimit)
            == SOURCE_CURRENT_TRANSITION_TO_CURRENT_LIMIT) ||
     getCurrentLimitState(&pMC_App->sourceLimits.sourceCurrentLimit) ==
             SOURCE_CURRENT_ABOVE_LIMIT) &&
            pMC_App->sourceLimits.sourceCurrentLimit.flags.b.enable)
    {
        controllerRawFaultStatus |= BUS_CURRENT_LIMIT_INDEX;
    }
    else
    {
        pUserOutputs->controllerFaultStatus  &= ~BUS_CURRENT_LIMIT_INDEX;
    }

    /* Voltage out of bounds */

    if(pMC_App->sourceLimits.sourceVoltageLimit.flags.b.underVoltageFaultStatus)
    {
        controllerRawFaultStatus |= UNDER_VOLTAGE_FAULT_INDEX;
    }

    if(pMC_App->sourceLimits.sourceVoltageLimit.flags.b.overVoltageFaultStatus)
    {
        controllerRawFaultStatus |= OVER_VOLTAGE_FAULT_INDEX;
    }
    

    if((pUserOutputs->controllerFaultStatus & (0x7FFFFFFF)) == 0)
    {
        pUserOutputs->controllerFaultStatus = 0;
    }
}

/* Report the fault status based on fault configuration for different faults with
 * respect to report OR no report. Latched faults need to be reported always
 *
 * */
void FOC_faultReport(UNIVERSAL_FOC_APPLICATION_T *pMC_App, FAULT_SERVICE_T *pFault)
{
    USER_STATUS_INTERFACE_T *pUserOutputs = (pMC_App->pAppInterface->pUserOutputs);
    /* update fault status register based on latest raw status and
      user configuration for reporting corresponding faults */
    if(((gateDriverRawFaultStatus & gateDrivernFaultReport) != 0) ||
            ((controllerRawFaultStatus & controllernFaultReport) != 0))
    {

        if((gateDriverRawFaultStatus & gateDrivernFaultReport) != 0)
        {
            pUserOutputs->gateDriverFaultStatus |=
                   (gateDriverRawFaultStatus & gateDrivernFaultReport);

            pUserOutputs->gateDriverFaultStatus |= 0x80000000;   /* Controller fault status bit */
        }
        if((controllerRawFaultStatus & controllernFaultReport) != 0)
        {
            pUserOutputs->controllerFaultStatus  |=
                    (controllerRawFaultStatus & controllernFaultReport);

            pUserOutputs->controllerFaultStatus  |= 0x80000000;   /* Controller fault status bit */
        }

        if( ((gateDriverRawFaultStatus & gateDriverFaultAction) != 0) ||
            ((controllerRawFaultStatus & controllerFaultAction) != 0) )
        {
            /* Move to fault state if any faults configured for action have been detected */
            pMC_App->state = APP_FAULT; /* Move FOC algo to fault state */
            HAL_ClearNFault();    /* Set nFault if cleared as it is to be cleared for action only */
        }
    }
    else
    {
        HAL_SetNFault();      /* do not indicate nFault anymore if there are no active faults */
    }
 }

/* fault recovery, retry mode and clearing latched faults */
void FOC_fault_Recovery(UNIVERSAL_FOC_APPLICATION_T *pMC_App, FAULT_SERVICE_T *pFault)
{
    _Bool actionToNoAction;

   /* Move to fault state if any active faults configured for No action have been detected */

    actionToNoAction = ((gateDriverRawFaultStatus & gateDriverFaultAction) == 0) &&
            ((controllerRawFaultStatus & controllerFaultAction) == 0);

    if(((gateDriverRawFaultStatus & gateDriverFaultAction) != 0) &&
            ((HAL_getRawFaultStatus()) == 0))
    {
        /* clear sticky status on DRV if DRV recovered from fault state */
        gateDriverClearFault();
        /* Need to trigger clearFault here to avoid additional 200ms delay with DRV retry */
        if((controllerRawFaultStatus & controllerFaultAction) == 0)
        {
            actionToNoAction =  TRUE;
        }
    }

    if((actionToNoAction && pMC_App->state == APP_FAULT))
    {
        pFault->flags.b.clearFault = TRUE;
    }

    if(pUserCtrlRegs->algoDebugCtrl1.b.clearFlt)
    {
       faultReportClear(pFault);
    }
}

void faultReportClear(FAULT_SERVICE_T *pFault)
{

    if(!pFault->flags.b.retryStatus)
    {
        gateDriverClearFault();

        HAL_clearFaultStatus();

	    gateDriverRawFaultStatus = 0;
	    g_pAppInterface->pUserOutputs->gateDriverFaultStatus = 0;
    }
    pFault->flags.b.clearFault = TRUE;

    HAL_SetNFault();     /* do not pull down nFault anymore */

    pFault->count = 0;

     /* Clear fault status and fault flags if any */

    controllerRawFaultStatus = 0;
    g_pAppInterface->pUserOutputs->controllerFaultStatus = 0;

    pUserCtrlRegs->algoDebugCtrl1.b.clearFlt = FALSE;
}
