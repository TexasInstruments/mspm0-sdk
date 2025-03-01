let Common = system.getScript("/ti/driverlib/Common.js");

//TODO: Update file whenever new devices are added

/* Some devices with CSC only have one bank, which results in fixed banks */
let SUPPORT_BANKSWAP = {
    MSPM0L122X_L222X    : true,
    MSPM0GX51X          : true,
    MSPM0L111X          : true,
    MSPM0H321X          : false,
    MSPM0C1105_C1106    : false,
}

/* Some devices with CSC have KEYSTORE IP */
let SUPPORT_KEYSTORE = {
    MSPM0L122X_L222X    : true,
    MSPM0GX51X          : true,
    MSPM0L111X          : true,
    MSPM0H321X          : false,
    MSPM0C1105_C1106    : false,
}

/* Some devices with CSC have AESADV IP */
let SUPPORT_ADVANCED_AES = {
    MSPM0L122X_L222X    : true,
    MSPM0GX51X          : true,
    MSPM0L111X          : true,
    MSPM0H321X          : false,
    MSPM0C1105_C1106    : false,
}

let index = Common.getDeviceFamily();
exports = {
    SUPPORT_BANKSWAP      : SUPPORT_BANKSWAP[index],
    SUPPORT_KEYSTORE      : SUPPORT_KEYSTORE[index],
    SUPPORT_ADVANCED_AES  : SUPPORT_ADVANCED_AES[index],
}
