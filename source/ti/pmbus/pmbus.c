/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
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
#include "ti/smbus/smbus.h"
#include "ti/smbus/smbus_nwk.h"
#include "ti/smbus/smbus_phy.h"
#include "pmbus.h"

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#define PMB_RESERVED                                0x00

#define PMB_NO_READ                                 0x01
#define PMB_READ_BYTE                               0x02
#define PMB_READ_WORD                               0x03
#define PMB_BLOCK_READ                              0x04

#define PMB_NO_WRITE                                0x10
#define PMB_SEND_BYTE                               0x20
#define PMB_WRITE_BYTE                              0x30
#define PMB_WRITE_WORD                              0x40
#define PMB_BLOCK_WRITE                             0x50
#define PMB_EXTENDED_CMD                            0xFF

uint8_t PMBus_Commands[] =
{
    // writeTransaction|readTransaction,       // Command name, opcode
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // PAGE,00h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // OPERATION,01h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // ON_OFF_CONFIG,02h
    PMB_SEND_BYTE | PMB_NO_READ,               // CLEAR_FAULTS,03h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // PHASE,04h
    PMB_BLOCK_WRITE | PMB_NO_READ,             // PAGE_PLUS_WRITE,05h
    PMB_RESERVED,                              // PAGE_PLUS_READ,06h
    PMB_RESERVED,                              // RESERVED,07h
    PMB_WRITE_WORD | PMB_READ_WORD,            // ZONE_ACTIVE,08h
    PMB_RESERVED,                              // RESERVED,09h
    PMB_RESERVED,                              // RESERVED,0Ah
    PMB_RESERVED,                              // RESERVED,0Bh
    PMB_RESERVED,                              // RESERVED,0Ch
    PMB_RESERVED,                              // RESERVED,0Dh
    PMB_RESERVED,                              // RESERVED,0Eh
    PMB_RESERVED,                              // RESERVED,0Fh
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // WRITE_PROTECT,10h
    PMB_SEND_BYTE | PMB_NO_READ,               // STORE_DEFAULT_ALL,11h
    PMB_SEND_BYTE | PMB_NO_READ,               // RESTORE_DEFAULT_ALL,12h
    PMB_WRITE_BYTE | PMB_NO_READ,              // STORE_DEFAULT_CODE,13h
    PMB_WRITE_BYTE | PMB_NO_READ,              // RESTORE_DEFAULT_CODE,14h
    PMB_SEND_BYTE | PMB_NO_READ,               // STORE_USER_ALL,15h
    PMB_SEND_BYTE | PMB_NO_READ,               // RESTORE_USER_ALL,16h
    PMB_WRITE_BYTE | PMB_NO_READ,              // STORE_USER_CODE,17h
    PMB_WRITE_BYTE | PMB_NO_READ,              // RESTORE_USER_CODE,18h
    PMB_NO_WRITE | PMB_READ_BYTE,              // CAPABILITY,19h
    PMB_RESERVED,                              // QUERY,1Ah
    PMB_RESERVED,                              // SMBALERT_MASK,1Bh
    PMB_RESERVED,                              // RESERVED,1Ch
    PMB_RESERVED,                              // RESERVED,1Dh
    PMB_RESERVED,                              // RESERVED,1Eh
    PMB_RESERVED,                              // RESERVED,1Fh
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // VOUT_MODE,20h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VOUT_COMMAND,21h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VOUT_TRIM,22h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VOUT_CAL_OFFSET,23h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VOUT_MAX,24h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VOUT_MARGIN_HIGH,25h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VOUT_MARGIN_LOW,26h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VOUT_TRANSITION_RATE,27h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VOUT_DROOP,28h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VOUT_SCALE_LOOP,29h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VOUT_SCALE_MONITOR,2Ah
    PMB_WRITE_WORD | PMB_READ_WORD,            // VOUT_MIN,2Bh
    PMB_RESERVED,                              // RESERVED,2Ch
    PMB_RESERVED,                              // RESERVED,2Dh
    PMB_RESERVED,                              // RESERVED,2Eh
    PMB_RESERVED,                              // RESERVED,2Fh
    PMB_RESERVED,                              // COEFFICIENTS,30h
    PMB_WRITE_WORD | PMB_READ_WORD,            // POUT_MAX,31h
    PMB_WRITE_WORD | PMB_READ_WORD,            // MAX_DUTY,32h
    PMB_WRITE_WORD | PMB_READ_WORD,            // FREQUENCY_SWITCH,33h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // POWER_MODE,34h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VIN_ON,35h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VIN_OFF,36h
    PMB_WRITE_WORD | PMB_READ_WORD,            // INTERLEAVE,37h
    PMB_WRITE_WORD | PMB_READ_WORD,            // IOUT_CAL_GAIN,38h
    PMB_WRITE_WORD | PMB_READ_WORD,            // IOUT_CAL_OFFSET,39h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // FAN_CONFIG_1_2,3Ah
    PMB_WRITE_WORD | PMB_READ_WORD,            // FAN_COMMAND_1,3Bh
    PMB_WRITE_WORD | PMB_READ_WORD,            // FAN_COMMAND_2,3Ch
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // FAN_CONFIG_3_4,3Dh
    PMB_WRITE_WORD | PMB_READ_WORD,            // FAN_COMMAND_3,3Eh
    PMB_WRITE_WORD | PMB_READ_WORD,            // FAN_COMMAND_4,3Fh
    PMB_WRITE_WORD | PMB_READ_WORD,            // VOUT_OV_FAULT_LIMIT,40h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // VOUT_OV_FAULT_RESPONSE,41h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VOUT_OV_WARN_LIMIT,42h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VOUT_UV_WARN_LIMIT,43h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VOUT_UV_FAULT_LIMIT,44h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // VOUT_UV_FAULT_RESPONSE,45h
    PMB_WRITE_WORD | PMB_READ_WORD,            // IOUT_OC_FAULT_LIMIT,46h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // IOUT_OC_FAULT_RESPONSE,47h
    PMB_WRITE_WORD | PMB_READ_WORD,            // IOUT_OC_LV_FAULT_LIMIT,48h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // IOUT_OC_LV_FAULT_RESPONSE,49h
    PMB_WRITE_WORD | PMB_READ_WORD,            // IOUT_OC_WARN_LIMIT,4Ah
    PMB_WRITE_WORD | PMB_READ_WORD,            // IOUT_UC_FAULT_LIMIT,4Bh
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // IOUT_UC_FAULT_RESPONSE,4Ch
    PMB_RESERVED,                              // RESERVED,4Dh
    PMB_RESERVED,                              // RESERVED,4Eh
    PMB_WRITE_WORD | PMB_READ_WORD,            // OT_FAULT_LIMIT,4Fh
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // OT_FAULT_RESPONSE,50h
    PMB_WRITE_WORD | PMB_READ_WORD,            // OT_WARN_LIMIT,51h
    PMB_WRITE_WORD | PMB_READ_WORD,            // UT_WARN_LIMIT,52h
    PMB_WRITE_WORD | PMB_READ_WORD,            // UT_FAULT_LIMIT,53h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // UT_FAULT_RESPONSE,54h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VIN_OV_FAULT_LIMIT,55h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // VIN_OV_FAULT_RESPONSE,56h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VIN_OV_WARN_LIMIT,57h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VIN_UV_WARN_LIMIT,58h
    PMB_WRITE_WORD | PMB_READ_WORD,            // VIN_UV_FAULT_LIMIT,59h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // VIN_UV_FAULT_RESPONSE,5Ah
    PMB_WRITE_WORD | PMB_READ_WORD,            // IIN_OC_FAULT_LIMIT,5Bh
    PMB_WRITE_WORD | PMB_READ_WORD,            // IIN_OC_FAULT_RESPONSE,5Ch
    PMB_WRITE_WORD | PMB_READ_WORD,            // IIN_OC_WARN_LIMIT,5Dh
    PMB_WRITE_WORD | PMB_READ_WORD,            // POWER_GOOD_ON,5Eh
    PMB_WRITE_WORD | PMB_READ_WORD,            // POWER_GOOD_OFF,5Fh
    PMB_WRITE_WORD | PMB_READ_WORD,            // TON_DELAY,60h
    PMB_WRITE_WORD | PMB_READ_WORD,            // TON_RISE,61h
    PMB_WRITE_WORD | PMB_READ_WORD,            // TON_MAX_FAULT_LIMIT,62h
    PMB_WRITE_WORD | PMB_READ_WORD,            // TON_MAX_FAULT_RESPONSE,63h
    PMB_WRITE_WORD | PMB_READ_WORD,            // TOFF_DELAY,64h
    PMB_WRITE_WORD | PMB_READ_WORD,            // TOFF_FALL,65h
    PMB_WRITE_WORD | PMB_READ_WORD,            // TOFF_MAX_WARN_LIMIT,66h
    PMB_RESERVED,                              // RESERVED,67h
    PMB_WRITE_WORD | PMB_READ_WORD,            // POUT_OP_FAULT_LIMIT,68h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // POUT_OP_FAULT_RESPONSE,69h
    PMB_WRITE_WORD | PMB_READ_WORD,            // POUT_OP_WARN_LIMIT,6Ah
    PMB_WRITE_WORD | PMB_READ_WORD,            // PIN_OP_WARN_LIMIT,6Bh
    PMB_RESERVED,                              // RESERVED,6Ch
    PMB_RESERVED,                              // RESERVED,6Dh
    PMB_RESERVED,                              // RESERVED,6Eh
    PMB_RESERVED,                              // RESERVED,6Fh
    PMB_RESERVED,                              // RESERVED,70h
    PMB_RESERVED,                              // RESERVED,71h
    PMB_RESERVED,                              // RESERVED,72h
    PMB_RESERVED,                              // RESERVED,73h
    PMB_RESERVED,                              // RESERVED,74h
    PMB_RESERVED,                              // RESERVED,75h
    PMB_RESERVED,                              // RESERVED,76h
    PMB_RESERVED,                              // RESERVED,77h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // STATUS_BYTE,78h
    PMB_WRITE_WORD | PMB_READ_WORD,            // STATUS_WORD,79h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // STATUS_VOUT,7Ah
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // STATUS_IOUT,7Bh
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // STATUS_INPUT,7Ch
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // STATUS_TEMPERATURE,7Dh
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // STATUS_CML,7Eh
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // STATUS_OTHER,7Fh
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // STATUS_MFR_SPECIFIC,80h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // STATUS_FANS_1_2,81h
    PMB_WRITE_BYTE | PMB_READ_BYTE,            // STATUS_FANS_3_4,82h
    PMB_RESERVED,                              // RESERVED,83h
    PMB_RESERVED,                              // RESERVED,84h
    PMB_WRITE_WORD | PMB_READ_WORD,            // READ_KWH_CONFIG,85h
    PMB_NO_WRITE | PMB_BLOCK_READ,             // READ_EIN,86h
    PMB_NO_WRITE | PMB_BLOCK_READ,             // READ_EOUT,87h
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_VIN,88h
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_IIN,89h
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_VCAP,8Ah
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_VOUT,8Bh
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_IOUT,8Ch
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_TEMPERATURE_1,8Dh
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_TEMPERATURE_2,8Eh
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_TEMPERATURE_3,8Fh
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_FAN_SPEED_1,90h
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_FAN_SPEED_2,91h
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_FAN_SPEED_3,92h
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_FAN_SPEED_4,93h
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_DUTY_CYCLE,94h
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_FREQUENCY,95h
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_POUT,96h
    PMB_NO_WRITE | PMB_READ_WORD,              // READ_PIN,97h
    PMB_NO_WRITE | PMB_READ_BYTE,              // PMBUS_REVISION,98h
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // MFR_ID,99h
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // MFR_MODEL,9Ah
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // MFR_REVISION,9Bh
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // MFR_LOCATION,9Ch
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // MFR_DATE,9Dh
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // MFR_SERIAL,9Eh
    PMB_NO_WRITE | PMB_BLOCK_READ,             // APP_PROFILE_SUPPORT,9Fh
    PMB_NO_WRITE | PMB_READ_WORD,              // MFR_VIN_MIN,A0h
    PMB_NO_WRITE | PMB_READ_WORD,              // MFR_VIN_MAX,A1h
    PMB_NO_WRITE | PMB_READ_WORD,              // MFR_IIN_MAX,A2h
    PMB_NO_WRITE | PMB_READ_WORD,              // MFR_PIN_MAX,A3h
    PMB_NO_WRITE | PMB_READ_WORD,              // MFR_VOUT_MIN,A4h
    PMB_NO_WRITE | PMB_READ_WORD,              // MFR_VOUT_MAX,A5h
    PMB_NO_WRITE | PMB_READ_WORD,              // MFR_IOUT_MAX,A6h
    PMB_NO_WRITE | PMB_READ_WORD,              // MFR_POUT_MAX,A7h
    PMB_NO_WRITE | PMB_READ_WORD,              // MFR_TAMBIENT_MAX,A8h
    PMB_NO_WRITE | PMB_READ_WORD,              // MFR_TAMBIENT_MIN,A9h
    PMB_NO_WRITE | PMB_BLOCK_READ,             // MFR_EFFICIENCY_LL,AAh
    PMB_NO_WRITE | PMB_BLOCK_READ,             // MFR_EFFICIENCY_HL,ABh
    PMB_NO_WRITE | PMB_READ_BYTE,              // MFR_PIN_ACCURACY,ACh
    PMB_NO_WRITE | PMB_BLOCK_READ,             // IC_DEVICE_ID,ADh
    PMB_NO_WRITE | PMB_BLOCK_READ,             // IC_DEVICE_REV,AEh
    PMB_RESERVED,                              // RESERVED,Afh
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_00,B0h
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_01,B1h
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_02,B2h
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_03,B3h
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_04,B4h
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_05,B5h
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_06,B6h
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_07,B7h
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_08,B8h
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_09,B9h
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_10,BAh
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_11,BBh
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_12,BCh
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_13,BDh
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_14,BEh
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // USER_DATA_15,BFh
    PMB_WRITE_WORD | PMB_READ_WORD,            // MFR_MAX_TEMP_1,C0h
    PMB_WRITE_WORD | PMB_READ_WORD,            // MFR_MAX_TEMP_2,C1h
    PMB_WRITE_WORD | PMB_READ_WORD,            // MFR_MAX_TEMP_3,C2h
    PMB_RESERVED,                              // RESERVED,C3h
    PMB_RESERVED,                              // RESERVED,C4h
    PMB_RESERVED,                              // RESERVED,C5h
    PMB_RESERVED,                              // RESERVED,C6h
    PMB_RESERVED,                              // RESERVED,C7h
    PMB_RESERVED,                              // RESERVED,C8h
    PMB_RESERVED,                              // RESERVED,C9h
    PMB_RESERVED,                              // RESERVED,CAh
    PMB_RESERVED,                              // RESERVED,CBh
    PMB_RESERVED,                              // RESERVED,CCh
    PMB_RESERVED,                              // RESERVED,CDh
    PMB_RESERVED,                              // RESERVED,CEh
    PMB_RESERVED,                              // RESERVED,CFh
    PMB_WRITE_WORD | PMB_READ_WORD,            // MFR_defined,D0h
    PMB_BLOCK_WRITE | PMB_BLOCK_READ,          // MFR_defined,D1h
    PMB_RESERVED,                              // RESERVED,D2h
    PMB_RESERVED,                              // RESERVED,D3h
    PMB_RESERVED,                              // RESERVED,D4h
    PMB_RESERVED,                              // RESERVED,D5h
    PMB_RESERVED,                              // RESERVED,D6h
    PMB_RESERVED,                              // RESERVED,D7h
    PMB_RESERVED,                              // RESERVED,D8h
    PMB_RESERVED,                              // RESERVED,D9h
    PMB_RESERVED,                              // RESERVED,DAh
    PMB_RESERVED,                              // RESERVED,DBh
    PMB_RESERVED,                              // RESERVED,DCh
    PMB_RESERVED,                              // RESERVED,DDh
    PMB_RESERVED,                              // RESERVED,DEh
    PMB_RESERVED,                              // RESERVED,DFh
    PMB_RESERVED,                              // RESERVED,E0h
    PMB_RESERVED,                              // RESERVED,E1h
    PMB_RESERVED,                              // RESERVED,E2h
    PMB_RESERVED,                              // RESERVED,E3h
    PMB_RESERVED,                              // RESERVED,E4h
    PMB_RESERVED,                              // RESERVED,E5h
    PMB_RESERVED,                              // RESERVED,E6h
    PMB_RESERVED,                              // RESERVED,E7h
    PMB_RESERVED,                              // RESERVED,E8h
    PMB_RESERVED,                              // RESERVED,E9h
    PMB_RESERVED,                              // RESERVED,EAh
    PMB_RESERVED,                              // RESERVED,EBh
    PMB_RESERVED,                              // RESERVED,ECh
    PMB_RESERVED,                              // RESERVED,EDh
    PMB_RESERVED,                              // RESERVED,EEh
    PMB_RESERVED,                              // RESERVED,EFh
    PMB_RESERVED,                              // RESERVED,F0h
    PMB_RESERVED,                              // RESERVED,F1h
    PMB_RESERVED,                              // RESERVED,F2h
    PMB_RESERVED,                              // RESERVED,F3h
    PMB_RESERVED,                              // RESERVED,F4h
    PMB_RESERVED,                              // RESERVED,F5h
    PMB_RESERVED,                              // RESERVED,F6h
    PMB_RESERVED,                              // RESERVED,F7h
    PMB_RESERVED,                              // RESERVED,F8h
    PMB_RESERVED,                              // RESERVED,F9h
    PMB_RESERVED,                              // RESERVED,FAh
    PMB_RESERVED,                              // RESERVED,FBh
    PMB_RESERVED,                              // RESERVED,FCh
    PMB_NO_WRITE | PMB_NO_READ,                // RESERVED,FDh
    PMB_EXTENDED_CMD,                          // RESERVED,FEh
    PMB_EXTENDED_CMD,                          // RESERVED,FFh
};

/*! SMB Reception Buffer      */
static uint8_t au8RxBuff[SMB_MAX_PACKET_SIZE];
/*! SMB Transmission Buffer   */
static uint8_t au8TxBuff[SMB_MAX_PACKET_SIZE];

void PMBus_controllerInit(SMBus *SMB, I2C_Regs* i2cAddr, uint32_t busClk)
{
    /* Initialize SMBus as Controller   */
    SMBus_controllerInit(SMB,i2cAddr, busClk);
    /* Enable SMBus Interrupts, after initializing I2C  */
    SMBus_controllerEnableInt(SMB);
}

void PMBus_targetInit(SMBus *SMB, I2C_Regs* i2cAddr, uint8_t targetAddr)
{
    /* Initialize SMBus as target   */
    SMBus_targetInit(SMB,i2cAddr);
    /* Set the own target address   */
    SMBus_targetSetAddress(SMB, targetAddr);

    SMBus_targetSetRxBuffer(SMB, au8RxBuff, sizeof(au8RxBuff));
    SMBus_targetSetTxBuffer(SMB, au8TxBuff, sizeof(au8TxBuff));

    /* Enable SMBus Interrupts, after initializing I2C  */
    SMBus_targetEnableInt(SMB);
}

void PMBus_controllerProcessInt(SMBus *SMB)
{
    /*
     * Process SMBus ISR as Controller, store result flags
     * Note that the Controller starts transfers in Main loop and this function
     * handles the SMBus State machine. The main loop just waits for completion
     */
    SMBus_State sSMBState = SMBus_controllerProcessInt(SMB);
}

SMBus_State PMBus_targetProcessInt(SMBus *SMB)
{
    return SMBus_targetProcessInt(SMB);
}

void PMBus_enablePEC(SMBus *SMB)
{
    SMBus_enablePEC(SMB);
}

void PMBus_disablePEC(SMBus *SMB)
{
    SMBus_disablePEC(SMB);
}

uint16_t PMBus_getTargetCommand(SMBus *SMB)
{
    return (uint16_t)SMBus_targetGetCommand(SMB);
}

uint16_t PMBus_getTargetExtendedCommand(SMBus *SMB)
{
    uint16_t cmd = (uint16_t)(((uint16_t)SMB->nwk.rxBuffPtr[0]) | (((uint16_t)SMB->nwk.rxBuffPtr[1]) << 8));
    return cmd;
}

void PMBus_processDone(SMBus *SMB)
{
    SMBus_processDone(SMB);
}

void PMBus_targetReportBlock(SMBus *SMB)
{
    SMBus_targetReportBlock(SMB);
}

void PMBus_targetReportLength(SMBus *SMB, uint16_t length)
{
    SMBus_targetReportLength(SMB, length);
}

int8_t PMBus_cmdWrite(SMBus *SMB,
                      uint8_t targetAddress,
                      uint8_t commandId,
                      uint8_t* txData,
                      uint8_t txSize,
                      PMBUS_GRP_CMD groupCmd,
                      uint32_t timeout)
{
    // Make sure commandId is in range
    if((uint16_t)commandId >= sizeof(PMBus_Commands) / sizeof(PMBus_Commands[0]))
    {
        return(PMBUS_RET_ERROR);
    }
    uint8_t transaction = (PMBus_Commands[commandId] & 0xF0);

    int8_t ret = PMBUS_RET_ERROR;
    switch(transaction)
    {
    case PMB_NO_WRITE:
        if(txSize != 0)
        {
            return (PMBUS_RET_ERROR);
        }
        ret = PMBus_QuickCommand(SMB,
                                 targetAddress,
                                 true,
                                 groupCmd,
                                 timeout);
        break;
    case PMB_SEND_BYTE:
        if(txSize != 0)
        {
            return (PMBUS_RET_ERROR);
        }
        ret = PMBus_cmdSendByte(SMB,
                                targetAddress,
                                commandId,
                                groupCmd,
                                timeout);
        break;
    case PMB_WRITE_BYTE:
        if(txSize != 1)
        {
            return (PMBUS_RET_ERROR);
        }
        ret = PMBus_cmdWriteByte(SMB,
                                 targetAddress,
                                 commandId,
                                 txData,
                                 txSize,
                                 groupCmd,
                                 timeout);
        break;
    case PMB_WRITE_WORD:
        if(txSize != 2)
        {
            return (PMBUS_RET_ERROR);
        }
        ret = PMBus_cmdWriteWord(SMB,
                                 targetAddress,
                                 commandId,
                                 txData,
                                 txSize,
                                 groupCmd,
                                 timeout);
        break;
    case PMB_BLOCK_WRITE:
        ret = PMBus_cmdBlockWrite(SMB,
                                  targetAddress,
                                  commandId,
                                  txData,
                                  txSize,
                                  groupCmd,
                                  timeout);
        break;
    case PMB_RESERVED:
    default:
        ret = PMBUS_RET_ERROR;
    }
    return(ret);
}

int8_t PMBus_cmdRead(SMBus *SMB,
                     uint8_t targetAddress,
                     uint8_t commandId,
                     uint8_t* rxData,
                     uint8_t* rxSize,
                     uint32_t timeout)
{
    /* Make sure commandId is in range  */
    if((uint16_t)commandId >= sizeof(PMBus_Commands) / sizeof(PMBus_Commands[0]))
    {
        return(PMBUS_RET_ERROR);
    }

    int8_t ret = PMBUS_RET_ERROR;
    uint8_t transaction = (PMBus_Commands[commandId] & 0x0F);

    switch(transaction)
    {
    case PMB_NO_READ:
        ret = PMBUS_RET_OK;
        break;
    case PMB_READ_BYTE:
        ret = PMBus_cmdReadByte(SMB,
                                targetAddress,
                                commandId,
                                rxData,
                                timeout);
        *rxSize = 1;
        break;
    case PMB_READ_WORD:
        ret = PMBus_cmdReadWord(SMB,
                                targetAddress,
                                commandId,
                                rxData,
                                timeout);
        *rxSize = 2;
        break;
    case PMB_BLOCK_READ:
        ret = PMBus_cmdBlockRead(SMB,
                                 targetAddress,
                                 commandId,
                                 rxData,
                                 rxSize,
                                 timeout);
        break;
    case PMB_RESERVED:
    default:
        ret = PMBUS_RET_ERROR;
    }
    return(ret);
}

int8_t PMBus_QuickCommand(SMBus *SMB,
                          uint8_t targetAddress,
                          bool write,
                          PMBUS_GRP_CMD groupCmd,
                          uint32_t timeout)
{
    if(groupCmd == PMBUS_GRP_CMD_DISABLE)
    {
        if((SMB)->nwk.eState != SMBus_NwkState_Idle)
        {
            return(PMBUS_RET_ERROR);
        }
    }

    if(write != true)
    {
        return(PMBUS_RET_ERROR);
    }

    (SMB)->nwk.rxLen = 0;  //No read
    (SMB)->nwk.txLen = 0; // No Command
    (SMB)->nwk.eState = SMBus_NwkState_TXQuickCMD;
    (SMB)->nwk.currentAddr = targetAddress;

    if(write == true)
    {
        PMBus_startTxTransfer(SMB, groupCmd);
    }

    if (groupCmd == PMBUS_GRP_CMD_ENABLE)
    {
        DL_I2C_disableInterrupt(SMB->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE);
        while (!DL_I2C_getRawInterruptStatus(SMB->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE))
        {

        }
        DL_I2C_clearInterruptStatus(SMB->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE);
    }
    else
    {
        if((SMBus_controllerWaitUntilDone(SMB,timeout) != PMBUS_RET_OK) ||
           (SMBus_getState(SMB) != SMBus_State_OK))
        {
            SMBus_controllerReset(SMB); // Force a reset, so we can retry later
            return(PMBUS_RET_ERROR);
        }
    }

    return(PMBUS_RET_OK);
}

int8_t PMBus_cmdSendByte(SMBus *SMB,
                         uint8_t targetAddress,
                         uint8_t commandByte,
                         PMBUS_GRP_CMD groupCmd,
                         uint32_t timeout)
{
    if(groupCmd == PMBUS_GRP_CMD_DISABLE)
    {
        if((SMB)->nwk.eState != SMBus_NwkState_Idle)
        {
            return(PMBUS_RET_ERROR);
        }
    }

    (SMB)->nwk.currentCmd = commandByte;
    (SMB)->nwk.rxLen = 0;  //No read
    (SMB)->nwk.txLen = 1; // Only Command(Data)
    (SMB)->nwk.eState = SMBus_NwkState_TX;
    (SMB)->nwk.currentAddr = targetAddress;

    PMBus_startTxTransfer(SMB, groupCmd);

    if (groupCmd == PMBUS_GRP_CMD_ENABLE)
    {
        DL_I2C_disableInterrupt(SMB->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE);
        while (!DL_I2C_getRawInterruptStatus(SMB->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE))
        {

        }
        DL_I2C_clearInterruptStatus(SMB->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE);
    }
    else
    {
        if((SMBus_controllerWaitUntilDone(SMB,timeout) != PMBUS_RET_OK) ||
           (SMBus_getState(SMB) != SMBus_State_OK))
        {
            SMBus_controllerReset(SMB); // Force a reset, so we can retry later
            return(PMBUS_RET_ERROR);
        }
    }

    return(PMBUS_RET_OK);
}

int8_t PMBus_cmdWriteByte(SMBus *SMB,
                          uint8_t targetAddress,
                          uint8_t commandByte,
                          uint8_t *txData,
                          uint8_t txSize,
                          PMBUS_GRP_CMD groupCmd,
                          uint32_t timeout)
{
    if(groupCmd == PMBUS_GRP_CMD_DISABLE)
    {
        if((SMB)->nwk.eState != SMBus_NwkState_Idle)
        {
            return(PMBUS_RET_ERROR);
        }
    }
    if((txSize != 1))
    {
        return(PMBUS_RET_ERROR);
    }

    (SMB)->nwk.currentCmd = commandByte;
    (SMB)->nwk.rxLen = 0; // Read 1 or 2 bytes
    (SMB)->nwk.txLen = 1 + txSize; // Command + Size
    (SMB)->nwk.txBuffPtr = txData;
    (SMB)->nwk.eState = SMBus_NwkState_TX;
    (SMB)->nwk.currentAddr = targetAddress;

    PMBus_startTxTransfer(SMB, groupCmd);

    if (groupCmd == PMBUS_GRP_CMD_ENABLE)
    {
        DL_I2C_disableInterrupt(SMB->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE);
        while (!DL_I2C_getRawInterruptStatus(SMB->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE))
        {

        }
        DL_I2C_clearInterruptStatus(SMB->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE);
    }
    else
    {
        if((SMBus_controllerWaitUntilDone(SMB,timeout) != PMBUS_RET_OK) ||
           (SMBus_getState(SMB) != SMBus_State_OK))
        {
            SMBus_controllerReset(SMB); // Force a reset, so we can retry later
            return(PMBUS_RET_ERROR);
        }
    }

    return(PMBUS_RET_OK);
}

int8_t PMBus_cmdWriteWord(SMBus *SMB,
                          uint8_t targetAddress,
                          uint8_t commandByte,
                          uint8_t *txData,
                          uint8_t txSize,
                          PMBUS_GRP_CMD groupCmd,
                          uint32_t timeout)
{
    if(groupCmd == PMBUS_GRP_CMD_DISABLE)
    {
        if((SMB)->nwk.eState != SMBus_NwkState_Idle)
        {
            return(PMBUS_RET_ERROR);
        }
    }

    if((txSize != 2))
    {
        return(PMBUS_RET_ERROR);
    }

    (SMB)->nwk.currentCmd = commandByte;
    (SMB)->nwk.rxLen = 0; // Read 1 or 2 bytes
    (SMB)->nwk.txLen = 1 + txSize; // Command + Size
    (SMB)->nwk.txBuffPtr = txData;
    (SMB)->nwk.eState = SMBus_NwkState_TX;
    (SMB)->nwk.currentAddr = targetAddress;

    PMBus_startTxTransfer(SMB, groupCmd);

    if (groupCmd == PMBUS_GRP_CMD_ENABLE)
    {
        DL_I2C_disableInterrupt(SMB->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE);
        while (!DL_I2C_getRawInterruptStatus(SMB->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE))
        {

        }
        DL_I2C_clearInterruptStatus(SMB->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE);
    }
    else
    {
        if((SMBus_controllerWaitUntilDone(SMB,timeout) != PMBUS_RET_OK) ||
           (SMBus_getState(SMB) != SMBus_State_OK))
        {
            SMBus_controllerReset(SMB); // Force a reset, so we can retry later
            return(PMBUS_RET_ERROR);
        }
    }
    return(PMBUS_RET_OK);
}

int8_t PMBus_cmdBlockWrite(SMBus *SMB,
                           uint8_t targetAddress,
                           uint8_t commandByte,
                           uint8_t *txData,
                           uint16_t txSize,
                           PMBUS_GRP_CMD groupCmd,
                           uint32_t timeout)
{
    if(groupCmd == PMBUS_GRP_CMD_DISABLE)
    {
        if((SMB)->nwk.eState != SMBus_NwkState_Idle)
        {
            return(PMBUS_RET_ERROR);
        }
    }

    if((txSize == 0) || (txSize > SMB_MAX_PAYLOAD_SIZE))
    {
        return(PMBUS_RET_ERROR);
    }

    (SMB)->nwk.currentCmd = commandByte;
    (SMB)->nwk.rxLen = 0;
    (SMB)->nwk.txLen = txSize + 2; // Size includes Command and byte count
    (SMB)->nwk.txBuffPtr = txData;
    (SMB)->nwk.eState = SMBus_NwkState_TX_Block;
    (SMB)->nwk.currentAddr = targetAddress;

    PMBus_startTxTransfer(SMB, groupCmd);

    if (groupCmd == PMBUS_GRP_CMD_ENABLE)
    {
        DL_I2C_disableInterrupt(SMB->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE);
        while (!DL_I2C_getRawInterruptStatus(SMB->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE))
        {

        }
        DL_I2C_clearInterruptStatus(SMB->phy.SMBus_Phy_i2cBase, DL_I2C_INTERRUPT_CONTROLLER_TX_DONE);
    }
    else
    {
        if((SMBus_controllerWaitUntilDone(SMB,timeout) != PMBUS_RET_OK) ||
           (SMBus_getState(SMB) != SMBus_State_OK))
        {
            SMBus_controllerReset(SMB); // Force a reset, so we can retry later
            return(PMBUS_RET_ERROR);
        }
    }

    return(PMBUS_RET_OK);
}

int8_t PMBus_cmdReceiveByte(SMBus *SMB,
                            uint8_t targetAddress,
                            uint8_t* rxData,
                            uint32_t timeout)
{
    if(SMBus_controllerReceiveByte(SMB,                      // SMB struct
                               targetAddress,              // Target Addr
                               rxData) != PMBUS_RET_OK)   // DataPtr
    {
        return(PMBUS_RET_ERROR);
    }

    if((SMBus_controllerWaitUntilDone(SMB,timeout) != PMBUS_RET_OK) ||
       (SMBus_getState(SMB) != SMBus_State_OK))
    {
        SMBus_controllerReset(SMB); // Force a reset, so we can retry later
        return(PMBUS_RET_ERROR);
    }

    // Get the length of payload for response
    if(SMBus_getRxPayloadAvailable(SMB) != 1)
    {
        return(PMBUS_RET_ERROR);  // RX Data size Error
    }

    return(PMBUS_RET_OK);
}

int8_t PMBus_cmdReadByte(SMBus *SMB,
                         uint8_t targetAddress,
                         uint8_t commandByte,
                         uint8_t* rxData,
                         uint32_t timeout)
{
    if(SMBus_controllerReadByteWord(SMB,                      // SMB struct
                                targetAddress,              // Target Addr
                                commandByte,               // PMB Command
                                rxData,                    // DataPtr
                                1) != PMBUS_RET_OK)        // 1 byte data
    {
        return(PMBUS_RET_ERROR);
    }

    if((SMBus_controllerWaitUntilDone(SMB,timeout) != PMBUS_RET_OK) ||
       (SMBus_getState(SMB) != SMBus_State_OK))
    {
        SMBus_controllerReset(SMB); // Force a reset, so we can retry later
        return(PMBUS_RET_ERROR);
    }

    // Get the length of payload for response
    if(SMBus_getRxPayloadAvailable(SMB) != 1)
    {
        return(PMBUS_RET_ERROR);  // RX Data size Error
    }

    return(PMBUS_RET_OK);
}

int8_t PMBus_cmdReadWord(SMBus *SMB,
                         uint8_t targetAddress,
                         uint8_t commandByte,
                         uint8_t* rxData,
                         uint32_t timeout)
{
    if(SMBus_controllerReadByteWord(SMB,                      // SMB struct
                                targetAddress,              // Target Addr
                                commandByte,               // PMB Command
                                rxData,                    // DataPtr
                                2) != PMBUS_RET_OK)        // 2 byte data
    {
        return(PMBUS_RET_ERROR);
    }

    if((SMBus_controllerWaitUntilDone(SMB,timeout) != PMBUS_RET_OK) ||
       (SMBus_getState(SMB) != SMBus_State_OK))
    {
        SMBus_controllerReset(SMB); // Force a reset, so we can retry later
        return(PMBUS_RET_ERROR);
    }

    // Get the length of payload for response
    if(SMBus_getRxPayloadAvailable(SMB) != 2)
    {
        return(PMBUS_RET_ERROR); // RX Data size Error
    }

    return(PMBUS_RET_OK);
}

int8_t PMBus_cmdBlockRead(SMBus *SMB,
                          uint8_t targetAddress,
                          uint8_t commandByte,
                          uint8_t* rxData,
                          uint8_t* rxSize,
                          uint32_t timeout)
{
    if(SMBus_controllerReadBlock(SMB,                            // SMB struct
                             targetAddress,                    // Target Addr
                             commandByte,                     // PMB Command
                             rxData) != PMBUS_RET_OK)         // DataPtr
    {
        return(PMBUS_RET_ERROR);
    }

    if((SMBus_controllerWaitUntilDone(SMB,timeout) != PMBUS_RET_OK) ||
       (SMBus_getState(SMB) != SMBus_State_OK))
    {
        SMBus_controllerReset(SMB); // Force a reset, so we can retry later
        return(PMBUS_RET_ERROR);
    }

    // Return the number of bytes received
    *rxSize = SMBus_getRxPayloadAvailable(SMB);

    return(PMBUS_RET_OK);
}

int8_t PMBus_cmdProcessCall(SMBus *SMB,
                            uint8_t targetAddress,
                            uint8_t commandByte,
                            uint8_t* txData,
                            uint8_t* rxData,
                            uint32_t timeout)
{
    if(SMBus_controllerProcessCall(SMB,                      // SMB struct
                               targetAddress,              // Target Addr
                               commandByte,               // PMB Command
                               txData,                    // Tx data
                               rxData) != PMBUS_RET_OK)   // DataPtr
    {
        return(PMBUS_RET_ERROR);
    }

    if((SMBus_controllerWaitUntilDone(SMB,timeout) != PMBUS_RET_OK) ||
       (SMBus_getState(SMB) != SMBus_State_OK))
    {
        SMBus_controllerReset(SMB); // Force a reset, so we can retry later
        return(PMBUS_RET_ERROR);
    }

    // Get the length of payload for response
    if(SMBus_getRxPayloadAvailable(SMB) != 2)
    {
        return(PMBUS_RET_ERROR); // RX Data size Error
    }

    return(PMBUS_RET_OK);
}

int8_t PMBus_cmdBlockWriteBlockReadProcessCall(SMBus *SMB,
                                               uint8_t targetAddress,
                                               uint8_t commandByte,
                                               uint8_t* txData,
                                               uint8_t txSize,
                                               uint8_t* rxData,
                                               uint8_t* rxSize,
                                               uint32_t timeout)
{
    if(SMBus_controllerProcessCallBlock(SMB,                         // SMB struct
                                    targetAddress,                 // Target Addr
                                    commandByte,                  // PMB Command
                                    txData,                       // Tx data
                                    txSize,                       // Tx data size
                                    rxData) != PMBUS_RET_OK)      // DataPtr
    {
        return(PMBUS_RET_ERROR);
    }

    if((SMBus_controllerWaitUntilDone(SMB,timeout) != PMBUS_RET_OK) ||
       (SMBus_getState(SMB) != SMBus_State_OK))
    {
        SMBus_controllerReset(SMB); // Force a reset, so we can retry later
        return(PMBUS_RET_ERROR);
    }

    // Return the number of bytes received
    *rxSize = SMBus_getRxPayloadAvailable(SMB);

    return(PMBUS_RET_OK);
}

int8_t PMBus_cmdGroupCommand(SMBus *SMB,
                             uint8_t *targetAddr,
                             uint8_t *command,
                             uint8_t **txData,
                             uint8_t *txSize,
                             uint8_t length,
                             uint32_t timeout)
{
    if((SMB)->nwk.eState != SMBus_NwkState_Idle)
    {
        return(PMBUS_RET_ERROR);
    }

    uint8_t ret;

    for(int i=0;i<length-1;i++)
    {
        ret = PMBus_cmdWrite(SMB,
                             targetAddr[i],
                             command[i],
                             txData[i],
                             txSize[i],
                             PMBUS_GRP_CMD_ENABLE,
                             timeout);
        if (ret == (uint8_t) PMBUS_RET_ERROR)
        {
            return (PMBUS_RET_ERROR);
        }
    }
    ret = PMBus_cmdWrite(SMB,
                         targetAddr[length-1],
                         command[length-1],
                         txData[length-1],
                         txSize[length-1],
                         PMBUS_GRP_CMD_LAST_TARGET,
                         timeout);
    if (ret == (uint8_t) PMBUS_RET_ERROR)
    {
        return (PMBUS_RET_ERROR);
    }
    return (ret);
}

void PMBus_startTxTransfer(SMBus *SMB, PMBUS_GRP_CMD groupCmd)
{
    uint8_t targetAddr = (SMB)->nwk.currentAddr;
    uint16_t length = (SMB)->nwk.txLen;
    SMBus_Stop stop;
    SMBus_Auto_Ack ack;
    uint8_t setPEC;

    // Reset state
    (SMB)->state = SMBus_State_OK;
    (SMB)->nwk.rxIndex = 0;
    (SMB)->nwk.txIndex = 0;


    // TX only
    if ((SMB)->nwk.rxLen == 0) {
        stop = SMBus_Stop_After_Transfer; // Nothing after TX only, so send a stop
        ack = SMBus_Auto_Ack_Last_Byte; // ack is a don't care because the target will perform the ack

        if (((SMB)->nwk.eState != SMBus_NwkState_TXQuickCMD) && (((SMB)->nwk.eState != SMBus_NwkState_TXHostAlert)))
        {
          if ((SMB)->ctrl.bits.pecEn == 1) {
              // Add 1 to account for pec after payload
              length += 1;
          }
        }
        if ((SMB)->nwk.eState != SMBus_NwkState_TXHostAlert){
            setPEC = 1;
        }
        else
        {
            setPEC = 0;
        }
    }
    // TX packet followed by RX
    else
    {
        stop = SMBus_No_Stop_After_Transfer; // Repeated start so NO stop between TX and RX
        ack = SMBus_Auto_Ack_Last_Byte; // ack is a don't care because the target will perform the ack
        setPEC = 0;
    }

    if (groupCmd == PMBUS_GRP_CMD_ENABLE)
    {
        stop = SMBus_No_Stop_After_Transfer;
    }

    SMBus_PHY_controllerStartTx(SMB, targetAddr, length, stop, setPEC);
}

int8_t PMBus_cmdExtendedWriteByte(SMBus *SMB,
                                  uint8_t targetAddress,
                                  uint8_t commandByte1,
                                  uint8_t commandByte2,
                                  uint8_t *txData,
                                  uint8_t txSize,
                                  uint32_t timeout)
{
    if((SMB)->nwk.eState != SMBus_NwkState_Idle)
    {
        return(PMBUS_RET_ERROR);
    }

    if((txSize != 1))
    {
        return(PMBUS_RET_ERROR);
    }

    txData[1] = txData[0];
    txData[0] = commandByte2;

    (SMB)->nwk.currentCmd = commandByte1;
    (SMB)->nwk.rxLen = 0;
    (SMB)->nwk.txLen = 2 + txSize; // 2 Commands + Size
    (SMB)->nwk.txBuffPtr = txData;
    (SMB)->nwk.eState = SMBus_NwkState_TX;
    (SMB)->nwk.currentAddr = targetAddress;

    SMBus_NWK_startTxTransfer(SMB);

    if((SMBus_controllerWaitUntilDone(SMB,timeout) != PMBUS_RET_OK) ||
       (SMBus_getState(SMB) != SMBus_State_OK))
    {
        SMBus_controllerReset(SMB); // Force a reset, so we can retry later
        return(PMBUS_RET_ERROR);
    }

    return(PMBUS_RET_OK);
}

int8_t PMBus_cmdExtendedWriteWord(SMBus *SMB,
                                  uint8_t targetAddress,
                                  uint8_t commandByte1,
                                  uint8_t commandByte2,
                                  uint8_t *txData,
                                  uint8_t txSize,
                                  uint32_t timeout)
{
    if((SMB)->nwk.eState != SMBus_NwkState_Idle)
    {
        return(PMBUS_RET_ERROR);
    }

    if((txSize != 2))
    {
        return(PMBUS_RET_ERROR);
    }

    txData[2] = txData[1];
    txData[1] = txData[0];
    txData[0] = commandByte2;

    (SMB)->nwk.currentCmd = commandByte1;
    (SMB)->nwk.rxLen = 0;
    (SMB)->nwk.txLen = 2 + txSize; // 2 Commands + Size
    (SMB)->nwk.txBuffPtr = txData;
    (SMB)->nwk.eState = SMBus_NwkState_TX;
    (SMB)->nwk.currentAddr = targetAddress;

    SMBus_NWK_startTxTransfer(SMB);

    if((SMBus_controllerWaitUntilDone(SMB,timeout) != PMBUS_RET_OK) ||
       (SMBus_getState(SMB) != SMBus_State_OK))
    {
        SMBus_controllerReset(SMB); // Force a reset, so we can retry later
        return(PMBUS_RET_ERROR);
    }

    return(PMBUS_RET_OK);
}

int8_t PMBus_cmdExtendedReadByte(SMBus *SMB,
                                 uint8_t targetAddress,
                                 uint8_t commandByte1,
                                 uint8_t commandByte2,
                                 uint8_t* rxData,
                                 uint8_t rxSize,
                                 uint32_t timeout)
{
    if((SMB)->nwk.eState != SMBus_NwkState_Idle)
    {
        return(PMBUS_RET_ERROR);
    }

    if((rxSize != 1))
    {
        return(PMBUS_RET_ERROR);
    }

    uint8_t *txData = (uint8_t *)malloc(sizeof(uint8_t)*1);
    txData[0] = commandByte2;

    (SMB)->nwk.currentCmd = commandByte1;
    (SMB)->nwk.rxLen = rxSize; // Read 1 byte
    (SMB)->nwk.rxBuffPtr = rxData;
    (SMB)->nwk.txLen = 2; // Only send 2 command
    (SMB)->nwk.txBuffPtr = txData;
    (SMB)->nwk.eState = SMBus_NwkState_TX;
    (SMB)->nwk.currentAddr = targetAddress;

    SMBus_NWK_startTxTransfer(SMB);

    if((SMBus_controllerWaitUntilDone(SMB,timeout) != PMBUS_RET_OK) ||
       (SMBus_getState(SMB) != SMBus_State_OK))
    {
        SMBus_controllerReset(SMB); // Force a reset, so we can retry later
        return(PMBUS_RET_ERROR);
    }

    // Get the length of payload for response
    if(SMBus_getRxPayloadAvailable(SMB) != 1)
    {
        return(PMBUS_RET_ERROR);  // RX Data size Error
    }

    return(PMBUS_RET_OK);
}

int8_t PMBus_cmdExtendedReadWord(SMBus *SMB,
                                 uint8_t targetAddress,
                                 uint8_t commandByte1,
                                 uint8_t commandByte2,
                                 uint8_t* rxData,
                                 uint8_t rxSize,
                                 uint32_t timeout)
{
    if((SMB)->nwk.eState != SMBus_NwkState_Idle)
    {
        return(PMBUS_RET_ERROR);
    }

    if((rxSize != 2))
    {
        return(PMBUS_RET_ERROR);
    }

    uint8_t *txData = (uint8_t *)malloc(sizeof(uint8_t)*1);
    txData[0] = commandByte2;

    (SMB)->nwk.currentCmd = commandByte1;
    (SMB)->nwk.rxLen = rxSize; // Read 2 bytes
    (SMB)->nwk.rxBuffPtr = rxData;
    (SMB)->nwk.txLen = 2; // Only send 2 commands
    (SMB)->nwk.txBuffPtr = txData;
    (SMB)->nwk.eState = SMBus_NwkState_TX;
    (SMB)->nwk.currentAddr = targetAddress;

    SMBus_NWK_startTxTransfer(SMB);

    if((SMBus_controllerWaitUntilDone(SMB,timeout) != PMBUS_RET_OK) ||
       (SMBus_getState(SMB) != SMBus_State_OK))
    {
        SMBus_controllerReset(SMB); // Force a reset, so we can retry later
        return(PMBUS_RET_ERROR);
    }

    // Get the length of payload for response
    if(SMBus_getRxPayloadAvailable(SMB) != 2)
    {
        return(PMBUS_RET_ERROR); // RX Data size Error
    }

    return(PMBUS_RET_OK);
}

int8_t PMBus_ReceiveByteARA(SMBus *SMB, uint8_t *rxData, uint32_t timeout)
{
    return(PMBus_cmdReceiveByte(SMB, PMBUS_ARA, rxData, timeout));
}

