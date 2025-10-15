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
 *  @file       cdc_host.h
 *  @brief      CDC (Communication Device Class) Host Driver implementation
 * 
 * @details This file implements the CDC host driver for TinyUSB stack.
 * It handles the USB CDC protocol including:
 * - Device enumeration and configuration
 * - Serial data transfer between USB host and device
 * - Control requests handling (line coding, control signals)
 * - Buffer management for data transmission and reception
 *
 * The CDC host implementation supports:
 * - Multiple CDC interfaces
 * - Line coding configuration (baud rate, data bits, parity, stop bits)
 * - Line state signaling (DTR/RTS)
 * - Data transfer with connected CDC devices
 *
 *   <hr>
 ******************************************************************************/
/** @addtogroup CDC_HOST
 * @{
 */

#ifndef _TUSB_CDC_HOST_H_
#define _TUSB_CDC_HOST_H_

#include "cdc.h"

#ifdef __cplusplus
 extern "C" {
#endif

//--------------------------------------------------------------------+
// Class Driver Configuration
//--------------------------------------------------------------------+

/**
 * @brief Controls DTR/RTS line states set during device enumeration
 *
 * This configuration defines what control line states (DTR and RTS) should be set
 * when a CDC device is mounted after enumeration.
 *
 * @note Values are bit-mapped: bit 0 = DTR, bit 1 = RTS
 * - 0x00: No lines asserted
 * - 0x01: DTR asserted
 * - 0x02: RTS asserted 
 * - 0x03: Both DTR and RTS asserted
 *
 * Default value is 0 (no lines asserted) if not defined
 */
#ifndef CFG_TUH_CDC_LINE_CONTROL_ON_ENUM
#define CFG_TUH_CDC_LINE_CONTROL_ON_ENUM    0
#endif

// Set Line Coding on enumeration/mounted, value for cdc_line_coding_t
//#ifndef CFG_TUH_CDC_LINE_CODING_ON_ENUM
//#define CFG_TUH_CDC_LINE_CODING_ON_ENUM   { 115200, CDC_LINE_CODING_STOP_BITS_1, CDC_LINE_CODING_PARITY_NONE, 8 }
//#endif

/**
 * @brief Size of CDC host receive buffer
 *
 * This configuration defines the size in bytes of the buffer used to store
 * data received from CDC devices. A larger buffer allows handling more data
 * before the application needs to process it, but consumes more RAM.
 */
#ifndef CFG_TUH_CDC_RX_BUFSIZE
#define CFG_TUH_CDC_RX_BUFSIZE USBH_EPSIZE_BULK_MAX
#endif

/**
 * @brief Maximum CDC endpoint size for receiving data
 *
 * This configuration defines the maximum endpoint size in bytes that the CDC host
 * driver will use when communicating with CDC devices. This should be set to
 * accommodate the largest endpoint size of any CDC device you intend to support.
 */
#ifndef CFG_TUH_CDC_RX_EPSIZE
#define CFG_TUH_CDC_RX_EPSIZE  USBH_EPSIZE_BULK_MAX
#endif

/**
 * @brief Size of CDC host transmit buffer
 *
 * This configuration defines the size in bytes of the buffer used to store
 * data waiting to be transmitted to CDC devices. A larger buffer allows the
 * application to queue more data for transmission at once, but consumes more RAM.
 */
#ifndef CFG_TUH_CDC_TX_BUFSIZE
#define CFG_TUH_CDC_TX_BUFSIZE USBH_EPSIZE_BULK_MAX
#endif

/**
 * @brief Maximum CDC endpoint size for transmitting data
 *
 * This configuration defines the maximum endpoint size in bytes that the CDC host
 * driver will use when sending data to CDC devices. This should be set to
 * accommodate the largest endpoint size of any CDC device you intend to support.
 */
#ifndef CFG_TUH_CDC_TX_EPSIZE
#define CFG_TUH_CDC_TX_EPSIZE  USBH_EPSIZE_BULK_MAX
#endif

//--------------------------------------------------------------------+
// Application API
//--------------------------------------------------------------------+

/**
 * @brief Get CDC interface index from device address and interface number
 * 
 * This function searches for a CDC interface matching the specified device address
 * and interface number, returning its index within the CDC interface table.
 * 
 * @param[in] daddr      Device address
 * @param[in] itf_num    Interface number
 * 
 * @return Global instance index (0 to CFG_TUH_CDC-1) if found
 * @return TUSB_INDEX_INVALID_8 (0xFF) if the interface is not found
 */
uint8_t tuh_cdc_itf_get_index(uint8_t daddr, uint8_t itf_num);

/**
 * @brief Get detailed information about a CDC interface
 * 
 * This function retrieves detailed information about a CDC interface
 * identified by its index, populating the provided interface info structure.
 * The information includes device address and interface descriptor details.
 * 
 * @param[in]  idx        CDC interface index (from tuh_cdc_itf_get_index)
 * @param[out] info       Pointer to interface information structure to be filled
 * 
 * @return true if successful and interface is mounted
 * @return false if index is invalid or interface is not mounted
 */
bool tuh_cdc_itf_get_info(uint8_t idx, tuh_itf_info_t* info);

/**
 * @brief Check if a CDC device is mounted
 * 
 * This function checks whether a CDC device with the specified interface index
 * is currently mounted and ready for communication.
 * 
 * @param[in] idx        CDC interface index
 * 
 * @return true if the CDC interface is mounted and ready
 * @return false if the interface is not mounted or the index is invalid
 */
bool tuh_cdc_mounted(uint8_t idx);

/**
 * @brief Get current DTR (Data Terminal Ready) status
 * 
 * This function retrieves the current state of the DTR (Data Terminal Ready)
 * control signal for the specified CDC interface.
 * 
 * @param[in] idx        CDC interface index
 * 
 * @return true if DTR is active (asserted)
 * @return false if DTR is inactive or the interface index is invalid
 */
bool tuh_cdc_get_dtr(uint8_t idx);

/**
 * @brief Get current RTS (Request To Send) status
 * 
 * This function retrieves the current state of the RTS (Request To Send)
 * control signal for the specified CDC interface.
 * 
 * @param[in] idx        CDC interface index
 * 
 * @return true if RTS is active (asserted)
 * @return false if RTS is inactive or the interface index is invalid
 */
bool tuh_cdc_get_rts(uint8_t idx);

/**
 * @brief Check if a terminal client is connected to the CDC device
 * 
 * This function checks if a terminal client is connected to the CDC device
 * by examining the DTR (Data Terminal Ready) signal. A terminal is considered
 * connected when DTR is active.
 * 
 * @param[in] idx        CDC interface index
 * 
 * @return true if a terminal is connected (DTR is active)
 * @return false if no terminal is connected or the interface index is invalid
 */
TU_ATTR_ALWAYS_INLINE static inline bool tuh_cdc_connected(uint8_t idx) {
  return tuh_cdc_get_dtr(idx);
}

/**
 * @brief Get locally cached line coding parameters
 * 
 * This function retrieves the locally cached line coding parameters for the
 * specified CDC interface. These parameters include baud rate, stop bits,
 * parity, and data bits.
 * 
 * @param[in]  idx         CDC interface index
 * @param[out] line_coding Pointer to a structure where line coding parameters will be stored
 * 
 * @return true if successful and line coding parameters are valid
 * @return false if the interface index is invalid
 * 
 * @note This function returns cached values and does not make any USB transfers
 * @note Values will be valid if tuh_cdc_set_line_coding() was previously called,
 *       or if CFG_TUH_CDC_LINE_CODING_ON_ENUM is defined
 */
bool tuh_cdc_get_local_line_coding(uint8_t idx, cdc_line_coding_t* line_coding);

//--------------------------------------------------------------------+
// Write API
//--------------------------------------------------------------------+

/**
 * @brief Get the number of bytes available for writing
 * 
 * This function returns the amount of space available in the transmit buffer
 * for the specified CDC interface. Applications can use this to determine how
 * much data can be written without overflowing the buffer.
 * 
 * @param[in] idx        CDC interface index
 * 
 * @return The number of bytes available in the transmit buffer
 * @return 0 if the interface index is invalid
 */
uint32_t tuh_cdc_write_available(uint8_t idx);

/**
 * @brief Write data to CDC interface
 * 
 * This function writes data to the transmit buffer of the specified CDC interface.
 * The data will be sent to the connected CDC device when appropriate.
 * 
 * @param[in] idx        CDC interface index
 * @param[in] buffer     Pointer to the data to be written
 * @param[in] bufsize    Number of bytes to write
 * 
 * @return The number of bytes actually written to the buffer
 * @return 0 if the interface index is invalid or if write failed
 */
uint32_t tuh_cdc_write(uint8_t idx, void const* buffer, uint32_t bufsize);

/**
 * @brief Flush data in the transmit buffer to the CDC device
 * 
 * This function initiates the actual transfer of data from the transmit buffer
 * to the connected CDC device. It should be called after tuh_cdc_write() to
 * ensure data is sent, especially when the buffer is not filled completely.
 * 
 * @param[in] idx        CDC interface index
 * 
 * @return true if the flush operation was successfully initiated
 * @return false if the interface index is invalid or if the device is not ready
 */
uint32_t tuh_cdc_write_flush(uint8_t idx);

/**
 * @brief Clear the transmit buffer
 * 
 * This function clears all data in the transmit buffer for the specified CDC interface,
 * discarding any data that has not yet been sent to the device.
 * 
 * @param[in] idx        CDC interface index
 * 
 * @return true if the buffer was successfully cleared
 * @return false if the interface index is invalid
 */
bool tuh_cdc_write_clear(uint8_t idx);

//--------------------------------------------------------------------+
// Read API
//--------------------------------------------------------------------+

/**
 * @brief Get the number of bytes available for reading
 * 
 * This function returns the number of bytes available in the receive buffer
 * for the specified CDC interface. Applications can use this to determine how
 * much data can be read without blocking.
 * 
 * @param[in] idx        CDC interface index
 * 
 * @return The number of bytes available to read from the receive buffer
 * @return 0 if the interface index is invalid or no data is available
 */
uint32_t tuh_cdc_read_available(uint8_t idx);

/**
 * @brief Read data from CDC interface
 * 
 * This function reads data from the receive buffer of the specified CDC interface.
 * The data is copied from the internal buffer to the provided destination buffer.
 * 
 * @param[in]  idx       CDC interface index
 * @param[out] buffer    Pointer to the destination buffer where data will be stored
 * @param[in]  bufsize   Maximum number of bytes to read
 * 
 * @return The number of bytes actually read
 * @return 0 if the interface index is invalid or no data is available
 */
uint32_t tuh_cdc_read (uint8_t idx, void* buffer, uint32_t bufsize);

/**
 * @brief Peek at the next byte in the receive buffer without removing it
 * 
 * This function examines the next byte in the receive buffer without removing it.
 * The data remains in the buffer and can be later read using tuh_cdc_read().
 * 
 * @param[in]  idx       CDC interface index
 * @param[out] ch        Pointer to store the peeked character
 * 
 * @return true if a character was successfully peeked
 * @return false if the interface index is invalid or no data is available
 */
bool tuh_cdc_peek(uint8_t idx, uint8_t* ch);

/**
 * @brief Clear the receive buffer for a CDC interface
 * 
 * This function discards all data in the receive buffer of the specified CDC interface.
 * Any pending data waiting to be read will be lost.
 * 
 * @param[in] idx       CDC interface index
 * 
 * @return true if the buffer was successfully cleared
 * @return false if the interface index is invalid
 */
bool tuh_cdc_read_clear (uint8_t idx);

//--------------------------------------------------------------------+
// Control Endpoint (Request) API
// Each Function will make a USB control transfer request to/from device
// - If complete_cb is provided, the function will return immediately and invoke
// the callback when request is complete.
// - If complete_cb is NULL, the function will block until request is complete.
//   - In this case, user_data should be pointed to xfer_result_t to hold the transfer result.
//   - The function will return true if transfer is successful, false otherwise.
//--------------------------------------------------------------------+

/**
 * @brief Set control line state (DTR/RTS) for a CDC interface
 * 
 * This function sends a SET_CONTROL_LINE_STATE request to the CDC device to control
 * the DTR (Data Terminal Ready) and RTS (Request To Send) signals.
 * 
 * @param[in] idx         CDC interface index
 * @param[in] line_state  Control line state bitmap:
 *                        - Bit 0: DTR (CDC_CONTROL_LINE_STATE_DTR)
 *                        - Bit 1: RTS (CDC_CONTROL_LINE_STATE_RTS)
 * @param[in] complete_cb Callback function to be invoked when the request completes (can be NULL for blocking operation)
 * @param[in] user_data   User data to be passed to the callback function
 * 
 * @return true if the request was successfully submitted
 * @return false if the interface index is invalid or request submission failed
 */
bool tuh_cdc_set_control_line_state(uint8_t idx, uint16_t line_state, tuh_xfer_cb_t complete_cb, uintptr_t user_data);

/**
 * @brief Set the baud rate for a CDC interface
 * 
 * This function sends a request to set the baud rate of the specified CDC interface.
 * The actual implementation depends on the CDC device type (ACM, FTDI, CP210X, or CH34X).
 * 
 * @param[in] idx         CDC interface index
 * @param[in] baudrate    The desired baud rate in bits per second (e.g., 9600, 115200)
 * @param[in] complete_cb Callback function to be invoked when the request completes (can be NULL for blocking operation)
 * @param[in] user_data   User data to be passed to the callback function
 * 
 * @return true if the request was successfully submitted
 * @return false if the interface index is invalid or request submission failed
 */
bool tuh_cdc_set_baudrate(uint8_t idx, uint32_t baudrate, tuh_xfer_cb_t complete_cb, uintptr_t user_data);

/**
 * @brief Set the data format parameters for a CDC interface
 * 
 * This function configures the data format parameters (stop bits, parity, and data bits)
 * for the specified CDC interface. The actual implementation depends on the CDC device type
 * (ACM, FTDI, CP210X, or CH34X). Note that not all device types support this operation.
 * 
 * @param[in] idx         CDC interface index
 * @param[in] stop_bits   Number of stop bits (e.g., CDC_LINE_CODING_STOP_BITS_1)
 * @param[in] parity      Parity setting (e.g., CDC_LINE_CODING_PARITY_NONE)
 * @param[in] data_bits   Number of data bits (e.g., 8)
 * @param[in] complete_cb Callback function to be invoked when the request completes (can be NULL for blocking operation)
 * @param[in] user_data   User data to be passed to the callback function
 * 
 * @return true if the request was successfully submitted
 * @return false if the interface index is invalid or request submission failed
 */
bool tuh_cdc_set_data_format(uint8_t idx, uint8_t stop_bits, uint8_t parity, uint8_t data_bits, tuh_xfer_cb_t complete_cb, uintptr_t user_data);

/**
 * @brief Set the line coding parameters for a CDC interface
 * 
 * This function configures all line coding parameters (baud rate, stop bits, parity, and data bits)
 * for the specified CDC interface in a single call. The actual implementation depends on the 
 * CDC device type, with full support in ACM and CH34x devices, but limited or no support
 * in FTDI and CP210x devices.
 * 
 * @param[in] idx          CDC interface index
 * @param[in] line_coding  Pointer to a cdc_line_coding_t structure containing the desired parameters:
 *                         - bit_rate: Baud rate in bits per second (e.g., 115200)
 *                         - stop_bits: Number of stop bits (e.g., CDC_LINE_CODING_STOP_BITS_1)
 *                         - parity: Parity setting (e.g., CDC_LINE_CODING_PARITY_NONE)
 *                         - data_bits: Number of data bits (e.g., 8)
 * @param[in] complete_cb  Callback function to be invoked when the request completes (can be NULL for blocking operation)
 * @param[in] user_data    User data to be passed to the callback function
 * 
 * @return true if the request was successfully submitted
 * @return false if the interface index is invalid or request submission failed
 *
 * @note This function is only fully implemented for ACM and CH34x devices. FTDI and CP210x
 *       devices have limited or no support for this operation.
 */
bool tuh_cdc_set_line_coding(uint8_t idx, cdc_line_coding_t const* line_coding, tuh_xfer_cb_t complete_cb, uintptr_t user_data);

// Request to Get Line Coding (ACM only)
// Should only use if tuh_cdc_set_line_coding() / tuh_cdc_get_line_coding() never got invoked and
// CFG_TUH_CDC_LINE_CODING_ON_ENUM is not defined
// bool tuh_cdc_get_line_coding(uint8_t idx, cdc_line_coding_t* coding);

/**
 * @brief Connect to a CDC device by setting both DTR and RTS signals
 * 
 * This function establishes a connection to the specified CDC interface by setting both 
 * the DTR (Data Terminal Ready) and RTS (Request To Send) control signals. This is a 
 * common way to initialize communication with a CDC device.
 * 
 * @param[in] idx         CDC interface index
 * @param[in] complete_cb Callback function to be invoked when the request completes (can be NULL for blocking operation)
 * @param[in] user_data   User data to be passed to the callback function
 * 
 * @return true if the request was successfully submitted
 * @return false if the interface index is invalid or request submission failed
 */
TU_ATTR_ALWAYS_INLINE static inline
bool tuh_cdc_connect(uint8_t idx, tuh_xfer_cb_t complete_cb, uintptr_t user_data) {
  return tuh_cdc_set_control_line_state(idx, CDC_CONTROL_LINE_STATE_DTR | CDC_CONTROL_LINE_STATE_RTS, complete_cb, user_data);
}

/**
 * @brief Disconnect from a CDC device by clearing both DTR and RTS signals
 * 
 * This function terminates the connection to the specified CDC interface by clearing both 
 * the DTR (Data Terminal Ready) and RTS (Request To Send) control signals. This is a 
 * common way to properly disconnect from a CDC device.
 * 
 * @param[in] idx         CDC interface index
 * @param[in] complete_cb Callback function to be invoked when the request completes (can be NULL for blocking operation)
 * @param[in] user_data   User data to be passed to the callback function
 * 
 * @return true if the request was successfully submitted
 * @return false if the interface index is invalid or request submission failed
 */
TU_ATTR_ALWAYS_INLINE static inline
bool tuh_cdc_disconnect(uint8_t idx, tuh_xfer_cb_t complete_cb, uintptr_t user_data) {
  return tuh_cdc_set_control_line_state(idx, 0x00, complete_cb, user_data);
}

//--------------------------------------------------------------------+
// CDC APPLICATION CALLBACKS
//--------------------------------------------------------------------+

/**
 * @brief Callback invoked when a device with CDC interface is mounted
 * 
 * This callback is triggered when a USB device with a CDC interface is successfully 
 * enumerated and configured. It provides an opportunity for the application to 
 * initialize communication with the newly connected CDC device.
 * 
 * @param[in] idx  Index of the CDC interface in the internal pool
 */
TU_ATTR_WEAK extern void tuh_cdc_mount_cb(uint8_t idx);

/**
 * @brief Callback invoked when a device with CDC interface is unmounted
 * 
 * This callback is triggered when a USB device with a CDC interface is disconnected
 * or otherwise becomes unavailable to the host. It provides an opportunity for the
 * application to clean up any resources associated with the CDC interface.
 * 
 * @param[in] idx  Index of the CDC interface that was unmounted
 */
TU_ATTR_WEAK extern void tuh_cdc_umount_cb(uint8_t idx);

/**
 * @brief Callback invoked when new data is received from a CDC device
 * 
 * This callback is triggered when data has been received from a CDC device and is 
 * available to be read. The application should implement this callback to process
 * incoming data from the device.
 * 
 * @param[in] idx  Index of the CDC interface that received data
 */
TU_ATTR_WEAK extern void tuh_cdc_rx_cb(uint8_t idx);

/**
 * @brief Callback invoked when a transmit operation completes
 * 
 * This callback is triggered when a transmit operation to a CDC device completes,
 * indicating that space has become available in the transmit buffer. The application
 * can implement this callback to queue more data for transmission when buffer space
 * becomes available.
 * 
 * @param[in] idx  Index of the CDC interface that completed transmission
 */
TU_ATTR_WEAK extern void tuh_cdc_tx_complete_cb(uint8_t idx);

//--------------------------------------------------------------------+
// Internal Class Driver API
//--------------------------------------------------------------------+
/**
 * @brief Initialize CDC Host driver
 * 
 * This function initializes the CDC Host driver internal structures and buffers.
 * It clears the CDC interface data structures and sets up the endpoint streams
 * for transmit and receive operations.
 * 
 * @return true if initialization was successful
 * @return false if initialization failed
 */
bool cdch_init       (void);

/**
 * @brief De-initialize CDC Host driver
 * 
 * This function de-initializes the CDC Host driver, releasing resources
 * allocated during initialization. It cleans up endpoint streams for
 * all CDC interfaces.
 * 
 * @return true if de-initialization was successful
 * @return false if de-initialization failed
 */
bool cdch_deinit     (void);

/**
 * @brief Open a CDC interface for communication
 * 
 * This function is called during device enumeration when a CDC interface is detected.
 * It analyzes the interface descriptor to determine if it's a supported CDC interface
 * (ACM subclass or vendor-specific with matching VID/PID) and initializes the
 * appropriate driver for the device.
 * 
 * @param[in] rhport      Root hub port where the CDC device is connected
 * @param[in] dev_addr    Address of the USB device
 * @param[in] itf_desc    Pointer to the CDC interface descriptor
 * @param[in] max_len     Maximum length of the descriptor in bytes
 * 
 * @return true if the interface was successfully opened
 * @return false if the interface could not be opened or is not supported
 */
bool cdch_open       (uint8_t rhport, uint8_t dev_addr, tusb_desc_interface_t const *itf_desc, uint16_t max_len);

/**
 * @brief Configure a CDC interface after device is configured
 * 
 * This function is called after the device has been configured during the enumeration process.
 * It creates a fake transfer to initiate the configuration process for the specific CDC interface.
 * The actual configuration is handled by the appropriate serial driver based on the device type
 * (ACM, FTDI, CP210X, or CH34X).
 * 
 * @param[in] dev_addr   Device address
 * @param[in] itf_num    Interface number
 * 
 * @return true if the configuration process was successfully initiated
 * @return false if the interface was not found or the configuration process failed
 */
bool cdch_set_config (uint8_t dev_addr, uint8_t itf_num);

/**
 * @brief Process transfer completion for CDC host endpoints
 * 
 * This function is called when a transfer on a CDC endpoint completes. It handles
 * different types of endpoints:
 * - For TX endpoints: Invokes the tx_complete callback and initiates more transfers
 *   if data is available, or sends a ZLP if needed
 * - For RX endpoints: Processes received data (with special handling for FTDI devices),
 *   invokes the rx callback, and prepares for the next transfer
 * - For notification endpoints: Handles device status notifications
 * 
 * @param[in] dev_addr       Device address
 * @param[in] ep_addr        Endpoint address including direction bit
 * @param[in] event          Transfer result (success, failed, stalled)
 * @param[in] xferred_bytes  Number of bytes transferred
 * 
 * @return true if the transfer was handled successfully
 * @return false if an error occurred during transfer handling
 */
bool cdch_xfer_cb    (uint8_t dev_addr, uint8_t ep_addr, xfer_result_t event, uint32_t xferred_bytes);

/**
 * @brief Close a CDC interface and release its resources
 * 
 * This function is called when a CDC device is disconnected or when the host needs
 * to close communication with a CDC device. It performs the following tasks:
 * - Invokes the application unmount callback
 * - Resets the CDC interface data
 * - Closes the endpoint streams for TX and RX
 * 
 * @param[in] dev_addr    Device address of the CDC device to close
 */
void cdch_close      (uint8_t dev_addr);

#ifdef __cplusplus
 }
#endif

#endif /* _TUSB_CDC_HOST_H_ */

/** @}*/ /* End of CDC_HOST */