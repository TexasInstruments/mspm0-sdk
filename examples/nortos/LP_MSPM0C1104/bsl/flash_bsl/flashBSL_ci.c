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

#include <flashBSL_ci.h>
#include <flashBSL_defines.h>
#include "boot_config.h"

/*
 * Global Variables
 */

/* Buffer variables */
extern uint16_t BSL_maxBufferSize;
extern uint8_t *BSL_RXBuf;
extern uint8_t *BSL_TXBuf;

extern uint8_t BSLLockedStatus;
extern uint16_t pluginVersion;

/* Configuration structures */
extern volatile BSL_Config *pBSLConfig;
extern volatile BCR_Config *pBCRConfig;

/*
 * BSL_errorDetail is used to store the additional information in case of
 * any error. This data will be sent to BSL host as BSL_DETAILED_ERROR_RSP
 * */
static uint32_t BSL_errorDetail;

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
        case CMD_CONNECTION: {
            break;
        }
        case CMD_UNLOCK_BSL: {
            CMD_API_Unlock_BSL();
            break;
        }

#if BSL_CMD_FLASH_RANGE_ERASE
        case CMD_FLASH_RANGE_ERASE: {
            BSL_CI_sendMessage(CMD_API_Flash_Range_Erase(addr));
            break;
        }
#endif

#if BSL_CMD_MASS_ERASE
        case CMD_MASS_ERASE: {
            BSL_CI_sendMessage(CMD_API_Mass_Erase());
            break;
        }
#endif

        case CMD_PROGRAM_DATA: {
            BSL_CI_sendMessage(CMD_API_Program_Data(addr));
            break;
        }
        case CMD_PROGRAM_DATA_FAST: {
            CMD_API_Program_Data(addr);
            break;
        }
#if BSL_CMD_MEMORY_READ_BACK
        case CMD_MEMORY_READ_BACK: {
            CMD_API_Memory_Read_back(addr);
            break;
        }
#endif

        case CMD_FACTORY_RESET: {
            BSL_CI_sendMessage(CMD_API_Factory_Reset());
            break;
        }
#if BSL_CMD_GET_IDENTITY
        case CMD_GET_IDENTITY: {
            CMD_API_Get_Identity();
            break;
        }
#endif

#if BSL_CMD_STANDALONE_VERIFICATION
        case CMD_STANDALONE_VERIFICATION: {
            CMD_API_Standalone_Verification(addr);
            break;
        }
#endif
        case CMD_START_APPLICATION: {
            CMD_API_startApplication();
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

uint8_t CMD_API_Flash_Range_Erase(uint32_t addr_start)
{
    uint8_t ret        = BSL_SUCCESSFUL_OPERATION;
    bool cmdDoneStatus = true;
    uint32_t addr_end;
    uint32_t data_pointer;
    uint32_t flashEraseError = (uint32_t) DL_FLASHCTL_FAIL_TYPE_NO_FAILURE;

    addr_end     = *((uint32_t *) &BSL_RXBuf[END_ADDRESS_INDEX]);
    data_pointer = addr_start;

    /* Check if BSL is unlocked */
    if (BSLLockedStatus == BSL_STATUS_UNLOCKED) {
        /* If the address range is valid, then erase the memory range */
        if ((BSL_CI_isMainFlashAddr(addr_start, addr_end)) &&
            (addr_start <= addr_end)) {
            BSL_CI_disableCache();
            while ((data_pointer <= addr_end) &&
                   ((cmdDoneStatus == true) ||
                       (flashEraseError ==
                           (uint32_t)
                               DL_FLASHCTL_FAIL_TYPE_WRITE_ERASE_PROTECT))) {
                cmdDoneStatus =
                    cmdDoneStatus && BSL_CI_flashSectorErase(data_pointer);
                flashEraseError =
                    flashEraseError |
                    (uint32_t) DL_FlashCTL_getFailureStatus(FLASHCTL);

                data_pointer = data_pointer + MAIN_SECTOR_SIZE;
            }
            if ((cmdDoneStatus == true) ||
                (flashEraseError ==
                    (uint32_t) DL_FLASHCTL_FAIL_TYPE_WRITE_ERASE_PROTECT)) {
                cmdDoneStatus =
                    cmdDoneStatus && BSL_CI_flashSectorErase(addr_end);
                flashEraseError =
                    flashEraseError |
                    (uint32_t) DL_FlashCTL_getFailureStatus(FLASHCTL);
            }
            BSL_CI_enableCache();

            /* Check if all flash sector erase got passed */
            if (cmdDoneStatus == false) {
                ret             = BSL_FLASH_ERASE_FAILED;
                BSL_errorDetail = (uint16_t) flashEraseError;
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
        if (DL_FlashCTL_massErase(FLASHCTL) == false) {
            ret = BSL_MASS_ERASE_FAIL;
            BSL_errorDetail =
                (uint16_t) DL_FlashCTL_getFailureStatus(FLASHCTL);
        }
        BSL_CI_enableCache();
    } else {
        ret = BSL_LOCKED;
    }
    return ret;
}

uint8_t CMD_API_Program_Data(uint32_t addr_start)
{
    uint8_t ret = BSL_SUCCESSFUL_OPERATION;
    uint32_t addr_end;

    uint32_t info_flash_start_address = MSPM0_INFO_FLASH_START_ADDRESS;

    DL_FLASHCTL_REGION_SELECT flash_region;
    uint16_t length = (uint16_t) 0U;

    /* Initialize address */

    length = (uint16_t) BSL_RXBuf[1];
    length += (uint16_t) BSL_RXBuf[2] << 8U;
    length -= (uint16_t) 5U;  // Command byte (1) + Address (4)

    addr_end = addr_start + (uint32_t) length;

    if (BSLLockedStatus == BSL_STATUS_UNLOCKED) {
        /*
         * Checks if the memory address given is in the valid memory range.
         * Also checks if FACTORY reset mode is not disabled for programming Non-main Flash region
         */
        if (BSL_CI_isMainFlashAddr(addr_start, addr_end) ||
            BSL_CI_isInfoFlashAddr(addr_start, addr_end)) {
            /*
             * Since Flash allows programming for only 64 bits aligned address,
             * This condition checks if given memory address and length are 8 bytes aligned
             */
            if (((addr_start % (uint32_t) 8U) == (uint32_t) 0U) &&
                (((uint32_t) length % (uint32_t) 8U) == (uint32_t) 0U)) {
                flash_region = DL_FLASHCTL_REGION_SELECT_MAIN;

                if (info_flash_start_address <= addr_start) {
                    flash_region = DL_FLASHCTL_REGION_SELECT_NONMAIN;
                }
                BSL_CI_disableCache();
                if (DL_FlashCTL_programMemoryBlocking(FLASHCTL, addr_start,
                        (uint32_t *) &BSL_RXBuf[8],
                        (uint32_t) length / (uint32_t) 4,
                        flash_region) == false) {
                    ret = BSL_FLASH_CMD_FAILED;
                    BSL_errorDetail =
                        (uint16_t) DL_FlashCTL_getFailureStatus(FLASHCTL);
                }
                BSL_CI_enableCache();
            } else {
                ret = BSL_INVALID_ADDR_LEN_ALIGNMENT;
            }
        } else {
            ret = BSL_INVALID_MEMORY_RANGE;
        }
    } else {
        ret = BSL_LOCKED;
    }
    return ret;
}

void CMD_API_Memory_Read_back(uint32_t addr_start)
{
    uint16_t max_length;
    uint32_t length;
    uint32_t addr_end;
    uint16_t i, bytes;

    /* Initialize address */

    length   = *((uint32_t *) &BSL_RXBuf[DATA_LENGTH_INDEX]);
    addr_end = addr_start + length;

    /* Packet length = 8, (Header - 1, length - 2, cmd - 1, crc - 4 bytes) */
    max_length = BSL_maxBufferSize - (uint16_t) 8U;

    if (BSLLockedStatus == BSL_STATUS_UNLOCKED) {
        /* Check if Memory readout is enabled in BCR configuration */
        if (pBSLConfig->memoryRead == BSL_CFG_MEMORY_READOUT_ENABLE) {
            /* Check if address is in the valid memory region */
            if ((BSL_CI_isMainFlashAddr(addr_start, addr_end)) ||
                (BSL_CI_isInfoFlashAddr(addr_start, addr_end))) {
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
                while (addr_start < addr_end) {
                    if ((addr_end - addr_start) > (uint32_t) max_length) {
                        bytes = max_length;
                    } else {
                        bytes = (uint16_t)(addr_end - addr_start);
                    }
                    for (i = (uint16_t) 0; i < bytes; i++) {
                        BSL_TXBuf[i + (uint16_t) 4] =
                            (*((volatile uint8_t *) (addr_start)));
                        addr_start++;
                    }

                    /* Compute the Length of the packet */
                    bytes += (uint16_t) 1U;  // Adding 1 for Command byte

                    BSL_TXBuf[LEN_BYTE_L_INDEX] =
                        (uint8_t) bytes & (uint8_t) 0xFF;
                    BSL_TXBuf[LEN_BYTE_H_INDEX] =
                        (uint8_t)((bytes & (uint16_t) 0xFF00) >> 8U);

                    /* Transmit the Data packet */
                    ACTIVE_INTERFACE_SEND(
                        BSL_TXBuf, (bytes + PACKET_HEADER_LEN));
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
             ((uint16_t) BSL_RXBuf[LEN_BYTE_H_INDEX] << 8U);
    password = (uint32_t *) &BSL_RXBuf[PASSWORD_INDEX];

    /* Check if BSL is unlocked */
    if (BSLLockedStatus == BSL_STATUS_UNLOCKED) {
        /* Check if Factory reset is not enabled in the BCR configuration */
        if (pBCRConfig->factoryResetMode != BCR_CFG_FACTORY_RESET_EN) {
            ret = BSL_FACTORY_RESET_DISABLED;
        } else {
            /*
             * If factory reset is enabled,
             * perform factory reset and
             * return the status
             */
            if (ret == BSL_SUCCESSFUL_OPERATION) {
                BSL_CI_disableCache();
                if (DL_FlashCTL_factoryReset(FLASHCTL) == false) {
                    ret = BSL_FLASH_CMD_FAILED;
                    BSL_errorDetail =
                        (uint16_t) DL_FlashCTL_getFailureStatus(FLASHCTL);
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
        app_revision = *(pBSLConfig->appRev);
    }

    *(uint32_t *) &BSL_TXBuf[8]  = app_revision;
    *(uint16_t *) &BSL_TXBuf[12] = pluginVersion;
    *(uint16_t *) &BSL_TXBuf[14] = BSL_maxBufferSize;
    *(uint32_t *) &BSL_TXBuf[16] = BSL_SRAM_BUF_START_ADDR;
    *(uint32_t *) &BSL_TXBuf[20] = pBCRConfig->bcrConfigID;
    *(uint32_t *) &BSL_TXBuf[24] = pBSLConfig->configID;

    /* Transmit the device information */
    ACTIVE_INTERFACE_SEND(
        BSL_TXBuf, (GET_IDENTITY_RSP_LEN + PACKET_HEADER_LEN));
}

void CMD_API_Standalone_Verification(uint32_t addr_start)
{
    uint32_t main_flash_start_address = FLASHCTL_MAIN_ADDRESS;
    uint32_t main_flash_end_address;
    uint32_t info_flash_start_address = MSPM0_INFO_FLASH_START_ADDRESS;
    uint32_t info_flash_end_address   = MSPM0_INFO_FLASH_END_ADDRESS;

    uint32_t length;
    uint32_t addr_end;
    uint32_t result;

    main_flash_end_address = BSL_CI_getFlashEndAddress();

    length   = *((uint32_t *) &BSL_RXBuf[DATA_LENGTH_INDEX]);
    addr_end = addr_start + length;

    /* Check if BSL is unlocked */
    if (BSLLockedStatus == BSL_STATUS_UNLOCKED) {
        /* Check if length is valid else error is returned */
        if ((length >= MAIN_SECTOR_SIZE)) {
            /* Check if address is in the valid memory region */
            if ((BSL_CI_isMainFlashAddr(addr_start, addr_end)) ||
                (BSL_CI_isInfoFlashAddr(addr_start, addr_end))) {
                /* Frame the CRC packet */
                BSL_TXBuf[HEADER_INDEX]     = PACKET_HEADER_RESPONSE;
                BSL_TXBuf[LEN_BYTE_L_INDEX] = STANDALONE_VERIFY_RSP_LEN;
                BSL_TXBuf[LEN_BYTE_H_INDEX] = (uint8_t) 0x00;
                BSL_TXBuf[CMD_INDEX]        = STANDALONE_VERIFICATION;

                /* Compute the CRC for the given range */
                result = BSL_CI_calculateCRC((uint8_t *) addr_start, length);

                /* Store the CRC value in the response packet */
                *((uint32_t *) (&BSL_TXBuf[VERIFICATION_CRC_INDEX])) = result;

                /* Transmit the CRC packet */
                ACTIVE_INTERFACE_SEND(
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
    /* Issue reset to Start application */
    DL_SYSCTL_resetDevice(SYSCTL_RESETLEVEL_LEVEL_BOOT);

    /* Wait until reset happens */
    while ((bool) 1) {
        /* Empty Infinite Loop */
    }
}

static inline void BSL_CI_addErrorDetails(
    uint8_t message, uint16_t *responseLen, uint8_t *command)
{
    /* Check is detailed error response has to be sent to host */
    if (message == BSL_FLASH_CMD_FAILED) {
        *responseLen = DETAILED_ERROR_RSP_LEN;
        *command     = BSL_DETAILED_ERROR_RSP;

        /* Store the error details */
        BSL_TXBuf[ERROR1_INDEX] = (uint8_t) BSL_errorDetail & (uint8_t) 0xFF;
        BSL_TXBuf[ERROR2_INDEX] =
            (uint8_t)(BSL_errorDetail >> 8U) & (uint8_t) 0xFF;
    }
}

void BSL_CI_sendMessage(uint8_t message)
{
    uint16_t responseLen;
    uint8_t command;

    /* Store the default response */
    responseLen = MESSAGE_RSP_LEN;
    command     = BSL_MESSAGE_REPLY;

    BSL_CI_addErrorDetails(message, &responseLen, &command);

    /* Frame the BSL message / Detailed Error response packet */
    BSL_TXBuf[HEADER_INDEX]     = PACKET_HEADER_RESPONSE;
    BSL_TXBuf[LEN_BYTE_L_INDEX] = (uint8_t) responseLen;
    BSL_TXBuf[LEN_BYTE_H_INDEX] = (uint8_t) 0x00;
    BSL_TXBuf[CMD_INDEX]        = command;
    BSL_TXBuf[MESSAGE_INDEX]    = message;

    /* Transmit the response packet */
    (void) ACTIVE_INTERFACE_SEND(BSL_TXBuf, (responseLen + PACKET_HEADER_LEN));
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

/*
 * Function declaration
 */

uint32_t BSL_CI_getRAMEndAddress(void)
{
    uint32_t SRAM_endAddress;
    uint16_t SRAM_size_kB;

    /*
     * SRAM size stored in Factory configuration will be in units of
     * Kilo bytes
     */
    SRAM_size_kB = DL_FactoryRegion_getSRAMFlashSize();

    /* Derive End address from the start address and memory size */
    SRAM_endAddress =
        MSPM0_RAM_START_ADDRESS + ((uint32_t) SRAM_size_kB * (uint32_t) 1024U);

    /* Returns the SRAM End address */
    return SRAM_endAddress;
}

uint32_t BSL_CI_getFlashEndAddress(void)
{
    uint32_t Flash_endAddress;
    uint16_t Flash_size_kB;

    /*
     * Flash size stored in Factory configuration will be in units of
     * Kilo bytes
     */
    Flash_size_kB = DL_FactoryRegion_getMAINFlashSize();

    /* Derive End address from the start address and memory size */
    Flash_endAddress =
        FLASHCTL_MAIN_ADDRESS + ((uint32_t) Flash_size_kB * (uint32_t) 1024U);

    /* Returns the Flash End address */
    return Flash_endAddress;
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

static inline bool BSL_CI_flashSectorErase(uint32_t address)
{
    bool eraseStatus;

    DL_FlashCTL_executeClearStatus(FLASHCTL);
    DL_FlashCTL_unprotectSector(
        FLASHCTL, address, DL_FLASHCTL_REGION_SELECT_MAIN);
    DL_FlashCTL_eraseMemory(
        FLASHCTL, address, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
    eraseStatus = DL_FlashCTL_waitForCmdDone(FLASHCTL);

    return eraseStatus;
}
static inline bool BSL_CI_isMainFlashAddr(uint32_t startAddr, uint32_t endAddr)
{
    bool ret                  = false;
    uint32_t mainFlashEndAddr = BSL_CI_getFlashEndAddress();

    /* Flash start address is '0'. Unsigned Integer can never be less than '0'.
     * Hence checking if startAddr is greater than the mainFlashStartAddr is not
     * necessary. That check has been shown as an issue by MISRA-C and is removed
     */
    if (endAddr <= mainFlashEndAddr) {
        ret = true;
    }
    return ret;
}

static inline bool BSL_CI_isInfoFlashAddr(uint32_t startAddr, uint32_t endAddr)
{
    bool ret = false;

    if ((startAddr >= MSPM0_INFO_FLASH_START_ADDRESS) &&
        (endAddr <= MSPM0_INFO_FLASH_END_ADDRESS)) {
        ret = true;
    }
    return ret;
}
