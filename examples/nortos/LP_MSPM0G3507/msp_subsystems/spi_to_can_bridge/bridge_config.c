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
#include "bridge_config.h"

SYSCONFIG_WEAK void SYSCFG_DL_user_init(void)
{
    SYSCFG_DL_GPIO_init_MCAN();
    DL_MCAN_enablePower(CAN_INST);
    delay_cycles(CAN_POWER_STARTUP_DELAY);
    SYSCFG_DL_MCAN_init();
}

/* GPIO Configurations for CANFD */
SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init_MCAN(void)
{
    /* CAN GPIO PINS configuration */
    DL_GPIO_initPeripheralInputFunction(GPIO_IOMUX_CAN_RX, GPIO_IOMUX_CAN_RX_FUNC);
    DL_GPIO_initPeripheralOutputFunction(GPIO_IOMUX_CAN_TX, GPIO_IOMUX_CAN_TX_FUNC);
}


/* MCAN clock Configuration */
static const DL_MCAN_ClockConfig clkConfig = {
  .clockSel = DL_MCAN_FCLK_SYSPLLCLK1,
  .divider  = DL_MCAN_FCLK_DIV_1,
};

/* MCAN Message Ram Address Configuration */
static const DL_MCAN_MsgRAMConfigParams msgRAMConfigParams = {
    /* Filter configuration */
    .flssa                = DL_MCAN_STD_ID_FILT_START_ADDR,         /* Standard ID Filter List Start Address. */
    .lss                  = DL_MCAN_STD_ID_FILTER_NUM,              /* List Size: Standard ID. */
    .flesa                = DL_MCAN_EXT_ID_FILT_START_ADDR,         /* Extended ID Filter List Start Address. */
    .lse                  = DL_MCAN_EXT_ID_FILTER_NUM,              /* List Size: Extended ID. */

    .txStartAddr          = DL_MCAN_TX_BUFF_START_ADDR,             /* Tx Buffers Start Address. */
    .txBufNum             = DL_MCAN_TX_BUFF_SIZE,                   /* Number of Dedicated Transmit Buffers. */
    .txFIFOSize           = DL_MCAN_TX_FIFO_QUEUE_SIZE,             /* No. of Tx FIFO/Queue. */
    .txBufMode            = DL_MCAN_TX_BUF_MODE_FIFO_QUEUE,         /* Tx FIFO operation */
    .txBufElemSize        = DL_MCAN_ELEM_SIZE_32BYTES,              /* Tx Buffer Element Size. */
    .txEventFIFOStartAddr = DL_MCAN_TX_EVENT_START_ADDR,            /* Tx Event FIFO Start Address. */
    .txEventFIFOSize      = DL_MCAN_TX_BUFF_SIZE,                   /* Event FIFO Size. */
    .txEventFIFOWaterMark = DL_MCAN_TX_EVENT_SIZE_WATERMARK,        /* Level for Tx Event FIFO watermark interrupt. */

    /* Rx FIFO 0 configuration */
    .rxFIFO0startAddr     = DL_MCAN_FIFO_0_START_ADDR,              /* Rx FIFO0 Start Address. */
    .rxFIFO0size          = DL_MCAN_FIFO_0_NUM,                     /* Number of Rx FIFO elements. */
    .rxFIFO0waterMark     = DL_MCAN_FIFO_0_WATERMARK,               /* Rx FIFO0 Watermark. */
    .rxFIFO0OpMode        = DL_MCAN_FIFO_0_OPMODE_BLOCKING,         /* FIFO blocking mode. */
    .rxFIFO0ElemSize      = DL_MCAN_ELEM_SIZE_32BYTES,              /* Rx FIFO 0 Element Size. */

    /* Rx FIFO 1 configuration */
    .rxFIFO1startAddr     = DL_MCAN_FIFO_1_START_ADDR,              /* Rx FIFO1 Start Address. */
    .rxFIFO1size          = DL_MCAN_FIFO_1_NUM,                     /* Number of Rx FIFO elements. */
    .rxFIFO1waterMark     = DL_MCAN_FIFO_1_WATERMARK,               /* Level for Rx FIFO 1 watermark interrupt. */
    .rxFIFO1OpMode        = DL_MCAN_FIFO_1_OPMODE_BLOCKING,         /* FIFO blocking mode. */
    .rxFIFO1ElemSize      = DL_MCAN_ELEM_SIZE_32BYTES,              /* Rx FIFO 1 Element Size. */

    /* Rx Buffer configuration */
    .rxBufStartAddr       = DL_MCAN_RX_BUFF_START_ADDR,             /* Rx Buffer Start Address. */
    .rxBufElemSize        = DL_MCAN_ELEM_SIZE_32BYTES,              /* Rx Buffer Element Size. */
};


void SYSCFG_DL_MCAN_init(void)
{
    DL_MCAN_setClockConfig(CAN_INST, (DL_MCAN_ClockConfig *) &clkConfig);

    DL_MCAN_enableModuleClock(CAN_INST);

    /* Wait for Memory initialization to be completed. */
    while (false == DL_MCAN_isMemInitDone(CAN_INST));

    /* Put MCAN in SW initialization mode. */
    DL_MCAN_setOpMode(CAN_INST, DL_MCAN_OPERATION_MODE_SW_INIT);

    /* Wait till MCAN is not initialized. */
    while (DL_MCAN_OPERATION_MODE_SW_INIT != DL_MCAN_getOpMode(CAN_INST));

    /* Configure Message RAM Sections */
    DL_MCAN_msgRAMConfig(CAN_INST, (DL_MCAN_MsgRAMConfigParams *) &msgRAMConfigParams);

    /* Take MCAN out of the SW initialization mode */
    DL_MCAN_setOpMode(CAN_INST, DL_MCAN_OPERATION_MODE_NORMAL);

    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(CAN_INST));

}












