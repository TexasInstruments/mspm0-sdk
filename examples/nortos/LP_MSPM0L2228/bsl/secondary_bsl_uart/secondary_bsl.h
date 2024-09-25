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

#ifndef SECONDARY_BSL_H_
#define SECONDARY_BSL_H_

#ifdef UART_Plugin
#include "bsl_uart.h"
#endif

/*
 * NOTE: The value of BSL_SRAM_BUF_START_ADDR might change based on the
 * SRAM usage. Refer to the final map file to get the correct value.
 */
#define BSL_SRAM_BUF_START_ADDR ((uint32_t) 0x20000170)
#define BSL_STACK_SIZE ((uint32_t) 0x100)

/* Definitions for FLASH */
#define MAIN_SECTOR_SIZE ((uint32_t) 1024)
#define INFO_SECTOR_SIZE (1024)

/* Definitions for Memory range */

#define FLASHCTL_NONMAIN_ADDRESS_END ((uint32_t) 0x41C00400)
#define SRAM_ADDRESS_START ((uint32_t) 0x20000000)

#define BSL_PASSWORD_LEN ((uint8_t) 8U)
#define BSL_FACT_RST_PASSWORD_LEN ((uint8_t) 4U)
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

/* Configuration structures */
extern volatile BSL_Config *pBSLConfig;
extern volatile BCR_Config *pBCRConfig;

extern uint32_t BSL_password[BSL_PASSWORD_LEN];
extern uint32_t BSL_factoryResetPassword[BSL_FACT_RST_PASSWORD_LEN];

#ifdef UART_Plugin
#define BSL_PI_init BSL_PI_UART_init
#define BSL_PI_receive BSL_PI_UART_receive
#define BSL_PI_send BSL_PI_UART_send
#define BSL_PI_deinit BSL_PI_UART_deinit
#endif

#endif /* SECONDARY_BSL_H_ */
