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

#include "modbus_target.h"

/* ADC result ready flag */
volatile bool gCheckADC;

/*!
 * @brief Callback for user defined FunctionCode.
 */
void userFCCallback()
{
    /* Turn LED ON */
    DL_GPIO_clearPins(GPIO_LEDS_USER_LED_1_PORT, GPIO_LEDS_USER_LED_1_PIN);
}

/*!
 * @brief Callback for MODBUS_COILS register type.
 *        Controls user LED based on MODBUS_WRITE or returns state on MODBUS_READ.
 * @param data Pointer to data value.
 * @param op   Operation type (MODBUS_READ/MODBUS_WRITE).
 */
void coilCallback(uint16_t *data, uint8_t op)
{
    static int status = 0;
    if (op == MODBUS_WRITE)
    {
        if (*data == GPIO_HIGH)
        {
            status = GPIO_HIGH;
            /* Turn LED ON */
            DL_GPIO_clearPins(GPIO_LEDS_USER_LED_1_PORT, GPIO_LEDS_USER_LED_1_PIN);
        }
        else
        {
            status = GPIO_LOW;
            /* Turn LED OFF */
            DL_GPIO_setPins(GPIO_LEDS_USER_LED_1_PORT, GPIO_LEDS_USER_LED_1_PIN);
        }
    }
    else
    {
        /* Return LED status */
        *data = status;
    }
}

/*!
 * @brief Callback for MODBUS_DISCRETE_INPUTS register type.
 *        Reads GPIO input pin state and returns as MODBUS value.
 * @param data Pointer to data value.
 * @param op   Operation type (MODBUS_READ).
 */
void discreteIpCallback(uint16_t *data, uint8_t op)
{
    uint32_t status = DL_GPIO_readPins(GPIO_Input_PORT, GPIO_Input_PIN_0_PIN);
    *data = status ? GPIO_HIGH : GPIO_LOW;
}

/*!
 * @brief Callback for MODBUS_INPUT_REGISTER register type.
 *        Starts ADC conversion and returns result.
 *        Handles single-shot ADC conversion and waits for completion.
 * @param data Pointer to data value.
 * @param op   Operation type (MODBUS_READ).
 */
void inputRegCallback(uint16_t *data, uint8_t op)
{
    uint16_t adcResult;
    /* Enable ADC interrupt for conversion complete */
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
    gCheckADC = false;
    /* Start ADC conversion */
    DL_ADC12_startConversion(ADC12_0_INST);
    while (!gCheckADC)
    {
        /* Wait for event (low power wait until ADC conversion completes) */
        __WFE();
    }
    gCheckADC = false;
    /* Get ADC result */
    adcResult = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
    *data = adcResult;
    /* Disable ADC interrupt after conversion */
    NVIC_DisableIRQ(ADC12_0_INST_INT_IRQN);
}

/*!
 * @brief Callback for MODBUS_HOLDING_REGISTER register type.
 *        On MODBUS_WRITE, sets and starts hardware timer with value.
 *        On MODBUS_READ, returns elapsed timer value.
 * @param data Pointer to data value.
 * @param op   Operation type (MODBUS_READ/MODBUS_WRITE).
 */
void holdingRegCallback(uint16_t *data, uint8_t op)
{
    if (op == MODBUS_WRITE) {
        /* Stop timer before updating */
        DL_Timer_stopCounter(TIMER_1_INST);
        /* Set new timer count */
        DL_Timer_setLoadValue(TIMER_1_INST, *data);
        /* Start timer */
        DL_Timer_startCounter(TIMER_1_INST);
    }
    else
    {
        /* Return elapsed timer value */
        *data = TIMER_1_INST_LOAD_VALUE - DL_Timer_getTimerCount(TIMER_1_INST);
    }
}

/*!
 * @brief Callback for restarting communication.
 *        Example: Re-initializes UART interrupt (can be extended for full UART re-init).
 */
void funcRestart(void)
{
    NVIC_DisableIRQ(MODBUS_UART_INT_IRQ);
    NVIC_EnableIRQ(MODBUS_UART_INT_IRQ);
}

/*!
 * @brief Main entry point for MODBUS RTU target application.
 *        Initializes hardware, registers device callbacks, and enters main loop to process MODBUS frames.
 *        Note: User must set baud rate, CPU clock frequency, and timer frequency correctly,
 *        as configured in SysConfig as these values are used to calculate MODBUS timer values.
 */
int main(void)
{
    /* Initialize system clocks, peripherals, and board configuration */
    SYSCFG_DL_init();

    /* Initialize the modbus instance */
    Modbus_init();

    /* Set UART baud rate */
    Modbus_setUartBaudRate(MODBUS_UART_BAUD_RATE);

    /* Set timer frequency */
    Modbus_setTimerClkFreq(MODBUS_TIMER_FREQ);

    /* Set CPU frequency */
    Modbus_setCpuClkFreq(MODBUS_CPU_CLK_FREQ);

    /* Configure MODBUS target with timer, UART and other instances. */
    Modbus_configTarget(MODBUS_TIMER_INST, MODBUS_UART_INST);

    /* Set MODBUS target address */
    Modbus_setTargetID(MODBUS_TARGET_ID);

    /* Register a restart communication callback */
    Modbus_configRestartComm(funcRestart);

    /* Set write timeout time */
    Modbus_setWriteTimeoutTimer(MODBUS_WRITE_TIMEOUT_TIME);

    /* Running status of target */
    Modbus_setServerStatus(MODBUS_TARGET_STATUS);

    /* Register callback for coil device */
    uint8_t status;
    status = Modbus_configDevice(MODBUS_COIL_ADDR, MODBUS_COILS, coilCallback);
    if (status == MODBUS_TARGET_DEVICE_INIT_ERR)
    {
        /* Device not initialized: coil type */
        __BKPT(0);
    }

    /* Register callback for discrete input device */
    status = Modbus_configDevice(MODBUS_DISCRETE_INPUT_ADDR, MODBUS_DISCRETE_INPUTS, discreteIpCallback);
    if (status == MODBUS_TARGET_DEVICE_INIT_ERR)
    {
        /* Device not initialized: discrete input type */
        __BKPT(0);
    }

    /* Register callback for input device */
    status = Modbus_configDevice(MODBUS_INPUT_REG_ADDR, MODBUS_INPUT_REG, inputRegCallback);
    if (status == MODBUS_TARGET_DEVICE_INIT_ERR)
    {
        /* Device not initialized: input type */
        __BKPT(0);
    }

    /* Register callback for holding device */
    status = Modbus_configDevice(MODBUS_HOLDING_REG_ADDR, MODBUS_HOLDING_REG, holdingRegCallback);
    if (status == MODBUS_TARGET_DEVICE_INIT_ERR)
    {
        /* Device not initialized: holding type */
        __BKPT(0);
    }

    /* Register User Defined Function Code */
    status = Modbus_configUserFunctionCode(userFCCallback,MODBUS_USER_FUNCTION_CODE);
    if (status == MODBUS_USER_FUNCTION_INIT_ERR)
    {
        /* User defined function code not initialized */
        __BKPT(0);
    }

    /* Enable UART interrupt for MODBUS */
    NVIC_ClearPendingIRQ(MODBUS_UART_INT_IRQ);
    NVIC_EnableIRQ(MODBUS_UART_INT_IRQ);

    /* Main loop to process MODBUS frames */
    while (1)
    {
        Modbus_processFunctionCode();
    }
}

/*!
 * @brief ADC interrupt handler.
 *        Sets flag when ADC conversion is complete (used in MODBUS input register callback).
 */
void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
            /* Signal main loop that ADC result is ready */
            gCheckADC = true;
            break;
        default:
            break;
    }
}

/*!
 * @brief UART interrupt handler.
 *        Delegates to MODBUS protocol handler for UART RX/TX and error handling.
 */
void UART_0_INST_IRQHandler(void)
{
    Modbus_IRQHandler();
}