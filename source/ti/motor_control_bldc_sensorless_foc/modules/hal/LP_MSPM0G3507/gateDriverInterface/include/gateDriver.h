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
 *  @file       gateDriver.h
 *  @brief      Gate Driver Module
 *
 * 
 *  @anchor gateDriver_h
 *  # Overview
 *
 *  defines gateDriver APIs 
 *
 *  <hr>
 ******************************************************************************/

#ifndef GATEDRIVER_H
#define GATEDRIVER_H

#include "measure.h"
#include "ti_msp_dl_config.h"
#include "appInputCtrlInterface.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief FOC current ADC0 instance */
#define FOC_CURR_ADC0_INST            (ADC0_INST)
/*! @brief FOC current ADC1 instance */
#define FOC_CURR_ADC1_INST            (ADC1_INST)

/*! @brief FOC ADC0 instance */
#define FOC_ADC0_INST            (ADC0_INST)
/*! @brief FOC ADC1 instance */
#define FOC_ADC1_INST            (ADC1_INST)

/*! @brief ADC 0 IDX 0 */
#define ADC0_IDX_0               (ADC0_ADCMEM_0)           
/*! @brief ADC 0 IDX 1 */
#define ADC0_IDX_1               (ADC0_ADCMEM_1)           
/*! @brief ADC 1 IDX 0 */
#define ADC1_IDX_0               (ADC1_ADCMEM_0)           
/*! @brief ADC 1 IDX 1 */
#define ADC1_IDX_1               (ADC1_ADCMEM_1)           

/*! @brief FOC current ADC0 IDX 0 */
#define FOC_CURR_ADC0_IDX_0      (ADC0_ADCMEM_0)      
/*! @brief FOC current ADC0 IDX 1 */
#define FOC_CURR_ADC0_IDX_1      (ADC0_ADCMEM_1)   
/*! @brief FOC current ADC1 IDX 0 */
#define FOC_CURR_ADC1_IDX_0      (ADC1_ADCMEM_0)     
/*! @brief FOC current ADC1 IDX 1 */
#define FOC_CURR_ADC1_IDX_1      (ADC1_ADCMEM_1)           

#ifdef DRV8323
#include "drv8323_spi_hal.h"
#include "drv8323.h"

/*! @brief Memory Load Register for ADC Interrupt */
#define FOC_ADC_MEM_RES_LOAD     DL_ADC12_IIDX_MEM1_RESULT_LOADED 

/*! @brief Channel for Phase U current */
#define ADC1_CURRENT_U_CH        (DL_ADC12_INPUT_CHAN_2)     
/*! @brief Channel for Phase V current */
#define ADC0_CURRENT_V_CH        (DL_ADC12_INPUT_CHAN_3)     
/*! @brief Channel for Phase V current */
#define ADC1_CURRENT_W_CH        (DL_ADC12_INPUT_CHAN_3)     
/*! @brief Channel for Phase W current */
#define ADC0_CURRENT_W_CH        (DL_ADC12_INPUT_CHAN_3)     

/*! @brief ADC instance for DC bus voltage */
#define FOC_ADC_VOLT_DC_INST     (ADC0_INST)
/*! @brief IDX for the DC bus voltage */
#define ADC_VOLT_DC_IDX          (ADC0_ADCMEM_1)    

/*! @brief ADC instance for Phase U voltage */
#define ADC_VOLTAGE_U_INST        (ADC1_INST)       
/*! @brief ADC instance for Phase V voltage */
#define ADC_VOLTAGE_V_INST        (ADC0_INST)       
/*! @brief ADC instance for Phase W voltage */
#define ADC_VOLTAGE_W_INST        (ADC1_INST)       

/*! @brief ADC index for Phase U voltage */
#define ADC_VOLTAGE_U_IDX        (ADC1_ADCMEM_0)     
/*! @brief ADC index for Phase V voltage */
#define ADC_VOLTAGE_V_IDX        (ADC0_ADCMEM_0)     
/*! @brief ADC index for Phase W voltage */
#define ADC_VOLTAGE_W_IDX        (ADC1_ADCMEM_1)     

/*! @brief Channel for Phase U voltage */
#define ADC_VOLTAGE_U_CH        (DL_ADC12_INPUT_CHAN_6)     
/*! @brief Channel for Phase V voltage */
#define ADC_VOLTAGE_V_CH        (DL_ADC12_INPUT_CHAN_7)     
/*! @brief Channel for Phase W voltage */
#define ADC_VOLTAGE_W_CH        (DL_ADC12_INPUT_CHAN_5)     
#endif

#ifdef DRV8316
#include "drv8316_spi_hal.h"
#include "drv8316.h"

/*! @brief Memory Load Register for ADC Interrupt */
#define FOC_ADC_MEM_RES_LOAD     DL_ADC12_IIDX_MEM1_RESULT_LOADED

#define FOC_ADC_VOLT_DC_INST     (ADC1_INST)
/*! @brief IDX for the DC bus voltage */
#define ADC_VOLT_DC_IDX          (ADC1_ADCMEM_2)    

/*! @brief Channel for Phase U current */
#define ADC0_CURRENT_U_CH        (DL_ADC12_INPUT_CHAN_3)     
/*! @brief Channel for Phase V current */
#define ADC0_CURRENT_V_CH        (DL_ADC12_INPUT_CHAN_2)
/*! @brief Channel for Phase V current */
#define ADC1_CURRENT_V_CH        (DL_ADC12_INPUT_CHAN_2)     
/*! @brief Channel for Phase W current */
#define ADC1_CURRENT_W_CH        (DL_ADC12_INPUT_CHAN_1)     

/*! @brief ADC instance for Phase U voltage */
#define ADC_VOLTAGE_U_INST        (ADC1_INST)       
/*! @brief ADC instance for Phase V voltage */
#define ADC_VOLTAGE_V_INST        (ADC0_INST)       
/*! @brief ADC instance for Phase W voltage */
#define ADC_VOLTAGE_W_INST        (ADC1_INST)       

/*! @brief ADC index for Phase U voltage */
#define ADC_VOLTAGE_U_IDX        (ADC1_ADCMEM_0)     
/*! @brief ADC index for Phase V voltage */
#define ADC_VOLTAGE_V_IDX        (ADC0_ADCMEM_0)     
/*! @brief ADC index for Phase W voltage */
#define ADC_VOLTAGE_W_IDX        (ADC1_ADCMEM_1)     

/*! @brief Channel for Phase U voltage */
#define ADC_VOLTAGE_U_CH        (DL_ADC12_INPUT_CHAN_6)     
/*! @brief Channel for Phase V voltage */
#define ADC_VOLTAGE_V_CH        (DL_ADC12_INPUT_CHAN_7)     
/*! @brief Channel for Phase W voltage */
#define ADC_VOLTAGE_W_CH        (DL_ADC12_INPUT_CHAN_5)     

#endif
#ifdef CUSTOM

#include "custom.h"
/*! @brief Memory Load Register for ADC Interrupt */
#define FOC_ADC_MEM_RES_LOAD     DL_ADC12_IIDX_MEM1_RESULT_LOADED

#define FOC_ADC_VOLT_DC_INST     (ADC1_INST)
/*! @brief IDX for the DC bus voltage */
#define ADC_VOLT_DC_IDX          (ADC1_ADCMEM_2)    

/*! @brief Channel for Phase U current */
#define ADC0_CURRENT_U_CH        (DL_ADC12_INPUT_CHAN_3)     
/*! @brief Channel for Phase V current */
#define ADC0_CURRENT_V_CH        (DL_ADC12_INPUT_CHAN_1)     
/*! @brief Channel for Phase V current */
#define ADC1_CURRENT_V_CH        (DL_ADC12_INPUT_CHAN_2)     
/*! @brief Channel for Phase W current */
#define ADC1_CURRENT_W_CH        (DL_ADC12_INPUT_CHAN_1)     

/*! @brief ADC instance for Phase U voltage */
#define ADC_VOLTAGE_U_INST        (ADC1_INST)       
/*! @brief ADC instance for Phase V voltage */
#define ADC_VOLTAGE_V_INST        (ADC0_INST)       
/*! @brief ADC instance for Phase W voltage */
#define ADC_VOLTAGE_W_INST        (ADC1_INST)       

/*! @brief ADC index for Phase U voltage */
#define ADC_VOLTAGE_U_IDX        (ADC1_ADCMEM_0)     
/*! @brief ADC index for Phase V voltage */
#define ADC_VOLTAGE_V_IDX        (ADC0_ADCMEM_0)     
/*! @brief ADC index for Phase W voltage */
#define ADC_VOLTAGE_W_IDX        (ADC1_ADCMEM_1)     

/*! @brief Channel for Phase U voltage */
#define ADC_VOLTAGE_U_CH        (DL_ADC12_INPUT_CHAN_6)     
/*! @brief Channel for Phase V voltage */
#define ADC_VOLTAGE_V_CH        (DL_ADC12_INPUT_CHAN_7)     
/*! @brief Channel for Phase W voltage */
#define ADC_VOLTAGE_W_CH        (DL_ADC12_INPUT_CHAN_5)     
#endif

/*! @brief Extern gate driver fault report */
extern uint32_t gateDrivernFaultReport;
/*! @brief Extern gate driver fault action */
extern uint32_t gateDriverFaultAction;

/**
 * @brief     Read Current from gate driver
 * @param[in]  pCurrent Pointer to measure current structure
 */
void HAL_GD_ReadCurrents(HAL_MEASURE_CURRENT_T *pCurrent);

/**
 * @brief     Read phase voltage from gate driver
 * @param[in]  pVoltage Pointer to measure voltage structure
 */
void HAL_GD_ReadVoltages(HAL_MEASURE_VOLTAGE_T *pVoltage);

/**
 * @brief     Configure phase voltage channels
 */
void HAL_GD_ConfigureVoltageChannels(void);

/**
 * @brief     Configure phase current channels
 * @param[in]  currentShunt Current shunt type
 */
void HAL_GD_ConfigureCurrentChannels(CURRENT_SHUNT_TYPES currentShunt);

/**
 * @brief     Reads DC bus voltage
 * @param[in]  pVoltage Pointer to measure voltage structure
 */
void HAL_GD_ReadDCVBusVoltage(HAL_MEASURE_VOLTAGE_T *pVoltage);

/**
 * @brief     Initializes gat driver register configuration
 */
void gateDriverConfig(void);

/**
 * @brief     Clears gate driver fault
 */
void gateDriverClearFault(void);

/**
 * @brief     Gets gate driver fault status
 * @return    Returns gate driver fault status
 */
uint32_t gateDriverGetFaultStatus(void);

/**
 * @brief     Response to any occurred fault
 * @param[in]  pGateDriverConfig Pointer to HV_DIE_EEPROM_INTERFACE_T structure
 */
void gateDriverFaultResponse(HV_DIE_EEPROM_INTERFACE_T *pGateDriverConfig);

/**
 * @brief     Updates gate driver parameters
 * @param[in]  pGateDriverConfig Pointer to HV_DIE_EEPROM_INTERFACE_T structure
 */
void gateDriverParamsUpdate(HV_DIE_EEPROM_INTERFACE_T *pGateDriverConfig);

/**
 * @brief     Calibrates gate driver CSA
 */
void gateDriverCSACalib(void);

/**
 * @brief     Calibrates gate driver CSA offset
 */
void gateDriverOffsetCalibSet(void);

/**
 * @brief     Calibrates gate driver CSA offset reset
 */
void gateDriverOffsetCalibReset(void);

/**
 * @brief     Initializes gate drive module
 */
void gateDriverInit(void);

/**
 * @brief     Reads any gate driver register, used for testing spi communication
 * @param[in] regAddr The register to be read
 * @return    Returns the register value
 */
uint16_t gateDriverRegRead(int16_t regAddr);

#ifdef __cplusplus
}
#endif
#endif /* GATEDRIVER_H */
