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

-uinterruptVectors

/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */

MEMORY
{
    FLASH_SBSL_INTVEC(RWX)  :	 org = 0x00001000,      len = 0x00000040,
    FLASH_SBSL_START(RWX)   :	 org = 0x00001040,      len = 0x00000040,
    FLASH_SBSL(RWX)   		:	 org = 0x00001080,      len = 0x00001F80,

    SRAM(RW)				:    org = 0x20200000,      len = 0x00008000,

    BCR_CFG		 		    : 	 org = 0x41C00000,      len = 0x00000080,
    BSL_CFG				    : 	 org = 0x41C00100,      len = 0x00000080,
    FACTORY_CFG		  		: 	 org = 0x41C40000,      len = 0x00000200,
}

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
	.intvecs			: PALIGN(8) {} > FLASH_SBSL_INTVEC
	.sblStart			: PALIGN(8) {} > FLASH_SBSL_START

    GROUP(FLASH_MEMORY) {
        .text          	: PALIGN(8) {}
        .const         	: PALIGN(8) {}
        .cinit         	: PALIGN(8) {}
        .rodata			: PALIGN(8) {}
    } > FLASH_SBSL

    GROUP(SRAM) {
        .vtable 		: {}
        .data          	: {}
        .bss           	: {}
   } > SRAM

    .stack  :   > SRAM (HIGH) SIZE(BSL_CI_stackSize)

    .factoryConfig          : {} > FACTORY_CFG
    .BCRConfig              : {} > BCR_CFG
    .BSLConfig              : {} > BSL_CFG
}
