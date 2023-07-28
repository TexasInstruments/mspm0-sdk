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
//      HAL functions to initialize the system for Dual Ray Smoke AFE demo
//
// Texas Instruments, Inc.
// ******************************************************************************

#include <Src/DualRaySmokeAFE_HAL/MSPM0x_TPS880x/tps880x_mspm0_i2c/tps880x_mspm0_i2c.h>
#include <Src/include/DualRaySmokeAFE_Config_Common.h>
#include <Src/include/DualRaySmokeAFE_HAL.h>

/**** Local Function Prototypes ***********************************************/
static void DualRaySmokeAFE_HAL_System_Clock_Init(void);
static void Clock_Software_Trim(void);

/**** Functions ***************************************************************/
int8_t DualRaySmokeAFE_HAL_System_Init(void)
{
    int8_t ret;
    uint8_t reg_val;

    DualRaySmokeAFE_HAL_IO_Init();
    DualRaySmokeAFE_HAL_System_Clock_Init();

    // Default configuration of TPS880x
    // Initialize TPS880x
    tps880x_msp430_i2c_Init();  // Initialize I2C for communication with TPS880x

    // Basic test to make sure TPS880x communication is working OK
    //  Write REG_MASK with default value and verify contents
    // mask all interrupts
    ret = tps880x_msp430_i2c_WriteReg(TPS880X_REG_MASK, 0xFF - STATUS_INT);

    //    if (ret < 0)
    //    {
    // try resetting the interface
    tps880x_msp430_i2c_Reset();
    ret = tps880x_msp430_i2c_WriteReg(TPS880X_REG_MASK, 0xFF - STATUS_INT);
    if (ret < 0) {
        // error writing data
        return -1;
    }
    //    }

    // Check response
    ret = tps880x_msp430_i2c_ReadReg(TPS880X_REG_MASK, (uint8_t *) &reg_val);
    if (ret < 0) {
        // error reading data
        return -1;
    }

    if (reg_val != (0xFF - STATUS_INT)) {
        // error while reading data from
        return -1;
    }

    // Clear flags
    tps880x_msp430_i2c_ReadReg(TPS880X_REG_STATUS1, &reg_val);
    tps880x_msp430_i2c_ReadReg(TPS880X_REG_STATUS2, &reg_val);

    // Mask all interrupts, enable INT_MCU
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_MASK, 0xFF);

    // After basic test, Set default configuration of TPS880x
    //0x04=0xB6 (interconnect deglitch=100ms, set VMCU=3.3V, SLP_BST=SLP_ANALOG=1)
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_CONFIG1,
        INT_DEG__125US | INT_PD | VMCUSET__3_3V | SLP_BST | SLP_ANALOG);
    //0x05=0x1A (3-terminal horn, 50% duty cycle)
    tps880x_msp430_i2c_WriteReg(
        TPS880X_REG_CONFIG2, HORN_SEL | HORN_THR__NOM | T_BSTACT__10MS);
    //0x06=0x10 (enable amplifiers when needed)
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_ENABLE1, INT_EN);
    //0x07=0x04 (set LEDPIN_EN=1)
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_ENABLE2, LEDPIN_EN);
    //0x08=0x01 (VCCLOW_BST=1 to prevent brownouts)
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_CONTROL, VCCLOW_BST);
    //0x09=0x00 (Disabled by default)
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_SLPTMR1, 0x00);
    //0x0A=0xD0 (Disabled by default)
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_SLPTMR2, 0x00);
    //0x0B=0x02 (set up GPIO as Status Interrupt)
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_GPIO_AMUX, GPIO__STATUS);
    //0x0C=0xF0 (set up CO amp for internal resistors/reference)
    tps880x_msp430_i2c_WriteReg(
        TPS880X_REG_CO_BATTEST, COSWRO | COSWRG | COSWRI | COSWREF);
    //0x0D=0x01 (Disable CO amp by default)
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_CO, 0x00);
    //0x0E=0xF0 (max boost current, set VBST=2.7V)
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_VBOOST, BST_CLIM__500MA);
    //0x0F=0x00 (LEDLDO set to 7.5V)
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_LEDLDO, 0);
    //0x10=0x07 (PREF_SEL=1, photo gain=35x. Need to modify based on LED config)
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_PH_CTRL, PREF_SEL | PGAIN__11);

    //0x11=0x00 (depends on LED current)
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_LED_DAC_A, 0);

    //0x12=0x00 (depends on LED current)
    tps880x_msp430_i2c_WriteReg(TPS880X_REG_LED_DAC_B, 0);

    return 0;  // OK
}

/**** Local Functions *********************************************************/
//! \brief Initializes the system clock
//!
//! \return none
static void DualRaySmokeAFE_HAL_System_Clock_Init(void)
{
    //Removed for MSPM0
    //Not Needed.  Clock is configured in MSP_dl_config.c
    //Default MCLK speed is 32MHz
}

//! \brief Software trim for DCO+FLL to lock target frequency
//!
//! \return none
static void Clock_Software_Trim(void)
{
    //Removed for MSPM0
}
