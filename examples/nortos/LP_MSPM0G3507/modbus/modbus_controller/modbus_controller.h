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

#ifndef __MODBUS_CONTROLLER_H__
#define __MODBUS_CONTROLLER_H__

#include "ti/modbus/ModbusRTU.h"
#include "ti_msp_dl_config.h"
#include <stdint.h>
#include <string.h>

/*! MODBUS target address */
#define MODBUS_TARGET_ADDR              (0x12)

/*! MODBUS controller turnaround timer in microseconds */
#define MODBUS_TURNAROUND_TIMER         (85000)

/*! MODBUS controller response timeout in microseconds */
#define MODBUS_RESPONSE_TIMER           (90000)

/*! MODBUS controller number of tries */
#define MODBUS_NUMBER_OF_TRIES          (3)

/*! LED delay after response in microseconds */
#define MODBUS_LED_DELAY_IN_RSP         (32000000)

/*! LED delay in instruction in microseconds */
#define MODBUS_LED_DELAY_IN_INST        (3200000)

/*! LED delay on complete in microseconds */
#define MODBUS_LED_DELAY_ON_COMPLETE    (5000000)

/*! MODBUS timer frequency in Hz (typically 32.768 kHz for RTC) */
#define MODBUS_TIMER_FREQ               (32768)

/*! MODBUS UART baud rate (defined in MCU config) */
#define MODBUS_UART_BAUD_RATE           (UART_0_BAUD_RATE)

/*! MODBUS UART interrupt IRQ number */
#define MODBUS_UART_INT_IRQ             (UART_0_INST_INT_IRQN)

/*! MODBUS CPU clock frequency (defined in MCU config) */
#define MODBUS_CPU_CLK_FREQ             (CPUCLK_FREQ)

/*! MODBUS timer instance (defined in MCU config) */
#define MODBUS_TIMER_INST               (TIMER_0_INST)

/*! MODBUS UART instance (defined in MCU config) */
#define MODBUS_UART_INST                (UART_0_INST)

/*! User defined FC */
#define MODBUS_USER_DEFINED_FC          (65)

/*!
 * \brief Enumeration of all MODBUS controller test cases.
 */
typedef enum Tests
{
    /*! Write coil test case */
    WRITE_COIL = 0,
    /*! Read coil 1 test case */
    READ_COIL1,
    /*! Write multiple coil test case */
    WRITE_MULTIPLE_COIL,
    /*! Read coil 2 test case */
    READ_COIL2,
    /*! Read discrete input test case */
    READ_DISCRETE_IP,
    /*! Write holding register test case */
    WRITE_HOLDING_REG,
    /*! Read holding register 1 test case */
    READ_HOLDING_REG1,
    /*! Write multiple holding register test case */
    WRITE_MLP_HLD_REG,
    /*! Read holding register 2 test case */
    READ_HOLDING_REG2,
    /*! Read/write multiple holding register test case */
    RW_MLP_HLD_REG,
    /*! Mask write register test case */
    MASK_WRITE_REG,
    /*! Read input register test case */
    READ_INP_REG,
    /*! Read FIFO queue test case */
    READ_FIFO_QUEUE,
    /*! Write file test case */
    WRITE_FILE,
    /*! Read file test case */
    READ_FILE,
    /*! Read exception status test case */
    READ_EXCEPTION_STATUS,
    /*! Get communication event counter test case */
    GET_COMM_EVENT_COUNTER,
    /*! Get communication event log test case */
    GET_COM_EVENT_LOG,
    /*! Get target ID test case */
    GET_TARGET_ID,
    /*! Return query data test case */
    RET_QUERY_DATA,
    /*! Restart communication 1 test case */
    RESTART_COMM1,
    /*! Return diagnostic register test case */
    RET_DIAG_REGISTER,
    /*! Force listen only mode test case */
    FORCE_LISTEN_ONLY_MODE,
    /*! Restart communication 2 test case */
    RESTART_COMM2,
    /*! Clear counter and diagnostic register test case */
    CLR_CNTR_AND_DIAG_REG,
    /*! Bus message count test case */
    BUS_MSG_COUNT,
    /*! Bus communication error count test case */
    BUS_COMM_ERROR_CNT,
    /*! Exception read coil 1 test case */
    EXC_READ_COIL1,
    /*! Bus exception error count test case */
    BUS_EXP_ERROR_CNT,
    /*! Server message count test case */
    SRVR_MSG_COUNT,
    /*! Server no response count test case */
    SRVR_NO_RSP_CNT,
    /*! Server NAK count test case */
    SRVR_NAK_CNT,
    /*! Server busy count test case */
    SRVR_BUSY_CNT,
    /*! Bus character overrun count test case */
    BUS_CHR_OVRRN_CNT,
    /*! Clear overrun counter and flag test case */
    CLR_OVRRN_CNTR_AND_FLG,
    /*! User defined function code */
    USER_DEFINED_FC,
    /*! End of tests indicator */
    END_OF_TESTS
} Modbus_Tests;
#endif
