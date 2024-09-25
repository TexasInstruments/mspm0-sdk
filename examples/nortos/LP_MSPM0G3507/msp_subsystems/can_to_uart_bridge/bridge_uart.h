#ifndef BRIDGE_UART_H_
#define BRIDGE_UART_H_

/* Variables for UART receive */
#define UART_RX_SIZE   (72)
extern Custom_Element gUART_RX_Element;
extern uint8_t gUartReceiveGroup[UART_RX_SIZE];
bool getUartRxMsg(uint8_t *UartReceiveGroup);
bool processUartRxMsg(uint8_t *UartReceiveGroup, Custom_Element *gUART_RX_Element);

/* Variables for UART Transmit */
#define UART_TX_SIZE   (78) //13+64=78
extern Custom_Element gUART_TX_Element;
extern uint8_t gUartTransmitGroup[UART_TX_SIZE];
extern bool gUartTxflag;
bool sendUartTxMsg(uint8_t *UartTransmitGroup, uint16_t lenth);
bool processUartTxMsg(uint8_t *UartTransmitGroup, Custom_Element *gUART_TX_Element);
void transmitPacketBlocking(uint8_t *buffer, uint32_t size);


#endif /* BRIDGE_UART_H_ */
