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
/*!****************************************************************************
 *  @file       dl_unicomm.h
 *  @brief      Unified Communication Module (UNICOMM) Driver Library
 *  @defgroup   UNICOMM Unified Communication Module (UNICOMM)
 *
 *  @anchor ti_dl_dl_m0p_unicomm_Overview
 *  # Overview
 *
 *  The Unified Communication Module (UNICOMM) Driver Library allows full
 *  configuration of the UNICOMM module.
 *  UNICOMM is a highly flexible peripheral which can be configured as a UART,
 *  SPI, I2C Controller, or I2C Target function.
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup UNICOMM
 * @{
 */
#ifndef ti_dl_dl_unicomm__include
#define ti_dl_dl_unicomm__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MCU_HAS_UNICOMM__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/*! @enum DL_UNICOMM_IP_MODE_SEL */
typedef enum {
    /*! Select UART functionality */
    DL_UNICOMM_UART = UNICOMM_IPMODE_SELECT_UART,
    /*! Select SPI functionality*/
    DL_UNICOMM_SPI = UNICOMM_IPMODE_SELECT_SPI,
    /*! Select I2C functionality in controller mode */
    DL_UNICOMM_I2C_CONTROLLER = UNICOMM_IPMODE_SELECT_I2C_CONTROLLER,
    /*! Select I2C functionality in target mode */
    DL_UNICOMM_I2C_TARGET = UNICOMM_IPMODE_SELECT_I2C_PERIPHERAL,
} DL_UNICOMM_IP_MODE_SEL;

/**
 *  @brief Enables power on unicomm module
 *
 *  @param unicomm        Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_UNICOMM_enablePower(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->inst->GPRCM.PWREN =
        (UNICOMM_PWREN_KEY_UNLOCK_W | UNICOMM_PWREN_ENABLE_ENABLE);
}

/**
 *  @brief Disables power on unicomm module
 *
 *  @param unicomm        Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_UNICOMM_disablePower(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->inst->GPRCM.PWREN =
        (UNICOMM_PWREN_KEY_UNLOCK_W | UNICOMM_PWREN_ENABLE_DISABLE);
}

/**
 *  @brief Returns if  power on unicomm module
 *
 *  @param unicomm        Pointer to the register overlay for the peripheral
 *
 *  @return true if power is enabled
 *  @return false if power is disabled
 */
__STATIC_INLINE bool DL_UNICOMM_isPowerEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->inst->GPRCM.PWREN & UNICOMM_PWREN_ENABLE_MASK) ==
            UNICOMM_PWREN_ENABLE_ENABLE);
}

/**
 *  @brief Resets unicomm peripheral
 *
 *  @param unicomm        Pointer to the register overlay for the UNICOMM peripheral
 */
__STATIC_INLINE void DL_UNICOMM_reset(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->inst->GPRCM.RSTCTL =
        (UNICOMM_RSTCTL_KEY_UNLOCK_W | UNICOMM_RSTCTL_RESETSTKYCLR_CLR |
            UNICOMM_RSTCTL_RESETASSERT_ASSERT);
}

/**
 *  @brief Returns if unicomm peripheral was reset
 *
 *  @param unicomm        Pointer to the register overlay for the UNICOMM peripheral
 *
 *  @return true if peripheral was reset
 *  @return false if peripheral wasn't reset
 *
 */
__STATIC_INLINE bool DL_UNICOMM_isReset(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->inst->GPRCM.STAT & UNICOMM_STAT_RESETSTKY_MASK) ==
            UNICOMM_STAT_RESETSTKY_RESET);
}

/**
 *  @brief Configure the unicomm peripheral for specified ip mode
 *
 *  @param unicomm        Pointer to the register overlay for the UNICOMM peripheral
 *  @param ipMode         Specifies IP mode functionality. One of @ref DL_UNICOMM_IP_MODE_SEL
 */
__STATIC_INLINE void DL_UNICOMM_setIPMode(UNICOMM_Inst_Regs *unicomm, DL_UNICOMM_IP_MODE_SEL ipMode)
{
    unicomm->inst->IPMODE = (ipMode & UNICOMM_IPMODE_SELECT_MASK);
}

/**
 *  @brief Returns ip mode the unicomm peripheral is configured to
 *
 *  @param unicomm        Pointer to the register overlay for the UNICOMM peripheral
 *
 *  @return  Configured IP mode selection
 *  @retval  One of @ref DL_UNICOMM_IP_MODE_SEL
 */
__STATIC_INLINE DL_UNICOMM_IP_MODE_SEL DL_UNICOMM_getIPMode(UNICOMM_Inst_Regs *unicomm)
{
    return (DL_UNICOMM_IP_MODE_SEL)(unicomm->inst->IPMODE & UNICOMM_IPMODE_SELECT_MASK);
}

#ifdef __cplusplus
}
#endif

#endif /* __MCU_HAS_UNICOMM__ */

#endif /* ti_dl_dl_unicomm__include */
/** @}*/
