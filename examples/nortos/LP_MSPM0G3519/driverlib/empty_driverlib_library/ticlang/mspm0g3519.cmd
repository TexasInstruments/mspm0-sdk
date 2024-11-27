/*****************************************************************************

  Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/ 

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
-uinterruptVectors
--stack_size=256

/*
 * Note: SRAM on this device is continuous memory but partitioned in the 
 * linker into two separate sections. This is to account for the upper 64kB
 * of SRAM being wiped out upon the device entering any low-power mode 
 * stronger than SLEEP. Thus, it is up to the end-user to enable SRAM_BANK1 for
 * applications where the memory is considered lost outside of RUN and SLEEP Modes.
 */

MEMORY
{
    FLASH           (RX)  : origin = 0x00000000, length = 0x00080000
    SRAM_BANK0      (RWX) : origin = 0x20200000, length = 0x00010000
    SRAM_BANK1      (RWX) : origin = 0x20210000, length = 0x00010000
    BCR_CONFIG      (R)   : origin = 0x41C00000, length = 0x000000FF
    BSL_CONFIG      (R)   : origin = 0x41C00100, length = 0x00000080
    DATA            (R)   : origin = 0x41D00000, length = 0x00004000

}

SECTIONS
{
    .intvecs:   > 0x00000000
    .text   : palign(8) {} > FLASH
    .const  : palign(8) {} > FLASH
    .cinit  : palign(8) {} > FLASH
    .pinit  : palign(8) {} > FLASH
    .rodata : palign(8) {} > FLASH
    .ARM.exidx    : palign(8) {} > FLASH
    .init_array   : palign(8) {} > FLASH
    .binit        : palign(8) {} > FLASH
    .TI.ramfunc   : load = FLASH, palign(8), run=SRAM_BANK0, table(BINIT)

    .vtable :   > SRAM_BANK0
    .args   :   > SRAM_BANK0
    .data   :   > SRAM_BANK0
    .bss    :   > SRAM_BANK0
    .sysmem :   > SRAM_BANK0
    .TrimTable :  > SRAM_BANK0
    .stack  :   > SRAM_BANK0 (HIGH)

    .BCRConfig  : {} > BCR_CONFIG
    .BSLConfig  : {} > BSL_CONFIG
    .DataBank   : {} > DATA
}
