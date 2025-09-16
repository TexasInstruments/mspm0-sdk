/* --COPYRIGHT--,BSD
 * Copyright (c) 2025, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

#include "modbus_controller.h"

/* Data buffer for MODBUS ADU transmission and reception */
uint8_t data[MODBUS_MAX_ADU_SIZE];

/* Length of valid data in the buffer */
uint8_t dataLen = 0;

/* Exception code returned by last MODBUS operation */
uint8_t expCode = 0;

/* Counter for failed responses */
uint8_t errorResponseCounter = 0;

/*!
 * @brief Checks the response exception code from the last MODBUS operation.
 *        - If successful (expCode == MODBUS_NO_EXCEPTION), toggles the user LED.
 *        - Otherwise, increments the error counter.
 *        A breakpoint is placed for debugging to inspect the data buffer.
 */
void checkResp()
{
    /* Check MODBUS response and indicate result */
    if (expCode == MODBUS_NO_EXCEPTION)
    {
        /* Toggle user LED to indicate success */
        DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        /* Delay for visual indication */
        delay_cycles(MODBUS_LED_DELAY_IN_RSP);
        DL_GPIO_setPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
    }
    else
    {
        /* Increment error counter on exception */
        errorResponseCounter++;
    }
    /* Breakpoint to analyze the result */
    /* User can match the expected and actual result by looking at the contents of data. */
    __BKPT(0);
}

/*!
 * @brief Runs a sequence of MODBUS controller tests.
 *        Each test sends a MODBUS request, checks the response, and provides visual feedback via the user LED.
 *        After all tests are complete, toggles the LED in an infinite loop to indicate completion.
 */
void Modbus_runTests()
{
    /* Start with WRITE_COIL test */
    Modbus_Tests modbusTest = WRITE_COIL;

    while (1)
    {
        switch (modbusTest)
        {
            /* Each case prepares the data buffer and sends a MODBUS request */
            case WRITE_COIL:
                data[0] = 0x00; data[1] = 0x00; data[2] = 0xFF; data[3] = 0x00;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_WRITE_SINGLE_COIL, data, dataLen, &expCode);
                break;
            case READ_COIL1:
                data[0] = 0x00; data[1] = 0x00; data[2] = 0x00; data[3] = 0x01;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_READ_COILS, data, dataLen, &expCode);
                break;
            case WRITE_MULTIPLE_COIL:
                data[0] = 0x00; data[1] = 0x00; data[2] = 0x00; data[3] = 0x01;
                data[4] = 0x01; data[5] = 0x00;
                dataLen = 6;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_WRITE_MULTIPLE_COILS, data, dataLen, &expCode);
                break;
            case READ_COIL2:
                data[0] = 0x00; data[1] = 0x00; data[2] = 0x00; data[3] = 0x01;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_READ_COILS, data, dataLen, &expCode);
                break;
            case READ_DISCRETE_IP:
                data[0] = 0x00; data[1] = 0x00; data[2] = 0x00; data[3] = 0x01;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_READ_DISCRETE_INPUTS, data, dataLen, &expCode);
                break;
            case WRITE_HOLDING_REG:
                data[0] = 0x00; data[1] = 0x00; data[2] = 0x00; data[3] = 0xFF;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_WRITE_SINGLE_REGISTER, data, dataLen, &expCode);
                break;
            case READ_HOLDING_REG1:
                data[0] = 0x00; data[1] = 0x00; data[2] = 0x00; data[3] = 0x01;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_READ_HOLDING_REGISTERS, data, dataLen, &expCode);
                break;
            case WRITE_MLP_HLD_REG:
                data[0] = 0x00; data[1] = 0x00; data[2] = 0x00; data[3] = 0x01;
                data[4] = 0x02; data[5] = 0x00; data[6] = 0xFF;
                dataLen = 7;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_WRITE_MULTIPLE_REGISTERS, data, dataLen, &expCode);
                break;
            case READ_HOLDING_REG2:
                data[0] = 0x00; data[1] = 0x00; data[2] = 0x00; data[3] = 0x01;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_READ_HOLDING_REGISTERS, data, dataLen, &expCode);
                break;
            case RW_MLP_HLD_REG:
                data[0] = 0x00; data[1] = 0x00; data[2] = 0x00; data[3] = 0x01;
                data[4] = 0x00; data[5] = 0x00; data[6] = 0x00; data[7] = 0x01;
                data[8] = 0x02; data[9] = 0x00; data[10] = 0xFF;
                dataLen = 11;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_RW_MULTIPLE_REGISTERS, data, dataLen, &expCode);
                break;
            case MASK_WRITE_REG:
                data[0] = 0x00; data[1] = 0x00; data[2] = 0x00; data[3] = 0x00;
                data[4] = 0x00; data[5] = 0x00;
                dataLen = 6;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_MASK_WRITE_REGISTER, data, dataLen, &expCode);
                break;
            case READ_INP_REG:
                data[0] = 0x00; data[1] = 0x00; data[2] = 0x00; data[3] = 0x01;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_READ_INPUT_REGISTERS, data, dataLen, &expCode);
                break;
            case READ_FIFO_QUEUE:
                data[0] = 0x00; data[1] = 0x00;
                dataLen = 2;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_READ_FIFO_QUEUE, data, dataLen, &expCode);
                break;
            case WRITE_FILE:
                data[0] = 0x09; data[1] = 0x06; data[2] = 0x00; data[3] = 0x01;
                data[4] = 0x00; data[5] = 0x00; data[6] = 0x00; data[7] = 0x01;
                data[8] = 0xFF; data[9] = 0xFF;
                dataLen = 10;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_WRITE_FILE_RECORDS, data, dataLen, &expCode);
                break;
            case READ_FILE:
                data[0] = 0x07; data[1] = 0x06; data[2] = 0x00; data[3] = 0x01;
                data[4] = 0x00; data[5] = 0x00; data[6] = 0x00; data[7] = 0x01;
                dataLen = 8;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_READ_FILE_RECORDS, data, dataLen, &expCode);
                break;
            case READ_EXCEPTION_STATUS:
                dataLen = 0;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_READ_EXCEPTION_STATUS, data, dataLen, &expCode);
                break;
            case GET_COMM_EVENT_COUNTER:
                dataLen = 0;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_GET_COM_EVENT_COUNTER, data, dataLen, &expCode);
                break;
            case GET_COM_EVENT_LOG:
                dataLen = 0;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_GET_COM_EVENT_LOGS, data, dataLen, &expCode);
                break;
            case GET_TARGET_ID:
                dataLen = 0;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_REPORT_SERVER_ID, data, dataLen, &expCode);
                break;
            case RET_QUERY_DATA:
                data[0] = (MODBUS_RETURN_QUERY_DATA >> 8) & 0xFF;
                data[1] = MODBUS_RETURN_QUERY_DATA & 0xFF;
                data[2] = 0x12; data[3] = 0x34;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_DIAGNOSTICS, data, dataLen, &expCode);
                break;
            case RESTART_COMM1:
                data[0] = (MODBUS_RESTART_COMM_OPTION >> 8) & 0xFF;
                data[1] = MODBUS_RESTART_COMM_OPTION & 0xFF;
                data[2] = 0x00; data[3] = 0x00;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_DIAGNOSTICS, data, dataLen, &expCode);
                break;
            case RET_DIAG_REGISTER:
                data[0] = (MODBUS_RETURN_DIAG_REG >> 8) & 0xFF;
                data[1] = MODBUS_RETURN_DIAG_REG & 0xFF;
                data[2] = 0x00; data[3] = 0x00;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_DIAGNOSTICS, data, dataLen, &expCode);
                break;
            case FORCE_LISTEN_ONLY_MODE:
                data[0] = (MODBUS_FORCE_LISTEN_ONLY_MODE >> 8) & 0xFF;
                data[1] = MODBUS_FORCE_LISTEN_ONLY_MODE & 0xFF;
                data[2] = 0x00; data[3] = 0x00;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_DIAGNOSTICS, data, dataLen, &expCode);
                break;
            case RESTART_COMM2:
                data[0] = (MODBUS_RESTART_COMM_OPTION >> 8) & 0xFF;
                data[1] = MODBUS_RESTART_COMM_OPTION & 0xFF;
                data[2] = 0xFF; data[3] = 0x00;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_DIAGNOSTICS, data, dataLen, &expCode);
                break;
            case BUS_MSG_COUNT:
                data[0] = (MODBUS_RETURN_BUS_MSG_COUNT >> 8) & 0xFF;
                data[1] = MODBUS_RETURN_BUS_MSG_COUNT & 0xFF;
                data[2] = 0x00; data[3] = 0x00;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_DIAGNOSTICS, data, dataLen, &expCode);
                break;
            case BUS_COMM_ERROR_CNT:
                data[0] = (MODBUS_RETURN_BUS_COMM_ERR_CNT >> 8) & 0xFF;
                data[1] = MODBUS_RETURN_BUS_COMM_ERR_CNT & 0xFF;
                data[2] = 0x00; data[3] = 0x00;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_DIAGNOSTICS, data, dataLen, &expCode);
                break;
            /* This will cause error to increase exception count */
            case EXC_READ_COIL1:
                data[0] = 0x00; data[1] = 0x01; data[2] = 0x00; data[3] = 0x01;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_READ_COILS, data, dataLen, &expCode);
                break;
            case BUS_EXP_ERROR_CNT:
                data[0] = (MODBUS_RETURN_BUS_EXP_ERR_CNT >> 8) & 0xFF;
                data[1] = MODBUS_RETURN_BUS_EXP_ERR_CNT & 0xFF;
                data[2] = 0x00; data[3] = 0x00;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_DIAGNOSTICS, data, dataLen, &expCode);
                break;
            case SRVR_MSG_COUNT:
                data[0] = (MODBUS_RETURN_SRVR_MSG_CNT >> 8) & 0xFF;
                data[1] = MODBUS_RETURN_SRVR_MSG_CNT & 0xFF;
                data[2] = 0x00; data[3] = 0x00;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_DIAGNOSTICS, data, dataLen, &expCode);
                break;
            case SRVR_NO_RSP_CNT:
                data[0] = (MODBUS_RETURN_SRVR_NO_RSP_CNT >> 8) & 0xFF;
                data[1] = MODBUS_RETURN_SRVR_NO_RSP_CNT & 0xFF;
                data[2] = 0x00; data[3] = 0x00;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_DIAGNOSTICS, data, dataLen, &expCode);
                break;
            case SRVR_NAK_CNT:
                data[0] = (MODBUS_RETURN_SRVR_NAK_CNT >> 8) & 0xFF;
                data[1] = MODBUS_RETURN_SRVR_NAK_CNT & 0xFF;
                data[2] = 0x00; data[3] = 0x00;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_DIAGNOSTICS, data, dataLen, &expCode);
                break;
            case SRVR_BUSY_CNT:
                data[0] = (MODBUS_RETURN_SRVR_BUSY_CNT >> 8) & 0xFF;
                data[1] = MODBUS_RETURN_SRVR_BUSY_CNT & 0xFF;
                data[2] = 0x00; data[3] = 0x00;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_DIAGNOSTICS, data, dataLen, &expCode);
                break;
            case BUS_CHR_OVRRN_CNT:
                data[0] = (MODBUS_RETURN_BUS_CHR_OVRRN_CNT >> 8) & 0xFF;
                data[1] = MODBUS_RETURN_BUS_CHR_OVRRN_CNT & 0xFF;
                data[2] = 0x00; data[3] = 0x00;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_DIAGNOSTICS, data, dataLen, &expCode);
                break;
            case CLR_OVRRN_CNTR_AND_FLG:
                data[0] = (MODBUS_CLR_OVRRN_CNTR_AND_FLG >> 8) & 0xFF;
                data[1] = MODBUS_CLR_OVRRN_CNTR_AND_FLG & 0xFF;
                data[2] = 0x00; data[3] = 0x00;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_DIAGNOSTICS, data, dataLen, &expCode);
                break;
            case CLR_CNTR_AND_DIAG_REG:
                data[0] = (MODBUS_CLR_CNTR_AND_DIAG_REG >> 8) & 0xFF;
                data[1] = MODBUS_CLR_CNTR_AND_DIAG_REG & 0xFF;
                data[2] = 0x00; data[3] = 0x00;
                dataLen = 4;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_DIAGNOSTICS, data, dataLen, &expCode);
                break;
            case USER_DEFINED_FC:
                dataLen = 0;
                Modbus_sendCommand(MODBUS_TARGET_ADDR, MODBUS_USER_DEFINED_FC, data, dataLen, &expCode);
                break;
            default:
                modbusTest = WRITE_COIL;
                break;
        }
        /* Check MODBUS response and indicate result */
        checkResp();
        modbusTest++;
        if (modbusTest >= END_OF_TESTS)
        {
            /* All tests complete: toggle LED forever to indicate completion */
            while (1)
            {
                DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
                delay_cycles(MODBUS_LED_DELAY_ON_COMPLETE);
            }
        }
        /* Clear data buffer */
        memset(data, 0, MODBUS_MAX_ADU_SIZE);
        /* Delay before next test */
        delay_cycles(MODBUS_LED_DELAY_IN_INST);
    }
}

/*!
 * @brief Main entry point for MODBUS RTU controller application.
 *        Initializes hardware, sets timers, and runs a sequence of MODBUS controller tests.
 *        Each test sends a MODBUS request and checks the response.
 *        After all tests, the LED toggles in an infinite loop.
 *        Note: User must set baud rate, CPU clock frequency, and timer frequency correctly,
 *        as configured in SysConfig as these values are used to calculate MODBUS timer values.
 */
int main(void)
{
    SYSCFG_DL_init();

    Modbus_init();

    /* Set UART baud rate */
    Modbus_setUartBaudRate(MODBUS_UART_BAUD_RATE);

    /* Set timer frequency */
    Modbus_setTimerClkFreq(MODBUS_TIMER_FREQ);

    /* Set CPU frequency */
    Modbus_setCpuClkFreq(MODBUS_CPU_CLK_FREQ);

    /* Initialize MODBUS controller with timer and UART instances */
    Modbus_configController(MODBUS_TIMER_INST, MODBUS_UART_INST);

    /* Set controller turn-around timer */
    Modbus_setControllerTurnAroundTimer(MODBUS_TURNAROUND_TIMER);

    /* Set controller response timeout */
    Modbus_setControllerRspTimer(MODBUS_RESPONSE_TIMER);

    /* Set controller trials to get response */
    Modbus_setNumberOfTries(MODBUS_NUMBER_OF_TRIES);

    /* Enable UART interrupt for MODBUS */
    NVIC_ClearPendingIRQ(MODBUS_UART_INT_IRQ);
    NVIC_EnableIRQ(MODBUS_UART_INT_IRQ);

    Modbus_runTests();
}

/*!
 * @brief UART interrupt handler for MODBUS controller.
 *        Delegates to the MODBUS protocol handler.
 */
void UART_0_INST_IRQHandler(void)
{
    Modbus_IRQHandler();
}
