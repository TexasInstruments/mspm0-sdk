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
	INTVEC(RWX)  			:	org = 0x00000000,		len = 0x00000040,
    INVOCATION_START(RWX)   :	org = 0x00000040,		len = 0x00000010,

    FLASH_BSL_START(RWX)	:	org = 0x00000050,		len = 0x00000030,
	FLASH_BSL (RWX)			:	org = 0x00000100,		len = 0x00001700,

	SRAM (RW)				:	org = 0x20000000,		len = 0x00000400,

    BCR_CFG		 		    :	org = 0x41C00000,		len = 0x00000080,
    BSL_CFG				    :	org = 0x41C00100,		len = 0x00000080,
    FACTORY_CFG		  		:	org = 0x41C40000,		len = 0x00000200,
}

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
	.intvecs			: PALIGN(8) {} > INTVEC
	.invStart			: PALIGN(8) {} > INVOCATION_START
	.flashBSL_start		: PALIGN(8) {} > FLASH_BSL_START



    GROUP(FLASH_MEMORY)
    {
        .text          	: PALIGN(8) {}
        .const         	: PALIGN(8) {}
        .cinit         	: PALIGN(8) {}
        .rodata			: PALIGN(8) {}

    } > FLASH_BSL



    GROUP(SRAM)
    {
    	.vtable			: {}
        .data          	: {}
        .bss           	: {}
    } > SRAM


    .stack  :   > SRAM (HIGH)


    .BCRConfig              : PALIGN(8){} > BCR_CFG
    .BSLConfig              : PALIGN(8){} > BSL_CFG
}
