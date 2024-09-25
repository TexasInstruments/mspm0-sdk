let Common = system.getScript("/ti/driverlib/Common.js");
let BSL_CFG_INVOKE_PIN_PINCM_INDEX_MASK   = {
    MSPM0G1X0X_G3X0X    : "0x3FU",
    MSPM0L11XX_L13XX    : "0x3FU",
    MSPM0L122X_L222X    : "0x7FU",
    MSPM0C110X          : undefined,
    MSPM0GX51X          : "0x7FU",
}

let BSL_CFG_INVOKE_PIN_GPIO_PORT_NUM_MASK    = {
    MSPM0G1X0X_G3X0X    : "0x20U",
    MSPM0L11XX_L13XX    : "0x20U",
    MSPM0L122X_L222X    : "0x60U",
    MSPM0C110X          : undefined,
    MSPM0GX51X          : "0x60U",
}

/* Number of words for BSL Password */
let BSL_PASSWORD_LEN    = {
    MSPM0G1X0X_G3X0X    : 8,
    MSPM0L11XX_L13XX    : 8,
    MSPM0L122X_L222X    : 8,
    MSPM0C110X          : undefined,
    MSPM0GX51X          : 8,
}

let index = Common.getDeviceFamily();
exports = {
    BSL_CFG_INVOKE_PIN_PINCM_INDEX_MASK     : BSL_CFG_INVOKE_PIN_PINCM_INDEX_MASK[index],
    BSL_CFG_INVOKE_PIN_GPIO_PORT_NUM_MASK   : BSL_CFG_INVOKE_PIN_GPIO_PORT_NUM_MASK[index],
    BSL_PASSWORD_LEN                        : BSL_PASSWORD_LEN[index],
}
