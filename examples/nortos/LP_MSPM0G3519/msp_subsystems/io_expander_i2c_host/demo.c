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
/*
 *  Demo.c
 *
 *  Use the #define in demo.h file to select which target MSPM0 will be used
 *  as the IO expander.
 *
 *  [BYTE 0]: 0 = PORTA, 1 = PORTB, 2 = PORTC
 *  [BYTE 1]: 0 = DIN, 1 = DOUT, 2 = CLEAR, 3 = SET, 4 = TOGGLE
 *  [BYTE 2][BYTE 3][BYTE 4][BYTE 5] = 32-BIT data
 *
 */

#include "ti_msp_dl_config.h"
#include "demo.h"

/*============================ EXTERNS =====================================*/

/*============================ MACROS =====================================*/

/*============================ VARIABLES ===================================*/
tPacket IOx_PortA = {.PortNum = ePortA};

#if(IOX_NUM_OF_PORTS > 1)

tPacket IOx_PortB = {.PortNum = ePortB};
#endif

#if(IOX_NUM_OF_PORTS > 2)

tPacket IOx_PortC = {.PortNum = ePortC};
#endif

struct StateMachine
{
    uint8_t state;

}Demo = {0}; /* assign starting state */

/*====================== FORWARD DECLARATIONS ==============================*/

/*============================ FUNCTIONS ===================================*/
#if TARGET==MSPM0L1306_TARGET
/* MSPM0L1306 Demonstration code */
void demoIOx(void)
{
    switch(Demo.state)
    {
    case 0:
        /* CLEAR ALL LEDS */
        IOx_clearPins(&IOx_PortA, (LED_RGB_GREEN | LED_RGB_RED | LED_RGB_BLUE ));

        /* check if any of the function calls returned an error */
        if(gI2cControllerStatus == I2C_STATUS_ERROR)
            break;

        msecDelayBlocking(1);

        /* SET GREEN LED = ON */
        while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
        IOx_setPins(&IOx_PortA, LED_RGB_GREEN);

        Demo.state++;
        break;

    case 1:
        /* CLEAR GREEN LED = OFF */
        while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
        IOx_clearPins(&IOx_PortA, LED_RGB_GREEN);
        msecDelayBlocking(1);

        Demo.state++;
        break;

    case 2:
        /* TOGGLE RED LED ON */
        while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
        IOx_togglePins(&IOx_PortA, LED_RGB_RED);
        msecDelayBlocking(1);

        Demo.state++;
        break;

    case 3:
        /* TOGGLE RED LED OFF */
        while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
        IOx_togglePins(&IOx_PortA, LED_RGB_RED);
        msecDelayBlocking(1);

        Demo.state++;
        break;

    case 4:
        /* READ PORTA */
        while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
        IOx_readPins(&IOx_PortA);

        msecDelayBlocking(1);

        if((IOx_PortA.Data.ui32Full & SW_S1) == SW_S1)
        {
            while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
            IOx_setPins(&IOx_PortA, LED_RGB_BLUE);
        }

        Demo.state = 0;
        break;

    default:
        break;
    }

    /* check if any of the function calls returned an error */
    if(gI2cControllerStatus == I2C_STATUS_ERROR)
        I2C_errorTrap();
}

#elif TARGET==MSPM0G3507_TARGET
/* MSPM0L3507 Launch Pad Demonstration code */
void demoIOx(void)
{
    switch(Demo.state)
    {
    case 0:
        /* CLEAR ALL LEDS */
        IOx_clearPins(&IOx_PortB, (LED_RGB_GREEN | LED_RGB_RED | LED_RGB_BLUE ));

        /* check if any of the function calls returned an error */
        if(gI2cControllerStatus == I2C_STATUS_ERROR)
            break;

        msecDelayBlocking(1);

        /* SET GREEN LED = ON */
        while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
        IOx_setPins(&IOx_PortB, LED_RGB_GREEN);

        Demo.state++;
        break;

    case 1:
        /* CLEAR GREEN LED = OFF */
        while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
        IOx_clearPins(&IOx_PortB, LED_RGB_GREEN);
        msecDelayBlocking(1);

        Demo.state++;
        break;

    case 2:
        /* TOGGLE RED LED ON */
        while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
        IOx_setPins(&IOx_PortB, LED_RGB_RED);
        msecDelayBlocking(1);

        Demo.state++;
        break;

    case 3:
        /* TOGGLE RED LED OFF */
        while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
        IOx_togglePins(&IOx_PortB, LED_RGB_RED);
        msecDelayBlocking(1);

        Demo.state++;
        break;

    case 4:
        /* READ PORTA */
        while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
        IOx_readPins(&IOx_PortA);
        msecDelayBlocking(1);

        if((IOx_PortA.Data.ui32Full & SW_S2) == SW_S2)
        {
            while (!(DL_I2C_getControllerStatus(I2C_CONTROLLER_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
            IOx_setPins(&IOx_PortB, LED_RGB_BLUE);
        }

        Demo.state = 0;
        break;

    default:
        break;
    }

    /* check if any of the function calls returned an error */
    if(gI2cControllerStatus == I2C_STATUS_ERROR)
        I2C_errorTrap();
}

#endif

