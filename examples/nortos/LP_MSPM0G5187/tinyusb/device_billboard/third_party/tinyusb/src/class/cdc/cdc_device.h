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
 *  @file       cdc_device.h
 *  @brief      CDC (Communication Device Class) Device Driver implementation
 * 
 * @details This file implements the CDC device driver for TinyUSB stack.
 * It handles the USB CDC protocol including:
 * - Device enumeration and configuration
 * - Serial data transfer between USB host and device
 * - Control requests handling (line coding, control signals)
 * - FIFO buffer management for TX and RX data
 *
 * The CDC implementation supports:
 * - Multiple CDC interfaces
 * - Configurable buffer sizes via CFG_TUD_CDC_RX_BUFSIZE and CFG_TUD_CDC_TX_BUFSIZE
 * - Line coding configuration (baud rate, data bits, parity, stop bits)
 * - Line state signaling (DTR/RTS)
 * - Character matching for simplified input processing
 *
 *   <hr>
 ******************************************************************************/
/** @addtogroup CDC_DEVICE
 * @{
 */

#ifndef TUSB_CDC_DEVICE_H_
#define TUSB_CDC_DEVICE_H_

#include "cdc.h"

//--------------------------------------------------------------------+
// Class Driver Configuration
//--------------------------------------------------------------------+
#if !defined(CFG_TUD_CDC_EP_BUFSIZE) && defined(CFG_TUD_CDC_EPSIZE)
  #warning CFG_TUD_CDC_EPSIZE is renamed to CFG_TUD_CDC_EP_BUFSIZE, please update to use the new name
  #define CFG_TUD_CDC_EP_BUFSIZE    CFG_TUD_CDC_EPSIZE
#endif

/*!
 * @brief Size of CDC endpoint buffer
 */
#ifndef CFG_TUD_CDC_EP_BUFSIZE
  #define CFG_TUD_CDC_EP_BUFSIZE    (TUD_OPT_HIGH_SPEED ? 512 : 64)
#endif

#ifdef __cplusplus
 extern "C" {
#endif

//--------------------------------------------------------------------+
// Driver Configuration
//--------------------------------------------------------------------+
/**
 * @brief Configuration structure for CDC FIFO persistence options
 *
 * This structure defines configuration options for CDC FIFO persistence
 * across USB bus reset or disconnect events. The structure uses bit fields
 * to minimize memory usage.
 */
typedef struct TU_ATTR_PACKED {
  /*! keep rx fifo on bus reset or disconnect */
  uint8_t rx_persistent : 1;
  /*! keep tx fifo on bus reset or disconnect */
  uint8_t tx_persistent : 1;
} tud_cdc_configure_fifo_t;

/**
 * @brief Configure CDC FIFO persistence behavior
 *
 * This function configures how the CDC FIFOs behave during USB bus reset or
 * disconnect events. By default, FIFOs are cleared on reset/disconnect, but
 * this behavior can be modified to maintain data integrity in certain applications.
 *
 * @param[in] cfg    Pointer to configuration structure containing persistence options.
 *                   Set rx_persistent to keep RX FIFO data on reset/disconnect.
 *                   Set tx_persistent to keep TX FIFO data on reset/disconnect.
 *
 * @return bool      true if configuration was successful, false if cfg pointer is NULL
 */
bool tud_cdc_configure_fifo(tud_cdc_configure_fifo_t const* cfg);

//--------------------------------------------------------------------+
// Application API (Multiple Ports) i.e. CFG_TUD_CDC > 1
//--------------------------------------------------------------------+

/**
 * @brief Check if CDC interface is ready for communication
 *
 * This function checks if the specified CDC interface is ready for data transfer.
 * An interface is considered ready when the device is connected to a host,
 * the USB stack is ready, and both IN and OUT endpoints are configured.
 *
 * @param[in] itf    CDC interface number (zero-based)
 *
 * @return bool      true if the interface is ready for data transfer,
 *                   false if not ready or not properly configured
 */
bool tud_cdc_n_ready(uint8_t itf);

/**
 * @brief Check if terminal is connected to this CDC port
 *
 * This function checks if a terminal client (like a serial terminal program)
 * is connected to the specified CDC interface. The connection state is determined
 * by checking the DTR (Data Terminal Ready) bit in the line state.
 *
 * @param[in] itf    CDC interface number (zero-based)
 *
 * @return bool      true if a terminal is connected (DTR bit is active),
 *                   false otherwise
 */
bool tud_cdc_n_connected(uint8_t itf);

/**
 * @brief Get current line state for a CDC interface
 *
 * This function returns the current line state control signals for the specified
 * CDC interface. The line state contains information about the DTR (Data Terminal Ready)
 * and RTS (Request To Send) signals from the host.
 *
 * @param[in] itf    CDC interface number (zero-based)
 *
 * @return uint8_t   The current line state with bit fields:
 *                   - Bit 0: DTR (Data Terminal Ready) signal
 *                   - Bit 1: RTS (Request To Send) signal
 *                   A value of 1 indicates the signal is active
 */
uint8_t tud_cdc_n_get_line_state(uint8_t itf);

/**
 * @brief Get current line coding parameters for a CDC interface
 *
 * This function retrieves the current line coding parameters for the specified CDC interface.
 * Line coding includes communication parameters such as baud rate, number of data bits,
 * stop bits, and parity type.
 *
 * @param[in]  itf     CDC interface number (zero-based)
 * @param[out] coding  Pointer to a cdc_line_coding_t structure where the line coding
 *                     parameters will be stored
 */
void tud_cdc_n_get_line_coding(uint8_t itf, cdc_line_coding_t* coding);

/**
 * @brief Set special character that will trigger a callback when received
 *
 * This function configures a specific character that, when received on the
 * specified CDC interface, will trigger the tud_cdc_rx_wanted_cb() callback.
 * This provides a convenient way to detect and respond to special characters
 * in the incoming data stream without having to manually scan the received data.
 *
 * @param[in] itf      CDC interface number (zero-based)
 * @param[in] wanted   The character to detect in the incoming data stream
 */
void tud_cdc_n_set_wanted_char(uint8_t itf, char wanted);

/**
 * @brief Get the number of bytes available for reading
 *
 * This function returns the number of bytes currently available in the receive buffer
 * of the specified CDC interface. These bytes have been received from the host
 * and are ready to be read by the application.
 *
 * @param[in] itf      CDC interface number (zero-based)
 *
 * @return uint32_t    The number of bytes available to read from the receive buffer
 */
uint32_t tud_cdc_n_available(uint8_t itf);

// Read received bytes
/**
 * @brief Read data from CDC interface receive buffer
 *
 * This function reads data that has been received from the host on the specified
 * CDC interface. It transfers bytes from the internal receive FIFO buffer to the
 * provided application buffer.
 *
 * @param[in]  itf      CDC interface number (zero-based)
 * @param[out] buffer   Pointer to the buffer where data will be stored
 * @param[in]  bufsize  Maximum number of bytes to read
 *
 * @return uint32_t     The actual number of bytes read, which may be less than requested
 *                      if fewer bytes are available
 */
uint32_t tud_cdc_n_read(uint8_t itf, void* buffer, uint32_t bufsize);

/**
 * @brief Read a single character from CDC interface
 *
 * This function reads a single byte (character) from the receive buffer
 * of the specified CDC interface. It's a convenience function for applications
 * that process data one character at a time.
 *
 * @param[in] itf      CDC interface number (zero-based)
 *
 * @return int32_t     The read character (0-255) if data is available,
 *                     or -1 if no data is available
 */
TU_ATTR_ALWAYS_INLINE static inline int32_t tud_cdc_n_read_char(uint8_t itf) {
  uint8_t ch;
  return tud_cdc_n_read(itf, &ch, 1) ? (int32_t) ch : -1;
}

/**
 * @brief Clear the received data FIFO buffer
 *
 * This function clears (discards) all data in the receive FIFO buffer for the
 * specified CDC interface. It's useful when you want to ignore previously received
 * data and start fresh.
 *
 * @param[in] itf      CDC interface number (zero-based)
 */
void tud_cdc_n_read_flush(uint8_t itf);

/**
 * @brief Peek at the next byte in the receive buffer without removing it
 *
 * This function allows examining the next byte in the receive buffer of the specified
 * CDC interface without actually removing it from the buffer. This is useful when
 * you need to look ahead in the data stream without consuming the data.
 *
 * @param[in]  itf     CDC interface number (zero-based)
 * @param[out] ui8     Pointer to a byte variable where the peeked data will be stored
 *
 * @return bool        true if data was available and stored in ui8,
 *                     false if no data is available
 */
bool tud_cdc_n_peek(uint8_t itf, uint8_t* ui8);

/**
 * @brief Write data to CDC interface transmit buffer
 *
 * This function writes data to the transmit buffer of the specified CDC interface.
 * The data will be sent to the host when the buffer is sufficiently full or when
 * explicitly flushed.
 *
 * @param[in] itf       CDC interface number (zero-based)
 * @param[in] buffer    Pointer to the data to be written
 * @param[in] bufsize   Number of bytes to write
 *
 * @return uint32_t     The number of bytes actually written to the buffer, which may be
 *                      less than requested if there's not enough space in the buffer
 */
uint32_t tud_cdc_n_write(uint8_t itf, void const* buffer, uint32_t bufsize);

/**
 * @brief Write a single character to CDC interface
 *
 * This function writes a single byte (character) to the transmit buffer of the
 * specified CDC interface. It's a convenience wrapper around tud_cdc_n_write()
 * for applications that process data one character at a time.
 *
 * @param[in] itf     CDC interface number (zero-based)
 * @param[in] ch      The character to write
 *
 * @return uint32_t   1 if the character was written successfully,
 *                    0 if there was no space in the buffer
 */
TU_ATTR_ALWAYS_INLINE static inline uint32_t tud_cdc_n_write_char(uint8_t itf, char ch) {
  return tud_cdc_n_write(itf, &ch, 1);
}

/**
 * @brief Write a null-terminated string to CDC interface
 *
 * This function writes a null-terminated string to the transmit buffer of the
 * specified CDC interface. It's a convenience wrapper around tud_cdc_n_write()
 * for applications that need to send string data.
 *
 * @param[in] itf     CDC interface number (zero-based)
 * @param[in] str     Pointer to the null-terminated string to write
 *
 * @return uint32_t   The number of bytes written to the buffer, which may be
 *                    less than the string length if there's not enough space
 */
TU_ATTR_ALWAYS_INLINE static inline uint32_t tud_cdc_n_write_str(uint8_t itf, char const* str) {
  return tud_cdc_n_write(itf, str, strlen(str));
}

/**
 * @brief Force sending data if possible
 *
 * This function attempts to send any data currently in the transmit buffer
 * of the specified CDC interface to the host. It initiates a USB transfer
 * with the buffered data.
 *
 * @param[in] itf      CDC interface number (zero-based)
 *
 * @return uint32_t    The number of bytes sent in this transfer,
 *                     or 0 if no transfer was initiated (e.g., if USB is not ready
 *                     or if there's no data to send)
 */
uint32_t tud_cdc_n_write_flush(uint8_t itf);

/**
 * @brief Return the number of bytes available for writing to TX FIFO buffer
 *
 * This function returns the amount of free space available in the transmit FIFO buffer
 * of the specified CDC interface. Applications can use this to determine how much data
 * can be written in a single call to tud_cdc_n_write().
 *
 * @param[in] itf      CDC interface number (zero-based)
 *
 * @return uint32_t    The number of bytes that can be written to the transmit buffer
 */
uint32_t tud_cdc_n_write_available(uint8_t itf);

/**
 * @brief Clear the transmit FIFO buffer
 *
 * This function clears (discards) all data in the transmit FIFO buffer for the
 * specified CDC interface. It's useful when you want to abort a pending transmission
 * and start fresh.
 *
 * @param[in] itf      CDC interface number (zero-based)
 *
 * @return bool        true if the operation was successful,
 *                     false otherwise
 */
bool tud_cdc_n_write_clear(uint8_t itf);

//--------------------------------------------------------------------+
// Application API (Single Port)
//--------------------------------------------------------------------+

/**
 * @brief Check if the default CDC interface is ready for communication
 *
 * This function checks if the default CDC interface (interface 0) is ready for data transfer.
 * An interface is considered ready when the device is connected to a host,
 * the USB stack is ready, and both IN and OUT endpoints are configured.
 *
 * @return bool      true if the default CDC interface is ready for data transfer,
 *                   false if not ready or not properly configured
 */
TU_ATTR_ALWAYS_INLINE static inline bool tud_cdc_ready(void) {
  return tud_cdc_n_ready(0);
}

/**
 * @brief Check if terminal is connected to the default CDC port
 *
 * This function checks if a terminal client (like a serial terminal program)
 * is connected to the default CDC interface (interface 0). The connection state
 * is determined by checking the DTR (Data Terminal Ready) bit in the line state.
 *
 * @return bool      true if a terminal is connected (DTR bit is active),
 *                   false otherwise
 */
TU_ATTR_ALWAYS_INLINE static inline bool tud_cdc_connected(void) {
  return tud_cdc_n_connected(0);
}

/**
 * @brief Get current line state for the default CDC interface
 *
 * This function returns the current line state control signals for the default
 * CDC interface (interface 0). The line state contains information about the
 * DTR (Data Terminal Ready) and RTS (Request To Send) signals from the host.
 *
 * @return uint8_t   The current line state with bit fields:
 *                   - Bit 0: DTR (Data Terminal Ready) signal
 *                   - Bit 1: RTS (Request To Send) signal
 *                   A value of 1 indicates the signal is active
 */
TU_ATTR_ALWAYS_INLINE static inline uint8_t tud_cdc_get_line_state(void) {
  return tud_cdc_n_get_line_state(0);
}

/**
 * @brief Get current line coding parameters for the default CDC interface
 *
 * This function retrieves the current line coding parameters for the default CDC
 * interface (interface 0). Line coding includes communication parameters such as
 * baud rate, number of data bits, stop bits, and parity type.
 *
 * @param[out] coding  Pointer to a cdc_line_coding_t structure where the line
 *                     coding parameters will be stored
 */
TU_ATTR_ALWAYS_INLINE static inline void tud_cdc_get_line_coding(cdc_line_coding_t* coding) {
  tud_cdc_n_get_line_coding(0, coding);
}

/**
 * @brief Set special character that will trigger a callback when received
 *
 * This function configures a specific character that, when received on the
 * default CDC interface (interface 0), will trigger the tud_cdc_rx_wanted_cb() callback.
 * This provides a convenient way to detect and respond to special characters
 * in the incoming data stream without having to manually scan the received data.
 *
 * @param[in] wanted   The character to detect in the incoming data stream
 */
TU_ATTR_ALWAYS_INLINE static inline void tud_cdc_set_wanted_char(char wanted) {
  tud_cdc_n_set_wanted_char(0, wanted);
}

/**
 * @brief Get the number of bytes available for reading from the default CDC interface
 *
 * This function returns the number of bytes currently available in the receive buffer
 * of the default CDC interface (interface 0). These bytes have been received from the host
 * and are ready to be read by the application.
 *
 * @return uint32_t    The number of bytes available to read from the receive buffer
 */
TU_ATTR_ALWAYS_INLINE static inline uint32_t tud_cdc_available(void) {
  return tud_cdc_n_available(0);
}

/**
 * @brief Read a single character from the default CDC interface
 *
 * This function reads a single byte (character) from the receive buffer
 * of the default CDC interface (interface 0). It's a convenience function for applications
 * that process data one character at a time.
 *
 * @return int32_t     The read character (0-255) if data is available,
 *                     or -1 if no data is available
 */
TU_ATTR_ALWAYS_INLINE static inline int32_t tud_cdc_read_char(void) {
  return tud_cdc_n_read_char(0);
}

/**
 * @brief Read data from the default CDC interface
 *
 * This function reads data from the receive buffer of the default CDC interface 
 * (interface 0). It transfers up to 'bufsize' bytes from the CDC receive FIFO
 * into the provided buffer.
 *
 * @param[out] buffer  Pointer to the buffer where read data will be stored
 * @param[in]  bufsize Maximum number of bytes to read
 *
 * @return uint32_t    The number of bytes actually read from the receive buffer
 */
TU_ATTR_ALWAYS_INLINE static inline uint32_t tud_cdc_read(void* buffer, uint32_t bufsize) {
  return tud_cdc_n_read(0, buffer, bufsize);
}

/**
 * @brief Flush the default CDC interface's receive buffer
 *
 * This function discards all data in the receive buffer of the default CDC
 * interface (interface 0).
 *
 * @return bool  true if successful, false otherwise
 */
TU_ATTR_ALWAYS_INLINE static inline void tud_cdc_read_flush(void) {
  tud_cdc_n_read_flush(0);
}

/**
 * @brief Peek at the next byte in the default CDC interface's receive buffer
 *
 * This function looks at the next byte in the receive buffer of the default CDC
 * interface (interface 0) without removing it from the buffer.
 *
 * @param[out] ui8  Pointer to store the peeked character
 *
 * @return bool     true if a character was successfully peeked, false if the buffer is empty
 */
TU_ATTR_ALWAYS_INLINE static inline bool tud_cdc_peek(uint8_t* ui8) {
  return tud_cdc_n_peek(0, ui8);
}

/**
 * @brief Write a single character to the default CDC interface
 *
 * This function writes a single character to the transmit buffer of the default CDC
 * interface (interface 0).
 *
 * @param[in] ch  The character to write
 *
 * @return uint32_t  1 if character was successfully written, 0 if the buffer is full
 */
TU_ATTR_ALWAYS_INLINE static inline uint32_t tud_cdc_write_char(char ch) {
  return tud_cdc_n_write_char(0, ch);
}

/**
 * @brief Write data to the default CDC interface
 *
 * This function writes data to the transmit buffer of the default CDC interface
 * (interface 0). It transfers up to 'bufsize' bytes from the provided buffer
 * into the CDC transmit FIFO.
 *
 * @param[in] buffer   Pointer to the data to be written
 * @param[in] bufsize  Number of bytes to write
 *
 * @return uint32_t    The number of bytes actually written to the transmit buffer
 */
TU_ATTR_ALWAYS_INLINE static inline uint32_t tud_cdc_write(void const* buffer, uint32_t bufsize) {
  return tud_cdc_n_write(0, buffer, bufsize);
}

/**
 * @brief Write a null-terminated string to the default CDC interface
 *
 * This function writes a null-terminated string to the transmit buffer of the default CDC
 * interface (interface 0). It calls tud_cdc_n_write_str with interface ID 0.
 *
 * @param[in] str  Pointer to the null-terminated string to be written
 *
 * @return uint32_t  The number of bytes written to the transmit buffer
 */
TU_ATTR_ALWAYS_INLINE static inline uint32_t tud_cdc_write_str(char const* str) {
  return tud_cdc_n_write_str(0, str);
}

/**
 * @brief Flush the default CDC interface's transmit buffer
 *
 * This function attempts to send all buffered data in the transmit buffer of the 
 * default CDC interface (interface 0) to the host. It ensures that any pending 
 * data is actually transmitted.
 *
 * @return bool  true if successful, false otherwise
 */
TU_ATTR_ALWAYS_INLINE static inline uint32_t tud_cdc_write_flush(void) {
  return tud_cdc_n_write_flush(0);
}

/**
 * @brief Get the available space in the default CDC interface's transmit buffer
 *
 * This function returns the number of bytes that can be written to the transmit buffer
 * of the default CDC interface (interface 0) without overflowing.
 *
 * @return uint32_t  The number of bytes available in the transmit buffer
 */
TU_ATTR_ALWAYS_INLINE static inline uint32_t tud_cdc_write_available(void) {
  return tud_cdc_n_write_available(0);
}

/**
 * @brief Clear the default CDC interface's transmit buffer
 *
 * This function discards all data in the transmit buffer of the default CDC
 * interface (interface 0). Any pending data waiting to be sent will be lost.
 *
 * @return bool  true if successful, false otherwise
 */
TU_ATTR_ALWAYS_INLINE static inline bool tud_cdc_write_clear(void) {
  return tud_cdc_n_write_clear(0);
}

//--------------------------------------------------------------------+
// Application Callback API (weak is optional)
//--------------------------------------------------------------------+

/**
 * @brief Callback function invoked when CDC interface receives data from host
 *
 * This callback is called when new data has been received from the USB host on
 * the specified CDC interface. Application code should implement this function
 * to process the received data. The received data can be retrieved using the
 * tud_cdc_read() or tud_cdc_n_read() functions.
 *
 * @param[in] itf  CDC interface number on which data was received
 */
TU_ATTR_WEAK void tud_cdc_rx_cb(uint8_t itf);

/**
 * @brief Callback function invoked when a specific character is received
 *
 * This callback is triggered when the character previously set by tud_cdc_set_wanted_char()
 * is received on the specified CDC interface. This provides an efficient way to detect
 * special characters in the incoming data stream without having to manually scan
 * all received data.
 *
 * @param[in] itf          CDC interface number on which the character was received
 * @param[in] wanted_char  The specific character that was detected in the data stream
 */
TU_ATTR_WEAK void tud_cdc_rx_wanted_cb(uint8_t itf, char wanted_char);

/**
 * @brief Callback function invoked when a CDC transmit operation completes
 *
 * This callback is triggered when all pending data in the CDC transmit buffer
 * has been successfully sent to the USB host. It can be used to implement flow control
 * or to initiate the next data transmission.
 *
 * @param[in] itf  CDC interface number on which the transmission completed
 */
TU_ATTR_WEAK void tud_cdc_tx_complete_cb(uint8_t itf);

/**
 * @brief Callback function invoked when CDC line state changes
 *
 * This callback is triggered when the USB host changes the CDC line state,
 * specifically the DTR (Data Terminal Ready) and RTS (Request To Send) signals.
 * It can be used to detect when a terminal program connects or disconnects
 * from the CDC interface.
 *
 * @param[in] itf       CDC interface number
 * @param[in] dtr       True if Data Terminal Ready (DTR) is active
 * @param[in] rts       True if Request To Send (RTS) is active
 */
TU_ATTR_WEAK void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts);

/**
 * @brief Callback function invoked when CDC line coding parameters change
 *
 * This callback is triggered when the USB host changes the line coding parameters
 * such as baud rate, data bits, parity, and stop bits. It allows the device to
 * respond to these configuration changes.
 *
 * @param[in] itf         CDC interface number
 * @param[in] p_line_coding Pointer to the new line coding parameters structure (cdc_line_coding_t)
 */
TU_ATTR_WEAK void tud_cdc_line_coding_cb(uint8_t itf, cdc_line_coding_t const* p_line_coding);

/**
 * @brief Callback function invoked when a break signal is received
 *
 * This callback is triggered when the USB host sends a break signal
 * through the CDC interface. The duration parameter indicates the
 * length of the break signal in milliseconds.
 *
 * @param[in] itf       CDC interface number
 * @param[in] duration_ms  Duration of the break signal in milliseconds
 */
TU_ATTR_WEAK void tud_cdc_send_break_cb(uint8_t itf, uint16_t duration_ms);

//--------------------------------------------------------------------+
// INTERNAL USBD-CLASS DRIVER API
//--------------------------------------------------------------------+
/**
 * @brief Initialize CDC device driver
 *
 * This internal function initializes the CDC device driver data structures and buffers.
 * It's called during the USB device stack initialization process and should not be
 * called directly by applications.
 * 
 * The function:
 * - Clears CDC interface structures
 * - Sets default line coding parameters (115200 baud, 8N1)
 * - Configures RX and TX FIFOs with appropriate buffer sizes
 * - Sets up mutex protection for the FIFOs if OSAL_MUTEX_REQUIRED is enabled
 */
void     cdcd_init            (void);

/**
 * @brief Deinitialize CDC device driver
 *
 * This internal function deinitializes the CDC device driver, releasing any resources
 * that were allocated during initialization. It's called during the USB device stack
 * deinitialization process and should not be called directly by applications.
 *
 * The function:
 * - Releases any allocated buffers
 * - Clears CDC interface state information
 * - Destroys mutex protection for the FIFOs if OSAL_MUTEX_REQUIRED is enabled
 *
 * @return bool  true if deinitialization was successful, false otherwise
 */
bool     cdcd_deinit          (void);

/**
 * @brief Reset CDC device driver
 *
 * This internal function resets the CDC device driver to its initial state.
 * It's called when a USB bus reset occurs or when the device is disconnected.
 * The function resets all CDC interfaces to their default states without
 * deallocating resources.
 *
 * @param[in] rhport  USB root hub port where the device is connected
 */
void     cdcd_reset           (uint8_t rhport);

/**
 * @brief Open CDC device interface
 *
 * This internal function is called when a CDC interface is opened by the USB host.
 * It prepares the specified CDC interface for operation by initializing its state
 * and preparing the data structures needed for communication.
 *
 * @param[in] rhport   USB root hub port where the device is connected
 * @param[in] itf_desc Interface descriptor
 * @param[in] max_len  Maximum length of the descriptor that can be processed
 *
 * @return uint16_t    Number of bytes processed from the descriptor, or 0 if the descriptor cannot be processed
 */
uint16_t cdcd_open            (uint8_t rhport, tusb_desc_interface_t const * itf_desc, uint16_t max_len);

/**
 * @brief CDC device control transfer callback
 *
 * This internal function handles USB control transfers specific to CDC interfaces.
 * It processes CDC-specific requests such as SET_LINE_CODING, GET_LINE_CODING,
 * SET_CONTROL_LINE_STATE, and SEND_BREAK.
 *
 * @param[in]     rhport   USB root hub port where the device is connected
 * @param[in]     stage    Current stage of the control transfer (setup, data, ack)
 * @param[in,out] request  USB setup packet containing the control request
 *
 * @return bool  true if the control request was handled, false otherwise
 */
bool     cdcd_control_xfer_cb (uint8_t rhport, uint8_t stage, tusb_control_request_t const * request);

/**
 * @brief CDC device data transfer callback
 *
 * This internal function is called when a data transfer on a CDC endpoint
 * completes. It handles both IN and OUT endpoint transfers, updating the
 * appropriate FIFOs and invoking user callbacks when necessary.
 *
 * @param[in] rhport          USB root hub port where the device is connected
 * @param[in] ep_addr         Endpoint address (including direction bit)
 * @param[in] result          Result of the transfer operation
 * @param[in] xferred_bytes   Number of bytes transferred
 *
 * @return bool  true if the transfer was handled successfully, false otherwise
 */
bool     cdcd_xfer_cb         (uint8_t rhport, uint8_t ep_addr, xfer_result_t result, uint32_t xferred_bytes);

#ifdef __cplusplus
 }
#endif

#endif /* _TUSB_CDC_DEVICE_H_ */

/** @}*/ /* End of CDC_DEVICE */