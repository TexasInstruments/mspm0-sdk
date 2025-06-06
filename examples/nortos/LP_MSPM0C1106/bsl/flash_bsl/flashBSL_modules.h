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
#ifndef flashBSL_modules_h
#define flashBSL_modules_h

#define ENABLE 1
#define DISABLE 0

/*
 * This file provides user the feasibility to enable/disable
 * certain features, saving flash size for application.
 * */

/*
 * Interfaces available for BSL Communication
 * Can select only one Interface
 * */
#define UART_INTERFACE ENABLE
#define I2C_INTERFACE DISABLE

/*
 * Select invocation mechanisms for Flash BSL
 * */
/* When invocation pin in user configuration is high, Flash BSL is invoked*/
#define BSL_PIN_INVOKE ENABLE
/* When Main Application address content is blank Flash BSL is invoked*/
#define BSL_BLANK_FLASH_INVOKE ENABLE

/*
 * BSL Commands available
 * (Refer BSL User guide for more details)
 * */
#define BSL_CMD_FLASH_RANGE_ERASE ENABLE
#define BSL_CMD_MASS_ERASE ENABLE
#define BSL_CMD_MEMORY_READ_BACK ENABLE
#define BSL_CMD_STANDALONE_VERIFICATION ENABLE
#define BSL_CMD_CHANGE_BAUDRATE ENABLE
#define BSL_CMD_GET_IDENTITY ENABLE

/*
 * Enabling TIMEOUT will help save power by locking BSL, enters low power mode
 *  after 10 seconds of no activity
 *  */
#define BSL_TIMEOUT ENABLE
#endif
