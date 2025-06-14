/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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
#include "i2c_target.h"

/*====================== FORWARD DECLARATIONS ==============================*/

/*============================ TYPEDEFS ====================================*/
typedef uint32_t tMask;

/*============================ EXTERNS =====================================*/
extern uint8_t TxBuffer[];
extern uint8_t RxBuffer[];

/*====================== DEFINES/MACROS ====================================*/
#define IOX_NUM_OF_PORTS                        (2)

/*
 * PORT MASK configuration
 * Set the NUM_OF_PORTS below according to the target device.
 *
 * To define which pins will be active on the IO expander, set the
 * corresponding PORTx as follows:
 *
 * Setting bit =  '1' indicates the pin is active and can be modified by the host MCU.
 * Setting bit =  '0' indicates the pin is not used.
 *
 *  PROTOCOL
 *  [BYTE 0]: 0 = PORTA, 1 = PORTB, 2 = PORTC
 *  [BYTE 1]: 0 = DIN, 1 = DOUT, 2 = CLEAR, 3 = SET, 4 = TOGGLE
 *  [BYTE 2][BYTE 3][BYTE 4][BYTE 5] = 32-BIT data
 */

/*
 * The GPIO defines represent the MSPM0L/G LaunchPad pins used in this example.
 * See the peripheralPinAssignments.txt file generated by Sysconfig.
 *
 * Be sure that the mask matches the physical pins used on the io expander.
 *

/*============================ VARIABLES ===================================*/
tPacket Packet;

//=================================================
// User modifies this section, PORT A:
// Configure InputMask and Output masks for PORTA.
//=================================================
const tMask PORTA_inputMask  = (GPIO_A_USER_IN_PIN |
                                GPIO_A_SW_S1_PIN |
                                GPIO_A_SW_S2_PIN);

const tMask PORTA_outputMask = (GPIO_A_USER_OUT_PIN |
                                GPIO_A_IRQ_OUT_PIN);

#if (IOX_NUM_OF_PORTS == 1)
GPIO_Regs* portList[] = {GPIOA};
const tMask inputMasks[] = {PORTA_inputMask};
const tMask outputMasks[] = {PORTA_outputMask};
#endif

//=================================================
// If device has PORT B:
// User modifies this section.
// Configure InputMask and Output masks for PORTB.
//=================================================
#if (IOX_NUM_OF_PORTS > 1)
const tMask PORTB_inputMask = (0x00000000);
const tMask PORTB_outputMask = (GPIO_B_LED_RGB_GREEN_PIN |
                                GPIO_B_LED_RGB_RED_PIN |
                                GPIO_B_LED_RGB_BLUE_PIN);
#endif

#if (IOX_NUM_OF_PORTS == 2)
GPIO_Regs* portList[] = {GPIOA, GPIOB};
const tMask inputMasks[] = {PORTA_inputMask, PORTB_inputMask};
const tMask outputMasks[] = {PORTA_outputMask, PORTB_outputMask};
#endif

//=================================================
// If device has PORT C:
// User modifies this section.
// Configure InputMask and Output masks for PORTC.
//=================================================
#if (IOX_NUM_OF_PORTS > 2)
const tMask PORTC_inputMask = (0x00000000);
const tMask PORTC_outputMask = (0x00000000);
#endif

#if (IOX_NUM_OF_PORTS == 3)
GPIO_Regs* portList[] = {GPIOA, GPIOB, GPIOC};
const tMask inputMasks[] = {PORTA_inputMask, PORTB_inputMask, PORTC_inputMask};
const tMask outputMasks[] = {PORTA_outputMask, PORTB_outputMask, PORTC_outputMask};
#endif

/*============================ FUNCTIONS ===================================*/

/*
 * Simple message processor
 */
void processMessage(void)
{
    GPIO_Regs* GPIO_Port;
    uint32_t ui32Data;

    /* Extract packet info and populate port object */
    deSerializePacket(&Packet, RxBuffer);

    /* Look up actual PORT address for this device */
    GPIO_Port = portList[Packet.PortNum];

    /* Use output mask to allow only specified bits to be modified */
    ui32Data = (Packet.Data.ui32Full & (tMask)outputMasks[Packet.PortNum]);

    /*======== Perform action on selected port register ==========*/
    switch(Packet.Register)
    {
    case eDIN:

        /* Read the requested GPIO.
         * Use input mask to allow only specified bits to be modified
         */
        Packet.Data.ui32Full =  DL_GPIO_readPins(GPIO_Port, (tMask)inputMasks[Packet.PortNum]);

        /* Serialize the object...populate Tx_Buffer */
        serializePacket(&Packet, TxBuffer);

        /*
         * Enabling TXFIFO interrupt will automatically load the TXFIFO register with the packet
         */
        DL_I2C_enableInterrupt(I2C_TARGET_INST, DL_I2C_INTERRUPT_TARGET_TXFIFO_TRIGGER);
        break;
        
    case eDOUT: /* Update all pins at same time*/
        DL_GPIO_writePins(GPIO_Port, ui32Data);
        break;
        
    case eDOUT_CLEAR: /* Clear only selected pins */
        DL_GPIO_clearPins(GPIO_Port, ui32Data);
        break;
        
    case eDOUT_SET: /* Set only selected pins */
        DL_GPIO_setPins(GPIO_Port, ui32Data);
        break;
        
    case eDOUT_TOGGLE: /* Toggle only selected pins */
        DL_GPIO_togglePins(GPIO_Port, ui32Data);
        break;
        
     }// end switch
}
