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
#include <ctype.h>

#include "bsp/board_api.h"
#include "tusb.h"

#include "usb_descriptors.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

#define MAX_BUFFER_SIZE         (63)
#define ENTER_KEY               (0x0DU)
#define BACKSPACE_KEY           (0x7FU)
#define HID_USAGE_CONSUMER_NONE (uint16_t)(0x0000U)

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

/* Enum for serialString array index */
enum {
  NEWLINE,
  UNKNOWNCMD,
  MSPM0,
  HELP,
};

void led_blinking_task(void);
void hid_task(void);
void process_buffer(void);
static void cdc_task(void);
static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;
static char cmdBuffer[MAX_BUFFER_SIZE];
uint8_t const caseDiff = 'a' - 'A';
uint8_t cmdBufferIndex = 0;
uint8_t hidCommandCounter = 0;
uint16_t hidCommandQueue[MAX_BUFFER_SIZE] = {HID_USAGE_CONSUMER_NONE};
uint8_t serialString[4][MAX_BUFFER_SIZE] = 
{
  {"\r\n"},
  {"\r\nUnknown command, type help to see list of commands\r\n"},
  {"\r\nMSPM0 is awesome\r\n"},
  {"\r\nCommands: mute|vol up|vol down|bright up|bright down|mspm0\r\n"},
};

/*------------- MAIN -------------*/
int main(void)
{
  memset(cmdBuffer, 0, sizeof(cmdBuffer));
  memset(hidCommandQueue, HID_USAGE_CONSUMER_NONE, sizeof(hidCommandQueue));
  board_init();

  /* Initialize device stack on configured roothub port */
  tusb_rhport_init_t dev_init = {
    .role = TUSB_ROLE_DEVICE,
    .speed = TUSB_SPEED_AUTO
  };
  tusb_init(BOARD_TUD_RHPORT, &dev_init);

  if (board_init_after_tusb) {
    board_init_after_tusb();
  }

  while (1)
  {
    /* TinyUSB device task */
    tud_task();
    /* CDC task */
    cdc_task();
    /* HID task */
    hid_task();
    /* LED task */
    led_blinking_task();
  }
}

/* Echo content to Serial port 0 */
static void echo_serial_port(uint8_t buf[], uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    tud_cdc_n_write_char(0, buf[i]);
  }
  tud_cdc_n_write_flush(0);
}

/* Transmit whatever is left and clean the FIFO */
static void clean_serial_port(void) {
  /* Forcefully transmit whatever we can */
  tud_cdc_n_write_flush(0);
  /* Clear the transmit FIFO */
  tud_cdc_n_write_clear(0);
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

/* Invoked when the USB bus is resumed */
void tud_resume_cb(void)
{
  blink_interval_ms = tud_mounted() ? BLINK_MOUNTED : BLINK_NOT_MOUNTED;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

static void send_hid_report(uint8_t report_id)
{
  /* Return if HID is not ready */
  if ( !tud_hid_ready() ) return;

  /* If the first index is 0 then there is nothing to be executed */
  if (hidCommandQueue[0] != HID_USAGE_CONSUMER_NONE) {
    /* Send HID consumer control command */
    tud_hid_report(report_id, &hidCommandQueue[0], sizeof(hidCommandQueue[0]));
    /* Shift everything left by one */
    for (uint8_t i = 0; i < hidCommandCounter; i++) {
      hidCommandQueue[i] = hidCommandQueue[i+1];
    }
    /* Decrement the command counter */
    hidCommandCounter--;
  }

  /*
   * When done performing task or if there is nothing to execute then send
   * HID_USAGE_CONSUMER_NONE
   */
  tud_hid_report(report_id, HID_USAGE_CONSUMER_NONE, sizeof(HID_USAGE_CONSUMER_NONE));
}

/*
 * Every 25 ms, a single report for keyboard HID profile is sent.
 * tud_hid_report_complete_cb() is used to send the next report after the previous
 * one is completed.
 */
void hid_task(void)
{
  /* Poll every 25ms */
  const uint32_t interval_ms = 25;
  static uint32_t start_ms = 0;

  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;

  /* Remote wakeup */
  if ( tud_suspended() && (hidCommandCounter != 0))
    tud_remote_wakeup();
  else
    send_hid_report(REPORT_ID_CONSUMER_CONTROL);
}

//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+
static void cdc_task(void) {
  /* If unavailable return */
  if (!tud_cdc_n_available(0)) return;

  /* Buffer to temporarily store content */
  uint8_t tempBuffer[MAX_BUFFER_SIZE];
  
  /*
   * Read the serial port and store the content within the temporary
   * buffer and then return the data length
   */
  uint32_t count = tud_cdc_n_read(0, tempBuffer, sizeof(tempBuffer));

  /* Echo the content inside the temporary buffer */
  echo_serial_port(tempBuffer, count);

  /* Go through each character received */
  for (uint8_t i = 0; i < count; i++) {
    /* If the character is upper case make it lower-case */
    if (isupper(tempBuffer[i]))
      tempBuffer[i] += caseDiff;
    
    /* 
     * Check the current character and perform specific actions
     * depending on the case
     * 
     * Case: Action
     * ----------------------------------------------------------------------
     * ENTER_KEY: Process the buffer and clear the buffer once done, then move
     *            to a new line
     * 
     * BACKSPACE_KEY: Remove last item from buffer and decrement the index counter,
     *                if nothing is in the buffer then notify the user that the buffer
     *                is empty
     * 
     * Default: Append latest data from temp buffer into global buffer and then
     *          increment the counter. If there is any overflow then notify the user
     *          and reset the buffer
     */
    if (tempBuffer[i] == ENTER_KEY) {
      /* Clean the transmit fifo before anything is done */
      clean_serial_port();
      /* Process the buffer */
      process_buffer();
      /* Clear the buffer */
      memset(cmdBuffer, 0, sizeof(cmdBuffer));
      /* Reset the index */
      cmdBufferIndex = 0;
    } else if (tempBuffer[i] == BACKSPACE_KEY) {
      /*
       * Remove last item in buffer and decrement index if
       * command buffer is not empty
       */
      if (cmdBufferIndex > 0) {
        /* Remove last item from buffer and decrement index */
        cmdBuffer[cmdBufferIndex - 1] = 0;
        cmdBufferIndex--;
      }
    } else {
      /* Check to see if incrementing index will cause overflow */
      if ((cmdBufferIndex + 1) < MAX_BUFFER_SIZE) {
        /* Append content to command buffer then increment index by one*/
        cmdBuffer[cmdBufferIndex] = (char)tempBuffer[i];
        cmdBufferIndex++;
      } else {
        /* Buffer had overflowed, lets start a new line */
        echo_serial_port(serialString[NEWLINE], sizeof(serialString[NEWLINE]));
        /* Clear out buffer */
        memset(cmdBuffer, 0, sizeof(cmdBuffer));
        /* Reset command buffer index */
        cmdBufferIndex = 0;
      }
    }
  }
}

/*
 * This function will compare the content within the command buffer
 * against specific strings to then update the consumer control
 * command that will be executed within hidtask()
 */
void process_buffer(void)
{
  /* Check if adding another command will cause buffer overflow */
  if ((hidCommandCounter + 1) >= MAX_BUFFER_SIZE) {
    /* If it will cause overflow clear out buffer and reset counter */
    memset(hidCommandQueue, HID_USAGE_CONSUMER_NONE, sizeof(hidCommandQueue));
    hidCommandCounter = 0;
  }

  if (strcmp(cmdBuffer, "mute") == 0) {
    hidCommandQueue[hidCommandCounter] = HID_USAGE_CONSUMER_MUTE;
    hidCommandCounter++;
    echo_serial_port(serialString[NEWLINE], sizeof(serialString[NEWLINE]));
  } else if (strcmp(cmdBuffer, "vol up") == 0) {
    hidCommandQueue[hidCommandCounter] = HID_USAGE_CONSUMER_VOLUME_INCREMENT;
    hidCommandCounter++;
    echo_serial_port(serialString[NEWLINE], sizeof(serialString[NEWLINE]));
  } else if (strcmp(cmdBuffer, "vol down") == 0) {
    hidCommandQueue[hidCommandCounter] = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
    hidCommandCounter++;
    echo_serial_port(serialString[NEWLINE], sizeof(serialString[NEWLINE]));
  } else if (strcmp(cmdBuffer, "bright up") == 0) {
    hidCommandQueue[hidCommandCounter] = HID_USAGE_CONSUMER_BRIGHTNESS_INCREMENT;
    hidCommandCounter++;
    echo_serial_port(serialString[NEWLINE], sizeof(serialString[NEWLINE]));
  } else if (strcmp(cmdBuffer, "bright down") == 0) {
    hidCommandQueue[hidCommandCounter] = HID_USAGE_CONSUMER_BRIGHTNESS_DECREMENT;
    hidCommandCounter++;
    echo_serial_port(serialString[NEWLINE], sizeof(serialString[NEWLINE]));
  } else if (strcmp(cmdBuffer, "mspm0") == 0) {
    echo_serial_port(serialString[MSPM0], sizeof(serialString[MSPM0]));
  } else if (strcmp(cmdBuffer, "help") == 0) {
    echo_serial_port(serialString[HELP], sizeof(serialString[HELP]));
  } else {
    echo_serial_port(serialString[UNKNOWNCMD], sizeof(serialString[UNKNOWNCMD]));
  }
}

/* 
 * Invoked when sent REPORT successfully to host
 * Application can use this to send the next report
 * Note: For composite reports, report[0] is report ID
 */
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) instance;
  (void) len;
  (void) report;
}

/*
 * Invoked when received GET_REPORT control request
 * Application must fill buffer report's content and return its length.
 * Return zero will cause the stack to STALL request
 */
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  /* TODO not Implemented */
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
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  /* blink is disabled */
  if (!blink_interval_ms) return;

  /* Blink every interval ms */
  if ( board_millis() - start_ms < blink_interval_ms) return;
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state;
}
