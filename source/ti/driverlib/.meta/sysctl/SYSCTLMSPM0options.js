
let Common = system.getScript("/ti/driverlib/Common.js");

const LFCLKSourceOptions = {
    "MSPM0G1X0X_G3X0X": [
        { name: "LFOSC" },
        { name: "LFXT", displayName: "LF External XTAL" },
        { name: "LFCLK_IN", displayName: "LFCLK Digital In" }
    ],
    "MSPM0L11XX_L13XX": [
        { name: "LFOSC" }
    ],
};

const MCLKSourceOptions = {
    "MSPM0G1X0X_G3X0X": [
        { name: "SYSOSC" },
        { name: "LFCLK" },
        { name: "HSCLK" },
    ],
    "MSPM0L11XX_L13XX": [
        { name: "SYSOSC" },
        { name: "LFCLK" },
    ],
};

const HFCLKSourceOptions = {
    "MSPM0G1X0X_G3X0X": [
        {name: "HFXT", displayName: "HF External XTAL"},
        {name: "HFCLK_IN", displayName: "HFCLK Digital In"}
    ],
    "MSPM0L11XX_L13XX": [
        /* HSCLK is not available for these devices */
    ],
};

const EXCLKSourceOptions = {
    "MSPM0G1X0X_G3X0X": [
        { name: "SYSOSC" },
        { name: "ULPCLK" },
        { name: "LFCLK" },
        { name: "MFPCLK" },
        { name: "HFCLK" },
        { name: "SYSPLLOUT1", displayName: "SYSPLL Clock 1" },
    ],
    "MSPM0L11XX_L13XX": [
        { name: "SYSOSC" },
        { name: "ULPCLK" },
        { name: "LFCLK" },
        { name: "MFPCLK" },
    ],
};

const HSCLKSourceOptions = {
    "MSPM0G1X0X_G3X0X": [
        { name: "SYSPLLCLK0" },
        { name: "SYSPLLCLK2X" },
        { name: "HFCLK" },
    ],
    "MSPM0L11XX_L13XX": [
        /* HSCLK is not available for these devices */
    ],
};

let index = Common.getDeviceFamily();


exports = {
    LFCLKSource : LFCLKSourceOptions[index],
    MCLKSource  : MCLKSourceOptions[index],
    HFCLKSource : HFCLKSourceOptions[index],
    EXCLKSource : EXCLKSourceOptions[index],
    HSCLKSource : HSCLKSourceOptions[index],
}
