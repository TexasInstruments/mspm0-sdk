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

#include "ModbusRTU_priv.h"

/* MODBUS communication parameters and state */
static volatile MODBUS_Params modbus;

#ifndef __MSPM0_HAS_CRCP__
/*!
 * Lookup tables for software MODBUS CRC16 calculation.
 * - Used if hardware CRC peripheral is not available.
 * - Modbus_calculateCRCHiByte:  Table for high byte of CRC.
 * - Modbus_calculateCRCLoByte:  Table for low byte of CRC.
 */
const uint8_t Modbus_calculateCRCHiByte[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
} ;
const uint8_t Modbus_calculateCRCLoByte[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
} ;

#endif

/*!
 * @brief Captures current time.
 *
 * This function returns the current time by subtracting load value
 * with current counter value(because using a down counter).
 */
static uint32_t Modbus_captureCurrentTime()
{
    return (DL_Timer_getLoadValue(modbus.timer) - DL_Timer_getTimerCount(modbus.timer));
}

/*!
 * @brief Insert a value into the MODBUS queue.
 *        Maintains FIFO order by shifting existing entries.
 * @param reg Value to insert into the queue.
 */
static void Modbus_insertQueue(uint16_t reg)
{
    /* Shift all non-default elements up one position, starting from the second last index. */
    for (int i = MODBUS_MAX_QUEUE_SIZE - 2; i >= 0; i--)
    {
        if (modbus.diagnostics.readQueue[i] != MODBUS_FIFO_DEFAULT_VAL)
        {
            modbus.diagnostics.readQueue[i + 1] = modbus.diagnostics.readQueue[i];
        }
    }
    /* Insert new value at the head of the queue. */
    modbus.diagnostics.readQueue[0] = reg;
}

/*!
 * @brief Delete the oldest value from the event FIFO queue.
 *        Finds the first non-default event from the end and marks it as default (deleted).
 */
static void Modbus_deleteQueue()
{
    for (int i = MODBUS_MAX_QUEUE_SIZE - 1; i >= 0; i--)
    {
        if (modbus.diagnostics.eventLog[i] != MODBUS_FIFO_DEFAULT_VAL)
        {
            /* Mark as deleted. */
            modbus.diagnostics.eventLog[i] = MODBUS_FIFO_DEFAULT_VAL;
            return;
        }
    }
}

/*!
 * @brief Clear the event log by setting all entries to the default value (0xFF).
 */
static void Modbus_clearEventLog()
{
    for (int i = 0; i < MODBUS_MAX_EVENT_CNT; i++)
        modbus.diagnostics.eventLog[i] = MODBUS_EVENTLOG_DEFAULT_VAL;
}

/*!
 * @brief Insert an event byte into the event log.
 *        Maintains event order by shifting existing entries.
 * @param eventByte Event code to insert.
 */
static void Modbus_insertEventLog(uint8_t eventByte)
{
    /* Shift all non-default events up one position, starting from the second last index. */
    for (int i = MODBUS_MAX_EVENT_CNT - 2; i >= 0; i--)
    {
        if (modbus.diagnostics.eventLog[i] != MODBUS_EVENTLOG_DEFAULT_VAL)
        {
            modbus.diagnostics.eventLog[i + 1] = modbus.diagnostics.eventLog[i];
        }
    }
    /* Insert new event at the head of the log. */
    modbus.diagnostics.eventLog[0] = eventByte;
}

/*!
 * @brief Clear all MODBUS diagnostic and protocol counters.
 *
 * This function resets all protocol and diagnostic counters to zero, including
 * error counters and message statistics.
 */
static void Modbus_clearAllRegsAndCounters()
{
    /* Initialize all diagnostic counters to 0. */
    modbus.diagnostics.diagReg = 0;
    modbus.diagnostics.eventCounter = 0;
    modbus.diagnostics.exceptionStatus = 0;
    modbus.diagnostics.msgProcessedCounter = 0;
    modbus.diagnostics.msgDetectedCounter = 0;
    modbus.diagnostics.crcErrorCounter = 0;
    modbus.diagnostics.exceptionCounter = 0;
    modbus.diagnostics.noRespCounter = 0;
    modbus.diagnostics.serverNAKCount = 0;
    modbus.diagnostics.serverBusyCount = 0;
}

/*!
 * @brief Calculate MODBUS CRC16 for a buffer.
 *        Uses software lookup tables unless hardware CRC peripheral is available.
 * @param buffer Pointer to data buffer.
 * @param length Length of buffer.
 * @return CRC16 value.
 */
static uint16_t Modbus_calculateCRC(uint8_t *buffer, uint16_t length)
{
#ifndef __MSPM0_HAS_CRCP__
    /* Software CRC calculation using lookup tables */
    uint8_t CRCByteHi = MODBUS_INITIAL_CRC_VAL;
    uint8_t CRCByteLo = MODBUS_INITIAL_CRC_VAL;
    uint16_t index;
    while (length--)
    {
        index = CRCByteLo ^ *buffer++;
        CRCByteLo = CRCByteHi ^ Modbus_calculateCRCHiByte[index];
        CRCByteHi = Modbus_calculateCRCLoByte[index];
    }
    return (CRCByteHi << 8 | CRCByteLo);

#else
    /*
     * Hardware CRC calculation using CRC peripheral
     * Set CRC peripheral seed value
     */
    DL_CRCP_setSeed16(modbus.crc, DL_CRCP_CRCSEED_SEED_16_MODBUS);
    for (int idx = 0; idx < length; idx++)
    {
        /* Feed data byte-by-byte. */
        DL_CRCP_feedData8(modbus.crc, buffer[idx]);
    }
    /* Get CRC16 result from peripheral. */
    return DL_CRCP_getResult16(modbus.crc);
#endif
}

/*!
 * @brief Handle MODBUS exception and send exception response to the controller.
 *        Builds and transmits an exception frame, updates diagnostics, and logs the event.
 * @param exceptionCode Exception code to send (see MODBUS exception codes).
 */
static void Modbus_exceptionHandling(uint8_t exceptionCode)
{
    /* Base event: response sent */
    uint8_t eventByte = MODBUS_EVNT_SND;

    /* Add exception-specific event flags */
    if (exceptionCode >= MODBUS_ILLEGAL_FUNCTION && exceptionCode <= MODBUS_ILLEGAL_DATA_VALUE)
        eventByte |= MODBUS_EVNT_SND_READ_EXP;
    /* Server abort */
    if (exceptionCode == MODBUS_SERVER_DEVICE_FAILURE)
        eventByte |= MODBUS_EVNT_SND_SERVER_ABORT;
    /* Server busy/acknowledge */
    if (exceptionCode == MODBUS_ACKNOWLEDGE || exceptionCode == MODBUS_SERVER_DEVICE_BUSY)
        eventByte |= MODBUS_EVNT_SND_SERVER_BUSY;
    /* Server NAK */
    if (exceptionCode == MODBUS_SERVER_NAK)
        eventByte |= MODBUS_EVNT_SND_SERVER_NAK;
    /* Listen only mode active */
    if (modbus.diagnostics.listenOnlyMode)
        eventByte |= MODBUS_EVNT_SND_LSTN_ONLY_MD;

    /* Build exception response frame */
    modbus.commVariables.txCount = 0;
    memset((uint8_t *)modbus.commVariables.txPacket, 0, sizeof(modbus.commVariables.txPacket));
    /* Target (Target) address */
    modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX];
    /* Function code | 0x80 (exception) */
    modbus.commVariables.txPacket[modbus.commVariables.txCount++] = MODBUS_EXCEPTION_MASK | modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX];
    /* Exception code */
    modbus.commVariables.txPacket[modbus.commVariables.txCount++] = exceptionCode;

    /* Calculate and append CRC */
    uint16_t crc = Modbus_calculateCRC((uint8_t *)modbus.commVariables.txPacket, modbus.commVariables.txCount);
    modbus.commVariables.txPacket[modbus.commVariables.txCount++] = MODBUS_16BITMASK_TO_GET_LAST_BYTE & crc;
    modbus.commVariables.txPacket[modbus.commVariables.txCount++] = MODBUS_16BITMASK_TO_GET_LAST_BYTE & (crc >> 8);

    modbus.state = MODBUS_SENDING;

    /* Send response only if not a broadcast request and not in listen-only mode */
    if (modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX] != MODBUS_BROADCAST_ADDR &&
        modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        uint32_t currTime = Modbus_captureCurrentTime();
        uint32_t lastTime = currTime;
        /* Transmit exception response frame */
        for (int i = 0; i < modbus.commVariables.txCount; i++)
        {
            DL_UART_Main_transmitDataBlocking(modbus.uart, modbus.commVariables.txPacket[i]);
        }
        /* Wait for UART to finish */
        while (DL_UART_isBusy(modbus.uart));

        /* Check for write timeout event (diagnostic) */
        currTime = Modbus_captureCurrentTime();
        int32_t difft = currTime - lastTime;
        if (difft < 0)
            difft += DL_Timer_getLoadValue(modbus.timer);
        /* Write timeout */
        if (difft > modbus.timeParams.writeTimeoutVal)
            eventByte |= MODBUS_EVNT_SND_WRT_TIMEOUT;
    }
    else
    {
        /* In listen-only mode: increment no response counter */
        modbus.diagnostics.noRespCounter++;
    }

    /* Log event and update diagnostic counters */
    Modbus_insertEventLog(eventByte);
    __disable_irq();
    modbus.commVariables.rxCount = MODBUS_DEFAULT_CNT_VAL;
    memset((uint8_t *)modbus.commVariables.rxPacket, MODBUS_DEFAULT_PKT_VAL, MODBUS_MAX_ADU_SIZE);
    modbus.commVariables.txCount = MODBUS_DEFAULT_CNT_VAL;
    memset((uint8_t *)modbus.commVariables.txPacket, MODBUS_DEFAULT_PKT_VAL, MODBUS_MAX_ADU_SIZE);
    __enable_irq();
    modbus.state = MODBUS_IDLE;
    /* Indicate exception occurred */
    modbus.diagnostics.exceptionStatus = 1;
    /* Increment exception counter */
    modbus.diagnostics.exceptionCounter++;
}

/*!
 * @brief Handle a received byte in MODBUS target (Target) mode.
 *        Maintains state machine, checks for inter-character timeout,
 *        and manages receive buffer.
 * @param byte Received byte from UART.
 */
static void Modbus_TargetRxByte(uint8_t byte)
{
    /* Only receive if not currently processing a frame */
    if (modbus.state != MODBUS_PROCESSING)
    {
        /* Set state to receiving */
        modbus.state = MODBUS_RECEIVING;

        /* Get current time for timeout calculation */
        modbus.timeParams.currTime = Modbus_captureCurrentTime();

        /* Calculate time difference since last byte */
        int32_t difft = modbus.timeParams.currTime - modbus.timeParams.lastTime;
        if (difft < 0)
            difft += DL_Timer_getLoadValue(modbus.timer);

        /* If bytes already received, check for inter-character timeout (T1.5) or buffer overflow */
        if ((modbus.commVariables.rxCount != 0) && ((difft > modbus.timeParams.ct1_5) || modbus.commVariables.rxCount == (MODBUS_MAX_ADU_SIZE - 1)))
        {
            /* Timeout or buffer full: reset receive count (start new frame) */
            modbus.commVariables.rxCount = 0;
        }

        /* Store received byte in buffer */
        modbus.commVariables.rxPacket[modbus.commVariables.rxCount++] = byte;

        /* Update last time */
        modbus.timeParams.lastTime = modbus.timeParams.currTime;
    }
    else
    {
        /* If already in processing state, increment server busy counter (diagnostic) */
        modbus.diagnostics.serverBusyCount++;
    }
}

/*!
 * @brief Send a MODBUS response frame.
 *        - Appends CRC to the response frame.
 *        - Handles listen-only mode and broadcast address (no response sent in these cases).
 *        - Sends the frame to the controller if appropriate.
 *        - Updates diagnostics, logs the event, and resets protocol state.
 */
static void Modbus_sendResponse()
{
    /* Base event: response sent */
    uint8_t eventByte = MODBUS_EVNT_SND;

    /* Mark event if in listen-only mode */
    if (modbus.diagnostics.listenOnlyMode)
        eventByte |= MODBUS_EVNT_SND_LSTN_ONLY_MD;

    /* Send response only if not a broadcast request and not in listen-only mode */
    if (modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX] != MODBUS_BROADCAST_ADDR &&
        modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Calculate and append CRC to response packet */
        uint16_t crc = Modbus_calculateCRC((uint8_t *)modbus.commVariables.txPacket, modbus.commVariables.txCount);
        /* CRC Low byte */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = MODBUS_16BITMASK_TO_GET_LAST_BYTE & crc;
        /* CRC High byte */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = MODBUS_16BITMASK_TO_GET_LAST_BYTE & (crc>>8);

        modbus.state = MODBUS_SENDING;

        /* Record time before sending (for write timeout diagnostics) */
        uint32_t currTime = Modbus_captureCurrentTime();
        uint32_t lastTime = currTime;

        /* Transmit response frame, byte by byte */
        for (int i = 0; i < modbus.commVariables.txCount; i++)
        {
            DL_UART_Main_transmitDataBlocking(modbus.uart, modbus.commVariables.txPacket[i]);
        }
        /* Wait for UART transmission to complete */
        while (DL_UART_isBusy(modbus.uart));

        /* Record time after sending and check if write timeout occurred */
        currTime = Modbus_captureCurrentTime();
        int32_t difft = currTime - lastTime;
        if (difft < 0)
            difft += DL_Timer_getLoadValue(modbus.timer);
        /* Add write timeout event flag */
        if (difft > modbus.timeParams.writeTimeoutVal)
            eventByte |= MODBUS_EVNT_SND_WRT_TIMEOUT;
    }
    else
    {
        /* No response sent: either broadcast request or listen-only mode active */
        modbus.diagnostics.noRespCounter++;
    }

    /* Log the event */
    Modbus_insertEventLog(eventByte);

    /* Reset protocol state and counters for next transaction */
    __disable_irq();
    modbus.commVariables.rxCount = 0;
    memset((uint8_t *)modbus.commVariables.rxPacket, 0, sizeof(modbus.commVariables.rxPacket));
    __enable_irq();
    modbus.state = MODBUS_IDLE;
    modbus.commVariables.txCount = 0;
    /* Increment event counter */
    modbus.diagnostics.eventCounter++;
    memset((uint8_t *)modbus.commVariables.txPacket, 0, sizeof(modbus.commVariables.txPacket));
    /* Clear exception status */
    modbus.diagnostics.exceptionStatus = 0;
}

/*!
 * @brief Process MODBUS Read Discrete Input function code (0x02).
 *        Handles extraction of the start address and quantity, reads discrete inputs via callbacks,
 *        builds the response with packed bits, and sends the response or an exception if there is an error.
 */
static void Modbus_readDiscreteInputs()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Check if received packet has minimum required length for this function */
        if (modbus.commVariables.rxCount < MODBUS_DI_MIN_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        uint16_t startAddress = 0, quantity = 0, regArrayIndex = 0, registerBitPosition = 0, txPacketIndex = 0, txPacketBitPosition = 0;

        /* Extract the starting address and quantity of inputs to read from the MODBUS request packet */
        startAddress = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_LO_IDX];
        quantity     = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_QUANTITY_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_QUANTITY_LO_IDX];

        /* Validate requested quantity must be within MODBUS allowed limits */
        if (quantity < MODBUS_DI_MIN_READ_CNT || quantity > MODBUS_DI_MAX_READ_CNT)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            return;
        }

        /* Validate start address and range must not exceed device's discrete input map */
        /* Doing startAddress + quantity - 1 to also include the start address register */
        if (startAddress >= MODBUS_MAX_NUM_OF_REGS || (startAddress + quantity - 1) >= MODBUS_MAX_NUM_OF_REGS)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_ADDRESS);
            return;
        }

        /* Calculate the starting byte and bit position in the discrete input array */
        regArrayIndex = startAddress / 8;
        registerBitPosition = startAddress % 8;
        uint8_t shiftIndex = registerBitPosition;

        /* Prepare the response packet: [Target Addr][Function Code][Byte Count][Data...] */
        modbus.commVariables.txPacket[txPacketIndex++] = modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX];
        modbus.commVariables.txPacket[txPacketIndex++] = modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX];
        /* Placeholder for byte count (to be filled later) */
        modbus.commVariables.txPacket[txPacketIndex++] = 0;
        uint8_t deviceCount = 0;

        /* Pack each discrete input value as a bit in the response */
        while (quantity--)
        {
            uint16_t data = 0;

            /* Check if a callback is registered for this discrete input */
            if (modbus.activeDeviceFunctionCall[MODBUS_DISCRETE_INPUTS][startAddress + deviceCount] == NULL)
            {
                Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
                return;
            }
            /* Call the user/device callback to obtain the input value */
            modbus.activeDeviceFunctionCall[MODBUS_DISCRETE_INPUTS][startAddress + deviceCount](&data, MODBUS_READ);
            deviceCount++;

            /* Set or clear the corresponding bit in the device's discrete inputs array */
            if (data == 1)
                modbus.regs.discreteInputs[regArrayIndex] |= (1 << registerBitPosition);
            else
                modbus.regs.discreteInputs[regArrayIndex] &= ~(1 << registerBitPosition);

            /* Set the corresponding bit in the response data (shift for proper placement in packed byte) */
            modbus.commVariables.txPacket[txPacketIndex] |= ((modbus.regs.discreteInputs[regArrayIndex] & (1 << registerBitPosition)) >> shiftIndex);

            txPacketBitPosition++;
            registerBitPosition++;

            /* If all bits in current byte have been used, move to next byte in both arrays */
            if (registerBitPosition > MODBUS_MAX_BIT_IDX_IN_BYTE)
            {
                regArrayIndex++;
                registerBitPosition = 0;
            }
            if (txPacketBitPosition > MODBUS_MAX_BIT_IDX_IN_BYTE)
            {
                txPacketIndex++;
                txPacketBitPosition = 0;
            }
        }

        /* Finalize the response length */
        modbus.commVariables.txCount = txPacketIndex;
        /* If there are remaining bits in the last byte, count that byte in the response */
        if (txPacketBitPosition)
            modbus.commVariables.txCount++;
        /* Set the actual byte count in the response */
        modbus.commVariables.txPacket[MODBUS_TX_PACKET_BYTE_CNT_IDX] = modbus.commVariables.txCount - MODBUS_TX_PACKET_BYTE_CNT_OFFSET;
    }

    /* Send the packed response to the controller */
    Modbus_sendResponse();
    /* Increment event counter for diagnostics */
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Process MODBUS Read Coil function code (0x01).
 *        Extracts start address and quantity, validates them,
 *        reads coil states via registered callbacks, packs the
 *        coil bits into the MODBUS response, and sends the response.
 *        Handles exceptions for invalid requests.
 */
static void Modbus_readCoils()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Check if received packet has minimum required length for this function */
        if (modbus.commVariables.rxCount < MODBUS_READ_COIL_MIN_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Variables for address, quantity, buffer indices, and bit positions */
        uint16_t startAddress = 0, quantity = 0, regArrayIndex = 0, registerBitPosition = 0;
        uint16_t txPacketIndex = 0, txPacketBitPosition = 0;

        /* Extract start address and quantity of coils to read from request */
        startAddress = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_LO_IDX];
        quantity    = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_QUANTITY_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_QUANTITY_LO_IDX];

        /* Validate quantity is within allowed MODBUS range */
        if (quantity < MODBUS_READ_COIL_MIN_READ_CNT || quantity > MODBUS_READ_COIL_MAX_READ_CNT)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            return;
        }

        /* Validate address range (no overflow beyond allowed MODBUS registers) */
        /* Doing startAddress + quantity - 1 to also include the start address register */
        if (startAddress >= MODBUS_MAX_NUM_OF_REGS || (startAddress + quantity - 1) >= MODBUS_MAX_NUM_OF_REGS)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_ADDRESS);
            return;
        }

        /* Calculate starting byte and bit position in coil array */
        regArrayIndex = startAddress / 8;
        registerBitPosition = startAddress % 8;
        uint8_t shiftIndex = registerBitPosition;

        /* Begin building response: [Target Addr][Function Code][Byte Count][Packed Coil Data...] */
        modbus.commVariables.txPacket[txPacketIndex++] = modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX];
        modbus.commVariables.txPacket[txPacketIndex++] = modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX];
        /* Placeholder for byte count (to be filled later) */
        modbus.commVariables.txPacket[txPacketIndex++] = 0;
        uint8_t deviceCount = 0;

        /* Loop through each requested coil, fetch value, and pack into response */
        while (quantity--)
        {
            uint16_t data = 0;

            /* Ensure callback is registered for this coil */
            if (modbus.activeDeviceFunctionCall[MODBUS_COILS][startAddress + deviceCount] == NULL)
            {
                Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
                return;
            }

            /* Call the callback to get the coil state (1 or 0) */
            modbus.activeDeviceFunctionCall[MODBUS_COILS][startAddress + deviceCount](&data, MODBUS_READ);
            deviceCount++;

            /* Set or clear bit in device coil state array */
            if (data == 1)
                modbus.regs.coils[regArrayIndex] |= (1 << registerBitPosition);
            else
                modbus.regs.coils[regArrayIndex] &= ~(1 << registerBitPosition);

            /* Pack the coil state into the response buffer at the correct bit position */
            modbus.commVariables.txPacket[txPacketIndex] |= ((modbus.regs.coils[regArrayIndex] & (1 << registerBitPosition)) >> shiftIndex);

            txPacketBitPosition++;
            registerBitPosition++;

            /* If we've filled a byte, move to next byte in both arrays */
            if (registerBitPosition > MODBUS_MAX_BIT_IDX_IN_BYTE)
            {
                regArrayIndex++;
                registerBitPosition = 0;
            }
            if (txPacketBitPosition > MODBUS_MAX_BIT_IDX_IN_BYTE)
            {
                txPacketIndex++;
                txPacketBitPosition = 0;
            }
        }

        /* Finalize response packet length */
        modbus.commVariables.txCount = txPacketIndex;
        /* If there are remaining bits, count the last byte */
        if (txPacketBitPosition)
            modbus.commVariables.txCount++;
        /* Set the byte count field in the response */
        modbus.commVariables.txPacket[MODBUS_TX_PACKET_BYTE_CNT_IDX] = modbus.commVariables.txCount - MODBUS_TX_PACKET_BYTE_CNT_OFFSET;
    }

    /* Send the packed response */
    Modbus_sendResponse();
    /* Update event counter for diagnostics */
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Process MODBUS Write Single Coil function code (0x05).
 *        Parses the request, validates input, updates the coil state via callback,
 *        updates internal coil array, and sends the response echoing the request.
 *        Handles exceptions for invalid requests or callback errors.
 */
static void Modbus_writeSingleCoil()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Check if received packet has the minimum required length for Write Single Coil */
        if (modbus.commVariables.rxCount < MODBUS_WRITE_SNGL_COIL_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Extract the coil address and value to write from the request */
        uint16_t startAddress = 0, value = 0, regArrayIndex = 0, registerBitPosition = 0;
        startAddress = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_LO_IDX];
        value       = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_QUANTITY_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_QUANTITY_LO_IDX];

        /* Validate address range (no overflow beyond allowed MODBUS registers) */
        if (startAddress >= MODBUS_MAX_NUM_OF_REGS)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_ADDRESS);
            return;
        }

        /* Value must be either 0x0000 (OFF) or 0xFF00 (ON) */
        if (!(value == MODBUS_WRITE_COIL_CLEAR_BIT || value == MODBUS_WRITE_COIL_SET_BIT))
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            return;
        }

        /* Calculate which byte and bit in the coil array corresponds to the address */
        regArrayIndex = startAddress / 8;
        registerBitPosition = startAddress % 8;

        /* Ensure a callback is registered for this coil address */
        if (modbus.activeDeviceFunctionCall[MODBUS_COILS][startAddress] == NULL)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Handle set or clear operation */
        if (value == MODBUS_WRITE_COIL_SET_BIT)
        {
            /* Set the bit in the user device/callback and the coil array */
            uint16_t data = 1;
            modbus.activeDeviceFunctionCall[MODBUS_COILS][startAddress](&data, MODBUS_WRITE);
            modbus.regs.coils[regArrayIndex] |= (1 << registerBitPosition);
        }
        else
        {
            /* Clear the bit in the user device/callback and the coil array */
            uint16_t data = 0;
            modbus.activeDeviceFunctionCall[MODBUS_COILS][startAddress](&data, MODBUS_WRITE);
            modbus.regs.coils[regArrayIndex] &= ~(1 << registerBitPosition);
        }

        /* Echo the request as the response (per MODBUS RTU protocol for this function) */
        for (uint8_t idx = 0; idx < MODBUS_WRITE_SNGL_COIL_PKT_CNT; idx++)
        {
            modbus.commVariables.txPacket[idx] = modbus.commVariables.rxPacket[idx];
        }
        modbus.commVariables.txCount = MODBUS_WRITE_SNGL_COIL_PKT_CNT;
    }

    /* Send the response frame */
    Modbus_sendResponse();
    /* Increment diagnostic event counter */
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Process MODBUS Write Multiple Coils function code (0x0F).
 *        Parses and validates request, updates coil states using device callbacks,
 *        updates the internal coil bitmap, and sends a response echoing the request header.
 *        Handles exceptions for invalid requests or missing device callbacks.
 */
static void Modbus_writeMultipleCoils()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Check if received packet has the minimum required length for Write Multiple Coils */
        if (modbus.commVariables.rxCount < MODBUS_WRITE_MLP_COIL_MIN_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Extract start address, quantity, and number of data bytes from the request */
        uint16_t startAddress = 0, numOfBytes = 0, quantity = 0, regArrayIndex = 0, registerBitPosition = 0;
        uint16_t rxPacketIndex = 0, rxPacketBitPosition = 0;
        startAddress = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_LO_IDX];
        quantity    = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_QUANTITY_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_QUANTITY_LO_IDX];
        numOfBytes  = modbus.commVariables.rxPacket[MODBUS_WRITE_MLP_COIL_BYTE_CNT_IDX];

        /* Validate quantity and byte count */
        if (quantity < MODBUS_WRITE_MLP_COIL_MIN_WRT_CNT ||
            quantity > MODBUS_WRITE_MLP_COIL_MAX_WRT_CNT ||
            numOfBytes * 8 < quantity)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            return;
        }

        /* Validate start address and range */
        /* Doing startAddress + quantity - 1 to also include the start address register */
        if (startAddress >= MODBUS_MAX_NUM_OF_REGS ||
            (startAddress + quantity - 1) >= MODBUS_MAX_NUM_OF_REGS)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_ADDRESS);
            return;
        }

        /* Calculate starting byte and bit position in coil array */
        regArrayIndex = startAddress / 8;
        registerBitPosition = startAddress % 8;
        rxPacketIndex = MODBUS_WRITE_MLP_COIL_DATA_IDX;

        uint16_t deviceCount = 0;

        /* Loop through each coil to be written */
        while (quantity--)
        {
            /* Extract the current bit from the request data bytes */
            uint16_t bit = (modbus.commVariables.rxPacket[rxPacketIndex] & (1 << rxPacketBitPosition)) ? 1 : 0;

            /* Ensure callback is registered for this coil address */
            if (modbus.activeDeviceFunctionCall[MODBUS_COILS][startAddress + deviceCount] == NULL)
            {
                Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
                return;
            }

            /* Call the user/device callback to set the output value */
            modbus.activeDeviceFunctionCall[MODBUS_COILS][startAddress + deviceCount](&bit, MODBUS_WRITE);
            deviceCount++;

            /* Set or clear the bit in the internal coil bitmap */
            if (bit == 1)
            {
                modbus.regs.coils[regArrayIndex] |= (1 << registerBitPosition);
            }
            else
            {
                modbus.regs.coils[regArrayIndex] &= ~(1 << registerBitPosition);
            }

            /* Move to next bit and handle byte/bit wrapping */
            rxPacketBitPosition++;
            registerBitPosition++;

            if (registerBitPosition > MODBUS_MAX_BIT_IDX_IN_BYTE)
            {
                regArrayIndex++;
                registerBitPosition = 0;
            }
            if (rxPacketBitPosition > MODBUS_MAX_BIT_IDX_IN_BYTE)
            {
                rxPacketIndex++;
                rxPacketBitPosition = 0;
            }
        }

        /* Echo the request header as the response (MODBUS RTU spec) */
        for (uint8_t idx = 0; idx < MODBUS_WRITE_MLP_COIL_RSP_CNT; idx++)
        {
            modbus.commVariables.txPacket[idx] = modbus.commVariables.rxPacket[idx];
        }
        modbus.commVariables.txCount = MODBUS_WRITE_MLP_COIL_RSP_CNT;
    }

    /* Send the response to the controller */
    Modbus_sendResponse();
    /* Increment event counter for diagnostics */
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Process MODBUS Read Input Register function code (0x04).
 *        Validates the request, reads input registers via device callbacks,
 *        builds the response packet in big-endian format, and sends the response.
 *        Handles exceptions for invalid requests or missing device callbacks.
 */
static void Modbus_readInputRegisters()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Ensure received packet has the minimum required length for this function */
        if (modbus.commVariables.rxCount < MODBUS_READ_IP_REG_MIN_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Extract starting address and quantity of registers to read from request */
        uint16_t startAddress = 0, quantity = 0, regArrayIndex = 0;
        uint16_t txPacketIndex = 0;

        startAddress = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_LO_IDX];
        quantity     = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_QUANTITY_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_QUANTITY_LO_IDX];

        /* Validate quantity is within allowed MODBUS range */
        if (quantity < MODBUS_READ_IP_REG_MIN_READ_CNT || quantity > MODBUS_READ_IP_REG_MAX_READ_CNT)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            return;
        }

        /* Validate address range */
        /* Doing startAddress + quantity - 1 to also include the start address register */
        if (startAddress >= MODBUS_MAX_NUM_OF_REGS || (startAddress + quantity - 1) >= MODBUS_MAX_NUM_OF_REGS)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_ADDRESS);
            return;
        }

        /* Calculate start index in input register buffer (each register is 2 bytes) */
        regArrayIndex = startAddress * 2;

        /* Build response: [Target Addr][Function Code][Byte Count][Data...] */
        modbus.commVariables.txPacket[txPacketIndex++] = modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX];
        modbus.commVariables.txPacket[txPacketIndex++] = modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX];
        modbus.commVariables.txPacket[txPacketIndex++] = 0; /* Placeholder for byte count, to be filled later */

        uint16_t deviceCount = 0;

        /* Loop through each requested input register */
        while (quantity--)
        {
            uint16_t data = 0;

            /* Ensure callback is registered for this input register */
            if (modbus.activeDeviceFunctionCall[MODBUS_INPUT_REG][startAddress + deviceCount] == NULL)
            {
                Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
                return;
            }

            /* Call the callback to get the register value */
            modbus.activeDeviceFunctionCall[MODBUS_INPUT_REG][startAddress + deviceCount](&data, MODBUS_READ);
            deviceCount++;

            /* Store register value as two bytes (big-endian: high byte first, then low byte) */
            modbus.regs.inputRegisters[regArrayIndex] = (data >> 8) & 0xFF;
            modbus.commVariables.txPacket[txPacketIndex++]    = modbus.regs.inputRegisters[regArrayIndex];
            regArrayIndex++;

            modbus.regs.inputRegisters[regArrayIndex] = data & 0xFF;
            modbus.commVariables.txPacket[txPacketIndex++]    = modbus.regs.inputRegisters[regArrayIndex];
            regArrayIndex++;
        }

        /* Set the total number of bytes in the response (number of registers * 2) */
        modbus.commVariables.txCount = txPacketIndex;
        modbus.commVariables.txPacket[MODBUS_TX_PACKET_BYTE_CNT_IDX] = modbus.commVariables.txCount - MODBUS_TX_PACKET_BYTE_CNT_OFFSET;
    }

    /* Send the response to the controller */
    Modbus_sendResponse();
    /* Increment event counter for diagnostics */
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Process MODBUS Read Holding Registers function code (0x03).
 *        Validates the request, reads holding registers via device callbacks,
 *        builds the response packet in big-endian format, and sends the response.
 *        Handles exceptions for invalid requests or missing device callbacks.
 */
static void Modbus_readHoldingRegisters()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Check if received packet has the minimum required length for this function */
        if (modbus.commVariables.rxCount < MODBUS_READ_HLD_REG_MIN_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Extract starting address and quantity of registers to read from the request */
        uint16_t startAddress = 0, quantity = 0, regArrayIndex = 0;
        uint16_t txPacketIndex = 0;

        startAddress = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_LO_IDX];
        quantity     = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_QUANTITY_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_QUANTITY_LO_IDX];

        /* Validate quantity is within allowed MODBUS range */
        if (quantity < MODBUS_READ_HLD_REG_MIN_READ_CNT || quantity > MODBUS_READ_HLD_REG_MAX_READ_CNT)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            return;
        }

        /* Validate address range */
        /* Doing startAddress + quantity - 1 to also include the start address register */
        if (startAddress >= MODBUS_MAX_NUM_OF_REGS || (startAddress + quantity - 1) >= MODBUS_MAX_NUM_OF_REGS)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_ADDRESS);
            return;
        }

        /* Calculate start index in holding/input register buffer (each register is 2 bytes) */
        regArrayIndex = startAddress * 2;

        /* Build response: [Target Addr][Function Code][Byte Count][Data...] */
        modbus.commVariables.txPacket[txPacketIndex++] = modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX];
        modbus.commVariables.txPacket[txPacketIndex++] = modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX];
        modbus.commVariables.txPacket[txPacketIndex++] = 0; /* Placeholder for byte count, to be filled later */

        uint16_t deviceCount = 0;

        /* Loop through each requested holding register */
        while (quantity--)
        {
            uint16_t data = 0;

            /* Ensure callback is registered for this holding register */
            if (modbus.activeDeviceFunctionCall[MODBUS_HOLDING_REG][startAddress + deviceCount] == NULL)
            {
                Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
                return;
            }

            /* Call the callback to get the register value */
            modbus.activeDeviceFunctionCall[MODBUS_HOLDING_REG][startAddress + deviceCount](&data, MODBUS_READ);
            deviceCount++;

            /* Store register value as two bytes (big-endian: high byte first, then low byte) */
            modbus.regs.inputRegisters[regArrayIndex] = (data >> 8) & 0xFF;
            modbus.commVariables.txPacket[txPacketIndex++]    = modbus.regs.inputRegisters[regArrayIndex];
            regArrayIndex++;

            modbus.regs.inputRegisters[regArrayIndex] = data & 0xFF;
            modbus.commVariables.txPacket[txPacketIndex++]    = modbus.regs.inputRegisters[regArrayIndex];
            regArrayIndex++;
        }

        /* Set the total number of bytes in the response (number of registers * 2) */
        modbus.commVariables.txCount = txPacketIndex;
        modbus.commVariables.txPacket[MODBUS_TX_PACKET_BYTE_CNT_IDX] = modbus.commVariables.txCount - MODBUS_TX_PACKET_BYTE_CNT_OFFSET;
    }

    /* Send the response to the controller */
    Modbus_sendResponse();
    /* Increment event counter for diagnostics */
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Process MODBUS Write Single Register function code (0x06).
 *        Parses and validates the request, writes the value to the register using a device callback,
 *        updates the internal holding register buffer, and sends a response echoing the request.
 *        Handles exceptions for invalid value or missing device callbacks.
 */
static void Modbus_writeSingleRegister()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Ensure the received packet has the minimum required length for Write Single Register */
        if (modbus.commVariables.rxCount < MODBUS_WRITE_SNGL_REG_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Extract register address and value to write from the request packet */
        uint16_t startAddress = 0, value = 0, regArrayIndex = 0;
        startAddress = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_LO_IDX];
        value       = (modbus.commVariables.rxPacket[MODBUS_WRITE_SNGL_REG_VAL_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_WRITE_SNGL_REG_VAL_LO_IDX];

        /* Validate that the value is within allowed MODBUS range */
        if (value < MODBUS_WRITE_SNGL_REG_MIN_VAL || value > MODBUS_WRITE_SNGL_REG_MAX_VAL)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            return;
        }

        /* Validate address range (no overflow beyond allowed MODBUS registers) */
        if (startAddress >= MODBUS_MAX_NUM_OF_REGS)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_ADDRESS);
            return;
        }

        /* Calculate the position in the holding register array (each register is 2 bytes) */
        regArrayIndex = startAddress * 2;

        /* Ensure a callback is registered for this holding register */
        if (modbus.activeDeviceFunctionCall[MODBUS_HOLDING_REG][startAddress] == NULL)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Write the value to the device/register using the callback */
        modbus.activeDeviceFunctionCall[MODBUS_HOLDING_REG][startAddress](&value, MODBUS_WRITE);

        /* Update the internal holding register buffer (big-endian: high byte first, then low byte) */
        modbus.regs.holdingRegisters[regArrayIndex]     = modbus.commVariables.rxPacket[MODBUS_WRITE_SNGL_REG_VAL_HI_IDX];
        modbus.regs.holdingRegisters[regArrayIndex + 1] = modbus.commVariables.rxPacket[MODBUS_WRITE_SNGL_REG_VAL_LO_IDX];

        /* Echo the original request as the response (per MODBUS RTU protocol) */
        for (uint8_t idx = 0; idx < MODBUS_WRITE_SNGL_REG_PKT_CNT; idx++)
        {
            modbus.commVariables.txPacket[idx] = modbus.commVariables.rxPacket[idx];
        }
        modbus.commVariables.txCount = MODBUS_WRITE_SNGL_REG_PKT_CNT;
    }

    /* Send the response to the controller */
    Modbus_sendResponse();
    /* Increment event counter for diagnostics */
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Process MODBUS Write Multiple Registers function code (0x10).
 *        Validates the request, writes values to the holding registers via device callbacks,
 *        updates the internal holding register buffer, and sends a response echoing the request header.
 *        Handles exceptions for invalid requests or missing device callbacks.
 */
static void Modbus_writeMultipleRegisters()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Ensure the received packet has the minimum required length for Write Multiple Registers */
        if (modbus.commVariables.rxCount < MODBUS_WRITE_MLP_REG_MIN_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Extract starting address, register quantity, and byte count from the request */
        uint16_t startAddress = 0, quantity = 0, numOfBytes = 0, regArrayIndex = 0, rxPacketIndex = 0;
        startAddress = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_LO_IDX];
        quantity    = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_QUANTITY_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_QUANTITY_LO_IDX];
        numOfBytes  = modbus.commVariables.rxPacket[MODBUS_WRITE_MLP_REG_BYTE_CNT_IDX];

        /* Validate address range */
        /* Doing startAddress + quantity - 1 to also include the start address register */
        if (startAddress >= MODBUS_MAX_NUM_OF_REGS || (startAddress + quantity - 1) >= MODBUS_MAX_NUM_OF_REGS)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_ADDRESS);
            return;
        }

        /* Validate quantity and byte count (each register is 2 bytes) */
        if (quantity < MODBUS_WRITE_MLP_REG_MIN_WRITE_CNT ||
            quantity > MODBUS_WRITE_MLP_REG_MAX_WRITE_CNT ||
            quantity * 2 != numOfBytes)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            return;
        }

        /* Calculate starting index in holding register array (each register is 2 bytes) */
        regArrayIndex = startAddress * 2;
        rxPacketIndex = MODBUS_WRITE_MLP_REG_DATA_IDX;

        uint16_t deviceCount = 0;

        /* Loop through each register to be written */
        while (quantity--)
        {
            /* Combine high and low bytes to form the register value */
            uint16_t data = (modbus.commVariables.rxPacket[rxPacketIndex++] << 8);
            data |= modbus.commVariables.rxPacket[rxPacketIndex++];

            /* Ensure callback is registered for this holding register */
            if (modbus.activeDeviceFunctionCall[MODBUS_HOLDING_REG][startAddress + deviceCount] == NULL)
            {
                Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
                return;
            }

            /* Write the value to the device/register using the callback */
            modbus.activeDeviceFunctionCall[MODBUS_HOLDING_REG][startAddress + deviceCount](&data, MODBUS_WRITE);
            deviceCount++;

            /* Update the internal holding register buffer (big-endian: high byte first, then low byte) */
            modbus.regs.holdingRegisters[regArrayIndex] = (data >> 8) & 0xFF;
            modbus.regs.holdingRegisters[regArrayIndex + 1] = data & 0xFF;
            /* Move to next register position */
            regArrayIndex += 2;
        }

        /* Echo the request header as the response (per MODBUS RTU protocol) */
        for (uint8_t idx = 0; idx < MODBUS_WRITE_MLP_REG_RSP_CNT; idx++)
        {
            modbus.commVariables.txPacket[idx] = modbus.commVariables.rxPacket[idx];
        }
        modbus.commVariables.txCount = MODBUS_WRITE_MLP_REG_RSP_CNT;
    }

    /* Send the response to the controller */
    Modbus_sendResponse();
    /* Increment event counter for diagnostics */
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Process MODBUS Read/Write Multiple Registers function code (0x17).
 *        Validates the request, writes values to holding registers via device callbacks,
 *        then reads values from holding registers, builds the response
 *        with the read values in big-endian format, and sends the response back.
 *        Handles exceptions for invalid requests or missing device callbacks.
 */
static void Modbus_readWriteMultipleRegisters()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Check if received packet has the minimum required length for this function */
        if (modbus.commVariables.rxCount < MODBUS_RW_MLP_REG_MIN_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Variables for read and write addresses, quantities, and indices */
        uint16_t startAddress = 0, quantity = 0, numOfBytes = 0, regArrayIndex = 0, rxPacketIndex = 0;
        uint16_t regArrayIndexRead = 0, startAddressRead = 0, quantityRead = 0, txPacketIndex = 0;

        /* --- Parse and validate READ parameters --- */
        startAddressRead = (modbus.commVariables.rxPacket[MODBUS_RW_MLP_REG_READ_ADD_IDX_HI] << 8) | modbus.commVariables.rxPacket[MODBUS_RW_MLP_REG_READ_ADD_IDX_LO];
        quantityRead     = (modbus.commVariables.rxPacket[MODBUS_RW_MLP_REG_READ_QTY_IDX_HI] << 8) | modbus.commVariables.rxPacket[MODBUS_RW_MLP_REG_READ_QTY_IDX_LO];

        /* Validate read quantity and address range */
        if (quantityRead < MODBUS_RW_MLP_REG_MIN_READ_CNT || quantityRead > MODBUS_RW_MLP_REG_MAX_READ_CNT)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            return;
        }
        /* Doing startAddressRead + quantityRead - 1 to also include the start address register */
        if (startAddressRead >= MODBUS_MAX_NUM_OF_REGS || (startAddressRead + quantityRead - 1) >= MODBUS_MAX_NUM_OF_REGS)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_ADDRESS);
            return;
        }

        /* --- Parse and validate WRITE parameters --- */
        startAddress = (modbus.commVariables.rxPacket[MODBUS_RW_MLP_REG_WRITE_ADD_IDX_HI] << 8) | modbus.commVariables.rxPacket[MODBUS_RW_MLP_REG_WRITE_ADD_IDX_LO];
        quantity    = (modbus.commVariables.rxPacket[MODBUS_RW_MLP_REG_WRITE_QTY_IDX_HI] << 8) | modbus.commVariables.rxPacket[MODBUS_RW_MLP_REG_WRITE_QTY_IDX_LO];
        numOfBytes  = modbus.commVariables.rxPacket[MODBUS_RW_MLP_REG_WRT_BYTCNT_IDX];

        /* Validate address range */
        /* Doing startAddress + quantity - 1 to also include the start address register */
        if (startAddress >= MODBUS_MAX_NUM_OF_REGS || (startAddress + quantity - 1) >= MODBUS_MAX_NUM_OF_REGS)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_ADDRESS);
            return;
        }

        /* Validate write quantity and byte count */
        if (quantity < MODBUS_RW_MLP_REG_MIN_WRITE_CNT ||
            quantity > MODBUS_RW_MLP_REG_MAX_WRITE_CNT ||
            quantity * 2 != numOfBytes)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            return;
        }

        /* Calculate starting index in holding register array for WRITE */
        regArrayIndex = startAddress * 2;
        rxPacketIndex = MODBUS_RW_MLP_REG_WRT_DATA_OFFSET; /* Data bytes start at this offset per spec */

        uint16_t deviceCount = 0;

        /* --- Write each register value as specified in the request --- */
        while (quantity--)
        {
            /* Combine high and low bytes for each register value */
            uint16_t data = (modbus.commVariables.rxPacket[rxPacketIndex++] << 8);
            data |= modbus.commVariables.rxPacket[rxPacketIndex++];

            /* Ensure callback is registered for this holding register */
            if (modbus.activeDeviceFunctionCall[MODBUS_HOLDING_REG][startAddress + deviceCount] == NULL)
            {
                Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
                return;
            }

            /* Write the value to the device/register using the callback */
            modbus.activeDeviceFunctionCall[MODBUS_HOLDING_REG][startAddress + deviceCount](&data, MODBUS_WRITE);
            deviceCount++;

            /* Update the internal holding register buffer (big-endian: high byte first, then low byte) */
            modbus.regs.holdingRegisters[regArrayIndex]     = (data >> 8) & 0xFF;
            modbus.regs.holdingRegisters[regArrayIndex + 1] = data & 0xFF;
            /* Move to next register position */
            regArrayIndex += 2;
        }

        /* --- Prepare the response: Target Addr, Function Code, Byte Count, Data... --- */
        regArrayIndexRead = startAddressRead * 2;
        modbus.commVariables.txPacket[txPacketIndex++] = modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX];
        modbus.commVariables.txPacket[txPacketIndex++] = modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX];
        modbus.commVariables.txPacket[txPacketIndex++] = 0; /* Placeholder for byte count */

        deviceCount = 0;
        /* --- Read and pack each requested register value into the response --- */
        while (quantityRead--)
        {
            uint16_t data = 0;

            /* Ensure callback is registered for this holding register (READ) */
            if (modbus.activeDeviceFunctionCall[MODBUS_HOLDING_REG][startAddressRead + deviceCount] == NULL)
            {
                Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
                return;
            }

            /* Read the register value from the device */
            modbus.activeDeviceFunctionCall[MODBUS_HOLDING_REG][startAddressRead + deviceCount](&data, MODBUS_READ);
            deviceCount++;

            /* Store register value as two bytes (big-endian: high byte, then low byte) */
            modbus.regs.inputRegisters[regArrayIndexRead] = (data >> 8) & 0xFF;
            modbus.commVariables.txPacket[txPacketIndex++] = modbus.regs.inputRegisters[regArrayIndexRead++];
            modbus.regs.inputRegisters[regArrayIndexRead] = data & 0xFF;
            modbus.commVariables.txPacket[txPacketIndex++] = modbus.regs.inputRegisters[regArrayIndexRead++];
        }

        /* Set the byte count field (number of registers read * 2) */
        modbus.commVariables.txCount = txPacketIndex;
        modbus.commVariables.txPacket[MODBUS_TX_PACKET_BYTE_CNT_IDX] = modbus.commVariables.txCount - MODBUS_TX_PACKET_BYTE_CNT_OFFSET;
    }

    /* Send the response to the controller */
    Modbus_sendResponse();
    /* Increment event counter for diagnostics */
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Process MODBUS Mask Write Register function code (0x16).
 *        Applies AND/OR masks to a single holding register, updates register via device callback,
 *        and echoes the request as the response. Handles basic input validation and errors.
 */
static void Modbus_maskWriteRegister()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Check if received packet has the minimum required length for Mask Write Register */
        if (modbus.commVariables.rxCount < MODBUS_MASK_WRITE_REG_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Extract register address, AND mask, and OR mask from request */
        uint16_t startAddress = 0, andMask = 0, orMask = 0, data = 0, regArrayIndex = 0;
        startAddress = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_LO_IDX];
        andMask      = (modbus.commVariables.rxPacket[MODBUS_MASK_WRT_REG_AND_MASK_IDX_HI] << 8) | modbus.commVariables.rxPacket[MODBUS_MASK_WRT_REG_AND_MASK_IDX_LO];
        orMask       = (modbus.commVariables.rxPacket[MODBUS_MASK_WRT_REG_OR_MASK_IDX_HI] << 8) | modbus.commVariables.rxPacket[MODBUS_MASK_WRT_REG_OR_MASK_IDX_LO];

        /* Validate address */
        if (startAddress >= MODBUS_MAX_NUM_OF_REGS)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_ADDRESS);
            return;
        }

        /* Compute register array index (each register is 2 bytes) */
        regArrayIndex = startAddress * 2;

        /* Read current register value (big-endian) */
        data = (modbus.regs.holdingRegisters[regArrayIndex] << 8) | modbus.regs.holdingRegisters[regArrayIndex + 1];

        /* Apply mask as per MODBUS spec: (Current & And_Mask) | (Or_Mask & ~And_Mask) */
        data = (data & andMask) | (orMask & (~andMask));

        /* Ensure a callback is registered for this holding register */
        if (modbus.activeDeviceFunctionCall[MODBUS_HOLDING_REG][startAddress] == NULL)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Write result through callback */
        modbus.activeDeviceFunctionCall[MODBUS_HOLDING_REG][startAddress](&data, MODBUS_WRITE);

        /* Store the updated value back to the internal register array (big-endian) */
        modbus.regs.holdingRegisters[regArrayIndex]     = (data >> 8) & 0xFF;
        modbus.regs.holdingRegisters[regArrayIndex + 1] = data & 0xFF;

        /* Echo the request as the response (per MODBUS RTU protocol) */
        for (uint8_t idx = 0; idx < MODBUS_MASK_WRITE_REG_PKT_CNT; idx++)
        {
            modbus.commVariables.txPacket[idx] = modbus.commVariables.rxPacket[idx];
        }
        modbus.commVariables.txCount = MODBUS_MASK_WRITE_REG_PKT_CNT;
    }

    /* Send response */
    Modbus_sendResponse();
    /* Increment event counter for diagnostics */
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Process MODBUS Read FIFO Queue function code (0x18).
 *        Reads a FIFO queue if address is valid, forms a response listing queue values,
 *        and sends the response to the controller. Handles overflow and invalid address errors.
 *        (In development phase)
 */
static void Modbus_readQueue()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Extract FIFO address from request */
        uint16_t fifoAdd = (modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_HI_IDX] << 8) | modbus.commVariables.rxPacket[MODBUS_RX_PACKET_ADDR_LO_IDX];

        /* Only one FIFO supported; check for valid address */
        if (fifoAdd != MODBUS_QUEUE_VALID_ADDR)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_ADDRESS);
            return;
        }

        /* If last index is filled, queue overflow occurred */
        if (modbus.diagnostics.readQueue[MODBUS_MAX_QUEUE_SIZE - 1] != MODBUS_FIFO_DEFAULT_VAL)
        {
            Modbus_deleteQueue();
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            return;
        }

        /* Begin forming response: [Target Addr][Function Code][Byte Count Hi][Byte Count Lo][FIFO Count Hi][FIFO Count Lo][FIFO Data...] */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX];
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX];
        /* Placeholder for Byte Count Hi */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = 0x00;
        /* Placeholder for Byte Count Lo */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = 0x00;
        /* Placeholder for FIFO Count Hi */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = 0x00;
        /* Placeholder for FIFO Count Lo */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = 0x00;

        /* Pack FIFO data, count how many items are in the queue */
        uint16_t fifoCount = 0;
        while (modbus.diagnostics.readQueue[fifoCount] != 0x0000 && fifoCount < MODBUS_MAX_QUEUE_SIZE)
        {
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = (modbus.diagnostics.readQueue[fifoCount] >> 8) & 0xFF;
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.diagnostics.readQueue[fifoCount] & 0xFF;
            fifoCount++;
        }

        /* Set byte count and FIFO count in the response packet */
        /* Byte count = FIFO count * 2 (bytes per register) + 2 (FIFO count field itself) */
        uint16_t byteCount = fifoCount * 2 + 2;
        /* Byte Count Hi */
        modbus.commVariables.txPacket[2] = (byteCount >> 8) & 0xFF;
        /* Byte Count Lo */
        modbus.commVariables.txPacket[3] = byteCount & 0xFF;
        /* FIFO Count Hi */
        modbus.commVariables.txPacket[4] = (fifoCount >> 8) & 0xFF;
        /* FIFO Count Lo */
        modbus.commVariables.txPacket[5] = fifoCount & 0xFF;
    }

    /* Send response */
    Modbus_sendResponse();
    /* Increment event counter for diagnostics */
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Process MODBUS Read File Record function code (0x14).
 *        Validates and parses the read file record request, checks for valid file/record range,
 *        and constructs a response with record data for each requested file record.
 *        Handles exceptions for invalid requests or addresses.
 */
static void Modbus_readFileRecord()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Validate packet length for minimum required bytes */
        if (modbus.commVariables.rxCount < MODBUS_RFILE_RCRD_MIN_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        uint8_t regArrayIndex = 0, nfiles = 0, ref = 0;
        uint16_t fileNo = 0, Rno = 0, Rlen = 0;
        regArrayIndex = modbus.commVariables.rxPacket[MODBUS_RFILE_RCRD_BYT_CNT_IDX];

        /* Validate request byte count range */
        if (regArrayIndex < MODBUS_RFILE_RCRD_MIN_BYT_CNT || regArrayIndex > MODBUS_RFILE_RCRD_MAX_BYT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            return;
        }

        /* Each sub-request occupies MODBUS_RFILE_EACH_RQST_BYT_CNT bytes */
        nfiles = regArrayIndex / MODBUS_RFILE_EACH_RQST_BYT_CNT;

        /* Prepare response: [Target Addr][Function Code][Byte Count][Sub-responses...] */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX];
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX];
        /* Placeholder for response byte count */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = 0x00;

        /* Parse and process each sub-request */
        for (int i = 0; i < nfiles; i++)
        {
            ref = modbus.commVariables.rxPacket[MODBUS_RFILE_REF_IDX + (MODBUS_RFILE_EACH_RQST_BYT_CNT * i)];
            fileNo = (modbus.commVariables.rxPacket[MODBUS_RFILE_FILE_IDX_HI + (MODBUS_RFILE_EACH_RQST_BYT_CNT * i)] << 8) |
                      modbus.commVariables.rxPacket[MODBUS_RFILE_FILE_IDX_LO + (MODBUS_RFILE_EACH_RQST_BYT_CNT * i)];
            /* MODBUS spec: file numbers are 1-based in request, convert to 0-based index */
            fileNo--;

            Rno = (modbus.commVariables.rxPacket[MODBUS_RFILE_RCRD_IDX_HI + (MODBUS_RFILE_EACH_RQST_BYT_CNT * i)] << 8) |
                   modbus.commVariables.rxPacket[MODBUS_RFILE_RCRD_IDX_LO + (MODBUS_RFILE_EACH_RQST_BYT_CNT * i)];

            Rlen = (modbus.commVariables.rxPacket[MODBUS_RFILE_RCRD_LEN_HI + (MODBUS_RFILE_EACH_RQST_BYT_CNT * i)] << 8) |
                    modbus.commVariables.rxPacket[MODBUS_RFILE_RCRD_LEN_LO + (MODBUS_RFILE_EACH_RQST_BYT_CNT * i)];

            /* Validate file/record reference, file number, and record range */
            if (ref != MODBUS_RFILE_REF_NO ||
                fileNo != MODBUS_RFILE_VALID_FILE_ADDR ||
                Rno > MODBUS_RECORDS_PER_FILE - 1 ||
                (Rno + Rlen) > MODBUS_RECORDS_PER_FILE - 1)
            {
                Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_ADDRESS);
                return;
            }

            /* Each sub-response: [dataLen][RefType][Data ...] */
            /* Data length (in bytes, includes RefType) */
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = Rlen * 2 + 1;
            /* Reference type (constant) */
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = MODBUS_RFILE_REF_NO;

            /* Copy register data for each record requested (big-endian) */
            for (int j = 0; j < Rlen; j++)
            {
                /* High byte */
                modbus.commVariables.txPacket[modbus.commVariables.txCount++] = (modbus.fileRecord[fileNo][Rno + j] >> 8) & 0xFF;
                /* Low byte */
                modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.fileRecord[fileNo][Rno + j] & 0xFF;
            }
        }

        /* Set the response byte count (does not include Target Addr, Function Code, or Byte Count field) */
        modbus.commVariables.txPacket[MODBUS_TX_PACKET_BYTE_CNT_IDX] = modbus.commVariables.txCount - MODBUS_TX_PACKET_BYTE_CNT_OFFSET;
    }

    Modbus_sendResponse();
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Process MODBUS Write File Record function code (0x15).
 *        Validates and parses the write file record request, checks for valid file/record range,
 *        writes the provided data to the appropriate file record locations,
 *        and echoes the request as the response.
 *        Handles exceptions for invalid requests or addresses.
 */
static void Modbus_writeFileRecord()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Validate packet length for minimum required bytes */
        if (modbus.commVariables.rxCount < MODBUS_WFILE_RCRD_MIN_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        uint8_t regArrayIndex = 0, nfiles = 0, ref = 0;
        uint16_t fileNo = 0, Rno = 0, Rlen = 0;
        regArrayIndex = modbus.commVariables.rxPacket[MODBUS_WFILE_RCRD_BYT_CNT_IDX];

        /* Validate request byte count range */
        if (regArrayIndex < MODBUS_WFILE_RCRD_MIN_BYT_CNT || regArrayIndex > MODBUS_WFILE_RCRD_MAX_BYT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            return;
        }

        /* Parse reference type, file number, record number, and record length */
        ref = modbus.commVariables.rxPacket[MODBUS_WFILE_REF_IDX];
        fileNo = (modbus.commVariables.rxPacket[MODBUS_WFILE_FILE_IDX_HI] << 8) | modbus.commVariables.rxPacket[MODBUS_WFILE_FILE_IDX_LO];
        /* MODBUS spec: file numbers are 1-based in request, convert to 0-based index */
        fileNo--;
        Rno = (modbus.commVariables.rxPacket[MODBUS_WFILE_RCRD_IDX_HI] << 8) | modbus.commVariables.rxPacket[MODBUS_WFILE_RCRD_IDX_LO];
        Rlen = (modbus.commVariables.rxPacket[MODBUS_WFILE_RCRD_LEN_HI] << 8) | modbus.commVariables.rxPacket[MODBUS_WFILE_RCRD_LEN_LO];

        /* Validate file/record reference, file number, and record range */
        if (ref != MODBUS_WFILE_REF_NO ||
            fileNo != MODBUS_WFILE_VALID_FILE_ADDR ||
            Rno > MODBUS_RECORDS_PER_FILE - 1 ||
            (Rno + Rlen) > MODBUS_RECORDS_PER_FILE - 1)
        {
            Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_ADDRESS);
            return;
        }

        /* Write data bytes to the file record (each record is 2 bytes, big-endian) */
        for (int i = 0; i < Rlen; i++)
        {
            modbus.fileRecord[fileNo][Rno + i] = (modbus.commVariables.rxPacket[MODBUS_WFILE_RCRD_DATA_IDX_HI + i] << 8) |
                                                 modbus.commVariables.rxPacket[MODBUS_WFILE_RCRD_DATA_IDX_LO + i];
        }

        /* Echo request as the response (excluding CRC) */
        memcpy((uint8_t *)modbus.commVariables.txPacket, (uint8_t *)modbus.commVariables.rxPacket, modbus.commVariables.rxCount - MODBUS_RTU_CRC_LEN);
        modbus.commVariables.txCount = modbus.commVariables.rxCount - MODBUS_RTU_CRC_LEN;
    }

    Modbus_sendResponse();
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Process MODBUS Read Exception Status function code (0x07).
 *        Responds with the Target address, function code, and current exception status.
 *        Handles minimal input validation.
 */
static void Modbus_readExceptionStatus()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Validate minimum packet length */
        if (modbus.commVariables.rxCount < MODBUS_EXP_STATUS_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Build response: [Target Addr][Function Code][Exception Status] */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX];
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX];
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.diagnostics.exceptionStatus;
    }
    /* Send the response frame */
    Modbus_sendResponse();
    /* Increment diagnostics event counter */
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Process MODBUS Get Communication Event Counter function code (0x0B).
 *        Responds with Target addr, function code, status, and the communication event counter.
 *        Handles minimal input validation.
 */
static void Modbus_getCommEventCounter()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Validate minimum packet length */
        if (modbus.commVariables.rxCount < MODBUS_EVENT_CNTR_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Build response: [Target Addr][Function Code][Status Hi][Status Lo][EventCnt Hi][EventCnt Lo] */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX];
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX];
        /* Status Hi: 0x0000 = successful */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = 0x00;
        /* Status Lo */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = 0x00;
        /* EventCnt Hi */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = (modbus.diagnostics.eventCounter >> 8) & 0xFF;
        /* EventCnt Lo */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.diagnostics.eventCounter & 0xFF;
    }
    /* Send the response frame */
    Modbus_sendResponse();
}

/*!
 * @brief Process MODBUS Get Communication Event Log function code (0x0C).
 *        Responds with a log of recent communication events (and counters).
 *        Handles minimal input validation and sets the byte count field.
 */
static void Modbus_getCommEventLog()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Validate minimum packet length */
        if (modbus.commVariables.rxCount < MODBUS_EVENT_LOG_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Build response: [Target Addr][Function Code][Byte Count][Status Hi][Status Lo]
           [EventCounter Hi][EventCounter Lo][MsgDetected Hi][MsgDetected Lo][Event Log ...] */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX];
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX];
        /* Placeholder for Byte Count (set later) */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = 0x00;
        /* Status Hi: 0x0000 = successful */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = 0x00;
        /* Status Lo */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = 0x00;
        /* EventCounter Hi */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = (modbus.diagnostics.eventCounter >> 8) & 0xFF;
        /* EventCounter Lo */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.diagnostics.eventCounter & 0xFF;
        /* MsgDetected Hi */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = (modbus.diagnostics.msgDetectedCounter >> 8) & 0xFF;
        /* MsgDetected Lo */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.diagnostics.msgDetectedCounter & 0xFF;

        /* Copy event log entries from the event queue into the response */
        uint8_t eventCount = 0;
        while (eventCount < MODBUS_MAX_EVENT_CNT && modbus.diagnostics.eventLog[eventCount] != MODBUS_EVENTLOG_DEFAULT_VAL)
        {
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.diagnostics.eventLog[eventCount++];
        }

        /* Set Byte Count field: total bytes after Function Code (excluding Target Addr, Function Code, and Byte Count itself) */
        modbus.commVariables.txPacket[MODBUS_TX_PACKET_BYTE_CNT_IDX] = modbus.commVariables.txCount - MODBUS_TX_PACKET_BYTE_CNT_OFFSET;
    }

    /* Send the response frame */
    Modbus_sendResponse();
}

/*!
 * @brief Process MODBUS Report Server ID function code (0x11).
 *        Validates the request, constructs a response with the server (Target) ID and status.
 *        The response can include additional data if required by the application.
 */
static void Modbus_reportServerID()
{
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE)
    {
        /* Validate that the received packet has the minimum required length for Report Server ID */
        if (modbus.commVariables.rxCount < MODBUS_REPORT_SRVR_ID_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Build the response:
           [Target Addr][Function Code][Byte Count][Server ID][Run Status][Additional data...] */
        /* Target address */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX];
        /* Function code */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX];
        /* Byte count (number of following bytes) */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = MODBUS_REPORT_SRVR_ID_PKT_CNT;
        /* Server ID (device-specific) */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.targetID;
        /* Run status (0xFF = ON, 0x00 = OFF) */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.serverStatus;

        /* Additional data can be added here if needed by the application */
    }

    /* Send the response to the controller */
    Modbus_sendResponse();
    /* Increment event counter for diagnostics */
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Process MODBUS Diagnostic function code (0x08) and its sub-functions.
 *        Handles a variety of diagnostic and counter operations as defined by the MODBUS specification.
 *        Responds with appropriate data or echoes the request, handles errors and updates diagnostics state.
 */
static void Modbus_diagnostics()
{
    /* Extract sub-function code (2 bytes, big-endian) */
    MODBUS_DiagnosticSubFunctionCode subFunc = (modbus.commVariables.rxPacket[MODBUS_DIAG_SUB_FUNC_IDX_HI] << 8) | modbus.commVariables.rxPacket[MODBUS_DIAG_SUB_FUNC_IDX_LO];
    if(modbus.diagnostics.listenOnlyMode == MODBUS_LISTEN_ONLY_MODE_INACTIVE || subFunc == MODBUS_RESTART_COMM_OPTION)
    {
        /* Validate minimum packet length for a MODBUS diagnostic request */
        if (modbus.commVariables.rxCount < MODBUS_DIAG_MIN_PKT_CNT)
        {
            Modbus_exceptionHandling(MODBUS_SERVER_DEVICE_FAILURE);
            return;
        }

        /* Prepare response: [Target Addr][Function Code][SubFunc Hi][SubFunc Lo]... */
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX];
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX];
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_DIAG_SUB_FUNC_IDX_HI];
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_DIAG_SUB_FUNC_IDX_LO];
        uint16_t data = (modbus.commVariables.rxPacket[MODBUS_DIAG_DATA_IDX_HI] << 8) | modbus.commVariables.rxPacket[MODBUS_DIAG_DATA_IDX_LO];

        /* Store last sub-function code in diagnostic register */
        if(subFunc != MODBUS_RETURN_DIAG_REG)
            modbus.diagnostics.diagReg = subFunc;

        switch (subFunc)
        {
        case MODBUS_RETURN_QUERY_DATA:
            /* Echo the request data (excluding CRC, per MODBUS spec) */
            memcpy((uint8_t *)modbus.commVariables.txPacket, (uint8_t *)modbus.commVariables.rxPacket, modbus.commVariables.rxCount - MODBUS_RTU_CRC_LEN);
            modbus.commVariables.txCount = modbus.commVariables.rxCount - MODBUS_RTU_CRC_LEN;
            Modbus_sendResponse();
            break;

        case MODBUS_RESTART_COMM_OPTION:
            if(data != MODBUS_DIAG_RSTRT_COMM_CLR_LOG && data != MODBUS_DIAG_RSTRT_COMM_PRSRV_LOG)
            {
                Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            }
            /* Respond with the data field, update diagnostic state, clear logs/counters as needed, and restart communications */
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_DIAG_DATA_IDX_HI];
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_DIAG_DATA_IDX_LO];
            uint16_t logClearCommand = (modbus.commVariables.rxPacket[MODBUS_DIAG_DATA_IDX_HI] << 8) | modbus.commVariables.rxPacket[MODBUS_DIAG_DATA_IDX_LO];
            Modbus_sendResponse();
            modbus.diagnostics.listenOnlyMode = MODBUS_LISTEN_ONLY_MODE_INACTIVE;
            if (logClearCommand == MODBUS_DIAG_RSTRT_COMM_CLR_LOG)
                Modbus_clearEventLog();
            Modbus_clearAllRegsAndCounters();
            Modbus_insertEventLog(MODBUS_EVNT_COMM_RESTART);
            modbus.restartComm();
            break;

        case MODBUS_RETURN_DIAG_REG:
            if(data != MODBUS_DIAG_DFLT_DATA)
            {
                Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            }
            /* Return the contents of the diagnostic register */
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = (modbus.diagnostics.diagReg >> 8) & 0xFF;
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.diagnostics.diagReg & 0xFF;
            Modbus_sendResponse();
            break;

        case MODBUS_CHANGE_ASCII_IP_DEL:
            /* Echo back the data field value (only relevant for ASCII, not RTU) */
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_DIAG_DATA_IDX_HI];
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_DIAG_DATA_IDX_LO];
            Modbus_sendResponse();
            break;

        case MODBUS_FORCE_LISTEN_ONLY_MODE:
            if(data != MODBUS_DIAG_DFLT_DATA)
            {
                Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            }
            /* Enter listen-only mode (no response to future requests) */
            Modbus_insertEventLog(MODBUS_EVNT_LSTN_ONLY_MD);
            modbus.diagnostics.listenOnlyMode = MODBUS_LISTEN_ONLY_MODE_ACTIVE;
            Modbus_sendResponse();
            break;

        case MODBUS_CLR_CNTR_AND_DIAG_REG:
            if(data != MODBUS_DIAG_DFLT_DATA)
            {
                Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            }
            /* Clear all counters and diagnostics register, echo back data field */
            Modbus_clearAllRegsAndCounters();
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_DIAG_DATA_IDX_HI];
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_DIAG_DATA_IDX_LO];
            Modbus_sendResponse();
            break;

        case MODBUS_RETURN_BUS_MSG_COUNT:
            if(data != MODBUS_DIAG_DFLT_DATA)
            {
                Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            }
            /* Return the total number of messages detected on the bus */
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = (modbus.diagnostics.msgDetectedCounter >> 8) & 0xFF;
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.diagnostics.msgDetectedCounter & 0xFF;
            Modbus_sendResponse();
            break;

        case MODBUS_RETURN_BUS_COMM_ERR_CNT:
            if(data != MODBUS_DIAG_DFLT_DATA)
            {
                Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            }
            /* Return the total number of CRC (communication) errors detected */
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = (modbus.diagnostics.crcErrorCounter >> 8) & 0xFF;
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.diagnostics.crcErrorCounter & 0xFF;
            Modbus_sendResponse();
            break;

        case MODBUS_RETURN_BUS_EXP_ERR_CNT:
            if(data != MODBUS_DIAG_DFLT_DATA)
            {
                Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            }
            /* Return the total number of MODBUS exception errors generated */
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = (modbus.diagnostics.exceptionCounter >> 8) & 0xFF;
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.diagnostics.exceptionCounter & 0xFF;
            Modbus_sendResponse();
            break;

        case MODBUS_RETURN_SRVR_MSG_CNT:
            if(data != MODBUS_DIAG_DFLT_DATA)
            {
                Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            }
            /* Return the total number of messages processed by the server */
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = (modbus.diagnostics.msgProcessedCounter >> 8) & 0xFF;
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.diagnostics.msgProcessedCounter & 0xFF;
            Modbus_sendResponse();
            break;

        case MODBUS_RETURN_SRVR_NO_RSP_CNT:
            if(data != MODBUS_DIAG_DFLT_DATA)
            {
                Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            }
            /* Return the total number of server no-response occurrences */
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = (modbus.diagnostics.noRespCounter >> 8) & 0xFF;
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.diagnostics.noRespCounter & 0xFF;
            Modbus_sendResponse();
            break;

        case MODBUS_RETURN_SRVR_NAK_CNT:
            if(data != MODBUS_DIAG_DFLT_DATA)
            {
                Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            }
            /* Return the total number of server negative acknowledge responses */
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = (modbus.diagnostics.serverNAKCount >> 8) & 0xFF;
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.diagnostics.serverNAKCount & 0xFF;
            Modbus_sendResponse();
            break;

        case MODBUS_RETURN_SRVR_BUSY_CNT:
            if(data != MODBUS_DIAG_DFLT_DATA)
            {
                Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            }
            /* Return the total number of times the server was busy */
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = (modbus.diagnostics.serverBusyCount >> 8) & 0xFF;
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.diagnostics.serverBusyCount & 0xFF;
            Modbus_sendResponse();
            break;

        case MODBUS_RETURN_BUS_CHR_OVRRN_CNT:
            if(data != MODBUS_DIAG_DFLT_DATA)
            {
                Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            }
            /* Return the total number of character overrun errors detected on the bus */
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = (modbus.diagnostics.characterOvrnCount >> 8) & 0xFF;
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.diagnostics.characterOvrnCount & 0xFF;
            Modbus_sendResponse();
            break;

        case MODBUS_CLR_OVRRN_CNTR_AND_FLG:
            if(data != MODBUS_DIAG_DFLT_DATA)
            {
                Modbus_exceptionHandling(MODBUS_ILLEGAL_DATA_VALUE);
            }
            /* Clear character overrun counter and flag, echo back data field */
            modbus.diagnostics.characterOvrnCount = 0;
            modbus.diagnostics.characterOvrnFlag = MODBUS_CHRCTR_OVRRN_CLR;
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_DIAG_DATA_IDX_HI];
            modbus.commVariables.txPacket[modbus.commVariables.txCount++] = modbus.commVariables.rxPacket[MODBUS_DIAG_DATA_IDX_LO];
            Modbus_sendResponse();
            break;

        default:
            /* Unknown or unsupported sub-function code */
            Modbus_exceptionHandling(MODBUS_ILLEGAL_FUNCTION);
        }
    }
    /* Increment diagnostic event counter for every diagnostic request processed */
    modbus.diagnostics.eventCounter++;
}

/*!
 * @brief Placeholder for MODBUS Miscellaneous function code.
 *        Intended for handling custom or vendor-specific MODBUS function codes.
 *        Implementation to be added as needed for future extensions (e.g., CAN integration).
 */
static void Modbus_misc()
{
    /* Placeholder for miscellaneous MODBUS function code handling. */
    /* To be implemented for custom or vendor-specific MODBUS extensions. */
    /* Example: Future support for CAN or other special features. */
    /* As of now returning the error MODBUS_ILLEGAL_FUNCTION. */
    Modbus_exceptionHandling(MODBUS_ILLEGAL_FUNCTION);
}

/*!
 * @brief  Check and execute user-defined Modbus function code.
 *
 * This function verifies if the received function code matches any configured user-defined function codes.
 * If a matching user-defined function code is found, its corresponding callback is executed.
 * The function supports two sets of user-defined function codes, each with defined lower and upper limits.
 * After executing the callback, the request packet is copied to the response buffer and sent back to the requester.
 *
 * @param functionCode Function code received in the Modbus request.
 * @return MODBUS_VALID_FUNCTION_CODE if a valid user-defined function code was handled, otherwise MODBUS_INVALID_FUNCTION_CODE.
 */
static uint8_t Modbus_isUserDefinedFunctionCode(uint8_t functionCode)
{
    uint8_t retVal = MODBUS_INVALID_FUNCTION_CODE;
    if ((functionCode >= MODBUS_USER_DEFINED_FC_SET1_LOWER_LIMIT
         && functionCode <= MODBUS_USER_DEFINED_FC_SET1_UPPER_LIMIT)
         && modbus.userDefinedFunctionCodeCall[functionCode - MODBUS_USER_DEFINED_FC_SET1_LOWER_LIMIT] != NULL)
    {
        modbus.userDefinedFunctionCodeCall[functionCode - MODBUS_USER_DEFINED_FC_SET1_LOWER_LIMIT]();
        retVal = MODBUS_VALID_FUNCTION_CODE;
    }
    else if((functionCode >= MODBUS_USER_DEFINED_FC_SET2_LOWER_LIMIT
            && functionCode <= MODBUS_USER_DEFINED_FC_SET2_UPPER_LIMIT)
            && modbus.userDefinedFunctionCodeCall[functionCode - MODBUS_USER_DEFINED_FC_SET2_LOWER_LIMIT + MODBUS_NUM_OF_FC_IN_SET1] != NULL)
    {
        modbus.userDefinedFunctionCodeCall[functionCode - MODBUS_USER_DEFINED_FC_SET2_LOWER_LIMIT + MODBUS_NUM_OF_FC_IN_SET1]();
        retVal = MODBUS_VALID_FUNCTION_CODE;
    }
    if(retVal == MODBUS_INVALID_FUNCTION_CODE)
    {
        return MODBUS_INVALID_FUNCTION_CODE;
    }
    memcpy((uint8_t *)modbus.commVariables.txPacket, (uint8_t *)modbus.commVariables.rxPacket, modbus.commVariables.rxCount - MODBUS_RTU_CRC_LEN);
    modbus.commVariables.txCount = modbus.commVariables.rxCount - MODBUS_RTU_CRC_LEN;
    Modbus_sendResponse();
    modbus.diagnostics.eventCounter++;
    return MODBUS_VALID_FUNCTION_CODE;
}

/*!
 * @brief  Initialize the MODBUS stack and all associated structures.
 *
 * This function sets all MODBUS-related data structures and variables to their default values.
 * It initializes hardware pointers, register arrays, communication variables, timing parameters,
 * diagnostics counters, file records, event logs, and callback pointers. All memory areas are
 * cleared or set to their respective default macros to ensure the MODBUS stack starts in a known
 * and stable state.
 */
void Modbus_init()
{
    /* Controller/Target mode */
    modbus.controllerEnable = MODBUS_CONTROLLER_DISABLE;
    modbus.targetID = MODBUS_DEFAULT_TARGET_ID;
    modbus.state = MODBUS_IDLE;

    /* Hardware pointers */
    modbus.timer = NULL;
    modbus.uart = NULL;

    modbus.serverStatus = MODBUS_REPORT_SRVR_ID_STATUS_OFF;
    modbus.tries = MODBUS_DEFAULT_TRIES;
#ifdef __MSPM0_HAS_CRCP__
    modbus.crc = CRCP0;
#endif

    /* Register arrays initialization */
    memset((uint8_t *)modbus.regs.coils, MODBUS_DEFAULT_REG_VAL, MODBUS_SIZE_OF_COILS);
    memset((uint8_t *)modbus.regs.discreteInputs, MODBUS_DEFAULT_REG_VAL, MODBUS_SIZE_OF_DISCRETE_INPUTS);
    memset((uint8_t *)modbus.regs.inputRegisters, MODBUS_DEFAULT_REG_VAL, MODBUS_SIZE_OF_INPUT_REGS);
    memset((uint8_t *)modbus.regs.holdingRegisters, MODBUS_DEFAULT_REG_VAL, MODBUS_SIZE_OF_HOLDING_REGS);

    /* Communication variables */
    memset((uint8_t *)modbus.commVariables.rxPacket, MODBUS_DEFAULT_PKT_VAL, MODBUS_MAX_ADU_SIZE);
    memset((uint8_t *)modbus.commVariables.txPacket, MODBUS_DEFAULT_PKT_VAL, MODBUS_MAX_ADU_SIZE);
    modbus.commVariables.rxCount = MODBUS_DEFAULT_CNT_VAL;
    modbus.commVariables.txCount = MODBUS_DEFAULT_CNT_VAL;

    /* Device function callbacks */
    for (int regType = 0; regType < MODBUS_NUM_OF_REG_TYPES; regType++) {
        for (int reg = 0; reg < MODBUS_MAX_NUM_OF_REGS; reg++) {
            modbus.activeDeviceFunctionCall[regType][reg] = NULL;
        }
    }

    /* Restart communication callback */
    modbus.restartComm = NULL;

    /* File records */
    for (int file = 0; file < MODBUS_NUM_OF_FILES; file++) {
        for (int record = 0; record < MODBUS_RECORDS_PER_FILE; record++) {
            modbus.fileRecord[file][record] = MODBUS_DEFAULT_FILE_RCRD_VAL;
        }
    }

    /* Timing parameters */
    modbus.timeParams.currTime = MODBUS_DEFAULT_CURR_TIME;
    modbus.timeParams.lastTime = MODBUS_DEFAULT_LAST_TIME;
    modbus.timeParams.ct3_5 = MODBUS_DEFAULT_CT3_5;
    modbus.timeParams.ct1_5 = MODBUS_DEFAULT_CT1_5;
    modbus.timeParams.cpuFreq = MODBUS_DEFAULT_CPU_FREQ;
    modbus.timeParams.tFreq = MODBUS_DEFAULT_TFREQ;
    modbus.timeParams.baud = MODBUS_DEFAULT_BAUD;
    modbus.timeParams.turnAroundTimer = MODBUS_DEFAULT_TURNAROUND_TIMER;
    modbus.timeParams.responseTimer = MODBUS_DEFAULT_RESPONSE_TIMER;
    modbus.timeParams.writeTimeoutVal = MODBUS_DEFAULT_WRITE_TIMEOUT_VAL;

    /* Diagnostics */
    modbus.diagnostics.exceptionStatus = MODBUS_DEFAULT_EXCEPTION_STATUS;
    modbus.diagnostics.eventCounter = MODBUS_DEFAULT_EVENT_COUNTER;
    modbus.diagnostics.msgDetectedCounter = MODBUS_DEFAULT_MSG_DETECTED_CNTR;
    modbus.diagnostics.msgProcessedCounter = MODBUS_DEFAULT_MSG_PROCESSED_CNTR;
    modbus.diagnostics.crcErrorCounter = MODBUS_DEFAULT_CRC_ERROR_COUNTER;
    modbus.diagnostics.exceptionCounter = MODBUS_DEFAULT_EXCEPTION_COUNTER;
    modbus.diagnostics.noRespCounter = MODBUS_DEFAULT_NO_RESP_COUNTER;
    modbus.diagnostics.diagReg = MODBUS_DEFAULT_DIAG_REG;
    modbus.diagnostics.listenOnlyMode = MODBUS_LISTEN_ONLY_MODE_INACTIVE;
    modbus.diagnostics.serverBusyCount = MODBUS_DEFAULT_SERVER_BUSY_COUNT;
    modbus.diagnostics.serverNAKCount = MODBUS_DEFAULT_SERVER_NAK_COUNT;
    modbus.diagnostics.characterOvrnCount = MODBUS_DEFAULT_CHARACTER_OVRN_COUNT;
    modbus.diagnostics.characterOvrnFlag = MODBUS_DEFAULT_CHARACTER_OVRN_FLAG;

    /* Event log initialization */
    for (int event = 0; event < MODBUS_MAX_EVENT_CNT; event++) {
        modbus.diagnostics.eventLog[event] = MODBUS_EVENTLOG_DEFAULT_VAL;
    }

    /* Read queue initialization */
    for (int queueItem = 0; queueItem < MODBUS_MAX_QUEUE_SIZE; queueItem++) {
        modbus.diagnostics.readQueue[queueItem] = MODBUS_FIFO_DEFAULT_VAL;
    }
}

/*!
 * @brief Set the MODBUS target address.
 * @param targetId Target address (1-247).
 */
void Modbus_setTargetID(uint8_t targetId)
{
    if(targetId < MODBUS_MIN_TARGET_ID || targetId > MODBUS_MAX_TARGET_ID)
        modbus.targetID = MODBUS_DEFAULT_TARGET_ID;
    else
        modbus.targetID = targetId;
}

/*!
 * @brief Set the controller response timer value, used for unicast requests.
 * @param timer Timer value in microseconds.
 *
 * This function converts the timer value from microseconds into timer ticks
 * using the timer frequency (tFreq), and stores the resulting value in
 * modbus.timeParams.responseTimer.
 */
void Modbus_setControllerRspTimer(uint32_t timer)
{
    /* Convert microseconds to timer ticks. */
    modbus.timeParams.responseTimer = timer / (MODBUS_US_IN_SECS / modbus.timeParams.tFreq);
}

/*!
 * @brief Set the MODBUS write timeout value.
 *
 * Converts the specified timeout in microseconds to timer ticks
 * based on the timer frequency and stores it in the MODBUS parameters.
 *
 * @param timer Timeout value in microseconds.
 */
void Modbus_setWriteTimeoutTimer(uint32_t timer)
{
    /* Convert microseconds to timer ticks based on timer frequency. */
    modbus.timeParams.writeTimeoutVal = timer / (MODBUS_US_IN_SECS / modbus.timeParams.tFreq);
}

/*!
 * @brief Set the number of retry attempts for MODBUS.
 *
 * Sets the number of tries to the specified value if it is greater than or equal
 * to the default. Otherwise, sets it to the default number of tries.
 *
 * @param tries Desired number of tries.
 */
void Modbus_setNumberOfTries(uint8_t tries)
{
    if (tries >= MODBUS_DEFAULT_TRIES)
    {
        modbus.tries = tries;
    }
    else
    {
        modbus.tries = MODBUS_DEFAULT_TRIES;
    }
}

/*!
 * @brief Set the controller turnaround timer value, used for broadcast requests.
 * @param timer Timer value in microseconds.
 *
 * This function converts the timer value from microseconds into CPU cycles
 * using the CPU frequency, and stores the resulting value inmodbus.timeParams.timeParams.turnAroundTimer.
 */
void Modbus_setControllerTurnAroundTimer(uint32_t timer)
{
    /* Convert microseconds to CPU cycles. */
   modbus.timeParams.turnAroundTimer = timer * (modbus.timeParams.cpuFreq / MODBUS_US_IN_SECS);
}

/*!
 * @brief Set MODBUS baud rate.
 * @param baud Baud rate (e.g., 9600, 19200).
 *
 * Stores the baud rate in the global modbusTimeParams structure.
 */
void Modbus_setUartBaudRate(uint32_t baud)
{
    modbus.timeParams.baud = baud;
}

/*!
 * @brief Set CPU frequency for timing calculations.
 * @param cpuFreq CPU Frequency in Hz.
 *
 * Stores the CPU frequency in the global modbusTimeParams structure.
 */
void Modbus_setCpuClkFreq(uint32_t cpuFreq)
{
    modbus.timeParams.cpuFreq = cpuFreq;
}

/*!
 * @brief Set timer frequency for MODBUS timing.
 * @param tFreq Timer Frequency in Hz.
 *
 * Stores the timer frequency in the global modbusTimeParams structure.
 */
void Modbus_setTimerClkFreq(uint32_t tFreq)
{
    modbus.timeParams.tFreq = tFreq;
}

/*!
 * @brief Set status of target/server.
 * @param status server status.
 *
 * Stores the server status in the global MODBUS_params structure.
 */
void Modbus_setServerStatus(uint8_t status)
{
    modbus.serverStatus = status;
}

/*!
 * @brief Initialize MODBUS target parameters and state.
 * @param mbTimer Pointer to timer peripheral registers.
 * @param mbUart Pointer to UART peripheral registers.
 *
 * This function sets up the MODBUS timer and UART hardware, and initializes
 * protocol timing based on baudrate and timer frequency. The MODBUS RTU
 * protocol defines character time intervals (T1.5 and T3.5) that is to
 * be calculated as follows:
 *
 *   - For baud rate = 9600 bps:
 *       1 bit time = 1/9600 seconds = 104.167 us
 *       1 character = 11 bits (start + 8 data + parity + stop)
 *       1 character time = 11 * 104.167 us = 1,145.83 us
 *   - For timer frequency = 32,768 Hz:
 *       1 tick = 1/32,768 seconds = 30.5176 us
 *       1 us = 32,768 / 1,000,000 = 0.032768 ticks
 *       1145 us = 0.032768 * 1145 = 37 ticks
 *   - To convert character times into timer ticks (after simplifying above equations):
 *       T3.5 = 3.5 characters time = 3.5 * 11 * timerFreq / baud
 *       T1.5 = 1.5 characters time = 1.5 * 11 * timerFreq / baud
 *
 * The above formula calculates the required timer ticks for T1.5 and T3.5
 * intervals, ensuring proper MODBUS RTU timing compliance.
 */
void Modbus_configTarget(GPTIMER_Regs *mbTimer, UART_Regs *mbUart)
{
    modbus.timer = mbTimer;
    modbus.uart = mbUart;
    if(modbus.timeParams.baud <= MODBUS_BAUD19200)
    {
        /* Calculate MODBUS RTU timing intervals (in timer ticks). */
        /* T3.5: 3.5 character time. */
        modbus.timeParams.ct3_5 = (uint32_t)(3.5 * MODBUS_BITS_IN_CHAR * modbus.timeParams.tFreq) / modbus.timeParams.baud;
        /* T1.5: 1.5 character times. */
        modbus.timeParams.ct1_5 = (uint32_t)(1.5 * MODBUS_BITS_IN_CHAR * modbus.timeParams.tFreq) / modbus.timeParams.baud;
    }
    else
    {
        modbus.timeParams.ct3_5 = (uint32_t)(MODBUS_DEFAULT_CT3_5 / (MODBUS_US_IN_SECS / modbus.timeParams.tFreq));
        modbus.timeParams.ct1_5 = (uint32_t)(MODBUS_DEFAULT_CT1_5 / (MODBUS_US_IN_SECS / modbus.timeParams.tFreq));
    }
    modbus.controllerEnable = MODBUS_CONTROLLER_DISABLE;
    /* Start the MODBUS timer and record the initial modbusTimeParams values. */
    DL_Timer_startCounter(modbus.timer);
    modbus.timeParams.currTime = Modbus_captureCurrentTime();
    modbus.timeParams.lastTime = modbus.timeParams.currTime;
}

/*!
 * @brief Register a callback function for MODBUS restart communication events.
 *        This callback is called by the stack when a restart communication request is received.
 * @param func Function pointer to restart routine (prototype: void func(void)).
 */
void Modbus_configRestartComm(void (*func)(void))
{
    modbus.restartComm = func;
}

/*!
 * @brief Register a device function callback for a specific MODBUS address and register type.
 *        This enables dynamic, per-address handling of MODBUS requests (read/write) for each register type.
 * @param add  Device address (1-255). Must be within MODBUS_MIN_DEVICE_ADDR and MODBUS_MAX_DEVICE_ADDR.
 * @param type Register type (see MBRegs, e.g., MODBUS_COILS, MODBUS_DISCRETE_INPUTS, etc.).
 * @param func Function pointer to device callback (prototype: void func(uint16_t *data, uint8_t op)).
 * @return MODBUS_TARGET_DEVICE_INIT_OK (1) on success, MODBUS_TARGET_DEVICE_INIT_ERR (0) on error (invalid address or type).
 *
 * @note The callback is stored in the modbus.activeDeviceFunctionCall table, indexed by register type and (address-1).
 *       This allows the MODBUS stack to dispatch register operations to the correct device handler.
 */
uint8_t Modbus_configDevice(uint8_t add, uint8_t type, void (*func)(uint16_t *data, uint8_t op))
{
    /* Validate register type and device address */
    if (type >= MODBUS_NUM_OF_REG_TYPES || add < MODBUS_MIN_DEVICE_ADDR || add > MODBUS_MAX_DEVICE_ADDR)
        return MODBUS_TARGET_DEVICE_INIT_ERR;

    /* Map address 1-255 to array index 0-254 (address-1) */
    modbus.activeDeviceFunctionCall[type][add - 1] = func;
    return MODBUS_TARGET_DEVICE_INIT_OK;
}

/*!
 * @brief Initialize MODBUS controller parameters and state.
 * @param mbTimer Pointer to timer peripheral registers.
 * @param mbUart Pointer to UART peripheral registers.
 *
 * This function sets up the MODBUS controller timer and UART hardware,
 * calculates protocol timing, and resets event and diagnostic counters.
 */
void Modbus_configController(GPTIMER_Regs *mbTimer, UART_Regs *mbUart)
{
    modbus.timer = mbTimer;
    modbus.uart = mbUart;
    if(modbus.timeParams.baud <= MODBUS_BAUD19200)
    {
        /* Calculate MODBUS RTU timing intervals (in timer ticks). */
        /* T3.5: 3.5 character time. */
        modbus.timeParams.ct3_5 = (uint32_t)(3.5 * MODBUS_BITS_IN_CHAR * modbus.timeParams.tFreq) / modbus.timeParams.baud;
        /* T1.5: 1.5 character times. */
        modbus.timeParams.ct1_5 = (uint32_t)(1.5 * MODBUS_BITS_IN_CHAR * modbus.timeParams.tFreq) / modbus.timeParams.baud;
    }
    else
    {
        modbus.timeParams.ct3_5 = (uint32_t)(MODBUS_DEFAULT_CT3_5 / (MODBUS_US_IN_SECS / modbus.timeParams.tFreq));
        modbus.timeParams.ct1_5 = (uint32_t)(MODBUS_DEFAULT_CT1_5 / (MODBUS_US_IN_SECS / modbus.timeParams.tFreq));
    }
    modbus.controllerEnable = MODBUS_CONTROLLER_ENABLE;
    DL_Timer_startCounter(modbus.timer);
    modbus.timeParams.currTime = Modbus_captureCurrentTime();
    modbus.timeParams.lastTime = modbus.timeParams.currTime;
}

/*!
 * @brief Send a MODBUS function code request as controller.
 *        Builds the request frame, sends it to the specified Target address, and waits for a response.
 *        Handles retries, CRC calculation, and exception/result parsing.
 * @param targetId      Target (Target) address.
 * @param functionCode  MODBUS function code.
 * @param data          Pointer to data buffer (contains request data, will be filled with response).
 * @param dataLen       Length of data to send.
 * @param exceptionCode Pointer to store the exception code result.
 */
void Modbus_sendCommand(uint8_t targetId, uint8_t functionCode, uint8_t *data, uint8_t dataLen, uint8_t *exceptionCode)
{
    /* Set default exception code to no exception */
    *exceptionCode = MODBUS_NO_EXCEPTION;
    uint8_t trials = modbus.tries;

    /* Initialize Tx and Rx counters and clear buffers */
    __disable_irq();
    modbus.commVariables.txCount = MODBUS_DEFAULT_CNT_VAL;
    modbus.commVariables.rxCount = MODBUS_DEFAULT_CNT_VAL;
    memset((uint8_t *)modbus.commVariables.rxPacket, MODBUS_DEFAULT_PKT_VAL, MODBUS_MAX_ADU_SIZE);
    memset((uint8_t *)modbus.commVariables.txPacket, MODBUS_DEFAULT_PKT_VAL, MODBUS_MAX_ADU_SIZE);
    __enable_irq();

    /* Build MODBUS request frame: [Target Addr][Function Code][Data...][CRC Lo][CRC Hi] */
    modbus.commVariables.txPacket[modbus.commVariables.txCount++] = targetId;
    modbus.commVariables.txPacket[modbus.commVariables.txCount++] = functionCode;
        for (int i = 0; i < dataLen; i++)
        modbus.commVariables.txPacket[modbus.commVariables.txCount++] = data[i];
    uint16_t crc = Modbus_calculateCRC((uint8_t *)modbus.commVariables.txPacket, modbus.commVariables.txCount);

    /* Store lower byte of CRC first, then higher byte */
    modbus.commVariables.txPacket[modbus.commVariables.txCount++] = MODBUS_16BITMASK_TO_GET_LAST_BYTE & crc;
    modbus.commVariables.txPacket[modbus.commVariables.txCount++] = MODBUS_16BITMASK_TO_GET_LAST_BYTE & (crc >> 8);

    /* Try up to max trials times to send the request and get a valid response */
    while ((trials--) > 0)
    {
        __disable_irq();
        /* Reset Rx counter for this attempt */
        modbus.commVariables.rxCount = MODBUS_DEFAULT_CNT_VAL;
        __enable_irq();

        /* Transmit the MODBUS request frame byte-by-byte (blocking) */
        modbus.state = MODBUS_SENDING;
        for (int i = 0; i < modbus.commVariables.txCount; i++)
        {
            DL_UART_Main_transmitDataBlocking(modbus.uart, modbus.commVariables.txPacket[i]);
        }
        /* Wait for transmission to complete */
        while (DL_UART_isBusy(modbus.uart));
        modbus.state = MODBUS_IDLE;

        /* Handle broadcast or listen-only mode cases */
        /* For diagnostics, check if sub-function code matches listen only or restart comm */
        if (targetId == MODBUS_BROADCAST_ADDR ||
            modbus.diagnostics.listenOnlyMode ||
           (functionCode == MODBUS_DIAGNOSTICS && (((data[MODBUS_TX_DATA_SUB_FC_IDX_HI] << 8) | data[MODBUS_TX_DATA_SUB_FC_IDX_LO]) == MODBUS_FORCE_LISTEN_ONLY_MODE)
            && (((data[MODBUS_TX_DATA_SUB_FC_DATA_HI] << 8) | data[MODBUS_TX_DATA_SUB_FC_DATA_LO]) == MODBUS_DIAG_DFLT_DATA)))
        {
            /* Update listen only mode flag if applicable */
            if (functionCode == MODBUS_DIAGNOSTICS && ((data[MODBUS_TX_DATA_SUB_FC_IDX_HI] << 8) | data[MODBUS_TX_DATA_SUB_FC_IDX_LO]) == MODBUS_FORCE_LISTEN_ONLY_MODE)
                modbus.diagnostics.listenOnlyMode = MODBUS_LISTEN_ONLY_MODE_ACTIVE;
            if (functionCode == MODBUS_DIAGNOSTICS && ((data[MODBUS_TX_DATA_SUB_FC_IDX_HI] << 8) | data[MODBUS_TX_DATA_SUB_FC_IDX_LO]) == MODBUS_RESTART_COMM_OPTION)
                modbus.diagnostics.listenOnlyMode = MODBUS_LISTEN_ONLY_MODE_INACTIVE;

            delay_cycles(modbus.timeParams.turnAroundTimer);
            return;
        }
        /* Unicast request flow: wait for a response or timeout */
        else
        {
            /* Record the current timer value for timeout tracking */
            __disable_irq();
            modbus.timeParams.currTime = Modbus_captureCurrentTime();
            modbus.timeParams.lastTime = modbus.timeParams.currTime;
            __enable_irq();

            while (1)
            {
                __disable_irq();
                modbus.timeParams.currTime = Modbus_captureCurrentTime();
                int32_t difft = modbus.timeParams.currTime - modbus.timeParams.lastTime;
                __enable_irq();

                /* Handle timer wrap-around */
                if (difft < 0)
                    difft += DL_Timer_getLoadValue(modbus.timer);

                /* If enough bytes received and inter-frame delay (T3.5) has elapsed, process response */
                if (modbus.commVariables.rxCount >= MODBUS_MIN_ADU_SIZE && difft > modbus.timeParams.ct3_5)
                {
                    /* Check if response address matches expected Target address */
                    if (modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX] != targetId)
                    {
                        /* Invalid/No valid response, possibly framing error */
                        *exceptionCode = MODBUS_INV_TARGET_RES;
                        memset(data, 0, MODBUS_MAX_ADU_SIZE);
                        for (int i = 0; i < modbus.commVariables.rxCount; i++)
                        {
                            data[i] = modbus.commVariables.rxPacket[i];
                        }
                        return;
                    }
                    /* Validate received CRC */
                    uint16_t vcrc = Modbus_calculateCRC((uint8_t *)modbus.commVariables.rxPacket, modbus.commVariables.rxCount - MODBUS_RTU_CRC_LEN);

                    /* CRC check: last 2 bytes are CRC (low, high) */
                    if (!(((MODBUS_16BITMASK_TO_GET_LAST_BYTE & vcrc) == modbus.commVariables.rxPacket[modbus.commVariables.rxCount - 2]) &&
                          ((MODBUS_16BITMASK_TO_GET_LAST_BYTE & (vcrc >> 8)) == modbus.commVariables.rxPacket[modbus.commVariables.rxCount - 1])))
                    {
                        /* CRC error in response */
                        *exceptionCode = MODBUS_CRC_ERROR_IN_TARGET_RES;
                        memset(data, 0, MODBUS_MAX_ADU_SIZE);
                        for (int i = 0; i < modbus.commVariables.rxCount; i++)
                        {
                            data[i] = modbus.commVariables.rxPacket[i];
                        }
                        return;
                    }
                    else
                    {
                        memset(data, 0, MODBUS_MAX_ADU_SIZE);
                        /* Check if response is an exception (function code MSB set) */
                        if (modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX] & MODBUS_EXCEPTION_MASK)
                        {
                            *exceptionCode = modbus.commVariables.rxPacket[MODBUS_EXCEPTION_CODE_IDX];
                            data[0] = modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX];
                            data[1] = modbus.commVariables.rxPacket[MODBUS_EXCEPTION_CODE_IDX];
                            return;
                        }
                        /* Successful response: copy PDU (excluding address and CRC) */
                        *exceptionCode = 0;
                        for (int i = 0; i < modbus.commVariables.rxCount - MODBUS_PDU_ADU_BYTE_DIFF; i++)
                        {
                            data[i] = modbus.commVariables.rxPacket[i + 1];
                        }
                        return;
                    }
                }
                /* Timeout: if no response in allotted response time */
                else if (difft >= modbus.timeParams.responseTimer)
                {
                    /* Only retry if no bytes received at all (to avoid duplicate write side effects) */
                    if (modbus.commVariables.rxCount == 0)
                    {
                        /* Retry sending */
                        break;
                    }
                    *exceptionCode = MODBUS_INV_TARGET_RES;
                    memset(data, 0, MODBUS_MAX_ADU_SIZE);
                    for (int i = 0; i < modbus.commVariables.rxCount; i++)
                    {
                        data[i] = modbus.commVariables.rxPacket[i];
                    }
                    return;
                }
            }
        }
    }
    /* If all retries failed, report no/invalid response */
    *exceptionCode = MODBUS_NO_TARGET_RES;
    memset(data, 0, MODBUS_MAX_ADU_SIZE);
    data[0] = modbus.commVariables.txPacket[MODBUS_FUNCTION_CODE_IDX];
    data[1] = *exceptionCode;
}

/*!
 * @brief
 * Register a callback function to handle a user-defined function code for Modbus.
 * This function allows the registration of a callback routine associated with a user-defined Modbus function code.
 *
 * Parameters:
 *   func - Function pointer to the user-defined callback routine (prototype: void func(void)).
 *   userFunctionCode - The user-defined function code to associate with the callback.
 * Returns:
 *   MODBUS_USER_FUNCTION_INIT_OK on success, MODBUS_USER_FUNCTION_INIT_ERR on error.
 */
uint8_t Modbus_configUserFunctionCode(void (*func)(void), uint8_t userFunctionCode)
{
    /* Check if the userFunctionCode is within any of the allowed user-defined function code sets */
    if ((userFunctionCode >= MODBUS_USER_DEFINED_FC_SET1_LOWER_LIMIT
         && userFunctionCode <= MODBUS_USER_DEFINED_FC_SET1_UPPER_LIMIT)
         || (userFunctionCode >= MODBUS_USER_DEFINED_FC_SET2_LOWER_LIMIT
         && userFunctionCode <= MODBUS_USER_DEFINED_FC_SET2_UPPER_LIMIT))
    {
        /* If the code is in the first set, store the callback in the corresponding slot */
        if ((userFunctionCode >= MODBUS_USER_DEFINED_FC_SET1_LOWER_LIMIT
             && userFunctionCode <= MODBUS_USER_DEFINED_FC_SET1_UPPER_LIMIT))
        {
            modbus.userDefinedFunctionCodeCall[userFunctionCode - MODBUS_USER_DEFINED_FC_SET1_LOWER_LIMIT] = func;
        }
        else
        {
            /* Otherwise, it must be in the second set; store accordingly */
            modbus.userDefinedFunctionCodeCall[userFunctionCode - MODBUS_USER_DEFINED_FC_SET2_LOWER_LIMIT + MODBUS_NUM_OF_FC_IN_SET1] = func;
        }
        /* Success */
        return MODBUS_USER_FUNCTION_INIT_OK;
    }
    else
    {
        /* Function code not allowed; return error */
        return MODBUS_USER_FUNCTION_INIT_ERR;
    }
}

/*!
 * @brief Main MODBUS target function code processing routine.
 *        Checks for valid frames, verifies CRC, manages timing, and dispatches to function code handlers.
 *        Handles events and errors, and updates diagnostic counters.
 */
void Modbus_processFunctionCode()
{
    if (modbus.state == MODBUS_RECEIVING)
    {
        /* Capture the elapsed time since the last byte received (for frame timing) */
        __disable_irq();
        modbus.timeParams.currTime = Modbus_captureCurrentTime();
        int32_t difft = modbus.timeParams.currTime - modbus.timeParams.lastTime;
        __enable_irq();

        /* Handle timer wraparound for inter-frame delay calculation */
        if (difft < 0)
            difft += DL_Timer_getLoadValue(modbus.timer);

        /* Check for minimum frame size and valid target or broadcast address */
        if ((modbus.commVariables.rxCount >= MODBUS_MIN_ADU_SIZE) &&
           ((modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX] == modbus.targetID) ||
            (modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX] == MODBUS_BROADCAST_ADDR)))
        {
            /* If sufficient silence (3.5 char times) detected, a full frame has been received */
            if (difft >= modbus.timeParams.ct3_5)
            {
                /* Increment total detected message counter (for diagnostics) */
                modbus.diagnostics.msgDetectedCounter++;

                /* Prepare event log byte for this message */
                uint8_t eventByte = MODBUS_EVNT_RCV;
                if (modbus.diagnostics.characterOvrnFlag)
                    eventByte |= MODBUS_EVNT_RCV_CHRCTR_OVERRUN;
                if (modbus.diagnostics.listenOnlyMode)
                    eventByte |= MODBUS_EVNT_RCV_LSTN_ONLY_MD;
                if (modbus.commVariables.rxPacket[MODBUS_TARGET_ADDR_IDX] == MODBUS_BROADCAST_ADDR)
                    eventByte |= MODBUS_EVNT_RCV_BRDCST;

                /* Validate CRC of the received frame */
                uint16_t vcrc = Modbus_calculateCRC((uint8_t *)modbus.commVariables.rxPacket, modbus.commVariables.rxCount - MODBUS_RTU_CRC_LEN);
                if (!(((MODBUS_16BITMASK_TO_GET_LAST_BYTE & vcrc) == modbus.commVariables.rxPacket[modbus.commVariables.rxCount - 2]) &&
                      ((MODBUS_16BITMASK_TO_GET_LAST_BYTE & (vcrc >> 8)) == modbus.commVariables.rxPacket[modbus.commVariables.rxCount - 1])))
                {
                    __disable_irq();
                    /* CRC error: clear RX, increment CRC error counter, log event */
                    modbus.commVariables.rxCount = MODBUS_DEFAULT_CNT_VAL;
                    memset((uint8_t *)modbus.commVariables.rxPacket, MODBUS_DEFAULT_PKT_VAL, MODBUS_MAX_ADU_SIZE);
                    __enable_irq();
                    modbus.diagnostics.crcErrorCounter++;
                    eventByte |= MODBUS_EVNT_RCV_COMM_ERR;
                    Modbus_insertEventLog(eventByte);
                    return;
                }

                /* Frame is valid: move to processing state and log the event */
                modbus.state = MODBUS_PROCESSING;
                Modbus_insertEventLog(eventByte);

                /* Dispatch based on function code to the appropriate handler */
                MODBUS_FunctionCode functionCode = modbus.commVariables.rxPacket[MODBUS_FUNCTION_CODE_IDX];
                switch (functionCode)
                {
                    case MODBUS_READ_DISCRETE_INPUTS:
                        Modbus_readDiscreteInputs();
                        break;
                    case MODBUS_READ_COILS:
                        Modbus_readCoils();
                        break;
                    case MODBUS_WRITE_SINGLE_COIL:
                        Modbus_writeSingleCoil();
                        break;
                    case MODBUS_WRITE_MULTIPLE_COILS:
                        Modbus_writeMultipleCoils();
                        break;
                    case MODBUS_READ_INPUT_REGISTERS:
                        Modbus_readInputRegisters();
                        break;
                    case MODBUS_READ_HOLDING_REGISTERS:
                        Modbus_readHoldingRegisters();
                        break;
                    case MODBUS_WRITE_SINGLE_REGISTER:
                        Modbus_writeSingleRegister();
                        break;
                    case MODBUS_WRITE_MULTIPLE_REGISTERS:
                        Modbus_writeMultipleRegisters();
                        break;
                    case MODBUS_RW_MULTIPLE_REGISTERS:
                        Modbus_readWriteMultipleRegisters();
                        break;
                    case MODBUS_MASK_WRITE_REGISTER:
                        Modbus_maskWriteRegister();
                        break;
                    case MODBUS_READ_FIFO_QUEUE:
                        Modbus_readQueue();
                        break;
                    case MODBUS_READ_FILE_RECORDS:
                        Modbus_readFileRecord();
                        break;
                    case MODBUS_DIAGNOSTICS:
                        Modbus_diagnostics();
                        break;
                    case MODBUS_WRITE_FILE_RECORDS:
                        Modbus_writeFileRecord();
                        break;
                    case MODBUS_READ_EXCEPTION_STATUS:
                        Modbus_readExceptionStatus();
                        break;
                    case MODBUS_GET_COM_EVENT_COUNTER:
                        Modbus_getCommEventCounter();
                        break;
                    case MODBUS_GET_COM_EVENT_LOGS:
                        Modbus_getCommEventLog();
                        break;
                    case MODBUS_REPORT_SERVER_ID:
                        Modbus_reportServerID();
                        break;
                    case MODBUS_MISC:
                        Modbus_misc();
                        break;
                    default:
                        /* Check if it is a user-defined function code */
                        if(Modbus_isUserDefinedFunctionCode(functionCode)==MODBUS_INVALID_FUNCTION_CODE)
                        {
                            /* Unknown or unsupported function code */
                            Modbus_exceptionHandling(MODBUS_ILLEGAL_FUNCTION);
                        }
                        break;
                }

                /* Increment processed message counter (for diagnostics) */
                modbus.diagnostics.msgProcessedCounter++;
            }
        }
        else
        {
            /* After 3.5 char times of silence, reset state and clear RX buffer */
            if (difft >= modbus.timeParams.ct3_5)
            {
                modbus.state = MODBUS_IDLE;
                __disable_irq();
                modbus.commVariables.rxCount = MODBUS_DEFAULT_CNT_VAL;
                memset((uint8_t *)modbus.commVariables.rxPacket, 0, sizeof(modbus.commVariables.rxPacket));
                __disable_irq();
            }
        }
    }
}

/*!
 * @brief MODBUS UART interrupt handler.
 *        Handles RX and overrun interrupts for both controller and target.
 *        Manages RX FIFO, time stamps, and overrun diagnostics.
 */
void Modbus_IRQHandler()
{
    /* Get the type of pending UART interrupt */
    switch (DL_UART_Main_getPendingInterrupt(modbus.uart))
    {
        case DL_UART_MAIN_IIDX_RX:
            /* RX interrupt: Read all bytes in the RX FIFO */
            while (!DL_UART_isRXFIFOEmpty(modbus.uart))
            {
                if (modbus.controllerEnable == MODBUS_CONTROLLER_DISABLE)
                {
                    /* Target mode: process received byte via target RX handler */
                    Modbus_TargetRxByte(DL_UART_receiveDataBlocking(modbus.uart));
                }
                else
                {
                    /* controller mode: store received byte and update modbus.timeParams */
                    modbus.timeParams.currTime = Modbus_captureCurrentTime();
                    modbus.timeParams.lastTime = modbus.timeParams.currTime;
                    modbus.commVariables.rxPacket[modbus.commVariables.rxCount++] = DL_UART_receiveDataBlocking(modbus.uart);
                }
            }
            break;

        case DL_UART_MAIN_IIDX_OVERRUN_ERROR:
        {
            /* RX FIFO overrun interrupt: Clear FIFO and update overrun diagnostics */
            while (!DL_UART_isRXFIFOEmpty(modbus.uart))
            {
                if (modbus.controllerEnable == MODBUS_CONTROLLER_DISABLE)
                {
                    /* Target mode: process received byte via target RX handler */
                    Modbus_TargetRxByte(DL_UART_receiveDataBlocking(modbus.uart));
                }
                else
                {
                    /* controller mode: store received byte and update modbus.timeParams */
                    modbus.timeParams.currTime = Modbus_captureCurrentTime();
                    modbus.timeParams.lastTime = modbus.timeParams.currTime;
                    modbus.commVariables.rxPacket[modbus.commVariables.rxCount++] = DL_UART_receiveDataBlocking(modbus.uart);
                }
            }
            /* Increment overrun counter and set overrun flag */
            modbus.diagnostics.characterOvrnCount++;
            modbus.diagnostics.characterOvrnFlag = MODBUS_CHRCTR_OVRRN_SET;
            break;
        }

        default:
            /* Other interrupts are not handled in this routine */
            break;
    }
}
