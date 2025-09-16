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

#include <ti/driverlib/dl_i2s.h>

#ifdef __MSPM0_HAS_I2S__

/**
 *  @brief I2S APIs
 */

void DL_I2S_setClockConfig(I2S_Regs *i2s, const DL_I2S_ClockConfig *config)
{
    i2s->GPRCM.CLKCFG = I2S_CLKCFG_KEY_UNLOCK | config->clockSel;

    DL_Common_updateReg(&i2s->WCLKSRC, (uint32_t) config->wordBaudClockSource,
        I2S_WCLKSRC_WBCLKSRC_MASK);

    DL_Common_updateReg(
        &i2s->CLKCTL, (uint32_t) config->wclkPhase, I2S_CLKCTL_WCLKPHASE_MASK);

    i2s->WCLKDIV = config->wclkDivider;
    i2s->BCLKDIV = config->bclkDivider;
}

void DL_I2S_getClockConfig(const I2S_Regs *i2s, DL_I2S_ClockConfig *config)
{
    uint32_t clockSel = i2s->GPRCM.CLKCFG & I2S_CLKCFG_DAICLK_MASK;
    config->clockSel  = (DL_I2S_CLOCK_SOURCE)(clockSel);

    uint32_t wordBaudClockSource = i2s->WCLKSRC & I2S_WCLKSRC_WBCLKSRC_MASK;
    config->wordBaudClockSource =
        (DL_I2S_WORD_BAUD_CLOCK_SOURCE)(wordBaudClockSource);

    uint32_t wclkPhase = i2s->CLKCTL & I2S_CLKCTL_WCLKPHASE_MASK;
    config->wclkPhase  = (DL_I2S_WCLK_PHASE)(wclkPhase);

    config->wclkDivider = i2s->WCLKDIV;
    config->bclkDivider = i2s->BCLKDIV;
}

void DL_I2S_init(I2S_Regs *i2s, const DL_I2S_Config *config)
{
    DL_Common_updateReg(&i2s->WCLKSRC, (uint32_t) config->wclkInvert,
        I2S_WCLKSRC_WCLKINV_MASK);

    DL_Common_updateReg(&i2s->FMTCFG,
        ((uint32_t) config->emptySlotOutput | (uint32_t) config->dataDelay |
            (uint32_t) config->memoryAccessLength |
            (uint32_t) config->samplingEdge | (uint32_t) config->phase),
        (I2S_FMTCFG_EMPTYSLOTOUTPUT_MASK | I2S_FMTCFG_DATADLY_MASK |
            I2S_FMTCFG_MEMLEN32_MASK | I2S_FMTCFG_SMPLEDGE_MASK |
            I2S_FMTCFG_DUALPHASE_MASK));

    DL_I2S_setSampleWordLength(i2s, config->sampleWordLength);

    DL_Common_updateReg(&i2s->DIRCFG,
        ((uint32_t) config->dataPin0Direction) << I2S_DIRCFG_AD0_OFS,
        I2S_DIRCFG_AD0_MASK);
    i2s->WMASK0 = config->dataPin0ValidChannelMask;

#ifdef DEVICE_HAS_MULTIPLE_DATA_PIN
    DL_Common_updateReg(&i2s->DIRCFG,
        ((uint32_t) config->dataPin1Direction) << I2S_DIRCFG_AD1_OFS,
        I2S_DIRCFG_AD1_MASK);
    i2s->WMASK1 = config->dataPin1ValidChannelMask;
#endif

    if (config->mode == DL_I2S_MODE_CONTROLLER) {
        if (config->enableMCLK) {
            /* If optional MCLK generation is required, configure MCLK */
            DL_Common_updateReg(&i2s->CLKCTL,
                (I2S_CLKCTL_WBEN_EN | I2S_CLKCTL_MEN_EN),
                (I2S_CLKCTL_WBEN_MASK | I2S_CLKCTL_MEN_MASK));
        } else {
            /* Otherwise generate WCLK and BCLK only */
            DL_Common_updateReg(&i2s->CLKCTL,
                (I2S_CLKCTL_WBEN_EN | I2S_CLKCTL_MEN_DIS),
                (I2S_CLKCTL_WBEN_MASK | I2S_CLKCTL_MEN_MASK));
        }
    } else {
        /* In target mode, WCLK and BCLK are input signals */
        DL_Common_updateReg(&i2s->CLKCTL,
            I2S_CLKCTL_WBEN_DIS | I2S_CLKCTL_MEN_DIS,
            I2S_CLKCTL_WBEN_MASK | I2S_CLKCTL_MEN_MASK);
    }

    /* Divider can be set outside of controller mode */
    i2s->MCLKDIV = config->mclkDivider;
}

DL_I2S_MODE DL_I2S_getMode(const I2S_Regs *i2s)
{
    DL_I2S_MODE mode = DL_I2S_MODE_CONTROLLER;

    if (DL_I2S_isWBCLKGenerationEnabled(i2s)) {
        mode = DL_I2S_MODE_CONTROLLER;
    } else {
        mode = DL_I2S_MODE_TARGET;
    }

    return mode;
}

void DL_I2S_setMode(I2S_Regs *i2s, DL_I2S_MODE mode)
{
    if (mode == DL_I2S_MODE_CONTROLLER) {
        /* Controller mode means that I2S generates WCLK and BCLK */
        DL_I2S_enableWBCLKGeneration(i2s);
    } else {
        /* Target mode means that I2S consumes WCLK and BCLK */
        DL_I2S_disableWBCLKGeneration(i2s);
    }
}

void DL_I2S_transmitDataBlocking8(I2S_Regs *i2s, uint8_t data)
{
    while (DL_I2S_isTXFIFOFull(i2s)) {
        ;
    }

    DL_I2S_transmitData8(i2s, data);
}

void DL_I2S_transmitDataBlocking16(I2S_Regs *i2s, uint16_t data)
{
    while (DL_I2S_isTXFIFOFull(i2s)) {
        ;
    }

    DL_I2S_transmitData16(i2s, data);
}

void DL_I2S_transmitDataBlocking32(I2S_Regs *i2s, uint32_t data)
{
    while (DL_I2S_isTXFIFOFull(i2s)) {
        ;
    }

    DL_I2S_transmitData32(i2s, data);
}

uint8_t DL_I2S_receiveDataBlocking8(const I2S_Regs *i2s)
{
    while (DL_I2S_isRXFIFOEmpty(i2s)) {
        ;
    }

    return DL_I2S_receiveData8(i2s);
}

uint16_t DL_I2S_receiveDataBlocking16(const I2S_Regs *i2s)
{
    while (DL_I2S_isRXFIFOEmpty(i2s)) {
        ;
    }

    return DL_I2S_receiveData16(i2s);
}

uint32_t DL_I2S_receiveDataBlocking32(const I2S_Regs *i2s)
{
    while (DL_I2S_isRXFIFOEmpty(i2s)) {
        ;
    }

    return DL_I2S_receiveData32(i2s);
}

bool DL_I2S_transmitDataCheck8(I2S_Regs *i2s, uint8_t data)
{
    bool status;
    if (DL_I2S_isTXFIFOFull(i2s)) {
        status = false;
    } else {
        DL_I2S_transmitData8(i2s, data);
        status = true;
    }
    return status;
}

bool DL_I2S_transmitDataCheck16(I2S_Regs *i2s, uint16_t data)
{
    bool status;
    if (DL_I2S_isTXFIFOFull(i2s)) {
        status = false;
    } else {
        DL_I2S_transmitData16(i2s, data);
        status = true;
    }
    return status;
}

bool DL_I2S_transmitDataCheck32(I2S_Regs *i2s, uint32_t data)
{
    bool status;
    if (DL_I2S_isTXFIFOFull(i2s)) {
        status = false;
    } else {
        DL_I2S_transmitData32(i2s, data);
        status = true;
    }
    return status;
}

bool DL_I2S_receiveDataCheck8(const I2S_Regs *i2s, uint8_t *buffer)
{
    bool status;
    if (DL_I2S_isRXFIFOEmpty(i2s)) {
        status = false;
    } else {
        *buffer = DL_I2S_receiveData8(i2s);
        status  = true;
    }
    return status;
}

bool DL_I2S_receiveDataCheck16(const I2S_Regs *i2s, uint16_t *buffer)
{
    bool status;
    if (DL_I2S_isRXFIFOEmpty(i2s)) {
        status = false;
    } else {
        *buffer = DL_I2S_receiveData16(i2s);
        status  = true;
    }
    return status;
}

bool DL_I2S_receiveDataCheck32(const I2S_Regs *i2s, uint32_t *buffer)
{
    bool status;
    if (DL_I2S_isRXFIFOEmpty(i2s)) {
        status = false;
    } else {
        *buffer = DL_I2S_receiveData32(i2s);
        status  = true;
    }
    return status;
}

uint32_t DL_I2S_drainRXFIFO8(
    const I2S_Regs *i2s, uint8_t *buffer, uint32_t maxCount)
{
    uint32_t i;
    for (i = 0; i < maxCount; i++) {
        if (!DL_I2S_isRXFIFOEmpty(i2s)) {
            buffer[i] = DL_I2S_receiveData8(i2s);
        } else {
            break;
        }
    }

    return i;
}

uint32_t DL_I2S_drainRXFIFO16(
    const I2S_Regs *i2s, uint16_t *buffer, uint32_t maxCount)
{
    uint32_t i;
    for (i = 0; i < maxCount; i++) {
        if (!DL_I2S_isRXFIFOEmpty(i2s)) {
            buffer[i] = DL_I2S_receiveData16(i2s);
        } else {
            break;
        }
    }

    return i;
}

uint32_t DL_I2S_drainRXFIFO32(
    const I2S_Regs *i2s, uint32_t *buffer, uint32_t maxCount)
{
    uint32_t i;
    for (i = 0; i < maxCount; i++) {
        if (!DL_I2S_isRXFIFOEmpty(i2s)) {
            buffer[i] = DL_I2S_receiveData32(i2s);
        } else {
            break;
        }
    }

    return i;
}

uint32_t DL_I2S_fillTXFIFO8(
    I2S_Regs *i2s, const uint8_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++) {
        if (!DL_I2S_isTXFIFOFull(i2s)) {
            DL_I2S_transmitData8(i2s, buffer[i]);
        } else {
            break;
        }
    }

    return i;
}

uint32_t DL_I2S_fillTXFIFO16(
    I2S_Regs *i2s, const uint16_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++) {
        if (!DL_I2S_isTXFIFOFull(i2s)) {
            DL_I2S_transmitData16(i2s, buffer[i]);
        } else {
            break;
        }
    }

    return i;
}

uint32_t DL_I2S_fillTXFIFO32(
    I2S_Regs *i2s, const uint32_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++) {
        if (!DL_I2S_isTXFIFOFull(i2s)) {
            DL_I2S_transmitData32(i2s, buffer[i]);
        } else {
            break;
        }
    }

    return i;
}

void DL_I2S_clearTXFIFO(I2S_Regs *i2s)
{
    DL_Common_updateReg(
        &i2s->IFLS, I2S_IFLS_TXCLR_ENABLE, I2S_IFLS_TXCLR_MASK);

    while (!DL_I2S_isTXFIFOClearComplete(i2s)) {
        ;
    }

    DL_Common_updateReg(
        &i2s->IFLS, I2S_IFLS_TXCLR_DISABLE, I2S_IFLS_TXCLR_MASK);
}

void DL_I2S_clearRXFIFO(I2S_Regs *i2s)
{
    DL_Common_updateReg(
        &i2s->IFLS, I2S_IFLS_RXCLR_ENABLE, I2S_IFLS_RXCLR_MASK);

    while (!DL_I2S_isRXFIFOClearComplete(i2s)) {
        ;
    }

    DL_Common_updateReg(
        &i2s->IFLS, I2S_IFLS_RXCLR_DISABLE, I2S_IFLS_RXCLR_MASK);
}

#endif /* __MSPM0_HAS_I2S__ */
