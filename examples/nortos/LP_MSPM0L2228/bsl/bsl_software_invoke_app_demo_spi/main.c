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

#include "ti_msp_dl_config.h"

volatile bool BSL_trigger_flag;

__STATIC_INLINE void invokeBSLAsm(void);
volatile uint8_t gData = 0;

int main(void)
{
    SYSCFG_DL_init();

    NVIC_EnableIRQ(GPIO_SWITCHES_INT_IRQN);
#ifdef UART_INTERFACE
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
#endif
#ifdef I2C_INTERFACE
    NVIC_EnableIRQ(I2C_INST_INT_IRQN);
#endif
#ifdef SPI_INTERFACE
    NVIC_ClearPendingIRQ(SPI_0_INST_INT_IRQN);
    NVIC_EnableIRQ(SPI_0_INST_INT_IRQN);
#endif

    BSL_trigger_flag = false;

    while (BSL_trigger_flag == false) {
        /* Toggle LED while waiting for button press */
        delay_cycles(8000000);
        DL_GPIO_togglePins(GPIO_LEDS_USER_LED_1_PORT, GPIO_LEDS_USER_LED_1_PIN);
    }

    /*
     * When GPIO_SWITCHES1_USER_SWITCH_1_PIN is pressed, set
     * GPIO_LEDS_USER_LED_1_PIN for one second before resetting the device
     * with BOOTLOADERENTRY reset level. This will run the bootcode, which
     * will check if there is a request from the application, and then invoke
     * the bootloader.
     */
    DL_GPIO_clearPins(GPIO_LEDS_USER_LED_1_PORT, GPIO_LEDS_USER_LED_1_PIN);
    DL_GPIO_setPins(GPIO_LEDS_USER_LED_2_PORT, GPIO_LEDS_USER_LED_2_PIN);
    delay_cycles(8000000);

    /* Force Reset calling BSL entry*/
    invokeBSLAsm();

    while (1) {
        __WFI();
    }
}

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case GPIO_SWITCHES_INT_IIDX:
            switch (DL_GPIO_getPendingInterrupt(GPIO_SWITCHES_PORT)) {
                case GPIO_SWITCHES_USER_SWITCH_1_IIDX:
                    /* Indicate that the button has been pressed */
                    BSL_trigger_flag = true;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

__STATIC_INLINE void invokeBSLAsm(void)
{
    /* Erase SRAM completely before jumping to BSL */
    __asm(
#if defined(__GNUC__)
        ".syntax unified\n" /* Load SRAMFLASH register*/
#endif
        "ldr     r4, = 0x41C40018\n" /* Load SRAMFLASH register*/
        "ldr     r4, [r4]\n"
        "ldr     r1, = 0x03FF0000\n" /* SRAMFLASH.SRAM_SZ mask */
        "ands    r4, r1\n"           /* Get SRAMFLASH.SRAM_SZ */
        "lsrs    r4, r4, #6\n"       /* SRAMFLASH.SRAM_SZ to kB */
        "ldr     r1, = 0x20300000\n" /* Start of ECC-code */
        "adds    r2, r4, r1\n"       /* End of ECC-code */
        "movs    r3, #0\n"
        "init_ecc_loop: \n" /* Loop to clear ECC-code */
        "str     r3, [r1]\n"
        "adds    r1, r1, #4\n"
        "cmp     r1, r2\n"
        "blo     init_ecc_loop\n"
        "ldr     r1, = 0x20200000\n" /* Start of NON-ECC-data */
        "adds    r2, r4, r1\n"       /* End of NON-ECC-data */
        "movs    r3, #0\n"
        "init_data_loop:\n" /* Loop to clear ECC-data */
        "str     r3, [r1]\n"
        "adds    r1, r1, #4\n"
        "cmp     r1, r2\n"
        "blo     init_data_loop\n"
        /* Force a reset calling BSL after clearing SRAM */
        "str     %[resetLvlVal], [%[resetLvlAddr], #0x00]\n"
        "str     %[resetCmdVal], [%[resetCmdAddr], #0x00]"
        : /* No outputs */
        : [ resetLvlAddr ] "r"(&SYSCTL->SOCLOCK.RESETLEVEL),
        [ resetLvlVal ] "r"(DL_SYSCTL_RESET_BOOTLOADER_ENTRY),
        [ resetCmdAddr ] "r"(&SYSCTL->SOCLOCK.RESETCMD),
        [ resetCmdVal ] "r"(
            SYSCTL_RESETCMD_KEY_VALUE | SYSCTL_RESETCMD_GO_TRUE)
        : "r1", "r2", "r3", "r4");
}

#ifdef UART_INTERFACE

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            gData = DL_UART_Main_receiveData(UART_0_INST);
            if (gData == 0x22) {
                /* Indicate that the BSL software trigger needed*/
                BSL_trigger_flag = true;
            }

            break;
        default:
            break;
    }
}

#endif

#ifdef I2C_INTERFACE
void I2C_INST_IRQHandler(void)
{
    switch (DL_I2C_getPendingInterrupt(I2C_INST)) {
        case DL_I2C_IIDX_TARGET_START:
            break;
        case DL_I2C_IIDX_TARGET_RXFIFO_TRIGGER:
            /* Store received data in buffer */
            while (DL_I2C_isTargetRXFIFOEmpty(I2C_INST) != true) {
                gData = DL_I2C_receiveTargetData(I2C_INST);
            }
            if (gData == 0x22) {
                /* Indicate that the BSL software trigger needed*/
                BSL_trigger_flag = true;
            }
            break;
        case DL_I2C_IIDX_TARGET_TXFIFO_TRIGGER:
            break;
        case DL_I2C_IIDX_TARGET_STOP:
            break;
        case DL_I2C_IIDX_TARGET_RX_DONE:
            /* Not used for this example */
        case DL_I2C_IIDX_TARGET_RXFIFO_FULL:
            /* Not used for this example */
        case DL_I2C_IIDX_TARGET_GENERAL_CALL:
            /* Not used for this example */
        case DL_I2C_IIDX_TARGET_EVENT1_DMA_DONE:
            /* Not used for this example */
        case DL_I2C_IIDX_TARGET_EVENT2_DMA_DONE:
            /* Not used for this example */
        default:
            break;
    }
}
#endif
#ifdef SPI_INTERFACE
void SPI_0_INST_IRQHandler(void)
{
    switch (DL_SPI_getPendingInterrupt(SPI_0_INST)) {
        case DL_SPI_IIDX_RX:
            /* Read RX FIFO and echo it back to the SPI Controller */
            gData = DL_SPI_receiveData8(SPI_0_INST);
            if (gData == 0x22) {
                /* Indicate that the BSL software trigger needed*/
                BSL_trigger_flag = true;
            }
            break;
        default:
            break;
    }
}

#endif
