/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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

#include "SYSTEM_INFO.h"

//EVM default address or previous address: 29, 30
struct DevConfig PreDev[DEVICE_CNT] = {
    {29, P0_INV_DIS, P1_INV_DIS, ACKEN_DIS, BAUD_1_0M, MLTCST_VLD_DIS, 0},
    {30, P0_INV_DIS, P1_INV_DIS, ACKEN_DIS, BAUD_1_0M, MLTCST_VLD_DIS, 0}};

//Init the device setting
struct DevConfig CurDev[DEVICE_CNT] = {
    {DEVICE1_ADD, P0_INV_DIS, P1_INV_DIS, ACKEN_EN, BAUD_1_0M, MLTCST_VLD_EN,
        DEVICE_MULTIADD},
    {DEVICE2_ADD, P0_INV_DIS, P1_INV_DIS, ACKEN_EN, BAUD_1_0M, MLTCST_VLD_EN,
        DEVICE_MULTIADD}};
