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

#ifndef __SMB_DEMO_H__
#define __SMB_DEMO_H__

//
// Include files
//
#include <stdint.h>
#include <ti/smbus/smbus.h>

//
// Macros and definitions
//
/*! Software timeout used to wait for SMBus completion */
#define DEMO_TIMEOUT                100000

/*! Response to PC if no error is found */
#define DEMO_NO_ERROR               0x00
/*! Response to PC if no error and data is available */
#define DEMO_NO_ERROR_DATA_AVAIL    0x50
/*! Response to PC of timeout is detected while waiting for completion */
#define DEMO_TIMEOUT_ERROR          0x20
/*! Response if an Comm interface error is detected */
#define DEMO_INTERFACE_ERROR        0x21
/*! Response if the parameters received are incorrect */
#define DEMO_PARAM_ERROR            0x22
/*! Response if a response is expected from target, but nothing was received */
#define DEMO_RX_ERROR               0x23
/*! Response if PEC Error check was successful */
#define DEMO_ERRORCHECK_OK          0x40
/*! Response if PEC Error check was not OK */
#define DEMO_ERRORCHECK_FAIL        0x41

#endif //__SMB_DEMO_H__
