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
#ifndef flashBSL_defines_h
#define flashBSL_defines_h

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/dl_common.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
#include "boot_config.h"
#include "flashBSL_modules.h"

#define POWER_STARTUP_DELAY (16)

/*
 * User Application Address Info
 */
#define MAIN_APP_START_ADDR ((uint32_t) 0x00001800U)
#define MAIN_APP_RESET_VECTOR_ADDR ((uint32_t) 0x00001804U)

#if UART_INTERFACE
#define ACTIVE_INTERFACE_INIT BSL_PI_UART_init
#define ACTIVE_INTERFACE_SEND BSL_PI_UART_send
#define ACTIVE_INTERFACE_RECEIVE BSL_PI_UART_receive
#define ACTIVE_INTERFACE_DEINIT BSL_PI_UART_deinit
#elif I2C_INTERFACE
#define ACTIVE_INTERFACE_INIT BSL_PI_I2C_init
#define ACTIVE_INTERFACE_SEND BSL_PI_I2C_send
#define ACTIVE_INTERFACE_RECEIVE BSL_PI_I2C_receive
#define ACTIVE_INTERFACE_DEINIT BSL_PI_I2C_deinit
#endif

/* GPIO configuration - UART */
#define BSL_GPIO_UART_TX_PIN ((&BSLConfig)->interfacePins.UART_TXD_pad_num)
#define BSL_GPIO_UART_RX_PIN ((&BSLConfig)->interfacePins.UART_RXD_pad_num)

/* MUX Selection*/
#define BSL_GPIO_UART_TX ((&BSLConfig)->interfacePins.UART_RXD_PF_mux_sel)
#define BSL_GPIO_UART_RX ((&BSLConfig)->interfacePins.UART_TXD_PF_mux_sel)

/* GPIO configuration - I2C */
#define BSL_GPIO_I2C_SDA_PIN ((&BSLConfig)->interfacePins.I2C_SDA_pad_num)
#define BSL_GPIO_I2C_SCL_PIN ((&BSLConfig)->interfacePins.I2C_SCL_pad_num)
#define BSL_GPIO_I2C_SDA ((&BSLConfig)->interfacePins.I2C_SDA_PF_mux_sel)
#define BSL_GPIO_I2C_SCL ((&BSLConfig)->interfacePins.I2C_SCL_PF_mux_sel)

/* Definitions for I2C */
#define BSL_I2C (I2C0)
#define BSL_I2C_INTERRUPT_NUM (I2C0_INT_IRQn)
#define BSL_I2C_CLOCK_SOURCE (DL_I2C_CLOCK_BUSCLK)
#define BSL_I2C_TARGET_ADDRESS ((uint32_t) 0x00000048)

/* Definitions for UART */
#define BSL_UART (UART0)
#define BSL_UART_INTERRUPT_NUM (UART0_INT_IRQn)
#define BSL_UART_CLOCK_SOURCE (DL_UART_MAIN_CLOCK_BUSCLK)
#define BSL_UART_DEFAULT_BAUD ((uint32_t) 9600U)

#define BSL_UART_CLOCK ((uint32_t) 24000000U)

/* Definitions for CRC */
#define BSL_CRC (CRC)
#define BSL_CRC_SEED ((uint32_t) 0xFFFFFFFFU)

/* CRC Polynomial Definition */
#define DL_CRC_POLYNOMIAL_TYPE DL_CRC_16_POLYNOMIAL

/* Definitions for TIMER */
#define BSL_TIMER TIMG14
#define BSL_GPTIMER_INT_VEC TIMG14_INT_IRQn

/* Equivalent time for 1 count = 1/((32KHz/60)) second.
 * where,
 * 32KHz clock frequency of LFCLK
 * 60 is the Pre-scale value loaded.
 * Equivalent count value for 1 second = (24KHz/60)
 */
#define BSL_ONE_SECOND_TIMEOUT ((uint16_t) 533U)
#define BSL_TEN_SECOND_TIMEOUT ((uint16_t) 10U * (BSL_ONE_SECOND_TIMEOUT))
#define BSL_TIMER_PRE_SCALE ((uint8_t) 60U)

/* BSL Invocation Indicators */
#define BL_INVOCATION_FALSE ((uint8_t) 0x00U)
#define BL_INVOCATION_TRUE ((uint8_t) 0x01U)
#define BSL_APP_INVOCATION_MEM DL_SYSCTL_SHUTDOWN_STORAGE_BYTE_0
#define BSL_APP_INVOCATION_LOW 0x0
#define BSL_APP_INVOCATION_HIGH 0x1

/* Definitions for FLASH */
#define MSPM0_INFO_FLASH_START_ADDRESS ((uint32_t) 0x41C00000)
#define MSPM0_INFO_FLASH_END_ADDRESS ((uint32_t) 0x41C00400)
#define MSPM0_RAM_START_ADDRESS ((uint32_t) 0x20000000)
#define MAIN_SECTOR_SIZE ((uint32_t) 1024)
#define INFO_SECTOR_SIZE (1024)

/*
 * SRAM Buffer start and end address are set in compile time
 * BSL_SRAM_BUF_START_ADDR = SRAM_START_ADDRESS +
 *                              SRAM space occupied by other SRAM data
 *
 * BSL_SRAM_BUF_END_ADDR = SRAM_START_END_ADDRESS -
 *                              STACK_SIZE Occupied
 *
 * (refer 'mspm0c1104.cmd' and map file)
 * */
#define BSL_SRAM_BUF_START_ADDR (uint32_t) 0x20000140
#define BSL_SRAM_BUF_END_ADDR (uint32_t) 0x20000300

/*! Enable static write protection for First 6KB memory */

#define BCR_CFG_MAIN_STATIC_PROT_EN_BSL_SEC (0xFFFFFFC0U)

/*
 * Enum that lists the various states involved in packet reception
 */
typedef enum {
    /*
     * IDLE state indicates that the BSL is ready to accept
     * the new data packet from the Host.
     */
    RX_idle = 0x0,
    /*
     * When the Interface receives first byte of the packet, the receive state
     * machine will be moved from IDLE to REVEIVING. At this state it reads
     * the data from Host
     */
    RX_receiving = 0x1,
    /*
     * BLOCKED is a state when complete packet is received and is yet to be
     * processed. At this state any data from the Host will be ignored.
     */
    RX_blocked = 0x2

} BSL_RX_states;

#endif
