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
 *  @file       msc_host.h
 *  @brief      MSC (Mass Storage Class) Host Driver implementation
 * 
 *   @anchor tinyusb_class_msc_host 
 *   # Overview
 * 
 *   This file implements the MSC host driver for TinyUSB stack.
 *   It handles the USB MSC protocol including:
 *   - Device enumeration and configuration
 *   - SCSI command processing (read, write, inquiry, etc.)
 *   - Block data transfer between USB host and device storage
 *   - Multiple Logical Unit Number (LUN) support
 *
 *   The MSC implementation supports:
 *   - Multiple storage interfaces
 *   - Configurable maximum LUN via CFG_TUH_MSC_MAXLUN
 *   - Standard SCSI commands and responses
 *   - Block read/write operations
 *
 *   <hr>
 ******************************************************************************/
/** @addtogroup MSC_HOST
 * @{
 */

#ifndef TUSB_MSC_HOST_H_
#define TUSB_MSC_HOST_H_

#include "msc.h"

#ifdef __cplusplus
 extern "C" {
#endif

//--------------------------------------------------------------------+
// Class Driver Configuration
//--------------------------------------------------------------------+

/**
 * @brief Maximum number of logical unit number (LUN) supported
 */
#ifndef CFG_TUH_MSC_MAXLUN
#define CFG_TUH_MSC_MAXLUN  4
#endif

/**
 * @brief MSC SCSI operation completion data structure
 * 
 * This structure contains all relevant data from a completed SCSI operation,
 * including the original command, status response, and any transferred data.
 * It is passed to the callback function when a SCSI operation completes.
 */
typedef struct {
  /*! SCSI command */
  msc_cbw_t const* cbw;
  /*! SCSI status */
  msc_csw_t const* csw;
  /*! SCSI data */
  void* scsi_data;
  /*! User argument */
  uintptr_t user_arg;
}tuh_msc_complete_data_t;

/**
 * @brief MSC SCSI operation completion callback
 */
typedef bool (*tuh_msc_complete_cb_t)(uint8_t dev_addr, tuh_msc_complete_data_t const* cb_data);

//--------------------------------------------------------------------+
// Application API
//--------------------------------------------------------------------+

/**
 * @brief Check if a device supports Mass Storage Class interface
 * 
 * This function checks if the specified device has a mounted MSC interface.
 * It returns true after the device has been successfully mounted (after 
 * tuh_msc_mounted_cb() is invoked) and returns false after the device has
 * been unmounted (after tuh_msc_unmounted_cb() is invoked).
 *
 * @param[in] dev_addr  Device address
 *
 * @return true         The device has a mounted MSC interface
 * @return false        The device does not have a mounted MSC interface
 */
bool tuh_msc_mounted(uint8_t dev_addr);

/**
 * @brief Check if a MSC device is ready for communication
 * 
 * This function checks if the specified MSC device is ready for new commands.
 * It verifies that the device is mounted and that no transfers are currently
 * in progress on either the IN or OUT endpoints.
 *
 * @param[in] dev_addr  Device address
 *
 * @return true         The device is mounted and not busy with any transfers
 * @return false        The device is either not mounted or busy with transfers
 *
 * @note This function should be called before initiating any new SCSI commands
 *       to ensure the device is in a state where it can accept them
 */
bool tuh_msc_ready(uint8_t dev_addr);

/**
 * @brief Get the maximum Logical Unit Number (LUN) of a MSC device
 * 
 * This function returns the maximum LUN value for the specified MSC device.
 * The LUN value indicates how many independent storage units are available
 * on the device. For example, a multi-card reader may expose each card slot
 * as a separate LUN.
 *
 * @param[in] dev_addr  Device address
 *
 * @return Maximum LUN value
 */
uint8_t tuh_msc_get_maxlun(uint8_t dev_addr);

/**
 * @brief Get the total number of blocks in a logical unit
 * 
 * This function returns the total number of addressable blocks for the
 * specified logical unit on the MSC device. This value is obtained during
 * device enumeration through a SCSI Read Capacity command.
 *
 * @param[in] dev_addr  Device address
 * @param[in] lun       Logical Unit Number
 *
 * @return Total number of addressable blocks
 */
uint32_t tuh_msc_get_block_count(uint8_t dev_addr, uint8_t lun);

/**
 * @brief Get the block size in bytes for a logical unit
 * 
 * This function returns the size in bytes of each addressable block for the
 * specified logical unit on the MSC device. This value is obtained during
 * device enumeration through a SCSI Read Capacity command.
 *
 * @param[in] dev_addr  Device address
 * @param[in] lun       Logical Unit Number
 *
 * @return Block size in bytes, Returns 0 if the device is not mounted or if the
 *         specified LUN is invalid
 */
uint32_t tuh_msc_get_block_size(uint8_t dev_addr, uint8_t lun);

/**
 * @brief Send a custom SCSI command to an MSC device
 * 
 * This function sends a custom SCSI command to the specified MSC device.
 * It provides low-level access to send any SCSI command defined in the 
 * SCSI specification or vendor-specific commands.
 *
 * @param[in]  daddr        Device address
 * @param[in]  cbw          Pointer to Command Block Wrapper (CBW) structure
 *                          Must contain a properly formatted SCSI command
 * @param[in,out] data      Pointer to data buffer for command
 *                          - For IN transfers: receives data from device
 *                          - For OUT transfers: contains data to send to device
 *                          - Can be NULL for commands with no data phase
 * @param[in]  complete_cb  Callback function to be invoked when the command completes
 * @param[in]  arg          User argument to be passed to the callback function
 *
 * @return True if the command was successfully submitted
 * @return False if the device is not configured or cannot accept commands
 */
bool tuh_msc_scsi_command(uint8_t daddr, msc_cbw_t const* cbw, void* data, tuh_msc_complete_cb_t complete_cb, uintptr_t arg);

/**
 * @brief Perform a SCSI INQUIRY command on an MSC device
 * 
 * This function sends a SCSI INQUIRY command to the specified logical unit
 * of an MSC device. The INQUIRY command retrieves basic information about
 * the device such as vendor ID, product ID, and revision level.
 *
 * @param[in]  dev_addr     Device address
 * @param[in]  lun          Logical Unit Number
 * @param[out] response     Pointer to buffer where the inquiry response will be stored
 *                          Must be at least sizeof(scsi_inquiry_resp_t) bytes
 * @param[in]  complete_cb  Callback function to be invoked when the command completes
 * @param[in]  arg          User argument to be passed to the callback function
 *
 * @return True if the command was successfully submitted
 * @return False if the device is not mounted or busy
 */
bool tuh_msc_inquiry(uint8_t dev_addr, uint8_t lun, scsi_inquiry_resp_t* response, tuh_msc_complete_cb_t complete_cb, uintptr_t arg);

/**
 * @brief Perform a SCSI TEST UNIT READY command on an MSC device
 * 
 * This function sends a SCSI TEST UNIT READY command to check if the
 * specified logical unit is ready to accept commands. This is commonly
 * used to check if a device is initialized and operational.
 *
 * @param[in] dev_addr     Device address
 * @param[in] lun          Logical Unit Number
 * @param[in] complete_cb  Callback function to be invoked when the command completes
 * @param[in] arg          User argument to be passed to the callback function
 *
 * @return True if the command was successfully submitted
 * @return False if the device is not mounted or busy
 */
bool tuh_msc_test_unit_ready(uint8_t dev_addr, uint8_t lun, tuh_msc_complete_cb_t complete_cb, uintptr_t arg);

/**
 * @brief Perform a SCSI REQUEST SENSE command on an MSC device
 * 
 * This function sends a SCSI REQUEST SENSE command to retrieve error information
 * from the specified logical unit of an MSC device. It is typically called after
 * a command fails to get detailed error information about why the command failed.
 *
 * @param[in]  dev_addr     Device address
 * @param[in]  lun          Logical Unit Number
 * @param[out] response     Pointer to buffer where the sense data will be stored
 *                          Must be accessible by USB/DMA controller, aligned correctly
 *                          and multiple of cache line if enabled
 * @param[in]  complete_cb  Callback function to be invoked when the command completes
 * @param[in]  arg          User argument to be passed to the callback function
 *
 * @return True if the command was successfully submitted
 * @return False if the device is not mounted or busy
 */
bool tuh_msc_request_sense(uint8_t dev_addr, uint8_t lun, void *response, tuh_msc_complete_cb_t complete_cb, uintptr_t arg);

/**
 * @brief Perform a SCSI READ10 command on an MSC device
 * 
 * This function sends a SCSI READ10 command to read data blocks from the
 * specified logical unit of an MSC device. It reads the specified number
 * of blocks starting from the given Logical Block Address (LBA) into the
 * provided buffer.
 *
 * @param[in]  dev_addr     Device address
 * @param[in]  lun          Logical Unit Number
 * @param[out] buffer       Pointer to buffer where the read data will be stored
 *                          Must be accessible by USB/DMA controller, aligned correctly
 *                          and multiple of cache line if enabled
 * @param[in]  lba          Logical Block Address to start reading from
 * @param[in]  block_count  Number of blocks to read
 * @param[in]  complete_cb  Callback function to be invoked when the command completes
 * @param[in]  arg          User argument to be passed to the callback function
 *
 * @return True if the command was successfully submitted
 * @return False if the device is not mounted or busy
 */
bool tuh_msc_read10(uint8_t dev_addr, uint8_t lun, void * buffer, uint32_t lba, uint16_t block_count, tuh_msc_complete_cb_t complete_cb, uintptr_t arg);

/**
 * @brief Perform a SCSI WRITE10 command on an MSC device
 * 
 * This function sends a SCSI WRITE10 command to write data blocks to the
 * specified logical unit of an MSC device. It writes the specified number
 * of blocks starting at the given Logical Block Address (LBA) from the
 * provided buffer.
 *
 * @param[in]  dev_addr     Device address
 * @param[in]  lun          Logical Unit Number
 * @param[in]  buffer       Pointer to buffer containing the data to be written
 *                          Must be accessible by USB/DMA controller, aligned correctly
 *                          and multiple of cache line if enabled
 * @param[in]  lba          Logical Block Address to start writing to
 * @param[in]  block_count  Number of blocks to write
 * @param[in]  complete_cb  Callback function to be invoked when the command completes
 * @param[in]  arg          User argument to be passed to the callback function
 *
 * @return True if the command was successfully submitted
 * @return False if the device is not mounted or busy
 */
bool tuh_msc_write10(uint8_t dev_addr, uint8_t lun, void const * buffer, uint32_t lba, uint16_t block_count, tuh_msc_complete_cb_t complete_cb, uintptr_t arg);

/**
 * @brief Perform a SCSI READ CAPACITY 10 command on an MSC device
 * 
 * This function sends a SCSI READ CAPACITY 10 command to retrieve capacity information
 * from the specified logical unit of an MSC device. The capacity information includes
 * the total number of blocks and the size of each block.
 * 
 * @note During enumeration, the host stack already performs this request automatically.
 *       Applications can simply call tuh_msc_get_block_count() and tuh_msc_get_block_size()
 *       to retrieve the capacity information without executing this command again.
 *
 * @param[in]  dev_addr     Device address
 * @param[in]  lun          Logical Unit Number
 * @param[out] response     Pointer to buffer where capacity data will be stored
 * @param[in]  complete_cb  Callback function to be invoked when the command completes
 * @param[in]  arg          User argument to be passed to the callback function
 *
 * @return True if the command was successfully submitted
 * @return False if the device is not mounted or busy
 */
bool tuh_msc_read_capacity(uint8_t dev_addr, uint8_t lun, scsi_read_capacity10_resp_t* response, tuh_msc_complete_cb_t complete_cb, uintptr_t arg);

//------------- Application Callback -------------//

/**
 * @brief Callback when a Mass Storage Class device is mounted
 * 
 * This callback function is invoked when a device with a Mass Storage interface
 * is successfully enumerated and ready to use. It's marked as weak so that applications
 * can override it to implement their own mount handling.
 *
 * @param[in] dev_addr  Device address of the newly mounted MSC device
 */
TU_ATTR_WEAK void tuh_msc_mount_cb(uint8_t dev_addr);

/**
 * @brief Callback when a Mass Storage Class device is unmounted
 * 
 * This callback function is invoked when a previously mounted MSC device
 * is disconnected from the host or otherwise becomes unavailable. It's marked
 * as weak so that applications can override it to implement their own unmount handling.
 *
 * @param[in] dev_addr  Device address of the MSC device that was unmounted
 */
TU_ATTR_WEAK void tuh_msc_umount_cb(uint8_t dev_addr);

//--------------------------------------------------------------------+
// Internal Class Driver API
//--------------------------------------------------------------------+

/**
 * @brief Initialize the Mass Storage Class Host driver
 * 
 * This function initializes the MSC host driver's internal state. It is called during
 * the TinyUSB host stack initialization process. The function clears the MSC interface
 * structures (_msch_itf) to set all values to their default state.
 *
 * This function is part of the internal class driver API and should not be called directly
 * by applications. It is automatically invoked by the TinyUSB host stack during
 * initialization.
 *
 * @return True if initialization is successful
 */
bool msch_init       (void);

/**
 * @brief De-initialize the Mass Storage Class Host driver
 * 
 * This function de-initializes the MSC host driver, releasing any resources
 * that were allocated during initialization. It is called when the TinyUSB host
 * stack is being shut down.
 * 
 * This function is part of the internal class driver API and should not be called directly
 * by applications. It is automatically invoked by the TinyUSB host stack during
 * the shutdown process.
 *
 * @return True if de-initialization is successful
 */
bool msch_deinit     (void);

/**
 * @brief Open a Mass Storage Class interface
 * 
 * This function initializes an MSC interface when a compatible interface is found
 * during the enumeration process. It verifies that the interface is a Mass Storage Class
 * interface with the correct subclass and protocol, and then configures the interface
 * by initializing the interface structure and opening the required endpoints.
 *
 * This function is part of the internal class driver API and should not be called directly
 * by applications. It is automatically invoked by the TinyUSB host stack during
 * the enumeration process.
 *
 * @param[in] rhport      Root Hub Port number where the device is connected
 * @param[in] dev_addr    Device address assigned by the host
 * @param[in] desc_itf    Pointer to the interface descriptor
 * @param[in] max_len     Maximum length of the descriptor set
 *
 * @return True if the interface is successfully opened
 * @return False if the interface is not supported or an error occurred
 */
bool msch_open       (uint8_t rhport, uint8_t dev_addr, tusb_desc_interface_t const *desc_itf, uint16_t max_len);

/**
 * @brief Configure a Mass Storage Class interface
 * 
 * This function configures an MSC interface after the device has been addressed 
 * and the interface has been successfully opened. It marks the interface as 
 * configured and initiates the MSC enumeration process by sending a GET_MAX_LUN 
 * control request to determine the number of logical units supported by the device.
 *
 * This function is part of the internal class driver API and should not be called directly
 * by applications. It is automatically invoked by the TinyUSB host stack during
 * the enumeration process.
 *
 * @param[in] daddr     Device address
 * @param[in] itf_num   Interface number
 *
 * @return True if the configuration process was successfully initiated
 * @return False if an error occurred during configuration
 */
bool msch_set_config (uint8_t daddr, uint8_t itf_num);

/**
 * @brief Close a Mass Storage Class interface
 * 
 * This function is called when a Mass Storage device is disconnected or needs to be 
 * deinitialized. It cleans up resources associated with the MSC interface and 
 * invokes the application callback to notify of the device unmount.
 *
 * This function is part of the internal class driver API and should not be called directly
 * by applications. It is automatically invoked by the TinyUSB host stack when
 * a device is disconnected or the host is shutting down.
 *
 * @param[in] dev_addr     Device address of the MSC device to close
 */
void msch_close      (uint8_t dev_addr);

/**
 * @brief Transfer callback for Mass Storage Class interface
 * 
 * This function is called when a USB transfer completes on an MSC interface endpoint.
 * It handles the state machine for the Bulk-Only Transport (BOT) protocol, processing
 * the different stages of a SCSI command: Command, Data, and Status.
 *
 * This function is part of the internal class driver API and should not be called directly
 * by applications. It is automatically invoked by the TinyUSB host stack when
 * a transfer completes on an MSC endpoint.
 *
 * @param[in] dev_addr       Device address
 * @param[in] ep_addr        Endpoint address
 * @param[in] event          Transfer result event (success, failed, etc.)
 * @param[in] xferred_bytes  Number of bytes transferred
 *
 * @return True if the transfer callback is successfully processed
 * @return False if an error occurred during processing
 */
bool msch_xfer_cb    (uint8_t dev_addr, uint8_t ep_addr, xfer_result_t event, uint32_t xferred_bytes);

#ifdef __cplusplus
}
#endif

#endif

/** @}*/ /* End of MSC_HOST */