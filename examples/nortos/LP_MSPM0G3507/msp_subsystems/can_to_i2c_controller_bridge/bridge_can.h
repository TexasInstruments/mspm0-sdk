#ifndef BRIDGE_CAN_H_
#define BRIDGE_CAN_H_

/* Variables for CAN receive */
extern DL_MCAN_RxBufElement rxMsg;
extern Custom_Element gCAN_RX_Element;
extern volatile uint32_t gInterruptLine1Status;
bool getCANRxMsg(DL_MCAN_RxBufElement *rxMsg);
bool processCANRxMsg(DL_MCAN_RxBufElement *rxMsg, Custom_Element *gCAN_RX_Element);

/* Variables for CAN transit */
extern DL_MCAN_TxBufElement txMsg0;
extern Custom_Element gCAN_TX_Element;
bool sendCANTxMsg(DL_MCAN_TxBufElement *txMsg0);
bool processCANTxMsg(DL_MCAN_TxBufElement *txMsg0, Custom_Element *gCAN_TX_Element);

#endif /* BRIDGE_CAN_H_ */
