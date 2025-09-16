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

#ifndef __MODBUS_RTU_PRIV_H__
#define __MODBUS_RTU_PRIV_H__
#include <stdint.h>
#include <string.h>
#include "ModbusRTU.h"

/*! Default target ID - 1*/
#define MODBUS_MIN_TARGET_ID                (0x01)

/*! Maximum target ID - 247*/
#define MODBUS_MAX_TARGET_ID                (0xF7)

/*! Valid function code return code*/
#define MODBUS_VALID_FUNCTION_CODE          (0)

/*! Invalid function code return code*/
#define MODBUS_INVALID_FUNCTION_CODE        (1)

/*! Number of user defined function calls*/
#define MODBUS_NUM_OF_USER_DEFINED_FC       (0x13)

/*! Lower limit of first user-defined function code set (Modbus spec: 65-72 recommended for user-defined) */
#define MODBUS_USER_DEFINED_FC_SET1_LOWER_LIMIT (65)

/*! Upper limit of first user-defined function code set */
#define MODBUS_USER_DEFINED_FC_SET1_UPPER_LIMIT (72)

/*! Lower limit of second user-defined function code set (Modbus spec: 100-110 often used for user-defined) */
#define MODBUS_USER_DEFINED_FC_SET2_LOWER_LIMIT (100)

/*! Upper limit of second user-defined function code set */
#define MODBUS_USER_DEFINED_FC_SET2_UPPER_LIMIT (110)

/*! Number of function codes in set 1 */
#define MODBUS_NUM_OF_FC_IN_SET1            (MODBUS_USER_DEFINED_FC_SET1_UPPER_LIMIT - MODBUS_USER_DEFINED_FC_SET1_LOWER_LIMIT + 1)

/*! Minimum target ID - 1*/
#define MODBUS_DEFAULT_TARGET_ID            (0x01)

/*! Default register value */
#define MODBUS_DEFAULT_REG_VAL              (0x00)

/*! Default value for current time */
#define MODBUS_DEFAULT_CURR_TIME            (0x00)

/*! Default 1 try should be done by controller */
#define MODBUS_DEFAULT_TRIES                (0x01)

/*! Default value for last time */
#define MODBUS_DEFAULT_LAST_TIME            (0x00)

/*! Default value for 3.5 character time */
#define MODBUS_DEFAULT_CT3_5                (1750)

/*! Modbus Baud rate value 19200 */
#define MODBUS_BAUD19200                    (19200)

/*! Default value for 1.5 character time */
#define MODBUS_DEFAULT_CT1_5                (750)

/*! Default value for CPU frequency */
#define MODBUS_DEFAULT_CPU_FREQ             (0x00)

/*! Default value for timer frequency */
#define MODBUS_DEFAULT_TFREQ                (0x00)

/*! Default value for baud rate */
#define MODBUS_DEFAULT_BAUD                 (9600)

/*! Default value for turnaround timer */
#define MODBUS_DEFAULT_TURNAROUND_TIMER     (0x00)

/*! Default value for response timer */
#define MODBUS_DEFAULT_RESPONSE_TIMER       (0x00)

/*! Default value for write timeout */
#define MODBUS_DEFAULT_WRITE_TIMEOUT_VAL    (0x00)

/*! Default value for exception status */
#define MODBUS_DEFAULT_EXCEPTION_STATUS     (0x00)

/*! Default value for event counter */
#define MODBUS_DEFAULT_EVENT_COUNTER        (0x00)

/*! Default value for message detected counter */
#define MODBUS_DEFAULT_MSG_DETECTED_CNTR    (0x00)

/*! Default value for message processed counter */
#define MODBUS_DEFAULT_MSG_PROCESSED_CNTR   (0x00)

/*! Default value for CRC error counter */
#define MODBUS_DEFAULT_CRC_ERROR_COUNTER    (0x00)

/*! Default value for exception counter */
#define MODBUS_DEFAULT_EXCEPTION_COUNTER    (0x00)

/*! Default value for no response counter */
#define MODBUS_DEFAULT_NO_RESP_COUNTER      (0x00)

/*! Default value for diagnostics register */
#define MODBUS_DEFAULT_DIAG_REG             (0x00)

/*! Default value for server busy count */
#define MODBUS_DEFAULT_SERVER_BUSY_COUNT    (0x00)

/*! Default value for server NAK count */
#define MODBUS_DEFAULT_SERVER_NAK_COUNT     (0x00)

/*! Default value for character overrun count */
#define MODBUS_DEFAULT_CHARACTER_OVRN_COUNT (0x00)

/*! Default value for character overrun flag */
#define MODBUS_DEFAULT_CHARACTER_OVRN_FLAG  (0x00)

/*! Default Rx/Tx packet value */
#define MODBUS_DEFAULT_PKT_VAL              (0x00)

/*! Default Rx/Tx counts */
#define MODBUS_DEFAULT_CNT_VAL              (0x00)

/*! Default File Record value */
#define MODBUS_DEFAULT_FILE_RCRD_VAL        (0x00)

/*! Number of MODBUS register types (as per Standard) */
#define MODBUS_NUM_OF_REG_TYPES             (4)

/*! Number of bytes to be allocated for coil outputs (255/8) = 32 */
#define MODBUS_SIZE_OF_COILS                (32)

/*! Number of bytes to be allocated for discrete inputs (255/8) = 32 */
#define MODBUS_SIZE_OF_DISCRETE_INPUTS      (32)

/*! Number of bytes to be allocated for input registers (255*2) = 510) */
#define MODBUS_SIZE_OF_INPUT_REGS           (510)

/*! Number of bytes to be allocated for holding registers (255*2) = 510) */
#define MODBUS_SIZE_OF_HOLDING_REGS         (510)

/*! Default value for FIFO buffer */
#define MODBUS_FIFO_DEFAULT_VAL             (0x0000)

/*! Default value for event log initialization */
#define MODBUS_EVENTLOG_DEFAULT_VAL         (0xFF)

#ifndef __MSPM0_HAS_CRCP__
/*! Initial value for CRC in software CRC calculation */
#define MODBUS_INITIAL_CRC_VAL              (0xFF)
#endif

/*! Microseconds in one second */
#define MODBUS_US_IN_SECS                   (1000000.0)

/*! Number of bits in a MODBUS RTU character */
#define MODBUS_BITS_IN_CHAR                 (11)

/*! Enable MODBUS controller mode */
#define MODBUS_CONTROLLER_ENABLE            (1)

/*! Disable MODBUS controller mode */
#define MODBUS_CONTROLLER_DISABLE           (0)

/*! Minimum device address allowed */
#define MODBUS_MIN_DEVICE_ADDR              (1)

/*! MODBUS TX data sub-function code index High */
#define MODBUS_TX_DATA_SUB_FC_IDX_HI        (0)

/*! MODBUS TX data sub-function code index Low */
#define MODBUS_TX_DATA_SUB_FC_IDX_LO        (1)

/*! MODBUS TX data sub-function data index High */
#define MODBUS_TX_DATA_SUB_FC_DATA_HI       (2)

/*! MODBUS TX data sub-function data index Low */
#define MODBUS_TX_DATA_SUB_FC_DATA_LO       (3)

/*! Maximum device address allowed */
#define MODBUS_MAX_DEVICE_ADDR              (255)

/*! Listen only mode active */
#define MODBUS_LISTEN_ONLY_MODE_ACTIVE      (1)

/*! Listen only mode inactive */
#define MODBUS_LISTEN_ONLY_MODE_INACTIVE    (0)

/*! Number of bytes in MODBUS RTU CRC field (as per Standard) */
#define MODBUS_RTU_CRC_LEN                  (2)

/*! Target address field index (as per Standard) */
#define MODBUS_TARGET_ADDR_IDX              (0)

/*! Function code field index (as per Standard) */
#define MODBUS_FUNCTION_CODE_IDX            (1)

/*! Exception code field index  (as per Standard)*/
#define MODBUS_EXCEPTION_CODE_IDX           (2)

/*! Difference in bytes between ADU and PDU (as per Standard) */
#define MODBUS_PDU_ADU_BYTE_DIFF            (3)

/*! Exception mask for function codes (as per Standard) */
#define MODBUS_EXCEPTION_MASK               (0x80)

/*! Mask to get lower byte from 16-bit value */
#define MODBUS_16BITMASK_TO_GET_LAST_BYTE   (0x00FF)

/*! Number of MODBUS file records supported (Standard says from 0x0001 to 0xFFFF)*/
#define MODBUS_NUM_OF_FILES                 (1)

/*! Number of records per MODBUS file (Standard says 100000)*/
#define MODBUS_RECORDS_PER_FILE             (10)

/*! Maximum number of events in event log (as per Standard) */
#define MODBUS_MAX_EVENT_CNT                (64)

/*! Maximum number of elements in FIFO queue (as per Standard) */
#define MODBUS_MAX_QUEUE_SIZE               (32)

/* Event Byte Mask: Used to set the bit in event byte to indicate MODBUS frame received (any type). */
#define MODBUS_EVNT_RCV                     (0x80)

/* Event Byte Mask: Used to set the bit in event byte to indicate broadcast frame received (no response sent). */
#define MODBUS_EVNT_RCV_BRDCST              (0x40)

/* Event Byte Mask: Used to set the bit in event byte to indicate frame received in Listen-Only mode (no response sent). */
#define MODBUS_EVNT_RCV_LSTN_ONLY_MD        (0x20)

/* Event Byte Mask: Used to set the bit in event byte to indicate character overrun on receive (UART buffer overflow). */
#define MODBUS_EVNT_RCV_CHRCTR_OVERRUN      (0x10)

/* Event Byte Mask: Used to set the bit in event byte to indicate communication error on receive (framing/parity/CRC). */
#define MODBUS_EVNT_RCV_COMM_ERR            (0x02)

/* Event Byte Mask: Used to set the bit in event byte to indicate MODBUS frame sent. */
#define MODBUS_EVNT_SND                     (0x40)

/* Event Byte Mask: Used to set the bit in event byte to indicate frame sent in Listen-Only mode. */
#define MODBUS_EVNT_SND_LSTN_ONLY_MD        (0x20)

/* Event Byte Mask: Used to set the bit in event byte to indicate write timeout occurred on send (no response/acknowledgement). */
#define MODBUS_EVNT_SND_WRT_TIMEOUT         (0x10)

/* Event Byte Mask: Used to set the bit in event byte to indicate server sent NAK (negative acknowledgment). */
#define MODBUS_EVNT_SND_SERVER_NAK          (0x08)

/* Event Byte Mask: Used to set the bit in event byte to indicate server busy response sent. */
#define MODBUS_EVNT_SND_SERVER_BUSY         (0x04)

/* Event Byte Mask: Used to set the bit in event byte to indicate server abort event sent. */
#define MODBUS_EVNT_SND_SERVER_ABORT        (0x02)

/* Event Byte Mask: Used to set the bit in event byte to indicate exception response sent (read). */
#define MODBUS_EVNT_SND_READ_EXP            (0x01)

/* Event Byte Mask: Used to set the bit in event byte to indicate Listen-Only mode active. */
#define MODBUS_EVNT_LSTN_ONLY_MD            (0x04)

/* Event Byte Mask: Used to set the bit in event byte to indicate communication restart occurred (counters/logs may reset). */
#define MODBUS_EVNT_COMM_RESTART            (0x00)

/*! High byte of target address */
#define MODBUS_RX_PACKET_ADDR_HI_IDX        (2)

/*! Low byte of target address */
#define MODBUS_RX_PACKET_ADDR_LO_IDX        (3)

/*! High byte of quantity field */
#define MODBUS_RX_PACKET_QUANTITY_HI_IDX    (4)

/*! Low byte of quantity field */
#define MODBUS_RX_PACKET_QUANTITY_LO_IDX    (5)

/*! Byte count field index in TX packet */
#define MODBUS_TX_PACKET_BYTE_CNT_IDX       (2)

/*! Offset for byte count in transmit packets */
#define MODBUS_TX_PACKET_BYTE_CNT_OFFSET    (3)

/*! Minimum packet size for Discrete Input read */
#define MODBUS_DI_MIN_PKT_CNT               (8)

/*! Minimum number of discrete inputs to read (as per Standard)*/
#define MODBUS_DI_MIN_READ_CNT              (0x0001)

/*! Maximum number of discrete inputs to read (as per Standard)*/
#define MODBUS_DI_MAX_READ_CNT              (0x07D0)

/*! Minimum packet size for Read Coil request */
#define MODBUS_READ_COIL_MIN_PKT_CNT        (8)

/*! Minimum number of coils to read (as per Standard)*/
#define MODBUS_READ_COIL_MIN_READ_CNT       (0x0001)

/*! Maximum number of coils to read (as per Standard)*/
#define MODBUS_READ_COIL_MAX_READ_CNT       (0x07D0)

/*! Packet size for Write Single Coil */
#define MODBUS_WRITE_SNGL_COIL_PKT_CNT      (8)

/*! Value to set a coil ON (as per Standard)*/
#define MODBUS_WRITE_COIL_SET_BIT           (0xFF00)

/*! Value to set a coil OFF (as per Standard)*/
#define MODBUS_WRITE_COIL_CLEAR_BIT         (0x0000)

/*! Minimum packet size for Write Multiple Coils request */
#define MODBUS_WRITE_MLP_COIL_MIN_PKT_CNT   (10)

/*! Starting index of data bits Write Multiple Coils request */
#define MODBUS_WRITE_MLP_COIL_DATA_IDX      (7)

/*! Index of byte count field in Write Multiple Coils request */
#define MODBUS_WRITE_MLP_COIL_BYTE_CNT_IDX  (6)

/*! Minimum number of coils to write in one request (as per Standard)*/
#define MODBUS_WRITE_MLP_COIL_MIN_WRT_CNT   (0x0001)

/*! Maximum number of coils to write in one request (as per Standard)*/
#define MODBUS_WRITE_MLP_COIL_MAX_WRT_CNT   (0x07B0)

/*! Packet size for Write Multiple Coils response */
#define MODBUS_WRITE_MLP_COIL_RSP_CNT       (6)

/*! Minimum packet size for Read Input Register request */
#define MODBUS_READ_IP_REG_MIN_PKT_CNT      (8)

/*! Minimum number of input registers to read (as per Standard)*/
#define MODBUS_READ_IP_REG_MIN_READ_CNT     (0x0001)

/*! Maximum number of input registers to read (as per Standard)*/
#define MODBUS_READ_IP_REG_MAX_READ_CNT     (0x007D)

/*! Minimum packet size for Read Holding Register request */
#define MODBUS_READ_HLD_REG_MIN_PKT_CNT     (8)

/*! Minimum number of holding registers to read (as per Standard)*/
#define MODBUS_READ_HLD_REG_MIN_READ_CNT    (0x0001)

/*! Maximum number of holding registers to read (as per Standard)*/
#define MODBUS_READ_HLD_REG_MAX_READ_CNT    (0x007D)

/*! Packet size for Write Single Register */
#define MODBUS_WRITE_SNGL_REG_PKT_CNT       (8)

/*! Index of high byte of register value in Write Single Register */
#define MODBUS_WRITE_SNGL_REG_VAL_HI_IDX    (4)

/*! Index of low byte of register value in Write Single Register */
#define MODBUS_WRITE_SNGL_REG_VAL_LO_IDX    (5)

/*! Minimum register value for Write Single Register (as per Standard)*/
#define MODBUS_WRITE_SNGL_REG_MIN_VAL       (0x0000)

/*! Maximum register value for Write Single Register (as per Standard)*/
#define MODBUS_WRITE_SNGL_REG_MAX_VAL       (0xFFFF)

/*! Minimum packet size for Write Multiple Registers request */
#define MODBUS_WRITE_MLP_REG_MIN_PKT_CNT    (9)

/*! Starting index of data bytes for Write Multiple Registers request */
#define MODBUS_WRITE_MLP_REG_DATA_IDX       (7)

/*! Index of byte count field in Write Multiple Registers request */
#define MODBUS_WRITE_MLP_REG_BYTE_CNT_IDX   (6)

/*! Minimum number of registers to write in one request (as per Standard)*/
#define MODBUS_WRITE_MLP_REG_MIN_WRITE_CNT  (0x0001)

/*! Maximum number of registers to write in one request (as per Standard)*/
#define MODBUS_WRITE_MLP_REG_MAX_WRITE_CNT  (0x007B)

/*! Packet size for Write Multiple Registers response */
#define MODBUS_WRITE_MLP_REG_RSP_CNT        (6)

/*! Minimum packet size for Read/Write Multiple Registers request */
#define MODBUS_RW_MLP_REG_MIN_PKT_CNT       (13)

/*! Index of high byte of read starting address */
#define MODBUS_RW_MLP_REG_READ_ADD_IDX_HI   (2)

/*! Index of low byte of read starting address */
#define MODBUS_RW_MLP_REG_READ_ADD_IDX_LO   (3)

/*! Index of high byte of read quantity */
#define MODBUS_RW_MLP_REG_READ_QTY_IDX_HI   (4)

/*! Index of low byte of read quantity */
#define MODBUS_RW_MLP_REG_READ_QTY_IDX_LO   (5)

/*! Minimum number of registers to read in one request (as per Standard) */
#define MODBUS_RW_MLP_REG_MIN_READ_CNT      (0x0001)

/*! Maximum number of registers to read in one request (as per Standard) */
#define MODBUS_RW_MLP_REG_MAX_READ_CNT      (0x007D)

/*! Index of high byte of write starting address (as per Standard) */
#define MODBUS_RW_MLP_REG_WRITE_ADD_IDX_HI  (6)

/*! Index of low byte of write starting address (as per Standard) */
#define MODBUS_RW_MLP_REG_WRITE_ADD_IDX_LO  (7)

/*! Index of high byte of write quantity */
#define MODBUS_RW_MLP_REG_WRITE_QTY_IDX_HI  (8)

/*! Index of low byte of write quantity */
#define MODBUS_RW_MLP_REG_WRITE_QTY_IDX_LO  (9)

/*! Minimum number of registers to write in one request (as per Standard) */
#define MODBUS_RW_MLP_REG_MIN_WRITE_CNT     (0x0001)

/*! Maximum number of registers to write in one request (as per Standard) */
#define MODBUS_RW_MLP_REG_MAX_WRITE_CNT     (0x0079)

/*! Index of byte count field in write data section */
#define MODBUS_RW_MLP_REG_WRT_BYTCNT_IDX    (10)

/*! Offset to start of write data in packet */
#define MODBUS_RW_MLP_REG_WRT_DATA_OFFSET   (11)

/*! Packet size for Mask Write Register request/response */
#define MODBUS_MASK_WRITE_REG_PKT_CNT       (10)

/*! Index of high byte of AND mask in packet */
#define MODBUS_MASK_WRT_REG_AND_MASK_IDX_HI (4)

/*! Index of low byte of AND mask in packet */
#define MODBUS_MASK_WRT_REG_AND_MASK_IDX_LO (5)

/*! Index of high byte of OR mask in packet */
#define MODBUS_MASK_WRT_REG_OR_MASK_IDX_HI  (6)

/*! Index of low byte of OR mask in packet */
#define MODBUS_MASK_WRT_REG_OR_MASK_IDX_LO  (7)

/*! Valid address value for read queue operations (since having one queue only)*/
#define MODBUS_QUEUE_VALID_ADDR             (0x0000)

/*! Minimum packet size for Read File Record request */
#define MODBUS_RFILE_RCRD_MIN_PKT_CNT       (12)

/*! Index of byte count field in Read File Record request */
#define MODBUS_RFILE_RCRD_BYT_CNT_IDX       (2)

/*! Maximum byte count for Read File Record request (as per Standard) */
#define MODBUS_RFILE_RCRD_MAX_BYT_CNT       (0xF5)

/*! Minimum byte count for Read File Record request */
#define MODBUS_RFILE_RCRD_MIN_BYT_CNT       (0x07)

/*! Index of reference type field in Read File Record request */
#define MODBUS_RFILE_REF_IDX                (3)

/*! Index of high byte of file number in Read File Record request */
#define MODBUS_RFILE_FILE_IDX_HI            (4)

/*! Index of low byte of file number in Read File Record request */
#define MODBUS_RFILE_FILE_IDX_LO            (5)

/*! Index of high byte of record number in Read File Record request */
#define MODBUS_RFILE_RCRD_IDX_HI            (6)

/*! Index of low byte of record number in Read File Record request */
#define MODBUS_RFILE_RCRD_IDX_LO            (7)

/*! Index of high byte of record length in Read File Record request */
#define MODBUS_RFILE_RCRD_LEN_HI            (8)

/*! Index of low byte of record length in Read File Record request */
#define MODBUS_RFILE_RCRD_LEN_LO            (9)

/*! Reference type field value for Read File Record request (as per Standard) */
#define MODBUS_RFILE_REF_NO                 (0x06)

/*! Valid file address for file record operations */
#define MODBUS_RFILE_VALID_FILE_ADDR        (0x0000)

/*! Number of bytes for each file record request structure */
#define MODBUS_RFILE_EACH_RQST_BYT_CNT      (7)

/*! Minimum packet size for Write File Record request */
#define MODBUS_WFILE_RCRD_MIN_PKT_CNT       (12)

/*! Index of byte count field in Write File Record request */
#define MODBUS_WFILE_RCRD_BYT_CNT_IDX       (2)

/*! Maximum byte count for Write File Record request (as per Standard) */
#define MODBUS_WFILE_RCRD_MAX_BYT_CNT       (0xF5)

/*! Minimum byte count for Write File Record request */
#define MODBUS_WFILE_RCRD_MIN_BYT_CNT       (0x07)

/*! Index of reference type field in Write File Record request */
#define MODBUS_WFILE_REF_IDX                (3)

/*! Index of high byte of file number in Write File Record request */
#define MODBUS_WFILE_FILE_IDX_HI            (4)

/*! Index of low byte of file number in Write File Record request */
#define MODBUS_WFILE_FILE_IDX_LO            (5)

/*! Index of high byte of record number in Write File Record request */
#define MODBUS_WFILE_RCRD_IDX_HI            (6)

/*! Index of low byte of record number in Write File Record request */
#define MODBUS_WFILE_RCRD_IDX_LO            (7)

/*! Index of high byte of record length in Write File Record request */
#define MODBUS_WFILE_RCRD_LEN_HI            (8)

/*! Index of low byte of record length in Write File Record request */
#define MODBUS_WFILE_RCRD_LEN_LO            (9)

/*! Index of high byte of data field in Write File Record request */
#define MODBUS_WFILE_RCRD_DATA_IDX_HI       (10)

/*! Index of low byte of data field in Write File Record request */
#define MODBUS_WFILE_RCRD_DATA_IDX_LO       (11)

/*! Reference type field value for Write File Record request */
#define MODBUS_WFILE_REF_NO                 (0x06)

/*! Valid file address value for file record operations */
#define MODBUS_WFILE_VALID_FILE_ADDR        (0x0000)

/*! Packet size for Read Exception Status request/response */
#define MODBUS_EXP_STATUS_PKT_CNT           (4)

/*! Packet size for Read Event Counter request/response */
#define MODBUS_EVENT_CNTR_PKT_CNT           (4)

/*! Packet size for Read Event Log request/response */
#define MODBUS_EVENT_LOG_PKT_CNT            (4)

/*! Packet size for Report Server ID request/response */
#define MODBUS_REPORT_SRVR_ID_PKT_CNT       (4)

/*! Server ID response byte count index */
#define MODBUS_REPORT_SRVR_ID_BYT_CNT_IDX   (2)

/*! Minimum packet size for Diagnostic function requests */
#define MODBUS_DIAG_MIN_PKT_CNT             (6)

/*! Index of high byte of sub-function in diagnostic packet */
#define MODBUS_DIAG_SUB_FUNC_IDX_HI         (2)

/*! Index of low byte of sub-function in diagnostic packet */
#define MODBUS_DIAG_SUB_FUNC_IDX_LO         (3)

/*! Index of high byte of data field in diagnostic packet */
#define MODBUS_DIAG_DATA_IDX_HI             (4)

/*! Index of low byte of data field in diagnostic packet */
#define MODBUS_DIAG_DATA_IDX_LO             (5)

/*! Default data in MODBUS diagnostic request */
#define MODBUS_DIAG_DFLT_DATA               (0x0000)

/*! Value for restart communications and clear event log sub-function (as per Standard) */
#define MODBUS_DIAG_RSTRT_COMM_CLR_LOG      (0xFF00)

/*! Value for restart communications and preserve event log sub-function (as per Standard) */
#define MODBUS_DIAG_RSTRT_COMM_PRSRV_LOG    (0x0000)

/*! Value to set a character overrun error */
#define MODBUS_CHRCTR_OVRRN_SET             (1)

/*! Value to clear a character overrun error */
#define MODBUS_CHRCTR_OVRRN_CLR             (0)

/*! Maximum bit index (0-7) in a byte. */
#define MODBUS_MAX_BIT_IDX_IN_BYTE          (7)

/*!
 * \struct MODBUS_Diagnostics
 * \brief MODBUS RTU diagnostic counters for errors and events.
 */
typedef struct
{
    /*! Current exception status */
    uint8_t exceptionStatus;

    /*! Event counter */
    uint16_t eventCounter;

    /*! Number of MODBUS messages detected */
    uint16_t msgDetectedCounter;

    /*! Number of MODBUS messages successfully processed */
    uint16_t msgProcessedCounter;

    /*! Number of CRC errors detected */
    uint16_t crcErrorCounter;

    /*! Number of exception responses issued */
    uint16_t exceptionCounter;

    /*! Number of times no response was sent */
    uint16_t noRespCounter;

    /*! Diagnostic register */
    uint16_t diagReg;

    /*! Listen-Only mode status */
    uint16_t listenOnlyMode;

    /*! Number of times server reported busy */
    uint16_t serverBusyCount;

    /*! Number of times server sent NAK */
    uint16_t serverNAKCount;

    /*! Character overrun error count */
    uint16_t characterOvrnCount;

    /*! Character overrun error flag */
    uint8_t characterOvrnFlag;

    /*!
     * MODBUS event log buffer.
     * Stores transmit and receive event logs.
     * Each event is a byte, with bits representing status fields (see ModbusRTU_priv.h).
     * Can be read by the controller via specific function code.
     */
    uint8_t eventLog[MODBUS_MAX_EVENT_CNT];

    /*!
     * MODBUS FIFO queue buffer.
     * Used for Read FIFO Queue function code.
     * controller can read up to MODBUS_MAX_QUEUE_SIZE FIFO entries.
     */
    uint16_t readQueue[MODBUS_MAX_QUEUE_SIZE];
} MODBUS_Diagnostics;

/*!
 * \struct MODBUS_Time
 * \brief MODBUS RTU frame timing information.
 */
typedef struct
{
    /*! Current timer value */
    uint32_t currTime;

    /*! Previous timer value */
    uint32_t lastTime;

    /*! 3.5 character time in timer ticks */
    uint32_t ct3_5;

    /*! 1.5 character time in timer ticks */
    uint32_t ct1_5;

    /*! CPU frequency in Hz */
    uint32_t cpuFreq;

    /*! Timer frequency in Hz */
    uint32_t tFreq;

    /*! UART baud rate */
    uint32_t baud;

    /*! Turnaround timer value */
    uint32_t turnAroundTimer;

    /*! Response timeout value */
    uint32_t responseTimer;

    /*! Write timeout value */
    uint32_t writeTimeoutVal;
} MODBUS_Time;


/*!
 * \struct MODBUS_Regs
 * \brief MODBUS RTU register storage.
 */
typedef struct
{
    /*! Array holding coil (output) states */
    uint8_t coils[MODBUS_SIZE_OF_COILS];

    /*! Array holding discrete input states */
    uint8_t discreteInputs[MODBUS_SIZE_OF_DISCRETE_INPUTS];

    /*! Array holding input register values */
    uint8_t inputRegisters[MODBUS_SIZE_OF_INPUT_REGS];

    /*! Array holding holding register values */
    uint8_t holdingRegisters[MODBUS_SIZE_OF_HOLDING_REGS];
} MODBUS_Regs;


/*!
 * \struct MODBUS_CommVar
 * \brief MODBUS RTU communication buffers and counters.
 */
typedef struct
{
    /*! Receive MODBUS ADU buffer */
    uint8_t rxPacket[MODBUS_MAX_ADU_SIZE];

    /*! Transmit MODBUS ADU buffer */
    uint8_t txPacket[MODBUS_MAX_ADU_SIZE];

    /*! Counter for receive buffer position */
    uint16_t rxCount;

    /*! Counter for transmit buffer position */
    uint16_t txCount;
} MODBUS_CommVar;


/*!
 * \struct MODBUS_Params
 * \brief MODBUS RTU parameters and current state.
 */
typedef struct
{
    /*! 1 if controller mode enabled, 0 if target mode */
    uint8_t controllerEnable;

    /*! Target address */
    uint8_t targetID;

    /*! Flag to store the running status of server/target */
    uint8_t serverStatus;

    /*! Current operating state */
    uint8_t state;

    /*! Pointer to timer peripheral registers */
    GPTIMER_Regs *timer;

    /*! Pointer to UART peripheral registers */
    UART_Regs *uart;

#ifdef __MSPM0_HAS_CRCP__
    /*! Pointer to CRC peripheral registers (if available) */
    CRCP_Regs *crc;
#endif

    /*! MODBUS registers instance */
    MODBUS_Regs regs;

    /*! MODBUS communication variables instance */
    MODBUS_CommVar commVariables;

    /*! MODBUS time instance for tracking frame timing and timeouts */
    MODBUS_Time timeParams;

    /*! Number of tries done by controller in case of failure */
    uint8_t tries;

    /*!
     * Function pointer table for user device callbacks.
     * Indexed by register type and device address.
     * Each callback receives a pointer to data and an operation code.
     */
    void (*activeDeviceFunctionCall[MODBUS_NUM_OF_REG_TYPES][MODBUS_MAX_NUM_OF_REGS])(uint16_t *data, uint8_t op);

    /*!
     * Function pointer table for user defined function code callbacks.
     */
    void (*userDefinedFunctionCodeCall[MODBUS_NUM_OF_USER_DEFINED_FC])(void);

    /*! Pointer to restart communication callback function */
    void (*restartComm)(void);

    /*!
     * MODBUS file record storage.
     * Supports up to MODBUS_NUM_OF_FILES files, each with MODBUS_RECORDS_PER_FILE records.
     */
    uint16_t fileRecord[MODBUS_NUM_OF_FILES][MODBUS_RECORDS_PER_FILE];

    /*! MODBUS diagnostic counter instance */
    MODBUS_Diagnostics diagnostics;
} MODBUS_Params;

#endif
