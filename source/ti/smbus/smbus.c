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
//
// Include files
//
#include <stdint.h>
#include "smbus.h"
#include "smbus_nwk.h"
#include "smbus_phy.h"


void SMBus_processDone(SMBus *smbus)
{
    if (smbus->ctrl.bits.swackEn == true)
    {
        /* Send a manual ACK if no issues processing received data */
        if (smbus->phy.SMBus_Phy_AckPending == true)
        {
            if ( (smbus->state == SMBus_State_Target_FirstByte) ||
                 (smbus->state == SMBus_State_Target_ByteReceived) )
            {
                SMBus_PHY_targetManualACK(smbus,  true);
            }
            else
            {
                if(smbus->arpStatus.arpInProgress)
                {
                    smbus->arpStatus.arpInProgress = 0;
                }
                SMBus_PHY_targetManualACK(smbus,  false);
            }
        }
    }
    // Clear the State of SMBus target
    smbus->state = SMBus_State_OK;
}

uint16_t SMBus_getRxPayloadAvailable(SMBus *smbus)
{
    return(SMBus_NWK_getRxPayloadSize(smbus));
}

SMBus_State SMBus_getState(SMBus *smbus)
{
    return(smbus->state);
}


void SMBus_enablePEC(SMBus *smbus)
{
    smbus->ctrl.bits.pecEn = 1;
}

void SMBus_disablePEC(SMBus *smbus)
{
    smbus->ctrl.bits.pecEn = 0;
}


void SMBus_targetInit(SMBus *smbus, I2C_Regs *i2cAddr)
{
    smbus->nwk.eState = SMBus_NwkState_Idle;
    smbus->nwk.currentCmd = 0;
    smbus->nwk.currentAddr = 0;
    smbus->nwk.rxBuffPtr = NULL;
    smbus->nwk.rxSize = 0;
    smbus->nwk.rxLen = 0;
    smbus->nwk.rxIndex = 0;
    smbus->nwk.txBuffPtr = NULL;
    smbus->nwk.txSize = 0;
    smbus->nwk.txIndex = 0;
    smbus->nwk.txLen = 0;
    smbus->nwk.recByteTxPtr = &smbus->status.u8byte;
    smbus->nwk.pecBlockLenOverride = 0;
    smbus->ctrl.u8byte = 0;
    smbus->status.u8byte = 0;
    smbus->state = SMBus_State_OK;
    smbus->ctrl.bits.controller = 0;  // Update control flag;

    SMBus_PHY_targetInit(smbus, i2cAddr);
}

void SMBus_targetEnableInt(SMBus *smbus)
{
    SMBus_PHY_targetEnableInt(smbus);
}

void SMBus_targetDisableInt(SMBus *smbus)
{
    SMBus_PHY_targetDisableInt(smbus);
}


SMBus_State SMBus_targetProcessInt(SMBus *smbus)
{
    SMBus_State New_State;
    New_State = SMBus_PHY_targetProcessInt(smbus);

    if(New_State != SMBus_State_Target_NTR)
    {
        smbus->state = New_State;
    }

    return(New_State);
}

void SMBus_targetSetAddress(SMBus *smbus,
                           uint8_t targetAddr)
{
    // Set new Target Address
    smbus->ownTargetAddr = targetAddr;
}

void SMBus_targetSetRxBuffer(SMBus *smbus,
                            uint8_t *data,
                            uint16_t size)
{
    // Set pointer and size of RX Buffer
    smbus->nwk.rxBuffPtr = data;
    smbus->nwk.rxSize = size;
}

void SMBus_targetSetTxBuffer(SMBus *smbus,
                            uint8_t *data,
                            uint16_t size)
{
    // Set pointer and size of TX Buffer
    smbus->nwk.txBuffPtr = data;
    smbus->nwk.txSize = size;
}

void SMBus_targetReportError(SMBus *smbus,
                            SMBus_ErrorCode errorCode)
{
    switch(errorCode)
    {
    case SMBus_ErrorCode_Packet:
        smbus->status.bits.packErr = 1;
        smbus->state = SMBus_State_Target_Error;
        break;
    case SMBus_ErrorCode_Cmd:
        smbus->status.bits.cmdErr = 1;
        smbus->state = SMBus_State_Target_Error;
        break;
    default:
        return;
    }
    // Update flags in order to invalidate current packet
    smbus->nwk.eState = SMBus_NwkState_Error;
}

uint8_t SMBus_targetGetCommand(SMBus *smbus)
{
    return (smbus->nwk.currentCmd);
}

void SMBus_targetReportBlock(SMBus *smbus){
    /* report a block such that the next state upon receiving should be considered
     * a block Length, such that it's interpreted as a length.
     */
    if (smbus->ctrl.bits.pecEn == 1) {
        smbus->nwk.pecBlockLenOverride = 1;
    }
}


void SMBus_targetReportLength(SMBus *smbus, uint16_t length){
    /* The PECCount should be the length plus the PECbit as well*/
    if (smbus->ctrl.bits.pecEn == 1) {
        SMBus_PHY_targetSetPECCount(smbus, length + 1);
    }
}

uint8_t SMBus_targetClearStatusReg(SMBus *smbus,
                                  uint8_t val)
{
    smbus->status.u8byte &= val;
    return(smbus->status.u8byte);
}

uint8_t SMBus_targetWriteCtrlReg(SMBus *smbus,
                                uint8_t val)
{
    smbus->ctrl.writeableBits.writeBits = val;

    return(smbus->ctrl.u8byte);
}

int8_t SMBus_targetHostAlert(SMBus *smbus,
                                 uint8_t deviceAddress,
                                 uint8_t *txData)
{
    if(smbus->nwk.eState != SMBus_NwkState_Idle)
    {
        return(SMBUS_RET_ERROR);
    }

    smbus->nwk.currentCmd = deviceAddress;
    smbus->nwk.rxLen = 0; // No bytes to read
    smbus->nwk.txLen = 1 + 2; // Command + Size
    smbus->nwk.txBuffPtr = txData;
    smbus->nwk.eState = SMBus_NwkState_TXHostAlert;
    smbus->nwk.currentAddr = SMB_HOST_DEFAULT_ADDRESS;

    SMBus_NWK_startTxTransfer(smbus);

    return(SMBUS_RET_OK);
}

void SMBus_controllerInit(SMBus *smbus,
                      I2C_Regs *i2cAddr,
                      uint32_t busClk)
{
    smbus->nwk.eState = SMBus_NwkState_Idle;
    smbus->nwk.currentAddr = 0;
    smbus->nwk.rxBuffPtr = NULL;
    smbus->nwk.rxSize = 0;
    smbus->nwk.rxLen = 0;
    smbus->nwk.rxIndex = 0;
    smbus->nwk.txBuffPtr = NULL;
    smbus->nwk.txSize = 0;
    smbus->nwk.txIndex = 0;
    smbus->nwk.txLen = 0;
    smbus->nwk.recByteTxPtr = &smbus->status.u8byte;
    smbus->nwk.pecBlockLenOverride = 0;
    smbus->ctrl.u8byte = 0;
    smbus->status.u8byte = 0;
    smbus->state = SMBus_State_OK;
    smbus->ctrl.bits.controller = 1;  // Update control flag;

    SMBus_PHY_controllerInit(smbus, i2cAddr, busClk);
}

void SMBus_controllerEnableInt(SMBus *smbus)
{
    SMBus_PHY_controllerEnableInt(smbus);
}

void SMBus_controllerDisableInt(SMBus *smbus)
{
    SMBus_PHY_controllerEnableInt(smbus);
}


SMBus_State SMBus_controllerProcessInt(SMBus *smbus)
{
    return(SMBus_PHY_controllerProcessInt(smbus));
}

void SMBus_controllerReset(SMBus *smbus)
{
    SMBus_NWK_controllerReset(smbus);
}

int8_t SMBus_controllerProcessCall(SMBus *smbus,
                               uint8_t targetAddr,
                               uint8_t command,
                               uint8_t *txData,
                               uint8_t *rxData)
{
    if(smbus->nwk.eState != SMBus_NwkState_Idle)
    {
        return(SMBUS_RET_ERROR);
    }

    smbus->nwk.currentCmd = command;
    smbus->nwk.rxLen = 2; // Read 2 bytes
    smbus->nwk.rxBuffPtr = rxData;
    smbus->nwk.txLen = 3; // 2 bytes + Cmd
    smbus->nwk.txBuffPtr = txData;
    smbus->nwk.eState = SMBus_NwkState_TX;
    smbus->nwk.currentAddr = targetAddr;

    SMBus_NWK_startTxTransfer(smbus);

    return(SMBUS_RET_OK);
}

int8_t SMBus_controllerProcessCallBlock(SMBus *smbus,
                                    uint8_t targetAddr,
                                    uint8_t command,
                                    uint8_t *txData,
                                    uint8_t txSize,
                                    uint8_t *rxData)
{
    if(smbus->nwk.eState != SMBus_NwkState_Idle)
    {
        return(SMBUS_RET_ERROR);
    }

    if((txSize == 0) || (txSize > SMB_MAX_PAYLOAD_SIZE))
    {
        return(SMBUS_RET_ERROR);
    }

    smbus->nwk.currentCmd = command;
    smbus->nwk.rxLen = 3; // Minimal size until we get the real Byte count
    smbus->nwk.rxBuffPtr = rxData;
    smbus->nwk.txLen = txSize + 2; // Size includes Command and byte count
    smbus->nwk.txBuffPtr = txData;
    smbus->nwk.eState = SMBus_NwkState_TX_Block;
    smbus->nwk.currentAddr = targetAddr;

    SMBus_NWK_startTxTransfer(smbus);

    return(SMBUS_RET_OK);
}

int8_t SMBus_controllerSendByte(SMBus *smbus,
                            uint8_t targetAddr,
                            uint8_t txData)
{
    if(smbus->nwk.eState != SMBus_NwkState_Idle)
    {
        return(SMBUS_RET_ERROR);
    }
    smbus->nwk.currentCmd = txData;
    smbus->nwk.rxLen = 0;  //No read
    smbus->nwk.txLen = 1; // Only Command(Data)
    smbus->nwk.eState = SMBus_NwkState_TX;
    smbus->nwk.currentAddr = targetAddr;

    SMBus_NWK_startTxTransfer(smbus);

    return(SMBUS_RET_OK);
}

int8_t SMBus_controllerReceiveByte(SMBus *smbus,
                               uint8_t targetAddr,
                               uint8_t *rxData)
{
    if(smbus->nwk.eState != SMBus_NwkState_Idle)
    {
        return(SMBUS_RET_ERROR);
    }
    smbus->nwk.rxLen = 1;  //Read 1 byte
    smbus->nwk.rxBuffPtr = rxData;
    smbus->nwk.txLen = 0; // Only Command(Data)
    smbus->nwk.eState = SMBus_NwkState_RX;
    smbus->nwk.currentAddr = targetAddr;

    // Send Start and stop after address is sent
    SMBus_NWK_startRxTransfer(smbus);

    return(SMBUS_RET_OK);
}

int8_t SMBus_controllerReadBlock(SMBus *smbus,
                             uint8_t targetAddr,
                             uint8_t command,
                             uint8_t *rxData)
{
    if(smbus->nwk.eState != SMBus_NwkState_Idle)
    {
        return(SMBUS_RET_ERROR);
    }

    smbus->nwk.currentCmd = command;
    smbus->nwk.rxLen = 1;
    smbus->nwk.rxBuffPtr = rxData;
    smbus->nwk.txLen = 1; // Only send command
    smbus->nwk.eState = SMBus_NwkState_TX_Block;
    smbus->nwk.currentAddr = targetAddr;

    SMBus_NWK_startTxTransfer(smbus);

    return(SMBUS_RET_OK);
}

int8_t SMBus_controllerWriteBlock(SMBus *smbus,
                              uint8_t targetAddr,
                              uint8_t command,
                              uint8_t *txData,
                              uint16_t txSize)
{
    if(smbus->nwk.eState != SMBus_NwkState_Idle)
    {
        return(SMBUS_RET_ERROR);
    }
    if((txSize == 0) || (txSize > SMB_MAX_PAYLOAD_SIZE))
    {
        return(SMBUS_RET_ERROR);
    }

    smbus->nwk.currentCmd = command;
    smbus->nwk.rxLen = 0;
    smbus->nwk.txLen = txSize + 2; // Size includes Command and byte count
    smbus->nwk.txBuffPtr = txData;
    smbus->nwk.eState = SMBus_NwkState_TX_Block;
    smbus->nwk.currentAddr = targetAddr;

    SMBus_NWK_startTxTransfer(smbus);

    return(SMBUS_RET_OK);
}

int8_t SMBus_controllerReadByteWord(SMBus *smbus,
                                uint8_t targetAddr,
                                uint8_t command,
                                uint8_t *rxData,
                                uint8_t rxSize)
{
    if(smbus->nwk.eState != SMBus_NwkState_Idle)
    {
        return(SMBUS_RET_ERROR);
    }

    if((rxSize != 1) && (rxSize != 2) && (rxSize != 4) && (rxSize != 8))
    {
        return(SMBUS_RET_ERROR);
    }

    smbus->nwk.currentCmd = command;
    smbus->nwk.rxLen = rxSize; // Read 1 or 2 bytes
    smbus->nwk.rxBuffPtr = rxData;
    smbus->nwk.txLen = 1; // Only send command
    smbus->nwk.eState = SMBus_NwkState_TX;
    smbus->nwk.currentAddr = targetAddr;

    SMBus_NWK_startTxTransfer(smbus);

    return(SMBUS_RET_OK);
}

int8_t SMBus_controllerWriteByteWord(SMBus *smbus,
                                 uint8_t targetAddr,
                                 uint8_t command,
                                 uint8_t *txData,
                                 uint8_t txSize)
{
    if(smbus->nwk.eState != SMBus_NwkState_Idle)
    {
        return(SMBUS_RET_ERROR);
    }

    if((txSize != 1) && (txSize != 2))
    {
        return(SMBUS_RET_ERROR);
    }

    smbus->nwk.currentCmd = command;
    smbus->nwk.rxLen = 0; // Read 1 or 2 bytes
    smbus->nwk.txLen = 1 + txSize; // Command + Size
    smbus->nwk.txBuffPtr = txData;
    smbus->nwk.eState = SMBus_NwkState_TX;
    smbus->nwk.currentAddr = targetAddr;

    SMBus_NWK_startTxTransfer(smbus);

    return(SMBUS_RET_OK);
}

int8_t SMBus_controllerQuickCommand(SMBus *smbus,
                                uint8_t targetAddr,
                                bool write)
{
    if(smbus->nwk.eState != SMBus_NwkState_Idle)
    {
        return(SMBUS_RET_ERROR);
    }

    if(write != true)
    {
        return(SMBUS_RET_ERROR);
    }

    smbus->nwk.rxLen = 0;  //No read
    smbus->nwk.txLen = 0; // No Command
    smbus->nwk.eState = SMBus_NwkState_TXQuickCMD;
    smbus->nwk.currentAddr = targetAddr;

    if(write == true)
    {
        SMBus_NWK_startTxTransfer(smbus);
    }

    return(SMBUS_RET_OK);
}

int8_t SMBus_controllerWaitUntilDone(SMBus *smbus,
                                 int32_t timeout)
{
    while((smbus->nwk.eState != SMBus_NwkState_Idle) && timeout)
    {
        timeout--;
    }

    if(!timeout)
    {
        return(SMBUS_RET_ERROR);
    }
    else
    {
        return(SMBUS_RET_OK);
    }
}

void SMBus_controllerEnableHostNotify(SMBus *smbus, uint8_t *hostAlertBuffer)
{
    SMBus_NWK_controllerEnableHostNotify(smbus, hostAlertBuffer);
    SMBus_PHY_controllerEnableHostNotify(smbus);
    smbus->ctrl.bits.hostNotifyEn = 1;
}

void SMBus_controllerDisableHostNotify(SMBus *smbus)
{
    SMBus_NWK_controllerDisableHostNotify(smbus);
    SMBus_PHY_controllerDisableHostNotify(smbus);
    smbus->ctrl.bits.hostNotifyEn = 0;
}
uint8_t SMBus_targetGetAddress(SMBus *smbus)
{
    // Return new Target Address
    return(smbus->ownTargetAddr);
}


void SMBusARPAssignAddress(SMBus *smbus)
{
    smbus->ownTargetAddr = smbus->nwk.rxBuffPtr[smbus->nwk.rxIndex -2] >> 1;

    smbus->arpStatus.arpAddResolved = 1;
    smbus->arpStatus.arpAddressValid = 1;
    /* Update the device's new Target Own Address from the Target through ARP */
    SMBus_Phy_ARP_TARGET_ADDR_Reset(smbus);
}

int8_t SMBusARPUDIDByteValidate(SMBus *smbus)
{
    smbus->arpStatus.arpRxData = smbus->nwk.rxBuffPtr[(smbus->nwk.rxIndex-1)];
    tSMBusUDID *pUDID = smbus->nwk.pUDID;
   switch((smbus->nwk.rxIndex - 1))
   {
       /* Length of the Bytes*/
       case 0x01:
           if(smbus->arpStatus.arpRxData != SMBUS_ARP_UDID_LEN)
           {
               return(SMBUS_RET_ERROR);
           }
           break;
       case 0x02:
           if(smbus->arpStatus.arpRxData != ((pUDID->ui8DeviceCapabilities) & 0xFF))
           {
               return(SMBUS_RET_ERROR);
           }
           break;
       case 0x03:
           if(smbus->arpStatus.arpRxData != ((pUDID->ui8Version) & 0xFF))
           {
               return(SMBUS_RET_ERROR);
           }
          break;
       case 0x04:
           if(smbus->arpStatus.arpRxData != ((uint8_t)((pUDID->ui16VendorID & 0xff00) >> 8)))
           {
               return(SMBUS_RET_ERROR);
           }
           break;
       case 0x05:
           if(smbus->arpStatus.arpRxData != ((uint8_t)(pUDID->ui16VendorID & 0x00ff)))
           {
               return(SMBUS_RET_ERROR);
           }
           break;
       case 0x06:
           if(smbus->arpStatus.arpRxData != ((uint8_t)((pUDID->ui16DeviceID & 0xff00) >> 8)))
           {
               return(SMBUS_RET_ERROR);
           }
          break;
       case 0x07:
           if(smbus->arpStatus.arpRxData != ((uint8_t)(pUDID->ui16DeviceID & 0x00ff)))
           {
               return(SMBUS_RET_ERROR);
           }
           break;
       case 0x08:
           if(smbus->arpStatus.arpRxData != ((uint8_t)((pUDID->ui16Interface & 0xff00) >> 8)))
           {
               return(SMBUS_RET_ERROR);
           }
           break;
       case 0x09:
           if(smbus->arpStatus.arpRxData != ((uint8_t)(pUDID->ui16Interface & 0x00ff)))
           {
               return(SMBUS_RET_ERROR);
           }
          break;
       case 0x0A:
           if(smbus->arpStatus.arpRxData != ((uint8_t)((pUDID->ui16SubSystemVendorID & 0xff00) >> 8)))
           {
               return(SMBUS_RET_ERROR);
           }
           break;
       case 0x0B:
           if(smbus->arpStatus.arpRxData != ((uint8_t)(pUDID->ui16SubSystemVendorID & 0x00ff)))
           {
               return(SMBUS_RET_ERROR);
           }
           break;
       case 0x0C:
           if(smbus->arpStatus.arpRxData != ((uint8_t)((pUDID->ui16SubSystemDeviceID & 0xff00) >> 8)))
           {
               return(SMBUS_RET_ERROR);
           }
          break;
       case 0x0D:
           if(smbus->arpStatus.arpRxData != ((uint8_t)(pUDID->ui16SubSystemDeviceID & 0x00ff)))
           {
               return(SMBUS_RET_ERROR);
           }
           break;
       case 0x0E:
           if(smbus->arpStatus.arpRxData != ((uint8_t)((pUDID->ui32VendorSpecificID & 0xff000000) >>
                   24)))
           {
               return(SMBUS_RET_ERROR);
           }
          break;
       case 0x0F:
           if(smbus->arpStatus.arpRxData != ((uint8_t)((pUDID->ui32VendorSpecificID & 0x00ff0000) >>
                   16)))
           {
               return(SMBUS_RET_ERROR);
           }
           break;
       case 0x10:
           if(smbus->arpStatus.arpRxData != ((uint8_t)((pUDID->ui32VendorSpecificID & 0x0000ff00) >>
                   8)))

           {
              return(SMBUS_RET_ERROR);
           }
           break;
       case 0x11:
           if(smbus->arpStatus.arpRxData != ((uint8_t)(pUDID->ui32VendorSpecificID & 0x000000ff)))
           {
               return(SMBUS_RET_ERROR);
           }
          break;
      /* Wait for the command completion to acknowledge the Target Address */
       default:
          break;
   }
   return(SMBUS_RET_OK_FIXED);
}

void SMBusARPGetUDIDPacketDirect(SMBus *smbus)
{
    tSMBusUDID *pUDID = smbus->nwk.pUDID;
    //
    // Place data from the UDID structure and address into the data buffer
    // using the correct MSB->LSB + address order.
    //
    smbus->nwk.txBuffPtr[0] = SMBUS_ARP_UDID_LEN;

    smbus->nwk.txBuffPtr[1] = pUDID->ui8DeviceCapabilities;
    smbus->nwk.txBuffPtr[2] = pUDID->ui8Version;
    smbus->nwk.txBuffPtr[3] = (uint8_t)((pUDID->ui16VendorID & 0xff00) >> 8);
    smbus->nwk.txBuffPtr[4] = (uint8_t)(pUDID->ui16VendorID & 0x00ff);
    smbus->nwk.txBuffPtr[5] = (uint8_t)((pUDID->ui16DeviceID & 0xff00) >> 8);
    smbus->nwk.txBuffPtr[6] = (uint8_t)(pUDID->ui16DeviceID & 0x00ff);
    smbus->nwk.txBuffPtr[7] = (uint8_t)((pUDID->ui16Interface & 0xff00) >> 8);
    smbus->nwk.txBuffPtr[8] = (uint8_t)(pUDID->ui16Interface & 0x00ff);
    smbus->nwk.txBuffPtr[9] = (uint8_t)((pUDID->ui16SubSystemVendorID & 0xff00) >> 8);
    smbus->nwk.txBuffPtr[10] = (uint8_t)(pUDID->ui16SubSystemVendorID & 0x00ff);
    smbus->nwk.txBuffPtr[11] = (uint8_t)((pUDID->ui16SubSystemDeviceID & 0xff00) >> 8);
    smbus->nwk.txBuffPtr[12] = (uint8_t)(pUDID->ui16SubSystemDeviceID & 0x00ff);
    smbus->nwk.txBuffPtr[13] = (uint8_t)((pUDID->ui32VendorSpecificID & 0xff000000) >>
                            24);
    smbus->nwk.txBuffPtr[14] = (uint8_t)((pUDID->ui32VendorSpecificID & 0x00ff0000) >>
                             16);
    smbus->nwk.txBuffPtr[15] = (uint8_t)((pUDID->ui32VendorSpecificID & 0x0000ff00) >>
                             8);
    smbus->nwk.txBuffPtr[16] = (uint8_t)(pUDID->ui32VendorSpecificID & 0x000000ff);

    /* If SMBus has Valid Address, send the DTA/PTA */
    if(smbus->arpStatus.arpAddressValid)
        smbus->nwk.txBuffPtr[17] = (smbus->ownTargetAddr << 1) | 0x01;
    /* if the AV Flag is cleared , Send 0x7F for the Address and 1b at LSB*/
    else
        smbus->nwk.txBuffPtr[17] = (0x7F << 1) | 0x01; //

    // Number of bytes to send (including LEN)
    smbus->nwk.txLen = (17 + 1);
}

void SMBusARPGetUDIDPacket(SMBus *smbus)
{
    /* If the Address of the target is already resolved and UDID Command is
    * acked from a different Target, Send 0xff to create arbitration loss
    * for resolved Target
    * */
    if(smbus->arpStatus.arpAddResolved == 1)
    {
        smbus->nwk.txBuffPtr[0] = RESPONSE_NTR;
        smbus->nwk.txLen = 1;
    }
    else
    {
        SMBusARPGetUDIDPacketDirect(smbus);
    }

}

void SMBusARP_RESET(SMBus *smbus)
{
    volatile uint8_t addressType = (smbus->nwk.pUDID->ui8DeviceCapabilities & 0xC0) >> 6;

    switch(addressType)
    {
        //Default Address Mode
        case 0x00:
            smbus->arpStatus.arpAddResolved = 0x00;
            smbus->arpStatus.arpAddressValid = 0x01;
            smbus->ownTargetAddr =  SMBUS_TARGET_OWN_ADDRESS;
            break;
        // Persistent Target Address (Non- Volatile)
        case 0x01:
            smbus->arpStatus.arpAddResolved = 0x00;
            smbus->arpStatus.arpAddressValid  = ARP_ADD_VALID;
            // TOD0: Update Target Address from NVM
            smbus->ownTargetAddr =  SMBUS_TARGET_OWN_ADDRESS;
            break;
        // Volatile Target Address
        case 0x02:
            smbus->arpStatus.arpAddResolved = 0x00;
            smbus->arpStatus.arpAddressValid    = 0x00;

            break;
        // Random Number
        case 0x03:
            smbus->arpStatus.arpAddResolved = 0x00;
            smbus->arpStatus.arpAddressValid    = 0x00;
            // TODO: Generate Random UDID and call SMBusARPUDIDPacketDecode()
            break;
    }
    SMBus_Phy_ARP_TARGET_ADDR_Reset(smbus);
}

void
SMBusARPUDIDPacketDecode(tSMBusUDID *pUDID, uint8_t *pui8Data)
{
    //
    // Populate the UDID structure with data from the input data buffer.
    //
    pUDID->ui8DeviceCapabilities = pui8Data[0];
    pUDID->ui8Version = pui8Data[1];
    pUDID->ui16VendorID = (uint16_t)((pui8Data[2] << 8) | pui8Data[3]);
    pUDID->ui16DeviceID = (uint16_t)((pui8Data[4] << 8) | pui8Data[5]);
    pUDID->ui16Interface = (uint16_t)((pui8Data[6] << 8) | pui8Data[7]);
    pUDID->ui16SubSystemVendorID = (uint16_t)((pui8Data[8] << 8) |
                                              pui8Data[9]);
    pUDID->ui16SubSystemDeviceID = (uint16_t)((pui8Data[10] << 8) |
                                              pui8Data[11]);
    pUDID->ui32VendorSpecificID = (uint32_t)((pui8Data[12] << 24) |
                                             (pui8Data[13] << 16) |
                                             (pui8Data[14] << 8) |
                                             pui8Data[15]);
}


uint8_t SMBus_targetGetARPInProgress(SMBus *smbus)
{
    return(smbus->arpStatus.arpInProgress);
}

uint8_t SMBus_targetGetARStatus(SMBus *smbus)
{
    return (smbus->arpStatus.arpAddResolved);
}

void SMBus_targetSetARStatus(SMBus *smbus , uint8_t val)
{
    smbus->arpStatus.arpAddResolved = val;
}

void SMBus_targetSetAVStatus(SMBus *smbus, uint8_t val)
{
    smbus->arpStatus.arpAddressValid = val;
}

uint8_t SMBus_targetGetAVStatus(SMBus *smbus)
{
    return (smbus->arpStatus.arpAddressValid);
}

uint8_t SMBus_targetGetAddressDirect(SMBus *smbus)
{
    return ((smbus->nwk.currentCmd & 0xFE) >> 1);
}

uint8_t SMBus_targetGetDirectCmd(SMBus *smbus)
{
    return (smbus->nwk.currentCmd & 0x01);
}

int8_t ARP_isCmdValid(SMBus *smbus)
{
    /* Read the First Byte of the command */
    smbus->arpStatus.arpCommand = SMBus_targetGetCommand(smbus);

    /* Valid ARP Transaction only starts with a Write Bit - Read transaction for Target*/
    if(SMBus_Phy_ARP_Get_Write_Status(smbus))
    {
        /* Validate the First Command from the SMBus Packet, Valid ARP Transaction only starts with Write Bit */
        smbus->arpStatus.arpUDIDTransmit = 0;
        switch(smbus->arpStatus.arpCommand)
        {
            /* General Prepare ARP command */
            case SMBUS_CMD_PREPARE_TO_ARP:
                /* Always ACK irrespective of Flags */
                smbus->arpStatus.arpDataLength = 0;
                break;
            /* General ARP Reset Command */
            case SMBUS_CMD_ARP_RESET_DEVICE:
                /* Always ACK irrespective of Flags */
                smbus->arpStatus.arpDataLength = 0;
                break;
            /* General GET UDID command */
            case SMBUS_CMD_ARP_GET_UDID:

                /* If the Address is already resolved for the given Address
                 * Send NACK for General UDID Call
                */
                if(SMBus_targetGetARStatus(smbus))
                {
                    smbus->arpStatus.arpUDIDNacked = 1;
                    ARP_invalidCMD(smbus);
                }
                else
                {
                    smbus->arpStatus.arpUDIDTransmit = 1;
                }
                smbus->arpStatus.arpDataLength = 0;
                break;
            /* General Assign Address Command */
            case SMBUS_CMD_ARP_ASSIGN_ADDRESS:
                smbus->nwk.pecBlockLenOverride = 1;
                /* SMBUS UDID Address length = 17 (0x11) */
                smbus->arpStatus.arpDataLength = SMBUS_ARP_UDID_LEN;
                /* Always ACK irrespective of Flags */
                break;
            default:
                /* Direct Address Handling */
                if(SMBus_targetGetAddressDirect(smbus) == SMBus_targetGetAddress(smbus))
                {
                    // If the Target AV Status is not set and Its a directed Command, Its an illegal Command
                    if(!SMBus_targetGetAVStatus(smbus))
                    {
                        smbus->arpStatus.arpUDIDNacked = 1;
                        ARP_invalidCMD(smbus);
                    }
                    else if(SMBus_targetGetDirectCmd(smbus))
                    {
                        smbus->arpStatus.arpUDIDTransmit = 1;
                    }
                    smbus->arpStatus.arpDataLength = 0;
                }
                /* If Valid Command is not received, return error */
                else
                {
                    ARP_invalidCMD(smbus);
                }
                break;
        }
    }

    /* All the ARP Command Responses are of Fixed Length */
    /* At the End of the State, an ACK will be sent by the Target
     * if the Return State is Fixed / Block*/
    return(SMBUS_RET_OK_FIXED);
}


int8_t ARP_UDIDValidate(SMBus *smbus)
{
    if(smbus->arpStatus.arpCommand == SMBUS_CMD_ARP_ASSIGN_ADDRESS)
    {
        return(SMBusARPUDIDByteValidate(smbus));
    }
    return(SMBUS_RET_OK_FIXED);
}

void  ARP_invalidCMD(SMBus *smbus)
{
    smbus->arpStatus.errorState = SMBus_State_Invalid_ARP_Cmd;
    smbus->arpStatus.arpInProgress = 0;

    /* send the NACK from the Target */
    SMBus_PHY_targetManualACK(smbus,  false);
    smbus->phy.SMBus_Phy_AckPending = false;

    /* Initiate the Buffer with NTR Packet In case of Multi Target System */
    smbus->nwk.pecBlockLenOverride = 1;
    smbus->arpStatus.arpDataLength = SMBUS_ARP_UDID_LEN;
    smbus->nwk.txBuffPtr[0] = RESPONSE_NTR;
    SMBus_targetReportLength(smbus, smbus->arpStatus.arpDataLength);

}

int8_t ARP_CmdComplete(SMBus *smbus)
{
    /* After the Stop Byte Reset the ARP Status */
    smbus->arpStatus.arpInProgress = 0;
    /* Evaluate the Command from the SMBus Packet to Process ARP */
    switch(smbus->arpStatus.arpCommand)
    {
        /* Prepare ARP command */
        case SMBUS_CMD_PREPARE_TO_ARP:
            /* Always ACK irrespective of Flags */
            SMBus_targetSetARStatus(smbus, 0);
            //TODO: Suspend Notify Host Function
            break;
        /* General ARP Reset Command */
        case SMBUS_CMD_ARP_RESET_DEVICE:
            SMBusARP_RESET(smbus);
            /* Always ACK irrespective of Flags */
            break;
        /* General GET UDID command */
        case SMBUS_CMD_ARP_GET_UDID:
            SMBusARPGetUDIDPacket(smbus);
            break;
        /* General Assign Address Command */
        case SMBUS_CMD_ARP_ASSIGN_ADDRESS:
            SMBusARPAssignAddress(smbus);
            /* Always ACK irrespective of Flags */
            break;
        default:
            /* Direct Address Handling */
            // If the Target AV Status is not set and Its a directed Command, Its an illegal Command
            // If last bit of Direct Address is 1b -> Get UDID.
            if(SMBus_targetGetAddressDirect(smbus) == SMBus_targetGetAddress(smbus))
            {
                if(SMBus_targetGetDirectCmd(smbus))
                {
                    SMBusARPGetUDIDPacketDirect(smbus);
                }
                else
                {
                    SMBusARP_RESET(smbus);
                }
            }
            /* If Valid Command is not received, return error */
            else
            {
                ARP_invalidCMD(smbus);
            }

            break;
    }
    /* All the ARP Command Responses are of Fixed Length */
    /* At the End of the State, an ACK will be sent by the Target */
    return(SMBUS_RET_OK_FIXED);
}
