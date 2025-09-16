/* --COPYRIGHT--,BSD
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
 * --/COPYRIGHT--*/

#ifndef __MODBUS_TARGET_H__
#define __MODBUS_TARGET_H__

#include "ti/modbus/ModbusRTU.h"
#include "ti_msp_dl_config.h"

/*! ADC12 module bit resolution */
#define ADC12_BIT_RESOLUTION                   (12)

/*! ADC12 reference voltage in volts */
#define ADC12_REF_VOLTAGE                      (2.5)

/*! Voltage monitored by ADC12 in volts */
#define ADC12_SUPPLY_MONITOR_VOLTAGE           (2.5)

/*!
 * ADC12 supply monitor value.
 * Calculation: (2^BIT_RESOLUTION) * (MONITOR_VOLTAGE / (3 * REF_VOLTAGE))
 * Used for scaling ADC readings to supply voltage.
 */
#define ADC12_SUPPLY_MONITOR_VALUE             ((1 << ADC12_BIT_RESOLUTION) *   \
                                                (ADC12_SUPPLY_MONITOR_VOLTAGE / \
                                                (3 * ADC12_REF_VOLTAGE)))

/*! MODBUS target device address */
#define MODBUS_TARGET_ID                      (0x12)

/*! MODBUS coil register address */
#define MODBUS_COIL_ADDR                      (0x01)

/*! MODBUS discrete input register address */
#define MODBUS_DISCRETE_INPUT_ADDR            (0x01)

/*! MODBUS input register address */
#define MODBUS_INPUT_REG_ADDR                 (0x01)

/*! MODBUS holding register address */
#define MODBUS_HOLDING_REG_ADDR               (0x01)

/*! GPIO logical high value */
#define GPIO_HIGH                             (1)

/*! GPIO logical low value */
#define GPIO_LOW                              (0)

/*! MODBUS timer frequency in Hz (typically 32.768 kHz for RTC) */
#define MODBUS_TIMER_FREQ                     (32768)

/*! MODBUS UART baud rate (defined in MCU config) */
#define MODBUS_UART_BAUD_RATE                 (UART_0_BAUD_RATE)

/*! MODBUS UART interrupt IRQ number */
#define MODBUS_UART_INT_IRQ                   (UART_0_INST_INT_IRQN)

/*! MODBUS CPU clock frequency (defined in MCU config) */
#define MODBUS_CPU_CLK_FREQ                   (CPUCLK_FREQ)

/*! MODBUS timer instance (defined in MCU config) */
#define MODBUS_TIMER_INST                     (TIMER_0_INST)

/*! MODBUS UART instance (defined in MCU config) */
#define MODBUS_UART_INST                      (UART_0_INST)

/*! Status value for server ON in Report Server ID response (as per Standard) */
#define MODBUS_TARGET_STATUS                  (MODBUS_REPORT_SRVR_ID_STATUS_ON)

/*! MODBUS write timeout in microseconds */
#define MODBUS_WRITE_TIMEOUT_TIME             (32000000)

/*! MODBUS user function code */
#define MODBUS_USER_FUNCTION_CODE             (65)

#endif
