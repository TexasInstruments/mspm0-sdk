/* --COPYRIGHT--,BSD
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
 * --/COPYRIGHT--*/

#include "pmbus_cmd_list.h"
#include "pmbus_cmd_handlers.h"

const PMBusCommand PMBus_cmdList[] =
{
 {PMB_CLEAR_FAULTS, PMBUS_SEND_BYTE_CMD_LENGTH_BYTES, PMBus_clearFaults_hdlr},
 {PMB_VOUT_MODE, PMBUS_RW_BYTE_CMD_LENGTH_BYTES, PMBus_voutMode_hdlr},
 {PMB_VOUT_OV_WARN_LIMIT, PMBUS_RW_WORD_CMD_LENGTH_BYTES, PMBus_voutOVWarnLimit_hdlr},
 {PMB_READ_VOUT, PMBUS_RW_WORD_CMD_LENGTH_BYTES, PMBus_readVout_hdlr},
 {PMB_MFR_ID, PMBUS_RW_BLOCK_CMD_LENGTH_BYTES, PMBus_MFRID_hdlr},
 {PMB_IC_DEVICE_ID, PMBUS_RW_BLOCK_CMD_LENGTH_BYTES, PMBus_deviceId_hdlr},
 {PMB_PROCESS_CALL_CMD, PMBUS_RW_WORD_CMD_LENGTH_BYTES, PMBus_processCall_hdlr},
 {PMB_BLOCK_PROCESS_CALL_CMD, PMBUS_RW_BLOCK_CMD_LENGTH_BYTES, PMBus_blockProcessCall_hdlr},
 {PMB_EXTENDED_BYTE_COMMAND, PMBUS_RW_BYTE_CMD_LENGTH_BYTES, PMBus_extByteCmd_hdlr},
 {PMB_EXTENDED_WORD_COMMAND, PMBUS_RW_WORD_CMD_LENGTH_BYTES, PMBus_extWordCmd_hdlr},
};







