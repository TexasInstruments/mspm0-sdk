/*
 * Copyright (c) 2018-2019, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== DMAChannel.syscfg.js ========
 */
"use strict";

/* get Common /ti/driverlib utility functions */
let Common = system.getScript("/ti/driverlib/Common.js");
let tusbCommon = system.getScript("/ti/tinyusb_meta/Common.js");
/* Determine channe availability depending on device */

/*
 *
 */
function validate(inst, validation){
    tusbCommon.validateClassStr(inst,validation);

    // Must at least enable one mode
    if(!(inst.enableKeyboard || inst.enableMouse || inst.enableConsumer || inst.enableGamepad || inst.enableGenericInOut)){
        validation.logError("At least one mode must be enabled", inst);
    }

    /* Unique Report ID validation */
    // Report IDs must be unique across all HID instances
    // TODO: improvement - add more information to error message (ie: where are there duplicates?)
    let reportIDs = [];
    let tusbMod = system.modules["/ti/tinyusb_meta/tinyusb"]?.$static;
    for(var myClass of tusbMod.associated_devHID){
        if(myClass.enableKeyboard){ reportIDs.push(myClass.keyboardID); }
        if(myClass.enableMouse){ reportIDs.push(myClass.mouseID); }
        if(myClass.enableConsumer){ reportIDs.push(myClass.consumerID); }
        if(myClass.enableGamepad){ reportIDs.push(myClass.gamepadID); }
        if(myClass.enableGenericInOut){ reportIDs.push(myClass.genericInOutID); }
    }
    if(inst.enableKeyboard){
        if(reportIDs.filter(x => x === inst.keyboardID).length>1){
            validation.logError("Report IDs must be unique", inst, ["keyboardID"]);
        }
    }
    if(inst.enableMouse){
        if(reportIDs.filter(x => x === inst.mouseID).length>1){
            validation.logError("Report IDs must be unique", inst, ["mouseID"]);
        }
    }
    if(inst.enableConsumer){
        if(reportIDs.filter(x => x === inst.consumerID).length>1){
            validation.logError("Report IDs must be unique", inst, ["consumerID"]);
        }
    }
    if(inst.enableGamepad){
        if(reportIDs.filter(x => x === inst.gamepadID).length>1){
            validation.logError("Report IDs must be unique", inst, ["gamepadID"]);
        }
    }
    if(inst.enableGenericInOut){
        if(reportIDs.filter(x => x === inst.genericInOutID).length>1){
            validation.logError("Report IDs must be unique", inst, ["genericInOutID"]);
        }
    }

}

function pinmuxRequirements(inst) {

}
let genericConfig = [
    {
        name: "pollingInterval",
        displayName: "Polling Interval (ms)",
        default: 1,
        isInteger: true,
        range: [1,255],
    }
]
let modeConfig = [
    {
        name: "enableKeyboard",
        displayName: "Enable Keyboard",
        default: false,
        onChange: (inst,ui)=>{
            ui.keyboardID.hidden = !inst.enableKeyboard;
        }
    },
    {
        name: "keyboardID",
        displayName: "Keyboard ID",
        default: 0,
        isInteger: true,
        range: [0,255],
        displayFormat: "hex",
        hidden: true,
    },
    {
        name: "enableMouse",
        displayName: "Enable Mouse",
        default: false,
        onChange: (inst,ui)=>{
            ui.mouseID.hidden = !inst.enableMouse;
        }
    },
    {
        name: "mouseID",
        displayName: "Mouse ID",
        default: 0,
        isInteger: true,
        range: [0,255],
        displayFormat: "hex",
        hidden: true,
    },
    {
        name: "enableConsumer",
        displayName: "Enable Consumer",
        default: false,
        onChange: (inst,ui)=>{
            ui.consumerID.hidden = !inst.enableConsumer;
        }
    },
    {
        name: "consumerID",
        displayName: "Consumer ID",
        default: 0,
        isInteger: true,
        range: [0,255],
        displayFormat: "hex",
        hidden: true,
    },
    {
        name: "enableGamepad",
        displayName: "Enable Gamepad",
        default: false,
        onChange: (inst,ui)=>{
            ui.gamepadID.hidden = !inst.enableGamepad;
        }
    },
    {
        name: "gamepadID",
        displayName: "Gamepad ID",
        default: 0,
        isInteger: true,
        range: [0,255],
        displayFormat: "hex",
        hidden: true,
    },
    {
        name: "enableGenericInOut",
        displayName: "Enable Generic Input/Output",
        default: false,
        onChange: (inst,ui)=>{
            ui.genericInOutID.hidden = !inst.enableGenericInOut;
        }
    },
    {
        name: "genericInOutID",
        displayName: "Generic Input/Output ID",
        default: 0,
        isInteger: true,
        range: [0,255],
        displayFormat: "hex",
        hidden: true,
    },
    {
        name: "hidProtocol",
        displayName: "HID Protocol",
        hidden: true,
        default: "NONE",
        options: [
            {name: "NONE", displayName: "None"},
            {name: "KEYBOARD", displayName: "KEYBOARD"},
            {name: "MOUSE", displayName: "MOUSE"},
        ],
        getValue: (inst)=>{
            if(inst.enableKeyboard &&
                !inst.enableMouse &&
                !inst.enableConsumer &&
                !inst.enableGamepad &&
                !inst.enableGenericInOut
            ){
                return "KEYBOARD";
            }
            else if(inst.enableMouse &&
                !inst.enableKeyboard &&
                !inst.enableConsumer &&
                !inst.enableGamepad &&
                !inst.enableGenericInOut
            ){
                return "MOUSE";
            }
            else{
                return "NONE";
            }
        }
    }
]

let hiddenConfig = tusbCommon.getGenericBufferSizeConfig();
let classStringConfig = tusbCommon.getClassStringConfigTemplate();
let endpointConfig = tusbCommon.getEndpointConfigTemplate();
let config = [];
config = config.concat(genericConfig,modeConfig, classStringConfig, endpointConfig,hiddenConfig);

function moduleInstances(inst){
    let modInstances = [];
    /*
    * Gets a Endpoint In module if available
    */
    let mod = {
        name: "HID_EP_IN_DATA",
        displayName: "Data In",
        moduleName: "/ti/tinyusb_meta/endpoint_in/EP_IN",
        group: "GROUP_ENDPOINTS",
        args: {
            //argName: initialAssignedValue
        },
        requiredArgs: {
            // argName: forcedAssignedValue
            /* empty for now */
            epSize: inst.epBufferSize,
        },
        collapsed:false,
    }
    modInstances.push(mod);
    if(inst.enableGenericInOut){
        mod = {
            name: "HID_EP_OUT_DATA",
            displayName: "Data Out",
            moduleName: "/ti/tinyusb_meta/endpoint_out/EP_OUT",
            group: "GROUP_ENDPOINTS",
            args: {
                //argName: initialAssignedValue
            },
            requiredArgs: {
                // argName: forcedAssignedValue
                epSize: inst.epBufferSize,
            },
            collapsed:false,
        }
        modInstances.push(mod);
    }

    return modInstances;
}


let base = {
    name: "devHID",
    displayName: "HID Configuration",
    longDescription: ``,
    config: config,
    // pinmuxRequirements: pinmuxRequirements,
    validate: validate,
    defaultInstanceName: "HID_",
    moduleInstances,
};

exports = base;
