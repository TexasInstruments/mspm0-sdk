#include <i2c.h>
#include "bsl.h"
#include "stdint.h"
#include "ti_msp_dl_config.h"

uint8_t test_d;
uint8_t I2C_writeBuffer(uint8_t *pData, uint8_t ui8Cnt)
{
    uint8_t gTxCount;

    gTxCount = DL_I2C_fillControllerTXFIFO(I2C_0_INST, pData, ui8Cnt);
    gI2cControllerStatus = I2C_STATUS_TX_STARTED;
    while (!(DL_I2C_getControllerStatus(I2C_0_INST) &
             DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    DL_I2C_startControllerTransfer(I2C_0_INST, I2C_TARGET_ADDRESS,
        DL_I2C_CONTROLLER_DIRECTION_TX, ui8Cnt);
    while (gTxCount < ui8Cnt) {
        while (DL_I2C_getRawInterruptStatus(
                   I2C_0_INST, DL_I2C_INTERRUPT_CONTROLLER_TXFIFO_EMPTY) == 0)
            ;
        gTxCount += DL_I2C_fillControllerTXFIFO(
            I2C_0_INST, pData + gTxCount, ui8Cnt - gTxCount);
    }
    /* Poll until the Controller writes all bytes */
    while (DL_I2C_getControllerStatus(I2C_0_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;
    /* Trap if there was an error */
    if (DL_I2C_getControllerStatus(I2C_0_INST) &
        DL_I2C_CONTROLLER_STATUS_ERROR) {
        /* LED will remain high if there is an error */
        return 1;
    }
    // res = DL_UART_receiveDataBlocking(UART_0_INST);
    return 0;
}

void I2C_readBuffer(uint8_t *pData, uint8_t ui8Cnt)
{
    /* Wait for I2C to be Idle */
    while (!(DL_I2C_getControllerStatus(I2C_0_INST) &
             DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    /* Send a read request to Target */
    DL_I2C_startControllerTransfer(I2C_0_INST, I2C_TARGET_ADDRESS,
        DL_I2C_CONTROLLER_DIRECTION_RX, ui8Cnt);

    /*
     * Receive all bytes from target. LED will remain high if not all bytes
     * are received
     */
    for (uint8_t i = 0; i < ui8Cnt; i++) {
        while (DL_I2C_isControllerRXFIFOEmpty(I2C_0_INST))
            ;
        *pData = DL_I2C_receiveControllerData(I2C_0_INST);
        pData++;
    }
}

//uint8_t test_d;

uint8_t Status_check(void)
{
    i2c_error_t i2c_ack;
    uint8_t buf[1] = {0xBB};
    i2c_ack        = I2C_writeBuffer(buf, 1);
    I2C_readBuffer(BSL_RX_buffer, 1);
    return BSL_RX_buffer[0];
}
