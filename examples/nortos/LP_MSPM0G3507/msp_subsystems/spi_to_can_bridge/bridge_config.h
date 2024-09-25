/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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

#include "ti_msp_dl_config.h"

#define CAN_POWER_STARTUP_DELAY                                           (500)

#define CAN_INST                                                       (CANFD0)

/* Defines for GPIO configuration for CANFD0 */
#define GPIO_CAN_PORT                                                   (GPIOA)
#define GPIO_CAN_TX_PIN                                        (DL_GPIO_PIN_12)
#define GPIO_IOMUX_CAN_TX                                       (IOMUX_PINCM34)
#define GPIO_IOMUX_CAN_TX_FUNC                  (IOMUX_PINCM34_PF_CANFD0_CANTX)

#define GPIO_CAN_RX_PIN                                        (DL_GPIO_PIN_13)
#define GPIO_IOMUX_CAN_RX                                       (IOMUX_PINCM35)
#define GPIO_IOMUX_CAN_RX_FUNC                  (IOMUX_PINCM35_PF_CANFD0_CANRX)


/* Defines for CANFD0 Message frame configurations */
#define DL_MCAN_STD_ID_FILT_START_ADDR                                   (0x0U)
#define DL_MCAN_STD_ID_FILTER_NUM                                          (8U)
#define DL_MCAN_EXT_ID_FILT_START_ADDR                                  (0x24U)
#define DL_MCAN_EXT_ID_FILTER_NUM                                          (8U)

#define DL_MCAN_TX_BUFF_START_ADDR                                      (0x88U)
#define DL_MCAN_TX_BUFF_SIZE                                               (4U)
#define DL_MCAN_TX_FIFO_QUEUE_SIZE                                         (4U)
#define DL_MCAN_TX_BUF_MODE_FIFO_QUEUE                                     (0U)

#define DL_MCAN_TX_EVENT_START_ADDR                                      (0x0U)
#define DL_MCAN_TX_EVENT_SIZE                                              (0U)
#define DL_MCAN_TX_EVENT_SIZE_WATERMARK                                    (0U)

#define DL_MCAN_FIFO_0_START_ADDR                                      (0x1D8U)
#define DL_MCAN_FIFO_0_NUM                                                 (4U)
#define DL_MCAN_FIFO_0_WATERMARK                                           (0U)
#define DL_MCAN_FIFO_0_OPMODE_BLOCKING                                     (0U)
#define DL_MCAN_FIFO_0_OPMODE_OVERWRITE                                    (1U)

#define DL_MCAN_FIFO_1_START_ADDR                                        (0x0U)
#define DL_MCAN_FIFO_1_WATERMARK                                           (0U)
#define DL_MCAN_FIFO_1_NUM                                                 (0U)
#define DL_MCAN_FIFO_1_OPMODE_BLOCKING                                     (0U)
#define DL_MCAN_FIFO_1_OPMODE_OVERWRITE                                    (1U)

#define DL_MCAN_RX_BUFF_START_ADDR                                     (0x288U)
#define RXBUF_SIZE                                                        (4U)

#define DL_MCAN_EXT_ID_AND_MASK                                   (0x1FFFFFFFU)

#define AVV_MCAN_ELEM_SIZE                                                 (40)
#define AVV_MCAN_RXFIFO_START_SOC_ADDR                           (DL_MCAN_FIFO_0_START_ADDR + CANFD0_BASE)
#define AVV_MCAN_TXBUF_START_SOC_ADDR                            (DL_MCAN_TX_BUFF_START_ADDR + CANFD0_BASE)
#define AVV_MCAN_RXBUF_START_SOC_ADDR                            (DL_MCAN_RX_BUFF_START_ADDR + CANFD0_BASE)
#define AVV_MCAN_TXFIFO_START_SOC_ADDR                           (DL_MCAN_TX_BUFF_START_ADDR + CANFD0_BASE + (DL_MCAN_TX_BUFF_SIZE * AVV_MCAN_ELEM_SIZE))

#define AVV_MCAN_RXFOS_FOGI_Msk                                    (0x00003F00)
#define AVV_MCAN_RXFOS_FOGI_Pos                                             (8)

#define AVV_MCAN_RXBUFFER_STATUS_MASK                              (0x0000000F)

#define AVV_MCAN_TXFOS_FOPI_Msk                                    (0x001F0000)
#define AVV_MCAN_TXFOS_FOPI_Pos                                            (16)

#define AVV_MCAN_RXF0S_FOFL_Msk                                    (0x0000000F)
#define AVV_MCAN_RXF0S_FOF_Msk                                     (0x01000000)
#define AVV_MCAN_RXF0S_FOF_Pos                                             (24)
#define AVV_MCAN_NDAT1_Msk                                         (0x0000000F)
#define AVV_MCAN_LEC_Msk                                           (0x00000007)
#define AVV_MCAN_DLEC_Msk                                          (0x00000700)

#define AVV_MCAN_TIMEOUT_SIGNAL_OFS                                        (15)
#define AVV_MCAN_ERROR_SIGNAL_OFS                                          (14)
#define AVV_MCAN_DLEC_OFS                                                  (11)
#define AVV_MCAN_LEC_OFS                                                    (8)
#define AVV_MCAN_NDAT_STATUS_OFS                                            (4)
#define AVV_MCAN_RxFIFO0_Full_OFS                                           (3)

#define AVV_OFFSET_ADDRESS_ARRAY_SIZE                                     (107)
#define AVV_INSTRUCTION_MASK                                           (0xFFF8)

/*32 bytes payload => 40 bytes CAN element size => 20 half-words(16 bits) + 1 instruction half-word = 21 (used in this example)*/
#define SPI_RX_SIZE                              (21)
#define SPI_TX_SIZE                              (21)

#define offset_address_size                      (30)

/* Variables for  receive */
extern uint16_t gSpiReceiveGroup[SPI_RX_SIZE];

/* Variables for transmit */
extern uint16_t gSpiTransmitGroup[SPI_TX_SIZE];

/* Variable for TIMEOUT status */
extern bool gtimeout_status;

/* Variable for DMA operation indication */
extern bool gDMA_operation;

/* Defines */
#define REGISTER_DATASIZE                     (2)
#define CAN_POWER_ENABLE_OFFSET_ADDRESS       (0x6800)
#define CAN_POWER_ENABLE_REGISTER_DATA        (0x26000001)
#define CAN_RESET_OFFSET_ADDRESS              (0x6804)
#define CAN_RESET_REGISTER_DATA               (0xB1000001)
#define CAN_TX_BUF_NUM_MASK                   (0b11)
#define CAN_RX_BUF_NUM_MASK                   (0b11)

/* Duration of GPIO Toggle */
#define GPIO_TOGGLE_DELAY                     (10)

/* Defines for DMA_CH0 */
#define DMA_CH0_CHAN_ID                                                      (0)
#define SPI_1_INST_DMA_TRIGGER_RX                                    (DMA_SPI1_RX_TRIG)

/* Defines for DMA_CH1 */
#define DMA_CH1_CHAN_ID                                                      (1)
#define SPI_1_INST_DMA_TRIGGER_TX                                       (DMA_SPI1_TX_TRIG)
/* Defines for DMA_CH1 */
#define DMA_CH2_CHAN_ID                                                      (2)
#define DMA_CH2_TRIGGER_SEL_SW                                       (DMA_SOFTWARE_TRIG)


typedef enum
{
    AVV_BIT_MODIFY_INSTRUCTION                          = 0b0000000000010000,
    AVV_RESET_INSTRUCTION                               = 0b0000000000100000,
    AVV_POWER_ENABLE_AND_CAN_CONFIG_INSTRUCTION         = 0b0000000000110000,
    AVV_LOAD_TX_BUFFER_INSTRUCTION                      = 0b0000000001000000,
    AVV_LOAD_TX_FIFO_INSTRUCTION                        = 0b0000000001010000,
    AVV_REQUEST_TO_SEND_TXBUFFER_INSTRUCTION            = 0b0000000001100000,
    AVV_REQUEST_TO_SEND_TXFIFO_INSTRUCTION              = 0b0000000001110000,
    AVV_WRITE_REGISTER_INSTRUCTION                      = 0b0000000010000000,
    AVV_READ_REGISTER_DATA_INSTRUCTION                  = 0b0000000010010000,
    AVV_FETCH_REGISTER_DATA_INSTRUCTION                 = 0b0000000010100000,
    AVV_READ_RX_BUF_DATA_INSTRUCTION                    = 0b0000000010110000,
    AVV_FETCH_RX_BUF_DATA_INSTRUCTION                   = 0b0000000011000000,
    AVV_READ_RX_FIFO_DATA_INSTRUCTION                   = 0b0000000011010000,
    AVV_FETCH_RX_FIFO_DATA_INSTRUCTION                  = 0b0000000011100000,
    AVV_READ_RX_STATUS                                  = 0b0000000011110000,
    AVV_FETCH_RX_STATUS                                 = 0b0000000000001000
} AVV_INSTRUCTION_CHECKING;





void SYSCFG_DL_GPIO_init_MCAN(void);
void SYSCFG_DL_MCAN_init(void);
void SYSCFG_DL_user_init(void);
bool instruction_check(uint16_t *SpiReceiveGroup);
bool address_check(uint16_t offset_address);
uint16_t DL_SPI_receiveDataBlocking16_tilltimeout(SPI_Regs *spi);
