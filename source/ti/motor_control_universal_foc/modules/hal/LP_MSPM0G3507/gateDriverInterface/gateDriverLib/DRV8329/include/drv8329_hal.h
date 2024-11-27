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
/*!****************************************************************************
 *  @file       drv8329_hal.h
 *  @brief      DRV8329 HAL Module
 *
 * 
 *  @anchor drv8329_hal_h
 *  # Overview
 *
 *  defines HAL APIs for drv8329
 *
 *  <hr>
 ******************************************************************************/

#ifndef DRV8329_HAL_H
#define DRV8329_HAL_H

#include "stdint.h"
#include "stdbool.h"
#include "appDefs.h"
#include "gateDriver.h"

#include "ti_msp_dl_config.h"
#include <ti/driverlib/driverlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief     Set NSleep pin of DRV8329
 */
void DRV8329_setNSleep(void);

/**
 * @brief     Clear NSleep pin of DRV8329
 */
void DRV8329_clearNSleep(void);

#ifdef __cplusplus
}
#endif
#endif /* DRV8329_SPI_HAL_H */
