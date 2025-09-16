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

#ifndef __MODBUS_RTU_H__
#define __MODBUS_RTU_H__
#include <ti/driverlib/driverlib.h>

/*! Status code for successful device initialization */
#define MODBUS_TARGET_DEVICE_INIT_OK        (1)

/*! Status code for failed device initialization */
#define MODBUS_TARGET_DEVICE_INIT_ERR       (0)

/*! Return value for successful function code registration */
#define MODBUS_USER_FUNCTION_INIT_OK        (1)

/*! Return value for failed function code registration */
#define MODBUS_USER_FUNCTION_INIT_ERR       (0)

/*! MODBUS broadcast address (as per Standard) */
#define MODBUS_BROADCAST_ADDR               (0x00)

/*! Maximum number of MODBUS registers supported(0-254) (Standard says 65536) */
#define MODBUS_MAX_NUM_OF_REGS              (255)

/*! Maximum Protocol Data Unit (PDU) size in bytes (as per Standard) */
#define MODBUS_MAX_PDU_SIZE                 (253)

/*! Maximum Application Data Unit (ADU) size in bytes (as per Standard) */
#define MODBUS_MAX_ADU_SIZE                 (256)

/*! Minimum Application Data Unit (ADU) size in bytes */
#define MODBUS_MIN_ADU_SIZE                 (4)

/*! Device type for coils (outputs): 0x00 */
#define MODBUS_COILS                        (0x00)

/*! Device type for discrete inputs: 0x01 */
#define MODBUS_DISCRETE_INPUTS              (0x01)

/*! Device type for input registers: 0x02 */
#define MODBUS_INPUT_REG                    (0x02)

/*! Device type for holding registers: 0x03 */
#define MODBUS_HOLDING_REG                  (0x03)

/*! Operation type for reading registers: 0x00 */
#define MODBUS_READ                         (0x00)

/*! Operation type for writing registers: 0x01 */
#define MODBUS_WRITE                        (0x01)

/*! State: MODBUS is idle/waiting for activity */
#define MODBUS_IDLE                         (0x00)

/*! State: MODBUS is receiving a frame */
#define MODBUS_RECEIVING                    (0x01)

/*! State: MODBUS is processing the received frame */
#define MODBUS_PROCESSING                   (0x02)

/*! State: MODBUS is sending a frame */
#define MODBUS_SENDING                      (0x03)

/*! No exception (0x00). Normal response. */
#define MODBUS_NO_EXCEPTION                 (0x00)

/*! Exception (standard): Illegal function code received (0x01). Function not supported. */
#define MODBUS_ILLEGAL_FUNCTION             (0x01)

/*! Exception (standard): Illegal data address (0x02). Address not available. */
#define MODBUS_ILLEGAL_DATA_ADDRESS         (0x02)

/*! Exception (standard): Illegal data value (0x03). Value not allowed. */
#define MODBUS_ILLEGAL_DATA_VALUE           (0x03)

/*! Exception (standard): Server device failure (0x04). Unrecoverable error. */
#define MODBUS_SERVER_DEVICE_FAILURE        (0x04)

/*! Exception (standard): Acknowledge (0x05). Request accepted, processing delayed. */
#define MODBUS_ACKNOWLEDGE                  (0x05)

/*! Exception (standard): Server device busy (0x06). Try again later. */
#define MODBUS_SERVER_DEVICE_BUSY           (0x06)

/*! Exception (standard): Server NAK (0x07). Negative acknowledgment. */
#define MODBUS_SERVER_NAK                   (0x07)

/*! Exception (standard): Memory parity error (0x08). */
#define MODBUS_MEMORY_PARITY_ERROR          (0x08)

/*! Exception (standard): Gateway path unavailable (0x0A). */
#define MODBUS_GW_PATH_UNAVAILABLE          (0x0A)

/*! Exception (standard): Gateway target device failed to respond (0x0B). */
#define MODBUS_GW_TD_UNRESPONSIVE           (0x0B)

/*! Response code indicating a CRC error detected in target response */
#define MODBUS_CRC_ERROR_IN_TARGET_RES      (0xFD)

/*! Response code indicating invalid target response */
#define MODBUS_INV_TARGET_RES               (0xFE)

/*! Response code indicating no target response */
#define MODBUS_NO_TARGET_RES                (0xFF)

/*! Status value for server ON in Report Server ID response (as per Standard) */
#define MODBUS_REPORT_SRVR_ID_STATUS_ON     (0xFF)

/*! Status value for server OFF in Report Server ID response (as per Standard) */
#define MODBUS_REPORT_SRVR_ID_STATUS_OFF    (0x00)

/*!
 * \brief Enum representing standard Modbus diagnostic sub-function codes.
 * Each enumerator corresponds to a specific Modbus diagnostic sub-function, as defined by the protocol.
 */
typedef enum {
    /*! Diagnostic sub-function (standard): Return Query Data (0x0000) */
    MODBUS_RETURN_QUERY_DATA = 0x0000,

    /*! Diagnostic sub-function (standard): Restart Communications Option (0x0001) */
    MODBUS_RESTART_COMM_OPTION = 0x0001,

    /*! Diagnostic sub-function (standard): Return Diagnostic Register (0x0002) */
    MODBUS_RETURN_DIAG_REG = 0x0002,

    /*! Diagnostic sub-function (standard): Change ASCII Input Delimiter (0x0003) */
    MODBUS_CHANGE_ASCII_IP_DEL = 0x0003,

    /*! Diagnostic sub-function (standard): Force Listen Only Mode (0x0004) */
    MODBUS_FORCE_LISTEN_ONLY_MODE = 0x0004,

    /*! Diagnostic sub-function (standard): Clear Counters and Diagnostic Register (0x000A) */
    MODBUS_CLR_CNTR_AND_DIAG_REG = 0x000A,

    /*! Diagnostic sub-function (standard): Return Bus Message Count (0x000B) */
    MODBUS_RETURN_BUS_MSG_COUNT = 0x000B,

    /*! Diagnostic sub-function (standard): Return Bus Communication Error Count (0x000C) */
    MODBUS_RETURN_BUS_COMM_ERR_CNT = 0x000C,

    /*! Diagnostic sub-function (standard): Return Bus Exception Error Count (0x000D) */
    MODBUS_RETURN_BUS_EXP_ERR_CNT = 0x000D,

    /*! Diagnostic sub-function (standard): Return Server Message Count (0x000E) */
    MODBUS_RETURN_SRVR_MSG_CNT = 0x000E,

    /*! Diagnostic sub-function (standard): Return Server No Response Count (0x000F) */
    MODBUS_RETURN_SRVR_NO_RSP_CNT = 0x000F,

    /*! Diagnostic sub-function (standard): Return Server NAK Count (0x0010) */
    MODBUS_RETURN_SRVR_NAK_CNT = 0x0010,

    /*! Diagnostic sub-function (standard): Return Server Busy Count (0x0011) */
    MODBUS_RETURN_SRVR_BUSY_CNT = 0x0011,

    /*! Diagnostic sub-function (standard): Return Bus Character Overrun Count (0x0012) */
    MODBUS_RETURN_BUS_CHR_OVRRN_CNT = 0x0012,

    /*! Diagnostic sub-function (standard): Clear Overrun Counter and Flag (0x0014) */
    MODBUS_CLR_OVRRN_CNTR_AND_FLG = 0x0014

} MODBUS_DiagnosticSubFunctionCode;

/*!
 * \enum MODBUS_FunctionCode
 * \brief Enum representing standard Modbus function codes.
 * Each enumerator corresponds to a specific Modbus function, as defined by the protocol.
 */
typedef enum {
    /*! Function code (standard): Read Discrete Inputs (0x02) */
    MODBUS_READ_DISCRETE_INPUTS = 0x02,

    /*! Function code (standard): Read Coils (0x01) */
    MODBUS_READ_COILS = 0x01,

    /*! Function code (standard): Write Single Coil (0x05) */
    MODBUS_WRITE_SINGLE_COIL = 0x05,

    /*! Function code (standard): Write Multiple Coils (0x0F) */
    MODBUS_WRITE_MULTIPLE_COILS = 0x0F,

    /*! Function code (standard): Read Input Registers (0x04) */
    MODBUS_READ_INPUT_REGISTERS = 0x04,

    /*! Function code (standard): Read Holding Registers (0x03) */
    MODBUS_READ_HOLDING_REGISTERS = 0x03,

    /*! Function code (standard): Write Single Register (0x06) */
    MODBUS_WRITE_SINGLE_REGISTER = 0x06,

    /*! Function code (standard): Write Multiple Registers (0x10) */
    MODBUS_WRITE_MULTIPLE_REGISTERS = 0x10,

    /*! Function code (standard): Read/Write Multiple Registers (0x17) */
    MODBUS_RW_MULTIPLE_REGISTERS = 0x17,

    /*! Function code (standard): Mask Write Register (0x16) */
    MODBUS_MASK_WRITE_REGISTER = 0x16,

    /*! Function code (standard): Read FIFO Queue (0x18) */
    MODBUS_READ_FIFO_QUEUE = 0x18,

    /*! Function code (standard): Read File Record (0x14) */
    MODBUS_READ_FILE_RECORDS = 0x14,

    /*! Function code (standard): Write File Record (0x15) */
    MODBUS_WRITE_FILE_RECORDS = 0x15,

    /*! Function code (standard): Read Exception Status (0x07) */
    MODBUS_READ_EXCEPTION_STATUS = 0x07,

    /*! Function code (standard): Diagnostics (0x08) */
    MODBUS_DIAGNOSTICS = 0x08,

    /*! Function code (standard): Get Communication Event Counter (0x0B) */
    MODBUS_GET_COM_EVENT_COUNTER = 0x0B,

    /*! Function code (standard): Get Communication Event Log (0x0C) */
    MODBUS_GET_COM_EVENT_LOGS = 0x0C,

    /*! Function code (standard): Report Server ID (0x11) */
    MODBUS_REPORT_SERVER_ID = 0x11,

    /*! Function code (standard): Miscellaneous (0x2B) - Encapsulated Interface Transport etc. */
    MODBUS_MISC = 0x2B

} MODBUS_FunctionCode;

/*!
 * \brief Initialize MODBUS target parameters and state.
 * \param mbTimer Pointer to timer peripheral registers.
 * \param mbUart Pointer to UART peripheral registers.
 */
extern void Modbus_configTarget(GPTIMER_Regs *mbTimer, UART_Regs *mbUart);

/*!
 * \brief Initialize MODBUS controller parameters and state.
 * \param mbTimer Pointer to timer peripheral registers.
 * \param mbUart Pointer to UART peripheral registers.
 */
extern void Modbus_configController(GPTIMER_Regs *mbTimer, UART_Regs *mbUart);

/*!
 * \brief Set the MODBUS target address.
 * \param targetId Target address (1-247).
 */
extern void Modbus_setTargetID(uint8_t targetId);

/*!
 * \brief Process and handle received MODBUS frame in target mode.
 *        Should be called from main loop.
 */
extern void Modbus_processFunctionCode(void);

/*!
 * \brief Send a MODBUS function code request as controller.
 * \param targetId Target address.
 * \param functionCode MODBUS function code to send.
 * \param data Pointer to data buffer for request.
 * \param dataLen Length of data buffer.
 * \param exceptionCode Pointer to variable for receiving exception code (if any).
 */
extern void Modbus_sendCommand(uint8_t targetId, uint8_t functionCode, uint8_t *data, uint8_t dataLen, uint8_t *exceptionCode);

/*!
 * \brief MODBUS UART interrupt handler. Handles RX/TX and error events.
 *        Should be called from UART ISR.
 */
extern void Modbus_IRQHandler(void);

/*!
 * \brief Register a callback function for a specific device register.
 * \param add Device address (1-255).
 * \param type Device type.
 * \param func Callback function pointer (prototype: void func(uint16_t *data, uint8_t op)).
 * \return MODBUS_TARGET_DEVICE_INIT_OK on success, MODBUS_TARGET_DEVICE_INIT_ERR on error.
 */
extern uint8_t Modbus_configDevice(uint8_t add, uint8_t type, void (*func)(uint16_t *data, uint8_t op));


/*!
 * \brief Set the MODBUS write timeout value.
 *
 * This function sets the write timeout value (in microseconds). The value is converted
 * to timer ticks based on the current timer frequency and stored in the MODBUS parameters.
 *
 * \param timer Timeout value in microseconds.
 */
extern void Modbus_setWriteTimeoutTimer(uint32_t timer);

/*!
 * \brief Set the number of retry attempts for MODBUS operations.
 *
 * This function sets the number of tries for a MODBUS transaction. If the provided value
 * is less than the default, the default is used instead.
 *
 * \param tries Desired number of tries.
 */
extern void Modbus_setNumberOfTries(uint8_t tries);

/*!
 * \brief Set the controller mode turnaround timer value.
 * \param timer Timer value in microseconds.
 */
extern void Modbus_setControllerTurnAroundTimer(uint32_t timer);

/*!
 * \brief Set the controller mode response timeout value.
 * \param timer Timer value in microseconds.
 */
extern void Modbus_setControllerRspTimer(uint32_t timer);

/*!
 * \brief Register a callback function to handle restart communication events.
 * \param func Function pointer to restart routine (prototype: void func(void)).
 */
extern void Modbus_configRestartComm(void (*func)(void));

/*!
 * \brief Set the MODBUS communication baud rate used for MODBUS timing calculations.
 * \param baud Desired baud rate (e.g., 9600, 19200).
 */
extern void Modbus_setUartBaudRate(uint32_t baud);

/*!
 * \brief Set the timer clock frequency used for MODBUS timing calculations.
 * \param tFreq Timer frequency in Hz as configured via SysConfig.
 */
extern void Modbus_setTimerClkFreq(uint32_t tFreq);

/*!
 * \brief Set the CPU clock frequency used for MODBUS timing calculations.
 * \param cpuFreq CPU frequency in Hz.
 */
extern void Modbus_setCpuClkFreq(uint32_t cpuFreq);

/*!
 * \brief Initialize the MODBUS module.
 *
 * This function must be called once before using any MODBUS functions.
 * It sets up all necessary hardware and software resources for MODBUS operations.
 */
extern void Modbus_init(void);

/*!
 * \brief Set status of target/server.
 * \param status server status.
 *
 * Stores the server status in the global MODBUS_params structure.
 */
extern void Modbus_setServerStatus(uint8_t status);

/*!
 * \brief Register a callback function to handle user defined function call.
 * \param func Function pointer to function callback routine (prototype: void func(void)).
 * \param userFunctionCode user defined function code.
 * \return MODBUS_USER_FUNCTION_INIT_OK on success, MODBUS_USER_FUNCTION_INIT_ERR on error.
 */
extern uint8_t Modbus_configUserFunctionCode(void (*func)(void),uint8_t userFunctionCode);

#endif
