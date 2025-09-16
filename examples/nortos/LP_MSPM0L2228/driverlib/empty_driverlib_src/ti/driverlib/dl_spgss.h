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
 *  @file       dl_spgss.h
 *  @brief      Serial Peripheral Group (SPGSS) Driver Library
 *  @defgroup   SPGSS Serial Peripheral Group (SPGSS)
 *
 *  @anchor ti_dl_dl_m0p_spgss_Overview
 *  # Overview
 *
 *  The Serial Peripheral Group (SPGSS) Driver Library allows full configuration
 *  of the SPG module.
 *  The SPG module combines one or more UNICOMM modules for special functions
 *  like I2C loopback.
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup SPGSS
 * @{
 */
#ifndef ti_dl_dl_spgss__include
#define ti_dl_dl_spgss__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>

#ifdef __MCU_HAS_SPGSS__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_SPGSS_INTERRUPTS
 *  @{
 */

/** @}*/

/*! @enum DL_SPGSS_PAIR_CONTROLLER_SEL */
typedef enum {
    /*! Select UC0 as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC0_= SPGSS_PAIR0_CTL_SPG1,
    /*! Select UC1 as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC1_= SPGSS_PAIR0_CTL_SPG2,
    /*! Select UC2 as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC2_= SPGSS_PAIR0_CTL_SPG3,
    /*! Select UC3 as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC3_= SPGSS_PAIR0_CTL_SPG4,
    /*! Select UC4 as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC4_= SPGSS_PAIR0_CTL_SPG5,
    /*! Select UC5 as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC5_= SPGSS_PAIR0_CTL_SPG6,
    /*! Select UC6 as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC6_= SPGSS_PAIR0_CTL_SPG7,
    /*! Select UC7 as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC7_= SPGSS_PAIR0_CTL_SPG8,
    /*! Select UC8 as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC8_= SPGSS_PAIR0_CTL_SPG9,
    /*! Select UC9 as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC9_= SPGSS_PAIR0_CTL_SPG10,
    /*! Select UC10_as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC10_= SPGSS_PAIR0_CTL_SPG11,
    /*! Select UC11_as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC11_= SPGSS_PAIR0_CTL_SPG12,
    /*! Select UC12_as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC12_= SPGSS_PAIR0_CTL_SPG13,
    /*! Select UC13_as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC13_= SPGSS_PAIR0_CTL_SPG14,
    /*! Select UC14_as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC14_= SPGSS_PAIR0_CTL_SPG15,
    /*! Select UC15_as controller for loopback */
    DL_SPGSS_PAIR_CONTROLLER_UC15_= SPGSS_PAIR0_CTL_SPG16,
} DL_SPGSS_PAIR_CONTROLLER_SEL;

/*! @enum DL_SPGSS_PAIR_TARGET_SEL */
typedef enum {
    /*! Select UC0 as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC0_= SPGSS_PAIR0_TARGET_SPG1,
    /*! Select UC1 as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC1_= SPGSS_PAIR0_TARGET_SPG2,
    /*! Select UC2 as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC2_= SPGSS_PAIR0_TARGET_SPG3,
    /*! Select UC3 as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC3_= SPGSS_PAIR0_TARGET_SPG4,
    /*! Select UC4 as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC4_= SPGSS_PAIR0_TARGET_SPG5,
    /*! Select UC5 as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC5_= SPGSS_PAIR0_TARGET_SPG6,
    /*! Select UC6 as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC6_= SPGSS_PAIR0_TARGET_SPG7,
    /*! Select UC7 as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC7_= SPGSS_PAIR0_TARGET_SPG8,
    /*! Select UC8 as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC8_= SPGSS_PAIR0_TARGET_SPG9,
    /*! Select UC9 as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC9_= SPGSS_PAIR0_TARGET_SPG10,
    /*! Select UC10_as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC10_= SPGSS_PAIR0_TARGET_SPG11,
    /*! Select UC11_as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC11_= SPGSS_PAIR0_TARGET_SPG12,
    /*! Select UC12_as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC12_= SPGSS_PAIR0_TARGET_SPG13,
    /*! Select UC13_as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC13_= SPGSS_PAIR0_TARGET_SPG14,
    /*! Select UC14_as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC14_= SPGSS_PAIR0_TARGET_SPG15,
    /*! Select UC15_as target for loopback */
    DL_SPGSS_PAIR_TARGET_UC15_= SPGSS_PAIR0_TARGET_SPG16,
} DL_SPGSS_PAIR_TARGET_SEL;

/*! @enum DL_SPGSS_PAIR_INDEX */
typedef enum {
    /*! Loopback pair index 0 */
    DL_SPGSS_PAIR_INDEX_0 = 0,
    /*! Loopback pair index 1 */
    DL_SPGSS_PAIR_INDEX_1 = 1,
    /*! Loopback pair index 2 */
    DL_SPGSS_PAIR_INDEX_2 = 2,
    /*! Loopback pair index 3 */
    DL_SPGSS_PAIR_INDEX_3 = 3,
    /*! Loopback pair index 4 */
    DL_SPGSS_PAIR_INDEX_4 = 4,
    /*! Loopback pair index 5 */
    DL_SPGSS_PAIR_INDEX_5 = 5,
    /*! Loopback pair index 6 */
    DL_SPGSS_PAIR_INDEX_6 = 6,
    /*! Loopback pair index 7 */
    DL_SPGSS_PAIR_INDEX_7 = 7,
} DL_SPGSS_PAIR_INDEX;

/**
 * @brief  Configuration struct for @ref DL_SPGSS_setLoopbackConfig.
 */
typedef struct {
    /*! SPGSS pairing controller selection. One of @ref DL_SPGSS_PAIR_CONTROLLER_SEL */
    DL_SPGSS_PAIR_CONTROLLER_SEL controller;
    /*! SPGSS pairing target selection. One of @ref DL_SPGSS_PAIR_TARGET_SEL */
    DL_SPGSS_PAIR_TARGET_SEL target;
} DL_SPGSS_LoopbackConfig;

/**
 *  @brief      Enable SPGSS pair loopback
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *  @param[in]  index  Specifies pair index to be configured. One of
 *                        @ref DL_SPGSS_PAIR_INDEX.
 */
__STATIC_INLINE void DL_SPGSS_enableLoopback(
    SPGSS_Regs *spgss, DL_SPGSS_PAIR_INDEX index)
{
    volatile uint32_t *pReg = &spgss->I2C_PAIR.PAIR0;

    *(pReg + (uint32_t) index) |= (SPGSS_PAIR0_EN_ENABLE);
}

/**
 *  @brief      Disable SPGSS pair loopback
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *  @param[in]  index  Specifies pair index to be configured. One of
 *                        @ref DL_SPGSS_PAIR_INDEX.
 */
__STATIC_INLINE void DL_SPGSS_disableLoopback(
    SPGSS_Regs *spgss, DL_SPGSS_PAIR_INDEX index)
{
    volatile uint32_t *pReg = &spgss->I2C_PAIR.PAIR0;

    *(pReg + (uint32_t) index) &= ~(SPGSS_PAIR0_EN_ENABLE);
}

/**
 *  @brief      Set SPG controller and target for loopback
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *  @param[in]  index  Specifies pair index to be configured. One of
 *                        @ref DL_SPGSS_PAIR_INDEX.
 *  @param[in]  config  Pointer to the pair configuration struct
 *                      @ref DL_SPGSS_LoopbackConfig.
 */
__STATIC_INLINE void DL_SPGSS_setLoopbackConfig(SPGSS_Regs *spgss, DL_SPGSS_PAIR_INDEX index, DL_SPGSS_LoopbackConfig *config)
{
    volatile uint32_t *pReg = &spgss->I2C_PAIR.PAIR0;

    DL_Common_updateReg((pReg + (uint32_t) index),
        ((uint32_t) config->controller | (uint32_t) config->target),
        (SPGSS_PAIR0_CTL_MASK | SPGSS_PAIR0_TARGET_MASK));
}

/**
 *  @brief      Get SPG controller and target for loopback
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *  @param[in]  index  Specifies loopback index to be configured. One of
 *                        @ref DL_SPGSS_PAIR_INDEX.
 *  @param[in]  config  Pointer to the pair configuration struct
 *                      @ref DL_SPGSS_LoopbackConfig.
 */
__STATIC_INLINE void DL_SPGSS_getLoopbackConfig(SPGSS_Regs *spgss, DL_SPGSS_PAIR_INDEX index, DL_SPGSS_LoopbackConfig *config)
{
    volatile uint32_t *pReg = &spgss->I2C_PAIR.PAIR0;

    config->controller = (DL_SPGSS_PAIR_CONTROLLER_SEL)(*(pReg + (uint32_t) index) & SPGSS_PAIR0_CTL_MASK);
    config->target = (DL_SPGSS_PAIR_TARGET_SEL)(*(pReg + (uint32_t) index) & SPGSS_PAIR0_TARGET_MASK);
}

/**
 *  @brief      Enable SPGSS interrupt
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPGSS_INTERRUPTS.
 */
__STATIC_INLINE void DL_SPGSS_enableInterrupt(
    SPGSS_Regs *spgss, uint32_t interruptMask)
{
    spgss->CPU_INT.IMASK |= (interruptMask);
}

/**
 *  @brief      Disable SPGSS interrupt
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPGSS_INTERRUPTS.
 *
 */
__STATIC_INLINE void DL_SPGSS_disableInterrupt(
    SPGSS_Regs *spgss, uint32_t interruptMask)
{
    spgss->CPU_INT.IMASK &= ~(interruptMask);
}

/**
 *  @brief      Check which SPGSS interrupts are enabled
 *
 *  @param[in]  spgss          Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPGSS_INTERRUPTS.
 *
 *  @return     Which of the requested SPGSS interrupts are enabled
 *
 *  @retval     Bitwise OR of @ref DL_SPGSS_INTERRUPTS values
 */
__STATIC_INLINE uint32_t DL_SPGSS_getEnabledInterrupts(
    SPGSS_Regs *spgss, uint32_t interruptMask)
{
    return (spgss->CPU_INT.IMASK & interruptMask);
}

/**
 *  @brief      Check interrupt flag of enabled SPGSS interrupt
 *
 *  Checks if the SPGSS interrupt that was previously enabled is pending.
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPGSS_INTERRUPTS.
 *
 *  @return     If the enabled SPGSS interrupt is pending
 *
 *  @sa         DL_SPGSS_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_SPGSS_getEnabledInterruptStatus(
    SPGSS_Regs *spgss, uint32_t interruptMask)
{
    return (spgss->CPU_INT.MIS & interruptMask);
}

/**
 *  @brief      Check interrupt flag of any SPGSS interrupt
 *
 *  Checks if the SPGSS interrupt is pending. Interrupt does not have to
 *  be previously enabled.
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPGSS_INTERRUPTS.
 *
 *  @return     If the SPGSS interrupt is pending
 *
 */
__STATIC_INLINE uint32_t DL_SPGSS_getRawInterruptStatus(
    SPGSS_Regs *spgss, uint32_t interruptMask)
{
    return (spgss->CPU_INT.RIS & interruptMask);
}

/**
 *  @brief      Clear pending SPGSS interrupt
 *
 *  @param[in]  spgss  Pointer to the register overlay for the peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPGSS_INTERRUPTS.
 *
 */
__STATIC_INLINE void DL_SPGSS_clearInterruptStatus(
    SPGSS_Regs *spgss, uint32_t interruptMask)
{
    spgss->CPU_INT.ICLR |= (interruptMask);
}

#ifdef __cplusplus
}
#endif

#endif /* __MCU_HAS_SPGSS__ */

#endif /* ti_dl_dl_spgss__include */
/** @}*/
