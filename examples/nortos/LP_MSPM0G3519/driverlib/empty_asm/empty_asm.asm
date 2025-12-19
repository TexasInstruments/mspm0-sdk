/*
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
 */

   .syntax unified
    .thumb
    .section .text
    .align 2
    .global main

    .type main, %function
main:
    @ Call SYSCFG_DL_init()
    bl      SYSCFG_DL_init
loop:
    BL   GPIO_clearPA0 // turn on LED0
    LDR  R0,= 40000000 // 500msec delay
    BL   Delay
    BL   GPIO_setPA0   // turn off LED0
    LDR  R0,= 40000000 // 500msec sec
    BL   Delay
    B    loop


GPIO_clearPA0:
    LDR  R1, = 0x400A1280
    LDR  R0, [R1]
    MOVS R2, #0x01
    BICS R0, R0, R2
    STR  R0, [R1]
    BX   LR

GPIO_setPA0:
    LDR  R1, = 0x400A1280
    LDR  R0, [R1]
    MOVS R2, #0x01
    ORRS R0, R0, R2
    STR  R0, [R1]
    BX   LR

Delay:
    SUBS R0,R0,#2
dloop:
    SUBS R0,R0,#4
    NOP
    BHS  dloop
    BX   LR
