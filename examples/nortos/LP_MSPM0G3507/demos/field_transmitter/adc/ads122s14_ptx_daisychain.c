/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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

#include "adc/ads122s14_ptx_daisychain.h"
#include "main.h"
#include "system/flash.h"
#include "system/uart.h"
#include "ti_msp_dl_config.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static enum status_enum help_cmd(char *cmd);
static enum status_enum get_cmd(char *cmd);
static enum status_enum stream_cmd(char *cmd);
static enum status_enum rreg_cmd(char *cmd);
static enum status_enum wreg_cmd(char *cmd);
static enum status_enum bridgetimer_cmd(char *cmd);

static void readRegistersDaisyChain(uint8_t address, uint8_t *data);
static void writeRegisterDaisyChain(uint8_t addressADC1, uint8_t dataADC1,
    uint8_t addressADC2, uint8_t dataADC2);
static void stopADCconversions(void);
static void startADCconversions(void);
static void init_ADCs(void);

struct command {
    char *name;
    char *help;
    enum status_enum (*function)(char *cmd);
};

static struct command commands[] = {
    {"help", "show this message", help_cmd},
    {"get", "return one raw reading", get_cmd},
    {"stream", "starts/stops streaming raw readings", stream_cmd},
    {"rreg", "read register on both ADCs - rreg 0xreg", rreg_cmd},
    {"wreg", "write register to one ADC - wreg [0|1] 0xreg 0xvalue", wreg_cmd},
    {"start", "start conversion",
        (enum status_enum(*)(char *)) startADCconversions},
    {"stop", "stop conversion",
        (enum status_enum(*)(char *)) stopADCconversions},
    {"reset", "rewrite default adc config",
        (enum status_enum(*)(char *)) init_ADCs},
    {"bridgetimer", "read/write bridge control timer",
        (enum status_enum(*)(char *)) bridgetimer_cmd},
};

struct ads122s14_config_struct {
    uint8_t reg;
    uint8_t value;
};

/*
 * default configuration ADC1 will be connected to bridge
 * ADC2 will be RTD input
 */
volatile struct ads122s14_config_struct gADSDefault[2][9] = {
    {
        /* ADC 1 */
        {MUX_CFG_ADDRESS,
            MUX_CFG_AINP_AIN2 | MUX_CFG_AINN_AIN3},  // set mux to AIN0-AIN1
        {REFERENCE_CFG_ADDRESS,
            REFERENCE_CFG_REFP_BUF_EN_ENABLED |
                REFERENCE_CFG_REFN_BUF_EN_ENABLED |
                REFERENCE_CFG_REF_SEL_EXTERNAL},  // set external reference and enable reference buffers
        {GAIN_CFG_ADDRESS, GAIN_CFG_GAIN_128},  // set gain to 128
        {IDAC_MUX_CFG_ADDRESS,
            IDAC_MUX_CFG_IUNIT_10UA |
                IDAC_MUX_CFG_I1MUX_AIN2},  // set IDAC1 to AIN2 and set units to 10uA
        {DATA_RATE_CFG_ADDRESS, DATA_RATE_CFG_FLTR_OSR_20_SPS},  // 20 sps
        {DEVICE_CFG_ADDRESS, DEVICE_CFG_CONV_MODE_SINGLE},       // single shot
        {DIGITAL_CFG_ADDRESS, DIGITAL_CFG_STATUS_EN_ENABLED},
        {IGNORE_ENTRY,
            IGNORE_ENTRY},  // not used entry, just make sure both dimensions of entry have same length
    },
    {
        /* ADC 2*/
        {MUX_CFG_ADDRESS,
            MUX_CFG_AINP_AIN2 | MUX_CFG_AINN_AIN3},  // set mux to AIN2-AIN3
        {GAIN_CFG_ADDRESS, GAIN_CFG_GAIN_1},         // set gain to 1
        {REFERENCE_CFG_ADDRESS,
            REFERENCE_CFG_REFP_BUF_EN_ENABLED |
                REFERENCE_CFG_REFN_BUF_EN_ENABLED |
                REFERENCE_CFG_REF_SEL_EXTERNAL},  // set external reference and enable reference buffers
        {IDAC_MUX_CFG_ADDRESS,
            IDAC_MUX_CFG_IUNIT_10UA | IDAC_MUX_CFG_I2MUX_AIN7 |
                IDAC_MUX_CFG_I1MUX_AIN6},  // set IDAC1 to AIN6 and sets unit to 10uA
        {IDAC_MAG_CFG_ADDRESS,
            IDAC_MAG_CFG_I1MAG_20X},  // enable IDAC1 and set magnitude to 10 x 10uA (0.1mA)
        {DATA_RATE_CFG_ADDRESS, DATA_RATE_CFG_FLTR_OSR_20_SPS},  // 20 sps
        {DEVICE_CFG_ADDRESS, DEVICE_CFG_CONV_MODE_SINGLE},       // single shot
        {DIGITAL_CFG_ADDRESS, DIGITAL_CFG_STATUS_EN_ENABLED},
    },
};

struct ads122s14_ptx_chain_config_struct {
    uint8_t config_length;
    uint8_t config_chksum;
    uint32_t bridge_time;
};
static struct ads122s14_ptx_chain_config_struct *gADSConfigMem;

enum spi_state_enum { idle, busy };
static volatile enum spi_state_enum gSPIState = idle;
static volatile uint8_t gSPIRxBuf[12];
static volatile uint8_t gSPITxBuf[12];

static int32_t gRawReading[2];
static bool gStreaming           = false;
static uint32_t gADCTimeoutTimer = 0;
static bool ADCRunning           = false;

void SPI_0_INST_IRQHandler(void)
{  // todo: this will need to be moved somewhere and get configurable
    // when multiple trx cards are available using spi irq
    static char dummy;
    switch (DL_SPI_getPendingInterrupt(SPI_0_INST)) {
        case DL_SPI_IIDX_DMA_DONE_RX:
            gSPIState = idle;
            DL_GPIO_setPins(GPIO_ADC_PORT, GPIO_ADC_ADC_CS_PIN);
            break;

        default:
            break;
    }
}

/**
 * @brief Prints help information about available commands.
 *
 * @param [in] cmd Pointer to the input command string (currently not used)
 * @return Information if completed successfully
 * @retval STATUS_OK if the function execution was successful
 */
static enum status_enum help_cmd(char *cmd)
{
    int i;
    uart_printf("all command must start with out \r\n");
    for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        uart_printf("%-10s - 0x%x - %s\r\n", commands[i].name,
            commands[i].function, commands[i].help);
    }
    return STATUS_OK;
}

/**
 * @brief     Prints the current ADC raw reading value as a hexadecimal string.
 *
 * @details  This function simply prints the contents of `_raw_reading` to the UART console and returns immediately.
 *
 * @param[in] cmd        Pointer to an unused command string (this parameter is not used by this function)
 *
 * @returns   STATUS_OK always, indicating that the function executed successfully
 */
static enum status_enum get_cmd(char *cmd)
{
    uart_printf("0x%x 0x%x\r\n", gRawReading[0], gRawReading[1]);
    return STATUS_OK;
}

/**
 * @brief Toggles gStreaming state.
 *
 * This function flips the `gStreaming` flag when called, turning it on or off.
 * The current value of `gStreaming` is used to stream the ADC readings to UART as well or not
 *
 * @param[in] cmd Unused command parameter; kept for consistency with other functions.
 *
 * @return STATUS_OK regardless of the new gStreaming state.
 */
static enum status_enum stream_cmd(char *cmd)
{
    if (gStreaming)
        gStreaming = false;
    else
        gStreaming = true;
    return STATUS_OK;
}

/**
 * @brief Reads and prints register values from a daisy-chained ADS122S14.
 *
 * This function parses the input command string, extracts the target register
 * address in hexadecimal format (0xXX), reads the corresponding value
 * from the daisy-chain of ADCs, and then prints out the register value to the UART.
 *
 * If any error occurs during parsing, an error status is returned.
 *
 * @param[in] cmd The command string containing the register address as "rreg 0xXX".
 *
 * @return status if command was parsed successfully
 *
 * @retval STATUS_OK on success
 * @retval STATUS_CMDERROR on syntax errors
 */
static enum status_enum rreg_cmd(char *cmd)
{
    uint8_t value[2];
    char *subcmd = strtok(cmd, " ");
    int reg;
    int ret;
    if (!subcmd)
        return STATUS_CMDERROR;
    else {
        ret = sscanf(cmd, "0x%x", &reg);
        if (ret != 1) return STATUS_CMDERROR;
        readRegistersDaisyChain(reg, value);
        uart_printf("Reg 0x%x, 0x%x 0x%x\r\n", reg, value[0], value[1]);
        return STATUS_OK;
    }
    return STATUS_CMDERROR; /* should never get here */
}

/**
 * @brief Writes values to specified registers in a daisy-chained ADS122S14 ADC.
 *
 * This function parses the input command string, extracts the target ADC
 * addresses and value in hexadecimal format, and then writes out
 * these values to the corresponding registers using a low-level write API.
 *
 * If any error occurs during parsing or execution, an error status is returned.
 *
 * @param[in] cmd The command string containing register addresses and value(s).
 *
 * @return Status indicating success or failure:
 *
 * @retval STATUS_OK on successful execution.
 * @retval STATUS_CMDERROR if there was a problem with parsering the input command.
 */
static enum status_enum wreg_cmd(char *cmd)
{
    int reg, val, adc;

    if (sscanf(cmd, "%d 0x%x 0x%x", &adc, &reg, &val) ==
        3) { /* need exactly 3 arguments */
        if (adc == 0) {
            writeRegisterDaisyChain(reg, val, IGNORE_ADC, 0);
        } else if (adc == 1) {
            writeRegisterDaisyChain(IGNORE_ADC, 0, reg, val);
        } else if (adc == 3) {
            writeRegisterDaisyChain(reg, val, reg, val);
        } else
            return STATUS_CMDERROR;
        return STATUS_OK;
    } else
        return STATUS_CMDERROR;
}

/**
 * @brief Handles command line input to read/update bridge timer
 *
 * @param [in] cmd Pointer to the input command string
 *
 * @return Information if completed successfully
 * @retval STATUS_OK if the function execution was successful
 * @retval STATUS_CMDERROR otherwise
 */
static enum status_enum bridgetimer_cmd(char *cmd)
{
    enum status_enum ret = STATUS_CMDERROR;
    int cnt;
    uint32_t timer;
    ;
    char *subcmd = strtok(cmd, " ");
    if (!subcmd) {
        uart_printf("%d\r\n", gADSConfigMem->bridge_time);
        ret = STATUS_OK;
    } else {
        sscanf(subcmd, "%d", &timer);
        gADSConfigMem->bridge_time = timer;
        gADSConfigMem->config_chksum =
            crc8((void *) (gADSConfigMem) + 2, gADSConfigMem->config_length);
        ret = STATUS_OK;
    }
    return ret;
}

/**
 * @brief Start a non-blocking write-read SPI transaction.
 *
 * This function initiates an SPI transfer between the internal SPI peripheral
 * and the external chip through the DMA controller. The data to be written is
 * copied from `buf` into the SPI transmit buffer, while the receive data goes
 * directly into the global `gSPIRxBuf`.
 *
 * @param[in] buf The data buffer containing data to be transmitted over SPI.
 * @param[in] len The length of the data in bytes.
 */
static void rw_spi_non_blocking(const char *buf, int len)
{
    gSPIState = busy;
    memcpy((void *) gSPITxBuf, buf, len);

    DL_DMA_setSrcAddr(DMA, DMA_CH2_CHAN_ID, (uint32_t) gSPITxBuf);
    DL_DMA_setDestAddr(DMA, DMA_CH2_CHAN_ID, (uint32_t)(&SPI_0_INST->TXDATA));
    DL_DMA_setTransferSize(DMA, DMA_CH2_CHAN_ID, len);

    DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)(&SPI_0_INST->RXDATA));
    DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) gSPIRxBuf);
    DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, len);
    DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);

    DL_GPIO_clearPins(GPIO_ADC_PORT, GPIO_ADC_ADC_CS_PIN);
    DL_DMA_enableChannel(DMA, DMA_CH2_CHAN_ID);
}

/**
 * @brief Write values to specified registers in a daisy-chained ADC system.
 *
 * This function writes values to two ADS122S14 chips connected in series,
 * using SPI protocol. The addresses and data pairs for each
 * chip are packed into a single 10-byte buffer, which is then sent through
 * the SPI API using spi_non_blocking().
 *
 * @param addressADC1 The register address on the first ADS122S14 chip.
 * @param dataADC1    The value to write to the corresponding register on the first
 *                     ADS122S14 chip.
 * @param addressADC2 The register address on the second ADS122S14 chip.
 * @param dataADC2    The value to write to the corresponding register on the second
 *                     ADS122S14 chip.
 *
 */
static void writeRegisterDaisyChain(uint8_t addressADC1, uint8_t dataADC1,
    uint8_t addressADC2, uint8_t dataADC2)
{
    static uint8_t spibuf[10];

    /* wait until previous send is done */
    while (gSPIState != idle) asm("wfi");

    memset(spibuf, 0, sizeof(spibuf));
    /* ADS122s1x write format is [0x80 | address (byte 1), Data (byte 2)] */
    /* first bytes go to ADC2 */
    spibuf[3] = 0x80 + addressADC2;
    spibuf[4] = dataADC2;
    /* last to ADC1 */
    spibuf[8] = 0x80 + addressADC1;
    spibuf[9] = dataADC1;

    rw_spi_non_blocking((const char *) spibuf, sizeof(spibuf));
}

/**
 * @brief Initialize ADCs with preconfigured values.
 *
 * This function iterates over a list of default register settings for each
 * ADS122S14 chip (loaded from `gADSDefault` array), and writes these to the
 * corresponding registers using writeRegisterDaisyChain()..
 *
 * @note Entries with IGNORE_ENTRY as register address in either list will be
 *       skipped during initialization.
 */
static void init_ADCs(void)
{
    int i;

    for (i = 0; i < sizeof(gADSDefault) / 2 / sizeof(gADSDefault[0][0]); i++) {
#ifdef DEBUG
        uart_printf("ADC1 0x%x -> 0x%x\r\n", gADSDefault[0][i].reg,
            gADSDefault[0][i].value);
        uart_printf("ADC2 0x%x -> 0x%x\r\n", gADSDefault[1][i].reg,
            gADSDefault[1][i].value);
#endif
        if (gADSDefault[0][i].reg != IGNORE_ENTRY)
            writeRegisterDaisyChain(gADSDefault[0][i].reg,
                gADSDefault[0][i].value, IGNORE_ADC, 0x00);
        if (gADSDefault[1][i].reg != IGNORE_ENTRY)
            writeRegisterDaisyChain(IGNORE_ADC, 0x00, gADSDefault[1][i].reg,
                gADSDefault[1][i].value);
    }
}

/**
 * @brief Start ADC conversions.
 *
 * This function writes a start signal to the conversion control register of both
 * ADS122S14 chips connected in daisy-chain configuration, triggering them to begin sampling.
 */
static void startADCconversions(void)
{
    writeRegisterDaisyChain(CONVERSION_CTRL_ADDRESS,
        CONVERSION_CTRL_START_START, CONVERSION_CTRL_ADDRESS,
        CONVERSION_CTRL_START_START);
    ADCRunning = true;
}

/**
 * @brief Stop ADC conversions.
 *
 * This function writes a stop signal to the conversion control register of both
 * ADS122S14 chips connected in daisy-chain configuration, halting any ongoing sampling.
 */
static void stopADCconversions(void)
{
    writeRegisterDaisyChain(CONVERSION_CTRL_ADDRESS, CONVERSION_CTRL_STOP_STOP,
        CONVERSION_CTRL_ADDRESS, CONVERSION_CTRL_STOP_STOP);
    ADCRunning = false;
}

/**
 * @brief Reads ADC status and conversion results from both ADCs.
 *
 * @param[out] res pointer to an array of 32-bit integers storing the conversion results
 * @param[out] status pointer to an array of 8-bit unsigned integers storing the ADC status
 */
static void readDataDaisyChain(int32_t *res, uint8_t *status)
{
    static uint8_t spibuf[10];
    /* wait until previous send is done */
    while (gSPIState != idle) asm("wfi");
    memset(spibuf, 0, sizeof(spibuf));

    /* Read data command is a 1 frame operation
    * DIN should be set to 0x000000 while clocking 24 SCLK bits
    * FYI, max data rate is 128KSPS, which converts to a 7.8us delay between
    * samples in continuous mode. */
    rw_spi_non_blocking((const char *) spibuf, sizeof(spibuf));

    /* wait until data trx is done */
    while (gSPIState != idle) asm("wfi");

    res[0] =
        (gSPIRxBuf[2] << 24 | gSPIRxBuf[3] << 16 | gSPIRxBuf[4] << 8) >> 8;
    res[1] =
        (gSPIRxBuf[7] << 24 | gSPIRxBuf[8] << 16 | gSPIRxBuf[9] << 8) >> 8;

    status[0] = gSPIRxBuf[0];
    status[1] = gSPIRxBuf[5];
}

/**
 * @brief Reads a register from both ADCs connected in a daisy-chain configuration.
 *
 * The register addresses are written to both SPI peripherals and read back using two separate frames.
 * This allows access to registers on adjacent ADC units in the chain.
 *
 * @param[in] address The 7-bit address of the register to be read
 * @param[out] data pointer to an array of 8-bit unsigned integers where the register values will be stored.
 */
static void readRegistersDaisyChain(uint8_t address, uint8_t *data)
{
    static uint8_t spibuf[10];
    /* wait until previous send is done */
    while (gSPIState != idle) asm("wfi");
    memset(spibuf, 0, sizeof(spibuf));
    /* first bytes go to ADC2 */
    spibuf[3] = (0x40 + address);
    /* last to ADC1 */
    spibuf[8] = (0x40 + address);

    /* wait until data trx is done */
    rw_spi_non_blocking((const char *) spibuf, sizeof(spibuf));
    while (gSPIState != idle) asm("wfi");

    /* transfer new frame and wait until completed */
    memset(spibuf, 0, sizeof(spibuf));
    rw_spi_non_blocking((const char *) spibuf, sizeof(spibuf));
    while (gSPIState != idle) asm("wfi");

    /* copy register values */
    data[0] = gSPIRxBuf[2];
    data[1] = gSPIRxBuf[7];
    return;
}

/**
 * @brief Returns true if both ADCs are signaling DRDY (Data Ready), indicating they have completed a conversion.
 *
 * This function reads the status registers from both ADC units and checks that the DRDY bit is set in each register.
 *
 * @return True if both ADCs are ready with new data, false otherwise.
 */
static bool get_drdy(void)
{
    uint8_t reg[2];
    readRegistersDaisyChain(STATUS_MSB_ADDRESS, reg);
    return reg[0] & STATUS_MSB_DRDY_MASK & reg[1] & STATUS_MSB_DRDY_MASK;
}

/**
 * @brief Cyclic function not used.
 *
 * @return STATUS_OK
 */
enum status_enum ads122s14_ptx_daisychain_cyclic(void)
{
    static int bridge_timeer = 0;

    if (bridge_timeer++ >= gADSConfigMem->bridge_time) {
        /* turn on bridge power */
        DL_GPIO_setPins(GPIO_ADC_PORT, GPIO_ADC_ADC_RIDGE_SWITCH_CONTROL_PIN);
        startADCconversions();
        bridge_timeer = 0;
    }

    /* this is a work around, as it can happen when starting up from the loop power, that the ADC get initialized to early and does not start sampling */
    if (gADCTimeoutTimer++ >
        500) { /* if not getting one sample in 0.5s something is odd, restart ADC. */
        if (ADCRunning) {
            init_ADCs();
            startADCconversions();
            gADSConfigMem->bridge_time   = 200; /* 5 samples per second */
            gADSConfigMem->config_chksum = crc8(
                (void *) (gADSConfigMem) + 2, gADSConfigMem->config_length);
            gADCTimeoutTimer = 0;
        }
    }
    return STATUS_OK;
}

/**
 * @brief init function of ADS122S14 driver. Inits SPI, DMA and the ADC
 *
 * @param[in] config pointer where config can be stored
 * @return STATUS_OK if everything went well
 */
enum status_enum ads122s14_ptx_daisychain_init(void *config)
{
    enum status_enum ret = STATUS_OK;

    static const DL_SPI_Config SPI_0_config = {
        .mode          = DL_SPI_MODE_CONTROLLER,
        .frameFormat   = DL_SPI_FRAME_FORMAT_MOTO3_POL0_PHA1,
        .parity        = DL_SPI_PARITY_NONE,
        .dataSize      = DL_SPI_DATA_SIZE_8,
        .bitOrder      = DL_SPI_BIT_ORDER_MSB_FIRST,
        .chipSelectPin = DL_SPI_CHIP_SELECT_0,
    };

    static const DL_SPI_ClockConfig SPI_0_clockConfig = {
        .clockSel    = DL_SPI_CLOCK_BUSCLK,
        .divideRatio = DL_SPI_CLOCK_DIVIDE_RATIO_1};

    static const DL_DMA_Config DMA_CH0Config = {
        .transferMode  = DL_DMA_SINGLE_TRANSFER_MODE,
        .extendedMode  = DL_DMA_NORMAL_MODE,
        .destIncrement = DL_DMA_ADDR_UNCHANGED,
        .srcIncrement  = DL_DMA_ADDR_INCREMENT,
        .destWidth     = DL_DMA_WIDTH_BYTE,
        .srcWidth      = DL_DMA_WIDTH_BYTE,
        .trigger       = DMA_SPI0_TX_TRIG,
        .triggerType   = DL_DMA_TRIGGER_TYPE_EXTERNAL,
    };

    static const DL_DMA_Config DMA_CH1Config = {
        .transferMode  = DL_DMA_SINGLE_TRANSFER_MODE,
        .extendedMode  = DL_DMA_NORMAL_MODE,
        .destIncrement = DL_DMA_ADDR_INCREMENT,
        .srcIncrement  = DL_DMA_ADDR_UNCHANGED,
        .destWidth     = DL_DMA_WIDTH_BYTE,
        .srcWidth      = DL_DMA_WIDTH_BYTE,
        .trigger       = DMA_SPI0_RX_TRIG,
        .triggerType   = DL_DMA_TRIGGER_TYPE_EXTERNAL,
    };

    /* turn off bridge power */
    DL_GPIO_clearPins(GPIO_ADC_PORT, GPIO_ADC_ADC_RIDGE_SWITCH_CONTROL_PIN);

    uart_printf("Using ADS122S14 in daisy chain for pressure sensing \r\n");
    DL_GPIO_setPins(GPIO_ADC_PORT, GPIO_ADC_ADC_CS_PIN);

    __disable_irq();
    DL_SPI_disable(SPI_0_INST);
    DL_SPI_setClockConfig(
        SPI_0_INST, (DL_SPI_ClockConfig *) &SPI_0_clockConfig);
    DL_SPI_init(SPI_0_INST, (DL_SPI_Config *) &SPI_0_config);
    DL_SPI_setBitRateSerialClockDivider(SPI_0_INST, 3);
    DL_SPI_setFIFOThreshold(SPI_0_INST, DL_SPI_RX_FIFO_LEVEL_ONE_FRAME,
        DL_SPI_TX_FIFO_LEVEL_ONE_FRAME);
    NVIC_ClearPendingIRQ(SPI_0_INST_INT_IRQN);
    DL_SPI_enableDMATransmitEvent(SPI_0_INST);
    DL_SPI_enableDMAReceiveEvent(SPI_0_INST, DL_SPI_DMA_INTERRUPT_RX);
    DL_SPI_enableInterrupt(SPI_0_INST, DL_SPI_INTERRUPT_DMA_DONE_RX);
    DL_SPI_disableInterrupt(SPI_0_INST, DL_SPI_INTERRUPT_RX);

    DL_SPI_enable(SPI_0_INST);
    NVIC_EnableIRQ(SPI_0_INST_INT_IRQN);
    __enable_irq();

    DL_DMA_initChannel(DMA, DMA_CH2_CHAN_ID, (DL_DMA_Config *) &DMA_CH0Config);
    DL_DMA_initChannel(DMA, DMA_CH1_CHAN_ID, (DL_DMA_Config *) &DMA_CH1Config);

    gADSConfigMem = config;
    gADSConfigMem->config_length =
        sizeof(struct ads122s14_ptx_chain_config_struct) - 2;
    if (crc8(config + 2, gADSConfigMem->config_length) !=
        gADSConfigMem->config_chksum) {
        ERR_LOG("ADS122S14 config reset");
        gADSConfigMem->bridge_time = 500; /* 2 samples per second */
        gADSConfigMem->config_chksum =
            crc8(config + 2, gADSConfigMem->config_length);
        ret = STATUS_CFG_CHANGED;
    }

    init_ADCs();
    startADCconversions();

    gADSConfigMem->config_chksum =
        crc8(config + 2, gADSConfigMem->config_length);
    return ret;
}

/**
 * @brief called by main loop to poll for new ADC reading. Needs to provide new reading if available
 *
 * @param[out] adc_raw pointer to store readings to
 * @return STATUS_OK if new readings where available; STATUS_NOCHANGE if no new readings are available
 */
enum status_enum ads122s14_ptx_daisychain_get_reading(int32_t *adc_raw)
{
    uint8_t status[2];
    readDataDaisyChain(adc_raw, status);
    if (status[0] & status[1] &
        1) { /* check status bits, if ADCs have finished conversion */
        gADCTimeoutTimer = 0;
        //startADCconversions();
        /* turn off bridge power */
        DL_GPIO_clearPins(
            GPIO_ADC_PORT, GPIO_ADC_ADC_RIDGE_SWITCH_CONTROL_PIN);
        memcpy(gRawReading, adc_raw, sizeof(gRawReading));
        if (gStreaming)
            uart_printf("0x%x 0x%x\r\n", gRawReading[0], gRawReading[1]);
        return STATUS_OK;
    }
    return STATUS_NOCHANGE;
}

/**
 * @brief handles UART commands
 *
 * @param[in] cmd pointer of UART input
 * @return return value of command function, STATUS_CMDNOTFOUND if command is not available, STATUS_ERR in case of generic error
 */
enum status_enum ads122s14_ptx_daisychain_cmd(char *cmd)
{
    int i;
    char *subcmd = strtok(cmd, " ");
    bool found   = false;
    for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (!strcmp(commands[i].name, subcmd)) {
            /* command found, call it */
            return (commands[i].function(cmd + strlen(commands[i].name) + 1));
            found = true;
            break;
        }
    }
    if (found == false) { /* unknown command */
        uart_printf("command not found - try out help\r\n");
        return STATUS_CMDNOTFOUND;
    }
    return STATUS_ERR; /* should never get here */
}
