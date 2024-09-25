#ifndef USER_DEFINE_H_
#define USER_DEFINE_H_

/* User can test different CAN config */
//#define CAN_ID_EXTEND

/*user-defined information storage structure */
typedef struct {
    /*! Identifier */
    uint32_t id;
    /*! Data Length Code */
    uint32_t dlc;
    /*! Data bytes */
    uint16_t data[64];
} Custom_Element;


#include "bridge_can.h"
#include "bridge_uart.h"


#endif /* USER_DEFINE_H_ */
