let Common = system.getScript("/ti/driverlib/Common.js");

/*
 * ADC Assisted Chopping
 */
let GPAMP_adcAssistedChopping = {
    MSPM0G1X0X_G3X0X: true,
    MSPM0L11XX_L13XX: false,
    MSPM0L122X_L222X: undefined,
    MSPM0L111X      : undefined,
    MSPM0C110X      : undefined,
    MSPM0GX51X      : undefined,
    MSPM0G352X      : undefined,
    MSPM0H321X      : undefined,
    MSPM0C1105_C1106: undefined,
    MSPM0G511X      : undefined,
    MSPM0G518X      : undefined,
    MSPM0L211X_L112X: undefined,
    MSPM0L210X      : undefined,
    MSPM0GX218_GX207: undefined,
    MSPM0G122X      : undefined,
};

let index = Common.getDeviceFamily();

exports = {
    supportsADCAssistedChopping  : GPAMP_adcAssistedChopping[index],
}
