/*
 * Copyright (c) 2018 Open Source Foundries Limited
 * Copyright (c) 2019 Arm Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __MCUBOOT_CONFIG_H__
#define __MCUBOOT_CONFIG_H__

/*
 * Template configuration file for MCUboot.
 *
 * When porting MCUboot to a new target, copy it somewhere that your
 * include path can find it as mcuboot_config/mcuboot_config.h, and
 * make adjustments to suit your platform.
 *
 * For examples, see:
 *
 * boot/zephyr/include/mcuboot_config/mcuboot_config.h
 * boot/mynewt/mcuboot_config/include/mcuboot_config/mcuboot_config.h
 */

/*
 * Signature
 *
 * You must uncomment below to enable image signature.
 */

#define MCUBOOT_USE_MSP_CRYPTO
#define MCUBOOT_SIGN_EC256

/*
 * Support for off-chip memory (external flash)
 *
 * If the desired configuration is for MCUBoot to search for valid images
 * in external flash, TI_BOOT_USE_EXTERNAL_FLASH must be defined.
 *
 * Note that if external flash is used, then also uncomment
 * MCUBOOT_OVERWRITE_ONLY and comment out MCUBOOT_DIRECT_XIP.
 * MCUBOOT_DIRECT_XIP is not supported when external flash configuration
 * is enabled.
 *
 * */
/* Uncomment to enable external flash configuration */
//#define TI_BOOT_USE_EXTERNAL_FLASH

/*
 * Upgrade mode
 *
 * The default is to support A/B image swapping with rollback.  Other modes
 * with simpler code path, which only supports overwriting the existing image
 * with the update image or running the newest image directly from its flash
 * partition, are also available.
 *
 * You can enable only one mode at a time from the list below to override
 * the default upgrade mode.
 */

/* Uncomment to enable the overwrite-only code path. */
//#define MCUBOOT_OVERWRITE_ONLY

#ifdef MCUBOOT_OVERWRITE_ONLY
/*software based downgrade prevention the image version numbers are compared */
#define MCUBOOT_DOWNGRADE_PREVENTION
#define MCUBOOT_WATCHDOG_FEED() \
    {                           \
    }
/* Uncomment to only erase and overwrite those primary slot sectors needed
 * to install the new image, rather than the entire image slot. */
//#define MCUBOOT_OVERWRITE_ONLY_FAST
#endif

/* Uncomment to enable the direct-xip code path. */
#define MCUBOOT_DIRECT_XIP

#if defined(TI_BOOT_USE_EXTERNAL_FLASH) && \
    (!defined(MCUBOOT_OVERWRITE_ONLY) || defined(MCUBOOT_DIRECT_XIP))
#error \
    "If external flash is configured, only MCUBOOT_OVERWRITE_ONLY is supported"
#endif

/* Uncomment to enable the revert mechanism in direct-xip mode. */
/* #define MCUBOOT_DIRECT_XIP_REVERT */

/* Uncomment to enable the ram-load code path. */
/* #define MCUBOOT_RAM_LOAD */

/* Uncomment to use image version for rollback protection. */
//#define MCUBOOT_HW_ROLLBACK_PROT

/*
 * Always check the signature of the image in the primary slot before booting,
 * even if no upgrade was performed. This is recommended if the boot
 * time penalty is acceptable.
 */
#define MCUBOOT_VALIDATE_PRIMARY_SLOT

/*
 * Flash abstraction
 */

/* Uncomment if your flash map API supports flash_area_get_sectors().
 * See the flash APIs for more details. */
#define MCUBOOT_USE_FLASH_AREA_GET_SECTORS

/* Default maximum number of flash sectors per image slot; change
 * as desirable. */
#define MCUBOOT_MAX_IMG_SECTORS 128

/* Default number of separately updateable images; change in case of
 * multiple images. */
#define MCUBOOT_IMAGE_NUMBER 1

/*
 * Logging
 */

/*
 * If logging is enabled the following functions must be defined by the
 * platform:
 *
 *    MCUBOOT_LOG_MODULE_REGISTER(domain)
 *      Register a new log module and add the current C file to it.
 *
 *    MCUBOOT_LOG_MODULE_DECLARE(domain)
 *      Add the current C file to an existing log module.
 *
 *    MCUBOOT_LOG_ERR(...)
 *    MCUBOOT_LOG_WRN(...)
 *    MCUBOOT_LOG_INF(...)
 *    MCUBOOT_LOG_DBG(...)
 *
 * The function priority is:
 *
 *    MCUBOOT_LOG_ERR > MCUBOOT_LOG_WRN > MCUBOOT_LOG_INF > MCUBOOT_LOG_DBG
 */
#define MCUBOOT_HAVE_LOGGING 1

/*
 * Assertions
 */

/* Uncomment if your platform has its own mcuboot_config/mcuboot_assert.h.
 * If so, it must provide an ASSERT macro for use by bootutil. Otherwise,
 * "assert" is used. */
#define MCUBOOT_HAVE_ASSERT_H

/*
 * Watchdog feeding
 */

/* This macro might be implemented if the OS / HW watchdog is enabled while
 * doing a swap upgrade and the time it takes for a swapping is long enough
 * to cause an unwanted reset. If implementing this, the OS main.c must also
 * enable the watchdog (if required)!
 *
 * #define MCUBOOT_WATCHDOG_FEED()
 *    do { do watchdog feeding here! } while (0)
 */

#endif /* __MCUBOOT_CONFIG_H__ */
