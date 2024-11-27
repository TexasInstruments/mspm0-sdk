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
#include <math.h>

#include "drv8329.h"
#include "drv8329_hal.h"
#include "main.h"
#include "ISR.h"
#include "focHALInterface.h"

#ifdef DRV8329

/*! @brief Gate driver fault report register, faults with 1 are set to report */
uint32_t gateDrivernFaultReport = 0x18000000;
/*! @brief Gate driver fault action register, faults with 1 are set to clear if reported */
uint32_t gateDriverFaultAction = 0x18000000;

/*! @brief Reset pulse duration */
#define DRV8329_RESET_PULSE_DURATION_US (2)

/**************************************************************************/

void gateDriverConfig(void)
{


}


void gateDriverClearFault(void)
{
    /* nSleep pin is set low for time greater than Max Reset Pulse Time */
    DRV8329_setNSleep();

    HAL_delayMicroSeconds(DRV8329_RESET_PULSE_DURATION_US);

    DRV8329_clearNSleep();

}

/* Get Fault Status */
uint32_t gateDriverGetFaultStatus(void)
{
    return  (0);
}

/*Update the Gate Driver Parameters when the Config Enable Flg is Set */
void gateDriverParamsUpdate(HV_DIE_EEPROM_INTERFACE_T *pGateDriverConfig)
{

}


void gateDriverFaultResponse(HV_DIE_EEPROM_INTERFACE_T *pGateDriverConfig)
{

}

void gateDriverCSACalib(void)
{

}

void gateDriverOffsetCalibSet(void)
{

}

void gateDriverOffsetCalibReset(void)
{

}
void gateDriverInit(void)
{
    gateDrivernFaultReport = 0;
    gateDriverFaultAction  = 0;

}

uint16_t gateDriverRegRead(int16_t regAddr)
{
    return 0;
}

#endif
