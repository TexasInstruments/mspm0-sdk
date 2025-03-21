/*****************************************************************************

  Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

   Redistributions of source code must retain the above copyright 
   notice, this list of conditions and the following disclaimer.

   Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the 
   documentation and/or other materials provided with the   
   distribution.

   Neither the name of Texas Instruments Incorporated nor the names of
   its contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

MEMORY
{
    /*
     * Flash memory for the BSL plugin. This can be any valid Main flash
     * memory region.
     */
    FLASH_PLUGIN_INIT(RWX)		:	 org = 0x00002000,     len = 0x00000100,
    FLASH_PLUGIN_RECEIVE(RWX)	:	 org = 0x00002100,     len = 0x00000200,
    FLASH_PLUGIN_SEND(RWX)		:	 org = 0x00002300,     len = 0x00000100,
    FLASH_PLUGIN_DEINIT(RWX)	:	 org = 0x00002400,     len = 0x00000080,

    FLASH_PLUGIN_OTHERS(RWX)	:	 org = 0x00002480,     len = 0x00000400,

	/*
	 * SRAM memory marked as reserved are used for the ROM BSL execution.
	 * Hence it can't be used for Flash plugin. 
     * Refer Readme for reserved SRAM regions
	 */

    /*
     * SRAM interrupt vectors should be placed at the start of SRAM to
     * comply with the ROM BSL vector table location
     */
    SRAM_BANK0_INT_VECS(RWX)		:    org = 0x20000000,     len = 0x000000C0,

    SRAM_BANK0            (RWX) : org = 0x20000160, len = 0x00000080,
    /*
    * Below is the SRAM space available for user application   
    * This can be derived from the response of Get_device_info command too.
    */
	SRAM_BANK1            (RWX) : org = 0x20210000, len = 0x00010000,

	/* Non-Main configuration memory */
    BCR_CONFIG      (R)   : org = 0x41C00000, len = 0x000000FF,
    BSL_CONFIG      (R)   : org = 0x41C00100, len = 0x00000080,
    DATA            (R)   : org = 0x41D00000, len = 0x00004000,

}

SECTIONS
{
    .flashPluginInit	: palign(8) {} > FLASH_PLUGIN_INIT
    .flashPluginReceive	: palign(8) {} > FLASH_PLUGIN_RECEIVE
    .flashPluginSend	: palign(8) {} > FLASH_PLUGIN_SEND
    .flashPluginDeinit	: palign(8) {} > FLASH_PLUGIN_DEINIT


    .text   : palign(8) {} > FLASH_PLUGIN_OTHERS
    .const  : palign(8) {} > FLASH_PLUGIN_OTHERS
    .cinit  : palign(8) {} > FLASH_PLUGIN_OTHERS
    .pinit  : palign(8) {} > FLASH_PLUGIN_OTHERS
    .rodata : palign(8) {} > FLASH_PLUGIN_OTHERS

    .ARM.exidx    : palign(8) {} > FLASH_PLUGIN_OTHERS
    .init_array   : palign(8) {} > FLASH_PLUGIN_OTHERS
    .binit        : palign(8) {} > FLASH_PLUGIN_OTHERS

    .vtable :   > SRAM_BANK0_INT_VECS , type =  NOINIT
    .args   :   > SRAM_BANK0
    .data   :   > SRAM_BANK0			, type =  NOINIT
    .bss    :   > SRAM_BANK0			, type =  NOINIT
    .sysmem :   > SRAM_BANK0
    .TrimTable :   > SRAM_BANK0
    .stack  :   > SRAM_BANK0 (HIGH)

    .BCRConfig  : {} > BCR_CONFIG
    .BSLConfig  : {} > BSL_CONFIG
    .DataBank   : {} > DATA
}
