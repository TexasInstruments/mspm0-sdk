let Common = system.getScript("/ti/driverlib/Common.js");
let peripherals = Object.keys(system.deviceData.peripherals);

//TODO: Update file whenever new devices are added

/* Some devices with CSC only have one bank, which results in fixed banks */
let SUPPORT_BANKSWAP = {
    MSPM0L122X_L222X    : true,
    MSPM0GX51X          : true,
    MSPM0G352X          : true,
    MSPM0L111X          : true,
    MSPM0H321X          : false,
    MSPM0C1105_C1106    : false,
    MSPM0G511X          : true,
    MSPM0G518X          : true,
    MSPM0L211X_L112X    : false,
    MSPM0L210X          : false,
}

/* Some devices with CSC have KEYSTORE IP */
let SUPPORT_KEYSTORE = /KEYSTORECTL/.test(peripherals);

/* Some devices with CSC have AESADV IP */
let SUPPORT_ADVANCED_AES = /AESADV/.test(peripherals);

let index = Common.getDeviceFamily();
exports = {
    SUPPORT_BANKSWAP      : SUPPORT_BANKSWAP[index],
    SUPPORT_KEYSTORE      : SUPPORT_KEYSTORE,
    SUPPORT_ADVANCED_AES  : SUPPORT_ADVANCED_AES,
}
