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
 *  @file       drv8323_spi_hal.h
 *  @brief      DRV8323 SPI HAL Module
 *
 * 
 *  @anchor drv8323_spi_hal_h
 *  # Overview
 *
 *  defines SPI HAL APIs for drv8323
 *
 *  <hr>
 ******************************************************************************/

#ifndef DRV8323_SPI_HAL_H
#define DRV8323_SPI_HAL_H

#include "stdint.h"
#include "stdbool.h"
#include "appDefs.h"

#include "ti_msp_dl_config.h"
#include <ti/driverlib/driverlib.h>

/*! @brief DRV8323 SPI instance */
#define DRV8323_SPI_INST    SPI_0_INST

/**
 * @brief     DRV8323 register Read
 * @param[in]  prevReadVal Previous read value
 * @param[in]  regAddr Register address to read
 * @return    Returns the register value
 */
uint16_t drv8323SPIRead(volatile uint16_t prevReadVal, volatile int16_t regAddr);

/**
 * @brief     DRV8323 Register write
 * @param[in]  regAddr Register address to write
 * @param[in]  data data to be written to the register
 */
void drv8323SPIWrite(int16_t regAddr, uint16_t data);

/**
 * @brief Write data to spi resister
 * @param[in] data  Data to write
 */
void HAL_writeSPI16(uint16_t data);

/**
 * @brief Read data in spi resister
 * @return  Data received
 */
uint16_t HAL_readSPI16(void);

#endif /* DRV8323_SPI_HAL_H */
