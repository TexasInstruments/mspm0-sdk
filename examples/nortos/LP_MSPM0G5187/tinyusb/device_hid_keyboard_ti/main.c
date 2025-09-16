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

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Make sure this match the max number of characters in a row */
#define MAX_LENGTH 62

/* Must ensure each row has the same number of characters */
const uint8_t text[][MAX_LENGTH] = {
" /$$      /$$  /$$$$$$  /$$$$$$$  /$$      /$$  /$$$$$$     \n",
"| $$$    /$$$ /$$__  $$| $$__  $$| $$$    /$$$ /$$$_  $$    \n",
"| $$$$  /$$$$| $$  \\__/| $$  \\ $$| $$$$  /$$$$| $$$$\\ $$ \n",
"| $$ $$/$$ $$|  $$$$$$ | $$$$$$$/| $$ $$/$$ $$| $$ $$ $$    \n",
"| $$  $$$| $$ \\____  $$| $$____/ | $$  $$$| $$| $$\\ $$$$  \n",
"| $$\\  $ | $$ /$$  \\ $$| $$      | $$\\  $ | $$| $$ \\ $$$\n",
"| $$ \\/  | $$|  $$$$$$/| $$      | $$ \\/  | $$|  $$$$$$/  \n",
"|__/     |__/ \\______/ |__/      |__/     |__/ \\______/   \n",
};

/* Max number of rows in the array. Calculated at runtime */
uint32_t max_rows = 0;

/* Keep track of the current character to output */
uint32_t current_col = 0;
uint32_t current_row = 0;

/* Translate ascii characters to TinyUSB HID key board keycode and modifier */
uint8_t const ascii_to_keycode_table[128][2] =  { HID_ASCII_TO_KEYCODE };

/* Program state machine */
/* Program state machine */
enum {
  STATE_WAIT_BTN_PRESS = 0,
  STATE_WAIT_BTN_RELEASE,
  STATE_WAIT_BTN_DEBOUNCE,
  STATE_SEND_KEY_PRESS,
  STATE_SEND_KEY_RELEASE,
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

  max_rows = sizeof(text)/sizeof(text[0]);


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
  /* Utilizing this variable to avoid sending multiple zero reports */
  static bool has_keyboard_key = false;
  /* Contain keycodes to send in a single report */
  uint8_t keycode[6] = {0};
  /* Modifier key to send (ie shift, alt, etc..) */
  uint8_t modifier = 0;
  /* The current character we want to send */
  uint8_t current_character = 0;
  /* Initialize the keyboard modifier and set it to no modifier */

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
      program_state = STATE_SEND_KEY_PRESS;
    }
  }

  /* Print out character one at a time via the keyboard report */
  if (program_state == STATE_SEND_KEY_PRESS) {
    current_character = text[current_row][current_col];

    if(ascii_to_keycode_table[current_character][0]) {
      modifier = KEYBOARD_MODIFIER_LEFTSHIFT;
    }
    keycode[0] = ascii_to_keycode_table[current_character][1];

    has_keyboard_key = true;
    tud_hid_keyboard_report(report_id, modifier, keycode);

    /* Make sure the NULL report is sent to mimic releasing a key before
     * sending the next one.
     */
    program_state = STATE_SEND_KEY_RELEASE;

    current_col++;
    if(current_col == MAX_LENGTH) {
      current_col = 0;
      current_row++;
    }
    if(current_row == max_rows) {
      program_state = 0;
      current_row = 0;
    }
  } else {
    /* Do nothing */
    if (has_keyboard_key)
      tud_hid_keyboard_report(report_id, modifier, NULL);
    has_keyboard_key = false;

    /* Send the next character */
    if(program_state == STATE_SEND_KEY_RELEASE) {
      program_state = STATE_SEND_KEY_PRESS;
    }
  }
}

/*
 * Every 10 ms, a single report for keyboard HID profile is sent.
 * tud_hid_report_complete_cb() is used to send the next report after the previous
 * one is completed.
 */
void hid_task(void)
{
  /* Poll every 10ms */
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  /* If there is not sufficient time then return */
  if ( board_millis() - start_ms < interval_ms) return;
  start_ms += interval_ms;

  uint32_t const btn = board_button_read();

  /* Remote wakeup */
  if ( tud_suspended() && btn ) {
    /*
     * Wake up the host if we are in suspend mode
     * and REMOTE_WAKEUP feature is enabled by host
     */
    tud_remote_wakeup();
  } else {
    /* Send the report for the device being utilized */
    send_hid_report(REPORT_ID_KEYBOARD, btn);
  }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) instance;
  (void) len;

  send_hid_report(report[0], board_button_read());
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

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    if (report_id == REPORT_ID_KEYBOARD)
    {
      /* bufsize should be (at least) 1 */
      if ( bufsize < 1 ) return;

      /* First byte of buffer will be modifier */
      uint8_t const kbd_leds = buffer[0];

      /* If it's caps lock then keep the LED on */
      if (kbd_leds & KEYBOARD_LED_CAPSLOCK) {
        /* Caps lock ON: Disable blink, turn LED on */
        blink_interval_ms = 0;
        board_led_write(true);
      } else {
        /* Caps lock OFF: Do not modify LED behavior */
        board_led_write(false);
        blink_interval_ms = BLINK_MOUNTED;
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