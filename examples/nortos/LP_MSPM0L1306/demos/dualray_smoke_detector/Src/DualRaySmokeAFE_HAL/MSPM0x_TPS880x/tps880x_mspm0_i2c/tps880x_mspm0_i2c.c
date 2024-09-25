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

#include <Src/include/DualRaySmokeAFE_Config_Common.h>

#define TPS88xx_VALIDATEADDR  // Enable to validate register address \
                              // Disable to save code space

#define TPS88xx_I2C_ADDRESS (0x3F)    // Slave address of TPS88xx
#define TPS88xx_I2C_TIMEOUT (400000)  // SW timeout for I2C commands
#define TPS88xx_REGISTER_NUM (19)     // Total number of TPS88x registers

static uint8_t I2C_TXBuff;  // I2C TX buffer (only 1 byte used)
static uint8_t I2C_RXBuff;  // I2C RX buffer (only 1 byte used)

static uint8_t TPS880x_RegShadow[TPS88xx_REGISTER_NUM];

int8_t tps880x_msp430_i2c_Init(void)
{
    uint8_t i;

    // Initialize shadow registers
    for (i = 0; i < TPS88xx_REGISTER_NUM; i++) {
        TPS880x_RegShadow[i] = 0;
    }

    return 0;  // no error
}

int8_t tps880x_msp430_i2c_Reset(void)
{
    //Reset the I2C Module
    DL_I2C_resetControllerTransfer(I2C_INST);
    //Return to I2C Mode
    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_IOMUX_SDA,
        GPIO_I2C_IOMUX_SDA_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_PULL_UP, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initPeripheralInputFunctionFeatures(GPIO_I2C_IOMUX_SCL,
        GPIO_I2C_IOMUX_SCL_FUNC, DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_PULL_UP, DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_enableHiZ(GPIO_I2C_IOMUX_SCL);
    DL_GPIO_enableHiZ(GPIO_I2C_IOMUX_SDA);

    SYSCFG_DL_I2C_init();

    return 0;  // no error
}

int8_t tps880x_msp430_i2c_WriteReg(uint8_t reg, uint8_t value)
{
#ifdef TPS88xx_VALIDATEADDR
    if (reg >= TPS88xx_REGISTER_NUM) {
        return -1;  // Return error
    }
#endif

    /*
     * Fill FIFO with data. This example will send a MAX of 8 bytes since it
     * doesn't handle the case where FIFO is full
     */
    DL_I2C_fillControllerTXFIFO(I2C_INST, &reg, 1);
    DL_I2C_fillControllerTXFIFO(I2C_INST, &value, 1);

    /* Wait for I2C to be Idle */
    while (!(
        DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    /* Send the packet to the controller.
     * This function will send Start + Stop automatically.
     */
    DL_I2C_startControllerTransfer(
        I2C_INST, TPS88xx_I2C_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_TX, 2);

    // save value in shadow register
    TPS880x_RegShadow[reg] = value;

    /* Poll until the Controller writes all bytes */
    while (
        DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_BUSY)

        /* Trap if there was an error */
        if (DL_I2C_getControllerStatus(I2C_INST) &
            DL_I2C_CONTROLLER_STATUS_ERROR) {
            return -1;  // Err
        }
    return 0;  // no err
}

int8_t tps880x_msp430_i2c_ReadReg(uint8_t reg, uint8_t *value)
{
    int8_t ret;
#ifdef TPS88xx_VALIDATEADDR
    if (reg >= TPS88xx_REGISTER_NUM) {
        return -1;  // Error
    }
#endif

    //Add register to I2C TX FIFO
    DL_I2C_fillControllerTXFIFO(I2C_INST, &reg, 1);

    /* Wait for I2C to be Idle */
    while (!(
        DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    DL_I2C_clearInterruptStatus(I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE);

    // ********Write Reg ***********************************************************
    // Specify target address and read/write mode
    I2C_INST->MASTER.MSA = ((TPS88xx_I2C_ADDRESS << I2C_MSA_SADDR_OFS) |
                            (uint32_t) DL_I2C_CONTROLLER_DIRECTION_TX);

    // STOP bit is generated after burst length number of bytes transferred
    I2C_INST->MASTER.MCTR = (((uint32_t) 1 << I2C_MCTR_MBLEN_OFS) |
                             I2C_MCTR_BURSTRUN_ENABLE | I2C_MCTR_START_ENABLE);
    /* Poll until the Controller writes all bytes */
    while (DL_I2C_getControllerStatus(I2C_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;

    //Wait for Register value to TX
    while (!DL_I2C_getRawInterruptStatus(
        I2C_INST, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE))
        ;
    //*********************************************************************************

    //****Restart and read register value *********************************************
    DL_I2C_startControllerTransfer(
        I2C_INST, TPS88xx_I2C_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_RX, 1);

    //Wait to recieve Data
    while (DL_I2C_isControllerRXFIFOEmpty(I2C_INST))
        ;

    //Read data
    I2C_RXBuff = DL_I2C_receiveControllerData(I2C_INST);

    // Return the value
    *value = I2C_RXBuff;
    // Update value in shadow register
    TPS880x_RegShadow[reg] = *value;

    return 0;  // No error
}

int8_t tps880x_msp430_i2c_SetBitsReg(uint8_t reg, uint8_t mask)
{
#ifdef TPS88xx_VALIDATEADDR
    if (reg >= TPS88xx_REGISTER_NUM) {
        return -1;  // Error
    }
#endif

    // Update shadow register
    TPS880x_RegShadow[reg] |= mask;
    // Write updated value
    return (tps880x_msp430_i2c_WriteReg(reg, TPS880x_RegShadow[reg]));
}

int8_t tps880x_msp430_i2c_ClearBitsReg(uint8_t reg, uint8_t mask)
{
#ifdef TPS88xx_VALIDATEADDR
    if (reg >= TPS88xx_REGISTER_NUM) {
        return -1;  // Error
    }
#endif
    // Update shadow register
    TPS880x_RegShadow[reg] &= ~mask;
    // Write updated value
    return (tps880x_msp430_i2c_WriteReg(reg, TPS880x_RegShadow[reg]));
}

int8_t tps880x_msp430_i2c_UpdateBitsReg(
    uint8_t reg, uint8_t mask, uint8_t value)
{
#ifdef TPS88xx_VALIDATEADDR
    if (reg >= TPS88xx_REGISTER_NUM) {
        return -1;  // Error
    }
#endif
    // Update shadow register
    TPS880x_RegShadow[reg] &= ~mask;  // Clear all bits being modified
    TPS880x_RegShadow[reg] |= value;  // Set the value
    // Write updated value
    return (tps880x_msp430_i2c_WriteReg(reg, TPS880x_RegShadow[reg]));
}
