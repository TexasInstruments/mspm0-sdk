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

#include "bsp/board_api.h"
#include "sd_card.h"
#include "tusb.h"

#define SCSI_FORMAT_UNIT (0x04U)
#define SCSI_SYNC_CACHE (0x35U)
#define SCSI_VERIFY (0x2FU)

/*
 * Invoked to determine maximum number of logical units, this example
 * utilizes 1 SD card so we will always return 1
 */
uint8_t tud_msc_get_maxlun_cb(void)
{
    return 1;
}

/*
 * Invoked when SCSI_CMD_INQUIRY has been received, which will then return vendor id, product id,
 * and product revision of the SD card.
 */
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8],
    uint8_t product_id[16], uint8_t product_rev[4])
{
    (void) lun;

    /* 1. OID is typically 2 bytes, needs to be formatted as a string */
    char oid_str[3] = {0};
    oid_str[0]      = (mspm0SdCard.CID.cidBits.OID >> 8) & 0xFF;
    oid_str[1]      = mspm0SdCard.CID.cidBits.OID & 0xFF;
    memcpy(vendor_id, oid_str, 2);

    /* 2. PNM is 5 bytes, needs to be handled correctly */
    char pnm_str[6] = {0};
    memcpy(pnm_str, &mspm0SdCard.CID.cidBits.PNM, 5);
    memcpy(product_id, pnm_str, 5);

    /* 3. PRV is a single byte with version info, format as string */
    char prv_str[2] = {0};
    prv_str[0]      = (mspm0SdCard.CID.cidBits.PRV);
    memcpy(product_rev, prv_str, 1);
}

/*
 * If SD card has been initialized, return true. Otherwise return false
 */
bool tud_msc_test_unit_ready_cb(uint8_t lun)
{
    (void) lun;

    if (mspm0SdCard.cardIsInitialized == 0)
        return false;
    else
        return true;
}

/*
 * Invoked when SCSI_CMD_READ_CAPACITY_10 or READ_FORMAT_CAPACITY command is received.
 * Block count and block size will be returned to determine size of SD card
 */
void tud_msc_capacity_cb(
    uint8_t lun, uint32_t* block_count, uint16_t* block_size)
{
    (void) lun;

    /* Block size will always be 512 */
    *block_size = 512;
    /* If it is a standard card then block count must be calculated */
    if (mspm0SdCard.cardIsSDHC_SDXC == 1)
        *block_count = mspm0SdCard.blockCount;
    else
        *block_count = (mspm0SdCard.blockCount * mspm0SdCard.blockSize) / 512;
}

/*
 * Invoked when received Start Stop Unit command
 * - Start = 0 : stopped power mode, if load_eject = 1 : unload disk storage
 * - Start = 1 : active mode, if load_eject = 1 : load disk storage
 */
bool tud_msc_start_stop_cb(
    uint8_t lun, uint8_t power_condition, bool start, bool load_eject)
{
    (void) lun;
    (void) power_condition;
    (void) load_eject;
    (void) start;
    if (load_eject) {
        if (start) {
            /* Load disk storage */
        } else {
            /* Unload disk storage */
        }
    }

    return true;
}

/* If SD card has been intialized then it is writable, otherwise return false if not initialized */
bool tud_msc_is_writable_cb(uint8_t lun)
{
    (void) lun;

    if (mspm0SdCard.cardIsInitialized == 1)
        return true;
    else
        return false;
}

/*
 * Callback invoked when received READ10 command.
 * Copy disk's data to buffer (up to bufsize) and return number of copied bytes.
 */
int32_t tud_msc_read10_cb(
    uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize)
{
    (void) offset;

    /* Check if SD card is initialized */
    if (!mspm0SdCard.cardIsInitialized) {
        tud_msc_set_sense(lun, SCSI_SENSE_NOT_READY, 0x3A, 0x00);
        return -1;
    }

    /* LBA is outside of expected memory range for SD card */
    if (lba >= mspm0SdCard.blockCount) {
        tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x21, 0x00);
        return -1;
    }

    /*
     * Since CFG_TUD_MSC_EP_BUFSIZE equals BLOCK_SIZE, we should always get full block requests.
     * Read the requested blocks directly into the provided buffer
     */
    uint32_t blocks_to_read = bufsize / mspm0SdCard.blockSize;

    for (uint32_t i = 0; i < blocks_to_read; i++) {
        if (!sdCardReadBlock(&mspm0SdCard, lba + i)) {
            tud_msc_set_sense(lun, SCSI_SENSE_MEDIUM_ERROR, 0x03, 0x00);
            return i > 0 ? i * mspm0SdCard.blockSize : -1;
        }
        memcpy((uint8_t*) buffer + (i * mspm0SdCard.blockSize),
            mspm0SdCard.buffer, mspm0SdCard.blockSize);
    }

    return bufsize;
}

/*
 * Callback invoked when received WRITE10 command.
 * Process data in buffer to disk's storage and return number of written bytes
 */
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset,
    uint8_t* buffer, uint32_t bufsize)
{
    (void) offset;

    /* Validate that there is a valid buffer */
    if (buffer == NULL || bufsize == 0) {
        tud_msc_set_sense(
            lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);  // Invalid parameter
        return -1;
    }

    /* Check if SD card is initialized */
    if (!mspm0SdCard.cardIsInitialized) {
        tud_msc_set_sense(lun, SCSI_SENSE_NOT_READY, 0x3A, 0x00);
        return -1;
    }

    /* LBA is outside of expected memory range for SD card */
    if (lba >= mspm0SdCard.blockCount) {
        tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x21, 0x00);
        return -1;
    }

    /*
     * Since CFG_TUD_MSC_EP_BUFSIZE equals BLOCK_SIZE, we should always get full block requests.
     * Write the requested blocks directly from the provided buffer
     */
    uint32_t blockToWrite = bufsize / mspm0SdCard.blockSize;
    for (uint32_t i = 0; i < blockToWrite; i++) {
        if (!sdCardWriteBlock(
                &mspm0SdCard, lba + i, buffer + (i * mspm0SdCard.blockSize))) {
            /* Write error */
            tud_msc_set_sense(lun, SCSI_SENSE_MEDIUM_ERROR, 0x03, 0x00);
            if (i > 0)
                return (i * mspm0SdCard.blockSize);
            else
                return -1;
        }
    }

    return bufsize;
}

/*
 * Callback invoked when received an SCSI command not in built-in list below
 * - READ_CAPACITY10, READ_FORMAT_CAPACITY, INQUIRY, MODE_SENSE6, REQUEST_SENSE
 * - READ10 and WRITE10 has their own callbacks (MUST not be handled here)
 */
int32_t tud_msc_scsi_cb(
    uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize)
{
    (void) bufsize;
    (void) buffer;
    void const* response = NULL;

    /* most scsi handled is input */
    bool in_xfer = true;

    switch (scsi_cmd[0]) {
        case SCSI_FORMAT_UNIT: {
            /* Check if SD card is ready */
            if (!mspm0SdCard.cardIsInitialized) {
                tud_msc_set_sense(lun, SCSI_SENSE_NOT_READY, 0x3A, 0x00);
                return -1;
            }

            /*
             * Here we'll just clear the MBR (first sector) to allow the
             * host to create a new filesystem, create a buffer of zeros
             */
            uint8_t zeros[512];
            memset(zeros, 0, sizeof(zeros));

            /* Write zeros to the first sector (MBR) */
            if (!sdCardWriteBlock(&mspm0SdCard, 0, zeros)) {
                tud_msc_set_sense(lun, SCSI_SENSE_MEDIUM_ERROR, 0x03, 0x00);
                return -1;
            }
            /* The host will follow this with its own writes to create the filesystem */
            return 0;
        }

        case SCSI_SYNC_CACHE:
            /*
         * Ensure all pending writes are flushed to the SD card
         * For SD cards in SPI mode, this is not needed as writes are synchronous
         */
            return 0;

        case SCSI_VERIFY:
            /*
         * The host is asking us to verify data. For a basic implementation,
         * we can just return success since we don't have a separate verify operation
         */
            return 0;
        default:
            /* REQUEST_SENSE */
            tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);
            /* negative means error -> tinyusb could stall and/or response with failed status */
            return -1;
    }
}
