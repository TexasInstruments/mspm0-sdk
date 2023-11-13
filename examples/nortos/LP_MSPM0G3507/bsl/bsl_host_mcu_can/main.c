/*
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
 */

#include <application_image.h>
#include <ti_msp_dl_config.h>
#include "bsl.h"
#include "can.h"

void ToggleLeds(void);

uint8_t bsl_err;

/*=============================================================================
* Here is password of the boot code for update. 
*/
const uint8_t BSL_PW_RESET[32] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF};
extern CANFD_baudrate_config br_cfg;
int main(void)
{
    uint8_t section;

    SYSCFG_DL_init();
    CAN_initialize();
    /*Indication of Initialization*/
    ToggleLeds();

     while (1)
    {
        if (!DL_GPIO_readPins(GPIO_Button_PORT, GPIO_Button_PIN_0_PIN))
        {
            delay_cycles(DELAY_BUTTON_PRESS);
            if (!DL_GPIO_readPins(GPIO_Button_PORT, GPIO_Button_PIN_0_PIN))
            {
                bsl_err = BSL_OPERATION_SUCCESFUL;
                ToggleLeds();
#ifdef Hardware_Invoke
                /* PLACE TARGET INTO BSL MODE by hardware invoke*/
                Host_BSL_entry_sequence();
                delay_cycles(DELAY_BSL_OP_LONG);
#endif
                bsl_err = Host_BSL_Connection();
                delay_cycles(DELAY_BSL_OP_LONG);

                if(bsl_err == BSL_OPERATION_SUCCESFUL)
                {
                    bsl_err = Host_BSL_Change_Bitrate(&br_cfg);

                    if(bsl_err == BSL_OPERATION_SUCCESFUL)
                    {
                        bsl_err = Host_BSL_GetID();
                        if (BSL_maxBufferSize >= MAX_PACKET_SIZE)
                        {
                            bsl_err =
                            Host_BSL_loadPassword((uint8_t*) BSL_PW_RESET);
                            if (bsl_err == BSL_OPERATION_SUCCESFUL)
                            {
                                bsl_err = Host_BSL_MassErase();
                                if (bsl_err == BSL_OPERATION_SUCCESFUL)
                                {
                                    delay_cycles(DELAY_BSL_OP);
                                    /*WRITE THE ENTIRE PROGRAM MEMORY SECTION TO TARGET*/
                                    for (section = 0;
                                         section < (sizeof(App1_Addr) /
                                                       sizeof(App1_Addr[0]));
                                         section++)
                                    {
                                        bsl_err = Host_BSL_writeMemory(
                                            App1_Addr[section], App1_Ptr[section],
                                            App1_Size[section]);
                                        if (bsl_err != BSL_OPERATION_SUCCESFUL) break;
                                    }
                                    /* Start the application */
                                    Host_BSL_StartApp();
                                }
                                else
                                {
                                    /* Mass Erase failed error */
                                    TurnOnErrorLED();
                                    __BKPT(0);
                                }
                            }
                            else
                            {
                                /* Password failed error */
                                TurnOnErrorLED();
                                __BKPT(0);
                            }
                        }
                        else
                        {
                            /* Buffer less than MAX_PACKET_SIZE error */
                            TurnOnErrorLED();
                            __BKPT(0);
                        }
                    }
                    else
                    {
                        /* Bitrate Change failed error */
                        TurnOnErrorLED();
                        __BKPT(0);
                    }
                }
            }
        }
        delay_cycles(DELAY_BSL_OP);
    }
}

/*
 * Toggle the LEDs to show we are going to BSL the target
 */
void ToggleLeds(void)
{
    DL_GPIO_togglePins(GPIO_LED_PA0_PORT, GPIO_LED_PA0_PIN);
    DL_GPIO_togglePins(GPIO_LED_Error_PORT, GPIO_LED_Error_PIN);
}
