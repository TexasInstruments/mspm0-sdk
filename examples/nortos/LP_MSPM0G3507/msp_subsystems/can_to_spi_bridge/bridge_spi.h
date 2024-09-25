#ifndef BRIDGE_SPI_H_
#define BRIDGE_SPI_H_

/* Variables for UART receive */
#define SPI_RX_SIZE   (72)
extern Custom_Element gSPI_RX_Element;
extern uint8_t gSpiReceiveGroup[SPI_RX_SIZE];
bool getSpiRxMsg(uint8_t *SpiReceiveGroup);
bool processSpiRxMsg(uint8_t *SpiReceiveGroup, Custom_Element *SPI_RX_Element);

/* Variables for UART transmit */
#define SPI_TX_SIZE   (78) //13+64=78
extern Custom_Element gSPI_TX_Element;
extern uint8_t gSpiTransmitGroup[SPI_TX_SIZE];
extern bool gSpiTxflag;
bool sendSpiTxMsg(uint8_t *SpiTransmitGroup, uint16_t length);
bool processSpiTxMsg(uint8_t *SpiTransmitGroup, Custom_Element *SPI_TX_Element);

#endif /* BRIDGE_SPI_H_ */
