/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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
 *  @file       dl_unicommspi.h
 *  @brief      Unified Communication Module (UNICOMM) - Serial Peripheral Interface (SPI) Driver Library
 *  @defgroup   UNICOMMSPI Unified Communication Module (UNICOMM) - Serial Peripheral Interface (SPI)
 *
 *  @anchor ti_dl_dl_unicommspi_Overview
 *  # Overview
 *
 *  The Unified Communication Module Serial Peripheral Interface Driver Library allows
 *  full configuration of the UNICOMM SPI module.
 *
 *  This Serial Peripheral Interface (SPI) module provides a standardized
 *  serial interface to transfer data between devices and other external
 *  devices with SPI interface.
 *
 *  <hr>
 ******************************************************************************
 */
/** @addtogroup UNICOMMSPI
 * @{
 */
#ifndef ti_dl_dl_unicommspi__include
#define ti_dl_dl_unicommspi__include

#include <stdbool.h>
#include <stdint.h>

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>
#include <ti/driverlib/dl_unicomm.h>

#ifdef __MCU_HAS_UNICOMMSPI__

#ifdef __cplusplus
extern "C" {
#endif

/* clang-format off */

/** @addtogroup DL_SPI_CD_MODE
 *  @{
 */
/*!
 * @brief Data mode
 */
#define DL_SPI_CD_MODE_DATA             (UNICOMMSPI_CTL1_CDMODE_DATA >> UNICOMMSPI_CTL1_CDMODE_OFS)

/*!
 * @brief Command mode
 */
#define DL_SPI_CD_MODE_COMMAND       (UNICOMMSPI_CTL1_CDMODE_COMMAND >> UNICOMMSPI_CTL1_CDMODE_OFS)


/** @}*/

/** @addtogroup DL_SPI_INTERRUPT
 *  @{
 */

/*!
 * @brief DMA done 1 event for transmit interrupt
 */
#define DL_SPI_INTERRUPT_DMA_DONE_TX                    (UNICOMMSPI_CPU_INT_IMASK_DMA_DONE_TX_SET)

/*!
 * @brief DMA done 1 event for receive interrupt
 */
#define DL_SPI_INTERRUPT_DMA_DONE_RX                    (UNICOMMSPI_CPU_INT_IMASK_DMA_DONE_RX_SET)

/*!
 * @brief DMA done 1 event for  PREIRQ RX interrupt
 */
#define DL_SPI_INTERRUPT_DMA_PREIRQ_RX                (UNICOMMSPI_CPU_INT_IMASK_DMA_PREIRQ_RX_SET)

/*!
 * @brief DMA done 1 event for  PREIRQ TX interrupt
 */
#define DL_SPI_INTERRUPT_DMA_PREIRQ_TX    (UNICOMMSPI_CPU_INT_IMASK_DMA_PREIRQ_TX_SET)

/*!
 * @brief DMA done 1 event for LTOUT interrupt
 */
#define DL_SPI_INTERRUPT_LTOUT    (UNICOMMSPI_CPU_INT_IMASK_LTOUT_SET)

/*!
 * @brief SPI has finished transfers and changed into idle mode interrupt
 */
#define DL_SPI_INTERRUPT_IDLE                   (UNICOMMSPI_CPU_INT_IMASK_IDLE_SET)

/*!
 * @brief Transmit FIFO empty interrupt
 */
#define DL_SPI_INTERRUPT_TX_EMPTY            (UNICOMMSPI_CPU_INT_IMASK_TXEMPTY_SET)

/*!
 * @brief Transmit FIFO interrupt
 */
#define DL_SPI_INTERRUPT_TX                       (UNICOMMSPI_CPU_INT_IMASK_TX_SET)

/*!
 * @brief Receive FIFO interrupt
 */
#define DL_SPI_INTERRUPT_RX                       (UNICOMMSPI_CPU_INT_IMASK_RX_SET)

/*!
 * @brief Receive timeout interrupt
 */
#define DL_SPI_INTERRUPT_RX_TIMEOUT            (UNICOMMSPI_CPU_INT_IMASK_RTOUT_SET)

/*!
 * @brief Receive FIFO full interrupt
 */
#define DL_SPI_INTERRUPT_RX_FULL              (UNICOMMSPI_CPU_INT_IMASK_RXFULL_SET)

/*!
 * @brief Transmit FIFO underflow interrupt
 */
#define DL_SPI_INTERRUPT_TX_UNDERFLOW     (UNICOMMSPI_CPU_INT_IMASK_TXFIFO_UNF_SET)

/*!
 * @brief Parity error
 */
#define DL_SPI_INTERRUPT_PARITY_ERROR            (UNICOMMSPI_CPU_INT_IMASK_PER_SET)

/*!
 * @brief Receive FIFO overflow interrupt
 */
#define DL_SPI_INTERRUPT_RX_OVERFLOW      (UNICOMMSPI_CPU_INT_IMASK_RXFIFO_OVF_SET)

/** @}*/


/** @addtogroup DL_SPI_DMA_INTERRUPT_RX
 *  @{
 */
/*!
 * @brief SPI interrupt for enabling SPI receive as DMA trigger
 */
#define DL_SPI_DMA_INTERRUPT_RX               (UNICOMMSPI_DMA_TRIG_RX_IMASK_RX_SET)

/*!
 * @brief SPI interrupt for enabling SPI receive timeout as DMA trigger
 */
#define DL_SPI_DMA_INTERRUPT_RX_TIMEOUT       (UNICOMMSPI_DMA_TRIG_RX_IMASK_RTOUT_SET)

/** @}*/

/*!
 * @brief SPI interrupt for enabling SPI transmit as DMA trigger
 */
#define DL_SPI_DMA_INTERRUPT_TX               (UNICOMMSPI_DMA_TRIG_TX_IMASK_TX_SET)

/* clang-format on */

/*! @enum DL_SPI_PARITY */
typedef enum {
    /*! Use even parity, enable transmit and receive parity */
    DL_SPI_PARITY_EVEN =
        (UNICOMMSPI_CTL1_PES_ENABLE | UNICOMMSPI_CTL1_PREN_ENABLE |
            UNICOMMSPI_CTL1_PTEN_ENABLE),
    /*! Use odd parity, enable transmit and receive parity  */
    DL_SPI_PARITY_ODD =
        (UNICOMMSPI_CTL1_PES_DISABLE | UNICOMMSPI_CTL1_PREN_ENABLE |
            UNICOMMSPI_CTL1_PTEN_ENABLE),
    /*! Disable receive and transmit parity */
    DL_SPI_PARITY_NONE =
        (UNICOMMSPI_CTL1_PREN_DISABLE | UNICOMMSPI_CTL1_PTEN_DISABLE)
} DL_SPI_PARITY;

/*! @enum DL_SPI_FRAME_FORMAT */
typedef enum {
    /*! Motorola 3 Wire with Polarity 0, Phase 0 */
    DL_SPI_FRAME_FORMAT_MOTO3_POL0_PHA0 =
        (UNICOMMSPI_CTL0_SPO_LOW | UNICOMMSPI_CTL0_SPH_FIRST |
            UNICOMMSPI_CTL0_FRF_MOTOROLA_3WIRE),
    /*! Motorola 3 Wire with Polarity 0, Phase 1 */
    DL_SPI_FRAME_FORMAT_MOTO3_POL0_PHA1 =
        (UNICOMMSPI_CTL0_SPO_LOW | UNICOMMSPI_CTL0_SPH_SECOND |
            UNICOMMSPI_CTL0_FRF_MOTOROLA_3WIRE),
    /*! Motorola 3 Wire with Polarity 1, Phase 0 */
    DL_SPI_FRAME_FORMAT_MOTO3_POL1_PHA0 =
        (UNICOMMSPI_CTL0_SPO_HIGH | UNICOMMSPI_CTL0_SPH_FIRST |
            UNICOMMSPI_CTL0_FRF_MOTOROLA_3WIRE),
    /*! Motorola 3 Wire with Polarity 1, Phase 1 */
    DL_SPI_FRAME_FORMAT_MOTO3_POL1_PHA1 =
        (UNICOMMSPI_CTL0_SPO_HIGH | UNICOMMSPI_CTL0_SPH_SECOND |
            UNICOMMSPI_CTL0_FRF_MOTOROLA_3WIRE),
    /*! Motorola 4 Wire with Polarity 0, Phase 0 */
    DL_SPI_FRAME_FORMAT_MOTO4_POL0_PHA0 =
        (UNICOMMSPI_CTL0_SPO_LOW | UNICOMMSPI_CTL0_SPH_FIRST |
            UNICOMMSPI_CTL0_FRF_MOTOROLA_4WIRE),
    /*! Motorola 4 Wire with Polarity 0, Phase 1 */
    DL_SPI_FRAME_FORMAT_MOTO4_POL0_PHA1 =
        (UNICOMMSPI_CTL0_SPO_LOW | UNICOMMSPI_CTL0_SPH_SECOND |
            UNICOMMSPI_CTL0_FRF_MOTOROLA_4WIRE),
    /*! Motorola 4 Wire with Polarity 1, Phase 0 */
    DL_SPI_FRAME_FORMAT_MOTO4_POL1_PHA0 =
        (UNICOMMSPI_CTL0_SPO_HIGH | UNICOMMSPI_CTL0_SPH_FIRST |
            UNICOMMSPI_CTL0_FRF_MOTOROLA_4WIRE),
    /*! Motorola 4 Wire with Polarity 1, Phase 1 */
    DL_SPI_FRAME_FORMAT_MOTO4_POL1_PHA1 =
        (UNICOMMSPI_CTL0_SPO_HIGH | UNICOMMSPI_CTL0_SPH_SECOND |
            UNICOMMSPI_CTL0_FRF_MOTOROLA_4WIRE),
    /*! TI Sync Frame Format */
    DL_SPI_FRAME_FORMAT_TI_SYNC = (UNICOMMSPI_CTL0_FRF_TI_SYNC),
} DL_SPI_FRAME_FORMAT;

/*! @enum DL_SPI_MODE */
typedef enum {
    /*! Controller mode */
    DL_SPI_MODE_CONTROLLER = (UNICOMMSPI_CTL1_CP_ENABLE),
    /*! Peripheral mode */
    DL_SPI_MODE_PERIPHERAL = (UNICOMMSPI_CTL1_CP_DISABLE)
} DL_SPI_MODE;

/*! @enum DL_SPI_BIT_ORDER */
typedef enum {
    /*! MSB First */
    DL_SPI_BIT_ORDER_MSB_FIRST = (UNICOMMSPI_CTL1_MSB_ENABLE),
    /*! LSB First */
    DL_SPI_BIT_ORDER_LSB_FIRST = (UNICOMMSPI_CTL1_MSB_DISABLE)
} DL_SPI_BIT_ORDER;

/*! @enum DL_SPI_DATA_SIZE */
typedef enum {
    /*! Data size 4 bits */
    DL_SPI_DATA_SIZE_4 = (UNICOMMSPI_CTL0_DSS_DSS_4),
    /*! Data size 5 bits */
    DL_SPI_DATA_SIZE_5 = (UNICOMMSPI_CTL0_DSS_DSS_5),
    /*! Data size 6 bits */
    DL_SPI_DATA_SIZE_6 = (UNICOMMSPI_CTL0_DSS_DSS_6),
    /*! Data size 7 bits */
    DL_SPI_DATA_SIZE_7 = (UNICOMMSPI_CTL0_DSS_DSS_7),
    /*! Data size 8 bits */
    DL_SPI_DATA_SIZE_8 = (UNICOMMSPI_CTL0_DSS_DSS_8),
    /*! Data size 9 bits */
    DL_SPI_DATA_SIZE_9 = (UNICOMMSPI_CTL0_DSS_DSS_9),
    /*! Data size 10 bits */
    DL_SPI_DATA_SIZE_10 = (UNICOMMSPI_CTL0_DSS_DSS_10),
    /*! Data size 11 bits */
    DL_SPI_DATA_SIZE_11 = (UNICOMMSPI_CTL0_DSS_DSS_11),
    /*! Data size 12 bits */
    DL_SPI_DATA_SIZE_12 = (UNICOMMSPI_CTL0_DSS_DSS_12),
    /*! Data size 13 bits */
    DL_SPI_DATA_SIZE_13 = (UNICOMMSPI_CTL0_DSS_DSS_13),
    /*! Data size 14 bits */
    DL_SPI_DATA_SIZE_14 = (UNICOMMSPI_CTL0_DSS_DSS_14),
    /*! Data size 15 bits */
    DL_SPI_DATA_SIZE_15 = (UNICOMMSPI_CTL0_DSS_DSS_15),
    /*! Data size 16 bits */
    DL_SPI_DATA_SIZE_16 = (UNICOMMSPI_CTL0_DSS_DSS_16),
} DL_SPI_DATA_SIZE;

/*! @enum DL_SPI_CHIP_SELECT */
typedef enum {
    /*! Chip Select 0 */
    DL_SPI_CHIP_SELECT_0 = (UNICOMMSPI_CTL0_CSSEL_CSSEL_0),
    /*! Chip Select 1 */
    DL_SPI_CHIP_SELECT_1 = (UNICOMMSPI_CTL0_CSSEL_CSSEL_1),
    /*! Chip Select 2 */
    DL_SPI_CHIP_SELECT_2 = (UNICOMMSPI_CTL0_CSSEL_CSSEL_2),
    /*! Chip Select 3 */
    DL_SPI_CHIP_SELECT_3 = (UNICOMMSPI_CTL0_CSSEL_CSSEL_3),
    /*! No chip select */
    DL_SPI_CHIP_SELECT_NONE = (0)
} DL_SPI_CHIP_SELECT;

/*! @enum DL_SPI_TX_FIFO_LEVEL */
typedef enum {
    /*! Interrupt triggers when TX FIFO <= 3/4 empty */
    DL_SPI_TX_FIFO_LEVEL_3_4_EMPTY = UNICOMMSPI_IFLS_TXIFLSEL_LVL_3_4,
    /*! Interrupt triggers when TX FIFO <= 1/2 empty (default) */
    DL_SPI_TX_FIFO_LEVEL_1_2_EMPTY = UNICOMMSPI_IFLS_TXIFLSEL_LVL_1_2,
    /*! Interrupt triggers when TX FIFO <= 1/4 empty */
    DL_SPI_TX_FIFO_LEVEL_1_4_EMPTY = UNICOMMSPI_IFLS_TXIFLSEL_LVL_1_4,
    /*! Interrupt triggers when TX FIFO is not totally full */
    DL_SPI_TX_FIFO_LEVEL_ONE_FRAME = UNICOMMSPI_IFLS_TXIFLSEL_LVL_NOT_FULL,
    /*! Interrupt triggers when TX FIFO is empty */
    DL_SPI_TX_FIFO_LEVEL_EMPTY = UNICOMMSPI_IFLS_TXIFLSEL_LVL_EMPTY,
    /*! Interrupt triggers when TX FIFO <= 1  */
    DL_SPI_TX_FIFO_LEVEL_ALMOST_EMPTY =
        UNICOMMSPI_IFLS_TXIFLSEL_LVL_ALMOST_EMPTY,
    /*! Interrupt triggers when TX FIFO >= (MAX FIFO LEN) - 1 */
    DL_SPI_TX_FIFO_LEVEL_ALMOST_FULL =
        UNICOMMSPI_IFLS_TXIFLSEL_LVL_ALMOST_FULL,
} DL_SPI_TX_FIFO_LEVEL;

/*! @enum DL_SPI_RX_FIFO_LEVEL */
typedef enum {
    /*! Interrupt triggers when RX FIFO >= 3/4 full */
    DL_SPI_RX_FIFO_LEVEL_3_4_FULL = UNICOMMSPI_IFLS_RXIFLSEL_LVL_3_4,
    /*! Interrupt triggers when RX FIFO >= 1/2 full (default) */
    DL_SPI_RX_FIFO_LEVEL_1_2_FULL = UNICOMMSPI_IFLS_RXIFLSEL_LVL_1_2,
    /*! Interrupt triggers when RX FIFO >= 1/4 full */
    DL_SPI_RX_FIFO_LEVEL_1_4_FULL = UNICOMMSPI_IFLS_RXIFLSEL_LVL_1_4,
    /*! Interrupt triggers when RX FIFO is not empty */
    DL_SPI_RX_FIFO_LEVEL_ONE_FRAME = UNICOMMSPI_IFLS_RXIFLSEL_LVL_NOT_EMPTY,
    /*! Interrupt triggers when RX FIFO is full */
    DL_SPI_RX_FIFO_LEVEL_FULL = UNICOMMSPI_IFLS_RXIFLSEL_LVL_FULL,
    /*! Interrupt triggers when RX FIFO >= (MAX FIFO LEN) - 1 */
    DL_SPI_RX_FIFO_LEVEL_ALMOST_FULL =
        UNICOMMSPI_IFLS_RXIFLSEL_LVL_ALMOST_FULL,
    /*! Interrupt triggers when RX FIFO <= 1 */
    DL_SPI_RX_FIFO_LEVEL_ALMOST_EMPTY =
        UNICOMMSPI_IFLS_RXIFLSEL_LVL_ALMOST_EMPTY,
} DL_SPI_RX_FIFO_LEVEL;

/*! @enum DL_SPI_IIDX */
typedef enum {

    /*! SPI interrupt index for DMA Done 1 event for transmit */
    DL_SPI_IIDX_DMA_DONE_TX = UNICOMMSPI_IIDX_STAT_DMA_DONE_TX,
    /*! SPI interrupt index for DMA Done 1 event for receive */
    DL_SPI_IIDX_DMA_DONE_RX = UNICOMMSPI_IIDX_STAT_DMA_DONE_RX,
    /*! SPI interrupt index for SPI to signal it has finished transfers and
     * changed into idle mode */

    DL_SPI_IIDX_IDLE = UNICOMMSPI_IIDX_STAT_IDLE_EVT,
    /*! SPI interrupt index for transmit FIFO empty */
    DL_SPI_IIDX_TX_EMPTY = UNICOMMSPI_IIDX_STAT_TX_EMPTY,
    /*! SPI interrupt index for transmit FIFO */
    DL_SPI_IIDX_TX = UNICOMMSPI_IIDX_STAT_TX_EVT,
    /*! SPI interrupt index for receive FIFO */
    DL_SPI_IIDX_RX = UNICOMMSPI_IIDX_STAT_RX_EVT,
    /*! SPI interrupt index for receive timeout */
    DL_SPI_IIDX_RX_TIMEOUT = UNICOMMSPI_IIDX_STAT_RTOUT_EVT,

    /*! SPI interrupt index for receive FIFO full  */
    DL_SPI_IIDX_RX_FULL = UNICOMMSPI_IIDX_STAT_RXFULL_EVT,
    /*! SPI interrupt index for transmit FIFO underflow  */
    DL_SPI_IIDX_TX_UNDERFLOW = UNICOMMSPI_IIDX_STAT_TXFIFO_UNF_EVT,

    /*! SPI interrupt index for parity error */
    DL_SPI_IIDX_PARITY_ERROR = UNICOMMSPI_IIDX_STAT_PER_EVT,
    /*! SPI interrupt index for receive FIFO overflow */
    DL_SPI_IIDX_RX_OVERFLOW = UNICOMMSPI_IIDX_STAT_RXFIFO_OFV_EVT,
    /*! SPI interrupt index for PREIRQ RX*/
    DL_SPI_IIDX_PREIRQ_RX = UNICOMMSPI_IIDX_STAT_DMA_PREIRQ_RX,
    /*! SPI interrupt index for PREIRQ TX*/
    DL_SPI_IIDX_PREIRQ_TX = UNICOMMSPI_IIDX_STAT_DMA_PREIRQ_TX,
    /*! SPI interrupt index for line time out*/
    DL_SPI_IIDX_LTOUT = UNICOMMSPI_IIDX_STAT_LTOUT,
} DL_SPI_IIDX;

/*! @enum DL_SPI_CLOCK_DIVIDE_RATIO */
typedef enum {
    /*! SPI source clock divide ratio set to 1 */
    DL_SPI_CLOCK_DIVIDE_RATIO_1 = UNICOMMSPI_CLKDIV_RATIO_DIV_BY_1,
    /*! SPI source clock divide ratio set to 2 */
    DL_SPI_CLOCK_DIVIDE_RATIO_2 = UNICOMMSPI_CLKDIV_RATIO_DIV_BY_2,
    /*! SPI source clock divide ratio set to 3 */
    DL_SPI_CLOCK_DIVIDE_RATIO_3 = UNICOMMSPI_CLKDIV_RATIO_DIV_BY_3,
    /*! SPI source clock divide ratio set to 4 */
    DL_SPI_CLOCK_DIVIDE_RATIO_4 = UNICOMMSPI_CLKDIV_RATIO_DIV_BY_4,
    /*! SPI source clock divide ratio set to 5 */
    DL_SPI_CLOCK_DIVIDE_RATIO_5 = UNICOMMSPI_CLKDIV_RATIO_DIV_BY_5,
    /*! SPI source clock divide ratio set to 6 */
    DL_SPI_CLOCK_DIVIDE_RATIO_6 = UNICOMMSPI_CLKDIV_RATIO_DIV_BY_6,
    /*! SPI source clock divide ratio set to 7 */
    DL_SPI_CLOCK_DIVIDE_RATIO_7 = UNICOMMSPI_CLKDIV_RATIO_DIV_BY_7,
    /*! SPI source clock divide ratio set to 8 */
    DL_SPI_CLOCK_DIVIDE_RATIO_8 = UNICOMMSPI_CLKDIV_RATIO_DIV_BY_8
} DL_SPI_CLOCK_DIVIDE_RATIO;

/*! @enum DL_SPI_CLOCK */
typedef enum {
    /*! Selects BUSCLK as the clock source */
    DL_SPI_CLOCK_BUSCLK = UNICOMMSPI_CLKSEL_BUSCLK_SEL_ENABLE,
    /*! Selects MFCLK as the clock source */
    DL_SPI_CLOCK_MFCLK = UNICOMMSPI_CLKSEL_MFCLK_SEL_ENABLE,
    /*! Selects LFCLK as the clock source */
    DL_SPI_CLOCK_LFCLK = UNICOMMSPI_CLKSEL_LFCLK_SEL_ENABLE,
    /*! Selects ASYNC LFCLK as the clock source */
    DL_SPI_CLOCK_ASYNC_LFCLK = UNICOMMSPI_CLKSEL_ASYNC_LFCLK_SEL_ENABLE,
    /*! Selects ASYNC SYSCLK as the clock source */
    DL_SPI_CLOCK_ASYNC_SYSCLK = UNICOMMSPI_CLKSEL_ASYNC_SYSCLK_SEL_ENABLE,
    /*! Selects ASYNC HFCLK as the clock source */
    DL_SPI_CLOCK_ASYNC_HFCLK = UNICOMMSPI_CLKSEL_ASYNC_HFCLK_SEL_ENABLE,
    /*! Selects ASYNC PLL as the clock source */
    DL_SPI_CLOCK_ASYNC_PLL = UNICOMMSPI_CLKSEL_ASYNC_PLL_SEL_ENABLE
} DL_SPI_CLOCK;

/*!
 *  @brief  Configuration struct for @ref DL_SPI_init.
 */
typedef struct {
    /*! The controller/peripheral mode configuration. One of @ref DL_SPI_MODE */
    DL_SPI_MODE mode;

    /*!
     *  The frame format to use for data transfer. One of @ref
     *  DL_SPI_FRAME_FORMAT
     */
    DL_SPI_FRAME_FORMAT frameFormat;

    /*!
     *  The parity configuration to use for data transfer. One of @ref
     *  DL_SPI_PARITY.
     */
    DL_SPI_PARITY parity;

    /*! The size of the data transfer. One of @ref DL_SPI_DATA_SIZE */
    DL_SPI_DATA_SIZE dataSize;

    /*! The order of bits during data transfer. One of @ref DL_SPI_BIT_ORDER */
    DL_SPI_BIT_ORDER bitOrder;

    /*! The pin to use for chip select. Used in Controller or Peripheral modes
     *  with Motorola 4-Wire or TI Sync frame formats. One of
     *  @ref DL_SPI_CHIP_SELECT.
     */
    DL_SPI_CHIP_SELECT chipSelectPin;

} DL_SPI_Config;

/*!
 *  @brief  Configuration struct for @ref DL_SPI_setClockConfig.
 */
typedef struct {
    /*! Selects SPI module clock source @ref DL_SPI_CLOCK */
    DL_SPI_CLOCK clockSel;

    /*! Selects the divide ratio. One of @ref DL_SPI_CLOCK_DIVIDE_RATIO */
    DL_SPI_CLOCK_DIVIDE_RATIO divideRatio;

} DL_SPI_ClockConfig;

/**
 * @brief Configuration structure to backup SPI peripheral state before going
 *        to STOP/STANDBY mode. Not required after PG 1.0 silicon. Used by
 *        @ref DL_SPI_saveConfiguration and @ref DL_SPI_restoreConfiguration
 */
typedef struct {
    /*! Combination of basic SPI control configurations that are
     *  compressed to a single word as they are stored in the SPI
     *  registers See @ref DL_SPI_init for how the peripheral control word 0
     *  is created. */
    uint32_t controlWord0;

    /*! Combination of basic SPI control configurations that are
     *  compressed to a single word as they are stored in the SPI
     *  registers. See @ref DL_SPI_init for how the peripheral control word 1
     *  is created. */
    uint32_t controlWord1;

    /*! Combination of serial clock divider and delayed sampling settings
     *  compressed to a single word as they are stored in the SPI registers. */
    uint32_t clockControl;

    /*! SPI module clock source. One of  @ref DL_SPI_CLOCK */
    uint32_t clockSel;

    /*! SPI clock divider. One of @ref DL_SPI_CLOCK_DIVIDE_RATIO */
    uint32_t divideRatio;

    /*! Combination of SPI interrupt FIFO level select configurations that are
     *  compressed to a single word as they are stored in the SPI
     *  registers. */
    uint32_t interruptFifoLevelSelectWord;

    /*! SPI interrupt status for EVENT0.
     *  Bitwise OR of @ref DL_SPI_INTERRUPT */
    uint32_t interruptMask0;

    /*! SPI interrupt status for EVENT1.
     *  Bitwise OR of @ref DL_SPI_DMA_INTERRUPT_RX */
    uint32_t interruptMask1;

    /*! SPI interrupt status for EVENT2.
     *  Bitwise OR of @ref DL_SPI_DMA_INTERRUPT_TX */
    uint32_t interruptMask2;

    /*! Boolean flag indicating whether or not a valid configuration structure
     *  exists. Should not be modified by the user. */
    bool backupRdy;
} DL_SPI_backupConfig;

/**
 *  @brief      Initialize the SPI peripheral
 *
 *  Initializes all the common configurable options for the SPI peripheral. Any
 *  other custom configuration can be done after calling this API. The SPI is
 *  not enabled in this API.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  config  Configuration for SPI peripheral
 */
void DL_SPI_init(UNICOMM_Inst_Regs *unicomm, DL_SPI_Config *config);

/**
 * @brief Enables power on SPI module
 *
 * @param unicomm        Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_enablePower(UNICOMM_Inst_Regs *unicomm)
{
    DL_UNICOMM_enablePower(unicomm);
    if (unicomm->fixedMode == false) {
        DL_UNICOMM_setIPMode(unicomm, DL_UNICOMM_SPI);
    }
}

/**
 * @brief Disables power on spi module
 *
 * @param unicomm        Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_disablePower(UNICOMM_Inst_Regs *unicomm)
{
    DL_UNICOMM_disablePower(unicomm);
}

/**
 * @brief Returns if  power on spi module
 *
 * @param unicomm        Pointer to the register overlay for the peripheral
 *
 * @return true if power is enabled
 * @return false if power is disabled
 */
__STATIC_INLINE bool DL_SPI_isPowerEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return DL_UNICOMM_isPowerEnabled(unicomm);
}

/**
 * @brief Resets spi peripheral
 *
 * @param unicomm        Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_reset(UNICOMM_Inst_Regs *unicomm)
{
    DL_UNICOMM_reset(unicomm);
}

/**
 * @brief Returns if spi peripheral was reset
 *
 * @param unicomm        Pointer to the register overlay for the peripheral
 *
 * @return true if peripheral was reset
 * @return false if peripheral wasn't reset
 *
 */
__STATIC_INLINE bool DL_SPI_isReset(UNICOMM_Inst_Regs *unicomm)
{
    return DL_UNICOMM_isReset(unicomm);
}

/**
 *  @brief      Enable the SPI peripheral
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_enable(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL1 |= UNICOMMSPI_CTL1_ENABLE_ENABLE;
}

/**
 *  @brief      Checks if the SPI peripheral is enabled
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     Returns the enabled status of the SPI
 *
 *  @retval     true  The SPI peripheral is enabled
 *  @retval     false The SPI peripheral is disabled
 */
__STATIC_INLINE bool DL_SPI_isEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->CTL1 & UNICOMMSPI_CTL1_ENABLE_MASK) ==
            UNICOMMSPI_CTL1_ENABLE_ENABLE);
}

/**
 *  @brief      Disable the SPI peripheral
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_disable(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL1 &= ~(UNICOMMSPI_CTL1_ENABLE_MASK);
}

/**
 *  @brief      Configure SPI source clock
 *
 *  @param[in]  unicomm    Pointer to the register overlay for the
 *                      peripheral
 *  @param[in]  config  Pointer to the clock configuration struct
 *                       @ref DL_SPI_ClockConfig.
 */
void DL_SPI_setClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_ClockConfig *config);

/**
 *  @brief      Get SPI source clock configuration
 *
 *  @param[in]  unicomm    Pointer to the register overlay for the
 *                      peripheral
 *  @param[in]  config  Pointer to the clock configuration struct
 *                      @ref DL_SPI_ClockConfig.
 */
void DL_SPI_getClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_ClockConfig *config);

/**
 *  @brief      Checks if the SPI is busy transmitting
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     Returns the busy status of the SPI
 *
 *  @retval     true  The SPI is transmitting
 *  @retval     false The SPI is idle
 */
__STATIC_INLINE bool DL_SPI_isBusy(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->STAT & UNICOMMSPI_STAT_BUSY_MASK) ==
            UNICOMMSPI_STAT_BUSY_ACTIVE);
}

/**
 *  @brief      Checks if the TX FIFO is empty
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     Returns the empty status of the TX FIFO
 *
 *  @retval     true  The TX FIFO is empty
 *  @retval     false The TX FIFO is not empty
 */
__STATIC_INLINE bool DL_SPI_isTXFIFOEmpty(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->STAT & UNICOMMSPI_STAT_TXFE_MASK) ==
            UNICOMMSPI_STAT_TXFE_EMPTY);
}

/**
 *  @brief      Checks if the TX FIFO is full
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     Returns the full status of the TX FIFO
 *
 *  @retval     true  The TX FIFO is full
 *  @retval     false The TX FIFO is not full
 */
__STATIC_INLINE bool DL_SPI_isTXFIFOFull(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->STAT & UNICOMMSPI_STAT_TXFF_MASK) ==
            UNICOMMSPI_STAT_TXFF_SET);
}

/**
 *  @brief      Checks if the RX FIFO is empty
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     Returns the empty status of the RX FIFO
 *
 *  @retval     true  The RX FIFO is empty
 *  @retval     false The RX FIFO is not empty
 */
__STATIC_INLINE bool DL_SPI_isRXFIFOEmpty(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->STAT & UNICOMMSPI_STAT_RXFE_MASK) ==
            UNICOMMSPI_STAT_RXFE_EMPTY);
}

/**
 *  @brief      Checks if the RX FIFO is full
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     Returns the full status of the RX FIFO
 *
 *  @retval     true  The RX FIFO is full
 *  @retval     false The RX FIFO is not full
 */
__STATIC_INLINE bool DL_SPI_isRXFIFOFull(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->STAT & UNICOMMSPI_STAT_RXFF_MASK) ==
            UNICOMMSPI_STAT_RXFF_SET);
}

/**
 *  @brief      Checks if the TX FIFO is cleared
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     Returns TX FIFO clear status
 *
 *  @retval     true  The TX FIFO is cleared
 *  @retval     false The TX FIFO is not cleared
 */

__STATIC_INLINE bool DL_SPI_isTXFIFOCleared(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->STAT & UNICOMMSPI_STAT_TXCLR_MASK) ==
            UNICOMMSPI_STAT_TXCLR_SET);
}

/**
 *  @brief      Checks if the RX FIFO is cleared
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     Returns RX FIFO clear status
 *
 *  @retval     true  The RX FIFO is cleared
 *  @retval     false The RX FIFO is not cleared
 */

__STATIC_INLINE bool DL_SPI_isRXFIFOCleared(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->STAT & UNICOMMSPI_STAT_RXCLR_MASK) ==
            UNICOMMSPI_STAT_RXCLR_SET);
}

/**
 *  @brief      returns current cdmode value
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     returns current cdmode value
 */
__STATIC_INLINE uint32_t DL_SPI_getCurrentCDMODEValue(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->STAT & UNICOMMSPI_STAT_CDMODE_MASK) >>
            UNICOMMSPI_STAT_CDMODE_OFS);
}

/**
 *  @brief      Sets the parity configuration used for transactions
 *
 *  This API sets the configuration for both receive parity and transmit parity.
 *  This includes which bit is used, and whether even or odd parity is set.
 *
 *  To individually enable or dsiable the receive or transmit parity, refer to
 *  the APIs listed below.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  parity  Parity configuration to use. One of @ref DL_SPI_PARITY.
 *
 *  @sa         DL_SPI_init
 *  @sa         DL_SPI_enableReceiveParity
 *  @sa         DL_SPI_disableReceiveParity
 *  @sa         DL_SPI_enableTransmitParity
 *  @sa         DL_SPI_disableTransmitParity
 */
__STATIC_INLINE void DL_SPI_setParity(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_PARITY parity)
{
    DL_Common_updateReg(&unicomm->spi->CTL1, (uint32_t) parity,
        (UNICOMMSPI_CTL1_PREN_MASK | UNICOMMSPI_CTL1_PTEN_MASK |
            UNICOMMSPI_CTL1_PES_MASK));
}

/**
 *  @brief      Get the current receive and transmit parity configuration
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     The current parity configuration being used
 *
 *  @retval     One of @ref DL_SPI_PARITY
 */
__STATIC_INLINE DL_SPI_PARITY DL_SPI_getParity(UNICOMM_Inst_Regs *unicomm)
{
    uint32_t parity = unicomm->spi->CTL1 &
                      (UNICOMMSPI_CTL1_PES_MASK | UNICOMMSPI_CTL1_PREN_MASK |
                          UNICOMMSPI_CTL1_PTEN_MASK);

    return (DL_SPI_PARITY)(parity);
}

/**
 *  @brief      Enables receive parity
 *
 *  This API only enables receive parity, it does not configure the parity mode
 *  used.
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 *
 *  @sa         DL_SPI_setParity
 */
__STATIC_INLINE void DL_SPI_enableReceiveParity(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL1 |= UNICOMMSPI_CTL1_PREN_ENABLE;
}

/**
 *  @brief      Disables receive parity
 *
 *  This API only disable receive parity, it does not configure the parity mode
 *  used.
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 *
 *  @sa         DL_SPI_setParity
 */
__STATIC_INLINE void DL_SPI_disableReceiveParity(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL1 &= ~(UNICOMMSPI_CTL1_PREN_MASK);
}

/**
 *  @brief      Checks if receive parity is enabled
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 *
 *  @return     If receive parity is enabled
 *
 *  @retval     true   Receive parity is enabled
 *  @retval     false  Receive parity is disabled
 */
__STATIC_INLINE bool DL_SPI_isReceiveParityEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->CTL1 & UNICOMMSPI_CTL1_PREN_MASK) ==
            UNICOMMSPI_CTL1_PREN_ENABLE);
}

/**
 *  @brief      Enables transmit parity
 *
 *  This API only enables transmit parity, it does not configure the parity mode
 *  used.
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 *
 *  @sa         DL_SPI_setParity
 */
__STATIC_INLINE void DL_SPI_enableTransmitParity(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL1 |= UNICOMMSPI_CTL1_PTEN_ENABLE;
}

/**
 *  @brief      Disables transmit parity
 *
 *  This API only disables transmit parity, it does not configure the parity
 *  mode used.
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 *
 *  @sa         DL_SPI_setParity
 */
__STATIC_INLINE void DL_SPI_disableTransmitParity(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL1 &= ~(UNICOMMSPI_CTL1_PTEN_MASK);
}

/**
 *  @brief      Checks if transmit parity is enabled
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 *
 *  @return     If transmit parity is enabled
 *
 *  @retval     true   Transmit parity is enabled
 *  @retval     false  Transmit parity is disabled
 */
__STATIC_INLINE bool DL_SPI_isTransmitParityEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->CTL1 & UNICOMMSPI_CTL1_PTEN_MASK) ==
            UNICOMMSPI_CTL1_PTEN_ENABLE);
}

/**
 *  @brief      Set the frame format to use
 *
 *  Configures the frame format to use for transactions. If you are using chip
 *  select you must use one of the Motorola 4 wire frame formats.
 *
 *  @param[in]  unicomm          Pointer to the register overlay for the peripheral
 *  @param[in]  frameFormat  Frame format to use. One of @ref
 *                           DL_SPI_FRAME_FORMAT.
 *
 *  @sa         DL_SPI_init
 */
__STATIC_INLINE void DL_SPI_setFrameFormat(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_FRAME_FORMAT frameFormat)
{
    DL_Common_updateReg(&unicomm->spi->CTL0, (uint32_t) frameFormat,
        (UNICOMMSPI_CTL0_FRF_MASK | UNICOMMSPI_CTL0_SPO_MASK |
            UNICOMMSPI_CTL0_SPH_MASK));
}

/**
 *  @brief      Get the frame format configuration
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     The current frame format being used
 *
 *  @retval     One of @ref DL_SPI_FRAME_FORMAT
 */
__STATIC_INLINE DL_SPI_FRAME_FORMAT DL_SPI_getFrameFormat(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t frameFormat = unicomm->spi->CTL0 & (UNICOMMSPI_CTL0_FRF_MASK |
                                                    UNICOMMSPI_CTL0_SPO_MASK |
                                                    UNICOMMSPI_CTL0_SPH_MASK);

    return (DL_SPI_FRAME_FORMAT)(frameFormat);
}

/**
 *  @brief      Set the size for transfers
 *
 *  @param[in]  unicomm       Pointer to the register overlay for the peripheral
 *  @param[in]  dataSize  Number of bits used in a transfer.
 *                        One of @ref DL_SPI_DATA_SIZE
 *
 *  @sa         DL_SPI_init
 */
__STATIC_INLINE void DL_SPI_setDataSize(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_DATA_SIZE dataSize)
{
    DL_Common_updateReg(
        &unicomm->spi->CTL0, (uint32_t) dataSize, UNICOMMSPI_CTL0_DSS_MASK);
}

/**
 *  @brief      Get the configured size for transfers
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     The currently configured size for transfers
 *
 *  @retval     One of @ref DL_SPI_DATA_SIZE
 */
__STATIC_INLINE DL_SPI_DATA_SIZE DL_SPI_getDataSize(UNICOMM_Inst_Regs *unicomm)
{
    uint32_t dataSize = unicomm->spi->CTL0 & UNICOMMSPI_CTL0_DSS_MASK;

    return (DL_SPI_DATA_SIZE)(dataSize);
}

/**
 *  @brief      Set whether the device should be in controller/peripheral mode
 *
 *  @param[in]  unicomm   Pointer to the register overlay for the peripheral
 *  @param[in]  mode  Mode to configure the SPI into. One of @ref DL_SPI_MODE.
 *
 *  @sa         DL_SPI_init
 */
__STATIC_INLINE void DL_SPI_setMode(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_MODE mode)
{
    DL_Common_updateReg(
        &unicomm->spi->CTL1, (uint32_t) mode, UNICOMMSPI_CTL1_CP_MASK);
}

/**
 *  @brief      Get the current mode for the SPI (controller/peripheral)
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     The currently configured mode for the SPI (controller/peripheral)
 *
 *  @retval     One of @ref DL_SPI_MODE.
 */
__STATIC_INLINE DL_SPI_MODE DL_SPI_getMode(UNICOMM_Inst_Regs *unicomm)
{
    uint32_t mode = unicomm->spi->CTL1 & UNICOMMSPI_CTL1_CP_MASK;

    return (DL_SPI_MODE)(mode);
}

/**
 *  @brief      Set the bit order used for transfers
 *
 *  @param[in]  unicomm       Pointer to the register overlay for the peripheral
 *  @param[in]  bitOrder  Order for bits to be sent out during transfer. One of
 *                        @ref DL_SPI_BIT_ORDER.
 *
 *  @sa         DL_SPI_init
 */
__STATIC_INLINE void DL_SPI_setBitOrder(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_BIT_ORDER bitOrder)
{
    DL_Common_updateReg(
        &unicomm->spi->CTL1, (uint32_t) bitOrder, UNICOMMSPI_CTL1_MSB_MASK);
}

/**
 *  @brief      Get the current bit order used for transfers
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     The currently configured bit order
 *
 *  @retval     One of @ref DL_SPI_BIT_ORDER.
 */
__STATIC_INLINE DL_SPI_BIT_ORDER DL_SPI_getBitOrder(UNICOMM_Inst_Regs *unicomm)
{
    uint32_t bitOrder = unicomm->spi->CTL1 & UNICOMMSPI_CTL1_MSB_MASK;

    return (DL_SPI_BIT_ORDER)(bitOrder);
}

/**
 *  @brief      Enables loopback mode
 *
 *  Enables the loopback mode. When enabled, the output of the transmit serial
 *  shifter is connected to the input of the receive serial shifter internally.
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_enableLoopbackMode(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL1 |= UNICOMMSPI_CTL1_LBM_ENABLE;
}

/**
 *  @brief      Disables loopback mode
 *
 *  Disables the loopback mode. When disabled, the transmit serial shifter and
 *  receive serial shifter are not connected internally.
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_disableLoopbackMode(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL1 &= ~(UNICOMMSPI_CTL1_LBM_MASK);
}

/**
 *  @brief      Checks if the loopback mode is enabled
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 *
 *  @return     The status of the loopback mode
 *
 *  @retval     true if loopback mode is enabled
 *  @retval     false if loopback mode is disabled
 */
__STATIC_INLINE bool DL_SPI_isLoopbackModeEnabled(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->CTL1 & UNICOMMSPI_CTL1_LBM_MASK) ==
            UNICOMMSPI_CTL1_LBM_ENABLE);
}

/**
 *  @brief      Set counter for repeated transmit
 *
 *  Repeated transmit allows you to send the same data multiple times. This is
 *  essentially the same as writing the data into the transmit buffer
 *  repeatedly.
 *
 *  @param[in]  unicomm         pointer to the register overlay for the peripheral
 *  @param[in]  numRepeats  number of times to repeat the transfer. Should be a
 *                          value between 0-255.
 *                          @arg 0 disables the repeated transfer
 *                          @arg 1-255 repeats that many times. So will be sent
 *                               numRepeats + 1 times in total.
 */
__STATIC_INLINE void DL_SPI_setRepeatTransmit(
    UNICOMM_Inst_Regs *unicomm, uint32_t numRepeats)
{
    DL_Common_updateReg(&unicomm->spi->CTL1,
        numRepeats << UNICOMMSPI_CTL1_REPEATTX_OFS,
        UNICOMMSPI_CTL1_REPEATTX_MASK);
}

/**
 *  @brief      Get counter for repeated transmit
 *
 *  Repeated transmit allows you to send the same data multiple times. This is
 *  essentially the same as writing the data into the transmit buffer
 *  repeatedly.
 *
 *  @param[in]  unicomm    pointer to the register overlay for the peripheral
 *
 *  @return     Number of repeats used for transfer
 *
 *  @retval     0      Repeated transfer is disabled
 *  @retval     1-255  Repeat that many times.
 */
__STATIC_INLINE uint32_t DL_SPI_getRepeatTransmit(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->CTL1 & UNICOMMSPI_CTL1_REPEATTX_MASK) >>
            UNICOMMSPI_CTL1_REPEATTX_OFS);
}

/**
 *  @brief      Enables data alignment on chip select for peripherals
 *
 *  When enabled, the receieve bit counter is cleared automatically when the
 *  chip select gets set inactive. This helps the peripheral synchronize again to
 *  the controller in case of a disturbance or glitch on the clock line or during
 *  initialization.
 *
 *  This is only relevant when in peripheral mode.
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_enablePeripheralAlignDataOnChipSelect(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL0 |= UNICOMMSPI_CTL0_CSCLR_ENABLE;
}

/**
 *  @brief      Disables data alignment on chip select for peripherals
 *
 *  When disable, the receieve bit counter is not cleared automatically when
 *  the chip select gets set inactive.
 *
 *  This is only relevant when in peripheral mode.
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_disablePeripheralAlignDataOnChipSelect(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL0 &= ~(UNICOMMSPI_CTL0_CSCLR_MASK);
}

/**
 *  @brief      Checks if data alignment on chip select for peripherals is enabled
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 *
 *  @return     The status of data alignment on chip select
 *
 *  @retval     true   Data alignment on chip select is enabled
 *  @retval     false  Data alignment on chip select is disabled
 */
__STATIC_INLINE bool DL_SPI_isPeripheralAlignDataOnChipSelectEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->CTL0 & UNICOMMSPI_CTL0_CSCLR_MASK) ==
            UNICOMMSPI_CTL0_CSCLR_ENABLE);
}

/**
 *  @brief      Set chip select used for controller or peripheral mode
 *
 *  Choose which chip select should be used for data transfer. User must ensure
 *  one of the 4-wire frame formats were selected using @ref DL_SPI_init or
 *  @ref DL_SPI_setFrameFormat.
 *
 * This API is for both controller and peripheral modes, and the chip select
 * can be changed in the application by calling this API.
 *
 *  @param[in]  unicomm         pointer to the register overlay for the peripheral
 *  @param[in]  chipSelect  the chip select pin to use. One of @ref
 *                          DL_SPI_CHIP_SELECT.
 */
__STATIC_INLINE void DL_SPI_setChipSelect(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_CHIP_SELECT chipSelect)
{
    DL_Common_updateReg(&unicomm->spi->CTL0, (uint32_t) chipSelect,
        UNICOMMSPI_CTL0_CSSEL_MASK);
}

/**
 *  @brief      Get chip select used for controller or peripheral mode
 *
 *  This API is for both controller and peripheral modes.
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 *
 *  @return     The current chip selected used for data transfer
 *
 *  @retval     One of @ref DL_SPI_CHIP_SELECT.
 */
__STATIC_INLINE DL_SPI_CHIP_SELECT DL_SPI_getChipSelect(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t chipSelect = unicomm->spi->CTL0 & UNICOMMSPI_CTL0_CSSEL_MASK;

    return (DL_SPI_CHIP_SELECT)(chipSelect);
}

/**
 *  @brief      Set peripheral receive timeout
 *
 *  Sets the number of clock cycles before a receive timeout occurs.
 *
 *  This is only relevant when in peripheral mode.
 *
 *  @param[in]  unicomm      pointer to the register overlay for the peripheral
 *  @param[in]  timeout  Number of clock cycles before a receive timeout
 *                       occurs. Must be between 0-63.
 *                       @arg 0 Disables receive timeout
 *                       @arg 1-63 Number of clock cycles before timeout
 */
__STATIC_INLINE void DL_SPI_setPeripheralReceiveTimeout(
    UNICOMM_Inst_Regs *unicomm, uint32_t timeout)
{
    DL_Common_updateReg(&unicomm->spi->CTL1,
        timeout << UNICOMMSPI_CTL1_RXTIMEOUT_OFS,
        UNICOMMSPI_CTL1_RXTIMEOUT_MASK);
}

/**
 *  @brief      Get peripheral receive timeout
 *
 *  This is only relevant when in peripheral mode.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *
 *  @return     The current amount of cycles used for a peripheral timeout
 *
 *  @retval     0     indicating that receive timeout is disabled
 *  @retval     1-63  number of clock cycles before a receive timeout occurs
 */
__STATIC_INLINE uint32_t DL_SPI_getPeripheralReceiveTimeout(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->CTL1 & UNICOMMSPI_CTL1_RXTIMEOUT_MASK) >>
            UNICOMMSPI_CTL1_RXTIMEOUT_OFS);
}

/**
 *  @brief      Configure the command/data mode
 *
 *  Command/data mode allows the hardware to control the C/D pin to indicate
 *  whether the data being transferred is a command or just data. The C/D pin
 *  is low for commands and high for data.
 *
 *  You can use this API to manually set the C/D pin by passing in @ref
 *  DL_SPI_CD_MODE_DATA or @ref DL_SPI_CD_MODE_COMMAND. You can also have the
 *  C/D pin switch automatically by passing in how many bytes are part of the
 *  command transfer. The C/D pin will stay low until that many bytes are sent
 *  out and then automatically switch to high.
 *
 *  This is only relevant when in controller mode.
 *
 *  @param[in]  unicomm    pointer to the register overlay for the peripheral
 *  @param[in]  config configuration for command/data mode. One of:
 *                     @arg DL_SPI_CD_MODE_DATA
 *                     @arg DL_SPI_CD_MODE_COMMAND
 *                     @arg A value between 1-14 to indicate how many bytes
 *                          should be sent as command
 */
__STATIC_INLINE void DL_SPI_setControllerCommandDataModeConfig(
    UNICOMM_Inst_Regs *unicomm, uint32_t config)
{
    DL_Common_updateReg(&unicomm->spi->CTL1,
        config << UNICOMMSPI_CTL1_CDMODE_OFS, UNICOMMSPI_CTL1_CDMODE_MASK);
}

/**
 *  @brief      Get the command/data mode configuration
 *
 *  Returns the current configuration for command/data mode. When using the
 *  automatic C/D pin the value read back will be the number of command bytes
 *  remaining to be sent.
 *
 *  @param[in]  unicomm    pointer to the register overlay for the peripheral
 *
 *  @return     The current configuration for command/data mode
 *
 *  @retval     DL_SPI_CD_MODE_DATA    if in manual data mode
 *  @retval     DL_SPI_CD_MODE_COMMAND if in manual command mode
 *  @retval     1-14 indicating how many command bytes still need to be sent
 */
__STATIC_INLINE uint32_t DL_SPI_getControllerCommandDataModeConfig(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->CTL1 & UNICOMMSPI_CTL1_CDMODE_MASK) >>
            UNICOMMSPI_CTL1_CDMODE_OFS);
}

/**
 *  @brief      Enables command/data mode
 *
 *  When command/data mode is enabled, the C/D pin is used to differentiate
 *  between command and data during the transaction.
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 *
 *  @sa         DL_SPI_setControllerCommandDataModeConfig
 */
__STATIC_INLINE void DL_SPI_enableControllerCommandDataMode(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL1 |= UNICOMMSPI_CTL1_CDENABLE_ENABLE;
}

/**
 *  @brief      Disables command/data mode
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_disableControllerCommandDataMode(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL1 &= ~(UNICOMMSPI_CTL1_CDENABLE_MASK);
}

/**
 *  @brief      Checks if command/data mode is enabled
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 *
 *  @return     The status of command/data mode
 *
 *  @retval     true  Command/data mode is enabled
 *  @retval     false Command/data mode is disabled
 */
__STATIC_INLINE bool DL_SPI_isControllerCommandDataModeEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->CTL1 & UNICOMMSPI_CTL1_CDENABLE_MASK) ==
            UNICOMMSPI_CTL1_CDENABLE_ENABLE);
}

/**
 *  @brief      Enables peripheral data output
 *
 *  When peripheral data output is enabled, the peripheral can drive the POCI output pin.
 *  This will cause problems if all peripherals have their RXD lines tied together
 *  and the controller is trying to broadcast a message to all peripherals while
 *  ensuring only one peripheral drives data onto its serial output line.
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_enablePeripheralDataOutput(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL1 &= ~(UNICOMMSPI_CTL1_POD_MASK);
}

/**
 *  @brief      Disables peripheral data output
 *
 *  When peripheral data output is disabled, the peripheral cannot drive the POCI output
 *  pin. This allows multiple peripherals that have their RXD lines tied together to
 *  receive a broadcasted message from a controller.
 *
 *  This is only relevant when in peripheral mode.
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_disablePeripheralDataOutput(
    UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL1 |= UNICOMMSPI_CTL1_POD_ENABLE;
}

/**
 *  @brief      Checks if peripheral data output is enabled
 *
 *  This is only relevant when in peripheral mode.
 *
 *  @param[in]  unicomm  pointer to the register overlay for the peripheral
 *
 *  @return     If peripheral data output is enabled
 *
 *  @retval     true if peripheral data output is enabled
 *  @retval     false if peripheral data output is disabled
 */
__STATIC_INLINE bool DL_SPI_isPeripheralDataOutputEnabled(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->CTL1 & UNICOMMSPI_CTL1_POD_MASK) ==
            UNICOMMSPI_CTL1_POD_DISABLE);
}

/**
 *  @brief      Set the delay sampling
 *
 *  In controller mode only, the data on the input pin will be delayed sampled
 *  by the defined SPI clock cycles. The delay can be adjusted in steps of SPI
 *  input clock steps. The maximum allowed delay should not exceed the length
 *  of one data frame
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  delay   The number of SPI clock cycles to delay sampling on
 *                      input pin. Value between 0-15.
 */
__STATIC_INLINE void DL_SPI_setDelayedSampling(
    UNICOMM_Inst_Regs *unicomm, uint32_t delay)
{
    DL_Common_updateReg(&unicomm->spi->CLKCTL,
        delay << UNICOMMSPI_CLKCTL_DSAMPLE_OFS,
        UNICOMMSPI_CLKCTL_DSAMPLE_MASK);
}

/**
 *  @brief      Get the delay sampling
 *
 *  In controller mode only, the data on the input pin will be delayed sampled
 *  by the defined SPI clock cycles. The delay can be adjusted in steps of SPI
 *  input clock steps. The maximum allowed delay should not exceed the length
 *  of one data frame
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     The amount of delay sampling on the input pin in SPI
 *              clock cycles.
 *
 *  @retval     0 - 15. The amount of delay sampling in SPI clock cycles.
 */
__STATIC_INLINE uint32_t DL_SPI_getDelayedSampling(UNICOMM_Inst_Regs *unicomm)
{
    return ((unicomm->spi->CLKCTL & UNICOMMSPI_CLKCTL_DSAMPLE_MASK) >>
            UNICOMMSPI_CLKCTL_DSAMPLE_OFS);
}

/**
 *  @brief      Set the RX and TX FIFO interrupt threshold level
 *
 *  Select the threshold for the receive and transmit FIFO interrupts. The
 *  interrupts are generated based on a transition through a level rather
 *  than being based on the level. That is, the interrupts are generated when
 *  the fill level progresses through the trigger level.
 *
 *  For example, if the trigger level is set to the half-way mark, the interrupt
 *  is triggered when the FIFO becomes half empty/full.
 *
 *  Out of reset, the FIFOs are triggered to interrupt at half-way mark.
 *
 *  @param[in]  unicomm             Pointer to the register overlay for the peripheral
 *  @param[in]  rxThreshold     One of @ref DL_SPI_RX_FIFO_LEVEL
 *  @param[in]  txThreshold     One of @ref DL_SPI_TX_FIFO_LEVEL
 *
 */
__STATIC_INLINE void DL_SPI_setFIFOThreshold(UNICOMM_Inst_Regs *unicomm,
    DL_SPI_RX_FIFO_LEVEL rxThreshold, DL_SPI_TX_FIFO_LEVEL txThreshold)
{
    DL_Common_updateReg(&unicomm->spi->IFLS,
        (uint32_t) rxThreshold | (uint32_t) txThreshold,
        UNICOMMSPI_IFLS_RXIFLSEL_MASK | UNICOMMSPI_IFLS_TXIFLSEL_MASK);
}

/**
 *  @brief      Get the TX FIFO interrupt threshold level
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     The TX FIFO interrupt threshold level
 *
 *  @retval     One of @ref DL_SPI_TX_FIFO_LEVEL
 */
__STATIC_INLINE DL_SPI_TX_FIFO_LEVEL DL_SPI_getTXFIFOThreshold(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t txThreshold = unicomm->spi->IFLS & UNICOMMSPI_IFLS_TXIFLSEL_MASK;

    return (DL_SPI_TX_FIFO_LEVEL)(txThreshold);
}

/**
 *  @brief      Get the RX FIFO interrupt threshold level
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     The RX FIFO interrupt threshold level
 *
 *  @retval     One of @ref DL_SPI_RX_FIFO_LEVEL
 */
__STATIC_INLINE DL_SPI_RX_FIFO_LEVEL DL_SPI_getRXFIFOThreshold(
    UNICOMM_Inst_Regs *unicomm)
{
    uint32_t rxThreshold = unicomm->spi->IFLS & UNICOMMSPI_IFLS_RXIFLSEL_MASK;

    return (DL_SPI_RX_FIFO_LEVEL)(rxThreshold);
}

/**
 *  @brief      Flushes/removes all elements in the TX FIFO
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_flushTXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    DL_Common_updateReg(&unicomm->spi->IFLS,
        (uint32_t) UNICOMMSPI_IFLS_TXCLR_ENABLE, UNICOMMSPI_IFLS_TXCLR_MASK);
}

/**
 *  @brief      Flushes/removes all elements in the RX FIFO
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_flushRXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    DL_Common_updateReg(&unicomm->spi->IFLS,
        (uint32_t) UNICOMMSPI_IFLS_RXCLR_ENABLE, UNICOMMSPI_IFLS_RXCLR_MASK);
}

/**
 *  @brief      Set the SPI bit rate serial clock divider (SCR)
 *
 * The SPI includes a programmable bit rate clock divider and prescaler to
 * generate the serial output clock. The bit rates are supported up to
 * FUNCCLK/2. The functional clock selection depends on the specific device,
 * please refer to the device datasheet and PMU/Clock section.
 *
 * The SPI output bit rate is calculated with the following formula:
 * SPI bit rate = (SPI functional clock) / ((1 + SCR)*2)
 * Given this formula, the SCR can be calculated:
 * SCR = (SPI functional clock) / ((2 * SPI bit rate) - 1)
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *  @param[in]  SCR  The SPI serial clock divider. Value between 0-1023.
 */
__STATIC_INLINE void DL_SPI_setBitRateSerialClockDivider(
    UNICOMM_Inst_Regs *unicomm, uint32_t SCR)
{
    DL_Common_updateReg(
        &unicomm->spi->CLKCTL, SCR, UNICOMMSPI_CLKCTL_SCR_MASK);
}

/**
 *  @brief      Get the SPI bit rate serial clock divider (SCR)
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @return     The current bit rate serial clock divider
 *
 *  @retval     The SPI SCR. Value from 0-1023
 */
__STATIC_INLINE uint32_t DL_SPI_getBitRateSerialClockDivider(
    UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->spi->CLKCTL & UNICOMMSPI_CLKCTL_SCR_MASK);
}

/**
 *  @brief      Writes 8-bit data into the TX FIFO for transmit
 *
 *  Puts the data into the TX FIFO without checking its status. Use if already
 *  sure the TX FIFO has space for the write. See related APIs for additional
 *  transmit options.
 *
 *  Can be used for any data transfers that are less than or equal to 8 bits.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *  @param[in]  data  data to send
 *
 *  @sa         DL_SPI_transmitDataBlocking8
 *  @sa         DL_SPI_transmitDataCheck8
 */
__STATIC_INLINE void DL_SPI_transmitData8(
    UNICOMM_Inst_Regs *unicomm, uint8_t data)
{
    unicomm->spi->TXDATA = data;
}

/**
 *  @brief      Writes 16-bit data into the TX FIFO for transmit
 *
 *  Puts the data into the TX FIFO without checking its status. Use if already
 *  sure the TX FIFO has space for the write. See related APIs for additional
 *  transmit options.
 *
 *  Can be used for any data transfers that are less than or equal to 16 bits.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *  @param[in]  data  data to send
 *
 *  @sa         DL_SPI_transmitDataBlocking16
 *  @sa         DL_SPI_transmitDataCheck16
 */
__STATIC_INLINE void DL_SPI_transmitData16(
    UNICOMM_Inst_Regs *unicomm, uint16_t data)
{
    unicomm->spi->TXDATA = data;
}

/**
 *  @brief      Writes 32-bit data into the TX FIFO for transmit
 *
 *  Puts the data into the TX FIFO without checking its status. Use if already
 *  sure the TX FIFO has space for the write. See related APIs for additional
 *  transmit options.
 *
 *  Can be used for any data transfers that are less than or equal to 32 bits.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *  @param[in]  data  data to send
 *
 *  @sa         DL_SPI_transmitDataBlocking32
 *  @sa         DL_SPI_transmitDataCheck32
 */
__STATIC_INLINE void DL_SPI_transmitData32(
    UNICOMM_Inst_Regs *unicomm, uint32_t data)
{
    unicomm->spi->TXDATA = data;
}

/**
 *  @brief      Reads 8-bit data from the RX FIFO
 *
 *  Reads the data from the RX FIFO without checking its status. Use if
 *  already sure the RX FIFO has data available. See related APIs for
 *  additional receive options.
 *
 *  Can be used for any data transfers that are less than or equal to 8 bits.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *
 *  @return     The data in the RX FIFO
 *
 *  @sa         DL_SPI_receiveDataBlocking8
 *  @sa         DL_SPI_receiveDataCheck8
 */
__STATIC_INLINE uint8_t DL_SPI_receiveData8(UNICOMM_Inst_Regs *unicomm)
{
    return ((uint8_t)(unicomm->spi->RXDATA));
}

/**
 *  @brief      Reads 16-bit data from the RX FIFO
 *
 *  Reads the data from the RX FIFO without checking its status. Use if
 *  already sure the RX FIFO has data available. See related APIs for
 *  additional receive options.
 *
 *  Can be used for any data transfers that are less than or equal to 16 bits.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *
 *  @return     The data in the RX FIFO
 *
 *  @sa         DL_SPI_receiveDataBlocking16
 *  @sa         DL_SPI_receiveDataCheck16
 */
__STATIC_INLINE uint16_t DL_SPI_receiveData16(UNICOMM_Inst_Regs *unicomm)
{
    return ((uint16_t)(unicomm->spi->RXDATA));
}

/**
 *  @brief      Reads 32-bit data from the RX FIFO
 *
 *  Reads the data from the RX FIFO without checking its status. Use if
 *  already sure the RX FIFO has data available. See related APIs for
 *  additional receive options.
 *
 *  Can be used for any data transfers that are less than or equal to 32 bits.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *
 *  @return     The data in the RX FIFO
 *
 *  @sa         DL_SPI_receiveDataBlocking32
 *  @sa         DL_SPI_receiveDataCheck32
 */
__STATIC_INLINE uint32_t DL_SPI_receiveData32(UNICOMM_Inst_Regs *unicomm)
{
    return unicomm->spi->RXDATA;
}

/**
 *  @brief      Enable SPI interrupts
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to enable. Bitwise OR of
 *                             @ref DL_SPI_INTERRUPT.
 */
__STATIC_INLINE void DL_SPI_enableInterrupt(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    unicomm->spi->CPU_INT.IMASK |= interruptMask;
}

/**
 *  @brief      Disable SPI interrupts
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to disable. Bitwise OR of
 *                             @ref DL_SPI_INTERRUPT.
 */
__STATIC_INLINE void DL_SPI_disableInterrupt(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    unicomm->spi->CPU_INT.IMASK &= ~(interruptMask);
}

/**
 *  @brief      Check which SPI interrupts are enabled
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPI_INTERRUPT.
 *
 *  @return     Which of the requested SPI interrupts are enabled
 *
 *  @retval     Bitwise OR of @ref DL_SPI_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_SPI_getEnabledInterrupts(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    return (unicomm->spi->CPU_INT.IMASK & interruptMask);
}

/**
 *  @brief      Check interrupt flag of enabled SPI interrupts
 *
 *  Checks if any of the SPI interrupts that were previously enabled are
 *  pending.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPI_INTERRUPT.
 *
 *  @return     Which of the requested SPI interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_SPI_INTERRUPT values
 *
 *  @sa         DL_SPI_enableInterrupt
 */
__STATIC_INLINE uint32_t DL_SPI_getEnabledInterruptStatus(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    return (unicomm->spi->CPU_INT.MIS & interruptMask);
}

/**
 *  @brief      Check interrupt flag of any SPI interrupt
 *
 *  Checks if any of the SPI interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPI_INTERRUPT.
 *
 *  @return     Which of the requested SPI interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_SPI_INTERRUPT values
 */
__STATIC_INLINE uint32_t DL_SPI_getRawInterruptStatus(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    return (unicomm->spi->CPU_INT.RIS & interruptMask);
}

/**
 *  @brief      Get highest priority pending SPI interrupt
 *
 *  Checks if any of the SPI interrupts are pending. Interrupts do not have to
 *  be previously enabled.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *
 *  @return     The highest priority pending SPI interrupt. One of @ref
 *               DL_SPI_IIDX
 */
__STATIC_INLINE DL_SPI_IIDX DL_SPI_getPendingInterrupt(
    UNICOMM_Inst_Regs *unicomm)
{
    return ((DL_SPI_IIDX) unicomm->spi->CPU_INT.IIDX);
}

/**
 *  @brief      Clear pending SPI interrupts
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to clear. Bitwise OR of
 *                             @ref DL_SPI_INTERRUPT.
 */
__STATIC_INLINE void DL_SPI_clearInterruptStatus(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    unicomm->spi->CPU_INT.ICLR = interruptMask;
}

/**
 *  @brief      Blocks to ensure transmit is ready before sending data
 *
 *  Puts the data into the TX FIFO after blocking to ensure the TX FIFO is not
 *  full. Will wait indefinitely until there is space in the TX FIFO. See
 *  related APIs for additional transmit options.
 *
 *  Can be used for any data transfers that are less than or equal to 8 bits.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *  @param[in]  data  data to send
 *
 *  @sa         DL_SPI_transmitData8
 *  @sa         DL_SPI_transmitDataCheck8
 */
void DL_SPI_transmitDataBlocking8(UNICOMM_Inst_Regs *unicomm, uint8_t data);

/**
 *  @brief      Blocks to ensure transmit is ready before sending data
 *
 *  Puts the data into the TX FIFO after blocking to ensure the TX FIFO is not
 *  full. Will wait indefinitely until there is space in the TX FIFO. See related
 *  APIs for additional transmit options.
 *
 *  Can be used for any data transfers that are less than or equal to 16 bits.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *  @param[in]  data  data to send
 *
 *  @sa         DL_SPI_transmitData16
 *  @sa         DL_SPI_transmitDataCheck16
 */
void DL_SPI_transmitDataBlocking16(UNICOMM_Inst_Regs *unicomm, uint16_t data);

/**
 *  @brief      Blocks to ensure transmit is ready before sending data
 *
 *  Puts the data into the TX FIFO after blocking to ensure the TX FIFO is not
 *  full. Will wait indefinitely until there is space in the TX FIFO. See related
 *  APIs for additional transmit options.
 *
 *  Can be used for any data transfers that are less than or equal to 32 bits.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *  @param[in]  data  data to send
 *
 *  @sa         DL_SPI_transmitData32
 *  @sa         DL_SPI_transmitDataCheck32
 */
void DL_SPI_transmitDataBlocking32(UNICOMM_Inst_Regs *unicomm, uint32_t data);

/**
 *  @brief      Blocks to ensure receive is ready before reading data
 *
 *  Reads the data from the RX FIFO after blocking to ensure the RX FIFO is not
 *  empty. Will wait indefinitely until there is data in the RX FIFO. See
 *  related APIs for additional receive options.
 *
 *  Can be used for any data transfers that are less than or equal to 8 bits.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *
 *  @return     The data in the RX FIFO
 *
 *  @sa         DL_SPI_transmitData8
 *  @sa         DL_SPI_transmitDataCheck8
 */
uint8_t DL_SPI_receiveDataBlocking8(UNICOMM_Inst_Regs *unicomm);

/**
 *  @brief      Blocks to ensure receive is ready before reading data
 *
 *  Reads the data from the RX FIFO after blocking to ensure the RX FIFO is not
 *  empty. Will wait indefinitely until there is data in the RX FIFO. See
 *  related APIs for additional receive options.
 *
 *  Can be used for any data transfers that are less than or equal to 16 bits.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *
 *  @return     The data in the RX FIFO
 *
 *  @sa         DL_SPI_transmitData16
 *  @sa         DL_SPI_transmitDataCheck16
 */
uint16_t DL_SPI_receiveDataBlocking16(UNICOMM_Inst_Regs *unicomm);

/**
 *  @brief      Blocks to ensure receive is ready before reading data
 *
 *  Reads the data from the RX FIFO after blocking to ensure the RX FIFO is not
 *  empty. Will wait indefinitely until there is data in the RX FIFO. See
 *  related APIs for additional receive options.
 *
 *  Can be used for any data transfers that are less than or equal to 32 bits.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *
 *  @return     The data in the RX FIFO
 *
 *  @sa         DL_SPI_transmitData32
 *  @sa         DL_SPI_transmitDataCheck32
 */
uint32_t DL_SPI_receiveDataBlocking32(UNICOMM_Inst_Regs *unicomm);

/**
 *  @brief      Checks the TX FIFO before trying to transmit data
 *
 *  Checks if the TX FIFO is already full before trying to add new data to the
 *  FIFO. Exits immediately if full rather than trying to block. See related
 *  APIs for additional transmit options.
 *
 *  Can be used for any data transfers that are less than or equal to 8 bits.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *  @param[in]  data  data to send
 *
 *  @return     If the transmit occurred
 *
 *  @retval     true  if data was added to the TX FIFO
 *  @retval     false if the TX FIFO was full and data was not added
 *
 *  @sa         DL_SPI_transmitData8
 *  @sa         DL_SPI_transmitDataBlocking8
 */
bool DL_SPI_transmitDataCheck8(UNICOMM_Inst_Regs *unicomm, uint8_t data);

/**
 *  @brief      Checks the TX FIFO before trying to transmit data
 *
 *  Checks if the TX FIFO is already full before trying to add new data to the
 *  FIFO. Exits immediately if full rather than trying to block. See related
 *  APIs for additional transmit options.
 *
 *  Can be used for any data transfers that are less than or equal to 16 bits.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *  @param[in]  data  data to send
 *
 *  @return     If the transmit occurred
 *
 *  @retval     true  if data was added to the TX FIFO
 *  @retval     false if the TX FIFO was full and data was not added
 *
 *  @sa         DL_SPI_transmitData16
 *  @sa         DL_SPI_transmitDataBlocking16
 */
bool DL_SPI_transmitDataCheck16(UNICOMM_Inst_Regs *unicomm, uint16_t data);

/**
 *  @brief      Checks the TX FIFO before trying to transmit data
 *
 *  Checks if the TX FIFO is already full before trying to add new data to the
 *  FIFO. Exits immediately if full rather than trying to block. See related
 *  APIs for additional transmit options.
 *
 *  Can be used for any data transfers that are less than or equal to 32 bits.
 *
 *  @param[in]  unicomm   pointer to the register overlay for the peripheral
 *  @param[in]  data  data to send
 *
 *  @return     If the transmit occurred
 *
 *  @retval     true  if data was added to the TX FIFO
 *  @retval     false if the TX FIFO was full and data was not added
 *
 *  @sa         DL_SPI_transmitData32
 *  @sa         DL_SPI_transmitDataBlocking32
 */
bool DL_SPI_transmitDataCheck32(UNICOMM_Inst_Regs *unicomm, uint32_t data);

/**
 *  @brief      Checks the RX FIFO before trying to transmit data
 *
 *  Checks if the RX FIFO is already empty before trying to read new data from
 *  the FIFO. Exits immediately if empty rather than trying to block. See
 *  related APIs for additional receive options.
 *
 *  Can be used for any data transfers that are less than or equal to 8 bits.
 *
 *  @param[in]  unicomm    pointer to the register overlay for the peripheral
 *  @param[in]  buffer a buffer to write the received data into
 *
 *  @return     If the receive occurred
 *
 *  @retval     true  if data was read from the RX FIFO
 *  @retval     false if the RX FIFO was empty and data was not read
 *
 *  @sa         DL_SPI_receiveData8
 *  @sa         DL_SPI_receiveDataBlocking8
 */
bool DL_SPI_receiveDataCheck8(UNICOMM_Inst_Regs *unicomm, uint8_t *buffer);

/**
 *  @brief      Checks the RX FIFO before trying to transmit data
 *
 *  Checks if the RX FIFO is already empty before trying to read new data from
 *  the FIFO. Exits immediately if empty rather than trying to block. See
 *  related APIs for additional receive options.
 *
 *  Can be used for any data transfers that are less than or equal to 16 bits.
 *
 *  @param[in]  unicomm    pointer to the register overlay for the peripheral
 *  @param[in]  buffer a buffer to write the received data into
 *
 *  @return     If the receive occurred
 *
 *  @retval     true  if data was read from the RX FIFO
 *  @retval     false if the RX FIFO was empty and data was not read
 *
 *  @sa         DL_SPI_receiveData16
 *  @sa         DL_SPI_receiveDataBlocking16
 */
bool DL_SPI_receiveDataCheck16(UNICOMM_Inst_Regs *unicomm, uint16_t *buffer);

/**
 *  @brief      Checks the RX FIFO before trying to transmit data
 *
 *  Checks if the RX FIFO is already empty before trying to read new data from
 *  the FIFO. Exits immediately if empty rather than trying to block. See
 *  related APIs for additional receive options.
 *
 *  Can be used for any data transfers that are less than or equal to 32 bits.
 *
 *  @param[in]  unicomm    pointer to the register overlay for the peripheral
 *  @param[in]  buffer a buffer to write the received data into
 *
 *  @return     If the receive occurred
 *
 *  @retval     true  if data was read from the RX FIFO
 *  @retval     false if the RX FIFO was empty and data was not read
 *
 *  @sa         DL_SPI_receiveData32
 *  @sa         DL_SPI_receiveDataBlocking32
 */
bool DL_SPI_receiveDataCheck32(UNICOMM_Inst_Regs *unicomm, uint32_t *buffer);

/**
 *  @brief       Read all available data out of the RX FIFO using 8 bit access
 *
 *  @param[in]   unicomm   Pointer to the register overlay for the peripheral
 *  @param[out]  buffer    Buffer to write received data into
 *  @param[in]   maxCount  Max number of bytes to read from the RX FIFO
 *
 *  @return      Number of bytes read from the RX FIFO
 */
uint32_t DL_SPI_drainRXFIFO8(
    UNICOMM_Inst_Regs *unicomm, uint8_t *buffer, uint32_t maxCount);

/**
 *  @brief       Read all available data out of the RX FIFO using 16 bit access
 *
 *  @param[in]   unicomm   Pointer to the register overlay for the peripheral
 *  @param[out]  buffer    Buffer to write received data into
 *  @param[in]   maxCount  Max number of halfwords to read from the RX FIFO
 *
 *  @return      Number of halfwords read from the RX FIFO
 */
uint32_t DL_SPI_drainRXFIFO16(
    UNICOMM_Inst_Regs *unicomm, uint16_t *buffer, uint32_t maxCount);

/**
 *  @brief       Read all available data out of the RX FIFO using 32 bit access
 *
 *  @param[in]   unicomm   Pointer to the register overlay for the peripheral
 *  @param[out]  buffer    Buffer to write received data into
 *  @param[in]   maxCount  Max number of words to read from the RX FIFO
 *
 *  @return      Number of words read from the RX FIFO
 *
 */
uint32_t DL_SPI_drainRXFIFO32(
    UNICOMM_Inst_Regs *unicomm, uint32_t *buffer, uint32_t maxCount);

/**
 *  @brief      Fill the TX FIFO using 8 bit access
 *
 *  Continuously write data into the TX FIFO until it is filled up or count has
 *  been reached.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  buffer  Buffer of data to write to the TX FIFO
 *  @param[in]  count   Max number of bytes to write to the TX FIFO
 *
 *  @return     Number of bytes written to the TX FIFO
 */
uint32_t DL_SPI_fillTXFIFO8(
    UNICOMM_Inst_Regs *unicomm, uint8_t *buffer, uint32_t count);

/**
 *  @brief      Fill the TX FIFO using 16 bit access
 *
 *  Continuously write data into the TX FIFO until it is filled up or count has
 *  been reached.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  buffer  Buffer of data to write to the TX FIFO
 *  @param[in]  count   Max number of halfwords to write to the TX FIFO
 *
 *  @return     Number of halfwords written to the TX FIFO
 */
uint32_t DL_SPI_fillTXFIFO16(
    UNICOMM_Inst_Regs *unicomm, uint16_t *buffer, uint32_t count);

/**
 *  @brief      Fill the TX FIFO using 32 bit access
 *
 *  Continuously write data into the TX FIFO until it is filled up or count has
 *  been reached.
 *  entries.
 *
 *  @param[in]  unicomm     Pointer to the register overlay for the peripheral
 *  @param[in]  buffer  Buffer of data to write to the TX FIFO
 *  @param[in]  count   Max number of words to write to the TX FIFO
 *
 *  @return     Number of words written to the TX FIFO
 */
uint32_t DL_SPI_fillTXFIFO32(
    UNICOMM_Inst_Regs *unicomm, uint32_t *buffer, uint32_t count);

/**
 *  @brief      Enable SPI interrupt for triggering the DMA receive event
 *
 * Enables the SPI interrupt to be used as the condition to generate an
 * event to directly trigger the DMA. This API configures the DMA_TRIG_RX
 * register, which is the event publisher used for triggering the DMA to do
 * a receive data transfer.
 *
 * @note Only one interrupt source should be enabled at a time.
 *
 *  @param[in]  unicomm       Pointer to the register overlay for the
 *                         peripheral
 *  @param[in]  interrupt  Interrupt to enable as the trigger condition for
 *                         the DMA. One of @ref DL_SPI_DMA_INTERRUPT_RX.
 */
__STATIC_INLINE void DL_SPI_enableDMAReceiveEvent(
    UNICOMM_Inst_Regs *unicomm, uint32_t interrupt)
{
    unicomm->spi->DMA_TRIG_RX.IMASK = interrupt;
}

/**
 *  @brief      Enable SPI interrupt for triggering the DMA transmit event
 *
 * Enables the SPI interrupt to be used as the condition to generate an
 * event to directly trigger the DMA. This API configures the DMA_TRIG_TX
 * register, which is the event publisher used for triggering the DMA to do
 * a transmit data transfer.
 *
 * @note DMA_TRIG_TX only has one transmit interrupt source
 *
 *  @param[in]  unicomm       Pointer to the register overlay for the
 *                         peripheral
 */
__STATIC_INLINE void DL_SPI_enableDMATransmitEvent(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->DMA_TRIG_TX.IMASK = UNICOMMSPI_DMA_TRIG_TX_IMASK_TX_SET;
}

/**
 *  @brief      Disables SPI interrupt from triggering the DMA receive event
 *
 * Disables the SPI interrupt as the condition to generate an event to
 * directly trigger the DMA. This API configures the DMA_TRIG_RX
 * register, which is the event publisher used for triggering the DMA to do
 * a receive data transfer.
 *
 *  @param[in]  unicomm       Pointer to the register overlay for the
 *                         peripheral
 *  @param[in]  interrupt  Interrupt to disable as the trigger condition for
 *                         the DMA. One of @ref DL_SPI_DMA_INTERRUPT_RX.
 */
__STATIC_INLINE void DL_SPI_disableDMAReceiveEvent(
    UNICOMM_Inst_Regs *unicomm, uint32_t interrupt)
{
    unicomm->spi->DMA_TRIG_RX.IMASK &= ~(interrupt);
}

/**
 *  @brief      Disables SPI interrupt from triggering the DMA transmit event
 *
 * Disables the SPI interrupt as the condition to generate an event to
 * directly trigger the DMA. This API configures the DMA_TRIG_TX
 * register, which is the event publisher used for triggering the DMA to do
 * a transmit data transfer.
 *
 * @note DMA_TRIG_TX only has one transmit interrupt source
 *
 * @param[in]  unicomm       Pointer to the register overlay for the
 *                       peripheral
 */
__STATIC_INLINE void DL_SPI_disableDMATransmitEvent(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->DMA_TRIG_TX.IMASK = UNICOMMSPI_DMA_TRIG_TX_IMASK_TX_CLR;
}

/**
 *  @brief      Check which SPI interrupt for DMA receive events is enabled
 *
 *  This API checks the DMA_TRIG_RX register, which is the event publisher used
 *  for triggering the DMA to do a receive data transfer.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPI_DMA_INTERRUPT_RX.
 *
 *  @note Only one interrupt source should be enabled at a time.
 *
 *  @return     Which of the requested SPI interrupts is enabled
 *
 *  @retval     One of @ref DL_SPI_DMA_INTERRUPT_RX
 */
__STATIC_INLINE uint32_t DL_SPI_getEnabledDMAReceiveEvent(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    return (unicomm->spi->DMA_TRIG_RX.IMASK & interruptMask);
}

/**
 *  @brief      Check if SPI interrupt for DMA transmit event is enabled
 *
 *  This API checks the DMA_TRIG_TX register, which is the event publisher used
 *  for triggering the DMA to do a transmit data transfer.
 *
 *  @param[in]  unicomm           Pointer to the register overlay for the
 *                            peripheral
 *
 *  @return     The requested SPI interrupt status
 *
 *  @retval     DL_SPI_DMA_INTERRUPT_TX if enabled, 0 if not enabled
 */
__STATIC_INLINE uint32_t DL_SPI_getEnabledDMATransmitEvent(
    UNICOMM_Inst_Regs *unicomm)
{
    return (unicomm->spi->DMA_TRIG_TX.IMASK &
            UNICOMMSPI_DMA_TRIG_TX_IMASK_TX_MASK);
}

/**
 *  @brief      Check interrupt flag of enabled SPI interrupt for DMA receive event
 *
 * Checks if any of the SPI interrupts for the DMA receive event that were
 * previously enabled are pending.
 * This API checks the DMA_TRIG_RX register, which is the event publisher used
 * for triggering the DMA to do a receive data transfer.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPI_DMA_INTERRUPT_RX.
 *
 *  @note Only one interrupt source should be enabled at a time.
 *
 *  @return     The requested SPI interrupt status
 *
 *  @retval     One of @ref DL_SPI_DMA_INTERRUPT_RX
 *
 *  @sa         DL_SPI_enableDMAReceiveEvent
 */
__STATIC_INLINE uint32_t DL_SPI_getEnabledDMAReceiveEventStatus(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    return (unicomm->spi->DMA_TRIG_RX.MIS & interruptMask);
}

/**
 *  @brief      Check interrupt flag of enabled SPI interrupt for DMA transmit event
 *
 * Checks if the SPI interrupt for the DMA transmit event that was
 * previously enabled is pending.
 * This API checks the DMA_TRIG_TX register, which is the event publisher used
 * for triggering the DMA to do a transmit data transfer.
 *
 *  @param[in]  unicomm           Pointer to the register overlay for the
 *                            peripheral
 *
 *  @return     The requested SPI interrupt status
 *
 *  @retval     DL_SPI_DMA_INTERRUPT_TX if enabled, 0 if not enabled
 *
 *  @sa         DL_SPI_enableDMATransmitEvent
 */
__STATIC_INLINE uint32_t DL_SPI_getEnabledDMATransmitEventStatus(
    UNICOMM_Inst_Regs *unicomm)
{
    return (
        unicomm->spi->DMA_TRIG_TX.MIS & UNICOMMSPI_DMA_TRIG_TX_MIS_TX_MASK);
}

/**
 *  @brief      Check interrupt flag of any SPI interrupt for DMA receive event
 *
 *  Checks if any of the SPI interrupts for DMA receive event are pending.
 *  Interrupts do not have to be previously enabled.
 *  This API checks the DMA_TRIG_RX register, which is the event publisher used
 *  for triggering the DMA to do a receive data transfer.
 *
 *  @param[in]  unicomm            Pointer to the register overlay for the
 *                             peripheral
 *  @param[in]  interruptMask  Bit mask of interrupts to check. Bitwise OR of
 *                             @ref DL_SPI_DMA_INTERRUPT_RX.
 *
 *  @return     Which of the requested SPI interrupts are pending
 *
 *  @retval     Bitwise OR of @ref DL_SPI_DMA_INTERRUPT_RX values
 */
__STATIC_INLINE uint32_t DL_SPI_getRawDMAReceiveEventStatus(
    UNICOMM_Inst_Regs *unicomm, uint32_t interruptMask)
{
    return (unicomm->spi->DMA_TRIG_RX.RIS & interruptMask);
}

/**
 *  @brief      Check interrupt flag of any SPI interrupt for DMA transmit event
 *
 *  Checks if any of the SPI interrupts for DMA transmit event are pending.
 *  Interrupts do not have to be previously enabled.
 *  This API checks the DMA_TRIG_TX register, which is the event publisher used
 *  for triggering the DMA to do a transmit data transfer.
 *
 *  @param[in]  unicomm           Pointer to the register overlay for the
 *                            peripheral
 *
 *  @return     The requested SPI interrupt status
 *
 *  @retval     DL_SPI_DMA_INTERRUPT_TX if enabled, 0 if not enabled
 */
__STATIC_INLINE uint32_t DL_SPI_getRawDMATransmitEventStatus(
    UNICOMM_Inst_Regs *unicomm)
{
    return (
        unicomm->spi->DMA_TRIG_TX.RIS & UNICOMMSPI_DMA_TRIG_TX_RIS_TX_MASK);
}

/**
 *  @brief      Save SPI configuration before entering a power loss state.
 *
 *  Some peripherals residing in PD1 domain do not retain register
 *  contents when entering STOP or STANDBY modes. Please refer to the datasheet
 *  for the full list of peripheral instances that exhibit this behavior.
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @param[in]  ptr  Configuration backup setup structure. See
 *                  @ref DL_SPI_backupConfig.
 *
 *  @retval     FALSE if a configuration already exists in ptr (will not be
 *              overwritten). TRUE if a configuration was successfully saved
 *
 *  @sa         DL_SPI_restoreConfiguration
 */
bool DL_SPI_saveConfiguration(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_backupConfig *ptr);

/**
 *  @brief      Restore SPI configuration after leaving a power loss state.
 *
 *  Some peripherals residing in PD1 domain do not retain register
 *  contents when entering STOP or STANDBY modes. Please refer to the datasheet
 *  for the full list of peripheral instances that exhibit this behavior.
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 *
 *  @param[in]  ptr   Configuration backup setup structure. See
 *                    @ref DL_SPI_backupConfig.
 *
 *  @retval     FALSE if a configuration does not exist in ptr (will not be
 *              loaded). TRUE if a configuration successfully loaded
 *
 *  @sa         DL_SPI_saveConfiguration
 */
bool DL_SPI_restoreConfiguration(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_backupConfig *ptr);

/**
 *  @brief      Suspend external communication
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_enableSuspend(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL1 |= UNICOMMSPI_CTL1_SUSPEND_ENABLE;
}

/**
 *  @brief      Resume external communication
 *
 *  @param[in]  unicomm  Pointer to the register overlay for the peripheral
 */
__STATIC_INLINE void DL_SPI_disableSuspend(UNICOMM_Inst_Regs *unicomm)
{
    unicomm->spi->CTL1 &= ~(UNICOMMSPI_CTL1_SUSPEND_MASK);
}

#ifdef __cplusplus
}
#endif

#endif /* __MCU_HAS_UNICOMMSPI__ */

#endif /* ti_dl_dl_UNICOMMSPI__include */
/** @}*/
