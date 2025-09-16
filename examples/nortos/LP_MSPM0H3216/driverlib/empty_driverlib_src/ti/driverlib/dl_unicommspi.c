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

#include <ti/driverlib/dl_unicommspi.h>

#ifdef __MCU_HAS_UNICOMMSPI__

void DL_SPI_init(UNICOMM_Inst_Regs *unicomm, DL_SPI_Config *config)
{
    DL_Common_updateReg(&unicomm->spi->CTL0,
        (uint32_t) config->chipSelectPin | (uint32_t) config->frameFormat |
            (uint32_t) config->dataSize,
        UNICOMMSPI_CTL0_CSSEL_MASK | UNICOMMSPI_CTL0_FRF_MASK |
            UNICOMMSPI_CTL0_SPO_MASK | UNICOMMSPI_CTL0_SPH_MASK |
            UNICOMMSPI_CTL0_DSS_MASK);

    DL_Common_updateReg(&unicomm->spi->CTL1,
        (uint32_t) config->parity | (uint32_t) config->bitOrder |
            (uint32_t) config->mode,
        UNICOMMSPI_CTL1_PES_MASK | UNICOMMSPI_CTL1_PREN_MASK |
            UNICOMMSPI_CTL1_PTEN_MASK | UNICOMMSPI_CTL1_MSB_MASK |
            UNICOMMSPI_CTL1_CP_MASK);
}

void DL_SPI_setClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_ClockConfig *config)
{
    unicomm->spi->CLKSEL = (uint32_t) config->clockSel;

    unicomm->spi->CLKDIV = (uint32_t) config->divideRatio;
}

void DL_SPI_getClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_ClockConfig *config)
{
    config->clockSel = (DL_SPI_CLOCK) unicomm->spi->CLKSEL;

    config->divideRatio = (DL_SPI_CLOCK_DIVIDE_RATIO) unicomm->spi->CLKDIV;
}

uint8_t DL_SPI_receiveDataBlocking8(UNICOMM_Inst_Regs *unicomm)
{
    while (DL_SPI_isRXFIFOEmpty(unicomm)) {
    };

    return DL_SPI_receiveData8(unicomm);
}

uint16_t DL_SPI_receiveDataBlocking16(UNICOMM_Inst_Regs *unicomm)
{
    while (DL_SPI_isRXFIFOEmpty(unicomm)) {
    };
    return DL_SPI_receiveData16(unicomm);
}

uint32_t DL_SPI_receiveDataBlocking32(UNICOMM_Inst_Regs *unicomm)
{
    while (DL_SPI_isRXFIFOEmpty(unicomm)) {
    };
    return DL_SPI_receiveData32(unicomm);
}

void DL_SPI_transmitDataBlocking8(UNICOMM_Inst_Regs *unicomm, uint8_t data)
{
    while (DL_SPI_isTXFIFOFull(unicomm)) {
    };
    DL_SPI_transmitData8(unicomm, data);
    while (DL_SPI_isBusy(unicomm)) {
    };
}

void DL_SPI_transmitDataBlocking16(UNICOMM_Inst_Regs *unicomm, uint16_t data)
{
    while (DL_SPI_isTXFIFOFull(unicomm)) {
    };
    DL_SPI_transmitData16(unicomm, data);
    while (DL_SPI_isBusy(unicomm)) {
    };
}

void DL_SPI_transmitDataBlocking32(UNICOMM_Inst_Regs *unicomm, uint32_t data)
{
    while (DL_SPI_isTXFIFOFull(unicomm)) {
    };
    DL_SPI_transmitData32(unicomm, data);
    while (DL_SPI_isBusy(unicomm)) {
    };
}

bool DL_SPI_receiveDataCheck8(UNICOMM_Inst_Regs *unicomm, uint8_t *buffer)
{
    bool status;
    if (DL_SPI_isRXFIFOEmpty(unicomm)) {
        status = false;
    } else {
        *buffer = DL_SPI_receiveData8(unicomm);
        status  = true;
    }

    return status;
}

bool DL_SPI_receiveDataCheck16(UNICOMM_Inst_Regs *unicomm, uint16_t *buffer)
{
    bool status;
    if (DL_SPI_isRXFIFOEmpty(unicomm)) {
        status = false;
    } else {
        *buffer = DL_SPI_receiveData16(unicomm);
        status  = true;
    }

    return status;
}

bool DL_SPI_receiveDataCheck32(UNICOMM_Inst_Regs *unicomm, uint32_t *buffer)
{
    bool status;
    if (DL_SPI_isRXFIFOEmpty(unicomm)) {
        status = false;
    } else {
        *buffer = DL_SPI_receiveData32(unicomm);
        status  = true;
    }

    return status;
}

bool DL_SPI_transmitDataCheck8(UNICOMM_Inst_Regs *unicomm, uint8_t data)
{
    bool status;
    if (DL_SPI_isTXFIFOFull(unicomm)) {
        status = false;
    } else {
        DL_SPI_transmitData8(unicomm, data);
        status = true;
    }

    return status;
}

bool DL_SPI_transmitDataCheck16(UNICOMM_Inst_Regs *unicomm, uint16_t data)
{
    bool status;
    if (DL_SPI_isTXFIFOFull(unicomm)) {
        status = false;
    } else {
        DL_SPI_transmitData16(unicomm, data);
        status = true;
    }

    return status;
}

bool DL_SPI_transmitDataCheck32(UNICOMM_Inst_Regs *unicomm, uint32_t data)
{
    bool status;
    if (DL_SPI_isTXFIFOFull(unicomm)) {
        status = false;
    } else {
        DL_SPI_transmitData32(unicomm, data);
        status = true;
    }

    return status;
}

uint32_t DL_SPI_drainRXFIFO8(
    UNICOMM_Inst_Regs *unicomm, uint8_t *buffer, uint32_t maxCount)
{
    uint32_t i;
    for (i = 0; i < maxCount; i++) {
        if (!DL_SPI_isRXFIFOEmpty(unicomm)) {
            buffer[i] = DL_SPI_receiveData8(unicomm);
        } else {
            break;
        }
    }

    return i;
}

uint32_t DL_SPI_drainRXFIFO16(
    UNICOMM_Inst_Regs *unicomm, uint16_t *buffer, uint32_t maxCount)
{
    uint32_t i;
    for (i = 0; i < maxCount; i++) {
        if (!DL_SPI_isRXFIFOEmpty(unicomm)) {
            buffer[i] = DL_SPI_receiveData16(unicomm);
        } else {
            break;
        }
    }

    return i;
}

uint32_t DL_SPI_drainRXFIFO32(
    UNICOMM_Inst_Regs *unicomm, uint32_t *buffer, uint32_t maxCount)
{
    uint32_t i;
    for (i = 0; i < maxCount; i++) {
        if (!DL_SPI_isRXFIFOEmpty(unicomm)) {
            buffer[i] = DL_SPI_receiveData32(unicomm);
        } else {
            break;
        }
    }

    return i;
}

uint32_t DL_SPI_fillTXFIFO8(
    UNICOMM_Inst_Regs *unicomm, uint8_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++) {
        if (!DL_SPI_isTXFIFOFull(unicomm)) {
            DL_SPI_transmitData8(unicomm, buffer[i]);
        } else {
            break;
        }
    }

    return i;
}

uint32_t DL_SPI_fillTXFIFO16(
    UNICOMM_Inst_Regs *unicomm, uint16_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++) {
        if (!DL_SPI_isTXFIFOFull(unicomm)) {
            DL_SPI_transmitData16(unicomm, buffer[i]);
        } else {
            break;
        }
    }

    return i;
}

bool DL_SPI_saveConfiguration(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_backupConfig *ptr)
{
    bool stateSaved = !ptr->backupRdy;
    if (stateSaved) {
        ptr->controlWord0                 = unicomm->spi->CTL0;
        ptr->controlWord1                 = unicomm->spi->CTL1;
        ptr->clockControl                 = unicomm->spi->CLKCTL;
        ptr->clockSel                     = unicomm->spi->CLKSEL;
        ptr->divideRatio                  = unicomm->spi->CLKDIV;
        ptr->interruptFifoLevelSelectWord = unicomm->spi->IFLS;
        ptr->interruptMask0               = unicomm->spi->CPU_INT.IMASK;
        ptr->interruptMask1               = unicomm->spi->DMA_TRIG_RX.IMASK;
        ptr->interruptMask2               = unicomm->spi->DMA_TRIG_TX.IMASK;
        ptr->backupRdy                    = true;
    }
    return stateSaved;
}

bool DL_SPI_restoreConfiguration(
    UNICOMM_Inst_Regs *unicomm, DL_SPI_backupConfig *ptr)
{
    bool stateRestored = ptr->backupRdy;
    if (stateRestored) {
        /* Set CTL1.ENABLE=0 during initialization */
        unicomm->spi->CTL1 =
            ptr->controlWord1 & ~(UNICOMMSPI_CTL1_ENABLE_MASK);
        unicomm->spi->CTL0              = ptr->controlWord0;
        unicomm->spi->CLKCTL            = ptr->clockControl;
        unicomm->spi->CLKSEL            = ptr->clockSel;
        unicomm->spi->CLKDIV            = ptr->divideRatio;
        unicomm->spi->IFLS              = ptr->interruptFifoLevelSelectWord;
        unicomm->spi->CPU_INT.IMASK     = ptr->interruptMask0;
        unicomm->spi->DMA_TRIG_RX.IMASK = ptr->interruptMask1;
        unicomm->spi->DMA_TRIG_TX.IMASK = ptr->interruptMask2;

        /* Re-enable SPI if it was originally enabled */
        if ((ptr->controlWord1 & UNICOMMSPI_CTL1_ENABLE_MASK) ==
            UNICOMMSPI_CTL1_ENABLE_MASK) {
            DL_SPI_enable(unicomm);
        }
        ptr->backupRdy = false;
    }
    return stateRestored;
}

uint32_t DL_SPI_fillTXFIFO32(
    UNICOMM_Inst_Regs *unicomm, uint32_t *buffer, uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++) {
        if (!DL_SPI_isTXFIFOFull(unicomm)) {
            DL_SPI_transmitData32(unicomm, buffer[i]);
        } else {
            break;
        }
    }

    return i;
}

#endif /* __MCU_HAS_UNICOMMSPI__ */
