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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <ti/driverlib/dl_flashctl.h>

#include "aes_cmac.h"
#include "bootutil/bootutil.h"
#include "bootutil/image.h"
#include "bootutil/security_cnt.h"
#include "customer_secure_config.h"
#include "lockable_storage_private.h"
#include "mcuboot_config/mcuboot_logging.h"
#include "rollback.h"
#include "secret.h"
#include "ti_msp_dl_config.h"

/* Uncomment this define in order to have the device perform both phases
 * (privileged and unprivileged) without programming nonmain and without
 * requiring a BOOTRST. Useful if non-main not configured for debugging, or if
 * not programming non-main
 */
//#define DEBUG_NO_RESET_PATH

/* sets the range over the lockable storage (0x4000-0x4800). Includes secret */
#define LOCKABLE_FLASH_FIREWALL 0x00030000

#define BOOT_PRIMARY_SLOT 0
#define BOOT_SECONDARY_SLOT 1

/* current debug image settings. Image could obviously be much larger */
#define PRIMARY_SLOT_OFFSET (CSC_PRIMARY_SLOT_OFFSET)
#define SECONDARY_SLOT_OFFSET (CSC_BANK_SIZE + CSC_PRIMARY_SLOT_OFFSET)
#define IMG_SLOT_SIZE (CSC_APPLICATION_IMAGE_SIZE)

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
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_RED_PIN);

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
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_RED_PIN);
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
    DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_RED_PIN);

#endif /* EXCLUDE_GPIOS */

#ifdef DEBUG_NO_RESET_PATH
    goto noInitdoneLabel;
#endif

    if (DL_SYSCTL_isINITDONEIssued()) {
        /* Execution flow for the unprivileged state. Authentication should
         * already be accomplished and the verified image CMAC tag should live
         * in the locked storage. All firewalls should be active.
         */

#ifdef DEBUG_NO_RESET_PATH
    initdoneLabel:
        __asm("nop");
#endif
        /* TODO: Check that the swap, if desired, was successfully executed */

        /* Check Write Protect Firewall in place */
        if (gLockStgInFlash.bootStatus == LOCKSTG_BOOT_STATUS_SUCCESS) {
            if (!((DL_SYSCTL_getWriteProtectFirewallAddrRange() &
                      LOCKABLE_FLASH_FIREWALL) == LOCKABLE_FLASH_FIREWALL)) {
                mcubootFail();
            }
            /* Jump to the application */
            DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_RED_PIN);
            start_app((uint32_t *) (PRIMARY_SLOT_OFFSET + 0x100));
        } else {
            mcubootFail();
        }

    } else {
        /* Execution flow for the privileged state. Device has no features but
         * static write protect enabled, and images are unverified, and
         * therefore must be authenticated.
         */

#ifdef DEBUG_NO_RESET_PATH
    noInitdoneLabel:
        __asm("nop");
#endif

        trace_init();
        MCUBOOT_LOG_INF("mcuboot_app");

        boot_return_highest_version(&bootRsp);

#ifdef CSC_ENABLE_ROLLBACK_PROTECTION
        /* Check that the current version is at least as recent, if not, fail.
         * TODO: this is a preliminary version of rollback protection, more secure monotonic
         * variants will be explored in future releases of the Customer Secure Code, and
         * being integreated into MCUBoot.
         */
        Rollback_status status;
        status = Rollback_compareCount(bootRsp.br_hdr->ih_ver.iv_major);
        if (status != ROLLBACK_OK) {
            mcubootFail();
        }
#endif

#ifdef CSC_ENABLE_CMAC_ACCELERATION

        CMAC_status CMACstatus;

        CMACstatus = CMAC_init(0x0000);
        if (CMACstatus == CMAC_OK) {
            CMACstatus = CMAC_compareTag(
                &bootRsp.br_image_off, &bootRsp.br_hdr->ih_ver);
        }

        if (CMACstatus == CMAC_OK) {
            bootStatus = 0;
        } else {
            bootStatus = boot_go(&bootRsp);
        }
#else
        bootStatus = boot_go(&bootRsp);
#endif

        if ((0 == bootStatus) && (IMAGE_MAGIC == bootRsp.br_hdr->ih_magic)) {
            MCUBOOT_LOG_INF("bootRsp: slot = %x, offset = %x, ver=%d.%d.%d.%d",
                bootStatus, bootRsp.br_image_off,
                bootRsp.br_hdr->ih_ver.iv_major,
                bootRsp.br_hdr->ih_ver.iv_minor,
                bootRsp.br_hdr->ih_ver.iv_revision,
                bootRsp.br_hdr->ih_ver.iv_build_num);

#ifdef CSC_ENABLE_ROLLBACK_PROTECTION
            Rollback_storeCount(bootRsp.br_hdr->ih_ver.iv_major);
#endif

#ifdef CSC_ENABLE_CMAC_ACCELERATION
            /* Calculate the CMAC value of the image and store it in lockable storage */
            if (CMACstatus != CMAC_OK) {
                CMACstatus =
                    CMAC_generateTag((uint32_t *) bootRsp.br_image_off,
                        IMG_SLOT_SIZE, &bootRsp.br_hdr->ih_ver);
            }
#endif
            /* Stores new secret and writes Keys into the KeyStore*/
            Secret_writeOut();

            /* stores keys based on secret information. Must be done after
             * the secret write out function. */
#ifdef CSC_ENABLE_KEYSTORE
            Keystore_storeKeys();
#endif

            Lock_writeStatus(LOCKSTG_BOOT_STATUS_SUCCESS);
            Lock_writeOut();

            /* Set bank from which we wish to execute */
            if (bootRsp.br_image_off != PRIMARY_SLOT_OFFSET) {
                DL_SYSCTL_executeFromUpperFlashBank();
            } else {
                DL_SYSCTL_executeFromLowerFlashBank();
            }

#ifdef DEBUG_NO_RESET_PATH
            goto initdoneLabel;
#endif

        } else {
            Lock_writeStatus(LOCKSTG_BOOT_STATUS_FAILURE);
            Lock_writeOut();
        }

        /* Set Firewalls (to be enabled upon INITDONE) */
        DL_SYSCTL_setWriteProtectFirewallAddrRange(
            (uint32_t) LOCKABLE_FLASH_FIREWALL);

        DL_SYSCTL_setReadExecuteProtectFirewallAddrStart(CSC_SECRET_ADDR);
        DL_SYSCTL_setReadExecuteProtectFirewallAddrEnd(CSC_SECRET_END);

        DL_SYSCTL_enableReadExecuteProtectFirewall();

        DL_SYSCTL_issueINITDONE();
    }
    mcubootFail();
}
