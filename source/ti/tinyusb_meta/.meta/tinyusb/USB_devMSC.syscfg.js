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
}

function pinmuxRequirements(inst) {

}

function moduleInstances(inst){
    let modInstances = [];
    /*
    * Gets a Endpoint In module if available
    */
    let mod = {
        name: "MSC_EP_IN_DATA",
        displayName: "Data In",
        moduleName: "/ti/tinyusb_meta/endpoint_in/EP_IN",
        group: "GROUP_ENDPOINTS",
        args: {
            //argName: initialAssignedValue
        },
        requiredArgs: {
            // argName: forcedAssignedValue
            /* empty for now */
            epSize: inst.epIOSize,
        },
        collapsed:false,
    }
    modInstances.push(mod);

    mod = {
        name: "MSC_EP_OUT_DATA",
        displayName: "Data Out",
        moduleName: "/ti/tinyusb_meta/endpoint_out/EP_OUT",
        group: "GROUP_ENDPOINTS",
        args: {
            //argName: initialAssignedValue
        },
        requiredArgs: {
            // argName: forcedAssignedValue
            epSize: inst.epIOSize,
        },
        collapsed:false,
    }
    modInstances.push(mod);

    return modInstances;
}

let hiddenConfig = tusbCommon.getGenericBufferSizeConfig();
let classStringConfig = tusbCommon.getClassStringConfigTemplate();
let endpointConfig = {
    name: "GROUP_ENDPOINTS",
    displayName: "Endpoint Configuration",
    config: [
        {
            name: "epIOSize",
            displayName: "Endpoint I/O Size",
            default: 64,
            isInteger: true,
            range: [0,64],
        }
    ],
}


let config = [];
config = config.concat(classStringConfig,endpointConfig,hiddenConfig);

let base = {
    name: "devMSC",
    displayName: "MSC Configuration",
    longDescription: ``,
    config: config,
    // pinmuxRequirements: pinmuxRequirements,
    validate: validate,
    defaultInstanceName: "MSC_",
    moduleInstances: moduleInstances,
};

exports = base;
