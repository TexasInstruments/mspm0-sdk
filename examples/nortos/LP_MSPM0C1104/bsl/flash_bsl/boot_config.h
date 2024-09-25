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

/*
 *  ============ boot_config.h =============
 *  Configured MSPM0 NONMAIN options
 */

#ifndef boot_config_h
#define boot_config_h

#include <stdint.h>

/* clang-format off */
/* General defines */
/*! The default value used in the BCR and BSL Config structures */
#define CFG_DEFAULT_VALUE                                         (0xFFFFFFFFU)

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define PLACE_IN_MEMORY(x) __attribute__((section(x))) __attribute__((retain))
#elif defined(__GNUC__) || defined(__clang__)
#define PLACE_IN_MEMORY(x) __attribute__((section(x))) __attribute__((used))
#elif defined(__IAR_SYSTEMS_ICC__)
#define STRINGIFY(a) #a
#define PLACE_IN_MEMORY(x) _Pragma(STRINGIFY(location = x)) __root
#else
#error "Non-Main table currently not supported for this compiler"
#endif

/* UART PIN Config */
#define DEF_UART_RXD_PAD                                    ((uint8_t) 22)
#define DEF_UART_RXD_MUX                                    ((uint8_t) 2)
#define DEF_UART_TXD_PAD                                    ((uint8_t) 23)
#define DEF_UART_TXD_MUX                                    ((uint8_t) 2)

/* I2C PIN Config */
#define DEF_I2C_SDA_PAD                                     ((uint8_t) 0)
#define DEF_I2C_SDA_MUX                                     ((uint8_t) 3)
#define DEF_I2C_SCL_PAD                                     ((uint8_t) 1)
#define DEF_I2C_SCL_MUX                                     ((uint8_t) 3)
#define BSL_CFG_I2C_SLAVE_ADDRESS                           ((uint8_t) 0x48)

/* INVOCATION PIN Config */
#define DEFAULT_BSL_PIN_INVOCATION_DATA0                    ((uint8_t) 0x93)
#define DEFAULT_BSL_PIN_INVOCATION_DATA1                    ((uint8_t) 0x12)

/**
 * @brief Possible values used for BCR configuration struct
 */

/*! @enum BCR_CFG_DEBUG_ACCESS */
typedef enum
{
    /*! AHB-AP, ET-AP, PWR-AP are enabled */
    BCR_CFG_DEBUG_ACCESS_EN = 0xAABBU,
    /*! AHB-AP, ET-AP, PWR-AP are enabled with password through DSSM */
    BCR_CFG_DEBUG_ACCESS_EN_PW = 0xCCDDU,
    /*! AHB-AP, ET-AP, PWR-AP are disabled */
    BCR_CFG_DEBUG_ACCESS_DIS = 0xFFFFU,
} BCR_CFG_DEBUG_ACCESS;

/*! @enum BCR_CFG_SWDP_MODE */
typedef enum
{
    /*! SWD port access is enabled */
    BCR_CFG_SWDP_EN = 0xAABBU,
    /*! SWD port access is disabled */
    BCR_CFG_SWDP_DIS = 0xFFFFU,
} BCR_CFG_SWDP_MODE;

/*! @enum BCR_CFG_BOOTLOADER_MODE */
typedef enum
{
    /*! Enable Bootloader mode */
    BSL_CFG_BOOTLOADER_MODE_EN = 0xFFFFU,
    /*! Disable Bootloader mode */
    BSL_CFG_BOOTLOADER_MODE_DIS = 0xAABBU,
} BSL_CFG_BOOTLOADER_MODE;

/*! @enum BCR_CFG_FACTORY_RESET */
typedef enum
{
    /*! Enable Factory Reset without password */
    BCR_CFG_FACTORY_RESET_EN = 0xAABBU,
    /*! Enable Factory Reset with password */
    BCR_CFG_FACTORY_RESET_EN_PW = 0xCCDDU,
    /*! Disable Factory Reset */
    BCR_CFG_FACTORY_RESET_DIS = 0xFFFFU,
} BCR_CFG_FACTORY_RESET;

/*! @enum BCR_CFG_NON_MAIN_STATIC_PROT */
typedef enum
{
    /*! Enable static write protection for non-main memory */
    BCR_CFG_NON_MAIN_STATIC_PROT_EN = 0xFFFEU,
    /*! Disable static write protection for non-main memory */
    BCR_CFG_NON_MAIN_STATIC_PROT_DIS = 0xFFFFU,
} BCR_CFG_NON_MAIN_STATIC_PROT;

/* Bootcode user configuration structure */
typedef struct
{
    /*! Configuration signature */
    uint32_t bcrConfigID;
    /*! Enable/disable AHB-AP, ET-AP, PWR-AP.
     * One of @ref BCR_CFG_DEBUG_ACCESS */
    BCR_CFG_DEBUG_ACCESS debugAccess;
    /*! Enable/disable SWD port access. One of @ref BCR_CFG_SWDP_MODE */
    BCR_CFG_SWDP_MODE swdpMode;

    /*! The factory reset mode. One of @ref BCR_CFG_FACTORY_RESET */
    BCR_CFG_FACTORY_RESET factoryResetMode;

    /*!
     * Non Main Flash Static Write Protection  */
    uint16_t staticWriteProtectionNonMain;
    /*!
     * Programs static write protection of first 32K bytes.
     * One bit corresponds to one sector, LSB is Sector 0. Setting a bit
     * to 0 disables write, setting a bit to 1 enables write Possible values:
     *    - 0x0 to 0xFFFFFFFF */
    uint32_t staticWriteProtectionMainLow;
    /*!
     * Programs static write protection of first 32K bytes.
     * One bit corresponds to eight sectors. Setting a bit
     * to 0 disables write, setting a bit to 1 enables write Possible values:
     *    - 0x0 to 0xFFFFFFF0 */
    uint32_t staticWriteProtectionMainHigh;
} BCR_Config;

/* Structure to store the Bootloader UART and I2C Pin details. */
typedef struct
{
    uint8_t UART_RXD_pad_num;
    uint8_t UART_RXD_PF_mux_sel;
    uint8_t UART_TXD_pad_num;
    uint8_t UART_TXD_PF_mux_sel;
    uint8_t I2C_SDA_pad_num;
    uint8_t I2C_SDA_PF_mux_sel;
    uint8_t I2C_SCL_pad_num;
    uint8_t I2C_SCL_PF_mux_sel;
} BSLInterfacePins;

/* Structure to store the Bootloader invocation Pin details.
 * This structure is followed in TRIM memory where default BSL invocation
 * pin details are stored and also in Bootloader configuration memory.
 *
 * Pin_data_0
 * BIT [0-5]  :pincmIndex   -> IOMUX_SECCFG_PINCM index for the pin (0 to 63)
 * BIT [7]    :pinLevel     -> Pin state High(1)/ Low(0) which should be
 *                             considered as BSL invocation condition
 * Pin_data_1
 * BIT [0-4]  :gpioNum      -> GPIO Pin number (0 to 31)
 * BIT [5]    :gpioBase     -> GPIO Instance number or GPIO Base Index (0 or 1)
 */
typedef struct
{
    uint8_t pinData0;
    uint8_t pinData1;
} BSLInvokePin;

/**
 * @brief Possible values used for BSL configuration struct
 */

/*! Mask for BSLInvokePin.pinData0 to get IOMUX_PINCM */
#define BSL_CFG_INVOKE_PIN_PINCM_INDEX_MASK                             (0x3FU)
#define BSL_CFG_INVOKE_PIN_PINCM_INDEX_POS                                 (0U)

/*! Mask for BSLInvokePin.pinData0 to get the pin state, which determines the
 * Bootloader invocation condition) */
#define BSL_CFG_INVOKE_PIN_LEVEL_MASK                                   (0x80U)
#define BSL_CFG_INVOKE_PIN_LEVEL_POS                                       (7U)

/*! Mask for BSLInvokePin.pinData1 to get GPIO pin number */
#define BSL_CFG_INVOKE_PIN_GPIO_PIN_NUM_MASK                            (0x1FU)
#define BSL_CFG_INVOKE_PIN_GPIO_PIN_NUM_POS                                (0U)

/*! Mask for BSLInvokePin.pinData1 to get GPIO Base Index */
#define BSL_CFG_INVOKE_PIN_GPIO_PORT_NUM_MASK                           (0x20U)
#define BSL_CFG_INVOKE_PIN_GPIO_PORT_NUM_POS                               (5U)

#define BL_CFG_NRST_DIS                         ((uint16_t)0xAABB)
#define BL_CFG_NRST_EN                          ((uint16_t)0xFFFF)

#define BSL_CFG_UART_BAUDRATE_4800               ((uint16_t)0x1)
#define BSL_CFG_UART_BAUDRATE_9600               ((uint16_t)0x2)
#define BSL_CFG_UART_BAUDRATE_19200              ((uint16_t)0x3)
#define BSL_CFG_UART_BAUDRATE_38400              ((uint16_t)0x4)
#define BSL_CFG_UART_BAUDRATE_57600              ((uint16_t)0x5)
#define BSL_CFG_UART_BAUDRATE_115200             ((uint16_t)0x6)
#define BSL_CFG_UART_BAUDRATE_1000000            ((uint16_t)0x7)
#define BSL_CFG_UART_BAUDRATE_2000000            ((uint16_t)0x8)
#define BSL_CFG_UART_BAUDRATE_3000000            ((uint16_t)0x9)

/*! @enum BSL_CFG_MEMORY_READOUT */
typedef enum
{
    /*! Memory readout is enabled */
    BSL_CFG_MEMORY_READOUT_ENABLE = 0xAABBU,
    /*! Memory readout is disabled */
    BSL_CFG_MEMORY_READOUT_DISABLE = 0xFFFFU,
} BSL_CFG_MEMORY_READOUT;

/*! @enum BSL_CFG_NRST */
typedef enum
{
    /*! Memory readout is enabled */
    BSL_CFG_NRST_ENABLE = 0xFFFFU,
    /*! Memory readout is disabled */
    BSL_CFG_NRST_DISABLE = 0xAABBU,
} BSL_CFG_NRST_READOUT;

/*! @enum BSL_CFG_PLUGIN_TYPE */
typedef enum
{
    /*! Plugin type in Flash is UART */
    BSL_CFG_PLUGIN_TYPE_UART = 0x1000U,
    /*! Plugin type in Flash is I2C */
    BSL_CFG_PLUGIN_TYPE_I2C = 0x2000U,
    /*! Any other interface with valid hooks will be added to the plugin
     * list */
    BSL_CFG_PLUGIN_TYPE_ANY = 0xFFFFU,
} BSL_CFG_PLUGIN_TYPE;
/**
 * @brief Bootstrap loader (BSL) configuration structure values.
 * See BSL_CFG defines for possible values.
 */
typedef struct
{
    /*! Predetermined config signature magic ID */
    uint32_t configID;

    /*! BSL interface pins. Static Default BSL invocation Pin details are
     * as per TRIM open AREA default. */
    BSLInterfacePins interfacePins;
    /*! BSL pin invocation. Static Default BSL invocation Pin details are
     * as per TRIM open Area default. */
    BSLInvokePin pin;

    /*! Enable/disable memory readout. One of @ref BSL_CFG_MEMORY_READOUT */
    uint16_t memoryRead;

    /*! Bootloader 64-byte password */
    uint32_t password[8];

    /*! Pointer to the version information in MAIN flash. Possible values:
     *    - Valid Flash Address
     *    - All other values: Address will not be accessed */
    uint32_t *appRev;

    /*!
     * The bootloader mode. Possible values:
     *    - BSL_CFG_BOOTLOADER_MODE_EN
     *    - All other values: Bootloader mode disabled */
    uint16_t bootloaderMode;
    /*!
     * UART Baud rate for ROM BSL. Possible values:
     *    - BSL_CFG_UART_BAUDRATE_4800
     *    - BSL_CFG_UART_BAUDRATE_9600
     *    - BSL_CFG_UART_BAUDRATE_19200
     *    - BSL_CFG_UART_BAUDRATE_38400
     *    - BSL_CFG_UART_BAUDRATE_57600
     *    - BSL_CFG_UART_BAUDRATE_115200
     *    - BSL_CFG_UART_BAUDRATE_1000000
     *    - BSL_CFG_UART_BAUDRATE_2000000
     *    - BSL_CFG_UART_BAUDRATE_3000000
     *    - All other values: 9600bps */
    uint16_t uartBaudRate;
    /* I2C slave address to be used for BSL I2C communication. Any 7-bit value */
    uint16_t i2cSlaveAddress;

    /*!
     * Enable/disable NRST. Possible values:
     *    - BL_CFG_NRST_DIS
     *    - All other values: NRST Enabled */
    uint16_t disableNRST;

} BSL_Config;

/* Added for secondary_bsl to build */
extern const BSL_Config BSLConfig;
extern const BCR_Config BCRConfig;

/* clang-format on */

#endif /* boot_config_h */
