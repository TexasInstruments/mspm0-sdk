/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "bsl_ci.h"
#include "aes_gcm.h"
#include "boot_config.h"
#include "ti_msp_dl_config.h"

#define MAIN_SECTOR_SIZE (1024)
#define PASSWORD_ERROR_LIMIT 5

uint32_t BSL_CI_getFlashEndAddress(void);

/*
 * Global Variables
 */

/* Buffer variables */
extern uint16_t BSL_maxBufferSize;
extern uint8_t *BSL_RXBuf;
extern uint8_t *BSL_TXBuf;
extern uint32_t BSL_RAM_bufStartAddress;
extern uint32_t BSL_RAM_bufEndAddress;

extern uint8_t BSLLockedStatus;
extern uint16_t pluginVersion;
extern BSLPluginInfo *activePlugin;

static uint8_t password_wrong_count = 0;

/*
 * Static function declaration
 */

/**
 * @brief       Validates the given password and unlocks the BSL if the password
 *              is correct.
 *
 * @return      None
 */
void CMD_API_Unlock_BSL(void);

/*
 * @brief       Erases given flash regions and returns the Status of operation
 *
 * @param[in]   addrStart   Start address for the memory region to be erased
 *
 * @return      Status of operation
 */
uint8_t CMD_API_Flash_Range_Erase(uint32_t addrStart);

/*
 * @brief       Executes Mass erase and returns the Status of operation to Host
 *
 * @return      Status of operation
 */
uint8_t CMD_API_Mass_Erase(void);

/*
 * @brief       Programs the given data and returns the Status of operation
 *
 * @param[in]   addrStart   Start address for the memory region to be programmed
 *
 * @return      Status of operation
 */
uint8_t CMD_API_Program_Data(uint32_t addrStart);

/*
 * @brief       Readout the memory region and return the data to Host
 *
 * @param[in]   addrStart   Start address for the memory region to read from
 *
 * @return      None
 */
//void CMD_API_Memory_Read_back(uint32_t addrStart);

/*
 * @brief       Executes factory reset, based on the Bootcode configuration
 *              and returns the Status of operation to Host
 *
 * @return      Status of operation
 */
//uint8_t CMD_API_Factory_Reset(void);

/*
 * @brief       Returns the device information to Host
 *
 * @return      None
 */
void CMD_API_Get_Identity(void);

/*
 * @brief       Calculates the CRC for the given memory range and returns the
 *              CRC value to the Host
 *
 * @param[in]   addrStart   Start address from where the CRC should be computed
 *
 * @return      None
 */
void CMD_API_Standalone_Verification(uint32_t addrStart);

/*
 * @brief       Starts the application
  *
 * @return      None
 */
void CMD_API_startApplication(void);

/*
 * @brief       Frames and sends the BSL core message response packet
 *
 * @param[in]   message     Message Response to be sent to the Host
 *
 * @return      None
 */
void BSL_CI_sendMessage(uint8_t message);

/*
 * @brief       Calculates the CRC32 for the given data
 *
 * @param[in]   data        Pointer to the data byte array for which CRC should
 *                          be calculated
 * @param[in]   dataSize    Size of the data transferred for calculation
 *
 * @return      32 bit CRC value
 */
uint32_t BSL_CI_calculateCRC(uint8_t *data, uint32_t dataSize);

/*
 * @brief       Enables ICACHE and Prefetch
 *
 * @return      None
 */
void BSL_CI_enableCache(void);

/*
 * @brief       Disables ICACHE and Prefetch
 *
 * @return      None
 */
void BSL_CI_disableCache(void);

/*
 * Function definition
 */
void BSL_CI_interpretCommand(void)
{
    uint8_t error_buff;

    /* Pick the command from the packet */
    uint8_t command = BSL_RXBuf[CMD_INDEX];

    /*
     * Read the start address from the packet
     * All commands will not have start address as part of the packet
     * This value will be used only in commands where it is applicable
     */
    uint32_t addr = *(uint32_t *) &BSL_RXBuf[START_ADDRESS_INDEX];

    /*
     * Based on the Command value, branch to the appropriate APIs
     */
    switch (command) {
        case CMD_CONNECTION:
            //            /* acknowledge */
            //            BSL_CI_sendAck();
            break;
        case CMD_UNLOCK_BSL:
            CMD_API_Unlock_BSL();
            break;
        case CMD_FLASH_RANGE_ERASE:
            BSL_CI_sendMessage(CMD_API_Flash_Range_Erase(addr));
            break;
        case CMD_MASS_ERASE:
            BSL_CI_sendMessage(CMD_API_Mass_Erase());
            break;
        case CMD_PROGRAM_DATA:
            BSL_CI_sendMessage(CMD_API_Program_Data(addr));
            break;
        case CMD_PROGRAM_DATA_FAST:
            CMD_API_Program_Data(addr);
            break;
            //        case CMD_MEMORY_READ_BACK:
            //            CMD_API_Memory_Read_back(addr);
            //            break;
            //        case CMD_FACTORY_RESET: {
            //            BSL_CI_sendMessage(CMD_API_Factory_Reset());
            //            break;
            //        }
        case CMD_GET_IDENTITY:
            CMD_API_Get_Identity();
            break;
        case CMD_STANDALONE_VERIFICATION:
            CMD_API_Standalone_Verification(addr);
            break;
        case CMD_START_APPLICATION:
            CMD_API_startApplication();
            break;
        default:
            BSL_CI_sendMessage(BSL_UNKNOWN_COMMAND);
            break;
    }
}

/* To Do: since this is happening in unsecure side, we're going to override password for now */
void CMD_API_Unlock_BSL(void)
{
    uint8_t ret = BSL_PASSWORD_ERROR;

    uint32_t passwordComparison = (uint32_t) 0;
    uint16_t i;
    uint8_t index;
    uint16_t length;
    uint32_t *password;

    length = (uint16_t) BSL_RXBuf[LEN_BYTE_L_INDEX] |
             ((uint16_t) BSL_RXBuf[LEN_BYTE_H_INDEX] << 8U);
    password = (uint32_t *) &BSL_RXBuf[PASSWORD_INDEX];

    if (length == UNLOCK_BSL_CMD_LEN) {
#if MODULE_SHA_CHECK
        /*
         * checking if BSL unlock command is given after factory reset
         * if yes unlock the BSL with all "0xFF" password
         * else proceed password check with SHA
         */

        DL_FLASHCTL_FAIL_TYPE eraseBslPasswordField;

        /* Checking if BSL password field address pointer is Blank */
        DL_FlashCTL_executeClearStatus(FLASH_Reg);
        DL_FlashCTL_blankVerify(FLASH_Reg, BSL_PASSWORD_ADDRESS);
        (void) DL_FlashCTL_waitForCmdDone(FLASH_Reg);
        eraseBslPasswordField = DL_FlashCTL_getFailureStatus(FLASH_Reg);

        if (eraseBslPasswordField == DL_FLASHCTL_FAIL_TYPE_NO_FAILURE) {
            for (index = (uint8_t) 0; index < BSL_PASSWORD_LEN; index++) {
                passwordComparison |= ~password[index];
            }
        } else {
            /*
             * Design: MSPTBL-393
             */
            int_fast16_t result;
            uint32_t calculatedSha[8];

            SHA256SW_Object sha256SWObject;

            result = SHA256SW_hashData(&sha256SWObject, SHA2SW_HASH_TYPE_256,
                password, 32, (uint32_t *) calculatedSha);

            if (result < (int_fast16_t) 0) {
                //not correct format - sha check error
                ret = BSL_SHA_CHECK_ERROR;
                BSL_CI_sendMessage(BSL_SHA_CHECK_ERROR);
                passwordComparison = (uint32_t) 1;
            } else {
                for (index = (uint8_t) 0; index < BSL_PASSWORD_LEN; index++) {
                    passwordComparison |=
                        BSLConfig.password[index] ^ calculatedSha[index];
                }
            }
        }
#else
        for (index = (uint8_t) 0; index < BSL_PASSWORD_LEN; index++) {
            passwordComparison |= BSLConfig.password[index] ^ password[index];
        }
#endif
        // TODO: change this back to the correct comparison using hash if desired.
        /*
        * If the given password matches the BSL password in BSL configuration
        * structure, then Unlock the BSL and send success response
        */
        //        if(passwordComparison == (uint32_t)0)
        //        {
        BSLLockedStatus = BSL_STATUS_UNLOCKED;
        BSL_CI_sendMessage(BSL_SUCCESSFUL_OPERATION);
        ret = BSL_SUCCESSFUL_OPERATION;
        //        }
    }

    /*
     * If the password doesn't match, return password error
     * and upon getting multiple password errors take the specified alert action
     */
    if (ret != BSL_SUCCESSFUL_OPERATION) {
        password_wrong_count++;
        BSLLockedStatus = BSL_STATUS_LOCKED;

        if (password_wrong_count < PASSWORD_ERROR_LIMIT) {
            BSL_CI_sendMessage(BSL_PASSWORD_ERROR);
            //            (void) active_plugin->deinit();

            //            // Set timer interrupt mode and timeout to two seconds
            //            DL_TimerG_stopCounter(BSL_TIMER);
            //            DL_TimerG_setLoadValue(BSL_TIMER, BSL_TWO_SECOND_TIMEOUT);
            //            DL_TimerG_startCounter(BSL_TIMER);
            //
            //            DL_SYSCTL_setPowerPolicyRUN0SLEEP0();
            //            __WFI();
            //
            //            // Initialize the plugin after the device wakes up from Timer interrupt
            //            (void) active_plugin->init(BSL_Buf1, BSL_maxBufferSize);
            //
            //            // Set timer interrupt mode and timeout back to ten seconds
            //            DL_TimerG_stopCounter(BSL_TIMER);
            //            timeout_detected = (uint8_t)0;
            //            DL_TimerG_setLoadValue(BSL_TIMER, BSL_TEN_SECOND_TIMEOUT);
            //            DL_TimerG_startCounter(BSL_TIMER);
        } else {
            BSL_CI_sendMessage(BSL_MULTIPLE_PASSWORD_ERROR);
            password_wrong_count = (uint8_t) 0;

            //            if(BSLConfig.securityAlert == BL_CFG_SECURITY_FACTORY_RESET)
            //            {
            //                BSL_CI_disableCache();
            //                (void) DL_FlashCTL_factoryResetMultiBank(FLASHCTL);
            //                BSL_CI_enableCache();
            //            }
            //            /*
            //             * Disable Bootloader Alert action can be supported, only if the device has
            //             * greater than or equal to 2KB of SRAM memory, given that the Non-main Flash Sector size is 1KB
            //             */
            //            else if((BSLConfig.securityAlert == BL_CFG_SECURITY_DISABLE_BSL) &&
            //                    ((BSL_maxBufferSize * BSL_NUM_BUFFERS) >= (uint16_t)INFO_SECTOR_SIZE))
            //            {
            //                bcUserCfg_t *sram_bcUserCfg;
            //                uint8_t *data;
            //                uint32_t crcCalculationLen;
            //
            //                (void) active_plugin->deinit();
            //                data = (uint8_t*)(MSPM0_INFO_FLASH_START_ADDRESS);
            //
            //                for(i=(uint16_t)0; i < (uint16_t)INFO_SECTOR_SIZE; i++)
            //                {
            //                    BSL_Buf1[i] = data[i];
            //                }
            //
            //                sram_bcUserCfg = (bcUserCfg_t *)BSL_Buf1;
            //                sram_bcUserCfg->bootloaderMode = 0xFFFF;
            //
            //                crcCalculationLen = (uint32_t)sizeof(bcUserCfg_t) - (uint32_t)sizeof(sram_bcUserCfg->bcrConfigCRC);
            //                sram_bcUserCfg->bcrConfigCRC = BSL_CI_calculateCRC(BSL_Buf1, crcCalculationLen);
            //
            //                BSL_CI_disableCache();
            //                DL_FlashCTL_executeClearStatus(FLASH_Reg);
            //                DL_FlashCTL_unprotectSector(FLASHCTL, MSPM0_INFO_FLASH_START_ADDRESS, DL_FLASHCTL_REGION_SELECT_NONMAIN);
            //                DL_FlashCTL_eraseMemory(FLASHCTL, MSPM0_INFO_FLASH_START_ADDRESS,
            //                                     DL_FLASHCTL_COMMAND_SIZE_SECTOR);
            //                (void) DL_FlashCTL_waitForCmdDone(FLASHCTL);
            //
            //                DL_FlashCTL_unprotectSector(FLASHCTL, MSPM0_INFO_FLASH_START_ADDRESS, DL_FLASHCTL_REGION_SELECT_NONMAIN);
            //                (void) FLASH_PROGRAM_MEMORY64_BLOCKING(FLASHCTL, MSPM0_INFO_FLASH_START_ADDRESS,
            //                                               (uint32_t *)&BSL_Buf1[0],
            //                                               (uint32_t)INFO_SECTOR_SIZE/(uint32_t)4,
            //                                               DL_FLASHCTL_REGION_SELECT_NONMAIN);
            //                BSL_CI_enableCache();
            //
            //                DL_SYSCTL_resetDevice(DL_SYSCTL_RESET_BOOTLOADER_EXIT);
            //            }
            //            else
            //            {
            //                /* Empty else for MISRA-C */
            //            }
        }
    }
}

uint8_t CMD_API_Flash_Range_Erase(uint32_t addrStart)
{
    uint8_t ret                       = BSL_SUCCESSFUL_OPERATION;
    uint8_t erase_status              = (uint8_t) 1;
    uint32_t main_flash_start_address = MSPM0_MAIN_FLASH_START_ADDRESS;
    uint32_t main_flash_end_address;
    uint32_t addr_end;
    uint32_t data_pointer;

    main_flash_end_address = BSL_CI_getFlashEndAddress();
    addr_end               = *((uint32_t *) &BSL_RXBuf[END_ADDRESS_INDEX]);
    data_pointer           = addrStart;

    /* Check if BSL is unlocked */
    if (BSLLockedStatus == BSL_STATUS_UNLOCKED) {
        /* If the address range is valid, then erase the memory range */
        if ((addrStart >= main_flash_start_address) &&
            (addr_end <= main_flash_end_address) && (addrStart <= addr_end)) {
            BSL_CI_disableCache();
            while (data_pointer < addr_end) {
                DL_FlashCTL_unprotectSector(
                    FLASHCTL, data_pointer, DL_FLASHCTL_REGION_SELECT_MAIN);
                DL_FlashCTL_eraseMemoryFromRAM(
                    FLASHCTL, data_pointer, DL_FLASHCTL_COMMAND_SIZE_SECTOR);

                erase_status = erase_status &
                               (uint8_t) DL_FlashCTL_waitForCmdDone(FLASHCTL);
                data_pointer = data_pointer + MAIN_SECTOR_SIZE;
            }

            DL_FlashCTL_unprotectSector(
                FLASHCTL, addr_end, DL_FLASHCTL_REGION_SELECT_MAIN);
            DL_FlashCTL_eraseMemoryFromRAM(
                FLASHCTL, addr_end, DL_FLASHCTL_COMMAND_SIZE_SECTOR);

            erase_status =
                erase_status & (uint8_t) DL_FlashCTL_waitForCmdDone(FLASHCTL);

            BSL_CI_enableCache();

            /* Check if all flash sector erase got passed */
            if (erase_status == (uint8_t) 0) {
                ret = BSL_FLASH_ERASE_FAILED;
            }
        } else if (addrStart == ENCRYPTION_MAGIC_ADDR) {
            // IGNORE
        } else {
            ret = BSL_INVALID_MEMORY_RANGE;
        }
    } else {
        ret = BSL_LOCKED;
    }
    return ret;
}

uint8_t CMD_API_Mass_Erase(void)
{
    /* Mass erase is not allowed, so we simply do a sector by sector erase of the image slot */
    uint8_t ret                       = BSL_SUCCESSFUL_OPERATION;
    uint8_t erase_status              = (uint8_t) 1;
    uint32_t main_flash_start_address = MSPM0_MAIN_FLASH_START_ADDRESS;
    uint32_t main_flash_end_address   = BSL_CI_getFlashEndAddress();
    uint32_t data_pointer;

    data_pointer = main_flash_start_address;

    /* Check if BSL is unlocked */
    if (BSLLockedStatus == BSL_STATUS_UNLOCKED) {
        /* If the address range is valid, then erase the memory range */
        BSL_CI_disableCache();
        while (data_pointer <= main_flash_end_address) {
            DL_FlashCTL_unprotectSector(
                FLASHCTL, data_pointer, DL_FLASHCTL_REGION_SELECT_MAIN);
            DL_FlashCTL_eraseMemoryFromRAM(
                FLASHCTL, data_pointer, DL_FLASHCTL_COMMAND_SIZE_SECTOR);

            erase_status =
                erase_status & (uint8_t) DL_FlashCTL_waitForCmdDone(FLASHCTL);
            data_pointer = data_pointer + MAIN_SECTOR_SIZE;
        }

        erase_status =
            erase_status & (uint8_t) DL_FlashCTL_waitForCmdDone(FLASHCTL);

        BSL_CI_enableCache();

        /* Check if all flash sector erase got passed */
        if (erase_status == (uint8_t) 0) {
            ret = BSL_FLASH_ERASE_FAILED;
        }
    } else {
        ret = BSL_LOCKED;
    }
    return ret;
}

uint8_t CMD_API_Program_Data(uint32_t addrStart)
{
    uint8_t ret = BSL_SUCCESSFUL_OPERATION;

    uint32_t main_flash_start_address = MSPM0_MAIN_FLASH_START_ADDRESS;
    uint32_t main_flash_end_address;

    DL_FLASHCTL_REGION_SELECT flash_region;
    uint16_t length = (uint16_t) 0U;

    /* Initialize address */
    main_flash_end_address = BSL_CI_getFlashEndAddress();

    length = (uint16_t) BSL_RXBuf[1];
    length += BSL_RXBuf[2] << 8U;
    length -= (uint16_t) 5U;  // Command byte (1) + Address (4)

    /* Check if BSL is unlocked */
    if (BSLLockedStatus == BSL_STATUS_UNLOCKED) {
        /*
         * If the address range falls in valid SRAM memory range
         * Store the data directly in the address
         */
        if ((BSL_RAM_bufStartAddress <= addrStart) &&
            ((addrStart + (uint32_t) length) <= BSL_RAM_bufEndAddress)) {
            uint8_t *ram_pointer;
            uint8_t *data = &BSL_RXBuf[8];
            ram_pointer   = (uint8_t *) addrStart;
            uint16_t i;

            /* Copy data to RAM */
            for (i = (uint16_t) 0; i < length; i++) {
                ram_pointer[i] = data[i];
            }
        } else {
            /*
             * Since FLASHCTL allows programming for only 64 bits aligned
             * address, This condition checks if given memory address and length
             * are 8 bytes aligned
             */
            if (((addrStart % (uint32_t) 8U) == (uint32_t) 0U) &&
                (((uint32_t) length % (uint32_t) 8U) == (uint32_t) 0U)) {
                /*
                 * Checks if the memory address given is in valid
                 * flash memory range
                 */
                if (((main_flash_start_address <= addrStart) &&
                        ((addrStart + (uint32_t) length) <=
                            main_flash_end_address))) {
                    flash_region = DL_FLASHCTL_REGION_SELECT_MAIN;

                    uint8_t *source = &BSL_RXBuf[8];

#ifdef ENCRYPTION_ENABLED
                    // DECRYPT
                    GCM_status gcm_status;

                    gcm_status = GCM_streamDataToFlash(
                        addrStart, (uint32_t *) source, length);

                    if (!(gcm_status == GCM_OK ||
                            gcm_status == GCM_OPERATION_CONTINUES)) {
                        ret = BSL_FLASH_PROGRAM_FAILED;
                        // erase the programming, as the authentication failed
                        CMD_API_Mass_Erase();
                    }

#else
                    BSL_CI_disableCache();
                    /* Initiate Flash Programming */
                    if (DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(
                            FLASHCTL, addrStart, (uint32_t *) source,
                            (uint32_t) length / (uint32_t) 4,
                            flash_region) == false) {
                        ret = BSL_FLASH_PROGRAM_FAILED;
                    }
                    BSL_CI_enableCache();

#endif

                }
#ifdef ENCRYPTION_ENABLED
                else if (addrStart == ENCRYPTION_MAGIC_ADDR) {
                    GCM_status status;
                    // set up the GCM in the correct manner
                    status = GCM_init((uintptr_t) &BSL_RXBuf[8]);
                    if (status != GCM_OK) {
                        ret = BSL_PASSWORD_ERROR;
                    }
                }
#endif  // ENCRYPTION Enabled
                else {
                    ret = BSL_INVALID_MEMORY_RANGE;
                }
            } else {
                ret = BSL_INVALID_ADDR_LEN_ALIGNMENT;
            }
        }
    } else {
        ret = BSL_LOCKED;
    }
    return ret;
}

// Currently not part of common use and requires additional size
//void CMD_API_Memory_Read_back(uint32_t addrStart)
//{
//    uint32_t main_flash_start_address = MSPM0_MAIN_FLASH_START_ADDRESS;
//    uint32_t main_flash_end_address;
//
//    uint16_t max_length;
//    uint32_t length;
//    uint32_t addr_end;
//    uint16_t i, bytes;
//
//    /* Initialize address */
//    main_flash_end_address = BSL_CI_getFlashEndAddress();
//
//    length   = *((uint32_t *) &BSL_RXBuf[DATA_LENGTH_INDEX]);
//    addr_end = addrStart + length;
//
//    /* Packet length = 8, (Header - 1, length - 2, cmd - 1, crc - 4 bytes) */
//    max_length = BSL_maxBufferSize - (uint16_t) 8U;
//
//    /* Check if BSL is unlocked */
//    if (BSLLockedStatus == BSL_STATUS_UNLOCKED) {
//        /* Check if Memory readout is enabled in BCR configuration */
//        if (BSLConfig.memoryRead == BSL_CFG_MEMORY_READOUT_ENABLE) {
//            /* Check if address is in the valid memory region */
//            if (((BSL_RAM_bufStartAddress <= addrStart) &&
//                    (addr_end <= BSL_RAM_bufEndAddress)) ||
//                ((main_flash_start_address <= addrStart) &&
//                    (addr_end <= main_flash_end_address))) {
//                /*
//                 * Frame the BSL Core Response packet to send the readout
//                 * data
//                 */
//                BSL_TXBuf[HEADER_INDEX] = PACKET_HEADER_RESPONSE;
//                BSL_TXBuf[CMD_INDEX]    = BSL_MEMORY_READ_BACK;
//
//                /*
//                 * If the data to be returned is more than the maximum length
//                 * that can be transferred in one packet, send the data in
//                 * multiple packets
//                 */
//                while (addrStart < addr_end) {
//                    if ((addr_end - addrStart) > (uint32_t) max_length) {
//                        bytes = max_length;
//                    } else {
//                        bytes = addr_end - addrStart;
//                    }
//                    for (i = (uint16_t) 0; i < bytes; i++) {
//                        BSL_TXBuf[i + (uint16_t) 4] =
//                            (*((volatile uint8_t *) (addrStart)));
//                        addrStart++;
//                    }
//
//                    /* Compute the Length of the packet */
//
//                    bytes += (uint16_t) 1U;  // Adding 1 for Command byte
//
//                    BSL_TXBuf[LEN_BYTE_L_INDEX] =
//                        (uint8_t) bytes & (uint8_t) 0xFF;
//                    BSL_TXBuf[LEN_BYTE_H_INDEX] =
//                        (bytes & (uint16_t) 0xFF00) >> 8U;
//
//                    /* Transmit the Data packet */
//                    activePlugin->send(BSL_TXBuf, (bytes + PACKET_HEADER_LEN));
//                }
//            } else {
//                BSL_CI_sendMessage(BSL_INVALID_MEMORY_RANGE);
//            }
//        } else {
//            BSL_CI_sendMessage(BSL_READOUT_DISABLED);
//        }
//    } else {
//        BSL_CI_sendMessage(BSL_LOCKED);
//    }
//}

// Not to be supported
//uint8_t CMD_API_Factory_Reset(void)
//{
//    uint8_t ret = BSL_SUCCESSFUL_OPERATION;
//
//    uint32_t passwordComparison = (uint32_t) 0;
//    uint8_t index;
//    uint16_t length;
//    uint32_t *password;
//
//    length = (uint16_t) BSL_RXBuf[LEN_BYTE_L_INDEX] |
//             (BSL_RXBuf[LEN_BYTE_H_INDEX] << 8U);
//    password = (uint32_t *) &BSL_RXBuf[PASSWORD_INDEX];
//
//    /* Check if BSL is unlocked */
//    if (BSLLockedStatus == BSL_STATUS_UNLOCKED) {
//        /* Check if Factory reset is not enabled in the BCR configuration */
//        if ((pBCRConfig->factoryResetMode != BCR_CFG_FACTORY_RESET_EN) &&
//            (pBCRConfig->factoryResetMode != BCR_CFG_FACTORY_RESET_EN_PW)) {
//            ret = BSL_FACTORY_RESET_DISABLED;
//        } else {
//            /*
//             * If factory reset is enabled with Password, then validate the
//             * password received before executing the factory reset
//             */
//            if (pBCRConfig->factoryResetMode == BCR_CFG_FACTORY_RESET_EN_PW) {
//                if (length == FACTORY_RESET_CMD_LEN) {
//                    for (index = (uint8_t) 0; index < PASSWOD_WORD_LEN;
//                         index++) {
//                        passwordComparison |=
//                            (pBCRConfig->passwordFactoryReset[index] ^
//                                password[index]);
//                    }
//                    /*
//                     * If the password mismatches, return password error to host
//                     */
//                    if (passwordComparison != (uint32_t) 0) {
//                        ret = BSL_FACTORY_RESET_PASSWORD_ERROR;
//                    }
//                } else {
//                    ret = BSL_FACTORY_RESET_PASSWORD_ERROR;
//                }
//            }
//
//            /*
//             * If factory reset is enabled without password or if the password
//             * authentication is successful, perform factory reset and
//             * return the status
//             */
//            if (ret == BSL_SUCCESSFUL_OPERATION) {
//                BSL_CI_disableCache();
//                if (DL_FlashCTL_factoryReset(FLASHCTL) == false) {
//                    ret = BSL_FACTORY_RESET_FAILED;
//                }
//                BSL_CI_enableCache();
//            }
//        }
//    } else {
//        ret = BSL_LOCKED;
//    }
//    return ret;
//}

void CMD_API_Get_Identity(void)
{
    uint32_t app_revision = 0U;

    /* Frame the device information packet */
    BSL_TXBuf[HEADER_INDEX]     = PACKET_HEADER_RESPONSE;
    BSL_TXBuf[LEN_BYTE_L_INDEX] = GET_IDENTITY_RSP_LEN;
    BSL_TXBuf[LEN_BYTE_H_INDEX] = (uint8_t) 0x00;
    BSL_TXBuf[CMD_INDEX]        = GET_IDENTITY;

    *(uint16_t *) &BSL_TXBuf[4] = BSL_VERSION_CMD_INTERPRETER;
    *(uint16_t *) &BSL_TXBuf[6] = BSL_VERSION_BUILD_ID;

    /*
     * Check if the address is valid FLASHCTL Address
     * Else return '0' as application revision
     */
    if ((uint32_t) BSLConfig.appRev < BSL_CI_getFlashEndAddress()) {
        /*
         * Check if the address is blank
         * Access the data only if it is not blank
         * Else return '0' as application revision
         */
        DL_FlashCTL_blankVerify(FLASHCTL, (uint32_t) BSLConfig.appRev);
        DL_FlashCTL_waitForCmdDone(FLASHCTL);
        if (DL_FlashCTL_getFailureStatus(FLASHCTL) ==
            DL_FLASHCTL_FAIL_TYPE_VERIFY_ERROR) {
            app_revision = *(BSLConfig.appRev);
        }
    }

    *(uint32_t *) &BSL_TXBuf[8]  = app_revision;
    *(uint16_t *) &BSL_TXBuf[12] = pluginVersion;
    *(uint16_t *) &BSL_TXBuf[14] = BSL_maxBufferSize;
    *(uint32_t *) &BSL_TXBuf[16] = BSL_RAM_bufStartAddress;
    *(uint32_t *) &BSL_TXBuf[20] = BCRConfig.bcrConfigID;
    *(uint32_t *) &BSL_TXBuf[24] = BSLConfig.configID;

    /* Transmit the device information */
    activePlugin->send(BSL_TXBuf, (GET_IDENTITY_RSP_LEN + PACKET_HEADER_LEN));
}

void CMD_API_Standalone_Verification(uint32_t addrStart)
{
    uint32_t main_flash_start_address = 0x4000;
    uint32_t main_flash_end_address;

    uint32_t length;
    uint32_t addr_end;
    uint32_t result;

    main_flash_end_address = BSL_CI_getFlashEndAddress();

    length   = *((uint32_t *) &BSL_RXBuf[DATA_LENGTH_INDEX]);
    addr_end = addrStart + length;

    /* Check if BSL is unlocked */
    if (BSLLockedStatus == BSL_STATUS_UNLOCKED) {
        /* Check if length is valid else error is returned */
        if (length >= MAIN_SECTOR_SIZE) {
            /* Check if address is in the valid memory region */
            if (((BSL_RAM_bufStartAddress <= addrStart) &&
                    (addr_end <= BSL_RAM_bufEndAddress)) ||
                ((main_flash_start_address <= addrStart) &&
                    (addr_end <= main_flash_end_address))) {
                /* Frame the CRC packet */
                BSL_TXBuf[HEADER_INDEX]     = PACKET_HEADER_RESPONSE;
                BSL_TXBuf[LEN_BYTE_L_INDEX] = STANDALONE_VERIFY_RSP_LEN;
                BSL_TXBuf[LEN_BYTE_H_INDEX] = (uint8_t) 0x00;
                BSL_TXBuf[CMD_INDEX]        = STANDALONE_VERIFICATION;

                /* Compute the CRC for the given range */
                result = BSL_CI_calculateCRC((uint8_t *) addrStart, length);

                /* Store the CRC value in the response packet */
                *((uint32_t *) (&BSL_TXBuf[VERIFICATION_CRC_INDEX])) = result;

                /* Transmit the CRC packet */
                activePlugin->send(
                    BSL_TXBuf, ((uint16_t) STANDALONE_VERIFY_RSP_LEN +
                                   PACKET_HEADER_LEN));
            } else {
                BSL_CI_sendMessage(BSL_INVALID_MEMORY_RANGE);
            }
        } else {
            BSL_CI_sendMessage(BSL_STANDALONE_VERIFICATION_INVALID_LEN);
        }
    } else {
        BSL_CI_sendMessage(BSL_LOCKED);
    }
}

void CMD_API_startApplication(void)
{
    /* Start application */
    DL_SYSCTL_resetDevice(DL_SYSCTL_RESET_BOOT);

    /* Wait until reset happens */
    while (1)
        ;
}

void BSL_CI_sendMessage(uint8_t message)
{
    /* Frame the BSL core message response packet */
    BSL_TXBuf[HEADER_INDEX]     = PACKET_HEADER_RESPONSE;
    BSL_TXBuf[LEN_BYTE_L_INDEX] = MESSAGE_RSP_LEN;
    BSL_TXBuf[LEN_BYTE_H_INDEX] = (uint8_t) 0x00;
    BSL_TXBuf[CMD_INDEX]        = BSL_MESSAGE_REPLY;
    BSL_TXBuf[MESSAGE_INDEX]    = message;

    /* Transmit the response packet */
    activePlugin->send(BSL_TXBuf, (MESSAGE_RSP_LEN + PACKET_HEADER_LEN));
}

uint32_t BSL_CI_calculateCRC(uint8_t *data, uint32_t dataSize)
{
    uint32_t bufferIndex = (uint32_t) 0;

    /* Set the Seed value to reset the calculation */
    DL_CRCP_setSeed32(CRCP0, 0xFFFFFFFF);

    /* Feed the data to CRC module */
    for (bufferIndex = (uint32_t) 0; bufferIndex < dataSize; bufferIndex++) {
        DL_CRCP_feedData8(CRCP0, data[bufferIndex]);
    }

    /* Return the 32 bit result */
    return (DL_CRCP_getResult32(CRCP0));
}

inline void BSL_CI_disableCache(void)
{
    DL_CORE_configInstruction(CPUSS_CTL_ICACHE_DISABLE,
        CPUSS_CTL_PREFETCH_DISABLE, CPUSS_CTL_LITEN_DISABLE);
}

inline void BSL_CI_enableCache(void)
{
    DL_CORE_configInstruction(CPUSS_CTL_ICACHE_ENABLE,
        CPUSS_CTL_PREFETCH_ENABLE, CPUSS_CTL_LITEN_ENABLE);
}
