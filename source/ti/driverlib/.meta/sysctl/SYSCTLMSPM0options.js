
let Common = system.getScript("/ti/driverlib/Common.js");

const SYSOSCFreqOptions = {
    "MSPM0G1X0X_G3X0X": [
        { name: 32000000, displayName: "32 MHz" },
        { name: 4000000,  displayName: "4 MHz"  },
    ],
    "MSPM0L11XX_L13XX": [
        { name: 32000000, displayName: "32 MHz" },
        { name: 4000000,  displayName: "4 MHz"  },
    ],
    "MSPM0L122X_L222X": [
        { name: 32000000, displayName: "32 MHz" },
        { name: 4000000,  displayName: "4 MHz"  },
    ],
    "MSPM0C110X": [
        { name: 24000000, displayName: "24 MHz" },
    ],
    "MSPM0GX51X": [
        { name: 32000000, displayName: "32 MHz" },
        { name: 4000000,  displayName: "4 MHz"  },
    ],
    "MSPM0L111X": [
        { name: 32000000, displayName: "32 MHz" },
        { name: 4000000,  displayName: "4 MHz"  },
    ],
    "MSPM0H321X": [
        { name: 32000000, displayName: "32 MHz" },
        { name: 4000000,  displayName: "4 MHz"  },
    ],
    "MSPM0C1105_C1106": [
        { name: 32000000, displayName: "32 MHz" },
        { name: 4000000,  displayName: "4 MHz"  },
    ],
}

const LFCLKSourceOptions = {
    "MSPM0G1X0X_G3X0X": [
        { name: "LFOSC" },
        { name: "LFXT", displayName: "LF External XTAL" },
        { name: "LFCLK_IN", displayName: "LFCLK Digital In" }
    ],
    "MSPM0L11XX_L13XX": [
        { name: "LFOSC" }
    ],
    "MSPM0L122X_L222X": [
        { name: "LFOSC" },
        { name: "LFXT", displayName: "LF External XTAL" },
        { name: "LFCLK_IN", displayName: "LFCLK Digital In" }
    ],
    "MSPM0C110X": [
        { name: "LFOSC" },
        { name: "LFCLK_IN", displayName: "LFCLK Digital In" }
    ],
    "MSPM0GX51X": [
        { name: "LFOSC" },
        { name: "LFXT", displayName: "LF External XTAL" },
        { name: "LFCLK_IN", displayName: "LFCLK Digital In" }
    ],
    "MSPM0L111X": [
        { name: "LFOSC" },
        { name: "LFXT", displayName: "LF External XTAL" },
        { name: "LFCLK_IN", displayName: "LFCLK Digital In" }
    ],
    "MSPM0H321X": [
        { name: "LFOSC" },
        { name: "LFXT", displayName: "LF External XTAL" },
        { name: "LFCLK_IN", displayName: "LFCLK Digital In" }
    ],
    "MSPM0C1105_C1106": [
        { name: "LFOSC" },
        { name: "LFXT", displayName: "LF External XTAL" },
        { name: "LFCLK_IN", displayName: "LFCLK Digital In" }
    ],
};

const MFPCLKSourceOptions = {
    "MSPM0G1X0X_G3X0X": [
        { name: "SYSOSC" },
        { name: "HFCLK" },
    ],
    "MSPM0L11XX_L13XX": [
        { name: "SYSOSC" },
    ],
    "MSPM0L122X_L222X": [
        { name: "SYSOSC" },
        { name: "HFCLK" },
    ],
    "MSPM0C110X": [
        /* MFPCLK not present on MSPM0C devices */
        { name: "unavailable" },
    ],
    "MSPM0GX51X": [
        { name: "SYSOSC" },
        { name: "HFCLK" },
    ],
    "MSPM0L111X": [
        { name: "SYSOSC" },
        { name: "HFCLK" },
    ],
    "MSPM0H321X": [
        { name: "SYSOSC" },
        { name: "HFCLK" },
    ],
    "MSPM0C1105_C1106": [
        { name: "SYSOSC" },
        { name: "HFCLK" },
    ],
}

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
    "MSPM0L122X_L222X": [
        { name: "SYSOSC" },
        { name: "LFCLK" },
        { name: "HSCLK" },
    ],
    "MSPM0C110X": [
        { name: "SYSOSC" },
        { name: "LFCLK" },
        { name: "HSCLK" },
    ],
    "MSPM0GX51X": [
        { name: "SYSOSC" },
        { name: "LFCLK" },
        { name: "HSCLK" },
    ],
    "MSPM0L111X": [
        { name: "SYSOSC" },
        { name: "LFCLK" },
        { name: "HSCLK" },
    ],
    "MSPM0H321X": [
        { name: "SYSOSC" },
        { name: "LFCLK" },
        { name: "HSCLK" },
    ],
    "MSPM0C1105_C1106": [
        { name: "SYSOSC" },
        { name: "LFCLK" },
        { name: "HSCLK" },
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
    "MSPM0L122X_L222X": [
        {name: "HFXT", displayName: "HF External XTAL"},
        {name: "HFCLK_IN", displayName: "HFCLK Digital In"}
    ],
    "MSPM0C110X": [
        {name: "HFCLK_IN", displayName: "HFCLK Digital In"}
    ],
    "MSPM0GX51X": [
        {name: "HFXT", displayName: "HF External XTAL"},
        {name: "HFCLK_IN", displayName: "HFCLK Digital In"}
    ],
    "MSPM0L111X": [
        {name: "HFCLK_IN", displayName: "HFCLK Digital In"}
    ],
    "MSPM0H321X": [
        {name: "HFXT", displayName: "HF External XTAL"},
        {name: "HFCLK_IN", displayName: "HFCLK Digital In"}
    ],
    "MSPM0C1105_C1106": [
        {name: "HFXT", displayName: "HF External XTAL"},
        {name: "HFCLK_IN", displayName: "HFCLK Digital In"}
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
    "MSPM0L122X_L222X": [
        { name: "SYSOSC" },
        { name: "ULPCLK" },
        { name: "LFCLK" },
        { name: "MFPCLK" },
        { name: "HFCLK" },
    ],
    "MSPM0C110X": [
        { name: "SYSOSC" },
        { name: "ULPCLK" },
        { name: "LFCLK" },
        { name: "HFCLK" },
    ],
    "MSPM0GX51X": [
        { name: "SYSOSC" },
        { name: "ULPCLK" },
        { name: "LFCLK" },
        { name: "MFPCLK" },
        { name: "HFCLK" },
        { name: "SYSPLLOUT1", displayName: "SYSPLL Clock 1" },
    ],
    "MSPM0L111X": [
        { name: "SYSOSC" },
        { name: "ULPCLK" },
        { name: "LFCLK" },
        { name: "MFPCLK" },
        { name: "HFCLK" },
    ],
    "MSPM0H321X": [
        { name: "SYSOSC" },
        { name: "ULPCLK" },
        { name: "LFCLK" },
        { name: "MFPCLK" },
        { name: "HFCLK" },
    ],
    "MSPM0C1105_C1106": [
        { name: "SYSOSC" },
        { name: "ULPCLK" },
        { name: "LFCLK" },
        { name: "MFPCLK" },
        { name: "HFCLK" },
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
        { name: "unavailable "},
    ],
    "MSPM0L122X_L222X": [
        { name: "HFCLK" },
    ],
    "MSPM0C110X": [
        { name: "HFCLK" },
    ],
    "MSPM0GX51X": [
        { name: "SYSPLLCLK0" },
        { name: "SYSPLLCLK2X" },
        { name: "HFCLK" },
    ],
    "MSPM0L111X": [
        { name: "HFCLK" },
    ],
    "MSPM0H321X": [
        { name: "HFCLK" },
    ],
    "MSPM0C1105_C1106": [
        { name: "HFCLK" },
    ],
};

const BORThreshOptions = {
    "MSPM0G1X0X_G3X0X": [
        {name: "0"},
        {name: "1"},
        {name: "2"},
        {name: "3"},
    ],
    "MSPM0L11XX_L13XX": [
        {name: "0"},
        {name: "1"},
        {name: "2"},
        {name: "3"},
    ],
    "MSPM0L122X_L222X": [
        {name: "0"},
        {name: "1"},
        {name: "2"},
        {name: "3"},
    ],
    "MSPM0C110X": [
        {name: "0"},
    ],
    "MSPM0GX51X": [
        {name: "0"},
        {name: "1"},
        {name: "2"},
        {name: "3"},
    ],
    "MSPM0L111X": [
        {name: "0"},
        {name: "1"},
        {name: "2"},
        {name: "3"},
    ],
    "MSPM0H321X": [
        {name: "0"},
        {name: "1"},
        {name: "2"},
        {name: "3"},
    ],
    "MSPM0C1105_C1106": [
        {name: "0"},
        {name: "1"},
        {name: "2"},
        {name: "3"},
    ],
}

let WaitStateOptions = {
    "MSPM0G1X0X_G3X0X": [
        {name: "0"},
        {name: "1"},
        {name: "2"},
    ],
    "MSPM0L11XX_L13XX": [
        {name: "0"},
        {name: "1"},
        {name: "2"},
    ],
    "MSPM0L122X_L222X": [
        {name: "0"},
        {name: "1"},
        {name: "2"},
    ],
    "MSPM0C110X": [
        {name: "0"},
        {name: "1"},
        {name: "2"},
    ],
    "MSPM0GX51X": [
        {name: "0"},
        {name: "1"},
        {name: "2"},
    ],
    "MSPM0L111X": [
        {name: "0"},
        {name: "1"},
        {name: "2"},
    ],
    "MSPM0H321X": [
        {name: "0"},
        {name: "1"},
    ],
    "MSPM0C1105_C1106": [
        {name: "0"},
        {name: "1"},
    ],
}

let index = Common.getDeviceFamily();

exports = {
    SYSOSCFreq  : SYSOSCFreqOptions[index],
    LFCLKSource : LFCLKSourceOptions[index],
    MFPCLKSource: MFPCLKSourceOptions[index],
    MCLKSource  : MCLKSourceOptions[index],

    HFCLKSource : HFCLKSourceOptions[index],
    EXCLKSource : EXCLKSourceOptions[index],
    HSCLKSource : HSCLKSourceOptions[index],
    BORThresh   : BORThreshOptions[index],

    WaitStates  : WaitStateOptions[index],
}
