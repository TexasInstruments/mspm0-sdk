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

/************************Interrupt Functions **********************************/


/************************* General functions *******************************/


/************************* Profiles functions *******************************/


/************************* Basic functions *******************************/
/* onChange functions for Basic Configuration */

/************************* Advanced functions *******************************/
/* onChange functions for Advanced Configuration */


/************************Interrupt Functions **********************************/


/************************* General functions **********************************/

/*
 *  ======== validate ========
 *  Validate this inst's configuration
 *
 *  param inst       - module instance to be validated
 *  param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{

    /* Clock Validation */
    let sysctl_module = system.modules["/ti/driverlib/SYSCTL"].$static
    /* - Device Mode */
    if(inst.mode == "device"){
        /* - ClockTree Enabled */
        if(sysctl_module.clockTreeEn){

        }
        /* - SysCtl Configuration */
        else{

        }
    }
    /* - Host Mode */
    else if(inst.mode == "host"){
        /* - ClockTree Enabled */
        if(sysctl_module.clockTreeEn){

        }
        /* - SysCtl Configuration */
        else{

        }
    }

}

let deviceConfig = {
    name: "GROUP_DEVICE",
    displayName: "Device Configuration",
    description: "",
    longDescription: ``,
    collapsed: false,
    config: [],
};

let hostConfig = {
    name: "GROUP_HOST",
    displayName: "Host Configuration",
    description: "",
    longDescription: ``,
    collapsed: false,
    // cdc hid msc
    config: [],
};

let clockConfig = {
    name: "GROUP_CLOCK",
    displayName: "Clock Configuration",
    description: "",
    longDescription: ``,
    collapsed: false,
    config: [
        {
            name        : "USBCLKSrc",
            displayName : "USB Clock Source",
            description : 'USB sample clock source selection',
            hidden      : false,
            default     : "undefined",
            getValue: (inst) => {
                return system.modules["/ti/driverlib/SYSCTL"]?.$static.USBCLKSource;
            }
        },
        {
            name: "USBCLK_Freq",
            hidden: false,
            default: "undefined",
            getValue: (inst) => {
                return system.modules["/ti/driverlib/SYSCTL"]?.$static.USBCLK_Freq_unit;
            }
        },
    ],
};

/* PROFILES CONFIGURATION */
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
                solution = inst.peripheral.$solution.peripheralName;
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
        options: [
            { name: "device",   displayName: "Device"},
            { name: "host",     displayName: "Host"},
        ],
        // onChange: onChangeMode,
    },
    deviceConfig,
    hostConfig,
    clockConfig,
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

/*
 *  ======== moduleInstances ========
 *
 */
function moduleInstances(inst){

}

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base module configuration
 */
let devSpecific = {

    config: modConfig,


    maxInstances: Common.peripheralCount("USB"),

   /* override generic requirements with  device-specific reqs (if any) */
   validate: validate,

    pinmuxRequirements: pinmuxRequirements,

    filterHardware: filterHardware,

    /* override device-specific templates */
    templates: {
        boardc: "/ti/driverlib/usb/USB.Board.c.xdt",
        boardh: "/ti/driverlib/usb/USB.Board.h.xdt",
        Call: true,
        Reset: true,
        Power: true,
        GPIO: true,
        Function: true,
    },
    /*
        Files to generate:
            tusb_config.h
            usb_descriptors.c
            usb_descriptors.h
            Family.c
    */
    modules: setRequiredModules,
    // moduleInstances: moduleInstances,
};

function setRequiredModules(inst){
    let theModules = ["Board", "SYSCTL"]

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
    let DM = {
        name:"DMPin",
        displayName:"DM Pin",
        interfaceNames:["DM"],
    };
    let DP = {
        name:"DPPin",
        displayName:"DP Pin",
        interfaceNames:["DP"],
    };

    let resources = [];

    resources.push(DM);
    resources.push(DP);

    let usb = {
        name: "peripheral",
        displayName: "USB Peripheral",
        interfaceName: "USB",
        resources: resources,
        signalTypes: {
            DMPin: ["DM"],
            DPPin: ["DP"],
        }
    };

    return ([usb]);
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
    let result = Object.assign({}, base, devSpecific);

    /* concatenate device-specific configs */
    result.config = base.config.concat(devSpecific.config);

    return (result);
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base module module */
    extend: extend
};
