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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board_api.h"
#include "tusb.h"

#include "usb_descriptors.h"

const int8_t mouse_movements[][3] = {
  // Draw M
  {0,   0,    0},
  {0,   120,  0},
  {0,   0,    MOUSE_BUTTON_LEFT},
  {20,  -120, MOUSE_BUTTON_LEFT},
  {22,  47,   MOUSE_BUTTON_LEFT},
  {22,  -47,  MOUSE_BUTTON_LEFT},
  {22,  120,  MOUSE_BUTTON_LEFT},
  {0,   0,    0},
  {14,  0,    0}, // Add some space before next letter
  // Draw S
  {0,   0,    MOUSE_BUTTON_LEFT},
  {86,  0,    MOUSE_BUTTON_LEFT},
  {0,   -63,  MOUSE_BUTTON_LEFT},
  {-86, 0,    MOUSE_BUTTON_LEFT},
  {0,   -63,  MOUSE_BUTTON_LEFT},
  {86,  0,    MOUSE_BUTTON_LEFT},
  {0,   0,    0},
  {14,  0,    0}, // Add some space before next letter
  // Draw P
  {0,   0,    MOUSE_BUTTON_LEFT},
  {86,  0,    MOUSE_BUTTON_LEFT},
  {0,   54,   MOUSE_BUTTON_LEFT},
  {-86, 0,    MOUSE_BUTTON_LEFT},
  {0,   -54,  MOUSE_BUTTON_LEFT},
  {0,   120,  MOUSE_BUTTON_LEFT},
  {0,   0,    0},
};

/* Max number of rows in the array. Calculated at runtime */
uint32_t max_rows = 0;

/* Keep track of the current character to output */
uint32_t current_row = 0;

/* Program state machine */
enum {
  STATE_WAIT_BTN_PRESS = 0,
  STATE_WAIT_BTN_RELEASE,
  STATE_WAIT_BTN_DEBOUNCE,
  STATE_SEND_MOUSE_MOVEMENT,
};

uint8_t program_state = STATE_WAIT_BTN_PRESS;

/* Incorporate software debounce for button */
uint32_t btn_debounce_ms = 20;
uint32_t start_debounce_ms = 0;

/*
 * Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);
void hid_task(void);

/*------------- MAIN -------------*/
int main(void)
{
  board_init();

  max_rows = sizeof(mouse_movements)/sizeof(mouse_movements[0]);

  /* Initialize device stack on configured roothub port */
  tusb_rhport_init_t dev_init = {
    .role = TUSB_ROLE_DEVICE,
    .speed = TUSB_SPEED_AUTO
  };

  tusb_init(BOARD_TUD_RHPORT, &dev_init);

  if (board_init_after_tusb) {
    board_init_after_tusb();
  }

  while (1) {
    /* TinyUSB device task */
    tud_task();
    /* LED blinking task */
    led_blinking_task();
    /* HID task */
    hid_task();
  }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

/* Invoked when the device is mounted */
void tud_mount_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

/* Invoked when the device is unmounted */
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

/* Invoked when the USB bus is resumed */
void tud_resume_cb(void)
{
  blink_interval_ms = tud_mounted() ? BLINK_MOUNTED : BLINK_NOT_MOUNTED;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+
static void send_hid_report(uint8_t report_id, uint32_t btn)
{
  /* Return if HID is not ready yet */
  if ( !tud_hid_ready() ) return;

  /* Wait until button has been pressed to start state machine */
  if ( program_state == STATE_WAIT_BTN_PRESS && btn == true)
    program_state = STATE_WAIT_BTN_RELEASE;

  /* Wait until button has been released */
  if (program_state == STATE_WAIT_BTN_RELEASE && btn == false) {
    program_state = STATE_WAIT_BTN_DEBOUNCE;
    start_debounce_ms = board_millis();
  }

  /* Wait certain amount of time to ignore button debounce */
  if (program_state == STATE_WAIT_BTN_DEBOUNCE) {
    if ((board_millis() - start_debounce_ms) > btn_debounce_ms) {
      program_state = STATE_SEND_MOUSE_MOVEMENT;
    }
  }

  /* Print out character one at a time via the keyboard report */
  if (program_state == STATE_SEND_MOUSE_MOVEMENT) {

    int8_t x = mouse_movements[current_row][0];
    int8_t y = mouse_movements[current_row][1];
    uint8_t mouse_btns = mouse_movements[current_row][2];

    tud_hid_mouse_report(report_id, mouse_btns, x,y,0,0);

    current_row++;
    if(current_row == max_rows) {
      program_state = 0;
      current_row = 0;
    }
  }
}

/*
 * Every 50 ms, a single report for mouse HID profile is sent.
 */
void hid_task(void)
{
  /* Poll every 50 ms */
  const uint32_t interval_ms = 50;
  static uint32_t start_ms = 0;
  uint32_t  btn = 0;

  /* If there is not sufficient time then return */
  if ( board_millis() - start_ms < interval_ms) return;
  start_ms += interval_ms;

  btn = board_button_read();

  /* Remote wakeup */
  if ( tud_suspended() && btn ) {
    /*
     * Wake up the host if we are in suspend mode
     * and REMOTE_WAKEUP feature is enabled by host
     */
    tud_remote_wakeup();
  } else {
    /* Send the report for the device being utilized */
    send_hid_report(REPORT_ID_MOUSE, btn);
  }
}

/*
 * Invoked when received GET_REPORT control request
 * Application must fill buffer report's content and return its length.
 * Return zero will cause the stack to STALL request
 */
 uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
 {
   /* Not implemented */
   (void) instance;
   (void) report_id;
   (void) report_type;
   (void) buffer;
   (void) reqlen;

   return 0;
 }

/*
 * Invoked when received SET_REPORT control request or
 * received data on OUT endpoint ( Report ID = 0, Type = 0 )
 */
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) bufsize;

  return;
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  /* If blink interval is 0, do nothing and return */
  if (!blink_interval_ms) return;

  /*
   * If there is not sufficient time return, otherwise
   * blink every interval_ms
   */
  if ( board_millis() - start_ms < blink_interval_ms) return; // not enough time
  start_ms += blink_interval_ms;

  /* Set LED based off led_state */
  board_led_write(led_state);
  /* Update led_state for toggle */
  led_state = 1 - led_state;
}
