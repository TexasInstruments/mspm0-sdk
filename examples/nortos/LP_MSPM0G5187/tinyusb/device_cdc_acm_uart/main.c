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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board_api.h"
#include "ti_msp_dl_config.h"
#include "tusb.h"

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

static void led_blinking_task(void);
static void USB_to_UART_task(void);

//Size of the data received via USB
#define BUFF_SIZE (64)
uint8_t gDataBuff[BUFF_SIZE];

/*------------- MAIN -------------*/
int main(void)
{
    SYSCFG_DL_init();

    /*Board init will initialize a UART port. By default it uses UART0's PA10 and PA11 at 115200 baud.
     * Change the ti_msp_dl_config files if wanting to use a different UART port for the bridge application.
     */
    board_init();

    // init device stack on configured roothub port
    tusb_rhport_init_t dev_init = {
        .role = TUSB_ROLE_DEVICE, .speed = TUSB_SPEED_AUTO};

    tusb_init(BOARD_TUD_RHPORT, &dev_init);

    while (1) {
        tud_task();  // tinyusb device task
        USB_to_UART_task();
        led_blinking_task();
    }
}

// Invoked when device is mounted
void tud_mount_cb(void)
{
    blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
    blink_interval_ms = BLINK_NOT_MOUNTED;
}

//--------------------------------------------------------------------+
// USB to UART
//--------------------------------------------------------------------+

static void USB_to_UART_task(void)
{
    uint8_t itf;
    for (itf = 0; itf < CFG_TUD_CDC; itf++) {
        // connected() check for DTR bit
        // Most but not all terminal client set this when making connection
        // if ( tud_cdc_n_connected(itf) )
        if (tud_cdc_n_available(itf)) {
            volatile uint32_t count =
                tud_cdc_n_read(itf, gDataBuff, sizeof(gDataBuff));
            //Echos back to the USB port, so you can observe what you typed in,
            //then transmits over the configured UART
            for (uint32_t i = 0; i < count; i++) {
                tud_cdc_n_write_char(itf, gDataBuff[i]);
                DL_UART_Main_transmitData(UART_0_INST, gDataBuff[i]);
            }
        }
        tud_cdc_n_write_flush(itf);
    }
}

// Invoked when cdc when line state changed e.g connected/disconnected
// Use to reset to DFU when disconnect with 1200 bps
void tud_cdc_line_state_cb(uint8_t instance, bool dtr, bool rts)
{
    (void) rts;

    // DTR = false is counted as disconnected
    if (!dtr) {
        // touch1200 only with first CDC instance (Serial)
        if (instance == 0) {
            cdc_line_coding_t coding;
            tud_cdc_get_line_coding(&coding);
            if (coding.bit_rate == 1200) {
                board_reset_to_bootloader();
            }
        }
    }
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
    static uint32_t start_ms = 0;
    static bool led_state    = false;

    // Blink every interval ms
    if (board_millis() - start_ms < blink_interval_ms)
        return;  // not enough time
    start_ms += blink_interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state;  // toggle
}
