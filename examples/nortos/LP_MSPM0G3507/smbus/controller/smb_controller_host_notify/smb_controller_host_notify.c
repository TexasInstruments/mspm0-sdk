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
 *+----------------------------------------------------------------------------+
 * Please refer to the Examples Guide for more details on example usage.
 * ---------------------------------------------------------------------------*/


#include <stdint.h>

#include "ti_msp_dl_config.h"

#include <ti/smbus/smbus.h>
#include "SMB_Demo.h"

//
// Macros and definitions
//
#define TARGET_ADDRESS 0x43             /*! Address of target device */
#define SMB_DEFAULT_PEC_ENABLED         /*! Define to enable PEC by default */

//
//  Global variables
//
static SMBus sSMBController;                    /*! SMBus Controller Struct  */
static SMBus_State sSMBState;                   /*! SMBus state          */
static uint8_t Command_Buff[SMB_MAX_PACKET_SIZE + 4] = {0};  /*! TX Buffer */
static uint8_t Resp_Buff[SMB_MAX_PACKET_SIZE + 4];           /*! RX Buffer */

/*! Buffer for host Alert Response */
static uint8_t HostAlert_Buff[SMB_HOST_ALERT_PACKET_SIZE];
/* Prototype of function to process Host Notify message */
static void processHostNotify(uint8_t deviceAddress, uint16_t data);
/*! Flag to trigger Test message to toggle Target's LED */
static volatile bool bTriggerTestMessage = false;

//*****************************************************************************
//
//! Main Function
//!
//! SMBus Controller demo testing Host Notify function.
//!   Pressing a LaunchPad button will send a Write Byte message to the
//!   Target device.
//!   Pressing a button on the Target's LaunchPad will trigger a Host Notify.
//!
//! \return None
//
// *****************************************************************************
int main(void)
{
    uint8_t ret, RespLen;
    bool RXExpected;

    SYSCFG_DL_init();

    NVIC_ClearPendingIRQ(SMB_I2C_INST_INT_IRQN);
    NVIC_EnableIRQ(SMB_I2C_INST_INT_IRQN);

    SMBus_controllerInit(&sSMBController, SMB_I2C_INST, (32000000));

 #ifdef SMB_DEFAULT_PEC_ENABLED
    SMBus_enablePEC(&sSMBController);
 #endif
    SMBus_controllerEnableHostNotify(&sSMBController, &HostAlert_Buff[0]);

    /* Enable GPIO Interrupt to trigger Test message */
    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);

    while(1)
    {
        if (bTriggerTestMessage == true)
        {
            /*
             * This example sends a dummy byte when the button is pressed
             * The Target will toggle its LED
             *
             * Send Write byte, Command and data are needed
             *  Target will write directly to the corresponding register
             *  0x12 = Test Command to toggle LED
             *
             */
            if (Command_Buff[0] == 0)
            {
                Command_Buff[0] = 1;         /* Turn ON Target's LED */
            }
            else
            {
                Command_Buff[0] = 0;         /* Turn OFF Target's LED */
            }
            ret = SMBus_controllerWriteByteWord(&sSMBController, // SMB struct
                                            TARGET_ADDRESS,      // Target Addr
                                            0x12,                // SMB Command
                                            &Command_Buff[0],    // DataPtr
                                            1);                  // 1 byte data
            RXExpected = false;      // No response expected

            // After sending the command, wait for completion and check response
            if(ret == SMBUS_RET_OK)
            {
                // If the command was sent to target, wait for completion
                if(SMBus_controllerWaitUntilDone (&sSMBController,
                                              DEMO_TIMEOUT) != SMBUS_RET_OK)
                {
                    ret = DEMO_TIMEOUT_ERROR;
                    // Timeout detected in App but not by SCL line, restart interface
                    SMBus_controllerReset(&sSMBController);
                }
                else
                {
                    ret = sSMBState;
                }

                // If we are waiting for a response, check if we got it
                if((ret == SMBus_State_OK) && (RXExpected == true))
                {
                    // Get the length of payload for response
                    RespLen = SMBus_getRxPayloadAvailable(&sSMBController);

                    if(RespLen >= 1)
                    {
                        ret = DEMO_NO_ERROR_DATA_AVAIL; // Data Received OK
                    }
                    else
                    {
                        ret = DEMO_RX_ERROR;             // RX Data size Error
                    }
                }
            }
            bTriggerTestMessage = false;
            sSMBController.status.u8byte = 0x00; // Clear status
        }
        __WFE();
    }
}

//*****************************************************************************
//
//! I2C SMBus ISR
//!
//! Handles Controller interrupts
//!
//! \return  None
//
// *****************************************************************************
void SMB_I2C_INST_IRQHandler(void) {
    sSMBState = SMBus_controllerProcessInt(&sSMBController);

    switch (sSMBState)
    {
        case SMBus_State_Controller_HostNotify:
            /* Host Notify was received */
            if (sSMBController.nwk.rxIndex == 3)
            {
                /*
                 * Received data expected in hostNotifyRxBuffPtr as:
                 * [Device Address, Data Byte Low, Data Byte High]
                 */
                processHostNotify(sSMBController.nwk.hostNotifyRxBuffPtr[0],
                              ( (sSMBController.nwk.hostNotifyRxBuffPtr[2]<<8) +
                                (sSMBController.nwk.hostNotifyRxBuffPtr[1]) ));
            }
            break;
        default:
            break;
    }


}

//*****************************************************************************
//
//! Demo function to process Host Notify
//!
//!  Dummy Host Notify function just to check received address and data and
//!  toggle LED
//!
//! \return  None
//
// *****************************************************************************
static void processHostNotify(uint8_t deviceAddress, uint16_t data)
{
    volatile uint8_t address;
    volatile uint16_t rxData;

    address = deviceAddress;
    rxData = data;
    DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
}


//*****************************************************************************
//
//! GROUP1 ISR
//!
//! Used to check GPIO Interrupt
//!     USER_SWITCH_1 will trigger sending a Test message to the target
//!
//! \return  None
//
// *****************************************************************************
void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_SWITCHES_INT_IIDX:
            switch (DL_GPIO_getPendingInterrupt(GPIO_SWITCHES_PORT)) {
                case  GPIO_SWITCHES_USER_SWITCH_1_IIDX:
                    /* Trigger a test message when button is pressed */
                    bTriggerTestMessage = true;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

