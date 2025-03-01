let Common = system.getScript("/ti/driverlib/Common.js");

let ADC12_internalSignals = {
    MSPM0G1X0X_G3X0X: {
        ADC0: {
            CH11: "Temperature Sensor",
            CH13: "OPA0 output",
            CH14: "GPAMP output",
            CH15: "Supply/Battery Monitor"
        },
        ADC1: {
            CH12: "Temperature Sensor",
            CH13: "OPA1 output",
            CH14: "GPAMP output",
            CH15: "Supply/Battery Monitor"
        }
    },
    MSPM0L11XX_L13XX: {
        ADC0: {
            CH11: "Temperature Sensor",
            CH12: "OPA0 output",
            CH13: "OPA1 output",
            CH14: "GPAMP output",
            CH15: "Supply/Battery Monitor"
        },
    },
    MSPM0L122X_L222X: {
        ADC0: {
            CH29: "Temperature Sensor",
            CH30: "VBAT Battery monitor",
            CH31: "Supply/Battery Monitor",
        },
    },
    MSPM0C110X: {
        ADC0: {
            CH11: "Temperature Sensor",
            CH15: "Supply Monitor"
        },
    },
    MSPM0GX51X: {
        ADC0: {
            CH11: "Temperature Sensor",
            CH15: "Supply Monitor",
        },
        ADC1: {
            CH15: "Supply Monitor",
        }
    },
    MSPM0L111X: {
        ADC0: {
            CH11: "Temperature Sensor",
            CH28: "Internal VREF",
            CH31: "Supply Monitor",
        },
    },
    MSPM0H321X: {
        ADC0: {
            CH28: "Temperature Sensor",
            CH29: "Internal VREF",
            CH31: "Supply Monitor",
        },
    },
    MSPM0C1105_C1106: {
        ADC0: {
            CH28: "Temperature Sensor",
            CH29: "Internal VREF",
            CH31: "Supply Monitor",
        },
    }
}

/*
 * MSPM0G1X0X_G3X0X can configure Channels 0-8,11-15
 * M0L11XX_L13XX family can configure channels 0-9, 11-15
 * M0L122X_L222X family can configure channels 0-25,29-31
 * M0C can configure channels 0-9,11,15
 * MSPM0GX51X can configure channels 0-15
 * MSPM0L111X can configure Channels 0-9, 11-14,28,31
 * MSPM0H321X can configure channels 0-26, 28, 29, 31
 */

/* Pin Channels */
let allChannelPins = Object.keys(system.deviceData.peripheralPins).filter(element => (element.includes("ADC")))
let allPinChannels = allChannelPins.map(s=>(s.substring(s.indexOf(".") + 1).trim()))

/* Internal Channels */
let allInternalChannels = [];
for(let item of Object.keys(ADC12_internalSignals[Common.getDeviceFamily()])){
    let currentChannels = Object.keys(ADC12_internalSignals[Common.getDeviceFamily()][item]).map(s=>(s.substring(s.indexOf("H") + 1).trim()))
    allInternalChannels = allInternalChannels.concat(currentChannels);
}

let allChannels = allPinChannels.concat(allInternalChannels);
let uniqueChannels = allChannels.filter((item,index) => allChannels.indexOf(item) === index);
let ADC12_channels = uniqueChannels.sort(function(a, b) {return a - b;});

exports = {
    ADC12_internalSignals: ADC12_internalSignals,
    ADC12_channels: ADC12_channels,
}
