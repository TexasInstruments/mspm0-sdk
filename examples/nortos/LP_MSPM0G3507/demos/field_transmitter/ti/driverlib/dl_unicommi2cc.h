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
 *  @file       dl_unicommi2cc.h
 *  @brief      Unified Communication Module (UNICOMM) - Inter-Integrated Circuit Controller (I2CC) Driver Library
 *  @defgroup   UNICOMMI2CC Unified Communication Module (UNICOMM) - Inter-Integrated Circuit Controller (I2CC)
 *
 *  @anchor ti_dl_dl_m0p_unicommi2cc_Overview
 *  # Overview
 *
 *  Unified Communication Module Inter-Integrated Circuit Controller Driver Library allows
 *  full configuration of the UNICOMM I2CC module.
 *
 *  This Inter-Integrated Circuit Controller (I2CC) module provides a standardized
 *  interface to transfer data, as a controller, between devices and other external
 *  devices with the I2C interface.
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup UNICOMMI2CC
 * @{
 */
#ifndef ti_dl_dl_unicommi2cc__include
#define ti_dl_dl_unicommi2cc__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>
#include <ti/driverlib/dl_unicomm.h>

#ifdef __MCU_HAS_UNICOMMI2CC__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/*!
 * @brief I2CC number of bytes which could be put into the TX FIFO
 *
 * This variable is device specific and is calculated using the system parameter
 * UNICOMMI2CC_SYS_FENTRIES defined in each devices header file.
*/
#define DL_I2CC_TX_FIFO_COUNT_MAXIMUM          ((uint32_t)UNICOMMI2CC_SYS_FENTRIES << 8)

/*!
 * @brief I2CC number of bytes which could be put into the RX FIFO
 *
 * This variable is device specific and is calculated using the system parameter
 * UNICOMMI2CC_SYS_FENTRIES defined in each devices header file.
*/
#define DL_I2CC_RX_FIFO_COUNT_MAXIMUM               ((uint32_t)UNICOMMI2CC_SYS_FENTRIES)

/** @addtogroup DL_I2CC_STATUS
 *  @{
 */
/*!
 * @brief I2CC is busy
 *
 * The BUSY bit is set during an ongoing transaction, so is set during the
 * transmit/receive of the amount of data set in WBLEN including START, RESTART,
 * Address and STOP signal generation when required for the current transaction
 */
#define DL_I2CC_STATUS_BUSY                       (UNICOMMI2CC_SR_BUSY_MASK)

/*!
 * @brief I2CC error detected
 *
 * The error can be from the target address not being acknowledged or the
 * transmit data not being acknowledged
 */
#define DL_I2CC_STATUS_ERROR                       (UNICOMMI2CC_SR_ERR_MASK)

/*!
 * @brief I2CC address acknowledged
 */
#define DL_I2CC_STATUS_ADDR_ACK                 (UNICOMMI2CC_SR_ADRACK_MASK)

/*!
 * @brief I2CC data acknowledged
 */
#define DL_I2CC_STATUS_DATA_ACK                 (UNICOMMI2CC_SR_DATACK_MASK)

/*!
 * @brief I2CC arbitration lost
 */
#define DL_I2CC_STATUS_ARBITRATION_LOST         (UNICOMMI2CC_SR_ARBLST_MASK)

/*!
 * @brief I2CC idle
 */
#define DL_I2CC_STATUS_IDLE                       (UNICOMMI2CC_SR_IDLE_MASK)

/*!
 * @brief I2CC bus busy
 *
 * The bit changes based on the START and STOP conditions
 */
#define DL_I2CC_STATUS_BUSY_BUS                 (UNICOMMI2CC_SR_BUSBSY_MASK)

/** @}*/


/** @addtogroup DL_I2CC_INTERRUPT
 *  @{
 */
/*!
 * @brief Receive Transaction completed Interrupt
 */
#define DL_I2CC_INTERRUPT_RX_DONE  (UNICOMMI2CC_CPU_INT_IMASK_RXDONE_SET)

/*!
 * @brief Transmit Transaction completed Interrupt
 */
#define DL_I2CC_INTERRUPT_TX_DONE  (UNICOMMI2CC_CPU_INT_IMASK_TXDONE_SET)

/*!
 * @brief Receive FIFO Trigger when >= defined bytes
 */
#define DL_I2CC_INTERRUPT_RXFIFO_TRIGGER                            \
                                          (UNICOMMI2CC_CPU_INT_IMASK_RXTRG_SET)

/*!
 * @brief Transmit FIFO Trigger when <= defined bytes
 */
#define DL_I2CC_INTERRUPT_TXFIFO_TRIGGER                            \
                                          (UNICOMMI2CC_CPU_INT_IMASK_TXTRG_SET)

/*!
 * @brief Receive FIFO is full
 */
#define DL_I2CC_INTERRUPT_RXFIFO_FULL                               \
                                         (UNICOMMI2CC_CPU_INT_IMASK_RXFULL_SET)

/*!
 * @brief Transmit FIFO is empty
 */
#define DL_I2CC_INTERRUPT_TXFIFO_EMPTY                              \
                                            (UNICOMMI2CC_CPU_INT_IMASK_TXEMPTY_SET)

/*!
 * @brief Address/Data NACK Interrupt
 */
#define DL_I2CC_INTERRUPT_NACK       (UNICOMMI2CC_CPU_INT_IMASK_NACK_SET)

/*!
 * @brief START Detection Interrupt
 */
#define DL_I2CC_INTERRUPT_START     (UNICOMMI2CC_CPU_INT_IMASK_START_SET)

/*!
 * @brief STOP Detection Interrupt
 */
#define DL_I2CC_INTERRUPT_STOP       (UNICOMMI2CC_CPU_INT_IMASK_STOP_SET)

/*!
 * @brief Arbitration Lost Interrupt
 */
#define DL_I2CC_INTERRUPT_ARBITRATION_LOST                          \
                                            (UNICOMMI2CC_CPU_INT_IMASK_ARBLOST_SET)

/*!
 * @brief DMA Done on Event 1 publisher
 */
#define DL_I2CC_INTERRUPT_EVENT1_DMA_DONE                           \
                                       (UNICOMMI2CC_CPU_INT_IMASK_DMA_DONE_TX_SET)

/*!
 * @brief DMA Done on Event 2 publisher
 */
#define DL_I2CC_INTERRUPT_EVENT2_DMA_DONE                           \
                                       (UNICOMMI2CC_CPU_INT_IMASK_DMA_DONE_RX_SET)


/*!
 * @brief SMBus/PMBus PEC Receive Error Interrupt
 */
#define DL_I2CC_INTERRUPT_PEC_RX_ERROR                              \
                                         (UNICOMMI2CC_CPU_INT_IMASK_PEC_RX_ERR_SET)

/*!
 * @brief Timeout A Interrupt
 */
#define DL_I2CC_INTERRUPT_TIMEOUT_A          (UNICOMMI2CC_CPU_INT_IMASK_TIMEOUTA_SET)

/*!
 * @brief Timeout B Interrupt
 */
#define DL_I2CC_INTERRUPT_TIMEOUT_B          (UNICOMMI2CC_CPU_INT_IMASK_TIMEOUTB_SET)


/** @}*/

/** @addtogroup DL_I2CC_DMA_INTERRUPT
 *  @{
 */
/*!
 * @brief Controller Transmit FIFO Trigger interrupt for DMA trigger
 */
#define DL_I2CC_DMA_INTERRUPT_TXFIFO_TRIGGER                            \
                                          (UNICOMMI2CC_DMA_TRIG0_IMASK_TXTRG_SET)

/*!
 * @brief Controller Receive FIFO Trigger interrupt for DMA trigger
 */
#define DL_I2CC_DMA_INTERRUPT_RXFIFO_TRIGGER                        \
                                          (UNICOMMI2CC_DMA_TRIG1_IMASK_RXTRG_SET)

/** @}*/

/* clang-format on */

/** @enum DL_I2CC_EVENT_ROUTE */
typedef enum {
    /*! I2CC event route 1 */
    DL_I2CC_EVENT_ROUTE_1 = 0,
    /*! I2CC event route 2 */
    DL_I2CC_EVENT_ROUTE_2 = 12
} DL_I2CC_EVENT_ROUTE;

/** @enum DL_I2CC_CLOCK */
typedef enum {
    /*!  BUSCLK enabled as I2CC clock source */
    DL_I2CC_CLOCK_BUSCLK = UNICOMMI2CC_CLKSEL_BUSCLK_SEL_ENABLE,
    /*!  MFCLK enabled as I2CC clock source */
    DL_I2CC_CLOCK_MFCLK = UNICOMMI2CC_CLKSEL_MFCLK_SEL_ENABLE,
    /*!  Asynchronous SYSCLK enabled as I2CC clock source */
    DL_I2CC_CLOCK_ASYNC_SYSCLK = UNICOMMI2CC_CLKSEL_ASYNC_SYSCLK_SEL_ENABLE,
    /*!  Asynchronous HFCLK enabled as I2CC clock source */
    DL_I2CC_CLOCK_ASYNC_HFCLK = UNICOMMI2CC_CLKSEL_ASYNC_HFCLK_SEL_ENABLE,
    /*!  Asynchronous PLL enabled as I2CC clock source */
    DL_I2CC_CLOCK_ASYNC_PLL = UNICOMMI2CC_CLKSEL_ASYNC_PLL_SEL_ENABLE,
} DL_I2CC_CLOCK;

/** @enum DL_I2CC_CLOCK_DIVIDE */
typedef enum {
    /*!  I2CC Clock Divided by 1 */
    DL_I2CC_CLOCK_DIVIDE_1 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_1,
    /*!  I2CC Clock Divided by 2 */
    DL_I2CC_CLOCK_DIVIDE_2 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_2,
    /*!  I2CC Clock Divided by 3 */
    DL_I2CC_CLOCK_DIVIDE_3 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_3,
    /*!  I2CC Clock Divided by 4 */
    DL_I2CC_CLOCK_DIVIDE_4 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_4,
    /*!  I2CC Clock Divided by 5 */
    DL_I2CC_CLOCK_DIVIDE_5 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_5,
    /*!  I2CC Clock Divided by 6 */
    DL_I2CC_CLOCK_DIVIDE_6 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_6,
    /*!  I2CC Clock Divided by 7 */
    DL_I2CC_CLOCK_DIVIDE_7 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_7,
    /*!  I2CC Clock Divided by 8 */
    DL_I2CC_CLOCK_DIVIDE_8 = UNICOMMI2CC_CLKDIV_RATIO_DIV_BY_8,
} DL_I2CC_CLOCK_DIVIDE;

/** @enum DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH */
typedef enum {
    /*!  Pulses filtering disabled.  */
    DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_DISABLED =
        UNICOMMI2CC_GFCTL_DGFSEL_DISABLED,
    /*!  Pulses shorter than 1 functional clock tick in length are filtered.  */
    DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_1 =
        UNICOMMI2CC_GFCTL_DGFSEL_CLK_1,
    /*!  Pulses shorter than 2 functional clock ticks in length are filtered.  */
    DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_2 =
        UNICOMMI2CC_GFCTL_DGFSEL_CLK_2,
    /*!  Pulses shorter than 3 functional clock ticks in length are filtered.  */
    DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_3 =
        UNICOMMI2CC_GFCTL_DGFSEL_CLK_3,
    /*!  Pulses shorter than 4 functional clock ticks in length are filtered.  */
    DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_4 =
        UNICOMMI2CC_GFCTL_DGFSEL_CLK_4,
    /*!  Pulses shorter than 8 functional clock ticks in length are filtered.  */
    DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_8 =
        UNICOMMI2CC_GFCTL_DGFSEL_CLK_8,
    /*!  Pulses shorter than 16 functional clock ticks in length are filtered.  */
    DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_16 =
        UNICOMMI2CC_GFCTL_DGFSEL_CLK_16,
    /*!  Pulses shorter than 31 functional clock ticks in length are filtered.  */
    DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH_CLOCKS_31 =
        UNICOMMI2CC_GFCTL_DGFSEL_CLK_31,
} DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH;

/** @enum DL_I2CC_DIRECTION */
typedef enum {
    /*! Set direction to transmitting to target */
    DL_I2CC_DIRECTION_TX = UNICOMMI2CC_TA_DIR_TRANSMIT,
    /*! Set direction to receiving from target */
    DL_I2CC_DIRECTION_RX = UNICOMMI2CC_TA_DIR_RECEIVE,
} DL_I2CC_DIRECTION;

/** @enum DL_I2CC_ADDRESSING_MODE */
typedef enum {
    /*! Enable in 7-bit addressing mode */
    DL_I2CC_ADDRESSING_MODE_7_BIT = UNICOMMI2CC_TA_MODE_MODE7,
    /*! Enable in 10-bit addressing mode */
    DL_I2CC_ADDRESSING_MODE_10_BIT = UNICOMMI2CC_TA_MODE_MODE10,
} DL_I2CC_ADDRESSING_MODE;

/** @enum DL_I2CC_PEC_STATUS */
typedef enum {
    /*!  I2CC SMBus/PMBus PEC was checked in the transaction that
     *   occurred before the last Stop */
    DL_I2CC_PEC_STATUS_CHECKED = UNICOMMI2CC_PECSR_PECSTS_CHECK_SET,
    /*!  I2CC SMBus/PMBus PEC was not checked in the transaction that
     *   occurred the last Stop */
    DL_I2CC_PEC_STATUS_NOT_CHECKED = UNICOMMI2CC_PECSR_PECSTS_CHECK_CLEARED,
} DL_I2CC_PEC_STATUS;

/** @enum DL_I2CC_PEC_CHECK_ERROR */
typedef enum {
    /*! I2CC SMBus/PMBus PEC check error occurred in the transaction
     *  before the last stop */
    DL_I2CC_PEC_CHECK_ERROR_SET = UNICOMMI2CC_PECSR_PECSTS_ERROR_SET,
    /*! I2CC SMBus/PMBus PEC check error did not occur in the
     *  transaction before the last stop */
    DL_I2CC_PEC_CHECK_ERROR_CLEARED = UNICOMMI2CC_PECSR_PECSTS_ERROR_CLEARED,
} DL_I2CC_PEC_CHECK_ERROR;

/** @enum DL_I2CC_SCL */
typedef enum {
    /*!  I2CC SCL signal high */
    DL_I2CC_SCL_HIGH = UNICOMMI2CC_BMON_SCL_SET,
    /*!  I2CC SCL signal low */
    DL_I2CC_SCL_LOW = UNICOMMI2CC_BMON_SCL_CLEARED,
} DL_I2CC_SCL;

/** @enum DL_I2CC_SDA */
typedef enum {
    /*!  I2CC SDA signal high */
    DL_I2CC_SDA_HIGH = UNICOMMI2CC_BMON_SDA_SET,
    /*!  I2CC SDA signal low */
    DL_I2CC_SDA_LOW = UNICOMMI2CC_BMON_SDA_CLEARED,
} DL_I2CC_SDA;

/** @enum DL_I2CC_START */
typedef enum {
    /*!  generates START condition */
    DL_I2CC_START_ENABLE = UNICOMMI2CC_CTR_START_ENABLE,
    /*!  doesn't generate START condition */
    DL_I2CC_START_DISABLE = UNICOMMI2CC_CTR_START_DISABLE,
} DL_I2CC_START;

/** @enum DL_I2CC_STOP */
typedef enum {
    /*!  generates STOP condition */
    DL_I2CC_STOP_ENABLE = UNICOMMI2CC_CTR_STOP_ENABLE,
    /*!  doesn't generate STOP condition */
    DL_I2CC_STOP_DISABLE = UNICOMMI2CC_CTR_STOP_DISABLE,
} DL_I2CC_STOP;

/** @enum DL_I2CC_ACK */
typedef enum {
    /*!  Last received data byte of a transaction is not ACKed automatically */
    DL_I2CC_ACK_ENABLE = UNICOMMI2CC_CTR_ACK_ENABLE,
    /*!  Last received data byte of a transaction is ACKed automatically */
    DL_I2CC_ACK_DISABLE = UNICOMMI2CC_CTR_ACK_DISABLE,
} DL_I2CC_ACK;

/** @enum DL_I2CC_TX_FIFO_LEVEL */
typedef enum {
    /*! Interrupt triggers when FIFO <= 3/4 empty */
    DL_I2CC_TX_FIFO_LEVEL_3_4_EMPTY = UNICOMMI2CC_IFLS_TXIFLSEL_LVL_3_4,
    /*! Interrupt triggers when FIFO <= 1/2 empty */
    DL_I2CC_TX_FIFO_LEVEL_1_2_EMPTY = UNICOMMI2CC_IFLS_TXIFLSEL_LVL_1_2,
    /*! Interrupt triggers when FIFO <= 1/4 empty */
    DL_I2CC_TX_FIFO_LEVEL_1_4_EMPTY = UNICOMMI2CC_IFLS_TXIFLSEL_LVL_1_4,
    /*! Interrupt triggers when FIFO is not full */
    DL_I2CC_TX_FIFO_LEVEL_ONE_ENTRY = UNICOMMI2CC_IFLS_TXIFLSEL_LVL_NOT_FULL,
    /*! Interrupt triggers when FIFO is empty */
    DL_I2CC_TX_FIFO_LEVEL_EMPTY = UNICOMMI2CC_IFLS_TXIFLSEL_LVL_EMPTY,
    /*! Interrupt triggers when FIFO is almost empty (FIFO <= 1) */
    DL_I2CC_TX_FIFO_LEVEL_ALMOST_EMPTY =
        UNICOMMI2CC_IFLS_TXIFLSEL_LVL_ALMOST_EMPTY,
    /*! Interrupt triggers when FIFO is almost full (FIFO >= (MAX_FIFO_LEN - 1)) */
    DL_I2CC_TX_FIFO_LEVEL_ALMOST_FULL =
        UNICOMMI2CC_IFLS_TXIFLSEL_LVL_ALMOST_FULL
} DL_I2CC_TX_FIFO_LEVEL;

/** @enum DL_I2CC_RX_FIFO_LEVEL */
typedef enum {
    /*! Interrupt triggers when FIFO >= 3/4 full */
    DL_I2CC_RX_FIFO_LEVEL_3_4_FULL = UNICOMMI2CC_IFLS_RXIFLSEL_LVL_3_4,
    /*! Interrupt triggers when FIFO >= 1/2 full */
    DL_I2CC_RX_FIFO_LEVEL_1_2_FULL = UNICOMMI2CC_IFLS_RXIFLSEL_LVL_1_2,
    /*! Interrupt triggers when FIFO >= 1/4 full */
    DL_I2CC_RX_FIFO_LEVEL_1_4_FULL = UNICOMMI2CC_IFLS_RXIFLSEL_LVL_1_4,
    /*! Interrupt triggers when FIFO is not empty */
    DL_I2CC_RX_FIFO_LEVEL_ONE_ENTRY = UNICOMMI2CC_IFLS_RXIFLSEL_LVL_NOT_EMPTY,
    /*! Interrupt triggers when FIFO is full */
    DL_I2CC_RX_FIFO_LEVEL_FULL = UNICOMMI2CC_IFLS_RXIFLSEL_LVL_FULL,
    /*! Interrupt triggers when FIFO is almost full (FIFO >= (MAX_FIFO_LEN - 1)) */
    DL_I2CC_RX_FIFO_LEVEL_ALMOST_FULL =
        UNICOMMI2CC_IFLS_RXIFLSEL_LVL_ALMOST_FULL,
    /*! Interrupt triggers when FIFO is almost empty (FIFO <= 1) */
    DL_I2CC_RX_FIFO_LEVEL_ALMOST_EMPTY =
        UNICOMMI2CC_IFLS_RXIFLSEL_LVL_ALMOST_EMPTY,
} DL_I2CC_RX_FIFO_LEVEL;

/*! @enum DL_I2CC_IIDX */
typedef enum {
    /*! Interrupt index for I2CC if no interrupt is pending */
    DL_I2CC_IIDX_NO_INT = UNICOMMI2CC_IIDX_STAT_NO_INTR,
    /*! Interrupt index for I2CC Receive Transaction completed */
    DL_I2CC_IIDX_RX_DONE = UNICOMMI2CC_IIDX_STAT_RXDONEFG,
    /*! Interrupt index for Transmit Transaction completed */
    DL_I2CC_IIDX_TX_DONE = UNICOMMI2CC_IIDX_STAT_TXDONEFG,
    /*! Interrupt index for I2CC Receive FIFO Trigger */
    DL_I2CC_IIDX_RXFIFO_TRIGGER = UNICOMMI2CC_IIDX_STAT_RXTRG,
    /*! Interrupt index for I2CC Transmit FIFO Trigger */
    DL_I2CC_IIDX_TXFIFO_TRIGGER = UNICOMMI2CC_IIDX_STAT_TXTRG,
    /*! Interrupt index for I2CC Receive when FIFO is full */
    DL_I2CC_IIDX_RXFIFO_FULL = UNICOMMI2CC_IIDX_STAT_RXFULL,
    /*! Interrupt index for I2CC when Transmit FIFO is empty */
    DL_I2CC_IIDX_TXFIFO_EMPTY = UNICOMMI2CC_IIDX_STAT_TXEMPTY,
    /*! Interrupt index for Address/Data NACK */
    DL_I2CC_IIDX_NACK = UNICOMMI2CC_IIDX_STAT_NACKFG,
    /*! Interrupt index for I2CC START Detection */
    DL_I2CC_IIDX_START = UNICOMMI2CC_IIDX_STAT_STARTFG,
    /*! Interrupt index for I2CC STOP Detection */
    DL_I2CC_IIDX_STOP = UNICOMMI2CC_IIDX_STAT_STOPFG,
    /*! Interrupt index for I2CC Arbitration Lost */
    DL_I2CC_IIDX_ARBITRATION_LOST = UNICOMMI2CC_IIDX_STAT_ARBLOSTFG,
    /*! Interrupt index for I2CC Event 1 DMA Done */
    DL_I2CC_IIDX_EVENT1_DMA_DONE = UNICOMMI2CC_IIDX_STAT_DMA_DONE_TX,
    /*! Interrupt index for I2CC Event 2 DMA Done */
    DL_I2CC_IIDX_EVENT2_DMA_DONE = UNICOMMI2CC_IIDX_STAT_DMA_DONE_RX,
    /*! Interrupt index for I2CC receive pre-emptive interrupt */
    DL_I2CC_IIDX_PREEMPT_INT_TX = UNICOMMI2CC_IIDX_STAT_DMA_PREIRQ_TX,
    /*! Interrupt index for I2CC receive pre-emptive interrupt */
    DL_I2CC_IIDX_PREEMPT_INT_RX = UNICOMMI2CC_IIDX_STAT_DMA_PREIRQ_RX,
    /*! Interrupt index for I2CC SMBus/PMBus PEC Receive Error Event */
    DL_I2CC_IIDX_PEC_RX_ERROR = UNICOMMI2CC_IIDX_STAT_PEC_RX_ERR,
    /*! Interrupt index for I2CC Timeout A Event */
    DL_I2CC_IIDX_TIMEOUT_A = UNICOMMI2CC_IIDX_STAT_TIMEOUTA,
    /*! Interrupt index for I2CC Timeout B Event */
    DL_I2CC_IIDX_TIMEOUT_B = UNICOMMI2CC_IIDX_STAT_TIMEOUTB,
} DL_I2CC_IIDX;

/**
 * @brief  Configuration struct for @ref DL_I2CC_setClockConfig.
 */
typedef struct {
    /*! I2CC module clock source. One of @ref DL_I2CC_CLOCK */
    DL_I2CC_CLOCK clockSel;
    /*! I2CC clock divider selection. One of @ref DL_I2CC_CLOCK_DIVIDE */
    DL_I2CC_CLOCK_DIVIDE divideRatio;
} DL_I2CC_ClockConfig;

/**
 *  @brief      Configure I2CC source clock
 *
 *  @param[in]  unicomm    Pointer to the register overlay for the
 *                      peripheral
 *  @param[in]  config  Pointer to the clock configuration struct
 *                       @ref DL_I2CC_ClockConfig.
 */
void DL_I2CC_setClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_I2CC_ClockConfig *config);

/**
 *  @brief      Get I2CC source clock configuration
 *
 *  @param[in]  unicomm    Pointer to the register overlay for the
 *                      peripheral
 *  @param[in]  config  Pointer to the clock configuration struct
 *                      @ref DL_I2CC_ClockConfig.
 */
void DL_I2CC_getClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_I2CC_ClockConfig *config);

/**
 *  @brief      Fills the TX FIFO with data
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  buffer  Pointer to buffer of bytes
 *  @param[in]  count   Number of bytes to fill TX FIFO from buffer
 *                      [0x00, 0xFFF]
 *
 *  @return     Number of bytes that were successfully written
 */
uint16_t DL_I2CC_fillTXFIFO(
    UNICOMM_Inst_Regs *unicomm, uint8_t *buffer, uint16_t count);

/**
 *  @brief      Flushes/removes all elements in the TX FIFO
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
void DL_I2CC_flushTXFIFO(UNICOMM_Inst_Regs *unicomm);

/**
 *  @brief      Flushes/removes all elements in the RX FIFO
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
void DL_I2CC_flushRXFIFO(UNICOMM_Inst_Regs *unicomm);

/**
 *  @brief      Stop TX FIFO flush
 *
 *  Before stopping the flush, check if @ref DL_I2CC_isTXFIFOEmpty,
 *  indicating flush is complete.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_stopFlushTXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->IFLS &= ~(UNICOMMI2CC_IFLS_TXCLR_MASK);
}

/**
 *  @brief      Start TX FIFO flush
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_startFlushTXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->IFLS |= UNICOMMI2CC_IFLS_TXCLR_MASK;
}

/**
 *  @brief      Stop RX FIFO flush
 *
 *  Before stopping the flush, check if @ref DL_I2CC_isRXFIFOEmpty,
 *  indicating flush is complete.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_stopFlushRXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->IFLS &= ~(UNICOMMI2CC_IFLS_RXCLR_MASK);
}

/**
 *  @brief      Start RX FIFO flush
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_startFlushRXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->IFLS |= UNICOMMI2CC_IFLS_RXCLR_MASK;
}

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
__STATIC_INLINE bool DL_I2CC_isTXFIFOFull(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->SR & UNICOMMI2CC_SR_TXFF_MASK) ==
            UNICOMMI2CC_SR_TXFF_SET);
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
__STATIC_INLINE bool DL_I2CC_isTXFIFOEmpty(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->SR & UNICOMMI2CC_SR_TXFE_MASK) ==
            UNICOMMI2CC_SR_TXFE_SET);
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
__STATIC_INLINE bool DL_I2CC_isRXFIFOFull(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->SR & UNICOMMI2CC_SR_RXFF_MASK) ==
            UNICOMMI2CC_SR_RXFF_SET);
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
__STATIC_INLINE bool DL_I2CC_isRXFIFOEmpty(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->SR & UNICOMMI2CC_SR_RXFE_MASK) ==
            UNICOMMI2CC_SR_RXFE_SET);
}

/**
 *  @brief      Reset transfers from from UNICOMMI2CC
 *
 *  Resets transfer register to initialize UNICOMMI2CC
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_resetTransfer(UNICOMM_Inst_Regs *unicomm)
{
    // Resets all functionality
    unicomm->i2cc->CTR = 0x00;
}

/**
 *  @brief      Sets up a transfer from UNICOMMI2CC
 *
 *  Set target address, transfer direction, burst length, START+STOP generation.
 *  @note   Reading/writing data must be done separately.
 *
 *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
 *  @param[in]  targetAddr  Target address [0x00, 0x3FF]
 *  @param[in]  direction   One of @ref DL_I2CC_DIRECTION
 *  @param[in]  length      Intended burst length in number of bytes
 *                          [0x00, 0xFFF]
 */
__STATIC_INLINE void DL_I2CC_startTransfer(UNICOMM_Inst_Regs *unicomm,
    uint32_t targetAddr, DL_I2CC_DIRECTION direction, uint16_t length)
{
    // Specify target address and read/write mode
    DL_Common_updateReg(&unicomm->i2cc->TA,
        ((targetAddr << UNICOMMI2CC_TA_ADDR_OFS) | (uint32_t) direction),
        (UNICOMMI2CC_TA_ADDR_MASK | UNICOMMI2CC_TA_DIR_MASK));

    // STOP bit is generated after burst length number of bytes transferred
    DL_Common_updateReg(&unicomm->i2cc->CTR,
        (((uint32_t) length << UNICOMMI2CC_CTR_BLEN_OFS) |
            UNICOMMI2CC_CTR_FRM_START_ENABLE | UNICOMMI2CC_CTR_START_ENABLE |
            UNICOMMI2CC_CTR_STOP_ENABLE),
        (UNICOMMI2CC_CTR_BLEN_MASK | UNICOMMI2CC_CTR_FRM_START_MASK |
            UNICOMMI2CC_CTR_START_MASK | UNICOMMI2CC_CTR_STOP_MASK));
}

/**
 *  @brief      Sets up a transfer from I2CC with control of START,
 *  STOP and ACK
 *
 *  @note   Reading/writing data must be done separately.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  targetAddr  7-bit target address [0x00, 0x7f]
 *  @param[in]  direction   One of @ref DL_I2CC_DIRECTION
 *  @param[in]  length      Intended burst length in number of bytes
 *  @param[in]  start       One of @ref DL_I2CC_START
 *  @param[in]  stop        One of @ref DL_I2CC_STOP
 *  @param[in]  ack         One of @ref DL_I2CC_ACK
 */
__STATIC_INLINE void DL_I2CC_startTransferAdvanced(UNICOMM_Inst_Regs *unicomm,
    uint32_t targetAddr, DL_I2CC_DIRECTION direction, uint16_t length,
    DL_I2CC_START start, DL_I2CC_STOP stop, DL_I2CC_ACK ack)
{
    // Specify target address and read/write mode
    DL_Common_updateReg(&unicomm->i2cc->TA,
        ((targetAddr << UNICOMMI2CC_TA_ADDR_OFS) | (uint32_t) direction),
        (UNICOMMI2CC_TA_ADDR_MASK | UNICOMMI2CC_TA_DIR_MASK));

    DL_Common_updateReg(&unicomm->i2cc->CTR,
        (((uint32_t) length << UNICOMMI2CC_CTR_BLEN_OFS) |
            UNICOMMI2CC_CTR_FRM_START_ENABLE | (uint32_t) start |
            (uint32_t) stop | (uint32_t) ack),
        (UNICOMMI2CC_CTR_BLEN_MASK | UNICOMMI2CC_INTCTL_INTEVAL_MASK |
            UNICOMMI2CC_CTR_START_MASK | UNICOMMI2CC_CTR_STOP_MASK |
            UNICOMMI2CC_CTR_ACK_MASK));
}

/**
 * @brief Enables power on UNICOMM I2C module
 *
 * @param[in] unicomm        Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_enablePower(UNICOMM_Inst_Regs *unicomm)
{
    DL_UNICOMM_enablePower(unicomm);
    if (unicomm->fixedMode == false) {
        DL_UNICOMM_setIPMode(unicomm, DL_UNICOMM_I2C_CONTROLLER);
    }
}

/**
 * @brief Disables power on UNICOMM I2C module
 *
 * @param[in] unicomm        Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_disablePower(UNICOMM_Inst_Regs *unicomm)
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
__STATIC_INLINE bool DL_I2CC_isPowerEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return DL_UNICOMM_isPowerEnabled(unicomm);
}

/**
 * @brief Resets UNICOMM I2C peripheral
 *
 * @param[in] unicomm        Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_reset(UNICOMM_Inst_Regs *unicomm)
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
__STATIC_INLINE bool DL_I2CC_isReset(UNICOMM_Inst_Regs *unicomm)
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
 *  @param[in]  clockSource    One of @ref DL_I2CC_CLOCK.
 *
 */
__STATIC_INLINE void DL_I2CC_selectClockSource(
    UNICOMM_Inst_Regs *unicomm, DL_I2CC_CLOCK clockSource)
{
    DL_Common_updateReg(&unicomm->i2cc->CLKSEL, (uint32_t) clockSource,
        UNICOMMI2CC_CLKSEL_BUSCLK_SEL_MASK |
            UNICOMMI2CC_CLKSEL_MFCLK_SEL_MASK);
}

/**
 * @brief Set Clock Divider
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  clockDivider   One of @ref DL_I2CC_CLOCK_DIVIDE.
 *
 */
__STATIC_INLINE void DL_I2CC_selectClockDivider(
    UNICOMM_Inst_Regs *unicomm, DL_I2CC_CLOCK_DIVIDE clockDivider)
{
    DL_Common_updateReg(&unicomm->i2cc->CLKDIV, (uint32_t) clockDivider,
        UNICOMMI2CC_CLKDIV_RATIO_MASK);
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
 *  @retval     One of @ref DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH.
 */
__STATIC_INLINE DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH
DL_I2CC_getDigitalGlitchFilterPulseWidth(UNICOMM_Inst_Regs *unicomm)
{
    uint32_t filterWidth =
        unicomm->i2cc->GFCTL & UNICOMMI2CC_GFCTL_DGFSEL_MASK;

    return (DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH)(filterWidth);
}

/**
 *  @brief      Set Digital Glitch Suppression Pulse Width
 *
 *  Pulse width for the digital glitch suppression on SCL/SDA lines.
 *  Values are in terms of functional clock ticks.
 *
 *  @param[in]  unicomm             Pointer to the register overlay for the
 *                              peripheral
 *  @param[in]  filterWidth     One of @ref DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH.
 */
__STATIC_INLINE void DL_I2CC_setDigitalGlitchFilterPulseWidth(
    UNICOMM_Inst_Regs *unicomm,
    DL_I2CC_DIGITAL_GLITCH_FILTER_WIDTH filterWidth)
{
    DL_Common_updateReg(&unicomm->i2cc->GFCTL, (uint32_t) filterWidth,
        UNICOMMI2CC_GFCTL_DGFSEL_MASK);
}

/**
 *  @brief      Disable Analog Glitch Suppression
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_disableAnalogGlitchFilter(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->GFCTL &= ~(UNICOMMI2CC_GFCTL_AGFEN_MASK);
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
__STATIC_INLINE bool DL_I2CC_isAnalogGlitchFilterEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->GFCTL & UNICOMMI2CC_GFCTL_AGFEN_MASK) ==
            UNICOMMI2CC_GFCTL_AGFEN_ENABLE);
}

/**
 *  @brief      Enable Analog Glitch Suppression
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_enableAnalogGlitchFilter(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->GFCTL |= UNICOMMI2CC_GFCTL_AGFEN_ENABLE;
}

/**
 *  @brief      Get direction of next operation
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     Direction of next operation
 *
 *  @retval     One of @ref DL_I2CC_DIRECTION
 */
__STATIC_INLINE DL_I2CC_DIRECTION DL_I2CC_getDirection(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t direction = unicomm->i2cc->TA & UNICOMMI2CC_TA_DIR_MASK;

    return (DL_I2CC_DIRECTION)(direction);
}

/**
 *  @brief      Set direction of next operation
 *
 *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
 *
 *  @param[in]  direction   Direction of next operation.
 *                          One of @ref DL_I2CC_DIRECTION.
 */
__STATIC_INLINE void DL_I2CC_setDirection(
    UNICOMM_Inst_Regs *unicomm, DL_I2CC_DIRECTION direction)
{
    DL_Common_updateReg(
        &unicomm->i2cc->TA, (uint32_t) direction, UNICOMMI2CC_TA_DIR_MASK);
}

/**
 *  @brief      Get the address of the target being addressed when configured
 *              as an UNICOMMI2CC
 *
 *  Specifies bits A9 through A0 of the target address.
 *  In 7-bit addressing mode as selected by @ref DL_I2CC_setAddressingMode,
 *  the top 3 bits are don't care.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     The target address
 *
 *  @retval     [0x00, 0x07FE]
 *
 *  @sa         DL_I2CC_setAddressingMode
 */
__STATIC_INLINE uint32_t DL_I2CC_getTargetAddress(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->TA & UNICOMMI2CC_TA_ADDR_MASK) >>
            UNICOMMI2CC_TA_ADDR_OFS);
}

/**
 *  @brief      Set the address of the target being addressed when configured
 *              as an UNICOMMI2CC
 *
 *  Specifies bits A9 through A0 of the target address.
 *  In 7-bit addressing mode as selected by @ref DL_I2CC_setAddressingMode,
 *  the top 3 bits are don't care.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  targetAddress    Target address to set [0x00, 0x07FE]
 *
 *  @sa         DL_I2CC_setAddressingMode
 */
__STATIC_INLINE void DL_I2CC_setTargetAddress(
    UNICOMM_Inst_Regs *unicomm, uint32_t targetAddress)
{
    DL_Common_updateReg(&unicomm->i2cc->TA,
        (targetAddress << UNICOMMI2CC_TA_ADDR_OFS), UNICOMMI2CC_TA_ADDR_MASK);
}

/**
 *  @brief      Get addressing mode
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     Addressing mode the is set to
 *
 *  @retval     One of @ref DL_I2CC_ADDRESSING_MODE
 *
 */
__STATIC_INLINE DL_I2CC_ADDRESSING_MODE DL_I2CC_getAddressingMode(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t mode = unicomm->i2cc->TA & UNICOMMI2CC_TA_MODE_MASK;

    return (DL_I2CC_ADDRESSING_MODE)(mode);
}

/**
 *  @brief      Set addressing mode between 7-bit and 10-bit mode
 *
 *  Selects the addressing mode between 7-bit and 10-bit mode to be used when
 *  device is configured as a
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  mode    Addressing mode to set the target to.
 *                      One of @ref DL_I2CC_ADDRESSING_MODE.
 */
__STATIC_INLINE void DL_I2CC_setAddressingMode(
    UNICOMM_Inst_Regs *unicomm, DL_I2CC_ADDRESSING_MODE mode)
{
    DL_Common_updateReg(
        &unicomm->i2cc->TA, (uint32_t) mode, UNICOMMI2CC_TA_MODE_MASK);
}

/**
 *  @brief      Disable ACK override
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_disableACKOverride(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CTR &= ~(UNICOMMI2CC_CTR_ACKOEN_MASK);
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
__STATIC_INLINE bool DL_I2CC_isACKOverrideEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->CTR & UNICOMMI2CC_CTR_ACKOEN_MASK) ==
            UNICOMMI2CC_CTR_ACKOEN_ENABLE);
}

/**
 *  @brief      Enable ACK override
 *
 *  When enabled and the is receiving data and the number of bytes
 *  indicated in CTR.MBLEN have been received, the state machine will generate
 *  an RXDONE interrupt and wait at the start of the ACK for FW to indicate if
 *  an ACK or NACK should be sent. The ACK or NACK is selected by writing the
 *  CTR register and setting ACK accordingly. The other fields in this register
 *  can also be written at this time to continue on with the transaction. If a
 *  NACK is sent the state machine will automatically send a Stop.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 * @sa          DL_I2CC_setTransactionLength
 */
__STATIC_INLINE void DL_I2CC_enableACKOverride(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CTR |= UNICOMMI2CC_CTR_ACKOEN_ENABLE;
}

/**
 *  @brief      Disable read on TX empty
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_disableReadOnTXEmpty(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CTR &= ~(UNICOMMI2CC_CTR_RD_ON_TXEMPTY_MASK);
}

/**
 *  @brief      Checks if read on TX empty is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If read on TX empty is enabled
 *
 *  @retval     true if read on TX empty is enabled
 *  @retval     false if read on TX empty is disabled
 */
__STATIC_INLINE bool DL_I2CC_isReadOnTXEmptyEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->CTR & UNICOMMI2CC_CTR_RD_ON_TXEMPTY_MASK) ==
            UNICOMMI2CC_CTR_RD_ON_TXEMPTY_ENABLE);
}

/**
 *  @brief      Enable read on TX empty
 *
 *  When enabled, the will transmit all bytes from the TX FIFO
 *  before continuing with the programmed Burst Run Read. If the DIR is not
 *  set to Read in the MSA then this bit is ignored. The Start must be set in
 *  the CTR for proper I2CC protocol. The will first send the Start
 *  Condition, I2CC Address with R/W bit set to write, before sending the bytes
 *  in the TX FIFO. When the TX FIFO is empty, the I2CC transaction will
 *  continue as programmed in MTCR and MSA without sending a Stop Condition.
 *  This is intended to be used to perform simple I2CC command based reads
 *  transition that will complete after initiating them without having to get
 *  an interrupt to turn the bus around.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 */
__STATIC_INLINE void DL_I2CC_enableReadOnTXEmpty(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CTR |= UNICOMMI2CC_CTR_RD_ON_TXEMPTY_ENABLE;
}

/**
 *  @brief      Get the SMBus/PMBus Packet Error Checking (PEC) count value
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     The value the PEC count is set to
 *
 *  @retval     Value between [0x0, 0x01FF]
 */
__STATIC_INLINE uint32_t DL_I2CC_getPECCountValue(UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->i2cc->PECCTL & UNICOMMI2CC_PECCTL_PECCNT_MASK);
}

/**
 *  @brief      Set the SMBus/PMBus Packet Error Checking (PEC) count value
 *
 *  When this field is non zero, the number of I2CC data bytes are counted.
 *  Refer to the device TRM for more details.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  count   The value to set the PEC count to.
 *                      Value between [0x0, 0x01FF]
 */
__STATIC_INLINE void DL_I2CC_setPECCountValue(
    UNICOMM_Inst_Regs *unicomm, uint32_t count)
{
    DL_Common_updateReg(
        &unicomm->i2cc->PECCTL, count, UNICOMMI2CC_PECCTL_PECCNT_MASK);
}

/**
 *  @brief      Disable SMBus/PMBus Packet Error Checking (PEC)
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_disablePEC(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->PECCTL &= ~(UNICOMMI2CC_PECCTL_PECEN_MASK);
}

/**
 *  @brief      Checks if SMBus/PMBus Packet Error Checking (PEC)
 *              is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If PEC is enabled
 *
 *  @retval     true if PEC is enabled
 *  @retval     false if PEC is disabled
 */
__STATIC_INLINE bool DL_I2CC_isPECEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->PECCTL & UNICOMMI2CC_PECCTL_PECEN_MASK) ==
            UNICOMMI2CC_PECCTL_PECEN_ENABLE);
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
__STATIC_INLINE void DL_I2CC_enablePEC(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->PECCTL |= UNICOMMI2CC_PECCTL_PECEN_ENABLE;
}

/**
 *  @brief      Get the current SMBus/PMBus PEC byte count of the
 *              state machine
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     The current PEC byte count of the state machine
 *
 *  @retval     Value between [0x0, 0x01FF]
 */
__STATIC_INLINE uint32_t DL_I2CC_getCurrentPECCount(UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->i2cc->PECSR & UNICOMMI2CC_PECSR_PECSTS_CHECK_MASK);
}

/**
 *  @brief      If SMBus/PMBus PEC was checked in last transaction
 *
 *  The status of if the PEC was checked in the transaction that
 *  occurred before the last Stop. Latched on Stop.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     Status of PEC checked in last transaction
 *
 *  @retval     One of @ref DL_I2CC_PEC_STATUS
 */
__STATIC_INLINE DL_I2CC_PEC_STATUS DL_I2CC_getPECCheckedStatus(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t status =
        unicomm->i2cc->PECSR & UNICOMMI2CC_PECSR_PECSTS_CHECK_MASK;

    return (DL_I2CC_PEC_STATUS)(status);
}

/**
 *  @brief      Get the status of the SMBus/PMBus PEC Check error
 *
 *  The status of if a PEC check error occurred in the transaction that
 *  occurred before the last Stop. Latched on Stop.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     Status of PEC check error
 *
 *  @retval     One of @ref DL_I2CC_PEC_CHECK_ERROR
 */
__STATIC_INLINE DL_I2CC_PEC_CHECK_ERROR DL_I2CC_getPECCheckError(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t error =
        unicomm->i2cc->PECSR & UNICOMMI2CC_PECSR_PECSTS_ERROR_MASK;

    return (DL_I2CC_PEC_CHECK_ERROR)(error);
}

/**
 *  @brief      Disable I2CC START generation
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_disableStartCondition(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CTR &= ~(UNICOMMI2CC_CTR_START_MASK);
}

/**
 *  @brief      Checks if I2CC START generation is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If I2CC START generation is enabled
 *
 *  @retval     true if I2CC START generation is enabled
 *  @retval     false if I2CC START generation is disabled
 */
__STATIC_INLINE bool DL_I2CC_isStartConditionEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->CTR & UNICOMMI2CC_CTR_START_MASK) ==
            UNICOMMI2CC_CTR_START_ENABLE);
}

/**
 *  @brief      Enable I2CC START generation
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_enableStartCondition(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CTR |= UNICOMMI2CC_CTR_START_ENABLE;
}

/**
 *  @brief      Disable I2CC STOP generation
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_disableStopCondition(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CTR &= ~(UNICOMMI2CC_CTR_STOP_MASK);
}

/**
 *  @brief      Checks if I2CC STOP generation is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If I2CC STOP generation is enabled
 *
 *  @retval     true if I2CC STOP generation is enabled
 *  @retval     false if I2CC STOP generation is disabled
 */
__STATIC_INLINE bool DL_I2CC_isStopConditionEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->CTR & UNICOMMI2CC_CTR_STOP_MASK) ==
            UNICOMMI2CC_CTR_STOP_ENABLE);
}

/**
 *  @brief      Enable I2CC STOP generation
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_enableStopCondition(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CTR |= UNICOMMI2CC_CTR_STOP_ENABLE;
}

/**
 *  @brief      Disable I2CC data acknowledge (ACK or NACK)
 *
 *  When the I2CC module operates in receiver mode, the ACK bit can be
 *  cleared when no further data needs to be received from the target
 *  transmitter.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_disableACK(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CTR &= ~(UNICOMMI2CC_CTR_ACK_MASK);
}

/**
 *  @brief      Checks if I2CC data acknowledge (ACK or NACK) is
 *              enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If I2CC data acknowledge is enabled
 *
 *  @retval     true if I2CC data acknowledge is enabled
 *  @retval     false if I2CC data acknowledge is disabled
 */
__STATIC_INLINE bool DL_I2CC_isACKEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->CTR & UNICOMMI2CC_CTR_ACK_MASK) ==
            UNICOMMI2CC_CTR_ACK_ENABLE);
}

/**
 *  @brief      Enable I2CC data acknowledge (ACK or NACK)
 *
 *  When the I2CC module operates in receiver mode, the ACK bit is
 *  normally set causing the I2CC bus to transmit an acknowledge
 *  automatically after each byte.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_enableACK(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CTR |= UNICOMMI2CC_CTR_ACK_MASK;
}

/**
 *  @brief      Get transaction length in bytes
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     Length of I2CC transaction in bytes
 *
 *  @retval     [0x000, 0xfff]
 */
__STATIC_INLINE uint32_t DL_I2CC_getTransactionLength(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->CTR & UNICOMMI2CC_CTR_BLEN_MASK) >>
            UNICOMMI2CC_CTR_BLEN_OFS);
}

/**
 *  @brief      Set transaction length in bytes
 *
 *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
 *
 *  @param[in]  length      Length of I2CC transaction in bytes [0x000, 0xfff]
 */
__STATIC_INLINE void DL_I2CC_setTransactionLength(
    UNICOMM_Inst_Regs *unicomm, uint32_t length)
{
    DL_Common_updateReg(&unicomm->i2cc->CTR,
        (length << UNICOMMI2CC_CTR_BLEN_OFS), UNICOMMI2CC_CTR_BLEN_MASK);
}

/**
 *  @brief      Get status of I2CC bus for
 *
 *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
 *
 *  @return     Status of I2CC bus for
 *
 *  @retval     Bitwise OR of @ref DL_I2CC_STATUS
 */
__STATIC_INLINE uint32_t DL_I2CC_getStatus(UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->i2cc->SR);
}

/**
 *  @brief      Get transaction count in bytes
 *
 *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
 *
 *  @return     Current 12-bit countdown value of the transaction
 *
 *  @retval     [0x000, 0xfff]
 */
__STATIC_INLINE uint16_t DL_I2CC_getTransactionCount(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((uint16_t)((unicomm->i2cc->SR & UNICOMMI2CC_SR_BCNT_MASK) >>
                       UNICOMMI2CC_SR_BCNT_OFS));
}

/**
 *  @brief      Get byte of data from UNICOMMI2CC
 *
 *  If using FIFO, it is first byte from the RX FIFO.
 *
 *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
 *
 *  @return     Last received byte of data
 *
 *  @retval     [0x00, 0xff]
 */
__STATIC_INLINE uint8_t DL_I2CC_receiveData(UNICOMM_Inst_Regs *unicomm)
{
    return ((uint8_t)(unicomm->i2cc->RXDATA & UNICOMMI2CC_RXDATA_DATA_MASK));
}

/**
 *  @brief      Set next byte to be transferred during the next transaction
 *
 *  Does not transmit data until @ref DL_I2CC_startTransfer
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  data    Data to be transferred during the next transaction.
 *                      [0x00, 0xff]
 */
__STATIC_INLINE void DL_I2CC_transmitData(
    UNICOMM_Inst_Regs *unicomm, uint8_t data)
{
    unicomm->i2cc->TXDATA = data;
}

/**
 *  @brief      Get timer period
 *  This field is used in the equation to configure SCL_PERIOD:
 *
 *  SCL_PERIOD = (1 + TPR) * (SCL_LP + SCL_HP) * INT_CLK_PRD
 *
 *  where:
 *  SCL_PRD is the SCL line period (I2CC clock)
 *
 *  TPR is the Timer Period register value (range of 1 to 127)
 *
 *  SCL_LP is the SCL Low period (fixed at 6)
 *  SCL_HP is the SCL High period (fixed at 4)
 *
 *  CLK_PRD is the functional clock period in ns
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @return     Timer period
 *
 *  @retval     [0x00, 0x7f]
 */
__STATIC_INLINE uint8_t DL_I2CC_getTimerPeriod(UNICOMM_Inst_Regs *unicomm)
{
    return ((uint8_t)(unicomm->i2cc->TPR & UNICOMMI2CC_TPR_TPR_MASK));
}

/**
 *  @brief      Set timer period
 *  This field is used in the equation to configure SCL_PERIOD:
 *
 *  SCL_PERIOD = (1 + TPR) * (SCL_LP + SCL_HP) * INT_CLK_PRD
 *
 *  where:
 *  SCL_PRD is the SCL line period (I2CC clock)
 *
 *  TPR is the Timer Period register value (range of 1 to 127)
 *
 *  SCL_LP is the SCL Low period (fixed at 6)
 *  SCL_HP is the SCL High period (fixed at 4)
 *
 *  CLK_PRD is the functional clock period in ns
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  period  Timer period [0x00, 0x7f]
 */
__STATIC_INLINE void DL_I2CC_setTimerPeriod(
    UNICOMM_Inst_Regs *unicomm, uint8_t period)
{
    unicomm->i2cc->TPR = period;
}

/**
 *  @brief      Disable multimode
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_disableMultiMode(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CR &= ~(UNICOMMI2CC_CR_MCTL_MASK);
}

/**
 *  @brief      Checks if multimode is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If multimode is enabled
 *
 *  @retval     true if multimode is enabled
 *  @retval     false if multimode is disabled
 */
__STATIC_INLINE bool DL_I2CC_isMultiModeEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->CR & UNICOMMI2CC_CR_MCTL_MASK) ==
            UNICOMMI2CC_CR_MCTL_ENABLE);
}

/**
 *  @brief      Enable multimode
 *
 *  In multimode, the SCL high time counts once the SCL line has been
 *  detected high. If this is not enabled, the high time counts as soon as the
 *  SCL line has been set high by the UNICOMMI2CC
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_enableMultiMode(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CR |= UNICOMMI2CC_CR_MCTL_ENABLE;
}

/**
 *  @brief      Disable controller
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_disable(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CR &= ~(UNICOMMI2CC_CR_ENABLE_MASK);
}

/**
 *  @brief      Checks if controller is enabled
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *
 *  @return     If is enabled
 *
 *  @retval     true if is enabled
 *  @retval     false if is disabled
 */
__STATIC_INLINE bool DL_I2CC_isEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->CR & UNICOMMI2CC_CR_ENABLE_MASK) ==
            UNICOMMI2CC_CR_ENABLE_ENABLE);
}

/**
 *  @brief      Enable controller
 *
 *  After has enabled, it should not be re-enabled unless it has been
 *  disabled or by a reset, otherwise transfer failures may occur.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_enable(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CR |= UNICOMMI2CC_CR_ENABLE_ENABLE;
}

/**
 *  @brief      Disable clock stretching
 *
 *  Clock stretching can be disabled if no target on the bus supports clock
 *  stretching; however, it should be typically enabled to be compliant with
 *  I2CC specification.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_disableClockStretching(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CR &= ~(UNICOMMI2CC_CR_CLKSTRETCH_MASK);
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
__STATIC_INLINE bool DL_I2CC_isClockStretchingEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->CR & UNICOMMI2CC_CR_CLKSTRETCH_MASK) ==
            UNICOMMI2CC_CR_CLKSTRETCH_ENABLE);
}

/**
 *  @brief      Enable clock stretching
 *
 *  Clock stretching can be disabled if no target on the bus supports clock
 *  stretching; however, it should be typically enabled to be compliant with UNICOMMI2CC
 *  specification.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_enableClockStretching(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->CR |= UNICOMMI2CC_CR_CLKSTRETCH_ENABLE;
}

/**
 *  @brief      Get SCL signal status
 *
 *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
 *
 *  @return     Status of SCL signal
 *
 *  @retval     One of @ref DL_I2CC_SCL
 */
__STATIC_INLINE DL_I2CC_SCL DL_I2CC_getSCLStatus(UNICOMM_Inst_Regs *unicomm)
{
    uint32_t sclStatus = unicomm->i2cc->BMON & UNICOMMI2CC_BMON_SCL_MASK;

    return (DL_I2CC_SCL)(sclStatus);
}

/**
 *  @brief      Get SDA signal status
 *
 *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
 *
 *  @return     Status of SDA signal
 *
 *  @retval     One of @ref DL_I2CC_SDA
 */
__STATIC_INLINE DL_I2CC_SDA DL_I2CC_getSDAStatus(UNICOMM_Inst_Regs *unicomm)
{
    uint32_t sdaStatus = unicomm->i2cc->BMON & UNICOMMI2CC_BMON_SDA_MASK;

    return (DL_I2CC_SDA)(sdaStatus);
}

/**
 *  @brief      Get TX FIFO threshold level
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @return     Indicates at what fill level in the TX FIFO a threshold will be
 *              generated
 *
 *  @retval     One of @ref DL_I2CC_TX_FIFO_LEVEL
 */
__STATIC_INLINE DL_I2CC_TX_FIFO_LEVEL DL_I2CC_getTXFIFOThreshold(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t level = unicomm->i2cc->IFLS & UNICOMMI2CC_IFLS_TXIFLSEL_MASK;

    return (DL_I2CC_TX_FIFO_LEVEL)(level);
}

/**
 *  @brief      Set TX FIFO threshold level
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  level   Indicates at what fill level in the TX FIFO a threshold
 *                      will be generated.
 *                      One of @ref DL_I2CC_TX_FIFO_LEVEL.
 */
__STATIC_INLINE void DL_I2CC_setTXFIFOThreshold(
    UNICOMM_Inst_Regs *unicomm, DL_I2CC_TX_FIFO_LEVEL level)
{
    DL_Common_updateReg(&unicomm->i2cc->IFLS, (uint32_t) level,
        UNICOMMI2CC_IFLS_TXIFLSEL_MASK);
}

/**
 *  @brief      Get RX FIFO threshold level
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @return     Indicates at what fill level in the RX FIFO a threshold will be
 *              generated
 *
 *  @retval     One of @ref DL_I2CC_RX_FIFO_LEVEL
 */
__STATIC_INLINE DL_I2CC_RX_FIFO_LEVEL DL_I2CC_getRXFIFOThreshold(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t level = unicomm->i2cc->IFLS & UNICOMMI2CC_IFLS_RXIFLSEL_MASK;

    return (DL_I2CC_RX_FIFO_LEVEL)(level);
}

/**
 *  @brief      Set RX FIFO threshold level
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  level   Indicates at what fill level in the RX FIFO a threshold
 *                      will be generated.
 *                      One of @ref DL_I2CC_RX_FIFO_LEVEL.
 */
__STATIC_INLINE void DL_I2CC_setRXFIFOThreshold(
    UNICOMM_Inst_Regs *unicomm, DL_I2CC_RX_FIFO_LEVEL level)
{
    DL_Common_updateReg(&unicomm->i2cc->IFLS, (uint32_t) level,
        UNICOMMI2CC_IFLS_RXIFLSEL_MASK);
}

/**
 *  @brief      Get number of bytes which can be read from RX FIFO
 *
 *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
 *
 *  @return     Number of bytes which can be read from RX FIFO
 *
 *  @retval     [0x0, 0x8]
 */
__STATIC_INLINE uint32_t DL_I2CC_getRXFIFOCounter(UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->i2cc->SR & UNICOMMI2CC_SR_BCNT_MASK);
}

/**
 *  @brief      Get number of bytes which can be put into TX FIFO
 *
 *  @param[in]  unicomm         Pointer to the register overlay for the peripheral
 *
 *  @return     Number of bytes which can be put into TX FIFO
 *
 *  @retval     [0x0, 0x8]
 */
__STATIC_INLINE uint32_t DL_I2CC_getTXFIFOCounter(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->i2cc->SR & UNICOMMI2CC_SR_BCNT_MASK) >>
            UNICOMMI2CC_SR_BCNT_OFS);
}

/**
 *  @brief      Enable I2CC interrupts
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_I2CC_INTERRUPT.
 */
__STATIC_INLINE void DL_I2CC_enableInterrupt(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    unicomm->i2cc->CPU_INT.IMASK |= interruptMask;
}

/**
 *  @brief      Disable I2CC interrupts
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to disable. Bitwise OR of
 *                             @ref DL_I2CC_INTERRUPT.
 */
__STATIC_INLINE void DL_I2CC_disableInterrupt(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    unicomm->i2cc->CPU_INT.IMASK &= ~(interruptMask);
}

/**
 *  @brief      Check which I2CC interrupts are enabled
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_I2CC_INTERRUPT.
 *
 *  @return     Which of the requested I2CC interrupts are enabled
 *
 *  @retval     Bitwise OR of @ref DL_I2CC_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_I2CC_getEnabledInterrupts(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    return (unicomm->i2cc->CPU_INT.IMASK & interruptMask);
}

/**
 *  @brief      Check interrupt flag of enabled I2CC interrupts
 *
 *  Checks if any of the I2CC interrupts that were previously enabled are
 *  pending.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_I2CC_INTERRUPT.
 *
 *  @return     Which of the requested I2CC interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_I2CC_INTERRUPT values
 *
 *  @sa         DL_I2CC_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_I2CC_getEnabledInterruptStatus(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    return (unicomm->i2cc->CPU_INT.MIS & interruptMask);
}

/**
 *  @brief      Check interrupt flag of any I2CC interrupt
 *
 *  Checks if any of the I2CC interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_I2CC_INTERRUPT.
 *
 *  @return     Which of the requested I2CC interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_I2CC_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_I2CC_getRawInterruptStatus(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    return (unicomm->i2cc->CPU_INT.RIS & interruptMask);
}

/**
 *  @brief      Get highest priority pending I2CC interrupt
 *
 *  Checks if any of the I2CC interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     The highest priority pending I2CC interrupt
 *
 *  @retval     One of @ref DL_I2CC_IIDX
 */
__STATIC_INLINE DL_I2CC_IIDX DL_I2CC_getPendingInterrupt(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((DL_I2CC_IIDX) unicomm->i2cc->CPU_INT.IIDX);
}

/**
 *  @brief      Clear pending I2CC interrupts
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of
 *                             @ref DL_I2CC_INTERRUPT.
 */
__STATIC_INLINE void DL_I2CC_clearInterruptStatus(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    unicomm->i2cc->CPU_INT.ICLR = interruptMask;
}

/**
 *  @brief Enables I2CC interrupt for triggering DMA events
 *
 * Enables the I2CC interrupt to be used as the condition to generate an
 * event to directly trigger the DMA.
 *
 * Each event publisher @ref DL_I2CC_EVENT_ROUTE can set any one of
 * @ref DL_I2CC_DMA_INTERRUPT.
 *
 * @note Only one interrupt source should be enabled at a time.
 *
 *  @param[in]  unicomm        Pointer to the register overlay for the peripheral
 *  @param[in]  index      Specifies the register event publisher to configure
 *  @param[in]  interrupt  Interrupt to enable as the trigger condition for
 *                         the DMA. One of @ref DL_I2CC_DMA_INTERRUPT.
 */
__STATIC_INLINE void DL_I2CC_enableDMAEvent(
    UNICOMM_Inst_Regs *unicomm, DL_I2CC_EVENT_ROUTE index, uint32_t interrupt)
{
    switch (index) {
        case DL_I2CC_EVENT_ROUTE_1:
            unicomm->i2cc->DMA_TRIG1.IMASK = interrupt;
            break;
        case DL_I2CC_EVENT_ROUTE_2:
            unicomm->i2cc->DMA_TRIG0.IMASK = interrupt;
            break;
        default:
            break;
    }
}

/**
 *  @brief Disables I2CC interrupt from triggering DMA events
 *
 * Disables the I2CC interrupt from being used as the condition to generate an
 * event to directly trigger the DMA.
 *
 * Each event publisher @ref DL_I2CC_EVENT_ROUTE can set any one of
 * @ref DL_I2CC_DMA_INTERRUPT.
 *
 *  @param[in]  unicomm        Pointer to the register overlay for the peripheral
 *  @param[in]  index      Specifies the register event publisher to configure
 *  @param[in]  interrupt  Interrupt to disable as the trigger condition for
 *                         the DMA. One of @ref DL_I2CC_DMA_INTERRUPT.
 */
__STATIC_INLINE void DL_I2CC_disableDMAEvent(
    UNICOMM_Inst_Regs *unicomm, DL_I2CC_EVENT_ROUTE index, uint32_t interrupt)
{
    switch (index) {
        case DL_I2CC_EVENT_ROUTE_1:
            unicomm->i2cc->DMA_TRIG1.IMASK &= ~(interrupt);
            break;
        case DL_I2CC_EVENT_ROUTE_2:
            unicomm->i2cc->DMA_TRIG0.IMASK &= ~(interrupt);
            break;
        default:
            break;
    }
}

/**
 *  @brief Check which I2CC interrupt for DMA receive events is enabled
 *
 *  This API checks the event publisher register as selected by
 *  @ref DL_I2CC_EVENT_ROUTE, which are used for triggering the DMA for
 *  or Target and receive or transmit events.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the peripheral
 *  @param[in]  index          Specifies the register event publisher to configure
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_I2CC_DMA_INTERRUPT.
 *
 *  @note Only one interrupt source should be enabled at a time.
 *
 *  @return     The requested I2CC interrupt status
 *
 *  @retval     One of @ref DL_I2CC_DMA_INTERRUPT
 */
__STATIC_INLINE uint32_t DL_I2CC_getEnabledDMAEvents(
    UNICOMM_Inst_Regs *unicomm, DL_I2CC_EVENT_ROUTE index,
    uint32_t interruptMask)
{
    volatile uint32_t *pReg = &unicomm->i2cc->DMA_TRIG1.IMASK;

    return ((*(pReg + (uint32_t) index) & interruptMask));
}

/**
 *  @brief Check interrupt flag of enabled I2CC interrupt for DMA event
 *
 *  Checks if any of the I2CC interrupts for the DMA receive event that were
 *  previously enabled are pending.
 *  This API checks the event publisher register as selected by
 *  @ref DL_I2CC_EVENT_ROUTE, which are used for triggering the DMA for
 *  or Target and receive or transmit events.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the peripheral
 *  @param[in]  index          Specifies the register event publisher to configure
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_I2CC_DMA_INTERRUPT.
 *
 *  @note Only one interrupt source should be enabled at a time.
 *
 *  @return     The requested I2CC interrupt status
 *
 *  @retval     One of @ref DL_I2CC_DMA_INTERRUPT
 *
 *  @sa         DL_I2CC_enableDMAEvent
 */
__STATIC_INLINE uint32_t DL_I2CC_getEnabledDMAEventStatus(
    UNICOMM_Inst_Regs *unicomm, DL_I2CC_EVENT_ROUTE index,
    uint32_t interruptMask)
{
    const volatile uint32_t *pReg = &unicomm->i2cc->DMA_TRIG1.MIS;

    return ((*(pReg + (uint32_t) index) & interruptMask));
}

/**
 *  @brief Check interrupt flag of any I2CC interrupt for DMA event
 *
 *  Checks if any of the I2CC interrupts for DMA receive event are pending.
 *  Interrupts do not have to be previously enabled.
 *  This API checks the event publisher register as selected by
 *  @ref DL_I2CC_EVENT_ROUTE, which are used for triggering the DMA for
 *  or Target and receive or transmit events.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the peripheral
 *  @param[in]  index          Specifies the register event publisher to configure
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_I2CC_DMA_INTERRUPT.
 *
 *  @return     Which of the requested I2CC interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_I2CC_DMA_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_I2CC_getRawDMAEventStatus(
    UNICOMM_Inst_Regs *unicomm, DL_I2CC_EVENT_ROUTE index,
    uint32_t interruptMask)
{
    const volatile uint32_t *pReg = &unicomm->i2cc->DMA_TRIG1.RIS;

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
__STATIC_INLINE uint32_t DL_I2CC_getTimeoutACount(UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->i2cc->TIMEOUT_CTL & UNICOMMI2CC_TIMEOUT_CTL_TCNTLA_MASK);
}

/**
 *  @brief      Set the Timeout Counter A value
 *
 *  Timeout A is used for SCL low detection. This field contains the upper 8
 *  bits of a 12-bit pre-load value for the Timeout A count.
 *  NOTE: The value of the counter must be greater than 1 to enable the
 *  timeout. Each count is equal to 520 times the timeout period of the
 *  functional clock. For example, with 8MHz functional clock and a
 *  100KHz operating I2CC clock, one timeout period will be equal to
 *  (1 / 8MHz) * 520 = 65us.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the
 *                      peripheral
 *  @param[in]  count   The value to set the Timeout A counter to.
 *                      Value between [0x0, 0xFF]
 */
__STATIC_INLINE void DL_I2CC_setTimeoutACount(
    UNICOMM_Inst_Regs *unicomm, uint32_t count)
{
    DL_Common_updateReg(&unicomm->i2cc->TIMEOUT_CTL, count,
        UNICOMMI2CC_TIMEOUT_CTL_TCNTLA_MASK);
}

/**
 *  @brief      Disable Timeout Counter A
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_disableTimeoutA(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->TIMEOUT_CTL &= ~(UNICOMMI2CC_TIMEOUT_CTL_TCNTAEN_MASK);
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
__STATIC_INLINE bool DL_I2CC_isTimeoutAEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return (
        (unicomm->i2cc->TIMEOUT_CTL & UNICOMMI2CC_TIMEOUT_CTL_TCNTAEN_MASK) ==
        UNICOMMI2CC_TIMEOUT_CTL_TCNTAEN_ENABLE);
}

/**
 *  @brief      Enable Timeout Counter A
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_enableTimeoutA(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->TIMEOUT_CTL |= UNICOMMI2CC_TIMEOUT_CTL_TCNTAEN_ENABLE;
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
__STATIC_INLINE uint32_t DL_I2CC_getCurrentTimeoutACounter(
    UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->i2cc->TIMEOUT_CNT & UNICOMMI2CC_TIMEOUT_CNT_TCNTA_MASK);
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
__STATIC_INLINE uint32_t DL_I2CC_getTimeoutBCount(UNICOMM_Inst_Regs *unicomm)
{
    return (
        (unicomm->i2cc->TIMEOUT_CTL & UNICOMMI2CC_TIMEOUT_CTL_TCNTLB_MASK) >>
        UNICOMMI2CC_TIMEOUT_CTL_TCNTLB_OFS);
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
__STATIC_INLINE void DL_I2CC_setTimeoutBCount(
    UNICOMM_Inst_Regs *unicomm, uint32_t count)
{
    DL_Common_updateReg(&unicomm->i2cc->TIMEOUT_CTL,
        (count << UNICOMMI2CC_TIMEOUT_CTL_TCNTLB_OFS),
        UNICOMMI2CC_TIMEOUT_CTL_TCNTLB_MASK);
}

/**
 *  @brief      Disable Timeout Counter B
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_disableTimeoutB(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->TIMEOUT_CTL &= ~(UNICOMMI2CC_TIMEOUT_CTL_TCNTBEN_MASK);
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
__STATIC_INLINE bool DL_I2CC_isTimeoutBEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return (
        (unicomm->i2cc->TIMEOUT_CTL & UNICOMMI2CC_TIMEOUT_CTL_TCNTBEN_MASK) ==
        UNICOMMI2CC_TIMEOUT_CTL_TCNTBEN_ENABLE);
}

/**
 *  @brief      Enable Timeout Counter B
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_I2CC_enableTimeoutB(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->i2cc->TIMEOUT_CTL |= UNICOMMI2CC_TIMEOUT_CTL_TCNTBEN_ENABLE;
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
__STATIC_INLINE uint32_t DL_I2CC_getCurrentTimeoutBCounter(
    UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->i2cc->TIMEOUT_CNT & UNICOMMI2CC_TIMEOUT_CNT_TCNTB_MASK);
}

#ifdef __cplusplus
}
#endif

#endif /* __MCU_HAS_UNICOMMI2CC__ */

#endif /* ti_dl_dl_unicomm__include */
/** @}*/
