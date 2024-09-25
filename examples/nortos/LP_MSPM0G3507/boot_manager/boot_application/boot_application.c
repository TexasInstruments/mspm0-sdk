/*
 * Copyright (c) 2015-2023, Texas Instruments Incorporated
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

#include <stddef.h>
#include <stdint.h>

#include "bootutil/bootutil.h"
#include "bootutil/image.h"
#include "bootutil/security_cnt.h"
#include "mcuboot_config/mcuboot_logging.h"
#include "ti_msp_dl_config.h"

static void start_app(uint32_t *vector_table)
{
    /* The following code resets the SP to the value specified in the
     * provided vector table, and then the Reset Handler is invoked.
     *
     * Per ARM Cortex specification:
     *
     *           ARM Cortex VTOR
     *
     *
     *   Offset             Vector
     *
     * 0x00000000  ++++++++++++++++++++++++++
     *             |    Initial SP value    |
     * 0x00000004  ++++++++++++++++++++++++++
     *             |         Reset          |
     * 0x00000008  ++++++++++++++++++++++++++
     *             |          NMI           |
     *             ++++++++++++++++++++++++++
     *             |           .            |
     *             |           .            |
     *             |           .            |
     *
     * */

    /* Reset the SP with the value stored at vector_table[0] */
    __asm volatile(
        "LDR R3,[%[vectab],#0x0] \n"
        "MOV SP, R3       \n" ::[vectab] "r"(vector_table));

    /* Set the Reset Vector to the new vector table (Will be reset to 0x000) */
    SCB->VTOR = (uint32_t) vector_table;

    /* Jump to the Reset Handler address at vector_table[1] */

    ((void (*)(void))(*(vector_table + 1)))();
}

/* do_boot is called in the event of a successful verification of an image */
static void do_boot(struct boot_rsp *rsp)
{
#ifndef EXCLUDE_GPIOS

    /* Clears the red LED after successfully completing the boot */
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);

#endif

    MCUBOOT_LOG_INF("Starting Main Application");
    MCUBOOT_LOG_INF("  Image Start Offset: 0x%x", (int) rsp->br_image_off);

    /* SCB->VTOR = 0x0 + rsp->br_image_off + rsp->br_hdr->ih_hdr_size;
     * Currnetly the image is run from internal flash.
     * Base address of this application is 0x0000
     */
    uint32_t vector_table = 0x0 + rsp->br_image_off + rsp->br_hdr->ih_hdr_size;

    MCUBOOT_LOG_INF("  Vector Table Start Address: 0x%x", (int) vector_table);

    start_app((uint32_t *) vector_table);
}

/* Fail procedure for mcuboot if no valid image found. Currently just toggles
 * the red LED.
 */
void mcubootFail(void)
{
    while (1) {
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);
        delay_cycles(10000000);
    }
}

int main(void)
{
    fih_int bootStatus;
    struct boot_rsp bootRsp;
    SYSCFG_DL_init();

#ifndef EXCLUDE_GPIOS
    /* Sets the red LED solid during the validation phase. Will turn off
     * after the validation phase is complete.
     */
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);

#endif /* EXCLUDE_GPIOS */

    trace_init();
    MCUBOOT_LOG_INF("mcuboot_app");

    bootStatus = boot_go(&bootRsp);

    if ((0 == bootStatus) && (IMAGE_MAGIC == bootRsp.br_hdr->ih_magic)) {
        MCUBOOT_LOG_INF("bootRsp: slot = %x, offset = %x, ver=%d.%d.%d.%d",
            bootStatus, bootRsp.br_image_off, bootRsp.br_hdr->ih_ver.iv_major,
            bootRsp.br_hdr->ih_ver.iv_minor,
            bootRsp.br_hdr->ih_ver.iv_revision,
            bootRsp.br_hdr->ih_ver.iv_build_num);

        do_boot(&bootRsp);
    } else {
        MCUBOOT_LOG_INF("bootRsp: no image found");
        mcubootFail();
    }
}
