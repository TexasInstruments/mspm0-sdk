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

/*
 *  ============ ti_msp_dl_config.c =============
 *  Configured MSPM0 DriverLib module definitions
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */

#include "ti_msp_dl_config.h"

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform any initialization needed before using any board APIs
 */
SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower_();
    SYSCFG_DL_GPIO_init_();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init_();
    SYSCFG_DL_MCAN0_init();
    SYSCFG_DL_SYSCTL_CLK_init_();
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower_(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_MCAN_reset(MCAN0_INST);

    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    DL_MCAN_enablePower(MCAN0_INST);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init_(void)
{
    DL_GPIO_initPeripheralAnalogFunction(GPIO_HFXIN_IOMUX);
    DL_GPIO_initPeripheralAnalogFunction(GPIO_HFXOUT_IOMUX);

    DL_GPIO_initPeripheralOutputFunction(
        GPIO_MCAN0_IOMUX_CAN_TX, GPIO_MCAN0_IOMUX_CAN_TX_FUNC);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_MCAN0_IOMUX_CAN_RX, GPIO_MCAN0_IOMUX_CAN_RX_FUNC);
}

static const DL_SYSCTL_SYSPLLConfig gSYSPLLConfig = {
    .inputFreq   = DL_SYSCTL_SYSPLL_INPUT_FREQ_32_48_MHZ,
    .rDivClk2x   = 3,
    .rDivClk1    = 1,
    .rDivClk0    = 1,
    .enableCLK2x = DL_SYSCTL_SYSPLL_CLK2X_ENABLE,
    .enableCLK1  = DL_SYSCTL_SYSPLL_CLK1_ENABLE,
    .enableCLK0  = DL_SYSCTL_SYSPLL_CLK0_DISABLE,
    .sysPLLMCLK  = DL_SYSCTL_SYSPLL_MCLK_CLK2X,
    .sysPLLRef   = DL_SYSCTL_SYSPLL_REF_HFCLK,
    .qDiv        = 3,
    .pDiv        = DL_SYSCTL_SYSPLL_PDIV_1};
SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init_(void)
{
    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);
    /* Set default configuration */
    DL_SYSCTL_disableHFXT();
    DL_SYSCTL_disableSYSPLL();
    DL_SYSCTL_setHFCLKSourceHFXTParams(
        DL_SYSCTL_HFXT_RANGE_32_48_MHZ, 100, true);
    DL_SYSCTL_configSYSPLL((DL_SYSCTL_SYSPLLConfig *) &gSYSPLLConfig);
    DL_SYSCTL_setMCLKSource(SYSOSC, HSCLK, DL_SYSCTL_HSCLK_SOURCE_SYSPLL);
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_2);

    //Low Power Mode is configured to be SLEEP0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_0);
    DL_SYSCTL_setFlashWaitState(DL_SYSCTL_FLASH_WAIT_STATE_2);
}
SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_CLK_init_(void)
{
    while (
        (DL_SYSCTL_getClockStatus() & (DL_SYSCTL_CLK_STATUS_SYSPLL_GOOD |
                                          DL_SYSCTL_CLK_STATUS_HFCLK_GOOD |
                                          DL_SYSCTL_CLK_STATUS_HSCLK_GOOD |
                                          DL_SYSCTL_CLK_STATUS_LFOSC_GOOD)) !=
        (DL_SYSCTL_CLK_STATUS_SYSPLL_GOOD | DL_SYSCTL_CLK_STATUS_HFCLK_GOOD |
            DL_SYSCTL_CLK_STATUS_HSCLK_GOOD |
            DL_SYSCTL_CLK_STATUS_LFOSC_GOOD)) {
        /* Ensure that clocks are in default POR configuration before initialization.
		* Additionally once LFXT is enabled, the internal LFOSC is disabled, and cannot
		* be re-enabled other than by executing a BOOTRST. */
        ;
    }
}

static const DL_MCAN_ClockConfig gMCAN0ClockConf = {
    .clockSel = DL_MCAN_FCLK_SYSPLLCLK1,
    .divider  = DL_MCAN_FCLK_DIV_1,
};

static const DL_MCAN_InitParams gMCAN0InitParams = {

    /* Initialize MCAN Init parameters.    */
    .fdMode          = false,
    .brsEnable       = false,
    .txpEnable       = false,
    .efbi            = false,
    .pxhddisable     = false,
    .darEnable       = true,
    .wkupReqEnable   = true,
    .autoWkupEnable  = true,
    .emulationEnable = true,
    .tdcEnable       = true,
    .wdcPreload      = 255,

    /* Transmitter Delay Compensation parameters. */
    .tdcConfig.tdcf = 10,
    .tdcConfig.tdco = 6,
};

static const DL_MCAN_ConfigParams gMCAN0ConfigParams = {
    /* Initialize MCAN Config parameters. */
    .monEnable         = false,
    .asmEnable         = false,
    .tsPrescalar       = 15,
    .tsSelect          = 0,
    .timeoutSelect     = DL_MCAN_TIMEOUT_SELECT_CONT,
    .timeoutPreload    = 65535,
    .timeoutCntEnable  = false,
    .filterConfig.rrfs = true,
    .filterConfig.rrfe = true,
    .filterConfig.anfe = 0,
    .filterConfig.anfs = 0,
};

static const DL_MCAN_MsgRAMConfigParams gMCAN0MsgRAMConfigParams = {

    /* Standard ID Filter List Start Address. */
    .flssa = MCAN0_INST_MCAN_STD_ID_FILT_START_ADDR,
    /* List Size: Standard ID. */
    .lss = MCAN0_INST_MCAN_STD_ID_FILTER_NUM,
    /* Extended ID Filter List Start Address. */
    .flesa = MCAN0_INST_MCAN_EXT_ID_FILT_START_ADDR,
    /* List Size: Extended ID. */
    .lse = MCAN0_INST_MCAN_EXT_ID_FILTER_NUM,
    /* Tx Buffers Start Address. */
    .txStartAddr = MCAN0_INST_MCAN_TX_BUFF_START_ADDR,
    /* Number of Dedicated Transmit Buffers. */
    .txBufNum   = MCAN0_INST_MCAN_TX_BUFF_SIZE,
    .txFIFOSize = 10,
    /* Tx Buffer Element Size. */
    .txBufMode     = 0,
    .txBufElemSize = DL_MCAN_ELEM_SIZE_64BYTES,
    /* Tx Event FIFO Start Address. */
    .txEventFIFOStartAddr = MCAN0_INST_MCAN_TX_EVENT_START_ADDR,
    /* Event FIFO Size. */
    .txEventFIFOSize = MCAN0_INST_MCAN_TX_BUFF_SIZE,
    /* Level for Tx Event FIFO watermark interrupt. */
    .txEventFIFOWaterMark = 0,
    /* Rx FIFO0 Start Address. */
    .rxFIFO0startAddr = MCAN0_INST_MCAN_FIFO_0_START_ADDR,
    /* Number of Rx FIFO elements. */
    .rxFIFO0size = MCAN0_INST_MCAN_FIFO_0_NUM,
    /* Rx FIFO0 Watermark. */
    .rxFIFO0waterMark = 3,
    .rxFIFO0OpMode    = 0,
    /* Rx FIFO1 Start Address. */
    .rxFIFO1startAddr = MCAN0_INST_MCAN_FIFO_1_START_ADDR,
    /* Number of Rx FIFO elements. */
    .rxFIFO1size = MCAN0_INST_MCAN_FIFO_1_NUM,
    /* Level for Rx FIFO 1 watermark interrupt. */
    .rxFIFO1waterMark = 0,
    /* FIFO blocking mode. */
    .rxFIFO1OpMode = 0,
    /* Rx Buffer Start Address. */
    .rxBufStartAddr = MCAN0_INST_MCAN_RX_BUFF_START_ADDR,
    /* Rx Buffer Element Size. */
    .rxBufElemSize = DL_MCAN_ELEM_SIZE_64BYTES,
    /* Rx FIFO0 Element Size. */
    .rxFIFO0ElemSize = DL_MCAN_ELEM_SIZE_64BYTES,
    /* Rx FIFO1 Element Size. */
    .rxFIFO1ElemSize = DL_MCAN_ELEM_SIZE_64BYTES,
};

static const DL_MCAN_StdMsgIDFilterElement gMCAN0StdFiltelem = {
    .sfec  = 0x1,
    .sft   = 0x2,
    .sfid1 = 3,
    .sfid2 = 1023,
};

static const DL_MCAN_BitTimingParams gMCAN0BitTimes = {
    /* Arbitration Baud Rate Pre-scaler. */
    .nomRatePrescalar = 0,
    /* Arbitration Time segment before sample point. */
    .nomTimeSeg1 = 33,
    /* Arbitration Time segment after sample point. */
    .nomTimeSeg2 = 4,
    /* Arbitration (Re)Synchronization Jump Width Range. */
    .nomSynchJumpWidth = 4,
    /* Data Baud Rate Pre-scaler. */
    .dataRatePrescalar = 0,
    /* Data Time segment before sample point. */
    .dataTimeSeg1 = 0,
    /* Data Time segment after sample point. */
    .dataTimeSeg2 = 0,
    /* Data (Re)Synchronization Jump Width.   */
    .dataSynchJumpWidth = 0,
};

SYSCONFIG_WEAK void SYSCFG_DL_MCAN0_init(void)
{
    DL_MCAN_RevisionId revid_MCAN0;

    DL_MCAN_setClockConfig(
        MCAN0_INST, (DL_MCAN_ClockConfig *) &gMCAN0ClockConf);

    DL_MCAN_enableModuleClock(MCAN0_INST);

    /* Get MCANSS Revision ID. */
    DL_MCAN_getRevisionId(MCAN0_INST, &revid_MCAN0);

    /* Wait for Memory initialization to be completed. */
    while (false == DL_MCAN_isMemInitDone(MCAN0_INST))
        ;

    /* Put MCAN in SW initialization mode. */

    DL_MCAN_setOpMode(MCAN0_INST, DL_MCAN_OPERATION_MODE_SW_INIT);

    /* Wait till MCAN is not initialized. */
    while (DL_MCAN_OPERATION_MODE_SW_INIT != DL_MCAN_getOpMode(MCAN0_INST))
        ;

    /* Initialize MCAN module. */
    DL_MCAN_init(MCAN0_INST, (DL_MCAN_InitParams *) &gMCAN0InitParams);

    /* Configure MCAN module. */
    DL_MCAN_config(MCAN0_INST, (DL_MCAN_ConfigParams *) &gMCAN0ConfigParams);

    /* Configure Bit timings. */
    DL_MCAN_setBitTime(
        MCAN0_INST, (DL_MCAN_BitTimingParams *) &gMCAN0BitTimes);

    /* Configure Standard ID filter element */
    DL_MCAN_addStdMsgIDFilter(
        MCAN0_INST, 0U, (DL_MCAN_StdMsgIDFilterElement *) &gMCAN0StdFiltelem);

    /* Set Extended ID Mask. */
    DL_MCAN_setExtIDAndMask(MCAN0_INST, MCAN0_INST_MCAN_EXT_ID_AND_MASK);

    /* Configure Message RAM Sections */
    DL_MCAN_msgRAMConfig(
        MCAN0_INST, (DL_MCAN_MsgRAMConfigParams *) &gMCAN0MsgRAMConfigParams);

    /* Loopback mode */

    /* Take MCAN out of the SW initialization mode */
    DL_MCAN_setOpMode(MCAN0_INST, DL_MCAN_OPERATION_MODE_NORMAL);

    while (DL_MCAN_OPERATION_MODE_NORMAL != DL_MCAN_getOpMode(MCAN0_INST))
        ;

    /* Enable MCAN mopdule Interrupts */
    DL_MCAN_enableIntr(MCAN0_INST, MCAN0_INST_MCAN_INTERRUPTS, 1U);

    DL_MCAN_selectIntrLine(
        MCAN0_INST, DL_MCAN_INTR_MASK_ALL, DL_MCAN_INTR_LINE_NUM_1);
    DL_MCAN_enableIntrLine(MCAN0_INST, DL_MCAN_INTR_LINE_NUM_1, 1U);

    /* Enable MSPM0 MCAN interrupt */
    DL_MCAN_clearInterruptStatus(MCAN0_INST, (DL_MCAN_MSP_INTERRUPT_LINE1));
    DL_MCAN_enableInterrupt(MCAN0_INST, (DL_MCAN_MSP_INTERRUPT_LINE1));
}
