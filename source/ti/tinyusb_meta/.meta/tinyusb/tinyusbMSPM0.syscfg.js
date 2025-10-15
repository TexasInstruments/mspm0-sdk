/*
 * Copyright (c) 2023 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 *  ======== USBMSPM0.syscfg.js ========
 */

"use strict";

/* get Common /ti/driverlib utility functions */
let Common = system.getScript("/ti/driverlib/Common.js");
let tusbCommon = system.getScript("/ti/tinyusb_meta/Common.js");

/************************On Change Functions **********************************/
function onChangeMode(inst,ui){
    if(inst.generateDescriptors){
        if(inst.mode == "device"){
            // Device Groups
            system.utils.showGroupConfig("GROUP_DEVICE", inst, ui);
            // Host Groups
            system.utils.hideGroupConfig("GROUP_HOST", inst, ui);
        }
        else if(inst.mode=="host"){
            // Device Groups
            system.utils.hideGroupConfig("GROUP_DEVICE", inst, ui);
            // Host Groups
            system.utils.showGroupConfig("GROUP_HOST", inst, ui);
        }
    }
    else{
        system.utils.hideGroupConfig("GROUP_DEVICE", inst, ui);
        system.utils.hideGroupConfig("GROUP_HOST", inst, ui);
    }

    ui.configPID.hidden = !inst.generateDescriptors || inst.mode === "host";
    ui.pidVal.hidden = !inst.generateDescriptors || inst.mode === "host" || !inst.configPID;
    ui.vidVal.hidden = !inst.generateDescriptors || inst.mode === "host";

    ui.strManufacturer.hidden = !inst.generateDescriptors || inst.mode === "host";
    ui.strProduct.hidden = !inst.generateDescriptors || inst.mode === "host";

    // Host Configurables
    ui.enumerationBuffer.hidden = !inst.generateDescriptors || inst.mode === "device";
}
/************************Validation Functions **********************************/
/*
 *  ======== validate ========
 *  Validate this inst's configuration
 *
 *  param inst       - module instance to be validated
 *  param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    /* SysTick Validation */
    if(inst.configureSysTick){
        let systickMod = system.modules["/ti/driverlib/SYSTICK"].$static;
        if(!systickMod.periodEnable){
            validation.logError("SysTick period must be enabled", inst, ["configureSysTick"]);
        }
        else if(systickMod.calcPeriod != "1.00 ms"){
            validation.logError("SysTick period must be 1.00 ms", inst, ["configureSysTick"]);
        }
    }

    /* String Validation */
    if(!Common.isValidInputStr(inst.strManufacturer)){
        validation.logError("Invalid String", inst, "strManufacturer");
    }
    if(!Common.isValidInputStr(inst.strProduct)){
        validation.logError("Invalid String", inst, "strProduct");
    }

    // Disablin Configuration & Code Generation
    if(!inst.generateDescriptors){
        validation.logInfo("TinyUSB Configuration and Code Generation disabled. Only peripheral dependencies are being configured.", inst, "generateDescriptors");
    }
}
/************************Configurables **********************************/
/* Class Configurations */
/* CDC */
let devCDC_buffConfig = [
    {
        name: "DEV_CDC_RX_BUFSIZE", // change names to be specific to the class
        displayName: "RX FIFO size",
        description: "",
        longDescription: ``,
        default: 64,
        isInteger: true,
        range: [0,64],
    },
    {
        name: "DEV_CDC_TX_BUFSIZE",
        displayName: "TX FIFO size",
        description: "",
        longDescription: ``,
        default: 64,
        isInteger: true,
        range: [0,64],
    },
    {
        name: "DEV_CDC_EP_BUFSIZE",
        displayName: "Endpoint transfer buffer size",
        description: "",
        longDescription: ``,
        default: 64,
        isInteger: true,
        range: [0,64],
    },
]
let devCDCConfig = [].concat(devCDC_buffConfig);
let hostCDC_buffConfig = [
    {
        name: "HOST_CDC_RX_BUFSIZE",
        displayName: "RX FIFO size",
        description: "",
        longDescription: ``,
        default: 64,
        isInteger: true,
        range: [0,64],
        hidden: true,
    },
    {
        name: "HOST_CDC_TX_BUFSIZE",
        displayName: "TX FIFO size",
        description: "",
        longDescription: ``,
        default: 64,
        isInteger: true,
        range: [0,64],
        hidden: true,
    },
]
let hostCDCConfig = [].concat(hostCDC_buffConfig);
/* HID */
let devHID_buffConfig = [
    {
        name: "DEV_HID_EP_BUFSIZE",
        displayName: "Endpoint transfer buffer size",
        description: "",
        longDescription: ``,
        default: 16,
        isInteger: true,
        range: [0,64],
    },
]
let devHIDConfig = [].concat(devHID_buffConfig);
let hostHID_buffConfig = [
    {
        name: "HOST_HID_EP_BUFSIZE",
        displayName: "Endpoint transfer buffer size",
        description: "",
        longDescription: ``,
        default: 16,
        isInteger: true,
        range: [0,64],
        hidden: true,
    },
]
let hostHIDConfig = [].concat(hostHID_buffConfig);
/* AUDIO */
let devAudio_buffConfig = [
    {
        name: "DEV_AUDIO_EP_BUFSIZE",
        displayName: "Endpoint transfer buffer size",
        description: "",
        longDescription: ``,
        default: 64,
        isInteger: true,
        range: [0,64],
    },
]
let devAudio_funcConfig = [
    {
        name: "enableSpeakerFeedback",
        displayName: "Enable Speaker Feedback",
        default: false,
    },
    {
        name: "enableEncoding",
        displayName: "Enable Audio Encoding",
        default: false,
        onChange: (inst, ui) => {
            ui.enableTypeIEncoding.hidden = !inst.enableEncoding;
        }
    },
    {
        name: "enableTypeIEncoding",
        displayName: "Enable Type-I Encoding",
        default: false,
        hidden: true,
    },
    {
        name: "enableDecoding",
        displayName: "Enable Audio Decoding",
        default: false,
        onChange: (inst, ui) => {
            ui.enableTypeIDecoding.hidden = !inst.enableDecoding;
        }
    },
    {
        name: "enableTypeIDecoding",
        displayName: "Enable Type-I Decocoding",
        default: false,
        hidden: true,
    }
];
let devAudioConfig = [].concat(devAudio_buffConfig,devAudio_funcConfig);
/* MSC */
let devMSC_buffConfig = [
    {
        name: "DEV_MSC_EP_BUFSIZE",
        displayName: "Endpoint transfer buffer size",
        description: "",
        longDescription: ``,
        default: 64,
        isInteger: true,
        range: [0,512],
    },
]
let devMSCConfig = [].concat(devMSC_buffConfig);
let hostMSC_buffConfig = [];
let hostMSCConfig = [].concat(hostMSC_buffConfig);
let devBillboardConfig = [];
/* Class Configuration Groups */
let devCDCGroup = {
    name: "GROUP_DEV_CDC",
    displayName: "CDC Configuration",
    description: "",
    longDescription: `
USB communications device class (CDC)`,
    collapsed: true,
    config: devCDCConfig,
};

let devHIDGroup = {
    name: "GROUP_DEV_HID",
    displayName: "HID Configuration",
    description: "",
    longDescription: `
USB human interface device class (HID)`,
    collapsed: true,
    config: devHIDConfig,
};

let devMSCGroup = {
    name: "GROUP_DEV_MSC",
    displayName: "MSC Configuration",
    description: "",
    longDescription: `
The USB mass storage device class (MSC)`,
    collapsed: true,
    config: devMSCConfig,
};

let devAUDIOGroup = {
    name: "GROUP_DEV_AUDIO",
    displayName: "Audio Configuration",
    description: "",
    longDescription: ``,
    collapsed: true,
    config: devAudioConfig,
};

let devBILLBOARDGroup = {
    name: "GROUP_DEV_BILLBOARD",
    displayName: "Billboard Configuration",
    description: "",
    longDescription: ``,
    collapsed: true,
    config: devBillboardConfig,
};


let hostCDCGroup = {
    name: "GROUP_HOST_CDC",
    displayName: "CDC Configuration",
    description: "",
    longDescription: `
USB communications device class (CDC)`,
    collapsed: true,
    config: hostCDCConfig,
};

let hostHIDGroup = {
    name: "GROUP_HOST_HID",
    displayName: "HID Configuration",
    description: "",
    longDescription: `
USB human interface device class (HID)`,
    collapsed: true,
    config: hostHIDConfig,
};

let hostMSCGroup = {
    name: "GROUP_HOST_MSC",
    displayName: "MSC Configuration",
    description: "",
    longDescription: `
The USB mass storage device class (MSC)`,
    collapsed: true,
    config: hostMSCConfig,
};


/* Mode Configuration Groups */
let deviceConfig = {
    name: "GROUP_DEVICE",
    displayName: "Device Configuration",
    description: "",
    longDescription: ``,
    collapsed: false,
    config: [
        devCDCGroup,
        devHIDGroup,
        devAUDIOGroup,
        devMSCGroup,
        devBILLBOARDGroup,
    ],
};

let hostConfig = {
    name: "GROUP_HOST",
    displayName: "Host Configuration",
    description: "",
    longDescription: ``,
    collapsed: false,
    // cdc hid msc
    config: [
        hostCDCGroup,
        hostHIDGroup,
        hostMSCGroup,
    ],
};

/* Peripheral Dependencies Configuration Group */
let uartConfig = {
    name: "GROUP_UART",
    displayName: "UART Configuration",
    description: "",
    longDescription: ``,
    collapsed: true,
    config: [
        {
            name: "configureUART",
            displayName: "Configure UART Dependencies",
            longDescription: `
Enabling this option will configure required UART dependencies for TinyUSB
configuration. If the user disables this option, they should take care to handle
 any dependencies on the application code`,
            description: "",
            default: true,
        }
    ],
}

let gpioConfig = {
    name: "GROUP_GPIO",
    displayName: "GPIO Configuration",
    description: "",
    longDescription: ``,
    collapsed: true,
    config: [
        {
            name: "configureGPIO",
            displayName: "Configure GPIO Dependencies",
            description: "",
            longDescription: `
Enabling this option will configure required GPIO dependencies for TinyUSB
configuration. If the user disables this option, they should take care to handle
 any dependencies on the application code`,
            default: true,
        }
    ],
}

let systickConfig = {
    name: "GROUP_SYSTICK",
    displayName: "SysTick Configuration",
    description: "",
    longDescription: ``,
    collapsed: true,
    config: [
        {
            name: "configureSysTick",
            displayName: "Configure SysTick Dependencies",
            description: "",
            longDescription: `
Enabling this option will configure required SysTick dependencies for TinyUSB
configuration. If the user disables this option, they should take care to handle
 any dependencies on the application code`,
            default: true,
        }
    ],
}

/* Main Module Config */
let modConfig = [
    /* Show selected peripheral below instance name */
    {
        name: "selectedInstance",
        displayName: "Selected Peripheral",
        default: "",
        longDescription: `Refer to the PinMux section for peripheral selection. Chosen instance name is not indicative of active peripheral.`,
        getValue: (inst) => {
            let solution = "";
            try{
                solution = inst.associated_USB.peripheral.$solution.peripheralName;
            }catch (e) {
                // do nothing
            }
            return solution;
        }
    },
    {
        name: "mode",
        displayName: "Mode",
        default: "device",
        longDescription: `
Configure USB to act as either device or host.`,
        options: [
            { name: "device",   displayName: "Device"},
            { name: "host",     displayName: "Host"},
        ],
        onChange: onChangeMode,
    },
    {
        name: "generateDescriptors",
        displayName: "Generate USB Descriptors",
        default: true,
        longDescription: `
While this option is enabled, SysConfig will generate the following files:
\n* tusb_config.h
\n* usb_descriptors.c (if device mode is configured)
\n* usb_descriptors.h (if device mode is configured)`,
        onChange: onChangeMode,
    },
    {
        name: "configPID",
        displayName: "Configure PID Value",
        default: false,
        onChange: onChangeMode,
    },
    {
        name: "pidVal",
        displayName: "PID Value",
        default: 0,
        description: "Product ID",
        longDescription: `Product ID`,
        isInteger: true,
        displayFormat: "hex",
        hidden: true,
        range: [0, 0xFFFF],
    },
    {
        name: "vidVal",
        displayName: "VID Value",
        description: "Vendor ID",
        longDescription: `Vendor ID`,
        default: 0xCafe,
        isInteger: true,
        displayFormat: "hex",
        range: [0, 0xFFFF],
    },
    {
        name: "bcdVal",
        displayName: "BCD Value",
        description: "Vendor ID",
        longDescription: `Vendor ID`,
        default: 0x200,
        isInteger: true,
        displayFormat: "hex",
        range: [0, 0xFFFF],
    },
    {
        name: "strManufacturer",
        displayName: "Manufacturer",
        longDescription: `Manufacturer Name for descriptor string`,
        default: "TinyUSB",
    },
    {
        name: "strProduct",
        displayName: "Product",
        longDescription: `Product Name for descriptor string`,
        default: "TinyUSB Device",
    },
    {
        name: "enumerationBuffer",
        displayName: "Enumeration Buffer Size",
        default: 256,
        isInteger: true,
        range: [0, 256],
        hidden: true,
    },
    deviceConfig,
    hostConfig,
    uartConfig,
    gpioConfig,
    systickConfig,
]

let basicConfig = []

modConfig = modConfig.concat([
    {
        name: "GROUP_BASIC",
        displayName: "Basic Configuration",
        description: "",
        longDescription: ``,
        collapsed: false,
        config: basicConfig,
    }
])

modConfig = modConfig.concat([
    {
        name: "GROUP_ADVANCED",
        displayName: "Advanced Configuration",
        description: "",
        longDescription: ``,
        collapsed: false,
        config: [

        ],
    },
])

modConfig = modConfig.concat([
    {
        name: "GROUP_USB",
        displayName: "USB Peripheral Configuration",
        description: "",
        longDescription: ``,
        collapsed: true,
        config: [

        ],
    },
])

/*
 *  ======== moduleInstances ========
 *
 */
function moduleInstances(inst) {
    let modInstances = new Array();
    let longDescription = ``;

    let modInst = undefined;
    /* Device Module Instances */
    if(inst.generateDescriptors && inst.mode == "device"){
        modInst = {
            name: "associated_devCDC",
            displayName: "CDC Instance Configuration",
            useArray: true,
            defaultInstanceCount: 0,
            minInstanceCount: 0,
            longDescription: longDescription,
            collapsed: false,
            moduleName: "/ti/tinyusb_meta/tinyusb/USB_devCDC",
            hidden: false,
            args: {

            },
            requiredArgs: {
                epBufferSize: inst.DEV_CDC_EP_BUFSIZE,
            },
            group: "GROUP_DEV_CDC",

        };
        modInstances.push(modInst);
        modInst = {
            name: "associated_devHID",
            displayName: "HID Instance Configuration",
            useArray: true,
            defaultInstanceCount: 0,
            minInstanceCount: 0,
            longDescription: longDescription,
            collapsed: false,
            moduleName: "/ti/tinyusb_meta/tinyusb/USB_devHID",
            hidden: false,
            args: {

            },
            requiredArgs: {
                epBufferSize: inst.DEV_HID_EP_BUFSIZE,
            },
            group: "GROUP_DEV_HID",

        };
        modInstances.push(modInst);
        modInst = {
            name: "associated_devAudio",
            displayName: "Audio Instance Configuration",
            useArray: true,
            defaultInstanceCount: 0,
            minInstanceCount: 0,
            longDescription: longDescription,
            collapsed: false,
            moduleName: "/ti/tinyusb_meta/tinyusb/USB_devAudio",
            hidden: false,
            args: {

            },
            requiredArgs: {
                epBufferSize: inst.DEV_AUDIO_EP_BUFSIZE,
                enableFeedback: inst.enableSpeakerFeedback,
                enableEncoding: inst.enableEncoding,
                enableDecoding: inst.enableDecoding,
            },
            group: "GROUP_DEV_AUDIO",

        };
        modInstances.push(modInst);
        modInst = {
            name: "associated_devMSC",
            displayName: "MSC Instance Configuration",
            useArray: true,
            defaultInstanceCount: 0,
            minInstanceCount: 0,
            longDescription: longDescription,
            collapsed: false,
            moduleName: "/ti/tinyusb_meta/tinyusb/USB_devMSC",
            hidden: false,
            args: {

            },
            requiredArgs: {
                epBufferSize: inst.DEV_MSC_EP_BUFSIZE,
            },
            group: "GROUP_DEV_MSC",

        };
        modInstances.push(modInst);
        modInst = {
            name: "associated_devBillboard",
            displayName: "Billboard Instance Configuration",
            useArray: true,
            defaultInstanceCount: 0,
            minInstanceCount: 0,
            longDescription: longDescription,
            collapsed: false,
            moduleName: "/ti/tinyusb_meta/tinyusb/USB_devBillboard",
            hidden: false,
            args: {

            },
            requiredArgs: {

            },
            group: "GROUP_DEV_BILLBOARD",

        };
        modInstances.push(modInst);
    }

    /* Host Module Instances */
    if(inst.generateDescriptors && inst.mode == "host"){
        /* Host CDC Configuration*/
        modInst = {
            name: "associated_hostCDC",
            displayName: "CDC Instance Configuration",
            useArray: true,
            defaultInstanceCount: 0,
            minInstanceCount: 0,
            longDescription: longDescription,
            collapsed: false,
            moduleName: "/ti/tinyusb_meta/tinyusb/USB_hostCDC",
            hidden: false,
            args: {

            },
            requiredArgs: {

            },
            group: "GROUP_HOST_CDC",

        };
        modInstances.push(modInst);
        /* Host HID Configuration */
        modInst = {
            name: "associated_hostHID",
            displayName: "HID Instance Configuration",
            useArray: true,
            defaultInstanceCount: 0,
            minInstanceCount: 0,
            longDescription: longDescription,
            collapsed: false,
            moduleName: "/ti/tinyusb_meta/tinyusb/USB_hostHID",
            hidden: false,
            args: {

            },
            requiredArgs: {

            },
            group: "GROUP_HOST_HID",

        };
        modInstances.push(modInst);
        /* Host MSC Configuration */
        modInst = {
            name: "associated_hostMSC",
            displayName: "MSC Instance Configuration",
            useArray: true,
            defaultInstanceCount: 0,
            minInstanceCount: 0,
            longDescription: longDescription,
            collapsed: false,
            moduleName: "/ti/tinyusb_meta/tinyusb/USB_hostMSC",
            hidden: false,
            args: {

            },
            requiredArgs: {

            },
            group: "GROUP_HOST_MSC",

        };
        modInstances.push(modInst);
    }

    /* USB Module */
    modInst = {
        name: "associated_USB",
        displayName: "USB Configuration",
        moduleName: "/ti/driverlib/USB",
        longDescription: longDescription,
        collapsed: false,
        hidden: false,
        args: {

        },
        requiredArgs: {
            mode: inst.mode,
        },
        group: "GROUP_USB",

    };
    modInstances.push(modInst);

    if(inst.configureGPIO){
        let modInst = {
            name: "associatedGPIO_LED",
            displayName: "GPIO Peripheral Configuration (LED)",
            longDescription: longDescription,
            collapsed: true,
            moduleName: "/ti/driverlib/GPIO",
            group: "GROUP_GPIO",
            hidden: false,
        };
        modInstances.push(modInst);
        modInst = {
            name: "associatedGPIO_BUTTON",
            displayName: "GPIO Peripheral Configuration (Button)",
            longDescription: longDescription,
            collapsed: true,
            moduleName: "/ti/driverlib/GPIO",
            group: "GROUP_GPIO",
            hidden: false,
        };
        modInstances.push(modInst);
    }

    return (modInstances);
}

/*
 *  ======== sharedModuleInstances ========
 *
 */
function sharedModuleInstances(inst) {
    let modInstances = [];
    let longDescription = ``;
    if(inst.configureUART){
        let modInst = {
            name: "associatedUART",
            displayName: "UART Peripheral Configuration",
            longDescription: longDescription,
            collapsed: true,
            moduleName: "/ti/driverlib/UART",
            group: "GROUP_UART",
            hidden: false,
        };
        modInstances.push(modInst);
    }

    return (modInstances);
}

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base module configuration
 */
let devSpecific = {

    config: modConfig,

   /* override generic requirements with  device-specific reqs (if any) */
   validate: validate,

    // pinmuxRequirements: pinmuxRequirements,

    filterHardware: filterHardware,

    /*
        Files to generate:
            tusb_config.h
            usb_descriptors.c
            usb_descriptors.h
    */
    modules: setRequiredModules,

    moduleInstances: moduleInstances,

    sharedModuleInstances: sharedModuleInstances,
};

function setRequiredModules(inst){
    let theModules = ["Board", "SYSCTL"]
    if(inst.configureSysTick){
        theModules.push("SYSTICK");
    }

    let kwargs = theModules;
    let modArray = [];

    for (let args = kwargs.length - 1; args >= 0; args--) {
        let modPath = kwargs[args];
        if (modPath.indexOf('/') == -1) {
            modPath = "/ti/driverlib/" + modPath;
        }
        modArray.push({
            name      : modPath.substring(modPath.lastIndexOf('/') + 1),
            moduleName: modPath,
            hidden    : true
        });
    }

    if(inst.generateDescriptors){
        let mod = {
            name: "genTUSBFiles",
            moduleName: "/ti/tinyusb_meta/tinyusb_codegenTrigger",
        }
        modArray.push(mod);
        if(inst.mode == "device"){
            let mod = {
                name: "genDescriptors",
                moduleName: "/ti/tinyusb_meta/tinyusb_descriptorTrigger",
            }
            modArray.push(mod);
        }
    }

    return modArray;
}

/*  ======== pinmuxRequirements ========
 *  Returns peripheral pin requirements of the specified instance
 *
 *  param inst    - a fully configured module instance
 *
 *  returns req[] - an array of pin requirements needed by inst
 */
function pinmuxRequirements(inst)
{

}

/*
 *  ========= filterHardware ========
 *  Check 'component' signals for compatibility with module
 *
 *  param component - hardware object describing signals and
 *                    resources they're attached to
 *  returns Boolean indicating whether or not to allow the component to
 *           be assigned to an instance's $hardware config
 */
function filterHardware(component)
{
    return true;
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic module module to
 *  allow us to augment/override as needed for the MSP432.
 */
function extend(base)
{
    /* merge and overwrite base module attributes */
    let result = Object.assign({}, base.moduleStatic, devSpecific);

    /* concatenate device-specific configs */
    result.config = base.moduleStatic.config.concat(devSpecific.config);
    base.moduleStatic = result;

    return (base);
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base module module */
    extend: extend
};
