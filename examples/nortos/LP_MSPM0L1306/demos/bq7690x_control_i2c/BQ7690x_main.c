/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
 */
/*
 * BQ7690x demo code for MSPM0 + BQ76907EVM
 *
 *  Connection description: The I2C SCL and SDA pins are the only pin
 * connections required between the MSPM0 board and the BQ7690xEVM for this
 * demo code. Also a ground connection should be made between the 2 boards.
 *
 */

#include <stdint.h>
#include <stdio.h>
#include "BQ7690x.h"
#include "BQ7690x_functions.h"
#include "BQ7690x_i2c.h"
#include "ti_msp_dl_config.h"

/*----------------------------------------------------------
 * ---------------------------------------------------------*/

/********* Main Sequence *********/
int main(void)
{
    // local variables

    int debugCounter       = 10;  // Counter for debugging
    int i                  = 0;
    uint16_t regRead       = 0x00;
    int16_t CellVoltage[7] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    // initialization

    /* Power on GPIO, initialize pins as digital outputs */

    SYSCFG_DL_init();

    /* Init I2C IRQ */

    DL_SYSCTL_disableSleepOnExit();
    delayUS(300);
    NVIC_EnableIRQ(I2C_INST_INT_IRQN);
    DL_I2C_enableControllerClockStretching(I2C_INST);

    /* Default: LED1 and LED3 ON, LED2 OFF */

    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_2_PIN);

    DL_GPIO_setPins(
        GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN | GPIO_LEDS_USER_LED_3_PIN);

    /* Communicate with BQ7690x */

    printf("\nStarting Test Sequence with BQ7690x..\n\n");

    /* I2C Write Example */

    DirectCommands(AlarmEnable, 0x00, R);  // Read AlarmEnable Configuration
    regRead = BQ7690x_Get_RX_data();
    printf("AlarmEnable Pre Register =  %4x\n", regRead);
    delay_cycles(2000);

    DirectCommands(AlarmEnable, 0x1EA6, W);  // Write AlarmEnable Configuration
    delay_cycles(2000);

    DirectCommands(AlarmEnable, 0x00, R);  // Read AlarmEnable Configuration
    regRead = BQ7690x_Get_RX_data();
    printf("AlarmEnable Post Register =  %4x\n", regRead);
    delay_cycles(2000);

    /* Command Subcommand Example */

    CommandSubcommands(RESET);  // Reset Device
    printf("Resetting Device..\n");
    delay_cycles(50000);

    DirectCommands(AlarmEnable, 0x00, R);  // Read AlarmEnable Configuration
    regRead = BQ7690x_Get_RX_data();
    printf("AlarmEnable After Reset =  %4x\n", regRead);
    delay_cycles(5000);

    /* ReadAllVoltages Example */

    BQ7690x_ReadAllVoltages();

    BQ7690x_Get_CellVoltages(CellVoltage);  // Cell Voltages
    for (i = 0; i < 7; i++) {
        printf("Cell%1i Voltage Register = %4i mV\n", i + 1, CellVoltage[i]);
        delay_cycles(2000);
    }

    printf("Stack Voltage = %4i mV\n",
        BQ7690x_Get_Stack_Voltage());  // Stack Voltage
    delay_cycles(2000);

    printf("REG18 Voltage = %4i mV\n",
        BQ7690x_Get_REG18_Voltage());  // REG18 Voltage
    delay_cycles(2000);

    printf(
        "VSS Voltage = %4i mV\n", BQ7690x_Get_VSS_Voltage());  // VSS Voltage
    delay_cycles(2000);

    /* Read CC1 and CC2 Current Example */

    BQ7690x_ReadCurrent();
    printf(
        "CC2 Current = %4i mA\n", BQ7690x_Get_CC2_Current());  // CC2 Current
    delay_cycles(2000);

    BQ7690x_ReadCC1Current();
    printf(
        "CC1 Current = %4i mA\n", BQ7690x_Get_CC1_Current());  // CC1 Current
    delay_cycles(2000);

    /* Read Internal Temperature Example */

    BQ7690x_ReadIntTemperature();
    printf("Internal Temperature = %4i ºC\n",
        BQ7690x_Get_IntTemp());  // Internal Temperature
    delay_cycles(2000);

    /* Disable SLEEP mode Example */

    BQ7690x_Sleep_Disable();
    delay_cycles(2000);

    printf("\nTest Sequence with BQ7690x Completed.\n");

    /* Main loop - toggle MSP-M0 LEDs */

    while (true) {
        // Call togglePins API to flip the current value of LEDs 1-3.
        delay_cycles(10000000);  // Loop Delay
        DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN |
                                               GPIO_LEDS_USER_LED_2_PIN |
                                               GPIO_LEDS_USER_LED_3_PIN);
    }
}

/*----------------------------------------------------------
 * ---------------------------------------------------------*/
