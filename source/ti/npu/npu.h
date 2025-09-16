/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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
 *  @file       npu.h
 *  @brief      Neural Processing Unit (NPU) Driver Interface
 *
 *  The Neural Processing Unit (NPU) Driver builds on top of the MSP DriverLib
 *  dl_npu module for the NPU accelerator and provides an abstracted interface
 *  for compiled machine learning (ML) models build for NPU acceleration to
 *  call into for hardware access.
 *
 *  This driver is intended to be called into by compiled neural network
 *  libraries, which are built using TI's Neural Network Compiler (NNC) for
 *  MCUs.  This driver is not intended to be used directly by user software.
 *
 ******************************************************************************
 */

#ifndef NPU_DRIVER_H_
#define NPU_DRIVER_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @brief      Set the instruction memory of the NPU at the specified offset.
 *
 *  @param[in]  byteOffset     The offset from the beginning of the NPU
 *                             instruction memory to start the write from.
 *                             This number MUST be a multiple of 4 bytes.
 *                             If (byteOffset%4) != 0, a hard fault may occur.
 *  @param[in]  num            The number of 32-bit words to set.  
 *  @param[in]  src            A pointer to the 64-bit aligned instructions
 *                             in system memory to load to the NPU memory.
 *  @return     None
 */
extern void NPU_setInstrMem(uint32_t byteOffset, uint32_t num, \
                            const uint32_t *src);

/**
 *  @brief      Set the parameter memory of the NPU at the specified offset.
 *
 *  @param[in]  byteOffset     The offset from the beginning of the NPU
 *                             parameter memory to start the write from.
 *                             This number MUST be a multiple of 4 bytes.
 *                             If (byteOffset%4) != 0, a hard fault may occur.
 *  @param[in]  num            The number of 32-bit words to set.
 *  @param[in]  src            A pointer to the 64-bit aligned parameters
 *                             in system memory to load to the NPU memory.
 *  @return     None
 */
extern void NPU_setParamMem(uint32_t byteOffset, uint32_t num, \
                            const uint32_t *src);

/**
 *  @brief      Get the address of the NPU ACC register.
 *
 *  @return     The memory-mapped address of the NPU ACC register.
 */
extern uint32_t NPU_getAccReg(void);

/**
 *  @brief      Get the address of the NPU control register.
 *
 *  @return     The memory-mapped address of the NPU control register.
 */
extern uint32_t NPU_getCtlReg(void);

#ifdef __cplusplus
}
#endif

#endif /* NPU_DRIVER_H_ */
