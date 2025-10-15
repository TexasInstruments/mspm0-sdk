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

/* Blink pattern
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
uint8_t descriptorState;

static void led_blinking_task(void);
static void switch_billboard_task(void);
static void update_billboard_desc(void);
static void cdc_task(void);

/*------------- MAIN -------------*/
int main(void) {
  /* Initialize descriptor state, start with thunderbolt BB descriptor first */
  descriptorState = THUNDERBOLT;

  /* Initialize board */
  board_init();

  /* init device stack on configured roothub port */
  tusb_rhport_init_t dev_init = {
    .role = TUSB_ROLE_DEVICE,
    .speed = TUSB_SPEED_AUTO
  };

  tusb_init(BOARD_TUD_RHPORT, &dev_init);

  if (board_init_after_tusb) {
    board_init_after_tusb();
  }

  while (1) {
    /* tinyusb device task */
    tud_task();
    led_blinking_task();
    switch_billboard_task();
    cdc_task();
  }
}

/* Invoked when device is mounted */
void tud_mount_cb(void) {
  blink_interval_ms = BLINK_MOUNTED;
}

/* Invoked when device is unmounted */
void tud_umount_cb(void) {
  blink_interval_ms = BLINK_NOT_MOUNTED;
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void) {
  static uint32_t start_ms = 0;
  static bool led_state = false;

  /* Blink every interval ms */
  if (board_millis() - start_ms < blink_interval_ms) return; // not enough time
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}

//--------------------------------------------------------------------+
// BILLBOARD TASK
//--------------------------------------------------------------------+
void switch_billboard_task(void) {
  static bool last_button_state = false;
  static uint32_t start_ms = 0;

  /* If 250 ms has not passed since last attempt then return */
  if (board_millis() - start_ms < 250) return;  

  /* Check current button state */
  bool is_pressed = (board_button_read());

  /* 
   * If button on board is pressed then swap out which BOS descriptor
   * that is being used for Billboard enumeration
   */
  if (is_pressed && (last_button_state != true)) {
    descriptorState++;
    if (descriptorState == TOTAL_AUM_COUNT)
      descriptorState = 0;

    /* Upon updating Billboard state, disconnect the device */
    update_billboard_desc();
    tud_disconnect();
  }

  if (!is_pressed && (last_button_state == true)) {
    tud_connect();
  }

  last_button_state = is_pressed;
}

/* Updating Billboard descriptor */
void update_billboard_desc(void) {
  switch (descriptorState) {
  case DISPLAYPORT:
    /* Updating Capability Descriptor to DisplayPort */
    BOS_desc.billboardCapability.aum[0].wSVID = 0xFF01;
    BOS_desc.billboardCapability.aum[0].bAlternateOrUSB4Mode = 0x00;
    BOS_desc.billboardCapability.aum[0].iAlternateOrUSB4ModeString = 6;
    /* Updating AUM Capability Descriptor to DisplayPort */
    BOS_desc.billboardAumCapability[0].dwAlternateModeVdo = 0x000C000C5;
    break;
  case THUNDERBOLT:
    /* Updating Capability Descriptor to THUNDERBOLT */
    BOS_desc.billboardCapability.aum[0].wSVID = 0x8087;
    BOS_desc.billboardCapability.aum[0].bAlternateOrUSB4Mode = 0x01;
    BOS_desc.billboardCapability.aum[0].iAlternateOrUSB4ModeString = 7;
    /* Updating AUM Capability Descriptor to THUNDERBOLT */
    BOS_desc.billboardAumCapability[0].dwAlternateModeVdo = 0x00000002;
    break;
  case USB4:
        /* Updating Capability Descriptor to USB4 */
    BOS_desc.billboardCapability.aum[0].wSVID = 0xFF00;
    BOS_desc.billboardCapability.aum[0].bAlternateOrUSB4Mode = 0x00;
    BOS_desc.billboardCapability.aum[0].iAlternateOrUSB4ModeString = 8;
    /* Updating AUM Capability Descriptor to USB4 */
    BOS_desc.billboardAumCapability[0].dwAlternateModeVdo = 0x2045E000;
    break;
  default:
    /* Should not hit this point, catch in while(1) */
    while(1);
    break;
  }
}

//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+
static void cdc_task(void) {
  if (tud_cdc_n_available(0)) {
    uint8_t buf[64];
    uint32_t count = tud_cdc_n_read(0, buf, sizeof(buf));

    for (uint32_t i = 0; i < count; i++) {
      tud_cdc_n_write_char(0, buf[i]);
    }
    tud_cdc_n_write_flush(0);
  }
}
