/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board_api.h"
#include "sd_card.h"
#include "ti_msp_dl_config.h"
#include "tusb.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum {
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED     = 1000,
    BLINK_SUSPENDED   = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;
SD_Card_t mspm0SdCard;

void led_blinking_task(void);
void initializeSdCardChipSelectPin(void);
void increaseSpiSpeed(void);
void decreaseSpiSpeed(void);

/*------------- MAIN -------------*/
int main(void)
{
    board_init();

    /*
     * CS during SD card transactions must be manually handled to
     * ensure line is either high or low the entire transaction
     */
    initializeSdCardChipSelectPin();

    /* Enable interrupt for chip detect pin */
    NVIC_EnableIRQ(SD_CARD_INT_IRQN);

    /*
     * SD card pin could already be present before running application preventing interrupt from triggering.
     * Polling done to ensure SD card is properly caught.
     */
    while (mspm0SdCard.cardIsInSlot == 0) {
        if (DL_GPIO_readPins(SD_CARD_PORT, SD_CARD_DETECT_PIN))
            mspm0SdCard.cardIsInSlot = 1;
        else
            __WFI();
    }

    /* Once SD card has been detected, initialize it */
    sdCardInit(&mspm0SdCard);

    increaseSpiSpeed();

    /* init device stack on configured roothub port */
    tusb_rhport_init_t dev_init = {
        .role = TUSB_ROLE_DEVICE, .speed = TUSB_SPEED_AUTO};

    tusb_init(BOARD_TUD_RHPORT, &dev_init);

    if (board_init_after_tusb) {
        board_init_after_tusb();
    }

    while (1) {
        tud_task();
        led_blinking_task();
    }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

/* Invoked when device is mounted */
void tud_mount_cb(void)
{
    blink_interval_ms = BLINK_MOUNTED;
}

/* Invoked when device is unmounted */
void tud_umount_cb(void)
{
    blink_interval_ms = BLINK_NOT_MOUNTED;
}

/*
 * Invoked when usb bus is suspended
 * remote_wakeup_en : if host allow us  to perform remote wakeup
 * Within 7ms, device must draw an average of current less than 2.5 mA from bus
 */
void tud_suspend_cb(bool remote_wakeup_en)
{
    (void) remote_wakeup_en;
    blink_interval_ms = BLINK_SUSPENDED;
}

/* Invoked when usb bus is resumed */
void tud_resume_cb(void)
{
    blink_interval_ms = tud_mounted() ? BLINK_MOUNTED : BLINK_NOT_MOUNTED;
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
    static uint32_t start_ms = 0;
    static bool led_state    = false;

    /* Blink every interval ms */
    if (board_millis() - start_ms < blink_interval_ms) return;
    start_ms += blink_interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state;
}

//--------------------------------------------------------------------+
// MSPM0 functions
//--------------------------------------------------------------------+

/* Initialize CS pin to be utilized manually during SPI transactions*/
void initializeSdCardChipSelectPin(void)
{
    /* CS for SD cards are different */
    DL_GPIO_initDigitalOutput(GPIO_SPI_0_IOMUX_CS1);
    DL_GPIO_clearPins(GPIO_SPI_0_CS1_PORT, GPIO_SPI_0_CS1_PIN);
    DL_GPIO_enableOutput(GPIO_SPI_0_CS1_PORT, GPIO_SPI_0_CS1_PIN);
    DL_GPIO_setPins(GPIO_SPI_0_CS1_PORT, GPIO_SPI_0_CS1_PIN);
}

/* Increase speed of SPI */
void increaseSpiSpeed(void)
{
    /*
     * Increase speed of SPI peripheral once card is initialized
     * 80 MHz / ((24 + 1) * 2) = 1.6 MHz
     */
    DL_SPI_disable(SPI_0_INST);
    DL_SPI_setBitRateSerialClockDivider(SPI_0_INST, 24);
    DL_SPI_enable(SPI_0_INST);
}

/* Decrease speed of SPI */
void decreaseSpiSpeed(void)
{
    /*
     * Lower speed of SPI peripheral before initialization
     * 80 MHz / ((99 + 1) * 2) = 400 kHz
     */
    DL_SPI_disable(SPI_0_INST);
    DL_SPI_setBitRateSerialClockDivider(SPI_0_INST, 99);
    DL_SPI_enable(SPI_0_INST);
}

/* SD card detect interrupt handler */
void GROUP1_IRQHandler(void)
{
    decreaseSpiSpeed();
    /* Check to see if it's chip detect pin throwing the interrupt */
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        case SD_CARD_INT_IIDX:
            if (DL_GPIO_readPins(SD_CARD_PORT, SD_CARD_DETECT_PIN)) {
                mspm0SdCard.cardIsInSlot = 1;
                if (sdCardInit(&mspm0SdCard)) {
                    increaseSpiSpeed();
                    tud_connect();
                }
            } else {
                memset(&mspm0SdCard, 0, sizeof(SD_Card_t));
                mspm0SdCard.cardIsInSlot = 0;
                tud_disconnect();
            }
            break;
        default:
            break;
    }
}
