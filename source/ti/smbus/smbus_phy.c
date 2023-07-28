/* --COPYRIGHT--,BSD
 * Copyright (c) 2023, Texas Instruments Incorporated
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

#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>


#include "smbus.h"
#include "smbus_nwk.h"
#include "smbus_phy.h"

void SMBus_PHY_disable(SMBus *smbus)
{
    DL_I2C_disableController(smbus->phy.SMBus_Phy_i2cBase);
    smbus->ctrl.bits.phyEn = 0;      // Update control flag
}

void SMBus_PHY_enable(SMBus *smbus)
{
    if(smbus->ctrl.bits.controller)
    {
        SMBus_PHY_controllerEnable(smbus);
    }
    else
    {
        SMBus_PHY_targetEnable(smbus);
    }
}


void SMBus_PHY_targetEnable(SMBus *smbus)
{
    // Make sure the PHY is disabled
    SMBus_PHY_disable(smbus);

    // Write the Target Address
    DL_I2C_setTargetOwnAddress(smbus->phy.SMBus_Phy_i2cBase, smbus->ownTargetAddr);

    // Clear all flags
    DL_I2C_clearInterruptStatus(smbus->phy.SMBus_Phy_i2cBase, 0xFFFF);


    if(smbus->ctrl.bits.intEn == 1)
    {
        SMBus_targetEnableInt(smbus);
    }

    // Enable I2C module
    DL_I2C_enableTarget(smbus->phy.SMBus_Phy_i2cBase);
    smbus->ctrl.bits.phyEn = 1;      // Set global flag
}

void SMBus_PHY_targetEnableInt(SMBus *smbus)
{
    DL_I2C_clearInterruptStatus(smbus->phy.SMBus_Phy_i2cBase,
                           DL_I2C_INTERRUPT_TARGET_RXFIFO_TRIGGER |
                           DL_I2C_INTERRUPT_TARGET_START |
                           DL_I2C_INTERRUPT_TARGET_STOP |
                           DL_I2C_INTERRUPT_TARGET_TXFIFO_EMPTY);
    DL_I2C_enableInterrupt(smbus->phy.SMBus_Phy_i2cBase,
                           DL_I2C_INTERRUPT_TARGET_RXFIFO_TRIGGER |
                           DL_I2C_INTERRUPT_TARGET_START |
                           DL_I2C_INTERRUPT_TARGET_STOP |
                           DL_I2C_INTERRUPT_TARGET_TXFIFO_EMPTY);

    // Set global flag
    smbus->ctrl.bits.intEn = 1;
}

void SMBus_PHY_targetInit(SMBus *smbus,
                         I2C_Regs *i2cInst)
{
    SMBus_Phy *SMBusPHY = &smbus->phy;

    // Set the I2C Base Address
    smbus->phy.SMBus_Phy_i2cBase = i2cInst;

    // Enabling the I2C module is handled by SysConfig
}

SMBus_State SMBus_PHY_targetProcessInt(SMBus *smbus)
{
    SMBus_State ret_state = SMBus_State_Target_NTR;
    SMBus_Phy *SMBusPHY = &smbus->phy;
    uint8_t data, addr;

    switch (DL_I2C_getPendingInterrupt(SMBusPHY->SMBus_Phy_i2cBase)) {
    case DL_I2C_IIDX_TARGET_RXFIFO_TRIGGER:
        data = DL_I2C_receiveTargetData(SMBusPHY->SMBus_Phy_i2cBase);
        ret_state = SMBus_NWK_targetProcessRx(smbus, data);
        break;
    case DL_I2C_IIDX_TARGET_STOP:

        ret_state = SMBus_NWK_targetProcessStop(smbus);

        break;
    case DL_I2C_IIDX_TARGET_START:
        data = (uint8_t) DL_I2C_getTargetAddressMatch(SMBusPHY->SMBus_Phy_i2cBase);
        ret_state = SMBus_NWK_targetProcessStart(smbus, data);
        break;
    case DL_I2C_IIDX_TARGET_TXFIFO_EMPTY:
        ret_state = SMBus_NWK_targetProcessTx(smbus, &data);
        DL_I2C_fillTargetTXFIFO(SMBusPHY->SMBus_Phy_i2cBase, &data, 1);
        break;
    case DL_I2C_IIDX_TARGET_PEC_RX_ERROR:
        smbus->status.bits.pecErr = 1;
        ret_state = SMBus_State_PECError;
    default:
        break;
    }
    return(ret_state);
}

void SMBus_PHY_controllerEnable(SMBus *smbus)
{
    // Make sure the PHY is disabled
    SMBus_PHY_disable(smbus);

    DL_I2C_enableController(smbus->phy.SMBus_Phy_i2cBase);

    // Re-enable Interrupts since the Reset will clear them
    if(smbus->ctrl.bits.intEn == 1)
    {
        SMBus_controllerEnableInt(smbus);
    }

    smbus->ctrl.bits.phyEn = 1;      // Set global flag
}

void SMBus_PHY_controllerEnableInt(SMBus *smbus)
{
    DL_I2C_clearInterruptStatus(smbus->phy.SMBus_Phy_i2cBase,
                           DL_I2C_INTERRUPT_CONTROLLER_NACK |
                           DL_I2C_INTERRUPT_CONTROLLER_ARBITRATION_LOST |
                           DL_I2C_INTERRUPT_CONTROLLER_START |
                           DL_I2C_INTERRUPT_CONTROLLER_RXFIFO_TRIGGER |
                           DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER |
                           DL_I2C_INTERRUPT_CONTROLLER_STOP |
                           DL_I2C_INTERRUPT_TIMEOUT_A);
    DL_I2C_enableInterrupt(smbus->phy.SMBus_Phy_i2cBase,
                           DL_I2C_INTERRUPT_CONTROLLER_NACK |
                           DL_I2C_INTERRUPT_CONTROLLER_ARBITRATION_LOST |
                           DL_I2C_INTERRUPT_CONTROLLER_START |
                           DL_I2C_INTERRUPT_CONTROLLER_RXFIFO_TRIGGER |
                           DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER |
                           DL_I2C_INTERRUPT_CONTROLLER_STOP |
                           DL_I2C_INTERRUPT_TIMEOUT_A);

    // Set global flag
    smbus->ctrl.bits.intEn = 1;
}

void SMBus_PHY_controllerInit(SMBus *smbus,
                          I2C_Regs *i2cRegs,
                          uint32_t busClk)
{
    SMBus_Phy *SMBusPHY = &smbus->phy;
    smbus->phy.SMBus_Phy_i2cBase = i2cRegs;

    // Target Address not initialized by default
    smbus->ownTargetAddr = 0;

    // Enabling the I2C module is handled by SysConfig
}

void SMBus_PHY_controllerSendStop(SMBus *smbus)
{
        DL_I2C_startControllerTransferAdvanced(smbus->phy.SMBus_Phy_i2cBase,
                                               0x0, // This is a don't care
                                               DL_I2C_CONTROLLER_DIRECTION_TX,
                                               0,
                                               DL_I2C_CONTROLLER_START_DISABLE,
                                               DL_I2C_CONTROLLER_STOP_ENABLE,
                                               DL_I2C_CONTROLLER_ACK_DISABLE);
}

void SMBus_PHY_controllerStartTx(SMBus *smbus,
                             uint8_t targetaddr,
                             uint16_t length,
                             SMBus_Stop stopFlag,
                             uint8_t setPEC)

{
    uint8_t data;
    DL_I2C_CONTROLLER_STOP phy_stop;

    if (stopFlag == SMBus_Stop_After_Transfer)
    {
        phy_stop = DL_I2C_CONTROLLER_STOP_ENABLE;
        DL_I2C_disableInterrupt(smbus->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE);
    }
    else
    {
        phy_stop = DL_I2C_CONTROLLER_STOP_DISABLE;
        DL_I2C_clearInterruptStatus(smbus->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE);
        DL_I2C_enableInterrupt(smbus->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE);
    }


    if (length > 0) {
      DL_I2C_disableInterrupt(smbus->phy.SMBus_Phy_i2cBase,
                             DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
      DL_I2C_flushControllerTXFIFO(smbus->phy.SMBus_Phy_i2cBase);
      DL_I2C_clearInterruptStatus(smbus->phy.SMBus_Phy_i2cBase,
                                  DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);
      DL_I2C_enableInterrupt(smbus->phy.SMBus_Phy_i2cBase,
                            DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_TRIGGER);

      SMBus_NWK_controllerProcessTx(smbus, &data);
      DL_I2C_fillControllerTXFIFO(smbus->phy.SMBus_Phy_i2cBase, &data, 1);
    }

    if (setPEC == 1 && smbus->ctrl.bits.pecEn == 1) {
        DL_I2C_setControllerPECCountValue(smbus->phy.SMBus_Phy_i2cBase, length);
    }
    else {
        DL_I2C_setControllerPECCountValue(smbus->phy.SMBus_Phy_i2cBase, 0);
    }

    DL_I2C_startControllerTransferAdvanced(smbus->phy.SMBus_Phy_i2cBase,
                                           targetaddr,
                                           DL_I2C_CONTROLLER_DIRECTION_TX,
                                           length,
                                           DL_I2C_CONTROLLER_START_ENABLE,
                                           phy_stop,
                                           DL_I2C_CONTROLLER_ACK_ENABLE);
}

void SMBus_PHY_controllerStartRx(SMBus *smbus,
                             uint8_t targetaddr,
                             uint16_t length,
                             SMBus_Start startFlag,
                             SMBus_Stop stopFlag,
                             SMBus_Auto_Ack ackFlag,
                             uint8_t setPEC)
{
    DL_I2C_CONTROLLER_START phy_start;
    DL_I2C_CONTROLLER_STOP phy_stop;
    DL_I2C_CONTROLLER_ACK phy_ack;
    uint8_t data;

    if (startFlag == SMBus_Start_Before_Transfer)
    {
        phy_start = DL_I2C_CONTROLLER_START_ENABLE;
    }
    else
    {
        phy_start = DL_I2C_CONTROLLER_START_DISABLE;
    }

    if (stopFlag == SMBus_Stop_After_Transfer)
    {
        phy_stop = DL_I2C_CONTROLLER_STOP_ENABLE;
    }
    else
    {
        phy_stop = DL_I2C_CONTROLLER_STOP_DISABLE;
    }

    if (ackFlag == SMBus_Auto_Ack_Last_Byte)
    {
        phy_ack = DL_I2C_CONTROLLER_ACK_ENABLE;
    }
    else
    {
        phy_ack = DL_I2C_CONTROLLER_ACK_DISABLE;
    }

    if (setPEC == 1 && smbus->ctrl.bits.pecEn == 1) {
        DL_I2C_setControllerPECCountValue(smbus->phy.SMBus_Phy_i2cBase, length);
    }
    else {
        DL_I2C_setControllerPECCountValue(smbus->phy.SMBus_Phy_i2cBase, 0);
    }

    // HW will ignore new transfer requests while a transfer is active, so wait until the HW is ready + 1 delay_cycle
    // In normal operation this should just be a small region of time between when the HW has received a byte
    // and while ack is being generated.
    while ((DL_I2C_getControllerStatus(smbus->phy.SMBus_Phy_i2cBase) | DL_I2C_CONTROLLER_STATUS_BUSY) == DL_I2C_CONTROLLER_STATUS_BUSY )
        ;

    __NOP();

    DL_I2C_startControllerTransferAdvanced(smbus->phy.SMBus_Phy_i2cBase,
                                           targetaddr,
                                           DL_I2C_CONTROLLER_DIRECTION_RX,
                                           length,
                                           phy_start,
                                           phy_stop,
                                           phy_ack);

}

SMBus_State SMBus_PHY_controllerProcessInt(SMBus *smbus)
{
    uint8_t data;
    SMBus_State ret_state;
    SMBus_Phy *SMBusPHY = &smbus->phy;

    switch (DL_I2C_getPendingInterrupt(SMBusPHY->SMBus_Phy_i2cBase)) {
    // Check for NACK
    case DL_I2C_IIDX_CONTROLLER_NACK:
        //__BKPT(0);
        // Pass data to NWK layer
        ret_state = SMBus_NWK_controllerProcessNACK(smbus);
        break;
    case DL_I2C_IIDX_CONTROLLER_ARBITRATION_LOST:
        //__BKPT(0);
        break;
    case DL_I2C_IIDX_CONTROLLER_START:
        //__BKPT(0);
        break;
    // Check if there is a RX pending
    case DL_I2C_IIDX_CONTROLLER_RXFIFO_TRIGGER:
        //__BKPT(0);
        data = DL_I2C_receiveControllerData(SMBusPHY->SMBus_Phy_i2cBase);
        // Pass data to NWK layer
        ret_state = SMBus_NWK_controllerProcessRx(smbus, data);
        break;
    case DL_I2C_IIDX_CONTROLLER_TXFIFO_TRIGGER:

        //__BKPT(0);
        // TXIFG0 is set when the TX buffer is empty and we need to send data to
        // target

        ret_state = SMBus_NWK_controllerProcessTx(smbus, &data);
        // Send the data
        DL_I2C_fillControllerTXFIFO(SMBusPHY->SMBus_Phy_i2cBase, &data, 1);
        break;

    case DL_I2C_IIDX_CONTROLLER_TX_DONE:
        SMBus_NWK_controllerTxDone(smbus);
        break;
    case DL_I2C_IIDX_CONTROLLER_STOP:
        //__BKPT(0);
        // Inform the NWK later
        ret_state = SMBus_NWK_controllerProcessStop(smbus);
        break;
    case DL_I2C_IIDX_TIMEOUT_A:
        // This flag is set when a clock timeout is detected
        // Inform the NWK of the error
        ret_state = SMBus_NWK_controllerProcessTimeout(smbus);
    case DL_I2C_IIDX_TIMEOUT_B:
        // This flag is set when a clock timeout is detected
        // Inform the NWK of the error
        ret_state = SMBus_NWK_controllerProcessTimeout(smbus);
    default:
        break;
    }

    return(ret_state);
}



