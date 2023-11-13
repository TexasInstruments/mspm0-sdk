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

#define MCUBOOT_HEAD_SIZE             0x100

/* Memory map. Should match the map in the flash_map_backend.c of the boot application */
#define BOOT_PRIMARY_BASE_ADDRESS   0x05800
#define BOOT_PRIMARY_SIZE           0x0D000
#define BOOT_SECONDARY_BASE_ADDRESS 0x12800
#define BOOT_SECONDARY_SIZE         0x0D000

#ifdef PRIMARY_SLOT

#define FLASH_BASE (BOOT_PRIMARY_BASE_ADDRESS + MCUBOOT_HEAD_SIZE)
#define FLASH_SIZE (BOOT_PRIMARY_SIZE - MCUBOOT_HEAD_SIZE)

#else

#define FLASH_BASE (BOOT_SECONDARY_BASE_ADDRESS + MCUBOOT_HEAD_SIZE)
#define FLASH_SIZE (BOOT_SECONDARY_SIZE - MCUBOOT_HEAD_SIZE)

#endif


MEMORY
{
    FLASH           (RX)  : origin = FLASH_BASE, length = FLASH_SIZE
    SRAM            (RWX) : origin = 0x20200000, length = 0x00008000

}

SECTIONS
{
    .intvecs:   > FLASH_BASE
    .text   : palign(8) {} > FLASH
    .const  : palign(8) {} > FLASH
    .cinit  : palign(8) {} > FLASH
    .pinit  : palign(8) {} > FLASH
    .rodata : palign(8) {} > FLASH
    .ARM.exidx    : palign(8) {} > FLASH
    .init_array   : palign(8) {} > FLASH
    .binit        : palign(8) {} > FLASH
    .TI.ramfunc   : load = FLASH, palign(8), run=SRAM, table(BINIT)


    .vtable :   > SRAM
    .args   :   > SRAM
    .data   :   > SRAM
    .bss    :   > SRAM
    .sysmem :   > SRAM
    .stack  :   > SRAM (HIGH)

}
