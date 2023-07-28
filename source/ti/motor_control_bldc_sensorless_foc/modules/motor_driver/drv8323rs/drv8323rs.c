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
#include "drv8323rs.h"

/**
 * @brief Stores the gain values available to be set in drv
 */
int8_t drvGain[DRV8323RS_CSA_GAIN_MAX_VALUE] = { 5, 10, 20, 40};

void DRV8323RS_init(DRV8323RS_Instance *handle)
{
    HAL_enableADCInterrupt(handle->vsenvm);
    HAL_enableADCInterrupt(handle->isena);
    HAL_enableADCInterrupt(handle->isenb);

    DRV8323RS_disable(handle);
    DRV8323RS_enable(handle);

    DRV8323RS_clearFaults(handle);
}

uint16_t DRV8323RS_createSPIDataWord(
                DRV8323RS_SPI_CMD wrCmd, DRV8323RS_REG_ADDR addr, uint16_t data)
{   
    /* Bit 15 W0: write = 0 and read = 1 */
    uint16_t dataWord = ((wrCmd) << DRV8323RS_SPI_CMD_OFS);

    /* Bit 14-11: address */
    dataWord |= ((addr << DRV8323RS_SPI_ADDR_OFS) & DRV8323RS_SPI_ADDR_MASK);

    /* Bit 10-0: data */
    dataWord |= (data & DRV8323RS_SPI_DATA_MASK);
    return  dataWord;
}

void DRV8323RS_enable(DRV8323RS_Instance *handle)
{
    /* Enable the DRV8323RS */
    HAL_setGPIOVal(handle->enable, HAL_GPIO_VALUE_HIGH);

    /* Startup delay for the DRV8323RS SPI to be ready */
    HAL_delayMilliSeconds(DRV8323RS_SPI_READY_DELAY_MS);
}

void DRV8323RS_disable(DRV8323RS_Instance *handle)
{
    /* Disable the DRV8323RS */
    HAL_setGPIOVal(handle->enable, HAL_GPIO_VALUE_LOW);

    /* Allow DRV8323RS to go to sleep which will reset its registers values.
    Datasheet indicates that you need to wait atleast 1 ms to ensure it has
    gone into sleep mode. */
    HAL_delayMilliSeconds(DRV8323RS_TURNOFF_DELAY_MS);
}

void DRV8323RS_spiWrite(
            DRV8323RS_Instance *handle, DRV8323RS_REG_ADDR addr, uint16_t data)
{
    uint16_t dataWord = DRV8323RS_createSPIDataWord(
                                           DRV8323RS_SPI_CMD_WRITE, addr, data);
    HAL_transmitSPIData16(handle->spi, handle->spiCS, dataWord);
    while (HAL_isSPIBusy(handle->spi))
    {
        /* This is expected to be empty*/
    };
}

uint16_t DRV8323RS_spiRead(DRV8323RS_Instance *handle, DRV8323RS_REG_ADDR addr)
{
    HAL_SPI_clearRXFifo(handle->spi, handle->spiCS);
    uint16_t dataWord = DRV8323RS_createSPIDataWord(
                        DRV8323RS_SPI_CMD_READ, addr, DRV8323RS_SPI_DUMMY_DATA);
    HAL_transmitSPIData16(handle->spi, handle->spiCS, dataWord);
    while (HAL_isSPIBusy(handle->spi))
    {
        /* This is expected to be empty*/
    };
    return (uint16_t)(DRV8323RS_SPI_DATA_MASK & 
                              HAL_receiveSPIData16(handle->spi, handle->spiCS));
}

void DRV8323RS_spiUpdateRegister(DRV8323RS_Instance *handle, 
                          DRV8323RS_REG_ADDR addr, uint32_t mask, uint16_t data)
{
    uint16_t dataValue;
    dataValue = (uint16_t) DRV8323RS_spiRead(handle, addr);
    dataValue &= ~(mask);
    dataValue |= data;
    DRV8323RS_spiWrite(handle, addr, dataValue);
}

void DRV8323RS_setSPIRegisterLock(
                  DRV8323RS_Instance *handle, DRV8323RS_GATE_DRIVE_HS_LOCK lock)
{
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_GATE_DRIVE_HS,
                                    DRV8323RS_GATE_DRIVE_HS_LOCK_MASK, 
                                    (lock << DRV8323RS_GATE_DRIVE_HS_LOCK_OFS));
}

void DRV8323RS_setPeakSourceGateCurr(
                                DRV8323RS_Instance *handle,
                                DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR hsPeakCurr, 
                                DRV8323RS_GATE_DRIVE_PEAK_SOURCE_CUR lsPeakCurr)
{
    DRV8323RS_spiUpdateRegister(handle,DRV8323RS_REG_ADDR_GATE_DRIVE_HS,
                        DRV8323RS_GATE_DRIVE_HS_IDRIVEP_HS_MASK, 
                        (hsPeakCurr << DRV8323RS_GATE_DRIVE_HS_IDRIVEP_HS_OFS));

    DRV8323RS_spiUpdateRegister(handle,DRV8323RS_REG_ADDR_GATE_DRIVE_LS,
                        DRV8323RS_GATE_DRIVE_LS_IDRIVEP_LS_MASK, 
                        (lsPeakCurr << DRV8323RS_GATE_DRIVE_LS_IDRIVEP_LS_OFS));
}

void DRV8323RS_setPeakSinkGateCurr(
                                  DRV8323RS_Instance *handle,
                                  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR hsPeakCurr, 
                                  DRV8323RS_GATE_DRIVE_PEAK_SINK_CUR lsPeakCurr)
{
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_GATE_DRIVE_HS,
                        DRV8323RS_GATE_DRIVE_HS_IDRIVEN_HS_MASK, 
                        (hsPeakCurr << DRV8323RS_GATE_DRIVE_HS_IDRIVEN_HS_OFS));

    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_GATE_DRIVE_LS,
                        DRV8323RS_GATE_DRIVE_LS_IDRIVEN_LS_MASK, 
                        (lsPeakCurr << DRV8323RS_GATE_DRIVE_LS_IDRIVEN_LS_OFS));
}

void DRV8323RS_setPeakGateCurrTime(DRV8323RS_Instance *handle,
                                      DRV8323RS_GATE_DRIVE_PEAK_CUR_TIME tdrive)
{
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_GATE_DRIVE_LS,
                        DRV8323RS_GATE_DRIVE_LS_TDRIVE_MASK, 
                        (tdrive << DRV8323RS_GATE_DRIVE_LS_TDRIVE_OFS));
}

void DRV8323RS_setPwmMode(DRV8323RS_Instance *handle,
                                      DRV8323RS_DRIVER_CONTROL_PWM_MODE pwmMode)
{
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_DRIVER_CONTROL,
                            DRV8323RS_DRIVER_CONTROL_PWM_MODE_MASK, 
                            (pwmMode << DRV8323RS_DRIVER_CONTROL_PWM_MODE_OFS));
}

void DRV8323RS_setVdsLvl(DRV8323RS_Instance *handle,
                                           DRV8323RS_OCP_CONTROL_VDS_LVL vdsLvl)
{
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_OCP_CONTROL,
                                DRV8323RS_OCP_CONTROL_VDS_LVL_MASK, 
                                (vdsLvl << DRV8323RS_OCP_CONTROL_VDS_LVL_OFS));
}

void DRV8323RS_setOcpDeg(DRV8323RS_Instance *handle,
                                           DRV8323RS_OCP_CONTROL_OCP_DEG ocpDeg)
{
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_OCP_CONTROL,
                                DRV8323RS_OCP_CONTROL_OCP_DEG_MASK, 
                                (ocpDeg << DRV8323RS_OCP_CONTROL_OCP_DEG_OFS));
}

void DRV8323RS_setOcpMode(DRV8323RS_Instance *handle,
                                         DRV8323RS_OCP_CONTROL_OCP_MODE ocpMode)
{
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_OCP_CONTROL,
                               DRV8323RS_OCP_CONTROL_OCP_MODE_MASK, 
                               (ocpMode << DRV8323RS_OCP_CONTROL_OCP_MODE_OFS));
}

void DRV8323RS_setDeadtime(DRV8323RS_Instance *handle,
                                        DRV8323RS_OCP_CONTROL_DEADTIME deadtime)
{
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_OCP_CONTROL,
                             DRV8323RS_OCP_CONTROL_DEAD_TIME_MASK, 
                             (deadtime << DRV8323RS_OCP_CONTROL_DEAD_TIME_OFS));
}

void DRV8323RS_setTretry(DRV8323RS_Instance *handle,
                                            DRV8323RS_OCP_CONTROL_TRETRY tretry)
{
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_OCP_CONTROL,
                                  DRV8323RS_OCP_CONTROL_TRETRY_MASK, 
                                  (tretry << DRV8323RS_OCP_CONTROL_TRETRY_OFS));
}

void DRV8323RS_setSenLvl(DRV8323RS_Instance *handle,
                                           DRV8323RS_CSA_CONTROL_SEN_LVL senLvl)
{
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_CSA_CONTROL,
                                 DRV8323RS_CSA_CONTROL_SEN_LVL_MASK, 
                                 (senLvl << DRV8323RS_CSA_CONTROL_SEN_LVL_OFS));
}

void DRV8323RS_setDisSen(DRV8323RS_Instance *handle,
                                           DRV8323RS_CSA_CONTROL_DIS_SEN disSen)
{
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_CSA_CONTROL,
                                 DRV8323RS_CSA_CONTROL_DIS_SEN_MASK,
                                 (disSen << DRV8323RS_CSA_CONTROL_DIS_SEN_OFS));
}

void DRV8323RS_setCsaGain(DRV8323RS_Instance *handle,
                                         DRV8323RS_CSA_CONTROL_CSA_GAIN csaGain)
{
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_CSA_CONTROL,
                                DRV8323RS_CSA_CONTROL_CSA_GAIN_MASK,
                               (csaGain << DRV8323RS_CSA_CONTROL_CSA_GAIN_OFS));
}

void DRV8323RS_setLsRef(DRV8323RS_Instance *handle,
                                             DRV8323RS_CSA_CONTROL_LS_REF lsRef)
{
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_CSA_CONTROL,
                                   DRV8323RS_CSA_CONTROL_LS_REF_MASK,
                                   (lsRef << DRV8323RS_CSA_CONTROL_LS_REF_OFS));
}

void DRV8323RS_setVrefDiv(DRV8323RS_Instance *handle,
                                         DRV8323RS_CSA_CONTROL_VREF_DIV vrefDiv)
{
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_CSA_CONTROL,
                               DRV8323RS_CSA_CONTROL_VREF_DIV_MASK,
                               (vrefDiv << DRV8323RS_CSA_CONTROL_VREF_DIV_OFS));
}

void DRV8323RS_setCsaFet(DRV8323RS_Instance *handle,
                                           DRV8323RS_CSA_CONTROL_CSA_FET csaFet)
{
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_CSA_CONTROL,
                                 DRV8323RS_CSA_CONTROL_CSA_FET_MASK,
                                 (csaFet << DRV8323RS_CSA_CONTROL_CSA_FET_OFS));
}

_iq DRV8323RS_getIA(DRV8323RS_Instance *handle)
{
    _iq15 adcVal = HAL_getADCVoltage(handle->isena);
    _iq15 offset = _IQdiv2(HAL_getADCRefVoltage(handle->isena));
    return _IQ15toIQ(_IQ15mpy(adcVal - offset, handle->iSf));
}

_iq DRV8323RS_getIB(DRV8323RS_Instance *handle)
{
    _iq15 adcVal = HAL_getADCVoltage(handle->isenb);
    _iq15 offset = _IQdiv2(HAL_getADCRefVoltage(handle->isenb));
    return _IQ15toIQ(_IQ15mpy(adcVal - offset, handle->iSf));
}

_iq DRV8323RS_getIC(DRV8323RS_Instance *handle)
{
    _iq15 adcVal = HAL_getADCVoltage(handle->ic);
    _iq15 offset = _IQdiv2(HAL_getADCRefVoltage(handle->ic));
    return _IQ15toIQ(_IQ15mpy(adcVal - offset, handle->iSf));
}

_iq DRV8323RS_getVM(DRV8323RS_Instance *handle)
{
    _iq15 adcVal = (HAL_getADCVoltage(handle->vsenvm));
    return _IQ15toIQ(_IQ15mpy(adcVal, DRV8323RS_VOLTAGE_SF_IQ15));
}

void DRV8323RS_setISf(DRV8323RS_Instance *handle,
                             DRV8323RS_CSA_CONTROL_CSA_GAIN  gain, _iq15 rshunt)
{
    handle->iSf = _IQ15div(_IQ15(1),
                               _IQ15mpy(_IQ15((int32_t)drvGain[gain]), rshunt));
}

bool DRV8323RS_isFaultOccurred(DRV8323RS_Instance *handle)
{
    return HAL_getPWMFaultStatus(handle->nfault);
}

void DRV8323RS_clearFaults(DRV8323RS_Instance *handle)
{
    HAL_clearPWMFault(handle->nfault);
    DRV8323RS_spiUpdateRegister(handle, DRV8323RS_REG_ADDR_DRIVER_CONTROL, 
                                DRV8323RS_DRIVER_CONTROL_CLR_FLT_MASK, 
                                (1 << DRV8323RS_DRIVER_CONTROL_CLR_FLT_OFS));
}
