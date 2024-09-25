/*
 * Copyright (c) 2023-2024, Texas Instruments Incorporated
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

/*
 *  ======== cmdinterface.h ========
 */

#ifndef __CMDINTERFACE_H__
#define __CMDINTERFACE_H__

/*! Address of the I2C Target device */
#define TGT_ADDRESS 0x48

/*! Command: GET_STATUS command from controller.
 * Bytes written (controller to target)
 * - command[0] = Command ID
 * Bytes read (target to controller):
 * - response[0]: Status byte
 */
#define CMD_CTL_GET_STATUS 0x01

/*! Command: SET_STATUS command from Controller.
 * Bytes written (controller to target):
 * - command[0] = Command ID
 * - command[1] = Status value to be updated
 * Bytes read (target to controller):
 * - None
 */
#define CMD_CTL_SET_STATUS 0x02

/*! Command: READ_BLOCK command from Controller.
 * Bytes written (controller to target):
 * command[0] = Command ID
 * command[1] = Start offset of bytes to be read from mirrorRegister ("address)
 * command[2] = numberOfBytes to be read (MSB)
 * command[3] = numberOfBytes to be read (LSB)
 * Bytes read (target to controller):
 * - response[0]: Data Byte
 * - ...
 * - response[numberOfBytes-1]: Data Byte
 */
#define CMD_CTL_READ_BLOCK 0x03

/*! Command: WRITE_BLOCK command from Controller.
 * Bytes written (controller to target):
 * command[0] = Command ID
 * command[1] = start offset of bytes to be written to mirrorRegister
 * command[2] = numberOfBytes to be written (MSB)
 * command[3] = numberOfBytes to be written (LSB)
 * command[4-] = Data
 * Bytes read (target to controller):
 * - None
 */
#define CMD_CTL_WRITE_BLOCK 0x04

#endif  // #ifndef __CMDINTERFACE_H__
