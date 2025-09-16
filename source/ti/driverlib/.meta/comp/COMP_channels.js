let Common = system.getScript("/ti/driverlib/Common.js");


// TODO [6/6/25] All internal signals need to be audited/verified

let COMP_POS_internalSignals = {
    MSPM0G1X0X_G3X0X: {
        COMP0: {
            5: "DAC12 output",
            6: "OPA1 output",
            7: "COMPy positive terminal signal",
        },
        COMP1: {
            5: "DAC12 output",
            7: "COMPy positive terminal signal",
        },
        COMP2: {
            5: "DAC12 output",
        },
    },
    MSPM0L11XX_L13XX: {
        COMP0: {
            6: "OPA1 output",
        },
    },
    MSPM0L122X_L222X: {
        COMP0: {
            6: "OPA1 output",
        },
    },
    MSPM0C110X: undefined,
    MSPM0GX51X: { // UPDATE
        COMP0: {
            7: "COMPy positive terminal signal",
        },
        COMP1: {
            7: "COMPy positive terminal signal",
        },
        COMP2: undefined,
    },
    MSPM0G352X: {
        COMP0: {
            7: "COMPy positive terminal signal",
        },
        COMP1: {
            7: "COMPy positive terminal signal",
        },
        COMP2: undefined,
    },
    MSPM0L111X: undefined,
    MSPM0H321X: undefined,
    MSPM0C1105_C1106: { // VERIFY WITH SPEC/DS
        COMP0: {
            6: "OPA1 output",
        },
    },
    MSPM0G511X: {
        COMP0: undefined,
    },
    MSPM0G518X: {
        COMP0: undefined,
    },
    MSPM0L211X_L112X: {
        COMP0: undefined,
    },
    MSPM0L210X: undefined,
}

let COMP_NEG_internalSignals = {
    MSPM0G1X0X_G3X0X: {
        COMP0: {
            5: "Temperature Sensor",
            6: "OPA0 Output",
        },
        COMP1: {
            5: "Temperature Sensor",
        },
        COMP2: {
            5: "Temperature Sensor",
        },
    },
    MSPM0L11XX_L13XX: {
        COMP0: {
            5: "Temperature Sensor",
            6: "OPA0 Output",
        },
    },
    MSPM0L122X_L222X: {
        COMP0: {
            5: "Temperature Sensor",
            6: "OPA0 Output",
        },
    },
    MSPM0C110X: undefined,
    MSPM0GX51X: { // UPDATE
        COMP0: {
            5: "Temperature Sensor",
        },
        COMP1: {
            5: "Temperature Sensor",
        },
        COMP2: undefined,
    },
    MSPM0G352X: {
        COMP0: {
            5: "Temperature Sensor",
        },
        COMP1: {
            5: "Temperature Sensor",
        },
        COMP2: undefined,
    },
    MSPM0L111X: undefined,
    MSPM0H321X: undefined,
    MSPM0C1105_C1106: { // VERIFY WITH SPEC/DS
        COMP0: {
            5: "Temperature Sensor",
            6: "OPA0 Output",
        },
    },
    MSPM0G511X: {
        COMP0: {
            5: "Temperature Sensor",
        },
    },
    MSPM0G518X: {
        COMP0: {
            5: "Temperature Sensor",
        },
    },
    MSPM0L211X_L112X: {
        COMP0: {
            5: "Temperature Sensor",
        },
    },
    MSPM0L210X: undefined,
}

/* Pin Channels */
let allChannelPins = Object.keys(system.deviceData.peripheralPins).filter(element => (element.includes("COMP")))
let allPinChannels = allChannelPins.map(s=>(s.substring(s.indexOf(".") + 1).trim()))

/* Internal Channels */
let allInternalChannels = [];
// // ORIGINAL PIECE OF CODE INHERITED FROM ADC12_InternalConnections - adapt to COMP
// for(let item of Object.keys(COMP_internalSignals[Common.getDeviceFamily()])){
//     let currentChannels = Object.keys(COMP_internalSignals[Common.getDeviceFamily()][item]).map(s=>(s.substring(s.indexOf("H") + 1).trim()))
//     allInternalChannels = allInternalChannels.concat(currentChannels);
// }


let allChannels = allPinChannels.concat(allInternalChannels);

/* Remove Duplicates */
let uniqueChannels = allChannels.filter((item,index) => allChannels.indexOf(item) === index);

/* Filter Positive/Negative Channels
 * - simplify array to only the unique channel numbers
 * - sort the resulting array
 */
let positiveChannels = uniqueChannels.filter(element => (element.includes("+"))).map(s=>(s.replace(/\D/g,''))).sort(function(a, b) {return a - b;});
let negativeChannels = uniqueChannels.filter(element => (element.includes("-"))).map(s=>(s.replace(/\D/g,''))).sort(function(a, b) {return a - b;});

exports = {
    internalPosChannels : COMP_POS_internalSignals,
    internalNegChannels : COMP_NEG_internalSignals,
    positiveChannels    : positiveChannels,
    negativeChannels    : negativeChannels,
}
