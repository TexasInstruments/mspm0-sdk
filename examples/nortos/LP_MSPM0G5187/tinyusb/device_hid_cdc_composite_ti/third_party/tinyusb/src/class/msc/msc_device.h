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
 *  @file       msc_device.h
 *  @brief      MSC (Mass Storage Class) Device Driver implementation
 * 
 *   @anchor tinyusb_class_msc_device 
 *   # Overview
 * 
 *   This file implements the MSC device driver for TinyUSB stack.
 *   It handles the USB MSC protocol including:
 *   - Device enumeration and configuration
 *   - SCSI command processing (read, write, inquiry, etc.)
 *   - Block data transfer between USB host and device storage
 *   - Multiple Logical Unit Number (LUN) support
 *
 *   The MSC implementation supports:
 *   - Multiple storage interfaces
 *   - Configurable buffer sizes via CFG_TUD_MSC_EP_BUFSIZE
 *   - Read-only or read-write storage options
 *   - Standard SCSI commands and custom command handling
 *   - Ejection and unit ready status reporting
 *
 *   <hr>
 ******************************************************************************/
/** @addtogroup MSC_DEVICE
 * @{
 */

#ifndef _TUSB_MSC_DEVICE_H_
#define _TUSB_MSC_DEVICE_H_

#include "common/tusb_common.h"
#include "msc.h"

#ifdef __cplusplus
 extern "C" {
#endif

//--------------------------------------------------------------------+
// Class Driver Configuration
//--------------------------------------------------------------------+

#if !defined(CFG_TUD_MSC_EP_BUFSIZE) & defined(CFG_TUD_MSC_BUFSIZE)
  // TODO warn user to use new name later on
  // #warning CFG_TUD_MSC_BUFSIZE is renamed to CFG_TUD_MSC_EP_BUFSIZE, please update to use the new name
  #define CFG_TUD_MSC_EP_BUFSIZE  CFG_TUD_MSC_BUFSIZE
#endif

#ifndef CFG_TUD_MSC_EP_BUFSIZE
  #error CFG_TUD_MSC_EP_BUFSIZE must be defined, value of a block size should work well, the more the better
#endif

/*!
 * @brief Check to confirm MSC EP buffer size is smaller than UINT16_MAX
 */
TU_VERIFY_STATIC(CFG_TUD_MSC_EP_BUFSIZE < UINT16_MAX, "Size is not correct");

//--------------------------------------------------------------------+
// Application API
//--------------------------------------------------------------------+

/**
 * @brief Set SCSI sense data for error reporting
 * 
 * This function sets the SCSI sense data that will be returned to the host
 * when a REQUEST SENSE command is received. It should be called by the application
 * when an error occurs during command processing to provide detailed error information.
 *
 * @param[in] lun                  Logical Unit Number
 * @param[in] sense_key            SCSI sense key (e.g., SCSI_SENSE_NOT_READY, SCSI_SENSE_ILLEGAL_REQUEST)
 * @param[in] add_sense_code       Additional sense code providing more specific error information
 * @param[in] add_sense_qualifier  Additional sense code qualifier for even more specific error information
 *
 * @return                         true (always succeeds)
 */
bool tud_msc_set_sense(uint8_t lun, uint8_t sense_key, uint8_t add_sense_code, uint8_t add_sense_qualifier);

//--------------------------------------------------------------------+
// Application Callbacks (WEAK is optional)
//--------------------------------------------------------------------+

/**
 * @brief Callback function for handling SCSI READ10 commands
 * 
 * This function is invoked when a SCSI READ10 command is received from the host.
 * The application should fill the provided buffer with data from the specified
 * address (LBA + offset) and return the number of bytes read.
 * 
 * The address is calculated as: lba * BLOCK_SIZE + offset
 * Note that offset is only needed if CFG_TUD_MSC_EP_BUFSIZE is smaller than BLOCK_SIZE.
 * 
 * @param[in]  lun      Logical Unit Number
 * @param[in]  lba      Logical Block Address to read from
 * @param[in]  offset   Byte offset into the block
 * @param[out] buffer   Buffer to fill with read data
 * @param[in]  bufsize  Size of the buffer in bytes
 * 
 * @return              Number of bytes read into the buffer (positive),
 *                      0 if application is not ready (callback will be invoked again),
 *                      negative value if error occurred (request will be stalled)
 */
int32_t tud_msc_read10_cb (uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize);

/**
 * @brief Callback function for handling SCSI WRITE10 commands
 * 
 * This function is invoked when a SCSI WRITE10 command is received from the host.
 * The application should write the data from the provided buffer to the specified
 * address (LBA + offset) and return the number of bytes written.
 * 
 * The address is calculated as: lba * BLOCK_SIZE + offset
 * Note that offset is only needed if CFG_TUD_MSC_EP_BUFSIZE is smaller than BLOCK_SIZE.
 * 
 * @param[in] lun       Logical Unit Number
 * @param[in] lba       Logical Block Address to write to
 * @param[in] offset    Byte offset into the block
 * @param[in] buffer    Buffer containing data to write
 * @param[in] bufsize   Size of the buffer in bytes
 * 
 * @return              Number of bytes written (positive),
 *                      0 if application is not ready (callback will be invoked again),
 *                      negative value if error occurred (request will be STALLed)
 */
int32_t tud_msc_write10_cb (uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize);

/**
 * @brief Callback function for handling SCSI INQUIRY commands
 * 
 * This function is invoked when a SCSI INQUIRY command is received from the host.
 * The application should fill the vendor ID, product ID, and product revision 
 * arrays with appropriate identification strings.
 * 
 * @param[in]  lun          Logical Unit Number
 * @param[out] vendor_id    Buffer to fill with vendor identification (up to 8 characters)
 * @param[out] product_id   Buffer to fill with product identification (up to 16 characters)
 * @param[out] product_rev  Buffer to fill with product revision (up to 4 characters)
 */
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]);

/**
 * @brief Callback function for handling SCSI TEST UNIT READY commands
 *
 * This function is invoked when a SCSI TEST UNIT READY command is received from the host.
 * The application should check if the device is ready to accept read/write commands.
 *
 * @param[in] lun           Logical Unit Number
 *
 * @return true             If the device is ready
 * @return false            If the device is not ready (will lead to a SCSI sense with "Not Ready")
 */
bool tud_msc_test_unit_ready_cb(uint8_t lun);

/**
 * @brief Callback function for determining disk capacity
 *
 * This function is invoked when a SCSI_CMD_READ_CAPACITY_10 or SCSI_CMD_READ_FORMAT_CAPACITY
 * command is received from the host. The application should update the block count and 
 * block size parameters to reflect the storage device's capacity.
 *
 * For example, in an SD card implementation, this would return the total number of blocks
 * and the block size (typically 512 bytes) of the SD card.
 *
 * @param[in]  lun          Logical Unit Number
 * @param[out] block_count  Pointer to store the total number of addressable blocks
 * @param[out] block_size   Pointer to store the size of each block in bytes
 */
void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size);

/**
 *  @brief Invoked when received an SCSI command not in built-in list below.
 *
 * - READ_CAPACITY10, READ_FORMAT_CAPACITY, INQUIRY, TEST_UNIT_READY, START_STOP_UNIT, MODE_SENSE6, REQUEST_SENSE
 * - READ10 and WRITE10 has their own callbacks
 *
 * @param[in]   lun         Logical unit number
 * @param[in]   scsi_cmd    SCSI command contents which application must examine to response accordingly
 * @param[out]  buffer      Buffer for SCSI Data Stage.
 *                            - For INPUT: application must fill this with response.
 *                            - For OUTPUT it holds the Data from host
 * @param[in]   bufsize     Buffer's length.
 *
 * @return      Actual bytes processed, can be zero for no-data command.
 * @return      negative    Indicate error e.g unsupported command, tinyusb will \b STALL the corresponding
 *                          endpoint and return failed status in command status wrapper phase.
 */
int32_t tud_msc_scsi_cb (uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize);

/*------------- Optional callbacks -------------*/

/**
 * @brief Callback function to get the maximum logical unit number (LUN)
 *
 * This function is invoked during MSC enumeration to determine the number of
 * logical units supported by the device. For example, a device with multiple
 * storage partitions or multiple physical storage devices (like SD cards) might
 * expose each as a separate LUN.
 *
 * Most implementations with a single storage device will return 0.
 *
 * @return The maximum LUN value (number of LUNs - 1)
 */
TU_ATTR_WEAK uint8_t tud_msc_get_maxlun_cb(void);

/**
 * @brief Callback function for handling SCSI START STOP UNIT command
 *
 * This function is invoked when a SCSI START STOP UNIT command is received from the host.
 * The command is used to spin up/down the media or eject/load the media.
 *
 * In an SD card implementation, this could be used to safely unmount the file system
 * when the host requests media ejection, preventing data corruption.
 *
 * @param[in] lun           Logical Unit Number
 * @param[in] power_condition   Power condition field from the command
 * @param[in] start         Start bit (1 = start, 0 = stop)
 * @param[in] load_eject    Load/Eject operation (1 = load, 0 = eject)
 *
 * @return true             If the command is handled successfully
 * @return false            If the command failed
 */
TU_ATTR_WEAK bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject);

//Invoked when we receive the Prevent / Allow Medium Removal command
/**
 * @brief Callback function for handling SCSI PREVENT ALLOW MEDIUM REMOVAL command
 *
 * This function is invoked when a SCSI PREVENT ALLOW MEDIUM REMOVAL command is received
 * from the host. The command is used to mechanically lock/unlock the storage media to
 * prevent or allow its removal.
 *
 * In an SD card implementation, this could be used to indicate to the application whether
 * it should allow the user to physically remove the SD card. Some applications might use
 * an LED indicator or other mechanism to signal when it's safe to remove the media.
 *
 * @param[in] lun                Logical Unit Number
 * @param[in] prohibit_removal   Prohibit removal flag (1 = prevent removal, 0 = allow removal)
 * @param[in] control            Control field from the SCSI command
 *
 * @return true             If the command is handled successfully
 * @return false            If the command failed
 */
TU_ATTR_WEAK bool tud_msc_prevent_allow_medium_removal_cb(uint8_t lun, uint8_t prohibit_removal, uint8_t control);

/**
 * @brief Callback function for handling SCSI REQUEST SENSE command
 *
 * This function is invoked when a SCSI REQUEST SENSE command is received from the host.
 * It allows the application to provide detailed error information when a previous
 * command has failed. The application should fill the provided buffer with
 * appropriate sense data.
 *
 * In an SD card implementation, this could provide specific information about
 * card access errors, write protection status, or other media-specific issues.
 *
 * @param[in] lun        Logical Unit Number
 * @param[out] buffer    Buffer to fill with sense data
 * @param[in] bufsize    Maximum size of the buffer
 *
 * @return Number of bytes written to the buffer (positive), or negative value
 *         to indicate error
 */
TU_ATTR_WEAK int32_t tud_msc_request_sense_cb(uint8_t lun, void* buffer, uint16_t bufsize);

/**
 * @brief Callback function invoked when a SCSI READ10 command is complete
 *
 * This function is called after a READ10 operation has completed successfully.
 * It can be used to perform post-read operations such as updating internal state,
 * releasing resources, or performing additional processing after data has been 
 * transferred to the host.
 *
 * In an SD card implementation, this could be used to update access statistics,
 * manage power states after read operations, or prepare for subsequent operations.
 *
 * @param[in] lun        Logical Unit Number that completed the read operation
 */
TU_ATTR_WEAK void tud_msc_read10_complete_cb(uint8_t lun);

/**
 * @brief Callback function invoked when a SCSI WRITE10 command is complete
 *
 * This function is called after a WRITE10 operation has completed successfully.
 * It can be used to perform post-write operations such as flushing cached data,
 * updating internal state, or performing additional processing after data has
 * been received from the host.
 *
 * In an SD card or flash memory implementation, this is particularly useful for
 * flushing write caches, completing deferred writes, or updating file system
 * structures after data has been written to storage.
 *
 * @param[in] lun        Logical Unit Number that completed the write operation
 */
TU_ATTR_WEAK void tud_msc_write10_complete_cb(uint8_t lun);

/**
 * @brief Callback function invoked when a SCSI command has completed processing
 *
 * This function is called after a SCSI command processed in tud_msc_scsi_cb() has completed
 * execution. It provides an opportunity to perform post-command operations such as resource
 * cleanup, state updates, or additional processing based on the completed command.
 *
 * Unlike the read10_complete_cb and write10_complete_cb callbacks which are specific to
 * those operations, this callback handles completion of all other SCSI commands that were
 * processed through the tud_msc_scsi_cb() function.
 *
 * This can be useful for:
 * - Tracking command history
 * - Performing cleanup after complex commands
 * - Implementing state machines that depend on command completion
 * - Logging or monitoring SCSI command execution
 *
 * @param[in] lun        Logical Unit Number that the command was directed to
 * @param[in] scsi_cmd   The 16-byte SCSI command that was completed
 */
TU_ATTR_WEAK void tud_msc_scsi_complete_cb(uint8_t lun, uint8_t const scsi_cmd[16]);

/**
 * @brief Callback function to determine if the storage device is writable
 *
 * This function is invoked as part of processing SCSI WRITE10 commands to check
 * if the storage device is currently in a writable state. The application should
 * return whether the specified logical unit accepts write operations.
 *
 * This can be used to implement:
 * - Write protection mechanisms
 * - Read-only mode for certain operations
 * - Dynamic write permission based on device state
 * - Write protection for specific logical units
 *
 * If this function returns false, the MSC device will respond to WRITE10 commands
 * with a "Write Protected" sense key, informing the host that writing is not allowed.
 *
 * @param[in] lun        Logical Unit Number to check for write permission
 *
 * @return true          If the specified logical unit accepts write operations
 * @return false         If the specified logical unit is write-protected
 */
TU_ATTR_WEAK bool tud_msc_is_writable_cb(uint8_t lun);

//--------------------------------------------------------------------+
// Internal Class Driver API
//--------------------------------------------------------------------+

/**
 * @brief Initialize the Mass Storage Class Device driver
 *
 * This function initializes the MSC device driver's internal state. It is called during
 * the TinyUSB device stack initialization process. The function clears the MSC interface
 * structure (_mscd_itf) to set all values to their default state.
 *
 * This function is part of the internal class driver API and should not be called directly
 * by applications. It is automatically invoked by the TinyUSB device stack during
 * initialization.
 */
void     mscd_init            (void);

/**
 * @brief Deinitialize the Mass Storage Class Device driver
 *
 * This function deinitializes the MSC device driver, releasing any resources
 * and returning the driver to an uninitialized state. It's called during the
 * TinyUSB device stack deinitialization process.
 * 
 * Currently, the implementation simply returns true as there are no specific
 * resources to release for the MSC device class.
 *
 * This function is part of the internal class driver API and should not be called
 * directly by applications. It is automatically invoked by the TinyUSB device stack
 * during deinitialization.
 *
 * @return bool          Always returns true indicating successful deinitialization
 */
bool     mscd_deinit          (void);

/**
 * @brief Reset the Mass Storage Class Device driver state
 *
 * This function resets the MSC device driver to its initial state. It is called when
 * a USB reset event occurs or when the host sends a Bulk-Only Mass Storage Reset
 * class-specific request. The function clears the MSC interface structure (_mscd_itf)
 * to set all values to their default state.
 *
 * According to the USB Mass Storage Class specification, this reset operation prepares
 * the device for the next Command Block Wrapper (CBW) from the host.
 *
 * This function is part of the internal class driver API and should not be called directly
 * by applications. It is automatically invoked by the TinyUSB device stack during
 * reset events.
 *
 * @param[in] rhport     Root Hub Port number the device is connected to
 */
void     mscd_reset           (uint8_t rhport);

/**
 * @brief Open a Mass Storage Class interface
 *
 * This function initializes an MSC interface when the host configures the device.
 * It validates that the interface descriptor is for a Mass Storage Class with SCSI
 * subclass using the BOT (Bulk-Only Transport) protocol. If valid, it initializes
 * the interface structure, opens the required bulk IN and OUT endpoints, and
 * prepares for the first Command Block Wrapper (CBW).
 *
 * This function is part of the internal class driver API and is automatically
 * invoked by the TinyUSB device stack during the enumeration process. It should
 * not be called directly by applications.
 *
 * @param[in] rhport      Root Hub Port number the device is connected to
 * @param[in] itf_desc    Pointer to the interface descriptor
 * @param[in] max_len     Maximum length of the descriptor set
 *
 * @return Length of the descriptor set processed, or 0 if the interface is
 *         not supported or an error occurred
 */
uint16_t mscd_open            (uint8_t rhport, tusb_desc_interface_t const * itf_desc, uint16_t max_len);

/**
 * @brief Process control transfers for Mass Storage Class
 *
 * This function handles control transfers directed to the MSC interface.
 * It processes class-specific requests such as MSC_REQ_RESET (Bulk-Only Mass Storage Reset)
 * and standard requests like CLEAR_FEATURE for endpoint stall conditions during error recovery.
 * 
 * When a stall condition is cleared, this function will:
 * - Continue to stall if the device is in NEED_RESET state
 * - Resume sending CSW if previously in STATUS stage
 * - Prepare for new CBW reception if in CMD stage
 *
 * The function is called by the USB device stack during different stages of a control transfer
 * (setup, data, acknowledgment) and must handle each stage appropriately.
 *
 * @param[in] rhport      Root Hub Port number the device is connected to
 * @param[in] stage       The current stage of the control transfer (setup, data, ack)
 * @param[in] p_request   Pointer to the setup packet containing the control request
 *
 * @return bool           true if the request is handled, false to stall the control endpoint
 */
bool     mscd_control_xfer_cb (uint8_t rhport, uint8_t stage, tusb_control_request_t const * p_request);

/**
 * @brief Transfer callback for Mass Storage Class device
 *
 * This function handles USB transfer completion events for the MSC interface.
 * It processes different stages of the Bulk-Only Transport (BOT) protocol:
 * - Command stage: Processes received Command Block Wrapper (CBW)
 * - Data stage: Handles data transfers (both IN and OUT)
 * - Status stage: Manages Command Status Wrapper (CSW) transmission
 *
 * The function implements the state machine for BOT protocol, handling transitions
 * between states and managing error conditions such as stalls and resets.
 *
 * @param[in] rhport          Root Hub Port number the device is connected to
 * @param[in] ep_addr         Endpoint address that generated the transfer event
 * @param[in] event           Transfer result (success, failed, stalled)
 * @param[in] xferred_bytes   Number of bytes transferred
 *
 * @return true               If the event is handled successfully
 * @return false              If the event is not recognized or cannot be handled
 */
bool     mscd_xfer_cb         (uint8_t rhport, uint8_t ep_addr, xfer_result_t event, uint32_t xferred_bytes);

#ifdef __cplusplus
 }
#endif

#endif /* _TUSB_MSC_DEVICE_H_ */

/** @}*/ /* End of MSC_DEVICE */