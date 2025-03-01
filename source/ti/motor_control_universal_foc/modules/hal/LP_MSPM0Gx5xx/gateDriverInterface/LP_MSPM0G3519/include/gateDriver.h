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
/*! @brief ADC 0 IDX 2 */
#define ADC0_IDX_2               (ADC0_ADCMEM_2)
/*! @brief ADC 1 IDX 0 */
#define ADC1_IDX_0               (ADC1_ADCMEM_0)
/*! @brief ADC 1 IDX 1 */
#define ADC1_IDX_1               (ADC1_ADCMEM_1)
/*! @brief ADC 1 IDX 1 */
#define ADC1_IDX_2               (ADC1_ADCMEM_2)


/*! @brief FOC current ADC0 IDX 0 */
#define FOC_CURR_ADC0_IDX_0      (ADC0_ADCMEM_0)
/*! @brief FOC current ADC0 IDX 1 */
#define FOC_CURR_ADC0_IDX_1      (ADC0_ADCMEM_1)
/*! @brief FOC current ADC1 IDX 2 */
#define FOC_CURR_ADC0_IDX_2      (ADC0_ADCMEM_2)
/*! @brief FOC current ADC1 IDX 0 */
#define FOC_CURR_ADC1_IDX_0      (ADC1_ADCMEM_0)
/*! @brief FOC current ADC1 IDX 1 */
#define FOC_CURR_ADC1_IDX_1      (ADC1_ADCMEM_1)
/*! @brief FOC current ADC1 IDX 2 */
#define FOC_CURR_ADC1_IDX_2      (ADC1_ADCMEM_2)


#ifdef DRV8329
#include "drv8329.h"

/*! @brief Timer instance used for IPD */
#define FOC_CAPTURE_IPD_INST            (CAPTURE_INST)
/*! @brief Capture index used for IPD */
#define FOC_CAPTURE_IPD_IDX             (DL_TIMER_CC_0_INDEX)
/*! @brief Load value for IPD timer */
#define FOC_CAPTURE_IPD_LOAD_VALUE      (CAPTURE_INST_LOAD_VALUE)
/*! @brief IRQ number for IPD timer */
#define FOC_CAPTURE_IRQN                (CAPTURE_INST_INT_IRQN)
/*! @brief Event number for IPD timer */
#define FOC_IPD_EVENT_CH                (CAPTURE_INST_SUB_0_CH)

/*! @brief Definition to Toggle GPIO Pin Every Sector change */
#define FG_OUTPUT_ENABLE

#ifdef FOC_GPIO_OUT_PORT
/*! @brief GPIO port for drv8329 nsleep */
#define DRV8329_NSLEEP_PORT FOC_GPIO_OUT_PORT
#else
/*! @brief GPIO port for drv8329 nsleep */
#define DRV8329_NSLEEP_PORT FOC_GPIO_OUT_NSLEEP_PORT
#endif

/*! @brief GPIO pin for drv8329 nsleep pin */
#define DRV8329_NSLEEP_PIN FOC_GPIO_OUT_NSLEEP_PIN

/*! @brief PWM U phase A compare index */
#define FOC_PWMA0_U_IDX           (GPIO_PWMA0_C0_IDX)
/*! @brief PWM V phase A compare index */
#define FOC_PWMA0_V_IDX           (GPIO_PWMA0_C1_IDX)
/*! @brief PWM W phase A compare index */
#define FOC_PWMA0_W_IDX           (GPIO_PWMA0_C2_IDX)

/*! @brief PWM U phase A compare index */
#define FOC_PWMA0_U_B_IDX           (DL_TIMER_CC_3_INDEX)
/*! @brief PWM V phase B compare index */
#define FOC_PWMA0_V_B_IDX           (DL_TIMER_CC_4_INDEX)
/*! @brief PWM W phase B compare index */
#define FOC_PWMA0_W_B_IDX           (DL_TIMER_CC_5_INDEX)
/*! @brief Index used for ADC trigger 0 */
#define FOC_ADC_TRIG_0_IDX    (DL_TIMER_CC_0_INDEX)
/*! @brief Index used for ADC trigger 1 */
#define FOC_ADC_TRIG_1_IDX    (DL_TIMER_CC_1_INDEX)

/*! @brief FOC current ADC instance */
#define FOC_CURR_ADC_INSTANCE            (ADC1_INST)

/*! @brief ADC - Sample First Index  */
#define ADC_FIRST_IDX               (DL_ADC12_MEM_IDX_0)
/*! @brief ADC Sample Second Index */
#define ADC_SECOND_IDX              (DL_ADC12_MEM_IDX_1)

/*! @brief Instance for ADC Interrupt */
#define FOC_ADC_ISR_INST        (ADC0_INST)
/*! @brief Memory Load Register for ADC Interrupt */
#define FOC_ADC_MEM_RES_LOAD     DL_ADC12_IIDX_MEM3_RESULT_LOADED

/*! @brief Channel for ADC 0 DC current */
#define ADC_DC_CURRENT_CH        (DL_ADC12_INPUT_CHAN_14)


/*! @brief ADC instance for DC bus voltage */
#define FOC_ADC_VOLT_DC_INST     (ADC0_INST)
/*! @brief IDX for the DC bus voltage */
#define ADC_VOLT_DC_IDX          (ADC0_ADCMEM_1)

/*! @brief ADC instance for Phase U voltage */
#define ADC_VOLTAGE_U_INST        (ADC0_INST)
/*! @brief ADC instance for Phase V voltage */
#define ADC_VOLTAGE_V_INST        (ADC0_INST)       
/*! @brief ADC instance for Phase W voltage */
#define ADC_VOLTAGE_W_INST        (ADC0_INST)

/*! @brief ADC index for Phase U voltage */
#define ADC_VOLTAGE_U_IDX        (ADC0_ADCMEM_0)
/*! @brief ADC index for Phase V voltage */
#define ADC_VOLTAGE_V_IDX        (ADC0_ADCMEM_2)
/*! @brief ADC index for Phase W voltage */
#define ADC_VOLTAGE_W_IDX        (ADC0_ADCMEM_3)

/*! @brief Channel for Phase U voltage */
#define ADC_VOLTAGE_U_CH        (DL_ADC12_INPUT_CHAN_0)
/*! @brief Channel for Phase V voltage */
#define ADC_VOLTAGE_V_CH        (DL_ADC12_INPUT_CHAN_1)
/*! @brief Channel for Phase W voltage */
#define ADC_VOLTAGE_W_CH        (DL_ADC12_INPUT_CHAN_5)     
#endif

#ifdef DRV8323
#include "drv8323_spi_hal.h"
#include "drv8323.h"

/*! @brief Timer instance used for IPD */
#define FOC_CAPTURE_IPD_INST            (CAPTURE_INST)
/*! @brief Capture index used for IPD */
#define FOC_CAPTURE_IPD_IDX             (DL_TIMER_CC_0_INDEX)
/*! @brief Load value for IPD timer */
#define FOC_CAPTURE_IPD_LOAD_VALUE      (CAPTURE_INST_LOAD_VALUE)
/*! @brief IRQ number for IPD timer */
#define FOC_CAPTURE_IRQN                (CAPTURE_INST_INT_IRQN)
/*! @brief Event number for IPD timer */
#define FOC_IPD_EVENT_CH                (CAPTURE_INST_SUB_0_CH)

#ifndef __CURRENT_SINGLE_SHUNT

/*! @brief PWM U phase index */
#define FOC_PWMA0_U_IDX           (GPIO_PWMA0_C0_IDX)
/*! @brief PWM V phase index */
#define FOC_PWMA0_V_IDX           (GPIO_PWMA0_C1_IDX)
/*! @brief PWM W phase index */
#define FOC_PWMA0_W_IDX           (GPIO_PWMA0_C2_IDX)
/*! @brief PWM Index used for ADC trigger */
#define FOC_PWMA0_ADC_TRIG_IDX    (GPIO_PWMA0_C3_IDX)

/*! @brief Define for ADC trigger up event */
#define FOC_PWMA0_ADC_TRIG_UP_EVENT     (DL_TIMERA_EVENT_CC3_UP_EVENT)
/*! @brief Define for ADC trigger down event */
#define FOC_PWMA0_ADC_TRIG_DN_EVENT     (DL_TIMERA_EVENT_CC3_DN_EVENT)

/*! @brief Instance for ADC Interrupt */
#define FOC_ADC_ISR_INST        (ADC0_INST)
/*! @brief Memory Load Register for ADC Interrupt */
#define FOC_ADC_MEM_RES_LOAD     DL_ADC12_IIDX_MEM1_RESULT_LOADED

/*! @brief Channel for Phase U current */
#define ADC1_CURRENT_U_CH        (DL_ADC12_INPUT_CHAN_14)
/*! @brief Channel for Phase V current */
#define ADC1_CURRENT_V_CH        (DL_ADC12_INPUT_CHAN_13)
/*! @brief Channel for Phase W current */
#define ADC0_CURRENT_W_CH        (DL_ADC12_INPUT_CHAN_4)

#else

/*! @brief PWM U phase A compare index */
#define FOC_PWMA0_U_IDX           (GPIO_PWMA0_C2_IDX)
/*! @brief PWM V phase A compare index */
#define FOC_PWMA0_V_IDX           (GPIO_PWMA0_C3_IDX)
/*! @brief PWM W phase A compare index */
#define FOC_PWMA0_W_IDX           (GPIO_PWMA0_C1_IDX)

/*! @brief PWM U phase A compare index */
#define FOC_PWMA0_U_B_IDX           (DL_TIMER_CC_5_INDEX)
/*! @brief PWM V phase B compare index */
#define FOC_PWMA0_V_B_IDX           (DL_TIMER_CC_0_INDEX)
/*! @brief PWM W phase B compare index */
#define FOC_PWMA0_W_B_IDX           (DL_TIMER_CC_4_INDEX)
/*! @brief Index used for ADC trigger 0 */
#define FOC_ADC_TRIG_0_IDX    (DL_TIMER_CC_0_INDEX)
/*! @brief Index used for ADC trigger 1 */
#define FOC_ADC_TRIG_1_IDX    (DL_TIMER_CC_1_INDEX)

/*! @brief ADC interrupt for running FOC */
#define FOC_ADC_ISR_INST        ADC0_INST
/*! @brief Memory Load Register for ADC Interrupt */
#define FOC_ADC_MEM_RES_LOAD     DL_ADC12_IIDX_MEM0_RESULT_LOADED

/*! @brief Channel for ADC 0 DC current */
#define ADC0_DC_CURRENT_CH        (DL_ADC12_INPUT_CHAN_3)   
/*! @brief Channel for ADC 1 DC current */
#define ADC1_DC_CURRENT_CH        (DL_ADC12_INPUT_CHAN_3)      
#endif

/*! @brief ADC instance for DC bus voltage */
#define FOC_ADC_VOLT_DC_INST     (ADC0_INST)
/*! @brief IDX for the DC bus voltage */
#define ADC_VOLT_DC_IDX          (ADC0_ADCMEM_1)    

/*! @brief ADC instance for Phase U voltage */
#define ADC_VOLTAGE_U_INST        (ADC0_INST)
/*! @brief ADC instance for Phase V voltage */
#define ADC_VOLTAGE_V_INST        (ADC0_INST)       
/*! @brief ADC instance for Phase W voltage */
#define ADC_VOLTAGE_W_INST        (ADC0_INST)

/*! @brief ADC index for Phase U voltage */
#define ADC_VOLTAGE_U_IDX        (ADC0_ADCMEM_0)
/*! @brief ADC index for Phase V voltage */
#define ADC_VOLTAGE_V_IDX        (ADC0_ADCMEM_2)
/*! @brief ADC index for Phase W voltage */
#define ADC_VOLTAGE_W_IDX        (ADC0_ADCMEM_3)

/*! @brief Channel for Phase U voltage */
#define ADC_VOLTAGE_U_CH        (DL_ADC12_INPUT_CHAN_0)
/*! @brief Channel for Phase V voltage */
#define ADC_VOLTAGE_V_CH        (DL_ADC12_INPUT_CHAN_1)
/*! @brief Channel for Phase W voltage */
#define ADC_VOLTAGE_W_CH        (DL_ADC12_INPUT_CHAN_5)     
#endif

#ifdef DRV8316
#include "drv8316_spi_hal.h"
#include "drv8316.h"

#ifdef FOC_GPIO_OUT_PORT
/*! @brief GPIO port for drv8316 chip select */
#define DRV8316_SPI_NSCS_PORT FOC_GPIO_OUT_PORT
#else
/*! @brief GPIO port for drv8316 chip select */
#define DRV8316_SPI_NSCS_PORT FOC_GPIO_OUT_NSCS_PORT
#endif

/*! @brief GPIO pin for drv8329 chip select pin */
#define DRV8316_SPI_NSCS_PIN FOC_GPIO_OUT_NSCS_PIN

/*! @brief Timer instance used for IPD */
#define FOC_CAPTURE_IPD_INST            (CAPTURE_INST)
/*! @brief Capture index used for IPD */
#define FOC_CAPTURE_IPD_IDX             (DL_TIMER_CC_0_INDEX)
/*! @brief Load value for IPD timer */
#define FOC_CAPTURE_IPD_LOAD_VALUE      (CAPTURE_INST_LOAD_VALUE)
/*! @brief IRQ number for IPD timer */
#define FOC_CAPTURE_IRQN                (CAPTURE_INST_INT_IRQN)
/*! @brief Event number for IPD timer */
#define FOC_IPD_EVENT_CH                (CAPTURE_INST_SUB_0_CH)

/*! @brief PWM U phase index */
#define FOC_PWMA0_U_IDX           (GPIO_PWMA0_C0_IDX)
/*! @brief PWM V phase index */
#define FOC_PWMA0_V_IDX           (GPIO_PWMA0_C1_IDX)
/*! @brief PWM W phase index */
#define FOC_PWMA0_W_IDX           (GPIO_PWMA0_C2_IDX)
/*! @brief PWM Index used for ADC trigger */
#define FOC_PWMA0_ADC_TRIG_IDX    (GPIO_PWMA0_C3_IDX)

/*! @brief Define for ADC trigger up event */
#define FOC_PWMA0_ADC_TRIG_UP_EVENT     (DL_TIMERA_EVENT_CC3_UP_EVENT)
/*! @brief Define for ADC trigger down event */
#define FOC_PWMA0_ADC_TRIG_DN_EVENT     (DL_TIMERA_EVENT_CC3_DN_EVENT)

/*! @brief Instance for ADC Interrupt */
#define FOC_ADC_ISR_INST        (ADC0_INST)
/*! @brief Memory Load Register for ADC Interrupt */
#define FOC_ADC_MEM_RES_LOAD     DL_ADC12_IIDX_MEM1_RESULT_LOADED

/*! @brief ADC instance for DC bus voltage */
#define FOC_ADC_VOLT_DC_INST     (ADC0_INST)
/*! @brief IDX for the DC bus voltage */
#define ADC_VOLT_DC_IDX          (ADC0_ADCMEM_2)

/*! @brief Channel for Phase U current */
#define ADC1_CURRENT_U_CH        (DL_ADC12_INPUT_CHAN_13)
/*! @brief Channel for Phase V current */
#define ADC1_CURRENT_V_CH        (DL_ADC12_INPUT_CHAN_14)
/*! @brief Channel for Phase V current */
#define ADC0_CURRENT_V_CH        (DL_ADC12_INPUT_CHAN_12)
/*! @brief Channel for Phase W current */
#define ADC0_CURRENT_W_CH        (DL_ADC12_INPUT_CHAN_2)

/*! @brief ADC instance for Phase U voltage */
#define ADC_VOLTAGE_U_INST        (ADC0_INST)
/*! @brief ADC instance for Phase V voltage */
#define ADC_VOLTAGE_V_INST        (ADC0_INST)
/*! @brief ADC instance for Phase W voltage */
#define ADC_VOLTAGE_W_INST        (ADC0_INST)

/*! @brief ADC index for Phase U voltage */
#define ADC_VOLTAGE_U_IDX        (ADC0_ADCMEM_0)
/*! @brief ADC index for Phase V voltage */
#define ADC_VOLTAGE_V_IDX        (ADC0_ADCMEM_1)
/*! @brief ADC index for Phase W voltage */
#define ADC_VOLTAGE_W_IDX        (ADC0_ADCMEM_3)

/*! @brief Channel for Phase U voltage */
#define ADC_VOLTAGE_U_CH        (DL_ADC12_INPUT_CHAN_0)
/*! @brief Channel for Phase V voltage */
#define ADC_VOLTAGE_V_CH        (DL_ADC12_INPUT_CHAN_1)
/*! @brief Channel for Phase W voltage */
#define ADC_VOLTAGE_W_CH        (DL_ADC12_INPUT_CHAN_5)     

#endif


#ifdef CUSTOM
#include "custom.h"

/*! @brief Timer instance used for IPD */
#define FOC_CAPTURE_IPD_INST            (CAPTURE_INST)
/*! @brief Capture index used for IPD */
#define FOC_CAPTURE_IPD_IDX             (DL_TIMER_CC_0_INDEX)
/*! @brief Load value for IPD timer */
#define FOC_CAPTURE_IPD_LOAD_VALUE      (CAPTURE_INST_LOAD_VALUE)
/*! @brief IRQ number for IPD timer */
#define FOC_CAPTURE_IRQN                (CAPTURE_INST_INT_IRQN)
/*! @brief Event number for IPD timer */
#define FOC_IPD_EVENT_CH                (CAPTURE_INST_SUB_0_CH)

/*! @brief PWM U phase index */
#define FOC_PWMA0_U_IDX           (GPIO_PWMA0_C2_IDX)
/*! @brief PWM V phase index */
#define FOC_PWMA0_V_IDX           (GPIO_PWMA0_C3_IDX)
/*! @brief PWM W phase index */
#define FOC_PWMA0_W_IDX           (GPIO_PWMA0_C1_IDX)
/*! @brief PWM Index used for ADC trigger */
#define FOC_PWMA0_ADC_TRIG_IDX    (GPIO_PWMA0_C0_IDX)

/*! @brief Define for ADC trigger up event */
#define FOC_PWMA0_ADC_TRIG_UP_EVENT     (DL_TIMERA_EVENT_CC0_UP_EVENT)
/*! @brief Define for ADC trigger down event */
#define FOC_PWMA0_ADC_TRIG_DN_EVENT     (DL_TIMERA_EVENT_CC0_DN_EVENT)

/*! @brief Instance for ADC Interrupt */
#define FOC_ADC_ISR_INST        (ADC0_INST)
/*! @brief Memory Load Register for ADC Interrupt */
#define FOC_ADC_MEM_RES_LOAD     DL_ADC12_IIDX_MEM1_RESULT_LOADED

/*! @brief ADC instance for DC bus voltage */
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
 * @brief     Reconstruct phase current from bus current
 * @param[in]  pCurrent Pointer to measure current structure
 * @param[in]  sector Voltage sector
 */
void HAL_currentReconstruction(HAL_MEASURE_CURRENT_T *pCurrent, MC_SECTOR_TYPES sector);

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
