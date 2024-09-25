/*****************************************************************************

  Copyright (C) 2021 Texas Instruments Incorporated - http://www.ti.com/

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

#define CSC_APPLICATION_IMAGE_BASE_ADDRESS 0x4800
#define CSC_APPLICATION_IMAGE_SIZE 0x2800

//#include "customer_secure_config.h"

#define MCUBOOT_HEAD_SIZE             0x100

#define FLASH_BASE (CSC_APPLICATION_IMAGE_BASE_ADDRESS + MCUBOOT_HEAD_SIZE)
#define FLASH_SIZE (CSC_APPLICATION_IMAGE_SIZE - MCUBOOT_HEAD_SIZE)
#define CSC_LOCK_STORAGE_ADDR                (0x4400)
#define CSC_LOCK_STORAGE_SIZE                 (0x400)

MEMORY
{
  FLASH (RX)      : origin = FLASH_BASE, length = FLASH_SIZE,
  SRAM (RWX)      : origin = 0x20200000, length = 0x00008000,

  LOCK_STG (R)    : origin = CSC_LOCK_STORAGE_ADDR, length = CSC_LOCK_STORAGE_SIZE

  /* Non-Main configuration memory */
  BCR_CONFIG		(R)   : origin = 0x41C00000, length = 0x00000100,
  BSL_CONFIG		(R)   : origin = 0x41C00100, length = 0x00000080,

}

SECTIONS
{
    .intvecs:   > FLASH_BASE
    .text   : palign(8) {} > FLASH
    .const  : palign(8) {} > FLASH
    .cinit  : palign(8) {} > FLASH
    .pinit  : palign(8) {} > FLASH
    .rodata : palign(8) {} > FLASH
    .ARM.exidx    :  palign(8)  {} > FLASH
    .init_array   :  palign(8)  {} > FLASH
    .binit        : palign(8) {} > FLASH
    .TI.ramfunc      : load = FLASH, palign(8), run=SRAM, table(BINIT)

    .vtable :   > SRAM
    .args   :   > SRAM
    .data   :   > SRAM
    .bss    :   > SRAM
    .sysmem :   > SRAM
    .stack  :   > SRAM (HIGH)

    .lockStg : {} (NOLOAD) > LOCK_STG

    .BCRConfig : {} (NOLOAD) > BCR_CONFIG
    .BSLConfig : {} (NOLOAD) > BSL_CONFIG
}
