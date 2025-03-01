//##################################################
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the
//   distribution.
//
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//#############################################################################
/*
 * Name:    TIDA-010247-Example-Code
 * Version: 2.0
 * Date:    2025-02-14
 *
 * Description/Notes:
 *
 *  How to use this software:
 *  1. Only apply 10-12V to the BOT AFE BAT pin and disable top AFE,
 *      then OTP program the BOT AFE to different I2C address.
 *  2. Set AFE_MOS_TEST and comment out CAN_ID_Init_on_Startup()
 *      (set intp as both edge triggered) to use WAKEIN pin
 *      (set intp as rising edge triggered) to test the protection FET on/off.
 *  3. Check the check box here before download to MSPM0:
 *      Right click project - Properties - Debug - MSPM0 Flash Settings
 *      - Enable wait for debug (Warning: Do not enable if flash is empty)
 */
#include "App_task.h"
#include "BQ769x2_Configs/BQ769x2_protocol.h"
#include "Communications/CAN_communication.h"
#include "Communications/I2C_communication.h"
#include "Communications/UART_communication.h"

int main(void)
{
    /********************* MCU initiations ***************************/
    SYSCFG_DL_init();
    DL_SYSCTL_disableSleepOnExit();

    Gpio_Init();
    Variables_Init();

    NVIC_EnableIRQ(I2C_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(WAKE_INT_IRQN);  //Enable WAKE_INT_IRQn
    //    DL_TimerG_startCounter(PWM_0_INST);  //used for fan control

#if AFEOTP
    BQ769x2_OTP_Programming();  // Run this function once on production line
#else

    BQ769x2_BOT_Init();  // Bottom BQ769x2 initiation
    BQ769x2_TOP_Init();  // Top BQ769x2 initiation

    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_TimerA_startCounter(TIMER_0_INST);

    while (1) {
        if (gWAKEINMCU)  // wait for CAN wake signal
        {
            CAN_ID_Init_on_Startup();  // CAN Addressing
        }

#if !AFE_MOS_TEST
        switch (Working_mode) {
            case Ship_mode:
                BMU_Ship_Mode();
                break;
            case Shutdown_mode:
                BMU_Shutdown_Mode();
                break;
            case Standby_mode:
                BMU_Standby_Mode();
                break;
            case Normal_mode:
                BMU_Normal_Mode();
                break;
            default:
                break;
        }
#endif
    }
#endif
}  //end of main.c

// Timer Intp, 100ms period
void TIMER_0_INST_IRQHandler(void)
{
    switch (DL_TimerA_getPendingInterrupt(TIMER_0_INST)) {
        case DL_TIMERA_IIDX_ZERO:

            gTIMER0 = true;
            Systick_counter += 1;
            if (Systick_counter > 600) Systick_counter = 1;  //1min

            break;
        default:
            break;
    }
}

//GPIO Intp, this IRQ is triggered via GPIO pins,
//rising/falling/both edges.
void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case DL_INTERRUPT_GROUP1_IIDX_GPIOA:  //GPIOA
            switch (DL_GPIO_getPendingInterrupt(GPIOA)) {
                case DL_GPIO_IIDX_DIO0:  //PA0
                    break;               //Not in use
                case DL_GPIO_IIDX_DIO1:  //PA1
                    break;               //Not in use
                default:
                    break;
            }  //GPIOA
            break;

        case DL_INTERRUPT_GROUP1_IIDX_GPIOB:  //GPIOB
            switch (DL_GPIO_getPendingInterrupt(GPIOB)) {
                case DL_GPIO_IIDX_DIO0:  //PB0
                    break;               //Not in use
                case DL_GPIO_IIDX_DIO1:  //PB1
                    break;               //Not in use
                case WAKE_IN_MCU_IIDX:

                    check_signal_pattern();
                    /************************MOSFET on/off Test*********************************/
                    /*User can user external signal to trigger this interrupt to test host-controlled MOSFET ON/OFF operations
*In this example, PB.23 is used
*/
#if AFE_MOS_TEST
                    BMU_FET_Test();
#endif

                    break;
                default:
                    break;
            }  //GPIOB
            break;
        default:
            break;
    }  //DL_INTERRUPT_GROUP_1
}  //GROUP1_IRQHandler
