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
 * This file is part of the TinyUSB stack.
 */
/*!****************************************************************************
 *  @file       hid_device.h
 *  @brief      HID (Human Interface Device) Device Driver implementation
 * 
 *   @anchor tinyusb_class_hid_device 
 *   # Overview
 * 
 *   This file implements the HID device driver for TinyUSB stack.
 *   It handles the USB HID protocol including:
 *   - Report descriptor definition and parsing
 *   - Input/Output/Feature report transfers
 *   - Protocol mode selection (Boot/Report)
 *   - Idle rate management
 *
 *   The HID implementation supports:
 *   - Multiple HID interfaces
 *   - Various HID device types (keyboard, mouse, gamepad, etc.)
 *   - Boot protocol for keyboards and mice
 *   - Report protocol with descriptor templates
 *   - Configurable report buffer sizes
 *   - Synchronous and asynchronous report transfers
 *
 *   <hr>
 ******************************************************************************/
/** @addtogroup HID_DEVICE
 * @{
 */

#ifndef TUSB_HID_DEVICE_H_
#define TUSB_HID_DEVICE_H_

#include "hid.h"

#ifdef __cplusplus
 extern "C" {
#endif

//--------------------------------------------------------------------+
// Class Driver Default Configure & Validation
//--------------------------------------------------------------------+

#if !defined(CFG_TUD_HID_EP_BUFSIZE) & defined(CFG_TUD_HID_BUFSIZE)
  // TODO warn user to use new name later on
  // #warning CFG_TUD_HID_BUFSIZE is renamed to CFG_TUD_HID_EP_BUFSIZE, please update to use the new name
  #define CFG_TUD_HID_EP_BUFSIZE  CFG_TUD_HID_BUFSIZE
#endif

/**
 * @brief Set size of HID endpoint buffer
 */
#ifndef CFG_TUD_HID_EP_BUFSIZE
  #define CFG_TUD_HID_EP_BUFSIZE     64
#endif

//--------------------------------------------------------------------+
// Application API (Multiple Instances) i.e. CFG_TUD_HID > 1
//--------------------------------------------------------------------+

/**
 * @brief Check if the HID interface is ready to use
 * 
 * This function checks if the specified HID interface is ready to send reports to the host.
 * It verifies that:
 * - The device is ready (connected and configured)
 * - The interface has a valid IN endpoint
 * - The IN endpoint is not currently busy with a transfer
 *
 * @param[in] instance  The HID interface instance.
 *
 * @return true         If the interface is ready to send reports
 * @return false        If the interface is not ready (device not mounted, endpoint not available, or endpoint busy)
 */
bool tud_hid_n_ready(uint8_t instance);

/**
 * @brief Get the interface protocol of a HID device
 * 
 * This function returns the interface protocol value (bInterfaceProtocol) 
 * for the specified HID interface instance.
 *
 * @param[in] instance  The HID interface instance.
 *
 * @return Interface protocol value from hid_interface_protocol_enum_t
 */
uint8_t tud_hid_n_interface_protocol(uint8_t instance);


/**
 * @brief Get the current HID protocol mode
 * 
 * This function returns the current protocol mode (boot or report) 
 * for the specified HID interface instance.
 *
 * @param[in] instance  The HID interface instance.
 *
 * @return HID_PROTOCOL_BOOT (0) for Boot Protocol or HID_PROTOCOL_REPORT (1) for Report Protocol
 */
uint8_t tud_hid_n_get_protocol(uint8_t instance);

/**
 * @brief Send HID report to host
 * 
 * This function sends a HID report to the host through the specified HID interface.
 * It handles the proper formatting of the report data, including prepending the
 * report ID if necessary.
 *
 * @param[in] instance   The HID interface instance.
 * @param[in] report_id  The report ID. Use 0 if report ID is not used in the HID descriptor.
 * @param[in] report     Pointer to the report data buffer.
 * @param[in] len        Length of the report data in bytes.
 *
 * @return true if the report was successfully scheduled for transmission
 * @return false if the report could not be sent (e.g., device not ready, endpoint busy)
 */
bool tud_hid_n_report(uint8_t instance, uint8_t report_id, void const* report, uint16_t len);

/**
 * @brief Send keyboard HID report to host
 * 
 * This function sends a keyboard HID report to the host through the specified HID interface.
 * It uses the standard boot keyboard report format as defined in HID specification.
 *
 * @param[in] instance   The HID interface instance.
 * @param[in] report_id  The report ID. Use 0 if report ID is not used in the HID descriptor.
 * @param[in] modifier   Keyboard modifier keys bitmask.
 * @param[in] keycode    Array of 6 key codes (HID usage codes from HID usage tables).
 *                       Use 0 for unused positions.
 *
 * @return true if the report was successfully scheduled for transmission
 * @return false if the report could not be sent (e.g., device not ready, endpoint busy)
 */
bool tud_hid_n_keyboard_report(uint8_t instance, uint8_t report_id, uint8_t modifier, const uint8_t keycode[6]);

/**
 * @brief Send mouse HID report to host
 * 
 * This function sends a mouse HID report to the host through the specified HID interface.
 * It uses the standard mouse report format with relative positioning as defined in HID specification.
 *
 * @param[in] instance    The HID interface instance.
 * @param[in] report_id   The report ID. Use 0 if report ID is not used in the HID descriptor.
 * @param[in] buttons     Mouse buttons bitmask.
 * @param[in] x           Relative X movement (-127 to 127)
 * @param[in] y           Relative Y movement (-127 to 127)
 * @param[in] vertical    Vertical wheel movement (-127 to 127)
 * @param[in] horizontal  Horizontal wheel movement (-127 to 127)
 *
 * @return true if the report was successfully scheduled for transmission
 * @return false if the report could not be sent (e.g., device not ready, endpoint busy)
 */
bool tud_hid_n_mouse_report(uint8_t instance, uint8_t report_id, uint8_t buttons, int8_t x, int8_t y, int8_t vertical, int8_t horizontal);

/**
 * @brief Send absolute mouse HID report to host
 * 
 * This function sends an absolute positioning mouse HID report to the host through 
 * the specified HID interface. Unlike the standard mouse report which uses relative 
 * movement, this report uses absolute coordinates.
 *
 * @param[in] instance    The HID interface instance.
 * @param[in] report_id   The report ID. Use 0 if report ID is not used in the HID descriptor.
 * @param[in] buttons     Mouse buttons bitmask.
 * @param[in] x           Absolute X position (-32768 to 32767)
 * @param[in] y           Absolute Y position (-32768 to 32767)
 * @param[in] vertical    Vertical wheel movement (-127 to 127)
 * @param[in] horizontal  Horizontal wheel movement (-127 to 127)
 *
 * @return true if the report was successfully scheduled for transmission
 * @return false if the report could not be sent (e.g., device not ready, endpoint busy)
 */
bool tud_hid_n_abs_mouse_report(uint8_t instance, uint8_t report_id, uint8_t buttons, int16_t x, int16_t y, int8_t vertical, int8_t horizontal);

/**
 * @brief Send gamepad HID report to host
 * 
 * This function sends a gamepad HID report to the host through the specified HID interface.
 * It formats the data according to the standard gamepad report descriptor layout defined
 * by TUD_HID_REPORT_DESC_GAMEPAD.
 *
 * @param[in] instance    The HID interface instance.
 * @param[in] report_id   The report ID. Use 0 if report ID is not used in the HID descriptor.
 * @param[in] x           Left analog stick X-axis position (-127 to 127)
 * @param[in] y           Left analog stick Y-axis position (-127 to 127)
 * @param[in] z           Right analog stick X-axis position (-127 to 127)
 * @param[in] rz          Right analog stick Y-axis position (-127 to 127)
 * @param[in] rx          Left trigger position (-127 to 127)
 * @param[in] ry          Right trigger position (-127 to 127)
 * @param[in] hat         D-pad/hat switch position (1-8 for directions, 0 for released)
 * @param[in] buttons     Button states as a bitmap (up to 32 buttons)
 *
 * @return true if the report was successfully scheduled for transmission
 * @return false if the report could not be sent (e.g., device not ready, endpoint busy)
 */
bool tud_hid_n_gamepad_report(uint8_t instance, uint8_t report_id, int8_t x, int8_t y, int8_t z, int8_t rz, int8_t rx, int8_t ry, uint8_t hat, uint32_t buttons);

//--------------------------------------------------------------------+
// Application API (Single Port)
//--------------------------------------------------------------------+

/**
 * @brief Check if the default HID interface is ready to use
 * 
 * This function checks if the default HID interface is ready to send reports to the host.
 * It verifies that:
 * - The device is ready (connected and configured)
 * - The interface has a valid IN endpoint
 * - The IN endpoint is not currently busy with a transfer
 *
 * @return true         If the interface is ready to send reports
 * @return false        If the interface is not ready (device not mounted, endpoint not available, or endpoint busy)
 */
TU_ATTR_ALWAYS_INLINE static inline bool tud_hid_ready(void) {
  return tud_hid_n_ready(0);
}

/**
 * @brief Get the interface protocol of a HID device
 * 
 * This function returns the interface protocol value (bInterfaceProtocol) 
 * for the default HID interface.
 *
 * @return Interface protocol value from hid_interface_protocol_enum_t
 */
TU_ATTR_ALWAYS_INLINE static inline uint8_t tud_hid_interface_protocol(void) {
  return tud_hid_n_interface_protocol(0);
}

/**
 * @brief Get the current HID protocol mode
 * 
 * This function returns the current protocol mode (boot or report) 
 * for the default HID interface.
 *
 * @return HID_PROTOCOL_BOOT (0) for Boot Protocol or HID_PROTOCOL_REPORT (1) for Report Protocol
 */
TU_ATTR_ALWAYS_INLINE static inline uint8_t tud_hid_get_protocol(void) {
  return tud_hid_n_get_protocol(0);
}

/**
 * @brief Send HID report to host
 * 
 * This function sends a HID report to the host through the default HID interface.
 * It handles the proper formatting of the report data, including prepending the
 * report ID if necessary.
 *
 * @param[in] report_id  The report ID. Use 0 if report ID is not used in the HID descriptor.
 * @param[in] report     Pointer to the report data buffer.
 * @param[in] len        Length of the report data in bytes.
 *
 * @return true if the report was successfully scheduled for transmission
 * @return false if the report could not be sent (e.g., device not ready, endpoint busy)
 */
TU_ATTR_ALWAYS_INLINE static inline bool tud_hid_report(uint8_t report_id, void const* report, uint16_t len) {
  return tud_hid_n_report(0, report_id, report, len);
}

/**
 * @brief Send keyboard HID report to host
 * 
 * This function sends a keyboard HID report to the host through the default HID interface.
 * It uses the standard boot keyboard report format as defined in HID specification.
 *
 * @param[in] report_id  The report ID. Use 0 if report ID is not used in the HID descriptor.
 * @param[in] modifier   Keyboard modifier keys bitmask.
 * @param[in] keycode    Array of 6 key codes (HID usage codes from HID usage tables).
 *                       Use 0 for unused positions.
 *
 * @return true if the report was successfully scheduled for transmission
 * @return false if the report could not be sent (e.g., device not ready, endpoint busy)
 */
TU_ATTR_ALWAYS_INLINE static inline bool tud_hid_keyboard_report(uint8_t report_id, uint8_t modifier, const uint8_t keycode[6]) {
  return tud_hid_n_keyboard_report(0, report_id, modifier, keycode);
}

/**
 * @brief Send mouse HID report to host
 * 
 * This function sends a mouse HID report to the host through the default HID interface.
 * It uses the standard mouse report format with relative positioning as defined in HID specification.
 *
 * @param[in] report_id   The report ID. Use 0 if report ID is not used in the HID descriptor.
 * @param[in] buttons     Mouse buttons bitmask.
 * @param[in] x           Relative X movement (-127 to 127)
 * @param[in] y           Relative Y movement (-127 to 127)
 * @param[in] vertical    Vertical wheel movement (-127 to 127)
 * @param[in] horizontal  Horizontal wheel movement (-127 to 127)
 *
 * @return true if the report was successfully scheduled for transmission
 * @return false if the report could not be sent (e.g., device not ready, endpoint busy)
 */
TU_ATTR_ALWAYS_INLINE static inline bool tud_hid_mouse_report(uint8_t report_id, uint8_t buttons, int8_t x, int8_t y, int8_t vertical, int8_t horizontal) {
  return tud_hid_n_mouse_report(0, report_id, buttons, x, y, vertical, horizontal);
}

/**
 * @brief Send absolute mouse HID report to host
 * 
 * This function sends an absolute positioning mouse HID report to the host through 
 * the default HID interface. Unlike the standard mouse report which uses relative 
 * movement, this report uses absolute coordinates.
 *
 * @param[in] report_id   The report ID. Use 0 if report ID is not used in the HID descriptor.
 * @param[in] buttons     Mouse buttons bitmask.
 * @param[in] x           Absolute X position (-32768 to 32767)
 * @param[in] y           Absolute Y position (-32768 to 32767)
 * @param[in] vertical    Vertical wheel movement (-127 to 127)
 * @param[in] horizontal  Horizontal wheel movement (-127 to 127)
 *
 * @return true if the report was successfully scheduled for transmission
 * @return false if the report could not be sent (e.g., device not ready, endpoint busy)
 */
TU_ATTR_ALWAYS_INLINE static inline bool tud_hid_abs_mouse_report(uint8_t report_id, uint8_t buttons, int16_t x, int16_t y, int8_t vertical, int8_t horizontal) {
  return tud_hid_n_abs_mouse_report(0, report_id, buttons, x, y, vertical, horizontal);
}

/**
 * @brief Send gamepad HID report to host
 * 
 * This function sends a gamepad HID report to the host through the default HID interface.
 * It formats the data according to the standard gamepad report descriptor layout defined
 * by TUD_HID_REPORT_DESC_GAMEPAD.
 *
 * @param[in] report_id   The report ID. Use 0 if report ID is not used in the HID descriptor.
 * @param[in] x           Left analog stick X-axis position (-127 to 127)
 * @param[in] y           Left analog stick Y-axis position (-127 to 127)
 * @param[in] z           Right analog stick X-axis position (-127 to 127)
 * @param[in] rz          Right analog stick Y-axis position (-127 to 127)
 * @param[in] rx          Left trigger position (-127 to 127)
 * @param[in] ry          Right trigger position (-127 to 127)
 * @param[in] hat         D-pad/hat switch position (1-8 for directions, 0 for released)
 * @param[in] buttons     Button states as a bitmap (up to 32 buttons)
 *
 * @return true if the report was successfully scheduled for transmission
 * @return false if the report could not be sent (e.g., device not ready, endpoint busy)
 */
TU_ATTR_ALWAYS_INLINE static inline bool tud_hid_gamepad_report(uint8_t report_id, int8_t x, int8_t y, int8_t z, int8_t rz, int8_t rx, int8_t ry, uint8_t hat, uint32_t buttons) {
  return tud_hid_n_gamepad_report(0, report_id, x, y, z, rz, rx, ry, hat, buttons);
}

//--------------------------------------------------------------------+
// Application Callbacks
//--------------------------------------------------------------------+

/**
 * @brief Callback for USB HID Report Descriptor request
 * 
 * This callback is invoked when the USB host requests the HID Report Descriptor
 * via a GET_DESCRIPTOR request. The application must return a pointer to the
 * appropriate descriptor based on the interface instance.
 *
 * @param[in] instance    The HID interface instance for which the descriptor is requested.
 *                        For devices with multiple HID interfaces, this parameter identifies
 *                        which interface's descriptor should be returned.
 *
 * @return A pointer to the HID Report Descriptor for the specified interface instance.
 *         The descriptor must remain valid in memory until the transfer completes.
 */
uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance);

/**
 * @brief Callback for USB HID GET_REPORT control request
 * 
 * This callback is invoked when the USB host sends a GET_REPORT control request
 * to retrieve a specific HID report from the device. The application must fill
 * the provided buffer with the requested report data and return the actual length
 * of the report.
 *
 * @param[in] instance     The HID interface instance for which the report is requested.
 * @param[in] report_id    The Report ID being requested. If report IDs are not used
 *                         in the HID descriptor, this parameter will be zero.
 * @param[in] report_type  Type of report requested (INPUT, OUTPUT, or FEATURE).
 * @param[out] buffer      Buffer where the application should write the report data.
 * @param[in] reqlen       Maximum length of the buffer provided by the host.
 *
 * @return The actual number of bytes written to the buffer.
 *         Return zero to cause the stack to STALL the request, indicating
 *         the report is not supported or not available.
 */
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen);

/**
 * @brief Callback for USB HID SET_REPORT control request
 * 
 * This callback is invoked when:
 * 1. The USB host sends a SET_REPORT control request to set a specific HID report
 * 2. Data is received on the OUT endpoint (for Report ID = 0, Type = OUTPUT)
 *
 * The application should process the report data provided by the host and update 
 * the device state accordingly.
 *
 * @param[in] instance      The HID interface instance for which the report is being set.
 * @param[in] report_id     The Report ID being set. If report IDs are not used
 *                          in the HID descriptor, this parameter will be zero.
 * @param[in] report_type   Type of report being set (INPUT, OUTPUT, or FEATURE).
 * @param[in] buffer        Buffer containing the report data from the host.
 * @param[in] bufsize       Size of the report data in bytes.
 */
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize);

/**
 * @brief Callback for USB HID SET_PROTOCOL control request
 * 
 * This callback is invoked when the USB host sends a SET_PROTOCOL control request
 * to switch between Boot Protocol and Report Protocol modes. The device should
 * adjust its behavior according to the requested protocol mode.
 *
 * @param[in] instance    The HID interface instance for which the protocol mode is being set.
 * @param[in] protocol    The protocol mode being set:
 *                        - HID_PROTOCOL_BOOT (0): Boot Protocol
 *                        - HID_PROTOCOL_REPORT (1): Report Protocol
 */
void tud_hid_set_protocol_cb(uint8_t instance, uint8_t protocol);

/**
 * @brief Callback for USB HID SET_IDLE control request
 * 
 * This callback is invoked when the USB host sends a SET_IDLE control request
 * to configure the idle rate for HID reports. The idle rate determines how often
 * the device should send reports even when there are no changes in the data.
 *
 * @param[in] instance    The HID interface instance for which the idle rate is being set.
 * @param[in] idle_rate   The idle rate value:
 *                        - 0: Only send report if there are changes (i.e., skip duplicates)
 *                        - 1-255: Send at least one report every (idle_rate × 4) ms,
 *                          even if there are no changes in the data.
 *
 * @return true to accept the idle rate setting.
 * @return false to stall the request, indicating the device does not support
 *         the requested idle rate.
 */
bool tud_hid_set_idle_cb(uint8_t instance, uint8_t idle_rate);

/**
 * @brief Callback invoked when a HID report has been successfully sent to host
 * 
 * This callback is invoked when a HID report has been successfully transmitted to
 * the host over the USB IN endpoint. The application can use this callback to
 * send the next report or perform other actions after a successful transfer.
 *
 * @param[in] instance    The HID interface instance that completed the report transfer.
 * @param[in] report      Pointer to the report data that was sent. For reports using
 *                        report IDs, report[0] contains the report ID.
 * @param[in] len         Length of the report in bytes.
 */
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len);

/**
 * @brief Callback invoked when a HID report transmission to host fails
 * 
 * This callback is invoked when a HID report transmission to the host fails
 * due to errors such as bus errors, NAK handling timeout, or other USB
 * transmission issues. The application can use this callback to handle the
 * error, retry the transmission, or take other corrective actions.
 *
 * @param[in] instance       The HID interface instance that experienced the failed report transfer.
 * @param[in] report_type    Type of report that failed (INPUT, OUTPUT, or FEATURE).
 * @param[in] report         Pointer to the report data that failed to send. For reports using
 *                           report IDs, report[0] contains the report ID.
 * @param[in] xferred_bytes  Number of bytes transferred before the failure occurred.
 */
void tud_hid_report_failed_cb(uint8_t instance, hid_report_type_t report_type, uint8_t const* report, uint16_t xferred_bytes);

/* --------------------------------------------------------------------+
 * HID Report Descriptor Template
 *
 * Convenient for declaring popular HID device (keyboard, mouse, consumer,
 * gamepad etc...). Templates take "HID_REPORT_ID(n)" as input, leave
 * empty if multiple reports is not used
 *
 * - Only 1 report: no parameter
 *      uint8_t const report_desc[] = { TUD_HID_REPORT_DESC_KEYBOARD() };
 *
 * - Multiple Reports: "HID_REPORT_ID(ID)" must be passed to template
 *      uint8_t const report_desc[] =
 *      {
 *          TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(1) ) ,
 *          TUD_HID_REPORT_DESC_MOUSE   ( HID_REPORT_ID(2) )
 *      };
 *--------------------------------------------------------------------*/

/**
 * @brief Keyboard Report Descriptor Template
 */
#define TUD_HID_REPORT_DESC_KEYBOARD(...) \
  HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP     )                    ,\
  HID_USAGE      ( HID_USAGE_DESKTOP_KEYBOARD )                    ,\
  HID_COLLECTION ( HID_COLLECTION_APPLICATION )                    ,\
    /* Report ID if any */\
    __VA_ARGS__ \
    /* 8 bits Modifier Keys (Shift, Control, Alt) */ \
    HID_USAGE_PAGE ( HID_USAGE_PAGE_KEYBOARD )                     ,\
      HID_USAGE_MIN    ( 224                                    )  ,\
      HID_USAGE_MAX    ( 231                                    )  ,\
      HID_LOGICAL_MIN  ( 0                                      )  ,\
      HID_LOGICAL_MAX  ( 1                                      )  ,\
      HID_REPORT_COUNT ( 8                                      )  ,\
      HID_REPORT_SIZE  ( 1                                      )  ,\
      HID_INPUT        ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE )  ,\
      /* 8 bit reserved */ \
      HID_REPORT_COUNT ( 1                                      )  ,\
      HID_REPORT_SIZE  ( 8                                      )  ,\
      HID_INPUT        ( HID_CONSTANT                           )  ,\
    /* Output 5-bit LED Indicator Kana | Compose | ScrollLock | CapsLock | NumLock */ \
    HID_USAGE_PAGE  ( HID_USAGE_PAGE_LED                   )       ,\
      HID_USAGE_MIN    ( 1                                       ) ,\
      HID_USAGE_MAX    ( 5                                       ) ,\
      HID_REPORT_COUNT ( 5                                       ) ,\
      HID_REPORT_SIZE  ( 1                                       ) ,\
      HID_OUTPUT       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE  ) ,\
      /* led padding */ \
      HID_REPORT_COUNT ( 1                                       ) ,\
      HID_REPORT_SIZE  ( 3                                       ) ,\
      HID_OUTPUT       ( HID_CONSTANT                            ) ,\
    /* 6-byte Keycodes */ \
    HID_USAGE_PAGE ( HID_USAGE_PAGE_KEYBOARD )                     ,\
      HID_USAGE_MIN    ( 0                                   )     ,\
      HID_USAGE_MAX_N  ( 255, 2                              )     ,\
      HID_LOGICAL_MIN  ( 0                                   )     ,\
      HID_LOGICAL_MAX_N( 255, 2                              )     ,\
      HID_REPORT_COUNT ( 6                                   )     ,\
      HID_REPORT_SIZE  ( 8                                   )     ,\
      HID_INPUT        ( HID_DATA | HID_ARRAY | HID_ABSOLUTE )     ,\
  HID_COLLECTION_END \

/**
 * @brief Mouse Report Descriptor Template
 */
#define TUD_HID_REPORT_DESC_MOUSE(...) \
  HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP      )                   ,\
  HID_USAGE      ( HID_USAGE_DESKTOP_MOUSE     )                   ,\
  HID_COLLECTION ( HID_COLLECTION_APPLICATION  )                   ,\
    /* Report ID if any */\
    __VA_ARGS__ \
    HID_USAGE      ( HID_USAGE_DESKTOP_POINTER )                   ,\
    HID_COLLECTION ( HID_COLLECTION_PHYSICAL   )                   ,\
      HID_USAGE_PAGE  ( HID_USAGE_PAGE_BUTTON  )                   ,\
        HID_USAGE_MIN   ( 1                                      ) ,\
        HID_USAGE_MAX   ( 5                                      ) ,\
        HID_LOGICAL_MIN ( 0                                      ) ,\
        HID_LOGICAL_MAX ( 1                                      ) ,\
        /* Left, Right, Middle, Backward, Forward buttons */ \
        HID_REPORT_COUNT( 5                                      ) ,\
        HID_REPORT_SIZE ( 1                                      ) ,\
        HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
        /* 3 bit padding */ \
        HID_REPORT_COUNT( 1                                      ) ,\
        HID_REPORT_SIZE ( 3                                      ) ,\
        HID_INPUT       ( HID_CONSTANT                           ) ,\
      HID_USAGE_PAGE  ( HID_USAGE_PAGE_DESKTOP )                   ,\
        /* X, Y position [-127, 127] */ \
        HID_USAGE       ( HID_USAGE_DESKTOP_X                    ) ,\
        HID_USAGE       ( HID_USAGE_DESKTOP_Y                    ) ,\
        HID_LOGICAL_MIN ( 0x81                                   ) ,\
        HID_LOGICAL_MAX ( 0x7f                                   ) ,\
        HID_REPORT_COUNT( 2                                      ) ,\
        HID_REPORT_SIZE ( 8                                      ) ,\
        HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_RELATIVE ) ,\
        /* Verital wheel scroll [-127, 127] */ \
        HID_USAGE       ( HID_USAGE_DESKTOP_WHEEL                )  ,\
        HID_LOGICAL_MIN ( 0x81                                   )  ,\
        HID_LOGICAL_MAX ( 0x7f                                   )  ,\
        HID_REPORT_COUNT( 1                                      )  ,\
        HID_REPORT_SIZE ( 8                                      )  ,\
        HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_RELATIVE )  ,\
      HID_USAGE_PAGE  ( HID_USAGE_PAGE_CONSUMER ), \
       /* Horizontal wheel scroll [-127, 127] */ \
        HID_USAGE_N     ( HID_USAGE_CONSUMER_AC_PAN, 2           ), \
        HID_LOGICAL_MIN ( 0x81                                   ), \
        HID_LOGICAL_MAX ( 0x7f                                   ), \
        HID_REPORT_COUNT( 1                                      ), \
        HID_REPORT_SIZE ( 8                                      ), \
        HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_RELATIVE ), \
    HID_COLLECTION_END                                            , \
  HID_COLLECTION_END \

/**
 * @brief Absolute Mouse Report Descriptor Template
 */
#define TUD_HID_REPORT_DESC_ABSMOUSE(...) \
  HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP      )                   ,\
  HID_USAGE      ( HID_USAGE_DESKTOP_MOUSE     )                   ,\
  HID_COLLECTION ( HID_COLLECTION_APPLICATION  )                   ,\
    /* Report ID if any */\
    __VA_ARGS__ \
    HID_USAGE      ( HID_USAGE_DESKTOP_POINTER )                   ,\
    HID_COLLECTION ( HID_COLLECTION_PHYSICAL   )                   ,\
      HID_USAGE_PAGE  ( HID_USAGE_PAGE_BUTTON  )                   ,\
        HID_USAGE_MIN   ( 1                                      ) ,\
        HID_USAGE_MAX   ( 5                                      ) ,\
        HID_LOGICAL_MIN ( 0                                      ) ,\
        HID_LOGICAL_MAX ( 1                                      ) ,\
        /* Left, Right, Middle, Backward, Forward buttons */ \
        HID_REPORT_COUNT( 5                                      ) ,\
        HID_REPORT_SIZE ( 1                                      ) ,\
        HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
        /* 3 bit padding */ \
        HID_REPORT_COUNT( 1                                      ) ,\
        HID_REPORT_SIZE ( 3                                      ) ,\
        HID_INPUT       ( HID_CONSTANT                           ) ,\
      HID_USAGE_PAGE  ( HID_USAGE_PAGE_DESKTOP )                   ,\
        /* X, Y absolute position [0, 32767] */ \
        HID_USAGE       ( HID_USAGE_DESKTOP_X                    ) ,\
        HID_USAGE       ( HID_USAGE_DESKTOP_Y                    ) ,\
        HID_LOGICAL_MIN  ( 0x00                                ) ,\
        HID_LOGICAL_MAX_N( 0x7FFF, 2                           ) ,\
        HID_REPORT_SIZE  ( 16                                  ) ,\
        HID_REPORT_COUNT ( 2                                   ) ,\
        HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
        /* Vertical wheel scroll [-127, 127] */ \
        HID_USAGE       ( HID_USAGE_DESKTOP_WHEEL                )  ,\
        HID_LOGICAL_MIN ( 0x81                                   )  ,\
        HID_LOGICAL_MAX ( 0x7f                                   )  ,\
        HID_REPORT_COUNT( 1                                      )  ,\
        HID_REPORT_SIZE ( 8                                      )  ,\
        HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_RELATIVE )  ,\
      HID_USAGE_PAGE  ( HID_USAGE_PAGE_CONSUMER ), \
       /* Horizontal wheel scroll [-127, 127] */ \
        HID_USAGE_N     ( HID_USAGE_CONSUMER_AC_PAN, 2           ), \
        HID_LOGICAL_MIN ( 0x81                                   ), \
        HID_LOGICAL_MAX ( 0x7f                                   ), \
        HID_REPORT_COUNT( 1                                      ), \
        HID_REPORT_SIZE ( 8                                      ), \
        HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_RELATIVE ), \
    HID_COLLECTION_END                                            , \
  HID_COLLECTION_END \

/**
 * @brief Consumer Control Report Descriptor Template
 */
#define TUD_HID_REPORT_DESC_CONSUMER(...) \
  HID_USAGE_PAGE ( HID_USAGE_PAGE_CONSUMER    )              ,\
  HID_USAGE      ( HID_USAGE_CONSUMER_CONTROL )              ,\
  HID_COLLECTION ( HID_COLLECTION_APPLICATION )              ,\
    /* Report ID if any */\
    __VA_ARGS__ \
    HID_LOGICAL_MIN  ( 0x00                                ) ,\
    HID_LOGICAL_MAX_N( 0x03FF, 2                           ) ,\
    HID_USAGE_MIN    ( 0x00                                ) ,\
    HID_USAGE_MAX_N  ( 0x03FF, 2                           ) ,\
    HID_REPORT_COUNT ( 1                                   ) ,\
    HID_REPORT_SIZE  ( 16                                  ) ,\
    HID_INPUT        ( HID_DATA | HID_ARRAY | HID_ABSOLUTE ) ,\
  HID_COLLECTION_END \

/**
 * @brief System Control Report Descriptor Template
 *
 * 0x00 - do nothing
 * 0x01 - Power Off
 * 0x02 - Standby
 * 0x03 - Wake Host
 */
#define TUD_HID_REPORT_DESC_SYSTEM_CONTROL(...) \
  HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP           )        ,\
  HID_USAGE      ( HID_USAGE_DESKTOP_SYSTEM_CONTROL )        ,\
  HID_COLLECTION ( HID_COLLECTION_APPLICATION       )        ,\
    /* Report ID if any */\
    __VA_ARGS__ \
    /* 2 bit system power control */ \
    HID_LOGICAL_MIN  ( 1                                   ) ,\
    HID_LOGICAL_MAX  ( 3                                   ) ,\
    HID_REPORT_COUNT ( 1                                   ) ,\
    HID_REPORT_SIZE  ( 2                                   ) ,\
    HID_USAGE        ( HID_USAGE_DESKTOP_SYSTEM_POWER_DOWN ) ,\
    HID_USAGE        ( HID_USAGE_DESKTOP_SYSTEM_SLEEP      ) ,\
    HID_USAGE        ( HID_USAGE_DESKTOP_SYSTEM_WAKE_UP    ) ,\
    HID_INPUT        ( HID_DATA | HID_ARRAY | HID_ABSOLUTE ) ,\
    /* 6 bit padding */ \
    HID_REPORT_COUNT ( 1                                   ) ,\
    HID_REPORT_SIZE  ( 6                                   ) ,\
    HID_INPUT        ( HID_CONSTANT                        ) ,\
  HID_COLLECTION_END \

/**
 * @brief Gamepad Report Descriptor Template
 * 
 * Creates a standard HID report descriptor for a gamepad device with the following layout:
 * | X | Y | Z | Rz | Rx | Ry (1 byte each) | hat/DPAD (1 byte) | Button Map (4 bytes) |
 * 
 * The gamepad includes:
 * - 2 analog joysticks (X, Y for left stick, Z, Rz for right stick)
 * - 2 analog triggers (Rx, Ry)
 * - 1 hat/DPAD (8 directions)
 * - 32 buttons (in a 4-byte bitmap)
 */
#define TUD_HID_REPORT_DESC_GAMEPAD(...) \
  HID_USAGE_PAGE ( HID_USAGE_PAGE_DESKTOP     )                 ,\
  HID_USAGE      ( HID_USAGE_DESKTOP_GAMEPAD  )                 ,\
  HID_COLLECTION ( HID_COLLECTION_APPLICATION )                 ,\
    /* Report ID if any */\
    __VA_ARGS__ \
    /* 8 bit X, Y, Z, Rz, Rx, Ry (min -127, max 127 ) */ \
    HID_USAGE_PAGE     ( HID_USAGE_PAGE_DESKTOP                 ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_X                    ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_Y                    ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_Z                    ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_RZ                   ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_RX                   ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_RY                   ) ,\
    HID_LOGICAL_MIN    ( 0x81                                   ) ,\
    HID_LOGICAL_MAX    ( 0x7f                                   ) ,\
    HID_REPORT_COUNT   ( 6                                      ) ,\
    HID_REPORT_SIZE    ( 8                                      ) ,\
    HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
    /* 8 bit DPad/Hat Button Map  */ \
    HID_USAGE_PAGE     ( HID_USAGE_PAGE_DESKTOP                 ) ,\
    HID_USAGE          ( HID_USAGE_DESKTOP_HAT_SWITCH           ) ,\
    HID_LOGICAL_MIN    ( 1                                      ) ,\
    HID_LOGICAL_MAX    ( 8                                      ) ,\
    HID_PHYSICAL_MIN   ( 0                                      ) ,\
    HID_PHYSICAL_MAX_N ( 315, 2                                 ) ,\
    HID_REPORT_COUNT   ( 1                                      ) ,\
    HID_REPORT_SIZE    ( 8                                      ) ,\
    HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
    /* 32 bit Button Map */ \
    HID_USAGE_PAGE     ( HID_USAGE_PAGE_BUTTON                  ) ,\
    HID_USAGE_MIN      ( 1                                      ) ,\
    HID_USAGE_MAX      ( 32                                     ) ,\
    HID_LOGICAL_MIN    ( 0                                      ) ,\
    HID_LOGICAL_MAX    ( 1                                      ) ,\
    HID_REPORT_COUNT   ( 32                                     ) ,\
    HID_REPORT_SIZE    ( 1                                      ) ,\
    HID_INPUT          ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
  HID_COLLECTION_END \

/**
 * @brief FIDO U2F Authenticator Descriptor Template
 * 
 * - 1st parameter is report size, which is 64 bytes maximum in U2F
 * - 2nd parameter is HID_REPORT_ID(n) (optional)
 */
#define TUD_HID_REPORT_DESC_FIDO_U2F(report_size, ...) \
  HID_USAGE_PAGE_N ( HID_USAGE_PAGE_FIDO, 2                    ) ,\
  HID_USAGE      ( HID_USAGE_FIDO_U2FHID                       ) ,\
  HID_COLLECTION ( HID_COLLECTION_APPLICATION                  ) ,\
    /* Report ID if any */ \
    __VA_ARGS__ \
    /* Usage Data In */ \
    HID_USAGE         ( HID_USAGE_FIDO_DATA_IN                 ) ,\
    HID_LOGICAL_MIN   ( 0                                      ) ,\
    HID_LOGICAL_MAX_N ( 0xff, 2                                ) ,\
    HID_REPORT_SIZE   ( 8                                      ) ,\
    HID_REPORT_COUNT  ( report_size                            ) ,\
    HID_INPUT         ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
    /* Usage Data Out */ \
    HID_USAGE         ( HID_USAGE_FIDO_DATA_OUT                ) ,\
    HID_LOGICAL_MIN   ( 0                                      ) ,\
    HID_LOGICAL_MAX_N ( 0xff, 2                                ) ,\
    HID_REPORT_SIZE   ( 8                                      ) ,\
    HID_REPORT_COUNT  ( report_size                            ) ,\
    HID_OUTPUT        ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
  HID_COLLECTION_END \

/**
 * @brief HID Generic Input & Output
 *
 * - 1st parameter is report size (mandatory)
 * - 2nd parameter is report id HID_REPORT_ID(n) (optional)
 */
#define TUD_HID_REPORT_DESC_GENERIC_INOUT(report_size, ...) \
    HID_USAGE_PAGE_N ( HID_USAGE_PAGE_VENDOR, 2   ),\
    HID_USAGE        ( 0x01                       ),\
    HID_COLLECTION   ( HID_COLLECTION_APPLICATION ),\
      /* Report ID if any */\
      __VA_ARGS__ \
      /* Input */ \
      HID_USAGE       ( 0x02                                   ),\
      HID_LOGICAL_MIN ( 0x00                                   ),\
      HID_LOGICAL_MAX_N ( 0xff, 2                              ),\
      HID_REPORT_SIZE ( 8                                      ),\
      HID_REPORT_COUNT( report_size                            ),\
      HID_INPUT       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ),\
      /* Output */ \
      HID_USAGE       ( 0x03                                    ),\
      HID_LOGICAL_MIN ( 0x00                                    ),\
      HID_LOGICAL_MAX_N ( 0xff, 2                               ),\
      HID_REPORT_SIZE ( 8                                       ),\
      HID_REPORT_COUNT( report_size                             ),\
      HID_OUTPUT      ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE  ),\
    HID_COLLECTION_END \

/**
 * @brief HID Lighting and Illumination Report Descriptor Template
 *
 * - 1st parameter is report id (required)
 *   Creates 6 report ids for lighting HID usages in the following order:
 *     report_id+0: HID_USAGE_LIGHTING_LAMP_ARRAY_ATTRIBUTES_REPORT
 *     report_id+1: HID_USAGE_LIGHTING_LAMP_ATTRIBUTES_REQUEST_REPORT
 *     report_id+2: HID_USAGE_LIGHTING_LAMP_ATTRIBUTES_RESPONSE_REPORT
 *     report_id+3: HID_USAGE_LIGHTING_LAMP_MULTI_UPDATE_REPORT
 *     report_id+4: HID_USAGE_LIGHTING_LAMP_RANGE_UPDATE_REPORT
 *     report_id+5: HID_USAGE_LIGHTING_LAMP_ARRAY_CONTROL_REPORT
 */
#define TUD_HID_REPORT_DESC_LIGHTING(report_id) \
  HID_USAGE_PAGE ( HID_USAGE_PAGE_LIGHTING_AND_ILLUMINATION ),\
  HID_USAGE      ( HID_USAGE_LIGHTING_LAMP_ARRAY            ),\
  HID_COLLECTION ( HID_COLLECTION_APPLICATION               ),\
    /* Lamp Array Attributes Report */ \
    HID_REPORT_ID (report_id                                    ) \
    HID_USAGE ( HID_USAGE_LIGHTING_LAMP_ARRAY_ATTRIBUTES_REPORT ),\
    HID_COLLECTION ( HID_COLLECTION_LOGICAL                     ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_LAMP_COUNT                          ),\
      HID_LOGICAL_MIN   ( 0                                                      ),\
      HID_LOGICAL_MAX_N ( 65535, 3                                               ),\
      HID_REPORT_SIZE   ( 16                                                     ),\
      HID_REPORT_COUNT  ( 1                                                      ),\
      HID_FEATURE       ( HID_CONSTANT | HID_VARIABLE | HID_ABSOLUTE             ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_BOUNDING_BOX_WIDTH_IN_MICROMETERS   ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_BOUNDING_BOX_HEIGHT_IN_MICROMETERS  ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_BOUNDING_BOX_DEPTH_IN_MICROMETERS   ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_LAMP_ARRAY_KIND                     ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_MIN_UPDATE_INTERVAL_IN_MICROSECONDS ),\
      HID_LOGICAL_MIN   ( 0                                                      ),\
      HID_LOGICAL_MAX_N ( 2147483647, 3                                          ),\
      HID_REPORT_SIZE   ( 32                                                     ),\
      HID_REPORT_COUNT  ( 5                                                      ),\
      HID_FEATURE       ( HID_CONSTANT | HID_VARIABLE | HID_ABSOLUTE             ),\
    HID_COLLECTION_END ,\
    /* Lamp Attributes Request Report */ \
    HID_REPORT_ID       ( report_id + 1                                     ) \
    HID_USAGE           ( HID_USAGE_LIGHTING_LAMP_ATTRIBUTES_REQUEST_REPORT ),\
    HID_COLLECTION      ( HID_COLLECTION_LOGICAL                            ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_LAMP_ID             ),\
      HID_LOGICAL_MIN   ( 0                                      ),\
      HID_LOGICAL_MAX_N ( 65535, 3                               ),\
      HID_REPORT_SIZE   ( 16                                     ),\
      HID_REPORT_COUNT  ( 1                                      ),\
      HID_FEATURE       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ),\
    HID_COLLECTION_END ,\
    /* Lamp Attributes Response Report */ \
    HID_REPORT_ID       ( report_id + 2                                      ) \
    HID_USAGE           ( HID_USAGE_LIGHTING_LAMP_ATTRIBUTES_RESPONSE_REPORT ),\
    HID_COLLECTION      ( HID_COLLECTION_LOGICAL                             ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_LAMP_ID                        ),\
      HID_LOGICAL_MIN   ( 0                                                 ),\
      HID_LOGICAL_MAX_N ( 65535, 3                                          ),\
      HID_REPORT_SIZE   ( 16                                                ),\
      HID_REPORT_COUNT  ( 1                                                 ),\
      HID_FEATURE       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE            ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_POSITION_X_IN_MICROMETERS      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_POSITION_Y_IN_MICROMETERS      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_POSITION_Z_IN_MICROMETERS      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_UPDATE_LATENCY_IN_MICROSECONDS ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_LAMP_PURPOSES                  ),\
      HID_LOGICAL_MIN   ( 0                                                 ),\
      HID_LOGICAL_MAX_N ( 2147483647, 3                                     ),\
      HID_REPORT_SIZE   ( 32                                                ),\
      HID_REPORT_COUNT  ( 5                                                 ),\
      HID_FEATURE       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE            ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_RED_LEVEL_COUNT                ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_GREEN_LEVEL_COUNT              ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_BLUE_LEVEL_COUNT               ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_INTENSITY_LEVEL_COUNT          ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_IS_PROGRAMMABLE                ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_INPUT_BINDING                  ),\
      HID_LOGICAL_MIN   ( 0                                                 ),\
      HID_LOGICAL_MAX_N ( 255, 2                                            ),\
      HID_REPORT_SIZE   ( 8                                                 ),\
      HID_REPORT_COUNT  ( 6                                                 ),\
      HID_FEATURE       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE            ),\
    HID_COLLECTION_END ,\
    /* Lamp Multi-Update Report */ \
    HID_REPORT_ID       ( report_id + 3                               ) \
    HID_USAGE           ( HID_USAGE_LIGHTING_LAMP_MULTI_UPDATE_REPORT ),\
    HID_COLLECTION      ( HID_COLLECTION_LOGICAL                      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_LAMP_COUNT               ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_LAMP_UPDATE_FLAGS        ),\
      HID_LOGICAL_MIN   ( 0                                           ),\
      HID_LOGICAL_MAX   ( 8                                           ),\
      HID_REPORT_SIZE   ( 8                                           ),\
      HID_REPORT_COUNT  ( 2                                           ),\
      HID_FEATURE       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_LAMP_ID                  ),\
      HID_LOGICAL_MIN   ( 0                                           ),\
      HID_LOGICAL_MAX_N ( 65535, 3                                    ),\
      HID_REPORT_SIZE   ( 16                                          ),\
      HID_REPORT_COUNT  ( 8                                           ),\
      HID_FEATURE       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_RED_UPDATE_CHANNEL       ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_GREEN_UPDATE_CHANNEL     ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_BLUE_UPDATE_CHANNEL      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_INTENSITY_UPDATE_CHANNEL ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_RED_UPDATE_CHANNEL       ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_GREEN_UPDATE_CHANNEL     ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_BLUE_UPDATE_CHANNEL      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_INTENSITY_UPDATE_CHANNEL ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_RED_UPDATE_CHANNEL       ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_GREEN_UPDATE_CHANNEL     ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_BLUE_UPDATE_CHANNEL      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_INTENSITY_UPDATE_CHANNEL ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_RED_UPDATE_CHANNEL       ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_GREEN_UPDATE_CHANNEL     ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_BLUE_UPDATE_CHANNEL      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_INTENSITY_UPDATE_CHANNEL ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_RED_UPDATE_CHANNEL       ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_GREEN_UPDATE_CHANNEL     ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_BLUE_UPDATE_CHANNEL      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_INTENSITY_UPDATE_CHANNEL ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_RED_UPDATE_CHANNEL       ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_GREEN_UPDATE_CHANNEL     ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_BLUE_UPDATE_CHANNEL      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_INTENSITY_UPDATE_CHANNEL ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_RED_UPDATE_CHANNEL       ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_GREEN_UPDATE_CHANNEL     ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_BLUE_UPDATE_CHANNEL      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_INTENSITY_UPDATE_CHANNEL ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_RED_UPDATE_CHANNEL       ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_GREEN_UPDATE_CHANNEL     ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_BLUE_UPDATE_CHANNEL      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_INTENSITY_UPDATE_CHANNEL ),\
      HID_LOGICAL_MIN   ( 0                                           ),\
      HID_LOGICAL_MAX_N ( 255, 2                                      ),\
      HID_REPORT_SIZE   ( 8                                           ),\
      HID_REPORT_COUNT  ( 32                                          ),\
      HID_FEATURE       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE      ),\
    HID_COLLECTION_END ,\
    /* Lamp Range Update Report */ \
    HID_REPORT_ID       ( report_id + 4 ) \
    HID_USAGE           ( HID_USAGE_LIGHTING_LAMP_RANGE_UPDATE_REPORT ),\
    HID_COLLECTION      ( HID_COLLECTION_LOGICAL                      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_LAMP_UPDATE_FLAGS        ),\
      HID_LOGICAL_MIN   ( 0                                           ),\
      HID_LOGICAL_MAX   ( 8                                           ),\
      HID_REPORT_SIZE   ( 8                                           ),\
      HID_REPORT_COUNT  ( 1                                           ),\
      HID_FEATURE       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_LAMP_ID_START            ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_LAMP_ID_END              ),\
      HID_LOGICAL_MIN   ( 0                                           ),\
      HID_LOGICAL_MAX_N ( 65535, 3                                    ),\
      HID_REPORT_SIZE   ( 16                                          ),\
      HID_REPORT_COUNT  ( 2                                           ),\
      HID_FEATURE       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_RED_UPDATE_CHANNEL       ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_GREEN_UPDATE_CHANNEL     ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_BLUE_UPDATE_CHANNEL      ),\
      HID_USAGE         ( HID_USAGE_LIGHTING_INTENSITY_UPDATE_CHANNEL ),\
      HID_LOGICAL_MIN   ( 0                                           ),\
      HID_LOGICAL_MAX_N ( 255, 2                                      ),\
      HID_REPORT_SIZE   ( 8                                           ),\
      HID_REPORT_COUNT  ( 4                                           ),\
      HID_FEATURE       ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE      ),\
    HID_COLLECTION_END ,\
    /* Lamp Array Control Report */ \
    HID_REPORT_ID      ( report_id + 5                                ) \
    HID_USAGE          ( HID_USAGE_LIGHTING_LAMP_ARRAY_CONTROL_REPORT ),\
    HID_COLLECTION     ( HID_COLLECTION_LOGICAL                       ),\
      HID_USAGE        ( HID_USAGE_LIGHTING_AUTONOMOUS_MODE     ),\
      HID_LOGICAL_MIN  ( 0                                      ),\
      HID_LOGICAL_MAX  ( 1                                      ),\
      HID_REPORT_SIZE  ( 8                                      ),\
      HID_REPORT_COUNT ( 1                                      ),\
      HID_FEATURE      ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ),\
    HID_COLLECTION_END ,\
  HID_COLLECTION_END \

//--------------------------------------------------------------------+
// Internal Class Driver API
//--------------------------------------------------------------------+

/**
 * @brief Initialize the HID Device driver
 * 
 * This internal function initializes the HID Device driver component of the TinyUSB stack.
 * It sets up the necessary data structures and state for handling HID device functionality,
 * including report endpoints, interface settings, and protocol state.
 *
 * This function is called automatically by the TinyUSB device stack during device 
 * initialization. It prepares the HID class driver to handle HID-specific USB requests
 * and report transfers.
 */
void     hidd_init            (void);

/**
 * @brief De-initialize the HID Device driver
 * 
 * This internal function de-initializes the HID Device driver component of the TinyUSB stack.
 * It releases resources and cleans up any state used by the HID device functionality.
 *
 * This function is called automatically by the TinyUSB device stack during device 
 * de-initialization or when the device is being detached from the bus. It ensures
 * proper cleanup of HID-specific resources.
 *
 * @return true if the de-initialization is successful
 * @return false if the de-initialization fails
 */
bool     hidd_deinit          (void);

/**
 * @brief Reset the HID Device driver state
 * 
 * This internal function resets the HID Device driver's internal state when a USB reset
 * is detected on the specified root hub port. It clears all interface-specific data
 * structures and prepares the driver for a new enumeration sequence.
 *
 * @param[in] rhport  Root Hub Port index (USB controller index) where the reset occurred
 */
void     hidd_reset           (uint8_t rhport);

/**
 * @brief Open and initialize a HID interface
 * 
 * This internal function is called during device enumeration when the host configures
 * a HID interface. It parses the interface descriptor, initializes the interface-specific
 * data structures, and prepares endpoints for data transfer.
 *
 * The function validates that the interface is a HID class interface, finds an available
 * interface slot in the driver's internal array, and configures the endpoints according
 * to the interface descriptor.
 *
 * @param[in] rhport     Root Hub Port index (USB controller index)
 * @param[in] itf_desc   Pointer to the interface descriptor
 * @param[in] max_len    Maximum length of the descriptor set
 *
 * @return               The number of bytes processed from the descriptor set,
 *                       or 0 if the interface couldn't be configured
 */
uint16_t hidd_open            (uint8_t rhport, tusb_desc_interface_t const * itf_desc, uint16_t max_len);

/**
 * @brief Process HID-specific control transfers
 * 
 * This internal callback function handles control transfers targeted at HID interfaces.
 * It processes standard and class-specific HID requests according to the USB HID specification,
 * including descriptor requests, report requests, protocol mode selection, and idle rate control.
 *
 * The function is invoked by the USB device stack during different stages of control transfers
 * (setup, data, acknowledgment) and responds appropriately to each request type.
 *
 * @param[in] rhport    Root Hub Port index (USB controller index)
 * @param[in] stage     Current stage of the control transfer (setup, data, or ACK)
 * @param[in] request   Pointer to the USB control request structure
 *
 * @return              true if the request was processed successfully
 * @return              false to stall the control endpoint (unsupported request)
 */
bool     hidd_control_xfer_cb (uint8_t rhport, uint8_t stage, tusb_control_request_t const * request);

/**
 * @brief Process HID data transfers
 * 
 * This internal callback function handles data transfer completion events for HID endpoints.
 * It is invoked by the USB device stack when data has been transmitted or received on
 * a HID endpoint, and it dispatches the appropriate callbacks based on the endpoint
 * direction and transfer result.
 *
 * For IN endpoints (device-to-host), it calls the report completion or failure callbacks.
 * For OUT endpoints (host-to-device), it processes received reports and prepares the
 * endpoint for the next transfer.
 *
 * @param[in] rhport          Root Hub Port index (USB controller index)
 * @param[in] ep_addr         Endpoint address on which the transfer occurred
 * @param[in] event           Result of the transfer (success or error code)
 * @param[in] xferred_bytes   Number of bytes transferred
 *
 * @return                    true to continue with further processing
 * @return                    false to halt further processing
 */
bool     hidd_xfer_cb         (uint8_t rhport, uint8_t ep_addr, xfer_result_t event, uint32_t xferred_bytes);

#ifdef __cplusplus
 }
#endif

#endif

/** @}*/ /* End of HID_DEVICE */