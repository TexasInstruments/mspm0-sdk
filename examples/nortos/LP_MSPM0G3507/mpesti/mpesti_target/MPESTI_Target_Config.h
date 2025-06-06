/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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

/*!****************************************************************************
 *  @file       MPESTI_Target_Config.h
 *  @brief      MPESTI Target Config header file
 *  @defgroup   MPESTI_Config MPESTI_Target_Config
 *
 *  @anchor MPESTI_Target_Config_H__Overview
 *  # Overview
 *
 *  PESTI communication is based on a command and response method
 *  where the peripheral/target is permitted to only transmit data upon
 *  request by the initiator. This header file contains all the parameters that
 *  user has to configure for their use case.
 *
 *  <hr>
 ******************************************************************************/
/** @addtogroup MPESTI_Target_Config
 * @{
 */

#ifndef __MPESTI_TARGET_CONFIG_H__
#define __MPESTI_TARGET_CONFIG_H__

//*****************************************************************************
// Configuration
//*****************************************************************************
/** @addtogroup MPESTI_TARGET_USER_CONFIGURABLE_PARAMETERS
  *
  * MPESTI Target user configurable parameters.
 * @{
 */
/*!
 * @brief VWE payload size
 */
#define VWE_PAYLOAD_SIZE 32
/*!
 * @brief Vendor ID, This id is being used for testing purpose. User can replace/define with
 * their own vendor ID.
 */
#define VENDOR_ID 1 /* Not provided by PCI-SIG */
/*!
 * @brief Payload format version defined as per the requirements specification
 * The Payload Format Version must be 02h to differentiate it from the previous
 * 01h definition and match the PCI-SIG format version
 */
#define PAYLOAD_FORMAT_VERSION 2
/*!
 * @brief Device class, User can replace/define with
 * their own device class
 */
#define DEVICE_CLASS 0 /* Customer should use their device class here */
/*!
 * @brief PEC support enable/disable
 * Currently it is not supported in this driver
 */
#define AP_PEC_SUPPORT 0x00
/*!
 * @brief Vendor ID format, User can replace/define with
 * their own project need
 */
#define VENDOR_ID_FORMAT 0x01
/*!
 * @brief VWE support: 1: supported multibytes, 0: Not supported and respond with byte 0
 */
#define VWE_SUPPORT 1

/** @}*/

#endif /* __MPESTI_TARGET_CONFIG_H__ */
/** @}*/
