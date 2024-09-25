#include "lin_config.h"

void sendLINResponderTXMessage(UART_Regs *uart, uint8_t tableIndex,
    uint8_t *msgBuffer, LIN_table_record_t *responderMessageTable)
{
    uint8_t locIndex;
    uint8_t checksum;
    LIN_word_t tempChksum;

    /* Disable LIN RX */
    DL_UART_Extend_disableInterrupt(uart, DL_UART_EXTEND_INTERRUPT_RX);

    tempChksum.word = responderMessageTable[tableIndex].msgID;
    tempChksum.word = tempChksum.byte[0] + tempChksum.byte[1];

    for (locIndex = 0; locIndex < responderMessageTable[tableIndex].msgSize;
         locIndex++) {
        DL_UART_Extend_transmitDataBlocking(uart, msgBuffer[locIndex]);
        tempChksum.word += msgBuffer[locIndex];
    }
    /* Calculate and send checksum */
    checksum = tempChksum.byte[0];
    checksum += tempChksum.byte[1];
    checksum = 0xFF - checksum;

    DL_UART_Extend_transmitDataBlocking(uart, checksum);
    while (DL_UART_Extend_isBusy(uart)) {
        ;
    }

    DL_UART_Extend_receiveDataBlocking(uart);

    /* Enable LIN RX */
    DL_UART_Extend_clearInterruptStatus(uart, DL_UART_EXTEND_INTERRUPT_RX);
    DL_UART_Extend_enableInterrupt(uart, DL_UART_EXTEND_INTERRUPT_RX);
}
