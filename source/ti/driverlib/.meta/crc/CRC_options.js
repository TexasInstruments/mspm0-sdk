let Common = system.getScript("/ti/driverlib/Common.js");

/*
 * This only applies to base CRC, not CRCP
 */
let CRC_polynomial32Support = {
    MSPM0G1X0X_G3X0X: true,
    MSPM0L11XX_L13XX: true,
    MSPM0L122X_L222X: true,
    MSPM0L111X: true,
    MSPM0C110X: false,
    MSPM0GX51X: true,
    MSPM0G352X: true,
    MSPM0H321X: false,
    MSPM0C1105_C1106: false,
    MSPM0G511X: true,
    MSPM0G518X: true,
    MSPM0L211X_L112X: false,
    MSPM0L210X: false,
    MSPM0GX218_GX207: true,
    MSPM0G122X: true,
};

let index = Common.getDeviceFamily();

exports = {
    polynomial32Support  : CRC_polynomial32Support[index],
}
