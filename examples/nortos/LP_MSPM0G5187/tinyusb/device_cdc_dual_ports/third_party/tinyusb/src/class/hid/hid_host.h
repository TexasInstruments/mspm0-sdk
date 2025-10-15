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
 *  @file       hid_host.h
 *  @brief      HID (Human Interface Device) Host Driver implementation
 * 
 *   @anchor tinyusb_class_hid_host 
 *   # Overview
 * 
 *   This file implements the HID host driver for TinyUSB stack.
 *   It handles the USB HID protocol including:
 *   - Device enumeration and configuration
 *   - Report descriptor parsing
 *   - Input/Output/Feature report transfers
 *   - Protocol mode selection (Boot/Report)
 *   - Idle rate management
 *
 *   The HID implementation supports:
 *   - Multiple HID interfaces
 *   - Various HID device types (keyboard, mouse, gamepad, etc.)
 *   - Boot protocol for keyboards and mice
 *   - Report protocol with descriptor parsing
 *   - Configurable report buffer sizes
 *   - Synchronous and asynchronous report transfers
 *
 *   <hr>
 ******************************************************************************/
/** @addtogroup HID_HOST
 * @{
 */

#ifndef _TUSB_HID_HOST_H_
#define _TUSB_HID_HOST_H_

#include "hid.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------+
// Class Driver Configuration
//--------------------------------------------------------------------+

/**
 * @brief Set size of IN endpoint buffer for HID
 */
#ifndef CFG_TUH_HID_EPIN_BUFSIZE
#define CFG_TUH_HID_EPIN_BUFSIZE 64
#endif

/**
 * @brief Set size of OUT endpoint buffer for HID
 */
#ifndef CFG_TUH_HID_EPOUT_BUFSIZE
#define CFG_TUH_HID_EPOUT_BUFSIZE 64
#endif

/**
 * @brief HID report information structure
 * 
 * This structure contains information about a specific HID report,
 * including its identifier, usage, and usage page.
 */
typedef struct {
  /*! Report identifier (0 if not using report IDs) */
  uint8_t report_id;
  /*! HID usage identifier for this report */
  uint8_t usage;
  /*! HID usage page this report belongs to */
  uint16_t usage_page;

  // TODO still use the endpoint size for now
//  uint8_t in_len;      // length of IN report
//  uint8_t out_len;     // length of OUT report
} tuh_hid_report_info_t;

//--------------------------------------------------------------------+
// Interface API
//--------------------------------------------------------------------+

/**
 * @brief Get the total number of mounted HID interfaces for a specific device
 * 
 * This function counts how many HID interfaces are currently mounted for
 * the specified device address.
 * 
 * @param[in] dev_addr  Device address
 * 
 * @return Number of mounted HID interfaces for the device
 */
uint8_t tuh_hid_itf_get_count(uint8_t dev_addr);

/**
 * @brief Get the total number of mounted HID interfaces across all devices
 * 
 * This function returns the total count of HID interfaces currently mounted
 * across all connected USB devices.
 * 
 * @return Total number of mounted HID interfaces
 */
uint8_t tuh_hid_itf_get_total_count(void);

/**
 * @brief backward compatible rename
 */
#define tuh_hid_instance_count    tuh_hid_itf_get_count

/**
 * @brief Get detailed information about a specific HID interface
 * 
 * This function retrieves detailed information about a HID interface
 * identified by device address and instance number, populating the provided
 * interface info structure.
 * 
 * @param[in]  daddr       Device address
 * @param[in]  idx         Instance number relative to the device
 * @param[out] itf_info    Pointer to interface information structure to be filled
 * 
 * @return true if successful, false if parameters are invalid or interface not found
 */
bool tuh_hid_itf_get_info(uint8_t daddr, uint8_t idx, tuh_itf_info_t* itf_info);

/**
 * @brief Get the global instance index of a HID interface
 * 
 * This function searches for a HID interface matching the specified device address
 * and interface number, returning its global index within the HID interface table.
 * 
 * @param[in] daddr        Device address
 * @param[in] itf_num      Interface number
 * 
 * @return Global instance index (0 to CFG_TUH_HID-1) if found
 * @return TUSB_INDEX_INVALID_8 (0xFF) if the interface is not found
 */
uint8_t tuh_hid_itf_get_index(uint8_t daddr, uint8_t itf_num);

/**
 * @brief Get the interface protocol of a HID device
 * 
 * This function returns the interface protocol value (bInterfaceProtocol) 
 * for the specified HID interface.
 * 
 * @param[in] dev_addr    Device address
 * @param[in] idx         Instance number relative to the device
 * 
 * @return Interface protocol value from hid_interface_protocol_enum_t:
 */
uint8_t tuh_hid_interface_protocol(uint8_t dev_addr, uint8_t idx);

/**
 * @brief Check if a HID interface is mounted
 * 
 * This function determines whether the specified HID interface
 * is currently mounted and ready to use.
 * 
 * @param[in] dev_addr    Device address
 * @param[in] idx         Instance number relative to the device
 * 
 * @return true if the interface is mounted, false otherwise
 */
bool tuh_hid_mounted(uint8_t dev_addr, uint8_t idx);

/**
 * @brief Parse a HID report descriptor
 * 
 * This function parses a HID report descriptor and extracts information about
 * the reports it contains, storing results in the provided array.
 * 
 * @param[out] reports_info_arr  Array to store report information structures
 * @param[in]  arr_count         Maximum number of reports to parse (array size)
 * @param[in]  desc_report       Pointer to the report descriptor buffer
 * @param[in]  desc_len          Length of the report descriptor in bytes
 * 
 * @return Number of reports successfully parsed
 */
TU_ATTR_UNUSED uint8_t tuh_hid_parse_report_descriptor(tuh_hid_report_info_t* reports_info_arr, uint8_t arr_count,
                                                       uint8_t const* desc_report, uint16_t desc_len);

//--------------------------------------------------------------------+
// Control Endpoint API
//--------------------------------------------------------------------+

/**
 * @brief Get current protocol mode of a HID device
 * 
 * This function returns the current protocol mode (boot or report)
 * of the specified HID interface.
 * 
 * @param[in] dev_addr    Device address
 * @param[in] idx         Instance number relative to the device
 * 
 * @return Current protocol value
 */
uint8_t tuh_hid_get_protocol(uint8_t dev_addr, uint8_t idx);

/**
 * @brief Set the default protocol mode for HID devices
 * 
 * This function configures the default protocol mode that will be used
 * when HID devices are enumerated.
 * 
 * @param[in] protocol    Protocol mode to use:
 *                        - HID_PROTOCOL_BOOT (0): Boot Protocol
 *                        - HID_PROTOCOL_REPORT (1): Report Protocol
 */
void tuh_hid_set_default_protocol(uint8_t protocol);

/**
 * @brief Set protocol mode for a HID device
 * 
 * This function configures the protocol mode for the specified HID interface.
 * It is only supported by Boot interfaces (where tuh_hid_interface_protocol() != HID_ITF_PROTOCOL_NONE).
 * The function sends a SET_PROTOCOL control request to the device.
 * 
 * @param[in] dev_addr    Device address
 * @param[in] idx         Instance number relative to the device
 * @param[in] protocol    Protocol mode to set:
 *                        - HID_PROTOCOL_BOOT (0): Boot Protocol
 *                        - HID_PROTOCOL_REPORT (1): Report Protocol
 * 
 * @return true if the request was successfully sent
 *         false if the interface doesn't support protocol switching or request failed
 */
bool tuh_hid_set_protocol(uint8_t dev_addr, uint8_t idx, uint8_t protocol);

/**
 * @brief Send GET_REPORT control request to HID device
 * 
 * This function sends a GET_REPORT control request to the specified HID interface.
 * It allows the host to retrieve a report from the device, which could be an input,
 * output, or feature report depending on the report_type parameter.
 * 
 * @param[in] dev_addr    Device address
 * @param[in] idx         Instance number relative to the device
 * @param[in] report_id   Report ID (0 if report IDs are not used)
 * @param[in] report_type report_type is either Input, Output or Feature, (value from hid_report_type_t)
 * @param[out] report     Buffer to store the received report data
 * @param[in] len         Size of the report buffer in bytes
 * 
 * @return true if the request was successfully sent
 *         false if the request failed
 */
bool tuh_hid_get_report(uint8_t dev_addr, uint8_t idx, uint8_t report_id, uint8_t report_type, void* report, uint16_t len);

/**
 * @brief Send SET_REPORT control request to HID device
 * 
 * This function sends a SET_REPORT control request to the specified HID interface.
 * It allows the host to send a report to the device, which could be an output
 * report (e.g., setting LED states) or a feature report (configuring device behavior).
 * 
 * @param[in] dev_addr    Device address
 * @param[in] idx         Instance number relative to the device
 * @param[in] report_id   Report ID (0 if report IDs are not used)
 * @param[in] report_type report_type is either Input, Output or Feature, (value from hid_report_type_t)
 * @param[in] report      Buffer containing the report data to send
 * @param[in] len         Length of the report data in bytes
 * 
 * @return true if the request was successfully sent
 *         false if the request failed
 */
bool tuh_hid_set_report(uint8_t dev_addr, uint8_t idx, uint8_t report_id, uint8_t report_type,
                        void* report, uint16_t len);

//--------------------------------------------------------------------+
// Interrupt Endpoint API
//--------------------------------------------------------------------+

/**
 * @brief Check if HID interface is ready to receive a report
 * 
 * This function checks if the specified HID interface's IN endpoint is ready
 * to receive a new report from the device. It can be used to determine if
 * it's appropriate to call tuh_hid_receive_report().
 * 
 * @param[in] dev_addr    Device address
 * @param[in] idx         Instance number relative to the device
 * 
 * @return true if the interface is ready to receive a report
 *         false if the interface is busy or not available
 */
bool tuh_hid_receive_ready(uint8_t dev_addr, uint8_t idx);

/**
 * @brief Receive next report from HID device
 * 
 * This function initiates a transfer to receive the next report from the
 * specified HID interface's IN endpoint. When the report is received,
 * the tuh_hid_report_received_cb() callback will be invoked.
 * 
 * @param[in] dev_addr    Device address
 * @param[in] idx         Instance number relative to the device
 * 
 * @return true if the receive transfer was successfully queued
 *         false if failed to queue the transfer (e.g., endpoint is busy)
 */
bool tuh_hid_receive_report(uint8_t dev_addr, uint8_t idx);

/**
 * @brief Abort an ongoing receive transfer on HID interrupt endpoint
 * 
 * This function aborts any ongoing or pending transfer on the interrupt IN endpoint
 * of the specified HID interface. It can be used to cancel a previously
 * initiated receive operation that was started with tuh_hid_receive_report().
 * 
 * @param[in] dev_addr    Device address
 * @param[in] idx         Instance number relative to the device
 * 
 * @return true if the transfer was successfully aborted or no transfer was in progress
 *         false if the interface is not available or abort failed
 */
bool tuh_hid_receive_abort(uint8_t dev_addr, uint8_t idx);

/**
 * @brief Check if HID interface is ready to send a report
 * 
 * This function checks if the specified HID interface's OUT endpoint is ready
 * to send a new report to the device. It can be used to determine if
 * it's appropriate to call tuh_hid_send_report().
 * 
 * @param[in] dev_addr    Device address
 * @param[in] idx         Instance number relative to the device
 * 
 * @return true if the interface is ready to send a report
 *         false if the interface is busy or not available
 */
bool tuh_hid_send_ready(uint8_t dev_addr, uint8_t idx);

/**
 * @brief Send report to HID device using interrupt endpoint
 * 
 * This function sends a report to the specified HID interface using its interrupt OUT endpoint.
 * When the report is successfully delivered, the tuh_hid_report_sent_cb() callback will be invoked.
 * 
 * @param[in] dev_addr    Device address
 * @param[in] idx         Instance number relative to the device
 * @param[in] report_id   Report ID (0 if report IDs are not used)
 * @param[in] report      Buffer containing the report data to send
 * @param[in] len         Length of the report data in bytes
 * 
 * @return true if the send transfer was successfully queued
 *         false if the interface doesn't have an OUT endpoint or the transfer failed
 * 
 * @note If report_id > 0, it will be sent as the first byte followed by the report contents.
 *       If report_id = 0, only the report content is sent.
 */
bool tuh_hid_send_report(uint8_t dev_addr, uint8_t idx, uint8_t report_id, const void* report, uint16_t len);

//--------------------------------------------------------------------+
// Callbacks (Weak is optional)
//--------------------------------------------------------------------+

/**
 * @brief Callback when a HID device is mounted
 * 
 * This callback is invoked when a device with a HID interface is mounted and ready for use.
 * The report descriptor is provided and can be parsed using tuh_hid_parse_report_descriptor()
 * to extract information about the device's capabilities and report formats.
 * 
 * @param[in] dev_addr     Device address
 * @param[in] idx          Instance number relative to the device
 * @param[in] report_desc  Pointer to the HID report descriptor
 * @param[in] desc_len     Length of the report descriptor in bytes
 * 
 * @note If the report descriptor length exceeds CFG_TUH_ENUMERATION_BUFSIZE,
 *       the descriptor will be skipped (report_desc = NULL, desc_len = 0)
 */
TU_ATTR_WEAK void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t idx, uint8_t const* report_desc, uint16_t desc_len);

/**
 * @brief Callback when a HID device is unmounted
 * 
 * This callback is invoked when a device with a HID interface is unmounted
 * (disconnected or detached). This allows the application to clean up any
 * resources associated with the device.
 * 
 * @param[in] dev_addr    Device address
 * @param[in] idx         Instance number relative to the device
 */
TU_ATTR_WEAK void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t idx);

/**
 * @brief Callback when a report is received from a HID device
 * 
 * This callback is invoked when a report has been successfully received from
 * a HID device via its interrupt IN endpoint. This typically happens after
 * calling tuh_hid_receive_report().
 * 
 * @param[in] dev_addr    Device address
 * @param[in] idx         Instance number relative to the device
 * @param[in] report      Buffer containing the received report data
 * @param[in] len         Length of the received report in bytes
 * 
 * @note For devices using report IDs (composite reports), the report ID
 *       will be the first byte of the report buffer.
 */
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t idx, uint8_t const* report, uint16_t len);

/**
 * @brief Callback when a report has been sent to a HID device
 * 
 * This callback is invoked when a report has been successfully sent to
 * a HID device via its interrupt OUT endpoint. This happens after
 * a call to tuh_hid_send_report() has completed.
 * 
 * @param[in] dev_addr    Device address
 * @param[in] idx         Instance number relative to the device
 * @param[in] report      Buffer containing the report data that was sent
 * @param[in] len         Length of the report in bytes
 */
TU_ATTR_WEAK void tuh_hid_report_sent_cb(uint8_t dev_addr, uint8_t idx, uint8_t const* report, uint16_t len);

/**
 * @brief Callback when a GET_REPORT control request completes
 * 
 * This callback is invoked when a GET_REPORT control request initiated by
 * tuh_hid_get_report() has completed. It indicates whether the request was
 * successful and provides information about the report that was requested.
 * 
 * @param[in] dev_addr     Device address
 * @param[in] idx          Instance number relative to the device
 * @param[in] report_id    Report ID that was requested
 * @param[in] report_type  Type of report that was requested (HID_REPORT_TYPE_INPUT,
 *                         HID_REPORT_TYPE_OUTPUT, or HID_REPORT_TYPE_FEATURE)
 * @param[in] len          Length of the received report in bytes, or 0 if an error occurred
 *                         (e.g., device stalled the request)
 */
TU_ATTR_WEAK void tuh_hid_get_report_complete_cb(uint8_t dev_addr, uint8_t idx, uint8_t report_id, uint8_t report_type, uint16_t len);

/**
 * @brief Callback when a SET_REPORT control request completes
 * 
 * This callback is invoked when a SET_REPORT control request initiated by
 * tuh_hid_set_report() has completed. It indicates whether the request was
 * successful and provides information about the report that was sent.
 * 
 * @param[in] dev_addr     Device address
 * @param[in] idx          Instance number relative to the device
 * @param[in] report_id    Report ID that was sent
 * @param[in] report_type  Type of report that was sent (HID_REPORT_TYPE_INPUT,
 *                         HID_REPORT_TYPE_OUTPUT, or HID_REPORT_TYPE_FEATURE)
 * @param[in] len          Length of the report in bytes, or 0 if an error occurred
 *                         (e.g., device stalled the request)
 */
TU_ATTR_WEAK void tuh_hid_set_report_complete_cb(uint8_t dev_addr, uint8_t idx, uint8_t report_id, uint8_t report_type, uint16_t len);

/**
 * @brief Callback when a SET_PROTOCOL control request completes
 * 
 * This callback is invoked when a SET_PROTOCOL control request initiated by
 * tuh_hid_set_protocol() has completed. It indicates whether the protocol
 * mode was successfully changed on the device.
 * 
 * @param[in] dev_addr     Device address
 * @param[in] idx          Instance number relative to the device
 * @param[in] protocol     Protocol that was set (HID_PROTOCOL_BOOT or HID_PROTOCOL_REPORT)
 */
TU_ATTR_WEAK void tuh_hid_set_protocol_complete_cb(uint8_t dev_addr, uint8_t idx, uint8_t protocol);

//--------------------------------------------------------------------+
// Internal Class Driver API
//--------------------------------------------------------------------+

/**
 * @brief Initialize the HID Host driver
 * 
 * This internal function initializes the HID Host driver state.
 * It clears the interface array and prepares the driver for operation.
 * Called during the USB host stack initialization.
 *
 * @return true if initialization is successful
 * @return false if initialization is not successful
 */
bool hidh_init(void);

/**
 * @brief De-initialize the HID Host driver
 * 
 * This internal function cleans up resources used by the HID Host driver.
 * It's called during USB host stack de-initialization or when the host
 * controller is being disabled.
 *
 * @return true if de-initialization is successful
 * @return false if de-initialization is not successful
 */
bool hidh_deinit(void);

/**
 * @brief Open a HID interface
 * 
 * This internal function is called during device enumeration when a HID interface
 * is detected. It parses the interface descriptor, validates it's a HID class,
 * and initializes a new HID interface structure.
 *
 * @param[in] rhport      Root hub port the device is connected to
 * @param[in] dev_addr    Device address
 * @param[in] desc_itf    Pointer to the interface descriptor
 * @param[in] max_len     Maximum length of the descriptor set
 *
 * @return true if the interface was successfully opened
 * @return false if the interface is not HID class, no available interface slots,
 *         or descriptor validation failed
 */
bool hidh_open(uint8_t rhport, uint8_t dev_addr, tusb_desc_interface_t const* desc_itf, uint16_t max_len);

/**
 * @brief Configure a HID interface after device configuration
 * 
 * This internal function is called after the device's configuration has been set.
 * It initiates the HID-specific configuration sequence by creating a control transfer
 * that starts the multi-step process of:
 * 1. SET_IDLE (to disable continuous reports)
 * 2. SET_PROTOCOL (for boot devices)
 * 3. GET_REPORT_DESCRIPTOR
 *
 * @param[in] dev_addr    Device address
 * @param[in] itf_num     Interface number
 *
 * @return true if the configuration process was successfully initiated
 * @return false if the interface is not available or configuration failed
 */
bool hidh_set_config(uint8_t dev_addr, uint8_t itf_num);

/**
 * @brief Transfer callback for HID endpoints
 * 
 * This internal function is called by the USB host stack when a transfer on a HID
 * endpoint completes. It processes both IN transfers (received reports) and 
 * OUT transfers (sent reports).
 *
 * @param[in] dev_addr       Device address
 * @param[in] ep_addr        Endpoint address
 * @param[in] result         Result of the transfer (success, failed, etc.)
 * @param[in] xferred_bytes  Number of bytes transferred
 *
 * @return true to continue with the next transfer
 * @return false to stop the transfer
 */
bool hidh_xfer_cb(uint8_t dev_addr, uint8_t ep_addr, xfer_result_t result, uint32_t xferred_bytes);

/**
 * @brief Close HID interfaces for a device
 * 
 * This internal function is called when a device is disconnected or when
 * the host is disabling a device. It cleans up all HID interfaces associated
 * with the specified device address.
 *
 * @param[in] dev_addr       Device address
 */
void hidh_close(uint8_t dev_addr);

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_HID_HOST_H_ */

/** @}*/ /* End of HID_HOST */
