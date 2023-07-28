#include "lin_config.h"
#include <stdio.h>

uint8_t gMessageTableIndex = 0;

void LIN_Commander_receiveMessage(UART_Regs *uart, uint8_t rxByte,
    uint8_t *msgBuffer, LIN_table_record_t *messageTable)
{
    static uint8_t byteCounter = 0;
    static LIN_word_t tempChksum;
    static LIN_RX_STATE LIN_state = LIN_RX_STATE_DATA;
    uint8_t checksum;
    uint8_t rxChecksum;
    LIN_function_ptr_t callbackFunction;

    switch (LIN_state) {
        case LIN_RX_STATE_DATA:
            /* Save the received byte to the RX message buffer */
            msgBuffer[byteCounter] = rxByte;
            /* Use received byte to add to checksum calculation */
            tempChksum.word += msgBuffer[byteCounter];

            /*
            * Check if the number of received bytes is greater than the expected
            * number of data bytes. If yes, then the checksum byte was just
            * just received.
            */
            byteCounter++;
            if (byteCounter >= messageTable[gMessageTableIndex].msgSize) {
                LIN_state = LIN_RX_STATE_CHECKSUM;
            }
            break;

        case LIN_RX_STATE_CHECKSUM:
            rxChecksum = rxByte;

            /* Calculate the checksum based on the received data */
            tempChksum.word += messageTable[gMessageTableIndex].msgID;
            tempChksum.word = tempChksum.byte[0] + tempChksum.byte[1];
            checksum        = tempChksum.byte[0];
            checksum += tempChksum.byte[1];
            checksum = 0xFF - checksum;

            /* Compare the received checksum to the calculated checksum */
            if (rxChecksum == checksum) {
                /* If they match, call the corresponding callback function */
                callbackFunction =
                    messageTable[gMessageTableIndex].callbackFunction;
                callbackFunction();
            }

            /* Reset the data */
            byteCounter     = 0;
            tempChksum.word = 0;
            LIN_state       = LIN_RX_STATE_DATA;
            break;

        default:
            LIN_state = LIN_RX_STATE_DATA;
            break;
    }
}

void LIN_Commander_transmitMessage(UART_Regs *uart, uint8_t tableIndex,
    uint8_t *buffer, LIN_table_record_t *messageTable)
{
    uint8_t locIndex;
    uint8_t checksum;
    LIN_word_t tempChksum;

    tempChksum.word = 0;

    /* Disable LIN message reception */
    DL_UART_Extend_disableInterrupt(uart, DL_UART_EXTEND_INTERRUPT_RX);

    /* Transmit BREAK, SYNC byte, and PID */
    DL_UART_enableLINSendBreak(uart);
    delay_cycles(LIN_BREAK_LENGTH); /* Set break for ~1ms */
    DL_UART_disableLINSendBreak(uart);
    DL_UART_Extend_transmitDataBlocking(uart, LIN_SYNC_BYTE);
    DL_UART_Extend_transmitDataBlocking(uart, messageTable[tableIndex].msgID);

    tempChksum.word = messageTable[tableIndex].msgID;

    /*
     * If a callback function is present corresponding to the message PID that
     * was transmitted, then we do not transmit data, but expect to receive
     * data back from the Responder.
     */
    if (messageTable[tableIndex].callbackFunction != NULL) {
        /* Clear pending RX Interrupts and the RX buffer */
        DL_UART_Extend_receiveData(uart);
        DL_UART_Extend_clearInterruptStatus(uart, DL_UART_EXTEND_INTERRUPT_RX);

        /* Keep track of which message PID we are using */
        gMessageTableIndex = tableIndex;

        DL_UART_Extend_enableInterrupt(uart, DL_UART_EXTEND_INTERRUPT_RX);

    }

    else {
        /*
         * There is no callback associated with the message PID, so we
         * transmit a data packet to the Responder
         */
        for (locIndex = 0; locIndex < messageTable[tableIndex].msgSize;
             locIndex++) {
            DL_UART_Extend_transmitDataBlocking(uart, buffer[locIndex]);
            tempChksum.word += buffer[locIndex];
        }

        /* Calculate and then transmit the checksum*/
        tempChksum.word = tempChksum.byte[0] + tempChksum.byte[1];

        checksum = tempChksum.byte[0];
        checksum += tempChksum.byte[1];
        checksum = 0xFF - checksum;

        DL_UART_Extend_transmitDataBlocking(uart, checksum);
    }
}
