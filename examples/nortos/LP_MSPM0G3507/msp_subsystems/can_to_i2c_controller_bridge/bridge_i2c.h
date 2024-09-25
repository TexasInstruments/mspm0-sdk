#ifndef BRIDGE_I2C_H_
#define BRIDGE_I2C_H_

/* Variables for I2C receive */
#define I2C_RX_SIZE   (72)
extern Custom_Element gI2C_RX_Element;
extern uint8_t gI2CReceiveGroup[I2C_RX_SIZE];
bool readI2CRxMsg_controller(uint16_t length);
bool getI2CRxMsg_controller(uint8_t *I2CReceiveGroup);
bool getI2CRxMsg_target(uint8_t *I2CReceiveGroup);
bool processI2CRxMsg(uint8_t *I2CReceiveGroup, Custom_Element *I2C_RX_Element);

/* Variables for I2C Transmit */
#define I2C_TX_SIZE   (78) //13+64=78
extern Custom_Element gI2C_TX_Element;
extern uint8_t gI2CTransmitGroup[I2C_TX_SIZE];
extern bool gI2CTxflag;
extern uint32_t gTxLen, gTxCount;
bool sendI2CTxMsg_controller(uint8_t *I2CTransmitGroup);
bool sendI2CTxMsg_target(uint8_t *I2CTransmitGroup);
bool processI2CTxMsg(uint8_t *I2CTransmitGroup, Custom_Element *I2C_TX_Element);

#endif /* BRIDGE_I2C_H_ */
