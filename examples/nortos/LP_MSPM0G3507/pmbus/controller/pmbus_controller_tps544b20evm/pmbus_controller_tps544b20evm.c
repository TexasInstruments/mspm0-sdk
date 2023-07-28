/* --COPYRIGHT--,BSD
 * Copyright (c) 2023, Texas Instruments Incorporated
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
/*
 * ======== main.c ========
 * PMBus_controller_TPS544B20
 *
 * This example configures some parameters of TPS544B20 using PMBus showing
 *  basic calls of PMBusLib functions.
 *
 *
 *                               /|\  /|\
 *                               Rp    Rp
 *                Controller      |    |         Target
 *                  MSPM0         |    |        TPS544B20
 *            -----------------   |    |      -------------
 *           |             SDA*|<-|----+----> |DATA       |
 *           |                 |  |           |           |--> VOUT
 *           |             SCL*|<-+---------> |CLK        |
 *           |                 |              |           |
 *   LED <-- |PA0              |              |           |
 *           |                 |              |           |
 *
 *           * Check SysConfig for I/Os used for each device
 * ---------------------------------------------------------------------------*/

#include <stdint.h>
#include <ti/pmbus/pmbus.h>

#include "ti_msp_dl_config.h"

#define TPS544C20_ADDRESS 0x1B      /*! Slave address of TPS544C20 device */


/*! Macro used to check the output of each command.
 *  For demo purposes, the device will stay in a loop with both LEDs ON if an
 *  error was detected (i.e. timeout, NACK, etc)
 */
#define CHECK_RET() {if(ret != PMBUS_RET_OK) \
                     {DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_D0_PIN); while(1) {; } \
                     } \
}

/*! Buffer for command to slave */
static uint8_t Command_Buff[PMB_MAX_PACKET_SIZE + 4];
/*! Buffer for response from slave device */
static uint8_t Resp_Buff[PMB_MAX_PACKET_SIZE + 4];

//*****************************************************************************
//
//! Main Function
//!
//! This example configures a few registers of TPS544C20 just to show how to
//!  use PMBusLib.
//! At the end, the output of TPS544C20 is enabled
//!
//! \return none
//
// *****************************************************************************
int main(void)
{
    uint32_t timeout = 100000;
    int8_t ret;

    SYSCFG_DL_init();

    NVIC_ClearPendingIRQ(PMB_I2C_INST_INT_IRQN);
    NVIC_EnableIRQ(PMB_I2C_INST_INT_IRQN);

    // Initialize PMBus interface
    PMBus_init(PMB_I2C_INST, (32 * 1000000));



    // If desired, enable PEC. The TPS544B20 device will accept commands with or
    // without PEC, but it will only check PEC if it's sent/requested
    PMBus_enablePEC();



    // Verify the ID of the device (0x0143 for TPS544B20)
    // Note that DEVICE_CODE (0xFC) is manufacturer specific but it's accessed
    // with Read_Word/Write_Word commands.
    // So, we call the specific ReadWord function directly
    if((PMBus_cmdReadWord(TPS544C20_ADDRESS,
                          0xFC,
                          Resp_Buff,
                          timeout) != PMBUS_RET_OK)
       || (Resp_Buff[0] != 0x43)
       || (Resp_Buff[1] != 0x01))
    {
        // Stay in loop with LEDs ON if device didn't respond OK
        __BKPT(0);
        DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_D0_PIN);
        while(1)
        {
            ;
        }
    }

    // Write VIN_OFF 4.00 V: Minimum allowed value for VIN_OFF
    // VIN_OFF is a standard PMBus command defined in PMBus_Commands[], so
    // we can call a more generic PMBus_cmdWrite.
    // Optionally, we could call the specific PMBus_cmdWriteWord.
    Command_Buff[0] = 0x10; //LSB
    Command_Buff[1] = 0xF0; //MSB
    ret = PMBus_cmdWrite(TPS544C20_ADDRESS,
                         PMB_VIN_OFF,
                         Command_Buff,
                         2,
                         timeout);
    CHECK_RET();

    // Write VIN_ON 4.25 V
    Command_Buff[0] = 0x11; //LSB
    Command_Buff[1] = 0xF0; //MSB
    ret = PMBus_cmdWrite(TPS544C20_ADDRESS,
                         PMB_VIN_ON,
                         Command_Buff,
                         2,
                         timeout);
    CHECK_RET();

    // Write IOUT_OC_WARN_LIMIT 4.0 A
    Command_Buff[0] = 0x08; //LSB
    Command_Buff[1] = 0xF8; //MSB
    ret = PMBus_cmdWrite(TPS544C20_ADDRESS,
                         PMB_IOUT_OC_WARN_LIMIT,
                         Command_Buff,
                         2,
                         timeout);
    CHECK_RET();

    // Write ON_OFF_CONFIG Mode: OPERATION Only
    Command_Buff[0] = 0x1A;
    ret = PMBus_cmdWrite(TPS544C20_ADDRESS,
                         PMB_ON_OFF_CONFIG,
                         Command_Buff,
                         1,
                         timeout);
    CHECK_RET();

    // Write OPERATION register to enable the output, margin off
    Command_Buff[0] = 0x80;
    ret = PMBus_cmdWrite(TPS544C20_ADDRESS,
                         PMB_OPERATION,
                         Command_Buff,
                         1,
                         timeout);
    CHECK_RET();

    while(1)
    {
        // toggle LED0 if everything executed OK
        DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_D0_PIN);
        delay_cycles(200 * 32 * 1000); // 200ms
    }
}

//
// Events
//
//*****************************************************************************
//
//! I2C Event
//!
//! Event called when I2C TX, RX, Start, Stop and errors are detected
//!
//
// *****************************************************************************
void PMB_I2C_INST_IRQHandler(void) {
    PMBus_processInt();
}
