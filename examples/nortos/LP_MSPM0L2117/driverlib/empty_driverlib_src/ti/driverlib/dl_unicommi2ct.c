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

#include <ti/driverlib/dl_unicommi2ct.h>

#ifdef __MCU_HAS_UNICOMMI2CT__

void DL_I2CT_setClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_ClockConfig *config)
{
    DL_Common_updateReg(&unicomm->i2ct->CLKSEL, (uint32_t) config->clockSel,
        UNICOMMI2CT_CLKSEL_BUSCLK_SEL_MASK |
            UNICOMMI2CT_CLKSEL_MFCLK_SEL_MASK);

    DL_Common_updateReg(&unicomm->i2ct->CLKDIV, (uint32_t) config->divideRatio,
        UNICOMMI2CT_CLKDIV_RATIO_MASK);
}

void DL_I2CT_getClockConfig(
    UNICOMM_Inst_Regs *unicomm, DL_I2CT_ClockConfig *config)
{
    uint32_t clockSel =
        unicomm->i2ct->CLKSEL & (UNICOMMI2CT_CLKSEL_BUSCLK_SEL_MASK |
                                    UNICOMMI2CT_CLKSEL_MFCLK_SEL_MASK);
    config->clockSel = (DL_I2CT_CLOCK)(clockSel);

    uint32_t divideRatio =
        unicomm->i2ct->CLKDIV & UNICOMMI2CT_CLKDIV_RATIO_MASK;
    config->divideRatio = (DL_I2CT_CLOCK_DIVIDE)(divideRatio);
}

uint8_t DL_I2CT_fillTXFIFO(
    UNICOMM_Inst_Regs *unicomm, uint8_t *buffer, uint8_t count)
{
    uint8_t i;
    for (i = (uint8_t) 0; i < count; i++) {
        if (DL_I2CT_isTXFIFOFull(unicomm) == false) {
            DL_I2CT_transmitData(unicomm, buffer[i]);
        } else {  //  TX FIFO is full
            break;
        }
    }
    return i;
}

void DL_I2CT_transmitDataBlocking(UNICOMM_Inst_Regs *unicomm, uint8_t data)
{
    while ((DL_I2CT_getStatus(unicomm) & DL_I2CT_STATUS_TRANSMIT_REQUEST) !=
           DL_I2CT_STATUS_TRANSMIT_REQUEST) {
        ;
    }
    DL_I2CT_transmitData(unicomm, data);
    while ((DL_I2CT_getStatus(unicomm) & DL_I2CT_STATUS_BUS_BUSY) ==
           DL_I2CT_STATUS_BUS_BUSY) {
        ;
    }
}

bool DL_I2CT_transmitDataCheck(UNICOMM_Inst_Regs *unicomm, uint8_t data)
{
    bool status;
    if (DL_I2CT_isTXFIFOFull(unicomm)) {
        status = false;
    } else {
        DL_I2CT_transmitData(unicomm, data);
        status = true;
    }
    return status;
}

uint8_t DL_I2CT_receiveDataBlocking(UNICOMM_Inst_Regs *unicomm)
{
    while ((DL_I2CT_getStatus(unicomm) & DL_I2CT_STATUS_RECEIVE_REQUEST) !=
           DL_I2CT_STATUS_RECEIVE_REQUEST) {
        ;
    }
    return DL_I2CT_receiveData(unicomm);
}

bool DL_I2CT_receiveDataCheck(UNICOMM_Inst_Regs *unicomm, uint8_t *buffer)
{
    bool status;
    if (DL_I2CT_isRXFIFOEmpty(unicomm)) {
        status = false;
    } else {
        *buffer = DL_I2CT_receiveData(unicomm);
        status  = true;
    }
    return status;
}

void DL_I2CT_flushTXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    DL_I2CT_startFlushTXFIFO(unicomm);
    while (DL_I2CT_isTXFIFOEmpty(unicomm) == false) {
        ;
    }
    DL_I2CT_stopFlushTXFIFO(unicomm);
}

void DL_I2CT_flushRXFIFO(UNICOMM_Inst_Regs *unicomm)
{
    DL_I2CT_startFlushRXFIFO(unicomm);
    while (DL_I2CT_isRXFIFOEmpty(unicomm) == false) {
        ;
    }
    DL_I2CT_stopFlushRXFIFO(unicomm);
}

#endif /* __MCU_HAS_UNICOMMI2CT__ */
