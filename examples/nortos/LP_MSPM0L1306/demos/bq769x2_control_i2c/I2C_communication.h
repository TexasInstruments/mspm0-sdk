#ifndef I2C_COMMUNICATION_H_
#define I2C_COMMUNICATION_H_

/* Maximum size of TX packet */
#define I2C_TX_MAX_PACKET_SIZE (16)

/* Number of bytes to send to target device */
#define I2C_TX_PACKET_SIZE (16)

/* Maximum size of RX packet */
#define I2C_RX_MAX_PACKET_SIZE (16)

/* Number of bytes to received from target */
#define I2C_RX_PACKET_SIZE (16)

/* I2C Target address */
#define I2C_TARGET_ADDRESS \
    (0x08)  // BQ769x2 address is 0x10 including R/W bit or 0x8 as 7-bit address

#include <stdbool.h>
#include <stdint.h>
#include "ti_msp_dl_config.h"

//************I2C State Machine **********************
/* Indicates status of I2C */
enum I2cControllerStatus {
    I2C_STATUS_IDLE = 0,
    I2C_STATUS_TX_STARTED,
    I2C_STATUS_TX_INPROGRESS,
    I2C_STATUS_TX_COMPLETE,
    I2C_STATUS_RX_STARTED,
    I2C_STATUS_RX_INPROGRESS,
    I2C_STATUS_RX_COMPLETE,
    I2C_STATUS_ERROR,
} gI2cControllerStatus;

void I2C_WriteReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count);
void I2C_ReadReg(uint8_t reg_addr, uint8_t *reg_data, uint8_t count);

#endif /* I2C_COMMUNICATION_H_ */
