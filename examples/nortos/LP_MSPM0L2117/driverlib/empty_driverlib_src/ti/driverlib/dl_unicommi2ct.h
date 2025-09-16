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
 *  @file       dl_unicommi2ct.h
 *  @brief      Unified Communication Module (UNICOMM) - Inter-Integrated Circuit Target (I2CT) Driver Library
 *  @defgroup   UNICOMMI2CT Unified Communication Module (UNICOMM) - Inter-Integrated Circuit Target (I2CT)
 *
 *  @anchor ti_dl_dl_unicommi2ct_Overview
 *  # Overview
 *
 *  Unified Communication Module Inter-Integrated Circuit Target Driver Library allows
 *  full configuration of the UNICOMM I2CT module.
 *
 *  This Inter-Integrated Circuit Target (I2CT) module provides a standardized
 *  interface to transfer data, as a target, between devices and other external
 *  devices with the I2C interface.
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup UNICOMMI2CT
 * @{
 */
#ifndef ti_dl_dl_unicommi2ct__include
#define ti_dl_dl_unicommi2ct__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>
#include <ti/driverlib/dl_unicomm.h>

#ifdef __MCU_HAS_UNICOMMI2CT__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/*!
 * @brief I2CT number of bytes which could be put into the TX FIFO
 *
 * This variable is device specific and is calculated using the system parameter
 * UNICOMMI2CT_SYS_FENTRIES defined in each devices header file.
*/
#define DL_I2CT_TX_FIFO_COUNT_MAXIMUM          ((uint32_t)UNICOMMI2CT_SYS_FENTRIES << 8)

/*!
 * @brief I2CT number of bytes which could be put into the RX FIFO
 *
 * This variable is device specific and is calculated using the system parameter
 * UNICOMMI2CT_SYS_FENTRIES defined in each devices header file.
*/
#define DL_I2CT_RX_FIFO_COUNT_MAXIMUM               ((uint32_t)UNICOMMI2CT_SYS_FENTRIES)

/** @addtogroup DL_I2CT_STATUS
 *  @{
 */

/*!
 * @brief Indicates the address for which address match happened
 */
#define DL_I2CT_STATUS_ADDRESS_MATCH             (UNICOMMI2CT_SR_ADDRMATCH_MASK)

/*!
 * @brief I2CT Stale TX FIFO
 *
 * Set = The TX FIFO data is stale. This occurs when the TX FIFO was not
 *   emptied during the previous I2CT transaction
 * Clear = The TX FIFO data is not stale
 */
#define DL_I2CT_STATUS_STALE_TX_FIFO          (UNICOMMI2CT_SR_STALE_TXFIFO_MASK)

/*!
 * @brief I2CT TX Mode
 *
 * Set = is in the TX_DATA, TX_ACK, TX_WAIT, or ADDR_ACK state with the
 *   bus direction set to read
 * Clear = is not in the TX_DATA, TX_ACK, TX_WAIT, or ADDR_ACK state
 *   with the bus direction set to read
 */
#define DL_I2CT_STATUS_TX_MODE                      (UNICOMMI2CT_SR_TXMODE_MASK)

/*!
 * @brief I2CT Bus Busy
 *
 * Set = The I2CT bus is busy. This is cleared on a timeout
 * Clear = The I2CT bus is not busy. This bit is cleared when a STOP condition is
 *   detected on the bus.
 */
#define DL_I2CT_STATUS_BUS_BUSY                     (UNICOMMI2CT_SR_BUSBSY_MASK)

/*!
 * @brief I2CT RX Mode
 *
 * Set = is in the RX_DATA, RX_ACK, RX_WAIT, RX_ACK_WAIT or ADDR_ACK
 *   state with the bus direction set to write
 * Clear = is not in the RX_DATA, RX_ACK, RX_WAIT, RX_ACK_WAIT or ADDR_ACK
 *   state with the bus direction set to write
 */
#define DL_I2CT_STATUS_RX_MODE                      (UNICOMMI2CT_SR_RXMODE_MASK)


/*!
 * @brief Direction of I2CT Quick Command
 * Only valid when QCMDST is set. @sa DL_I2CT_STATUS_QUICK_COMMAND_STATUS.
 * Set = Quick command was a read
 * Clear = Quick command was a write
 */
#define DL_I2CT_STATUS_QUICK_COMMAND_READ_WRITE     (UNICOMMI2CT_SR_QCMDRW_MASK)

/*!
 * @brief Status of I2CT Quick Command
 * @sa DL_I2CT_STATUS_QUICK_COMMAND_READ_WRITE.
 * Set = Last transaction was a Quick Comand transaction
 * Clear = Last transaction was normal or has not occurred
 */
#define DL_I2CT_STATUS_QUICK_COMMAND_STATUS         (UNICOMMI2CT_SR_QCMDST_MASK)

/*!
 * @brief I2CT Own Address Alternate Matched
 */
#define DL_I2CT_STATUS_OWN_ADDR_ALTERNATE_MATCHED  (UNICOMMI2CT_SR_OAR2SEL_MASK)

/*!
 * @brief I2CT Transmit Request
 */
#define DL_I2CT_STATUS_TRANSMIT_REQUEST               (UNICOMMI2CT_SR_TREQ_MASK)

/*!
 * @brief I2CT Receive Request
 */
#define DL_I2CT_STATUS_RECEIVE_REQUEST                (UNICOMMI2CT_SR_RREQ_MASK)

/** @}*/

/** @addtogroup DL_I2CT_INTERRUPT
 *  @{
 */
/*!
 * @brief Receive Data Interrupt (byte has been received)
 */
#define DL_I2CT_INTERRUPT_RX_DONE      (UNICOMMI2CT_CPU_INT_IMASK_RXDONE_SET)

/*!
 * @brief Transmit Transaction completed Interrupt
 */
#define DL_I2CT_INTERRUPT_TX_DONE      (UNICOMMI2CT_CPU_INT_IMASK_TXDONE_SET)

/*!
 * @brief Receive FIFO Trigger
 */
#define DL_I2CT_INTERRUPT_RXFIFO_TRIGGER                                \
                                          (UNICOMMI2CT_CPU_INT_IMASK_RXTRG_SET)

/*!
 * @brief Transmit FIFO Trigger
 */
#define DL_I2CT_INTERRUPT_TXFIFO_TRIGGER                                \
                                          (UNICOMMI2CT_CPU_INT_IMASK_TXTRG_SET)

/*!
 * @brief RX FIFO full
 */
#define DL_I2CT_INTERRUPT_RXFIFO_FULL                                   \
                                         (UNICOMMI2CT_CPU_INT_IMASK_RXFULL_SET)

/*!
 * @brief TX FIFO empty. All data in Transmit FIFO shifted out and
 * transmit goes into idle mode.
 */
#define DL_I2CT_INTERRUPT_TXFIFO_EMPTY                                  \
                                            (UNICOMMI2CT_CPU_INT_IMASK_TXEMPTY_SET)

/*!
 * @brief Start Condition detected
 */
#define DL_I2CT_INTERRUPT_START                                         \
                                              (UNICOMMI2CT_CPU_INT_IMASK_START_SET)

/*!
 * @brief Stop Condition detected
 */
#define DL_I2CT_INTERRUPT_STOP           (UNICOMMI2CT_CPU_INT_IMASK_STOP_SET)

/*!
 * @brief General Call Interrupt
 */
#define DL_I2CT_INTERRUPT_GENERAL_CALL                                  \
                                            (UNICOMMI2CT_CPU_INT_IMASK_GENCALL_SET)

/*!
 * @brief DMA Done on Event 1 Publisher
 */
#define DL_I2CT_INTERRUPT_EVENT1_DMA_DONE                               \
                                        (UNICOMMI2CT_CPU_INT_IMASK_DMA_DONE_TX_SET)

/*!
 * @brief DMA Done on Event 2 Publisher
 */
#define DL_I2CT_INTERRUPT_EVENT2_DMA_DONE                               \
                                        (UNICOMMI2CT_CPU_INT_IMASK_DMA_DONE_RX_SET)


/*!
 * @brief SMBus/PMBus PEC Receive Error Interrupt
 */
#define DL_I2CT_INTERRUPT_PEC_RX_ERROR                                  \
                                         (UNICOMMI2CT_CPU_INT_IMASK_PEC_RX_ERR_SET)

/*!
 * @brief TX FIFO Underflow Interrupt
 */
#define DL_I2CT_INTERRUPT_TXFIFO_UNDERFLOW                              \
                                            (UNICOMMI2CT_CPU_INT_IMASK_TX_UNFL_SET)

/*!
 * @brief RX FIFO Overflow Interrupt
 */
#define DL_I2CT_INTERRUPT_RXFIFO_OVERFLOW                              \
                                            (UNICOMMI2CT_CPU_INT_IMASK_RX_OVFL_SET)

/*!
 * @brief Arbitration Lost Interrupt
 */
#define DL_I2CT_INTERRUPT_ARBITRATION_LOST                              \
                                            (UNICOMMI2CT_CPU_INT_IMASK_ARBLOST_SET)

/*!
 * @brief Interrupt Overflow Interrupt. Occurs when START or STOP
 *        interrupts overflow (i.e. occurs twice without being serviced)
 */
#define DL_I2CT_INTERRUPT_OVERFLOW   (UNICOMMI2CT_CPU_INT_IMASK_RX_OVFL_SET)

/*!
 * @brief Timeout A Interrupt
 */
#define DL_I2CT_INTERRUPT_TIMEOUT_A          (UNICOMMI2CT_CPU_INT_IMASK_TIMEOUTA_SET)

/*!
 * @brief Timeout B Interrupt
 */
#define DL_I2CT_INTERRUPT_TIMEOUT_B          (UNICOMMI2CT_CPU_INT_IMASK_TIMEOUTB_SET)


/** @}*/

/** @addtogroup DL_I2CT_DMA_INTERRUPT
 *  @{
 */
/*!
 * @brief Peripheral Transmit FIFO Trigger interrupt for DMA trigger
 */
#define DL_I2CT_DMA_INTERRUPT_TXFIFO_TRIGGER                            \
                                          (UNICOMMI2CT_DMA_TRIG0_IMASK_TXTRG_SET)

/*!
 * @brief Peripheral Receive FIFO Trigger interrupt for DMA trigger
 */
#define DL_I2CT_DMA_INTERRUPT_RXFIFO_TRIGGER                            \
                                          (UNICOMMI2CT_DMA_TRIG1_IMASK_RXTRG_SET)

/** @}*/

/* clang-format on */

/** @enum DL_I2CT_EVENT_ROUTE */
typedef enum {
    /*! I2CT event route 1 */
    DL_I2CT_EVENT_ROUTE_1 = 0,
    /*! I2CT event route 2 */
    DL_I2CT_EVENT_ROUTE_2 = 12
} DL_I2CT_EVENT_ROUTE;

/** @enum DL_I2CT_CLOCK */
typedef enum {
    /*!  BUSCLK enabled as I2CT clock source */
    DL_I2CT_CLOCK_BUSCLK = UNICOMMI2CT_CLKSEL_BUSCLK_SEL_ENABLE,
    /*!  MFCLK enabled as I2CT clock source */
    DL_I2CT_CLOCK_MFCLK = UNICOMMI2CT_CLKSEL_MFCLK_SEL_ENABLE,
    /*!  Asynchronous SYSCLK enabled as I2CT clock source */
    DL_I2CT_CLOCK_ASYNC_SYSCLK = UNICOMMI2CT_CLKSEL_ASYNC_SYSCLK_SEL_ENABLE,
    /*!  Asynchronous HFCLK enabled as I2CT clock source */
    DL_I2CT_CLOCK_ASYNC_HFCLK = UNICOMMI2CT_CLKSEL_ASYNC_HFCLK_SEL_ENABLE,
    /*!  Asynchronous PLL enabled as I2CT clock source */
    DL_I2CT_CLOCK_ASYNC_PLL = UNICOMMI2CT_CLKSEL_ASYNC_PLL_SEL_ENABLE,
} DL_I2CT_CLOCK;

/** @enum DL_I2CT_CLOCK_DIVIDE */
typedef enum {
    /*!  I2CT Clock Divided by 1 */
    DL_I2CT_CLOCK_DIVIDE_1 = UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_1,
    /*!  I2CT Clock Divided by 2 */
    DL_I2CT_CLOCK_DIVIDE_2 = UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_2,
    /*!  I2CT Clock Divided by 3 */
    DL_I2CT_CLOCK_DIVIDE_3 = UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_3,
    /*!  I2CT Clock Divided by 4 */
    DL_I2CT_CLOCK_DIVIDE_4 = UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_4,
    /*!  I2CT Clock Divided by 5 */
    DL_I2CT_CLOCK_DIVIDE_5 = UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_5,
    /*!  I2CT Clock Divided by 6 */
    DL_I2CT_CLOCK_DIVIDE_6 = UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_6,
    /*!  I2CT Clock Divided by 7 */
    DL_I2CT_CLOCK_DIVIDE_7 = UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_7,
    /*!  I2CT Clock Divided by 8 */
    DL_I2CT_CLOCK_DIVIDE_8 = UNICOMMI2CT_CLKDIV_RATIO_DIV_BY_8,
} DL_I2CT_CLOCK_DIVIDE;

/** @enum DL_I2CT_ADDRESSING_MODE */
typedef enum {
    /*! Enable in 7-bit addressing mode */
    DL_I2CT_ADDRESSING_MODE_7_BIT = UNICOMMI2CT_OAR_MODE_MODE7,
    /*! Enable in 10-bit addressing mode */
    DL_I2CT_ADDRESSING_MODE_10_BIT = UNICOMMI2CT_OAR_MODE_MODE10,
} DL_I2CT_ADDRESSING_MODE;

/** @enum DL_I2CT_PEC_STATUS */
typedef enum {
    /*!  I2CT SMBus/PMBus PEC was checked in the transaction that occurred
     *   before the last Stop */
    DL_I2CT_PEC_STATUS_CHECKED = UNICOMMI2CT_PECSR_PECSTS_CHECK_SET,
    /*!  I2CT SMBus/PMBus PEC was not checked in the transaction that
     *   occurred before the last Stop */
    DL_I2CT_PEC_STATUS_NOT_CHECKED = UNICOMMI2CT_PECSR_PECSTS_CHECK_CLEARED,
} DL_I2CT_PEC_STATUS;

/** @enum DL_I2CT_PEC_CHECK_ERROR */
typedef enum {
    /*!  Indicates PEC check error did not occurr in the transaction that
     *   occurred before the last Stop */
    DL_I2CT_PEC_CHECK_ERROR_CLEARED = UNICOMMI2CT_PECSR_PECSTS_ERROR_CLEARED,
    /*!  Indicates PEC check error occurred in the transaction that
     *   occurred before the last Stop */
    DL_I2CT_PEC_CHECK_ERROR_SET = UNICOMMI2CT_PECSR_PECSTS_ERROR_SET,
} DL_I2CT_PEC_CHECK_ERROR;

/** @enum DL_I2CT_DIGITAL_GLITCH_FILTER_WIDTH */
typedef enum {
    /*!  Pulses filtering disabled.  */
    DL_I2CT_DIGITAL_GLITCH_FILTER_WIDTH_DISABLED =
        UNICOMMI2CT_GFCTL_DGFSEL_DISABLED,
    /*!  Pulses shorter than 1 functional clock tick in length are filtered.  */
    DL_I2CT_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_1 =
        UNICOMMI2CT_GFCTL_DGFSEL_CLK_1,
    /*!  Pulses shorter than 2 functional clock ticks in length are filtered.  */
    DL_I2CT_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_2 =
        UNICOMMI2CT_GFCTL_DGFSEL_CLK_2,
    /*!  Pulses shorter than 3 functional clock ticks in length are filtered.  */
    DL_I2CT_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_3 =
        UNICOMMI2CT_GFCTL_DGFSEL_CLK_3,
    /*!  Pulses shorter than 4 functional clock ticks in length are filtered.  */
    DL_I2CT_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_4 =
        UNICOMMI2CT_GFCTL_DGFSEL_CLK_4,
    /*!  Pulses shorter than 8 functional clock ticks in length are filtered.  */
    DL_I2CT_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_8 =
        UNICOMMI2CT_GFCTL_DGFSEL_CLK_8,
    /*!  Pulses shorter than 16 functional clock ticks in length are filtered.  */
    DL_I2CT_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_16 =
        UNICOMMI2CT_GFCTL_DGFSEL_CLK_16,
    /*!  Pulses shorter than 31 functional clock ticks in length are filtered.  */
    DL_I2CT_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_31 =
        UNICOMMI2CT_GFCTL_DGFSEL_CLK_31,
} DL_I2CT_DIGITAL_GLITCH_FILTER_WIDTH;

/** @enum DL_I2CT_TX_FIFO_LEVEL */
typedef enum {
    /*! Interrupt triggers when FIFO <= 3/4 empty */
    DL_I2CT_TX_FIFO_LEVEL_3_4_EMPTY = UNICOMMI2CT_IFLS_TXIFLSEL_LVL_3_4,
    /*! Interrupt triggers when FIFO <= 1/2 empty */
    DL_I2CT_TX_FIFO_LEVEL_1_2_EMPTY = UNICOMMI2CT_IFLS_TXIFLSEL_LVL_1_2,
    /*! Interrupt triggers when FIFO <= 1/4 empty */
    DL_I2CT_TX_FIFO_LEVEL_1_4_EMPTY = UNICOMMI2CT_IFLS_TXIFLSEL_LVL_1_4,
    /*! Interrupt triggers when FIFO is not full */
    DL_I2CT_TX_FIFO_LEVEL_ONE_ENTRY = UNICOMMI2CT_IFLS_TXIFLSEL_LVL_NOT_FULL,
    /*! Interrupt triggers when FIFO is empty */
    DL_I2CT_TX_FIFO_LEVEL_EMPTY = UNICOMMI2CT_IFLS_TXIFLSEL_LVL_EMPTY,
    /*! Interrupt triggers when FIFO is almost empty (FIFO <= 1) */
    DL_I2CT_TX_FIFO_LEVEL_ALMOST_EMPTY =
        UNICOMMI2CT_IFLS_TXIFLSEL_LVL_ALMOST_EMPTY,
    /*! Interrupt triggers when FIFO is almost full (FIFO >= (MAX_FIFO_LEN - 1)) */
    DL_I2CT_TX_FIFO_LEVEL_ALMOST_FULL =
        UNICOMMI2CT_IFLS_TXIFLSEL_LVL_ALMOST_FULL
} DL_I2CT_TX_FIFO_LEVEL;

/** @enum DL_I2CT_RX_FIFO_LEVEL */
typedef enum {
    /*! Interrupt triggers when FIFO >= 3/4 full */
    DL_I2CT_RX_FIFO_LEVEL_3_4_FULL = UNICOMMI2CT_IFLS_RXIFLSEL_LVL_3_4,
    /*! Interrupt triggers when FIFO >= 1/2 full */
    DL_I2CT_RX_FIFO_LEVEL_1_2_FULL = UNICOMMI2CT_IFLS_RXIFLSEL_LVL_1_2,
    /*! Interrupt triggers when FIFO >= 1/4 full */
    DL_I2CT_RX_FIFO_LEVEL_1_4_FULL = UNICOMMI2CT_IFLS_RXIFLSEL_LVL_1_4,
    /*! Interrupt triggers when FIFO is not empty */
    DL_I2CT_RX_FIFO_LEVEL_ONE_ENTRY = UNICOMMI2CT_IFLS_RXIFLSEL_LVL_NOT_EMPTY,
    /*! Interrupt triggers when FIFO is full */
    DL_I2CT_RX_FIFO_LEVEL_FULL = UNICOMMI2CT_IFLS_RXIFLSEL_LVL_FULL,
    /*! Interrupt triggers when FIFO is almost full (FIFO >= (MAX_FIFO_LEN - 1)) */
    DL_I2CT_RX_FIFO_LEVEL_ALMOST_FULL =
        UNICOMMI2CT_IFLS_RXIFLSEL_LVL_ALMOST_FULL,
    /*! Interrupt triggers when FIFO is almost empty (FIFO <= 1) */
    DL_I2CT_RX_FIFO_LEVEL_ALMOST_EMPTY =
        UNICOMMI2CT_IFLS_RXIFLSEL_LVL_ALMOST_EMPTY
} DL_I2CT_RX_FIFO_LEVEL;

/** @enum DL_I2CT_RESPONSE_OVERRIDE_VALUE */
typedef enum {
    /*!  manual ACK for valid data or command.  */
    DL_I2CT_RESPONSE_OVERRIDE_VALUE_ACK = UNICOMMI2CT_ACKCTL_ACKOVAL_DISABLE,
    /*!  manual NACK for invalid data or command.  */
    DL_I2CT_RESPONSE_OVERRIDE_VALUE_NACK = UNICOMMI2CT_ACKCTL_ACKOVAL_ENABLE,
} DL_I2CT_RESPONSE_OVERRIDE_VALUE;

/*! @enum DL_I2CT_IIDX */
typedef enum {
    /*! Interrupt index for I2CT if no interrupt is pending */
    DL_I2CT_IIDX_NO_INT = UNICOMMI2CT_IIDX_STAT_NO_INTR,
    /*! Interrupt index for I2CT Timeout A Event */
    DL_I2CT_IIDX_TIMEOUT_A = UNICOMMI2CT_IIDX_STAT_TIMEOUTA,
    /*! Interrupt index for I2CT Timeout B Event */
    DL_I2CT_IIDX_TIMEOUT_B = UNICOMMI2CT_IIDX_STAT_TIMEOUTB,

    /*! Interrupt index for I2CT Receive Data */
    DL_I2CT_IIDX_RX_DONE = UNICOMMI2CT_IIDX_STAT_RXDONEFG,
    /*! Interrupt index for I2CT Transmit Transaction completed */
    DL_I2CT_IIDX_TX_DONE = UNICOMMI2CT_IIDX_STAT_TXDONEFG,
    /*! Interrupt index for I2CT Receive FIFO Trigger */
    DL_I2CT_IIDX_RXFIFO_TRIGGER = UNICOMMI2CT_IIDX_STAT_RXTRG,
    /*! Interrupt index for I2CT Transmit FIFO Trigger */
    DL_I2CT_IIDX_TXFIFO_TRIGGER = UNICOMMI2CT_IIDX_STAT_TXTRG,
    /*! Interrupt index for I2CT RX FIFO full */
    DL_I2CT_IIDX_RXFIFO_FULL = UNICOMMI2CT_IIDX_STAT_RXFULL,
    /*! Interrupt index for I2CT TX FIFO empty.
     * All data in Transmit FIFO shifted out and transmit goes into idle mode. */
    DL_I2CT_IIDX_TXFIFO_EMPTY = UNICOMMI2CT_IIDX_STAT_TXEMPTY,
    /*! Interrupt index for I2CT Start Condition detected */
    DL_I2CT_IIDX_START = UNICOMMI2CT_IIDX_STAT_STARTFG,
    /*! Interrupt index for I2CT Stop Condition detected */
    DL_I2CT_IIDX_STOP = UNICOMMI2CT_IIDX_STAT_STOPFG,
    /*! Interrupt index for I2CT General Call */
    DL_I2CT_IIDX_GENERAL_CALL = UNICOMMI2CT_IIDX_STAT_GENCALL,
    /*! Interrupt index for I2CT Event 1 DMA Done */
    DL_I2CT_IIDX_EVENT1_DMA_DONE = UNICOMMI2CT_IIDX_STAT_DMA_DONE_TX,
    /*! Interrupt index for I2CT Event 2 DMA Done */
    DL_I2CT_IIDX_EVENT2_DMA_DONE = UNICOMMI2CT_IIDX_STAT_DMA_DONE_RX,
    /*! Interrupt index for I2CT receive pre-emptive interrupt */
    DL_I2CT_IIDX_PREEMPT_INT_TX = UNICOMMI2CT_IIDX_STAT_DMA_PREIRQ_TX,
    /*! Interrupt index for I2CT receive pre-emptive interrupt */
    DL_I2CT_IIDX_PREEMPT_INT_RX = UNICOMMI2CT_IIDX_STAT_DMA_PREIRQ_RX,
    /*! Interrupt index for I2CT SMBus/PMBus PEC Receive Error Event */
    DL_I2CT_IIDX_PEC_RX_ERROR = UNICOMMI2CT_IIDX_STAT_PEC_RX_ERR,
    /*! Interrupt index for I2CT TX FIFO underflow event */
    DL_I2CT_IIDX_TXFIFO_UNDERFLOW = UNICOMMI2CT_IIDX_STAT_TX_UNFL,
    /*! Interrupt index for I2CT RX FIFO overflow event */
    DL_I2CT_IIDX_RXFIFO_OVERFLOW = UNICOMMI2CT_IIDX_STAT_RX_OVFL,
    /*! Interrupt index for I2CT arbitration lost event */
    DL_I2CT_IIDX_ARBITRATION_LOST = UNICOMMI2CT_IIDX_STAT_ARBLOSTFG,

} DL_I2CT_IIDX;

/**
 * @brief  Configuration struct for @ref DL_I2CT_setClockConfig.
 */
typedef struct {
    /*! I2CT module clock source. One of @ref DL_I2CT_CLOCK */
    DL_I2CT_CLOCK clockSel;
    /*! I2CT clock divider selection. One of @ref DL_I2CT_CLOCK_DIVIDE */
    DL_I2CT_CLOCK_DIVIDE divideRatio;
} DL_I2CT_ClockConfig;

/**
 *  @brief      Configure I2CT source clock
 *
 *  @param[in]  unicomm    Pointer to the register overlay for the
 *                      peripheral
 *  @param[in]  config  Pointer to the clock configuration struct
 *                       @ref DL_I2CT_ClockConfig.
 */
void DL_I2CT_setClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_ClockConfig *config);

/**
 *  @brief      Get I2CT source clock configuration
 *
 *  @param[in]  unicomm    Pointer to the register overlay for the
 *                      peripheral
 *  @param[in]  config  Pointer to the clock configuration struct
 *                      @ref DL_I2CT_ClockConfig.
 */
void DL_I2CT_getClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_ClockConfig *config);

/**
 *  @brief      Checks if TX FIFO is full
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If TX FIFO is full
 *
 *  @retval     true if TX FIFO is full
 *  @retval     false if TX FIFO is not full
 */
__STATIC_INLINE bool DL_I2CT_isTXFIFOFull(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->SR & UNICOMMI2CT_SR_TXFF_MASK) ==
            UNICOMMI2CT_SR_TXFF_SET);
}

/**
 *  @brief      Checks if TX FIFO is empty
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If TX FIFO is empty
 *
 *  @retval     true if TX FIFO is empty
 *  @retval     false if TX FIFO is not empty
 */
__STATIC_INLINE bool DL_I2CT_isTXFIFOEmpty(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->SR & UNICOMMI2CT_SR_TXFE_MASK) ==
            UNICOMMI2CT_SR_TXFE_SET);
}

/**
 *  @brief      Checks if RX FIFO is full
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If RX FIFO is full
 *
 *  @retval     true if RX FIFO is full
 *  @retval     false if RX FIFO is not full
 */
__STATIC_INLINE bool DL_I2CT_isRXFIFOFull(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->SR & UNICOMMI2CT_SR_RXFF_MASK) ==
            UNICOMMI2CT_SR_RXFF_SET);
}

/**
 *  @brief      Checks if RX FIFO is empty
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If RX FIFO is empty
 *
 *  @retval     true if RX FIFO is empty
 *  @retval     false if RX FIFO is not empty
 */
__STATIC_INLINE bool DL_I2CT_isRXFIFOEmpty(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->SR & UNICOMMI2CT_SR_RXFE_MASK) ==
            UNICOMMI2CT_SR_RXFE_SET);
}

/**
 *  @brief      Fills the TX FIFO with data
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  buffer  Pointer to buffer of bytes
 *  @param[in]  count   Number of bytes to fill TX FIFO from buffer
 *
 *  @return     Number of bytes that were successfully written
 */
uint8_t DL_I2CT_fillTXFIFO(
    UNICOMM_Inst_Regs *unicomm, uint8_t *buffer, uint8_t count);

/**
 *  @brief      Flushes/removes all elements in the TX FIFO
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
void DL_I2CT_flushTXFIFO(UNICOMM_Inst_Regs *unicomm);

/**
 *  @brief      Flushes/removes all elements in the RX FIFO
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
void DL_I2CT_flushRXFIFO(UNICOMM_Inst_Regs *unicomm);

/**
 *  @brief      Stop TX FIFO flush
 *
 *  Before stopping the flush, check if @ref DL_I2CT_isTXFIFOEmpty,
 *  indicating flush is complete.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_stopFlushTXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->IFLS &= ~(UNICOMMI2CT_IFLS_TXCLR_MASK);
}

/**
 *  @brief      Start TX FIFO flush
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_startFlushTXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->IFLS |= UNICOMMI2CT_IFLS_TXCLR_MASK;
}

/**
 *  @brief      Stop RX FIFO flush
 *
 *  Before stopping the flush, check if @ref DL_I2CT_isRXFIFOEmpty,
 *  indicating flush is complete.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_stopFlushRXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->IFLS &= ~(UNICOMMI2CT_IFLS_RXCLR_MASK);
}

/**
 *  @brief      Start RX FIFO flush
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_startFlushRXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->IFLS |= UNICOMMI2CT_IFLS_RXCLR_MASK;
}

/**
 *  @brief  Transmit data, waiting until transmit request
 *
 *  @note   Setting own addresses and enabling should be done
 *  separately.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  data    Byte of data to transmit
 */
void DL_I2CT_transmitDataBlocking(UNICOMM_Inst_Regs *unicomm, uint8_t data);

/**
 *  @brief  Transmit data
 *
 *  @note   Setting own addresses and enabling should be done
 *  separately.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  data    Byte of data to transmit
 *
 *  @return Whether data could be transmitted or not
 *  @retval true if data could be transmitted
 *  @retval false if data could not be transmitted
 */
bool DL_I2CT_transmitDataCheck(UNICOMM_Inst_Regs *unicomm, uint8_t data);

/**
 *  @brief  Receive data, waiting until receive request
 *
 *  @note   Setting own addresses and enabling should be done
 *  separately.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return Data received from
 */
uint8_t DL_I2CT_receiveDataBlocking(UNICOMM_Inst_Regs *unicomm);

/**
 *  @brief  Receive data
 *
 *  @note   Setting own addresses and enabling should be done
 *  separately.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  buffer  Pointer to where byte of received data should be stored
 *
 *  @return Whether data could be received or not
 *  @retval true if data could be received
 *  @retval false if data could not be received
 */
bool DL_I2CT_receiveDataCheck(UNICOMM_Inst_Regs *unicomm, uint8_t *buffer);

/**
 * @brief Enables power on UNICOMM I2C module
 *
 * @param[in] unicomm        Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_enablePower(UNICOMM_Inst_Regs *unicomm)
{
    DL_UNICOMM_enablePower(unicomm);
    if (unicomm->fixedMode == false) {
        DL_UNICOMM_setIPMode(unicomm, DL_UNICOMM_I2C_TARGET);
    }
}

/**
 * @brief Disables power on UNICOMM I2C module
 *
 * @param[in] unicomm        Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disablePower(UNICOMM_Inst_Regs *unicomm)
{
    DL_UNICOMM_disablePower(unicomm);
}

/**
 * @brief Returns if  power on UNICOMM I2C module
 *
 * @param[in] unicomm        Pointer to the register overlay for the peripheral
 *
 * @return true if power is enabled
 * @return false if power is disabled
 */
__STATIC_INLINE bool DL_I2CT_isPowerEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return DL_UNICOMM_isPowerEnabled(unicomm);
}

/**
 * @brief Resets UNICOMM I2C peripheral
 *
 * @param[in] unicomm        Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_reset(UNICOMM_Inst_Regs *unicomm)
{
    DL_UNICOMM_reset(unicomm);
}

/**
 * @brief Returns if UNICOMM I2C peripheral was reset
 *
 * @param[in] unicomm        Pointer to the register overlay for the peripheral
 *
 * @return true if peripheral was reset
 * @return false if peripheral wasn't reset
 *
 */
__STATIC_INLINE bool DL_I2CT_isReset(UNICOMM_Inst_Regs *unicomm)
{
    return DL_UNICOMM_isReset(unicomm);
}

/**
 * @brief Set Clock Source
 *
 *  Clock sources depend on device and clock should be enabled
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  clockSource    One of @ref DL_I2CT_CLOCK.
 *
 */
__STATIC_INLINE void DL_I2CT_selectClockSource(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_CLOCK clockSource)
{
    DL_Common_updateReg(&unicomm->i2ct->CLKSEL, (uint32_t) clockSource,
        UNICOMMI2CT_CLKSEL_BUSCLK_SEL_MASK |
            UNICOMMI2CT_CLKSEL_MFCLK_SEL_MASK);
}

/**
 * @brief Set Clock Divider
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  clockDivider   One of @ref DL_I2CT_CLOCK_DIVIDE.
 *
 */
__STATIC_INLINE void DL_I2CT_selectClockDivider(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_CLOCK_DIVIDE clockDivider)
{
    DL_Common_updateReg(&unicomm->i2ct->CLKDIV, (uint32_t) clockDivider,
        UNICOMMI2CT_CLKDIV_RATIO_MASK);
}

/**
 *  @brief      Get Digital Glitch Suppression Pulse Width
 *
 *  Pulse width for the digital glitch suppression on SCL/SDA lines.
 *  Values are in terms of functional clock ticks.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     Digital glitch suppression pulse width
 *
 *  @retval     One of @ref DL_I2CT_DIGITAL_GLITCH_FILTER_WIDTH.
 */
__STATIC_INLINE DL_I2CT_DIGITAL_GLITCH_FILTER_WIDTH
DL_I2CT_getDigitalGlitchFilterPulseWidth(UNICOMM_Inst_Regs *unicomm)
{
    uint32_t filterWidth =
        unicomm->i2ct->GFCTL & UNICOMMI2CT_GFCTL_DGFSEL_MASK;

    return (DL_I2CT_DIGITAL_GLITCH_FILTER_WIDTH)(filterWidth);
}

/**
 *  @brief      Set Digital Glitch Suppression Pulse Width
 *
 *  Pulse width for the digital glitch suppression on SCL/SDA lines.
 *  Values are in terms of functional clock ticks.
 *
 *  @param[in]  unicomm             Pointer to the register overlay for the
 *                              peripheral
 *  @param[in]  filterWidth     One of @ref DL_I2CT_DIGITAL_GLITCH_FILTER_WIDTH.
 */
__STATIC_INLINE void DL_I2CT_setDigitalGlitchFilterPulseWidth(
    UNICOMM_Inst_Regs *unicomm,
    DL_I2CT_DIGITAL_GLITCH_FILTER_WIDTH filterWidth)
{
    DL_Common_updateReg(&unicomm->i2ct->GFCTL, (uint32_t) filterWidth,
        UNICOMMI2CT_GFCTL_DGFSEL_MASK);
}

/**
 *  @brief      Disable Analog Glitch Suppression
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableAnalogGlitchFilter(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->GFCTL &= ~(UNICOMMI2CT_GFCTL_AGFEN_MASK);
}

/**
 *  @brief      Checks if analog glitch suppression is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If analog glitch suppression is enabled
 *
 *  @retval     true if analog glitch suppression is enabled
 *  @retval     false if analog glitch suppression is disabled
 */
__STATIC_INLINE bool DL_I2CT_isAnalogGlitchFilterEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->GFCTL & UNICOMMI2CT_GFCTL_AGFEN_MASK) ==
            UNICOMMI2CT_GFCTL_AGFEN_ENABLE);
}

/**
 *  @brief      Enable Analog Glitch Suppression
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_enableAnalogGlitchFilter(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->GFCTL |= UNICOMMI2CT_GFCTL_AGFEN_ENABLE;
}

/**
 *  @brief      Set own address
 *
 * This field specifies bits A9 through A0 of the own address.
 *
 * In 7-bit addressing mode as selected by @ref DL_I2CT_setAddressingMode,
 * the top 3 bits are don't care
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  addr    Value of own address [0x00, 0x3FF]
 */
__STATIC_INLINE void DL_I2CT_setOwnAddress(
    UNICOMM_Inst_Regs *unicomm, uint32_t addr)
{
    DL_Common_updateReg(&unicomm->i2ct->OAR, addr, UNICOMMI2CT_OAR_OAR_MASK);
}

/**
 *  @brief      Get own address
 *
 * Get bits A9 through A0 of the own address.
 *
 * In 7-bit addressing mode as selected by @ref DL_I2CT_setAddressingMode,
 * the top 3 bits are don't care
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @return     own address
 *
 *  @retval     [0x00, 0x3FF]
 */
__STATIC_INLINE uint32_t DL_I2CT_getOwnAddress(UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->i2ct->OAR & UNICOMMI2CT_OAR_OAR_MASK);
}

/**
 *  @brief      Enable own address
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @sa DL_I2CT_setOwnAddress
 */
__STATIC_INLINE void DL_I2CT_enableOwnAddress(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->OAR |= UNICOMMI2CT_OAR_OAREN_ENABLE;
}

/**
 *  @brief      Disable own address
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableOwnAddress(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->OAR &= ~(UNICOMMI2CT_OAR_OAR_MASK);
}

/**
 *  @brief      Checks if own address is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If own address is enabled
 *
 *  @retval     true if own address is enabled
 *  @retval     false if own address is disabled
 */
__STATIC_INLINE bool DL_I2CT_isOwnAddressEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->OAR & UNICOMMI2CT_OAR_OAREN_MASK) ==
            UNICOMMI2CT_OAR_OAREN_ENABLE);
}

/**
 *  @brief      Set addressing mode
 *
 *  Selects the addressing mode to be used when device is configured as a
 *
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  mode    Addressing mode to set the to.
 *                      One of @ref DL_I2CT_ADDRESSING_MODE.
 */
__STATIC_INLINE void DL_I2CT_setAddressingMode(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_ADDRESSING_MODE mode)
{
    DL_Common_updateReg(
        &unicomm->i2ct->OAR, (uint32_t) mode, UNICOMMI2CT_OAR_MODE_MASK);
}

/**
 *  @brief      Get addressing mode
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     Addressing mode the is set to
 *
 *  @retval     One of @ref DL_I2CT_ADDRESSING_MODE
 */
__STATIC_INLINE DL_I2CT_ADDRESSING_MODE DL_I2CT_getAddressingMode(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t mode = unicomm->i2ct->OAR & UNICOMMI2CT_OAR_MODE_MASK;

    return (DL_I2CT_ADDRESSING_MODE)(mode);
}

/**
 *  @brief      Get own address alternate
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @return     own address alternate
 *
 *  @retval     [0x00, 0x7f]
 */
__STATIC_INLINE uint32_t UNICOMMI2CT_getOwnAddressAlternate(
    UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->i2ct->OAR2 & UNICOMMI2CT_OAR2_OAR2_MASK);
}

/**
 *  @brief      Set own address alternate
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  addr    Value of own address alternate [0x00, 0x7f]
 */
__STATIC_INLINE void DL_I2CT_setOwnAddressAlternate(
    UNICOMM_Inst_Regs *unicomm, uint32_t addr)
{
    DL_Common_updateReg(
        &unicomm->i2ct->OAR2, addr, UNICOMMI2CT_OAR2_OAR2_MASK);
}

/**
 *  @brief      Get own address alternate mask
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     Which bits of the address are matched
 *
 *  @retval     Bit mask with each bit corresponding to bits A6 through A0 of
 *              the address. Value between [0x00, 0x7F]
 */
__STATIC_INLINE uint32_t DL_I2CT_getOwnAddressAlternateMask(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->OAR2 & UNICOMMI2CT_OAR2_OAR2_MASK_MASK) >>
            UNICOMMI2CT_OAR2_OAR2_MASK_OFS);
}

/**
 *  @brief      Set own address alternate mask
 *
 * This field specifies bits A6 through A0 of the address. The bits with
 * a value of 1 in the SOAR2.OAR2_MASK field will make the corresponding
 * incoming address bits to match by default regardless of the value inside
 * SOAR2.OAR2 i.e. corresponding SOAR2.OAR2 bit is a don't care.
 *
 *  @param[in]  unicomm           Pointer to the register overlay for the peripheral
 *  @param[in]  addressMask   Bit mask of address bits to match.
 *                            Value between [0x00, 0x7F]
 */
__STATIC_INLINE void DL_I2CT_setOwnAddressAlternateMask(
    UNICOMM_Inst_Regs *unicomm, uint32_t addressMask)
{
    DL_Common_updateReg(&unicomm->i2ct->OAR2,
        addressMask << UNICOMMI2CT_OAR2_OAR2_MASK_OFS,
        UNICOMMI2CT_OAR2_OAR2_MASK_MASK);
}

/**
 *  @brief      Disable usage of own address alternate
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableOwnAddressAlternate(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->OAR2 &= ~(UNICOMMI2CT_OAR2_OAR2EN_MASK);
}

/**
 *  @brief      Checks if own address alternate is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If own address alternate is enabled
 *
 *  @retval     true if own address alternate is enabled
 *  @retval     false if own address alternate is disabled
 */
__STATIC_INLINE bool DL_I2CT_isOwnAddressAlternateEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->OAR2 & UNICOMMI2CT_OAR2_OAR2EN_MASK) ==
            UNICOMMI2CT_OAR2_OAR2EN_ENABLE);
}

/**
 *  @brief      Enable usage of own address alternate
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_enableOwnAddressAlternate(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->OAR2 |= UNICOMMI2CT_OAR2_OAR2EN_ENABLE;
}

/**
 *  @brief      Get the address for which address match happened
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     Matched address
 *
 *  @retval     Bit mask with each bit corresponding to the address.
 *  Value between [0x00, 0x7F] in 7-bit mode, or [0x00, 0x3FF] in 10-bit mode.
 */
__STATIC_INLINE uint32_t DL_I2CT_getAddressMatch(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->SR & UNICOMMI2CT_SR_ADDRMATCH_MASK) >>
            UNICOMMI2CT_SR_ADDRMATCH_OFS);
}

/**
 *  @brief      Disable clock stretching
 *
 *  Clock stretching should be enabled to be compliant with I2CT specification
 *  and the SCL will be stretched when data must be read or written from the
 *  FIFO. It can optionally be disabled if a controller does not support it or
 *  to achieve maximum I2CT frequency; however the developer should ensure that
 *  the FIFO is written or read on time.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableClockStretching(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR &= ~(UNICOMMI2CT_CTR_CLKSTRETCH_MASK);
}

/**
 *  @brief      Checks if clock stretching is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If clock stretching is enabled
 *
 *  @retval     true if clock stretching is enabled
 *  @retval     false if clock stretching is disabled
 */
__STATIC_INLINE bool DL_I2CT_isClockStretchingEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->CTR & UNICOMMI2CT_CTR_CLKSTRETCH_MASK) ==
            UNICOMMI2CT_CTR_CLKSTRETCH_ENABLE);
}

/**
 *  @brief      Enable clock stretching
 *
 *  Clock stretching should be enabled to be compliant with I2CT specification
 *  and the SCL will be stretched when data must be read or written from the
 *  FIFO. It can optionally be disabled if a controller does not support it or
 *  to achieve maximum I2CT frequency; however the developer should ensure that
 *  the FIFO is written or read on time.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_enableClockStretching(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR |= UNICOMMI2CT_CTR_CLKSTRETCH_ENABLE;
}

/**
 *  @brief      Disable TX empty interrupt on transmit request
 *
 * When disabled, RIS.STXEMPTY will be set when only the TX FIFO is
 * empty. This allows the STXEMPTY interrupt to be used to indicate that the
 * I2CT bus is being clock stretched and that TX data is required.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @sa         DL_I2CT_isClockStretchEnabled
 */
__STATIC_INLINE void DL_I2CT_disableTXEmptyOnTXRequest(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR &= ~(UNICOMMI2CT_CTR_TXEMPTY_ON_TREQ_MASK);
}

/**
 *  @brief      Checks if TX empty interrupt on transmit request is
 *              enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If TX empty interrupt on transmit request is enabled
 *
 *  @retval     true if TX empty interrupt on transmit request is enabled
 *  @retval     false if TX empty interrupt on transmit request disabled
 */
__STATIC_INLINE bool DL_I2CT_isTXEmptyOnTXRequestEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->CTR & UNICOMMI2CT_CTR_TXEMPTY_ON_TREQ_MASK) ==
            UNICOMMI2CT_CTR_TXEMPTY_ON_TREQ_ENABLE);
}

/**
 *  @brief      Enable TX empty interrupt on transmit request
 *
 *  When enabled, RIS.STXEMPTY will be set when the state machine is in
 *  the TX_WAIT state which occurs when the TX FIFO is empty AND the UNICOMMI2CT
 *  transaction is clock stretched waiting for the FIFO to receive data.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_enableTXEmptyOnTXRequest(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR |= UNICOMMI2CT_CTR_TXEMPTY_ON_TREQ_ENABLE;
}

/**
 *  @brief      Disable TX trigger in TX mode
 *
 * When disabled, RIS.TXFIFOTRG will only be set when the TX FIFO is at
 * or above the trigger level.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableTXTriggerInTXMode(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR &= ~(UNICOMMI2CT_CTR_TXTRIG_TXMODE_MASK);
}

/**
 *  @brief      Checks if TX trigger in TX mode is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If TX trigger in TX mode is enabled
 *
 *  @retval     true if TX trigger in TX mode is enabled
 *  @retval     false if TX trigger in TX mode is disabled
 */
__STATIC_INLINE bool DL_I2CT_isTXTriggerInTXModeEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->CTR & UNICOMMI2CT_CTR_TXTRIG_TXMODE_MASK) ==
            UNICOMMI2CT_CTR_TXTRIG_TXMODE_ENABLE);
}

/**
 *  @brief      Enable TX trigger when is in TX mode
 *
 *  When enabled, RIS.TXFIFOTRG will be set when the TX FIFO has reached
 *  the trigger level AND the state machine is in the TXMODE as defined
 *  in the SSR register. When cleared RIS.TXFIFOTRG will be set when the
 *  TX FIFO is at or above the trigger level. This setting can be used to hold
 *  off the TX DMA until a transaction starts. This allows the DMA to be
 *  configured when the I2CT is idle but have it wait till the transaction
 *  starts to load the TX FIFO, so it can load from a memory buffer that
 *  might be changing over time.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @sa                 DL_I2CT_INTERRUPT_TXFIFO_TRIGGER
 */
__STATIC_INLINE void DL_I2CT_enableTXTriggerInTXMode(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR |= UNICOMMI2CT_CTR_TXTRIG_TXMODE_ENABLE;
}

/**
 *  @brief      Disable TX transfer waits when stale data in TX FIFO
 *
 *  When disabled, the TX FIFO empty signal to the state machine
 *  indicates that the TX FIFO is empty.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableTXWaitWhenTXFIFOStale(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR &= ~(UNICOMMI2CT_CTR_TXWAIT_STALE_TXFIFO_MASK);
}

/**
 *  @brief      Checks if TX transfer waits when stale data in TX FIFO
 *              is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If TX transfer waits when stale data in TX FIFO is
 *              enabled
 *
 *  @retval     true   if TX transfer waits when stale data in TX FIFO
 *                     is enabled
 *  @retval     false  if TX transfer waits when stale data in TX FIFO
 *                     is disabled
 */
__STATIC_INLINE bool DL_I2CT_isTXWaitWhenTXFIFOStaleEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->CTR & UNICOMMI2CT_CTR_TXWAIT_STALE_TXFIFO_MASK) ==
            UNICOMMI2CT_CTR_TXWAIT_STALE_TXFIFO_ENABLE);
}

/**
 *  @brief      Enable TX transfer waits when stale data in TX FIFO
 *
 *  When enabled, the TX FIFO empty signal to the state machine will
 *  indicate that the TX FIFO is empty or that the TX FIFO data is stale. The
 *  TX FIFO data is determined to be stale when there is data in the TX FIFO
 *  when the state machine leaves the TXMODE as defined in the SSR
 *  register. This can occur is a Stop or timeout occur when there are bytes
 *  left in the TX FIFO.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_enableTXWaitWhenTXFIFOStale(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR |= UNICOMMI2CT_CTR_TXWAIT_STALE_TXFIFO_ENABLE;
}

/**
 *  @brief      Disable RX full interrupt on receive request
 *
 *  When disabled, RIS.SRXFULL will be set when only the RX FIFO is
 *  full. This allows the SRXFULL interrupt to be used to indicate that the
 *  I2CT bus is being clock stretched and that the FW must either read the RX
 *  FIFO or ACK/NACK the current RX byte.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @sa         DL_I2CT_isClockStretchEnabled
 */
__STATIC_INLINE void DL_I2CT_disableRXFullOnRXRequest(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR &= ~(UNICOMMI2CT_CTR_RXFULL_ON_RREQ_MASK);
}

/**
 *  @brief      Checks if RX full interrupt on receive request is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If RX full interrupt on receive request enabled
 *
 *  @retval     true   if RX full interrupt on receive request enabled
 *  @retval     false  if RX full interrupt on receive request disabled
 */
__STATIC_INLINE bool DL_I2CT_isRXFullOnRXRequestEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->CTR & UNICOMMI2CT_CTR_RXFULL_ON_RREQ_MASK) ==
            UNICOMMI2CT_CTR_RXFULL_ON_RREQ_ENABLE);
}

/**
 *  @brief      Enable RX full interrupt on receive request
 *
 *  When enabled, RIS.SRXFULL will be set when the state machine is in
 *  the RX_WAIT or RX_ACK_WAIT states which occurs when the I2CT transaction is
 *  clock stretched because the RX FIFO is full or the ACKOEN has been set and
 *  the state machine is waiting for FW to ACK/NACK the current byte.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 */
__STATIC_INLINE void DL_I2CT_enableRXFullOnRXRequest(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR |= UNICOMMI2CT_CTR_RXFULL_ON_RREQ_ENABLE;
}

/**
 *  @brief      Disable SMBus/PMBus default host address of 000 1000b
 *
 *  When disabled, the default host address is not matched.
 *  NOTE: The host address may still be matched if programmed as an own address.
 *    The I2CT module can still be addressed as a if enabled and own
 *  address match is enabled.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableDefaultHostAddress(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR &= ~(UNICOMMI2CT_CTR_EN_DEFHOSTADR_MASK);
}

/**
 *  @brief      Checks if SMBus/PMBus default host address of 000 1000b is
 *              enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If default host address is enabled
 *
 *  @retval     true   if default host address is enabled
 *  @retval     false  if default host address disabled
 */
__STATIC_INLINE bool DL_I2CT_isDefaultHostAddressEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->CTR & UNICOMMI2CT_CTR_EN_DEFHOSTADR_MASK) ==
            UNICOMMI2CT_CTR_EN_DEFHOSTADR_ENABLE);
}

/**
 *  @brief      Enable SMBus/PMBus default host address of 000 1000b
 *
 *  When enabled, default host address of 0x0001000 is always matched by the
 *  address match logic
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_enableDefaultHostAddress(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR |= UNICOMMI2CT_CTR_EN_DEFHOSTADR_ENABLE;
}

/**
 *  @brief      Disable SMBus/PMBus Alert response address (ARA) of 000 1100b
 *
 *  When disabled, the alert response address 0x0001100 is not matched.
 *  NOTE: The alert response address may still be matched if programmed as an
 *  own address.
 *    The I2CT module can still be addressed as a if enabled and own
 *  address match is enabled.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableAlertResponseAddress(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR &= ~(UNICOMMI2CT_CTR_EN_ALRESPADR_MASK);
}

/**
 *  @brief      Checks if SMBus/PMBus Alert response address (ARA) of 000 1100b
 * is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If alert response address is enabled
 *
 *  @retval     true   if alert response address is enabled
 *  @retval     false  if alert response address disabled
 */
__STATIC_INLINE bool DL_I2CT_isAlertResponseAddressEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->CTR & UNICOMMI2CT_CTR_EN_ALRESPADR_MASK) ==
            UNICOMMI2CT_CTR_EN_ALRESPADR_ENABLE);
}

/**
 *  @brief      Enable SMBus/PMBus Alert response address (ARA) of 000 1100b
 *
 *  When enabled, default alert response address of 0x0001100 is always matched
 *  by the address match logic
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_enableAlertResponseAddress(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR |= UNICOMMI2CT_CTR_EN_ALRESPADR_ENABLE;
}

/**
 *  @brief      Disable SMBus/PMBus default device address of 110 0001b
 *
 *  Used for Address Resolution Protocol. When disabled, the default device
 *  address is not matched.
 *  NOTE: The host address may still be matched if programmed as an own address.
 *    The I2CT module can still be addressed as a if enabled and own
 *  address match is enabled.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableDefaultDeviceAddress(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR &= ~(UNICOMMI2CT_CTR_EN_DEFDEVADR_MASK);
}

/**
 *  @brief      Checks SMBus/PMBus default device address of 110 0001b is
 *              enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If default device address is enabled
 *
 *  @retval     true   if default device address is enabled
 *  @retval     false  if default device address disabled
 */
__STATIC_INLINE bool DL_I2CT_isDefaultDeviceAddressEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->CTR & UNICOMMI2CT_CTR_EN_DEFDEVADR_MASK) ==
            UNICOMMI2CT_CTR_EN_DEFDEVADR_ENABLE);
}

/**
 *  @brief      Enable SMBus/PMBus default device address of 110 0001b
 *
 *  Used for Address Resolution Protocol. When enabled, default device address
 *  of 110 0001b is always matched by the address match logic.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_enableDefaultDeviceAddress(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR |= UNICOMMI2CT_CTR_EN_DEFDEVADR_ENABLE;
}

/**
 *  @brief      Disable wakeup
 *
 *  When disabled, the is not allowed to clock stretch on START
 *  detection.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableWakeup(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR &= ~(UNICOMMI2CT_CTR_WUEN_MASK);
}

/**
 *  @brief      Checks if wakeup is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If wakeup is enabled
 *
 *  @retval     true   if wakeup is enabled
 *  @retval     false  if wakeup disabled
 */
__STATIC_INLINE bool DL_I2CT_isWakeupEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->CTR & UNICOMMI2CT_CTR_WUEN_MASK) ==
            UNICOMMI2CT_CTR_WUEN_ENABLE);
}

/**
 *  @brief      Enable wakeup
 *
 *  Enables low-power wake-up, however it is recommended to enable clock
 *  stretching to stretch the clock while the module is waking up.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @sa         DL_I2CT_enableClockStretching
 */
__STATIC_INLINE void DL_I2CT_enableWakeup(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR |= UNICOMMI2CT_CTR_WUEN_ENABLE;
}

/**
 *  @brief      Disable target
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disable(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR &= ~(UNICOMMI2CT_CTR_ENABLE_MASK);
}

/**
 *  @brief      Checks if target is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If functionality is enabled
 *
 *  @retval     true if functionality is enabled
 *  @retval     false if functionality is disabled
 */
__STATIC_INLINE bool DL_I2CT_isEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->CTR & UNICOMMI2CT_CTR_ENABLE_MASK) ==
            UNICOMMI2CT_CTR_ENABLE_ENABLE);
}

/**
 *  @brief      Enable target
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_enable(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR |= UNICOMMI2CT_CTR_ENABLE_ENABLE;
}

/**
 *  @brief      Disable general call address of 000 0000b
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableGeneralCall(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR &= ~(UNICOMMI2CT_CTR_GENCALL_MASK);
}

/**
 *  @brief      Checks if general call address of 000 0000b is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If general call is enabled
 *
 *  @retval     true if general call is enabled
 *  @retval     false if general call is disabled
 */
__STATIC_INLINE bool DL_I2CT_isGeneralCallEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->CTR & UNICOMMI2CT_CTR_GENCALL_MASK) ==
            UNICOMMI2CT_CTR_GENCALL_ENABLE);
}

/**
 *  @brief      Enable usage of general call address of 000 0000b
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_enableGeneralCall(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->CTR |= UNICOMMI2CT_CTR_GENCALL_ENABLE;
}

/**
 *  @brief      Get status of I2CT bus controller for target
 *
 *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
 *
 *  @return     Status of I2CT bus controller for target
 *
 *  @retval     Bitwise OR of @ref DL_I2CT_STATUS
 */
__STATIC_INLINE uint32_t DL_I2CT_getStatus(UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->i2ct->SR);
}

/**
 *  @brief      Get byte of data from UNICOMMI2CT
 *
 *  If using FIFO, it is first byte from the RX FIFO.
 *
 *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
 *
 *  @return     Last received byte of data
 *
 *  @retval     [0x00, 0xff]
 */
__STATIC_INLINE uint8_t DL_I2CT_receiveData(UNICOMM_Inst_Regs *unicomm)
{
    return (uint8_t)(unicomm->i2ct->RXDATA & UNICOMMI2CT_RXDATA_DATA_MASK);
}

/**
 *  @brief      Set next byte to be transferred during the next transaction
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  data    Byte of data to be transferred during the next
 *                      transaction. [0x00, 0xff]
 */
__STATIC_INLINE void DL_I2CT_transmitData(
    UNICOMM_Inst_Regs *unicomm, uint8_t data)
{
    unicomm->i2ct->TTXDATA = data;
}

/**
 *  @brief      Disable ACK override
 *
 *  Disable manual ACK override to automatically ACK all received bytes until
 *  the RX FIFO is full.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableACKOverride(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->ACKCTL &= ~(UNICOMMI2CT_ACKCTL_ACKOEN_MASK);
}

/**
 *  @brief      Checks if ACK override is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If ACK override is enabled
 *
 *  @retval     true if ACK override is enabled
 *  @retval     false if ACK override is disabled
 */
__STATIC_INLINE bool DL_I2CT_isACKOverrideEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->ACKCTL & UNICOMMI2CT_ACKCTL_ACKOEN_MASK) ==
            UNICOMMI2CT_ACKCTL_ACKOEN_ENABLE);
}

/**
 *  @brief      Enable ACK override
 *
 *  When manual ACK override is enabled, the I2CT SCL is pulled low after
 *  the last data is received until the ACK override value (through
 *  @ref DL_I2CT_setACKOverrideValue) is written.
 *  Disable manual ACK override to automatically ACK all received bytes until
 *  the RX FIFO is full.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_enableACKOverride(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->ACKCTL |= UNICOMMI2CT_ACKCTL_ACKOEN_ENABLE;
}

/**
 *  @brief      Get acknowledge override value
 *
 *  @note for General Call this bit will be ignored if set to NACK and
 *  continues to receive data.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     What type of response will be generated
 *
 *  @retval     One of @ref DL_I2CT_RESPONSE_OVERRIDE_VALUE.
 */
__STATIC_INLINE DL_I2CT_RESPONSE_OVERRIDE_VALUE DL_I2CT_getACKOverrideValue(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t value = unicomm->i2ct->ACKCTL & UNICOMMI2CT_ACKCTL_ACKOVAL_MASK;

    return (DL_I2CT_RESPONSE_OVERRIDE_VALUE)(value);
}

/**
 *  @brief      Set acknowledge override value
 *
 *  @note for General Call this bit will be ignored if set to NACK and
 *  continues to receive data.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  value   Indicates what type of response will be generated.
 *                      One of @ref DL_I2CT_RESPONSE_OVERRIDE_VALUE.
 *
 *  @sa         DL_I2CT_enableACKOverride
 */
__STATIC_INLINE void DL_I2CT_setACKOverrideValue(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_RESPONSE_OVERRIDE_VALUE value)
{
    DL_Common_updateReg(&unicomm->i2ct->ACKCTL, (uint32_t) value,
        UNICOMMI2CT_ACKCTL_ACKOVAL_MASK);
}

/**
 *  @brief      Disable ACK override on Start Condition
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableACKOverrideOnStart(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->ACKCTL &= ~(UNICOMMI2CT_ACKCTL_ACKOEN_ON_START_MASK);
}

/**
 *  @brief      Checks if ACK override on Start condition is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If ACK override on Start condition is enabled
 *
 *  @retval     true if ACK override on Start condition is enabled
 *  @retval     false if ACK override on Start condition is disabled
 */
__STATIC_INLINE bool DL_I2CT_isACKOverrideOnStartEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return (
        (unicomm->i2ct->ACKCTL & UNICOMMI2CT_ACKCTL_ACKOEN_ON_START_MASK) ==
        UNICOMMI2CT_ACKCTL_ACKOEN_ON_START_ENABLE);
}

/**
 *  @brief      Enable ACK override on Start condition
 *
 *  When enabled, this bit will automatically turn on the ACKOEN field
 *  following a Start condition.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @sa                 DL_I2CT_enableACKOverride
 */
__STATIC_INLINE void DL_I2CT_enableACKOverrideOnStart(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->ACKCTL |= UNICOMMI2CT_ACKCTL_ACKOEN_ON_START_ENABLE;
}

/**
 *  @brief      Disable ACK override when SMBus/PMBus PEC is next byte
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableACKOverrideOnPECNext(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->ACKCTL &= ~(UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECNEXT_MASK);
}

/**
 *  @brief      Checks if ACK override when SMBus/PMBus PEC is next byte
 *              is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If ACK override when PEC is next byte is enabled
 *
 *  @retval     true if ACK override when PEC is next byte is enabled
 *  @retval     false if ACK override when PEC is next byte is disabled
 */
__STATIC_INLINE bool DL_I2CT_isACKOverrideOnPECNextEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return (
        (unicomm->i2ct->ACKCTL & UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECNEXT_MASK) ==
        UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECNEXT_ENABLE);
}

/**
 *  @brief      Enable ACK override when SMBus/PMBus PEC is next byte
 *
 *  When enabled, this bit will automatically turn on the ACKOEN field
 *  following the ACK/NACK of the byte received just prior to the PEC byte.
 *  Note that when ACKOEN is set, the PEC byte will not automatically be
 *  ACKed/NACKed by the state machine and FW must perform this function by
 *  writing SLAVE_SACKCTL.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @sa                 DL_I2CT_enableACKOverride
 */
__STATIC_INLINE void DL_I2CT_enableACKOverrideOnPECNext(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->ACKCTL |= UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECNEXT_ENABLE;
}

/**
 *  @brief      Disable ACK override when SMBus/PMBus PEC is next byte
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableACKOverrideOnPECDone(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->ACKCTL &= ~(UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECDONE_MASK);
}

/**
 *  @brief      Checks if ACK override when SMBus/PMBus PEC is next byte
 *              is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If ACK override when PEC is next byte is enabled
 *
 *  @retval     true if ACK override when PEC is next byte is enabled
 *  @retval     false if ACK override when PEC is next byte is disabled
 */
__STATIC_INLINE bool DL_I2CT_isACKOverrideOnPECDoneEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return (
        (unicomm->i2ct->ACKCTL & UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECDONE_MASK) ==
        UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECDONE_ENABLE);
}

/**
 *  @brief      Enable ACK override when SMBus/PMBus PEC is done
 *
 *  When enabled, this bit will automatically turn on the ACKOEN field
 * following the ACK/NACK of the received PEC byte.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @sa                 DL_I2CT_enableACKOverride
 */
__STATIC_INLINE void DL_I2CT_enableACKOverrideOnPECDone(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->ACKCTL |= UNICOMMI2CT_ACKCTL_ACKOEN_ON_PECDONE_ENABLE;
}

/**
 *  @brief      Get the SMBus/PMBus Packet Error Checking (PEC) count
 *              value
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     The value the PEC count is set to
 *
 *  @retval     Value between [0x0, 0x01FF]
 */
__STATIC_INLINE uint32_t DL_I2CT_getPECCountValue(UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->i2ct->PECCTL & UNICOMMI2CT_PECCTL_PECCNT_MASK);
}

/**
 *  @brief      Set the SMBus/PMBus Packet Error Checking (PEC) count
 *              value
 *
 *  When this field is non zero, the number of I2CT data bytes are counted.
 *  Refer to the device TRM for more details.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  count   The value to set the PEC count to.
 *                      Value between [0x0, 0x01FF]
 */
__STATIC_INLINE void DL_I2CT_setPECCountValue(
    UNICOMM_Inst_Regs *unicomm, uint32_t count)
{
    DL_Common_updateReg(
        &unicomm->i2ct->PECCTL, count, UNICOMMI2CT_PECCTL_PECCNT_MASK);
}

/**
 *  @brief      Disable SMBus/PMBus Packet Error Checking (PEC)
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disablePEC(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->PECCTL &= ~(UNICOMMI2CT_PECCTL_PECEN_MASK);
}

/**
 *  @brief      Checks if SMBus/PMBus Packet Error Checking (PEC) is
 *              enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If PEC is enabled
 *
 *  @retval     true if PEC is enabled
 *  @retval     false if PEC is disabled
 */
__STATIC_INLINE bool DL_I2CT_isPECEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2ct->PECCTL & UNICOMMI2CT_PECCTL_PECEN_MASK) ==
            UNICOMMI2CT_PECCTL_PECEN_ENABLE);
}

/**
 *  @brief      Enable SMBus/PMBus Packet Error Checking (PEC)
 *
 *  When enabled, the PEC is calculated on all bits accept the Start, Stop, ACK
 *  and NACK. The PEC LSFR and the Byte Counter is set to 0 when the State
 *  Machine is in the IDLE state, which occurs following a Stop or when a
 *  timeout occurs. The Counter is also set to 0 after the PEC byte is sent or
 *  received. Note that the NACK is automatically sent following a PEC byte
 *  that results in a PEC error.
 *  The PEC Polynomial is x^8 + x^2 + x^1 + 1.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 */
__STATIC_INLINE void DL_I2CT_enablePEC(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->PECCTL |= UNICOMMI2CT_PECCTL_PECEN_ENABLE;
}

/**
 *  @brief      Get the current SMBus/PMBus PEC byte count of the state
 *              machine
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     The current PEC byte count of the state macione
 *
 *  @retval     Value between [0x0, 0x01FF]
 */
__STATIC_INLINE uint32_t DL_I2CT_getCurrentPECCount(UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->i2ct->PECSR & UNICOMMI2CT_PECSR_PECBYTECNT_MASK);
}

/**
 *  @brief      Get status if SMBus/PMBus PEC was checked in last
 *              transaction
 *
 *  The status indicates if the PEC was checked in the transaction that
 *  occurred before the last Stop. Latched on Stop.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     Status of PEC checked
 *
 *  @retval     One of @ref DL_I2CT_PEC_STATUS
 */
__STATIC_INLINE DL_I2CT_PEC_STATUS DL_I2CT_getPECCheckedStatus(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t status =
        unicomm->i2ct->PECSR & UNICOMMI2CT_PECSR_PECSTS_CHECK_MASK;

    return (DL_I2CT_PEC_STATUS)(status);
}

/**
 *  @brief      Get status if SMBus/PMBus PEC had an error
 *
 *  The status indicates if a PEC check error occurred in the transaction that
 *  occurred before the last Stop. Latched on Stop.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     Status of PEC error check
 *
 *  @retval     One of @ref DL_I2CT_PEC_CHECK_ERROR
 */
__STATIC_INLINE DL_I2CT_PEC_CHECK_ERROR DL_I2CT_getPECCheckError(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t status =
        unicomm->i2ct->PECSR & UNICOMMI2CT_PECSR_PECSTS_ERROR_MASK;

    return (DL_I2CT_PEC_CHECK_ERROR)(status);
}

/**
 *  @brief      Get TX FIFO threshold level
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @return     Indicates at what fill level in the TX FIFO a threshold will be
 *              generated
 *
 *  @retval     One of @ref DL_I2CT_TX_FIFO_LEVEL
 */
__STATIC_INLINE DL_I2CT_TX_FIFO_LEVEL DL_I2CT_getTXFIFOThreshold(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t level = unicomm->i2ct->IFLS & UNICOMMI2CT_IFLS_TXIFLSEL_MASK;

    return (DL_I2CT_TX_FIFO_LEVEL)(level);
}

/**
 *  @brief      Set TX FIFO threshold level
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  level   Indicates at what fill level in the TX FIFO a threshold
 *                      will be generated.
 *                      One of @ref DL_I2CT_TX_FIFO_LEVEL.
 */
__STATIC_INLINE void DL_I2CT_setTXFIFOThreshold(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_TX_FIFO_LEVEL level)
{
    DL_Common_updateReg(&unicomm->i2ct->IFLS, (uint32_t) level,
        (uint32_t) UNICOMMI2CT_IFLS_TXIFLSEL_MASK);
}

/**
 *  @brief      Get RX FIFO threshold level
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @return     Indicates at what fill level in the RX FIFO a threshold will be
 *              generated
 *
 *  @retval     One of @ref DL_I2CT_RX_FIFO_LEVEL
 */
__STATIC_INLINE DL_I2CT_RX_FIFO_LEVEL DL_I2CT_getRXFIFOThreshold(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t level = unicomm->i2ct->IFLS & UNICOMMI2CT_IFLS_RXIFLSEL_MASK;

    return (DL_I2CT_RX_FIFO_LEVEL)(level);
}

/**
 *  @brief      Set RX FIFO threshold level
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  level   Indicates at what fill level in the RX FIFO a threshold
 *                      will be generated.
 *                      One of @ref DL_I2CT_RX_FIFO_LEVEL.
 */
__STATIC_INLINE void DL_I2CT_setRXFIFOThreshold(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_RX_FIFO_LEVEL level)
{
    DL_Common_updateReg(&unicomm->i2ct->IFLS, (uint32_t) level,
        UNICOMMI2CT_IFLS_RXIFLSEL_MASK);
}

/**
 *  @brief      Enable I2CT interrupts
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_I2CT_INTERRUPT.
 */
__STATIC_INLINE void DL_I2CT_enableInterrupt(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    unicomm->i2ct->CPU_INT.IMASK |= interruptMask;
}

/**
 *  @brief      Disable I2CT interrupts
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to disable. Bitwise OR of
 *                             @ref DL_I2CT_INTERRUPT.
 */
__STATIC_INLINE void DL_I2CT_disableInterrupt(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    unicomm->i2ct->CPU_INT.IMASK &= ~(interruptMask);
}

/**
 *  @brief      Check which I2CT interrupts are enabled
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_I2CT_INTERRUPT.
 *
 *  @return     Which of the requested I2CT interrupts are enabled
 *
 *  @retval     Bitwise OR of @ref DL_I2CT_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_I2CT_getEnabledInterrupts(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    return (unicomm->i2ct->CPU_INT.IMASK & interruptMask);
}

/**
 *  @brief      Check interrupt flag of enabled I2CT interrupts
 *
 *  Checks if any of the I2CT interrupts that were previously enabled are
 *  pending.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_I2CT_INTERRUPT.
 *
 *  @return     Which of the requested I2CT interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_I2CT_INTERRUPT values
 *
 *  @sa         DL_I2CT_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_I2CT_getEnabledInterruptStatus(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    return (unicomm->i2ct->CPU_INT.MIS & interruptMask);
}

/**
 *  @brief      Check interrupt flag of any I2CT interrupt
 *
 *  Checks if any of the I2CT interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_I2CT_INTERRUPT.
 *
 *  @return     Which of the requested I2CT interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_I2CT_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_I2CT_getRawInterruptStatus(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    return (unicomm->i2ct->CPU_INT.RIS & interruptMask);
}

/**
 *  @brief      Get highest priority pending I2CT interrupt
 *
 *  Checks if any of the I2CT interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     The highest priority pending I2CT interrupt
 *
 *  @retval     One of @ref DL_I2CT_IIDX
 */
__STATIC_INLINE DL_I2CT_IIDX DL_I2CT_getPendingInterrupt(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((DL_I2CT_IIDX) unicomm->i2ct->CPU_INT.IIDX);
}

/**
 *  @brief      Clear pending I2CT interrupts
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of
 *                             @ref DL_I2CT_INTERRUPT.
 */
__STATIC_INLINE void DL_I2CT_clearInterruptStatus(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    unicomm->i2ct->CPU_INT.ICLR = interruptMask;
}

/**
 *  @brief Enables I2CT interrupt for triggering DMA events
 *
 * Enables the I2CT interrupt to be used as the condition to generate an
 * event to directly trigger the DMA.
 *
 * Each event publisher @ref DL_I2CT_EVENT_ROUTE can set any one of
 * @ref DL_I2CT_DMA_INTERRUPT.
 *
 * @note Only one interrupt source should be enabled at a time.
 *
 *  @param[in]  unicomm        Pointer to the register overlay for the peripheral
 *  @param[in]  index      Specifies the register event publisher to configure
 *  @param[in]  interrupt  Interrupt to enable as the trigger condition for
 *                         the DMA. One of @ref DL_I2CT_DMA_INTERRUPT.
 */
__STATIC_INLINE void DL_I2CT_enableDMAEvent(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_EVENT_ROUTE index, uint32_t interrupt)
{
    switch (index) {
        case DL_I2CT_EVENT_ROUTE_1:
            unicomm->i2ct->DMA_TRIG1.IMASK = interrupt;
            break;
        case DL_I2CT_EVENT_ROUTE_2:
            unicomm->i2ct->DMA_TRIG0.IMASK = interrupt;
            break;
        default:
            break;
    }
}

/**
 *  @brief Disables I2CT interrupt from triggering DMA events
 *
 * Disables the I2CT interrupt from being used as the condition to generate an
 * event to directly trigger the DMA.
 *
 * Each event publisher @ref DL_I2CT_EVENT_ROUTE can set any one of
 * @ref DL_I2CT_DMA_INTERRUPT.
 *
 *  @param[in]  unicomm        Pointer to the register overlay for the peripheral
 *  @param[in]  index      Specifies the register event publisher to configure
 *  @param[in]  interrupt  Interrupt to disable as the trigger condition for
 *                         the DMA. One of @ref DL_I2CT_DMA_INTERRUPT.
 */
__STATIC_INLINE void DL_I2CT_disableDMAEvent(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_EVENT_ROUTE index, uint32_t interrupt)
{
    switch (index) {
        case DL_I2CT_EVENT_ROUTE_1:
            unicomm->i2ct->DMA_TRIG1.IMASK &= ~(interrupt);
            break;
        case DL_I2CT_EVENT_ROUTE_2:
            unicomm->i2ct->DMA_TRIG0.IMASK &= ~(interrupt);
            break;
        default:
            break;
    }
}

/**
 *  @brief Check which I2CT interrupt for DMA receive events is enabled
 *
 *  This API checks the event publisher register as selected by
 *  @ref DL_I2CT_EVENT_ROUTE, which are used for triggering the DMA for
 *  Controller or and receive or transmit events.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the peripheral
 *  @param[in]  index          Specifies the register event publisher to configure
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_I2CT_DMA_INTERRUPT.
 *
 *  @note Only one interrupt source should be enabled at a time.
 *
 *  @return     The requested I2CT interrupt status
 *
 *  @retval     One of @ref DL_I2CT_DMA_INTERRUPT
 */
__STATIC_INLINE uint32_t DL_I2CT_getEnabledDMAEvents(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_EVENT_ROUTE index,
    uint32_t interruptMask)
{
    volatile uint32_t *pReg = &unicomm->i2ct->DMA_TRIG1.IMASK;

    return ((*(pReg + (uint32_t) index) & interruptMask));
}

/**
 *  @brief Check interrupt flag of enabled I2CT interrupt for DMA event
 *
 *  Checks if any of the I2CT interrupts for the DMA receive event that were
 *  previously enabled are pending.
 *  This API checks the event publisher register as selected by
 *  @ref DL_I2CT_EVENT_ROUTE, which are used for triggering the DMA for
 *  Controller or and receive or transmit events.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the peripheral
 *  @param[in]  index          Specifies the register event publisher to configure
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_I2CT_DMA_INTERRUPT.
 *
 *  @note Only one interrupt source should be enabled at a time.
 *
 *  @return     The requested I2CT interrupt status
 *
 *  @retval     One of @ref DL_I2CT_DMA_INTERRUPT
 *
 *  @sa         DL_I2CT_enableDMAEvent
 */
__STATIC_INLINE uint32_t DL_I2CT_getEnabledDMAEventStatus(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_EVENT_ROUTE index,
    uint32_t interruptMask)
{
    const volatile uint32_t *pReg = &unicomm->i2ct->DMA_TRIG1.MIS;

    return ((*(pReg + (uint32_t) index) & interruptMask));
}

/**
 *  @brief Check interrupt flag of any I2CT interrupt for DMA event
 *
 *  Checks if any of the I2CT interrupts for DMA receive event are pending.
 *  Interrupts do not have to be previously enabled.
 *  This API checks the event publisher register as selected by
 *  @ref DL_I2CT_EVENT_ROUTE, which are used for triggering the DMA for
 *  Controller or and receive or transmit events.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the peripheral
 *  @param[in]  index          Specifies the register event publisher to configure
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_I2CT_DMA_INTERRUPT.
 *
 *  @return     Which of the requested I2CT interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_I2CT_DMA_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_I2CT_getRawDMAEventStatus(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_EVENT_ROUTE index,
    uint32_t interruptMask)
{
    const volatile uint32_t *pReg = &unicomm->i2ct->DMA_TRIG1.RIS;

    return ((*(pReg + (uint32_t) index) & interruptMask));
}

/**
 *  @brief      Get the Timeout Counter A value
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     The Timeout A counter value
 *
 *  @retval     Value between [0x0, 0xFF]
 */
__STATIC_INLINE uint32_t DL_I2CT_getTimeoutACount(UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->i2ct->TIMEOUT_CTL & UNICOMMI2CT_TIMEOUT_CTL_TCNTLA_MASK);
}

/**
 *  @brief      Set the Timeout Counter A value
 *
 *  Timeout A is used for SCL low detection. This field contains the upper 8
 *  bits of a 12-bit pre-load value for the Timeout A count.
 *  NOTE: The value of the counter must be greater than 1 to enable the
 *  timeout. Each count is equal to 520 times the timeout period of the
 *  functional clock. For example, with 8MHz functional clock and a
 *  100KHz operating I2CT clock, one timeout period will be equal to
 *  (1 / 8MHz) * 520 = 65us.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the
 *                      peripheral
 *  @param[in]  count   The value to set the Timeout A counter to.
 *                      Value between [0x0, 0xFF]
 */
__STATIC_INLINE void DL_I2CT_setTimeoutACount(
    UNICOMM_Inst_Regs *unicomm, uint32_t count)
{
    DL_Common_updateReg(&unicomm->i2ct->TIMEOUT_CTL, count,
        UNICOMMI2CT_TIMEOUT_CTL_TCNTLA_MASK);
}

/**
 *  @brief      Disable Timeout Counter A
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableTimeoutA(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->TIMEOUT_CTL &= ~(UNICOMMI2CT_TIMEOUT_CTL_TCNTAEN_MASK);
}

/**
 *  @brief      Checks if Timeout Counter A is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If Timeout Counter A is enabled
 *
 *  @retval     true if Timeout Counter A is enabled
 *  @retval     false if Timeout Counter A is disabled
 */
__STATIC_INLINE bool DL_I2CT_isTimeoutAEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return (
        (unicomm->i2ct->TIMEOUT_CTL & UNICOMMI2CT_TIMEOUT_CTL_TCNTAEN_MASK) ==
        UNICOMMI2CT_TIMEOUT_CTL_TCNTAEN_ENABLE);
}

/**
 *  @brief      Enable Timeout Counter A
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_enableTimeoutA(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->TIMEOUT_CTL |= UNICOMMI2CT_TIMEOUT_CTL_TCNTAEN_ENABLE;
}

/**
 *  @brief      Get the current Timer Counter A value
 *
 *  This field contains the upper 8 bits of a 12-bit current counter for
 *  Timeout Counter A.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     The Timeout A counter value
 *
 *  @retval     Value between [0x0, 0xFF]
 */
__STATIC_INLINE uint32_t DL_I2CT_getCurrentTimeoutACounter(
    UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->i2ct->TIMEOUT_CNT & UNICOMMI2CT_TIMEOUT_CNT_TCNTA_MASK);
}

/**
 *  @brief      Get the Timeout Counter B value
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     The Timeout B counter value
 *
 *  @retval     Value between [0x0, 0xFF]
 */
__STATIC_INLINE uint32_t DL_I2CT_getTimeoutBCount(UNICOMM_Inst_Regs *unicomm)
{
    return (
        (unicomm->i2ct->TIMEOUT_CTL & UNICOMMI2CT_TIMEOUT_CTL_TCNTLB_MASK) >>
        UNICOMMI2CT_TIMEOUT_CTL_TCNTLB_OFS);
}

/**
 *  @brief      Set the Timeout Counter B value
 *
 *  Timeout B is used for SCL high detection. This field contains the upper 8
 *  bits of a 12-bit pre-load value for the Timeout A count.
 *  NOTE: The value of the counter must be greater than 1 to enable the
 *  timeout. Each count is equal to 1* clock period. For example, with 10MHz
 *  functional clock one timeout period will be equal to 1 * 10ns.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the
 *                      peripheral
 *  @param[in]  count   The value to set the Timeout A counter to.
 *                      Value between [0x0, 0xFF]
 */
__STATIC_INLINE void DL_I2CT_setTimeoutBCount(
    UNICOMM_Inst_Regs *unicomm, uint32_t count)
{
    DL_Common_updateReg(&unicomm->i2ct->TIMEOUT_CTL,
        (count << UNICOMMI2CT_TIMEOUT_CTL_TCNTLB_OFS),
        UNICOMMI2CT_TIMEOUT_CTL_TCNTLB_MASK);
}

/**
 *  @brief      Disable Timeout Counter B
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_disableTimeoutB(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->TIMEOUT_CTL &= ~(UNICOMMI2CT_TIMEOUT_CTL_TCNTBEN_MASK);
}

/**
 *  @brief      Checks if Timeout Counter B is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If Timeout Counter B is enabled
 *
 *  @retval     true if Timeout Counter B is enabled
 *  @retval     false if Timeout Counter B is disabled
 */
__STATIC_INLINE bool DL_I2CT_isTimeoutBEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return (
        (unicomm->i2ct->TIMEOUT_CTL & UNICOMMI2CT_TIMEOUT_CTL_TCNTBEN_MASK) ==
        UNICOMMI2CT_TIMEOUT_CTL_TCNTBEN_ENABLE);
}

/**
 *  @brief      Enable Timeout Counter B
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CT_enableTimeoutB(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2ct->TIMEOUT_CTL |= UNICOMMI2CT_TIMEOUT_CTL_TCNTBEN_ENABLE;
}

/**
 *  @brief      Get the current Timer Counter B value
 *
 *  This field contains the upper 8 bits of a 12-bit current counter for
 *  Timeout Counter B.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     The Timeout B counter value
 *
 *  @retval     Value between [0x0, 0xFF]
 */
__STATIC_INLINE uint32_t DL_I2CT_getCurrentTimeoutBCounter(
    UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->i2ct->TIMEOUT_CNT & UNICOMMI2CT_TIMEOUT_CNT_TCNTB_MASK);
}

#ifdef __cplusplus
}
#endif

#endif /* __MCU_HAS_UNICOMMI2CT__ */

#endif /* ti_dl_dl_unicommi2ct__include */
/** @}*/
