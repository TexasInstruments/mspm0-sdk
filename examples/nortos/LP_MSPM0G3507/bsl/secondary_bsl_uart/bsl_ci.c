/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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

#include "boot_config.h"
#include "ti_msp_dl_config.h"

#include "bsl_ci.h"

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

/* Configuration structures */
extern volatile BSL_Config *pBSLConfig;
extern volatile BCR_Config *pBCRConfig;

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
void CMD_API_Memory_Read_back(uint32_t addrStart);

/*
 * @brief       Executes factory reset, based on the Bootcode configuration
 *              and returns the Status of operation to Host
 *
 * @return      Status of operation
 */
uint8_t CMD_API_Factory_Reset(void);

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
        case CMD_UNLOCK_BSL: {
            CMD_API_Unlock_BSL();
            break;
        }
        case CMD_FLASH_RANGE_ERASE: {
            BSL_CI_sendMessage(CMD_API_Flash_Range_Erase(addr));
            break;
        }
        case CMD_MASS_ERASE: {
            BSL_CI_sendMessage(CMD_API_Mass_Erase());
            break;
        }
        case CMD_PROGRAM_DATA: {
            BSL_CI_sendMessage(CMD_API_Program_Data(addr));
            break;
        }
        case CMD_PROGRAM_DATA_FAST: {
            CMD_API_Program_Data(addr);
            break;
        }
        case CMD_MEMORY_READ_BACK: {
            CMD_API_Memory_Read_back(addr);
            break;
        }
        case CMD_FACTORY_RESET: {
            BSL_CI_sendMessage(CMD_API_Factory_Reset());
            break;
        }
        case CMD_GET_IDENTITY: {
            CMD_API_Get_Identity();
            break;
        }
        case CMD_STANDALONE_VERIFICATION: {
            CMD_API_Standalone_Verification(addr);
            break;
        }
        case CMD_START_APPLICATION: {
            CMD_API_startApplication();
            break;
        }
        case CMD_CONNECTION: {
            //   BSL_CI_sendMessage(BSL_SUCCESSFUL_OPERATION);
            break;
        }
        default: {
            BSL_CI_sendMessage(BSL_UNKNOWN_COMMAND);
            break;
        }
    }
}

void CMD_API_Unlock_BSL(void)
{
    uint8_t ret = BSL_PASSWORD_ERROR;

    uint32_t passwordComparison = (uint32_t) 0;
    uint16_t i;
    uint8_t index;
    uint16_t length;
    uint32_t *password;
    uint32_t expPassword;

    length = (uint16_t) BSL_RXBuf[LEN_BYTE_L_INDEX] |
             ((uint16_t) BSL_RXBuf[LEN_BYTE_H_INDEX] << 8U);
    password = (uint32_t *) &BSL_RXBuf[PASSWORD_INDEX];

    if (length == UNLOCK_BSL_CMD_LEN) {
        for (index = (uint8_t) 0; index < BSL_PASSWORD_LEN; index++) {
            passwordComparison |=
                pBSLConfig->password[index] ^ password[index];
        }

        /*
         * If the given password matches the BSL password in BSL configuration
         * structure, then Unlock the BSL and send success response
         */
        if (passwordComparison == (uint32_t) 0) {
            BSLLockedStatus = BSL_STATUS_UNLOCKED;
            BSL_CI_sendMessage(BSL_SUCCESSFUL_OPERATION);
            ret = BSL_SUCCESSFUL_OPERATION;
        }
    }

    /* If the password doesn't match, return password error */
    if (ret != BSL_SUCCESSFUL_OPERATION) {
        BSLLockedStatus = BSL_STATUS_LOCKED;
        BSL_CI_sendMessage(BSL_PASSWORD_ERROR);
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
            while (data_pointer <= addr_end) {
                DL_FlashCTL_unprotectSector(
                    FLASHCTL, data_pointer, DL_FLASHCTL_REGION_SELECT_MAIN);
                DL_FlashCTL_eraseMemory(
                    FLASHCTL, data_pointer, DL_FLASHCTL_COMMAND_SIZE_SECTOR);

                erase_status = erase_status &
                               (uint8_t) DL_FlashCTL_waitForCmdDone(FLASHCTL);
                data_pointer = data_pointer + MAIN_SECTOR_SIZE;
            }

            DL_FlashCTL_unprotectSector(
                FLASHCTL, addr_end, DL_FLASHCTL_REGION_SELECT_MAIN);
            DL_FlashCTL_eraseMemory(
                FLASHCTL, addr_end, DL_FLASHCTL_COMMAND_SIZE_SECTOR);

            erase_status =
                erase_status & (uint8_t) DL_FlashCTL_waitForCmdDone(FLASHCTL);

            BSL_CI_enableCache();

            /* Check if all flash sector erase got passed */
            if (erase_status == (uint8_t) 0) {
                ret = BSL_FLASH_ERASE_FAILED;
            }
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
    uint8_t ret = BSL_SUCCESSFUL_OPERATION;

    /* Check if BSL is unlocked */
    if (BSLLockedStatus == BSL_STATUS_UNLOCKED) {
        BSL_CI_disableCache();
        /* Execute Mass erase */
        if (DL_FlashCTL_massErase(FLASHCTL) == false) {
            ret = BSL_MASS_ERASE_FAIL;
        }
        BSL_CI_enableCache();
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
    uint32_t info_flash_start_address = MSPM0_INFO_FLASH_START_ADDRESS;
    uint32_t info_flash_end_address   = MSPM0_INFO_FLASH_END_ADDRESS;

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
                            main_flash_end_address)) ||
                    ((info_flash_start_address <= addrStart) &&
                        ((addrStart + (uint32_t) length) <=
                            info_flash_end_address))) {
                    /* Check if the address is in Non-main Flash */
                    flash_region = DL_FLASHCTL_REGION_SELECT_MAIN;
                    if (info_flash_start_address <= addrStart) {
                        flash_region = DL_FLASHCTL_REGION_SELECT_NONMAIN;
                    }

                    BSL_CI_disableCache();
                    /* Initiate Flash Programming */
                    if (DL_FlashCTL_programMemoryBlocking64WithECCGenerated(
                            FLASHCTL, addrStart, (uint32_t *) &BSL_RXBuf[8],
                            (uint32_t) length / (uint32_t) 4,
                            flash_region) == false) {
                        ret = BSL_FLASH_PROGRAM_FAILED;
                    }
                    BSL_CI_enableCache();
                } else {
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

void CMD_API_Memory_Read_back(uint32_t addrStart)
{
    uint32_t main_flash_start_address = MSPM0_MAIN_FLASH_START_ADDRESS;
    uint32_t main_flash_end_address;
    uint32_t info_flash_start_address = MSPM0_INFO_FLASH_START_ADDRESS;
    uint32_t info_flash_end_address   = MSPM0_INFO_FLASH_END_ADDRESS;

    uint16_t max_length;
    uint32_t length;
    uint32_t addr_end;
    uint16_t i, bytes;

    /* Initialize address */
    main_flash_end_address = BSL_CI_getFlashEndAddress();

    length   = *((uint32_t *) &BSL_RXBuf[DATA_LENGTH_INDEX]);
    addr_end = addrStart + length;

    /* Packet length = 8, (Header - 1, length - 2, cmd - 1, crc - 4 bytes) */
    max_length = BSL_maxBufferSize - (uint16_t) 8U;

    /* Check if BSL is unlocked */
    if (BSLLockedStatus == BSL_STATUS_UNLOCKED) {
        /* Check if Memory readout is enabled in BCR configuration */
        if (pBSLConfig->memoryRead == BSL_CFG_MEMORY_READOUT_ENABLE) {
            /* Check if address is in the valid memory region */
            if (((BSL_RAM_bufStartAddress <= addrStart) &&
                    (addr_end <= BSL_RAM_bufEndAddress)) ||
                ((main_flash_start_address <= addrStart) &&
                    (addr_end <= main_flash_end_address)) ||
                ((info_flash_start_address <= addrStart) &&
                    (addr_end <= info_flash_end_address))) {
                /*
                 * Frame the BSL Core Response packet to send the readout
                 * data
                 */
                BSL_TXBuf[HEADER_INDEX] = PACKET_HEADER_RESPONSE;
                BSL_TXBuf[CMD_INDEX]    = BSL_MEMORY_READ_BACK;

                /*
                 * If the data to be returned is more than the maximum length
                 * that can be transferred in one packet, send the data in
                 * multiple packets
                 */
                while (addrStart < addr_end) {
                    if ((addr_end - addrStart) > (uint32_t) max_length) {
                        bytes = max_length;
                    } else {
                        bytes = addr_end - addrStart;
                    }
                    for (i = (uint16_t) 0; i < bytes; i++) {
                        BSL_TXBuf[i + (uint16_t) 4] =
                            (*((volatile uint8_t *) (addrStart)));
                        addrStart++;
                    }

                    /* Compute the Length of the packet */

                    bytes += (uint16_t) 1U;  // Adding 1 for Command byte

                    BSL_TXBuf[LEN_BYTE_L_INDEX] =
                        (uint8_t) bytes & (uint8_t) 0xFF;
                    BSL_TXBuf[LEN_BYTE_H_INDEX] =
                        (bytes & (uint16_t) 0xFF00) >> 8U;

                    /* Transmit the Data packet */
                    activePlugin->send(BSL_TXBuf, (bytes + PACKET_HEADER_LEN));
                }
            } else {
                BSL_CI_sendMessage(BSL_INVALID_MEMORY_RANGE);
            }
        } else {
            BSL_CI_sendMessage(BSL_READOUT_DISABLED);
        }
    } else {
        BSL_CI_sendMessage(BSL_LOCKED);
    }
}

uint8_t CMD_API_Factory_Reset(void)
{
    uint8_t ret = BSL_SUCCESSFUL_OPERATION;

    uint32_t passwordComparison = (uint32_t) 0;
    uint8_t index;
    uint16_t length;
    uint32_t *password;

    length = (uint16_t) BSL_RXBuf[LEN_BYTE_L_INDEX] |
             (BSL_RXBuf[LEN_BYTE_H_INDEX] << 8U);
    password = (uint32_t *) &BSL_RXBuf[PASSWORD_INDEX];

    /* Check if BSL is unlocked */
    if (BSLLockedStatus == BSL_STATUS_UNLOCKED) {
        /* Check if Factory reset is not enabled in the BCR configuration */
        if ((pBCRConfig->factoryResetMode != BCR_CFG_FACTORY_RESET_EN) &&
            (pBCRConfig->factoryResetMode != BCR_CFG_FACTORY_RESET_EN_PW)) {
            ret = BSL_FACTORY_RESET_DISABLED;
        } else {
            /*
             * If factory reset is enabled with Password, then validate the
             * password received before executing the factory reset
             */
            if (pBCRConfig->factoryResetMode == BCR_CFG_FACTORY_RESET_EN_PW) {
                if (length == FACTORY_RESET_CMD_LEN) {
                    for (index = (uint8_t) 0; index < PASSWORD_WORD_LEN;
                         index++) {
                        passwordComparison |=
                            (pBCRConfig->passwordFactoryReset[index] ^
                                password[index]);
                    }
                    /*
                     * If the password mismatches, return password error to host
                     */
                    if (passwordComparison != (uint32_t) 0) {
                        ret = BSL_FACTORY_RESET_PASSWORD_ERROR;
                    }
                } else {
                    ret = BSL_FACTORY_RESET_PASSWORD_ERROR;
                }
            }

            /*
             * If factory reset is enabled without password or if the password
             * authentication is successful, perform factory reset and
             * return the status
             */
            if (ret == BSL_SUCCESSFUL_OPERATION) {
                BSL_CI_disableCache();
                if (DL_FlashCTL_factoryReset(FLASHCTL) == false) {
                    ret = BSL_FACTORY_RESET_FAILED;
                }
                BSL_CI_enableCache();
            }
        }
    } else {
        ret = BSL_LOCKED;
    }
    return ret;
}

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
    if ((uint32_t) pBSLConfig->appRev < BSL_CI_getFlashEndAddress()) {
        /*
         * Check if the address is blank
         * Access the data only if it is not blank
         * Else return '0' as application revision
         */
        DL_FlashCTL_blankVerify(FLASHCTL, (uint32_t) pBSLConfig->appRev);
        DL_FlashCTL_waitForCmdDone(FLASHCTL);
        if (DL_FlashCTL_getFailureStatus(FLASHCTL) ==
            DL_FLASHCTL_FAIL_TYPE_VERIFY_ERROR) {
            app_revision = *(pBSLConfig->appRev);
        }
    }

    *(uint32_t *) &BSL_TXBuf[8]  = app_revision;
    *(uint16_t *) &BSL_TXBuf[12] = pluginVersion;
    *(uint16_t *) &BSL_TXBuf[14] = BSL_maxBufferSize;
    *(uint32_t *) &BSL_TXBuf[16] = BSL_RAM_bufStartAddress;
    *(uint32_t *) &BSL_TXBuf[20] = pBCRConfig->bcrConfigID;
    *(uint32_t *) &BSL_TXBuf[24] = pBSLConfig->configID;

    /* Transmit the device information */
    activePlugin->send(BSL_TXBuf, (GET_IDENTITY_RSP_LEN + PACKET_HEADER_LEN));
}

void CMD_API_Standalone_Verification(uint32_t addrStart)
{
    uint32_t main_flash_start_address = MSPM0_MAIN_FLASH_START_ADDRESS;
    uint32_t main_flash_end_address;
    uint32_t info_flash_start_address = MSPM0_INFO_FLASH_START_ADDRESS;
    uint32_t info_flash_end_address   = MSPM0_INFO_FLASH_END_ADDRESS;

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
                    (addr_end <= main_flash_end_address)) ||
                ((info_flash_start_address <= addrStart) &&
                    (addr_end <= info_flash_end_address))) {
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
    DL_SYSCTL_resetDevice(DL_SYSCTL_RESET_BOOTLOADER_EXIT);

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
    DL_CRC_setSeed32(BSL_CRC, BSL_CRC_SEED);

    /* Feed the data to CRC module */
    for (bufferIndex = (uint32_t) 0; bufferIndex < dataSize; bufferIndex++) {
        DL_CRC_feedData8(BSL_CRC, data[bufferIndex]);
    }

    /* Return the 32 bit result */
    return (DL_CRC_getResult32(BSL_CRC));
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
