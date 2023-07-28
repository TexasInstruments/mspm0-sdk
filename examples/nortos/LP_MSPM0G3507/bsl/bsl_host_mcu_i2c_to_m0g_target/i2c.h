#include "stdint.h"

#define TIMEOUT_COUNT 1500000
#define I2C_TARGET_ADDRESS (0x48)

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

void Host_BSL_entry_software(void);
uint8_t Status_check(void);
void BSL_sendSingleByte(uint8_t ui8Byte);
uint8_t BSL_getResponse(void);
uint8_t I2C_writeBuffer(uint8_t *pData, uint8_t ui8Cnt);
void I2C_readBuffer(uint8_t *pData, uint8_t ui8Cnt);
