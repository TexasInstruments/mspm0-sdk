let Common = system.getScript("/ti/driverlib/Common.js");

//TODO: Update file whenever new devices are added

/* Some devices support hashed passwords */
let SUPPORT_PW_HASH = {
    MSPM0G1X0X_G3X0X    : false,
    MSPM0L11XX_L13XX    : false,
    MSPM0L122X_L222X    : true,
    MSPM0C110X          : undefined,
    MSPM0GX51X          : true,
    MSPM0L111X          : true,
    MSPM0H321X          : true,
    MSPM0C1105_C1106    : true,
}

let BCR_CONFIG_ID = {
    MSPM0G1X0X_G3X0X    : 0x00000001,
    MSPM0L11XX_L13XX    : 0x00000001,
    MSPM0L122X_L222X    : 0x01000002,
    MSPM0C110X          : 0x00000003,
    MSPM0GX51X          : 0x04000000,
    MSPM0L111X          : 0x06000000,
    MSPM0H321X          : 0x05000000,
    MSPM0C1105_C1106    : 0x05000000,
}

/* Some devices support hashing user application */
let BCR_SUPPORT_USER_APP_HASH = {
    MSPM0G1X0X_G3X0X    : false,
    MSPM0L11XX_L13XX    : false,
    MSPM0L122X_L222X    : true,
    MSPM0C110X          : false,
    MSPM0GX51X          : true,
    MSPM0L111X          : true,
    MSPM0H321X          : true,
    MSPM0C1105_C1106    : true,
}

/* Some devices have > 256kB Main Flash memory/dual bank */
let BCR_SUPPORT_DUAL_BANK = {
    MSPM0G1X0X_G3X0X    : false,
    MSPM0L11XX_L13XX    : false,
    MSPM0L122X_L222X    : false,
    MSPM0C110X          : false,
    MSPM0GX51X          : true,
    MSPM0L111X          : false,
    MSPM0H321X          : false,
    MSPM0C1105_C1106    : false,
}

/* Some devices support CSC (custom secure code) */
let BCR_SUPPORT_CSC = {
    MSPM0G1X0X_G3X0X    : false,
    MSPM0L11XX_L13XX    : false,
    MSPM0L122X_L222X    : true,
    MSPM0C110X          : false,
    MSPM0GX51X          : true,
    MSPM0L111X          : true,
    MSPM0H321X          : true,
    MSPM0C1105_C1106    : true,
}

/* Some devices support flash bank swap */
let BCR_SUPPORT_BANK_SWAP = {
    MSPM0G1X0X_G3X0X    : false,
    MSPM0L11XX_L13XX    : false,
    MSPM0L122X_L222X    : true,
    MSPM0C110X          : false,
    MSPM0GX51X          : true,
    MSPM0L111X          : true,
    MSPM0H321X          : false,
    MSPM0C1105_C1106    : false,
}

/* Some devices do not have BSL support */
let SUPPORT_BSL = {
    MSPM0G1X0X_G3X0X    : true,
    MSPM0L11XX_L13XX    : true,
    MSPM0L122X_L222X    : true,
    MSPM0C110X          : false,
    MSPM0GX51X          : true,
    MSPM0L111X          : true,
    MSPM0H321X          : true,
    MSPM0C1105_C1106    : true,
}

let BSL_CONFIG_ID = {
    MSPM0G1X0X_G3X0X    : 0x00000001,
    MSPM0L11XX_L13XX    : 0x00000001,
    MSPM0L122X_L222X    : 0x00000001,
    MSPM0C110X          : undefined,
    MSPM0GX51X          : 0x04000000,
    MSPM0L111X          : 0x06000000,
    MSPM0H321X          : 0x05000000,
    MSPM0C1105_C1106    : 0x05000000,
}

let SUPPORT_ROM_BSL = {
    MSPM0G1X0X_G3X0X    : true,
    MSPM0L11XX_L13XX    : true,
    MSPM0L122X_L222X    : true,
    MSPM0C110X          : undefined,
    MSPM0GX51X          : true,
    MSPM0L111X          : true,
    /* N1_48 devices have ROM code which places BSL into SRAM */
    MSPM0H321X          : false,
    MSPM0C1105_C1106    : false,
}

/* Default BSL UART and I2C pinCM values are different per device */
let BSL_UART_RX_PINCM = {
    MSPM0G1X0X_G3X0X    : "22",
    MSPM0L11XX_L13XX    : "23",
    MSPM0L122X_L222X    : "26",
    MSPM0C110X          : "",
    MSPM0GX51X          : "22",
    MSPM0L111X          : "22",
    MSPM0H321X          : "",
    MSPM0C1105_C1106    : "",
}

let BSL_UART_TX_PINCM = {
    MSPM0G1X0X_G3X0X    : "21",
    MSPM0L11XX_L13XX    : "24",
    MSPM0L122X_L222X    : "25",
    MSPM0C110X          : "",
    MSPM0GX51X          : "21",
    MSPM0L111X          : "21",
    MSPM0H321X          : "",
    MSPM0C1105_C1106    : "",
}

let BSL_I2C_SCL_PINCM = {
    MSPM0G1X0X_G3X0X    : "2",
    MSPM0L11XX_L13XX    : "2",
    MSPM0L122X_L222X    : "2",
    MSPM0C110X          : "",
    MSPM0GX51X          : "2",
    MSPM0L111X          : "2",
    MSPM0H321X          : "",
    MSPM0C1105_C1106    : "",
}

let BSL_I2C_SDA_PINCM = {
    MSPM0G1X0X_G3X0X    : "1",
    MSPM0L11XX_L13XX    : "1",
    MSPM0L122X_L222X    : "1",
    MSPM0C110X          : "",
    MSPM0GX51X          : "1",
    MSPM0L111X          : "1",
    MSPM0H321X          : "",
    MSPM0C1105_C1106    : "",
}

/* Default BSL GPIO invoke pin differs per device */
let BSL_INVOKE_PIN_PINCM = {
    MSPM0G1X0X_G3X0X    : 40,
    MSPM0L11XX_L13XX    : 19,
    MSPM0L122X_L222X    : 50,
    MSPM0C110X          : undefined,
    MSPM0GX51X          : 40,
    MSPM0L111X          : 40,
    MSPM0H321X          : 16,
    MSPM0C1105_C1106    : 30,
}

/* Some devices support wider range of IOMUX_PINCM values */
let BSL_CFG_INVOKE_PIN_PINCM_INDEX_MASK   = {
    MSPM0G1X0X_G3X0X    : "0x3FU",
    MSPM0L11XX_L13XX    : "0x3FU",
    MSPM0L122X_L222X    : "0x7FU",
    MSPM0C110X          : undefined,
    MSPM0GX51X          : "0x7FU",
    MSPM0L111X          : "0x3FU",
    MSPM0H321X          : "0x3FU",
    MSPM0C1105_C1106    : "0x3FU",
}

/* Some devices support more than 2 GPIO ports */
let BSL_CFG_INVOKE_PIN_GPIO_PORT_NUM_MASK    = {
    MSPM0G1X0X_G3X0X    : "0x20U",
    MSPM0L11XX_L13XX    : "0x20U",
    MSPM0L122X_L222X    : "0x60U",
    MSPM0C110X          : undefined,
    MSPM0GX51X          : "0x60U",
    MSPM0L111X          : "0x20U",
    MSPM0H321X          : "0x20U",
    MSPM0C1105_C1106    : "0x20U",

}

/* Number of words for BSL Password */
let BSL_PASSWORD_LEN    = {
    MSPM0G1X0X_G3X0X    : 8,
    MSPM0L11XX_L13XX    : 8,
    MSPM0L122X_L222X    : 8,
    MSPM0C110X          : undefined,
    MSPM0GX51X          : 8,
    MSPM0L111X          : 8,
    MSPM0H321X          : 8,
    MSPM0C1105_C1106    : 8,
}

/* Some devices do not have flash plugin support */
let BSL_SUPPORT_FLASH_PLUGIN = {
    MSPM0G1X0X_G3X0X    : true,
    MSPM0L11XX_L13XX    : true,
    MSPM0L122X_L222X    : true,
    MSPM0C110X          : undefined,
    MSPM0GX51X          : true,
    MSPM0L111X          : true,
    MSPM0H321X          : false,
    MSPM0C1105_C1106    : false,
}

/* Some devices have UART baud rate support */
let BSL_SUPPORT_UART_BAUD = {
    MSPM0G1X0X_G3X0X    : false,
    MSPM0L11XX_L13XX    : false,
    MSPM0L122X_L222X    : false,
    MSPM0C110X          : undefined,
    MSPM0GX51X          : true,
    MSPM0L111X          : true,
    MSPM0H321X          : true,
    MSPM0C1105_C1106    : true,
}

/* Some devices support disabling NRST */
let BSL_DISABLE_NRST = {
    MSPM0G1X0X_G3X0X    : false,
    MSPM0L11XX_L13XX    : false,
    MSPM0L122X_L222X    : false,
    MSPM0C110X          : undefined,
    MSPM0GX51X          : false,
    MSPM0L111X          : true,
    /*
     * For no Flash BSL devices, the field exists in the struct, but the value
     * is reserved
     */
    MSPM0H321X          : false,
    MSPM0C1105_C1106    : false,
}

let index = Common.getDeviceFamily();
exports = {
    SUPPORT_PW_HASH                         : SUPPORT_PW_HASH[index],
    BCR_CONFIG_ID                           : BCR_CONFIG_ID[index],
    BCR_SUPPORT_USER_APP_HASH               : BCR_SUPPORT_USER_APP_HASH[index],
    BCR_SUPPORT_DUAL_BANK                   : BCR_SUPPORT_DUAL_BANK[index],
    BCR_SUPPORT_CSC                         : BCR_SUPPORT_CSC[index],
    BCR_SUPPORT_BANK_SWAP                   : BCR_SUPPORT_BANK_SWAP[index],
    SUPPORT_BSL                             : SUPPORT_BSL[index],
    BSL_CONFIG_ID                           : BSL_CONFIG_ID[index],
    SUPPORT_ROM_BSL                         : SUPPORT_ROM_BSL[index],
    BSL_UART_RX_PINCM                       : BSL_UART_RX_PINCM[index],
    BSL_UART_TX_PINCM                       : BSL_UART_TX_PINCM[index],
    BSL_I2C_SCL_PINCM                       : BSL_I2C_SCL_PINCM[index],
    BSL_I2C_SDA_PINCM                       : BSL_I2C_SDA_PINCM[index],
    BSL_INVOKE_PIN_PINCM                    : BSL_INVOKE_PIN_PINCM[index],
    BSL_CFG_INVOKE_PIN_PINCM_INDEX_MASK     : BSL_CFG_INVOKE_PIN_PINCM_INDEX_MASK[index],
    BSL_CFG_INVOKE_PIN_GPIO_PORT_NUM_MASK   : BSL_CFG_INVOKE_PIN_GPIO_PORT_NUM_MASK[index],
    BSL_PASSWORD_LEN                        : BSL_PASSWORD_LEN[index],
    BSL_SUPPORT_FLASH_PLUGIN                : BSL_SUPPORT_FLASH_PLUGIN[index],
    BSL_SUPPORT_UART_BAUD                   : BSL_SUPPORT_UART_BAUD[index],
    BSL_DISABLE_NRST                        : BSL_DISABLE_NRST[index],
}
