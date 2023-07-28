/* --COPYRIGHT--,BSD
 * Copyright (c) 2019, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
//*****************************************************************************
//         Driver for TPS880x using MSP430 I2C
// *****************************************************************************
#ifndef _TPS880X_MSP430_I2C_H__
#define _TPS880X_MSP430_I2C_H__

#include <stdbool.h>
#include <stdint.h>
#include "tps880x_regs.h"

//! \brief Initializes the I2C for communication with TPS880x.
//!
//! \return 0 if OK, -1 if error
extern int8_t tps880x_msp430_i2c_Init(void);

//! \brief Reset the TPS880x I2C interface.
//!
//! \return 0 if OK, -1 if error
extern int8_t tps880x_msp430_i2c_Reset();

//! \brief Write a TPS880x Register.
//!
//! \param[in] reg is the address of the register being written.
//! \param[in] value is the value being written to the register.
//!
//! \return 0 if OK, -1 if error
extern int8_t tps880x_msp430_i2c_WriteReg(uint8_t reg, uint8_t value);

//! \brief Read a TPS880x Register.
//!
//! \param[in] reg is the address of the register being read.
//! \param[out] value is the value read from the register.
//!
//! \return 0 if OK, -1 if error
extern int8_t tps880x_msp430_i2c_ReadReg(uint8_t reg, uint8_t *value);

//! \brief Set bits of a TPS880x Register.
//!         This function uses "shadow" registers to reduce overhead.
//!         These registers are maintained by MCU and don't necessarily
//!         reflect the current status of the register.
//!
//! \param[in] reg is the address of the register being updated.
//! \param[in] mask contains the bits being set (1= set, 0= ignored).
//!
//! \return 0 if OK, -1 if error
extern int8_t tps880x_msp430_i2c_SetBitsReg(uint8_t reg, uint8_t mask);

//! \brief Clear bits of a TPS880x Register.
//!         This function uses "shadow" registers to reduce overhead.
//!         These registers are maintained by MCU and don't necessarily
//!         reflect the current status of the register.
//!
//! \param[in] reg is the address of the register being updated.
//! \param[in] mask contains the bits being cleared (1= clear, 0= ignored).
//!
//! \return 0 if OK, -1 if error
extern int8_t tps880x_msp430_i2c_ClearBitsReg(uint8_t reg, uint8_t mask);

//! \brief Updates bits of a TPS880x Register.
//!         This function uses "shadow" registers to reduce overhead.
//!         These registers are maintained by MCU and don't necessarily
//!         reflect the current status of the register.
//!
//! \param[in] reg is the address of the register being updated.
//! \param[in] mask contains the bits being updated (1= update, 0= ignored).
//! \param[in] value contains the value of the bits being updated
//!
//! \return 0 if OK, -1 if error
extern int8_t tps880x_msp430_i2c_UpdateBitsReg(
    uint8_t reg, uint8_t mask, uint8_t value);

#endif  //_TPS880X_MSP430_I2C_H__
