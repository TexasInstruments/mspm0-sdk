/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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
 *  @file       dl_npu.h
 *  @brief      Neural Processing Unit (NPU) Driver Library
 *  @defgroup   Neural Processing Unit (NPU)
 *
 *  @anchor ti_dl_dl_npu_Overview
 *  # Overview
 *
 *  The Neural Processing Unit (NPU) Driver Library provides low level
 *  NPU drivers for power control, interrupt management, and access
 *  to NPU control registers as well as NPU memory.
 *  The power control and interrupt management registers are expected to be
 *  called as a part of the user application, but the NPU control register
 *  and NPU memory address accessor functions are only intended to be used by
 *  the NPU library.
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup NPU
 * @{
 */
#ifndef ti_dl_dl_npu__include
#define ti_dl_dl_npu__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>

#ifdef __MSPM0_HAS_NPU__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_NPU_INTERRUPT
 *  @{
 */

/*!
 * @brief NPU done interrupt
 */
#define DL_NPU_INTERRUPT_DONE (NPU_IMASK_DONE_SET)

/** @}*/

/* clang-format on */

/*! @enum DL_NPU_IIDX */
typedef enum {
    /*! NPU interrupt index for no interrupt */
    DL_NPU_IIDX_NO_INTERRUPT = NPU_IIDX_STAT_NO_INTR,
    /*! NPU interrupt index for NPU done interrupt */
    DL_NPU_IIDX_OUTPUT_READY = NPU_IIDX_STAT_DONE
} DL_NPU_IIDX;

/**
 * @brief Enables the Peripheral Write Enable (PWREN) register for the NPU
 *
 *  Before any NPU registers can be configured by software, the
 *  NPU itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the NPU's PWREN register.
 *
 * @param npu        Pointer to the register overlay for the NPU
 */
__STATIC_INLINE void DL_NPU_enablePower(NPU_Regs *npu)
{
    npu->GPRCM.PWREN = (NPU_PWREN_KEY_UNLOCK_W | NPU_PWREN_ENABLE_ENABLE);
}

/**
 * @brief Disables the Peripheral Write Enable (PWREN) register for the NPU
 *
 *  When the PWREN.ENABLE bit is cleared, the NPU's registers and memory are
 *  not accessible for read/write operations.
 *
 *  @note This API does not provide large power savings.
 *
 * @param npu        Pointer to the register overlay for the NPU
 */
__STATIC_INLINE void DL_NPU_disablePower(NPU_Regs *npu)
{
    npu->GPRCM.PWREN = (NPU_PWREN_KEY_UNLOCK_W | NPU_PWREN_ENABLE_DISABLE);
}

/**
 * @brief Returns if the Peripheral Write Enable (PWREN) register for the NPU
 *        is enabled
 *
 *  Before any NPU registers can be configured by software, the
 *  NPU itself must be enabled by writing the ENABLE bit together with
 *  the appropriate KEY value to the NPU's PWREN register.
 *
 *  When the PWREN.ENABLE bit is cleared, the NPU's registers are not
 *  accessible for read/write operations.
 *
 * @param npu        Pointer to the register overlay for the NPU
 *
 * @return true if NPU register access is enabled
 * @return false if NPU register access is disabled
 */
__STATIC_INLINE bool DL_NPU_isPowerEnabled(const NPU_Regs *npu)
{
    return (
        (npu->GPRCM.PWREN & NPU_PWREN_ENABLE_MASK) == NPU_PWREN_ENABLE_ENABLE);
}

/**
 * @brief Resets NPU peripheral
 *
 * @param npu        Pointer to the register overlay for the NPU
 */
__STATIC_INLINE void DL_NPU_reset(NPU_Regs *npu)
{
    npu->GPRCM.RSTCTL =
        (NPU_RSTCTL_KEY_UNLOCK_W | NPU_RSTCTL_RESETSTKYCLR_CLR |
            NPU_RSTCTL_RESETASSERT_ASSERT);
}

/**
 * @brief Returns if NPU was reset
 *
 * @param npu        Pointer to the register overlay for the NPU
 *
 * @return true if NPU was reset
 * @return false if NPU wasn't reset
 *
 */
__STATIC_INLINE bool DL_NPU_isReset(const NPU_Regs *npu)
{
    return ((npu->GPRCM.STAT & NPU_STAT_RESETSTKY_MASK) ==
            NPU_STAT_RESETSTKY_RESET);
}

/**
 *  @brief      Enable NPU interrupts
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_NPU_INTERRUPT.
 */
__STATIC_INLINE void DL_NPU_enableInterrupt(
    NPU_Regs *npu, uint32_t interruptMask)
{
    npu->CPU_INT.IMASK |= interruptMask;
}

/**
 *  @brief      Disable NPU interrupts
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to disable. Bitwise OR of
 *                             @ref DL_NPU_INTERRUPT.
 */
__STATIC_INLINE void DL_NPU_disableInterrupt(
    NPU_Regs *npu, uint32_t interruptMask)
{
    npu->CPU_INT.IMASK &= ~(interruptMask);
}

/**
 *  @brief      Check which NPU interrupts are enabled
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_NPU_INTERRUPT.
 *
 *  @return     Which of the requested NPU interrupts are enabled
 *
 *  @retval     Bitwise OR of @ref DL_NPU_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_NPU_getEnabledInterrupts(
    NPU_Regs *npu, uint32_t interruptMask)
{
    return (npu->CPU_INT.IMASK & interruptMask);
}

/**
 *  @brief      Check interrupt flag of enabled NPU interrupts
 *
 *  Checks if any of the NPU interrupts that were previously enabled are
 *  pending.
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_NPU_INTERRUPT.
 *
 *  @return     Which of the requested NPU interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_NPU_INTERRUPT values
 *
 *  @sa         DL_NPU_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_NPU_getEnabledInterruptStatus(
    NPU_Regs *npu, uint32_t interruptMask)
{
    return (npu->CPU_INT.MIS & interruptMask);
}

/**
 *  @brief      Check interrupt flag of any NPU interrupt
 *
 *  Checks if any of the NPU interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_NPU_INTERRUPT.
 *
 *  @return     Which of the requested NPU interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_NPU_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_NPU_getRawInterruptStatus(
    NPU_Regs *npu, uint32_t interruptMask)
{
    return (npu->CPU_INT.RIS & interruptMask);
}

/**
 *  @brief      Get highest priority pending NPU interrupt
 *
 *  Checks if any of the NPU interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *
 *  @return     The highest priority pending NPU interrupt. One of @ref
 *              DL_NPU_IIDX
 */
__STATIC_INLINE DL_NPU_IIDX DL_NPU_getPendingInterrupt(NPU_Regs *npu)
{
    return (DL_NPU_IIDX)(npu->CPU_INT.IIDX);
}

/**
 *  @brief      Clear pending NPU interrupts
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *
 *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of
 *                             @ref DL_NPU_INTERRUPT.
 */
__STATIC_INLINE void DL_NPU_clearInterruptStatus(
    NPU_Regs *npu, uint32_t interruptMask)
{
    npu->CPU_INT.ICLR = interruptMask;
}

/**
 *  @brief      Get the physical starting address of the NPU instruction memory
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *
 *  @return     Address of the instruction memory
 */
__STATIC_INLINE uint32_t DL_NPU_getInstrMemAddress(const NPU_Regs *npu)
{
    return ((uint32_t)(&npu->DREG20));
}

/**
 *  @brief      Get the physical starting address of the NPU parameter memory
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *
 *  @return     Address of the parameter memory
 */
__STATIC_INLINE uint32_t DL_NPU_getParamMemAddress(const NPU_Regs *npu)
{
    return ((uint32_t)(&npu->DREG21));
}

/**
 *  @brief      Get the physical starting address of the NPU control registers
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *
 *  @return     Address of the NPU CTL register
 */
__STATIC_INLINE uint32_t DL_NPU_getCtlRegAddress(const NPU_Regs *npu)
{
    return ((uint32_t)(&npu->DREG0));
}

/**
 *  @brief      Get the physical address of the NPU ACC register
 *
 *  @param[in]  npu            Pointer to the register overlay for the NPU
 *
 *  @return     Address of the ACCVAL register
 */
__STATIC_INLINE uint32_t DL_NPU_getAccRegAddress(const NPU_Regs *npu)
{
    return ((uint32_t)(&npu->DREG19));
}

#ifdef __cplusplus
}
#endif

#endif /* __MSPM0_HAS_NPU__ */

#endif /* ti_dl_dl_npu__include */
/** @}*/
