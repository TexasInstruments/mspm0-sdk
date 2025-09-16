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
    if(!Common.isValidInputStr(inst.websiteString)){
        validation.logError("Invalid String", inst, "websiteString");
    }
}

function pinmuxRequirements(inst) {

}

function formatUUID(number){
    return ((number).toString(16).toUpperCase()).padStart(2, '0');
}

let classStringConfig = tusbCommon.getClassStringConfigTemplate();
let genericConfig = [
    {
        name: "websiteString",
        displayName: "Website String",
        default: "http://www.ti.com", //need custom validation for website string (allow "/", ":")
    },
    {
        name: "GROUP_UUID",
        displayName: "Universal Unique Identifier Configuration",
        description: "",
        longDescription: ``,
        collapsed: false,
        config: [
            {
                name: "combinedUUID",
                displayName: "UUID",
                default: "FFFFFFFF-FFFF-FFFF-FFFFFFFFFFFFFFFF",
                getValue: (inst)=>{
                    return formatUUID(inst.UUID_1) + formatUUID(inst.UUID_2) + formatUUID(inst.UUID_3) + formatUUID(inst.UUID_4)+"-"+
                            formatUUID(inst.UUID_5) + formatUUID(inst.UUID_6) + "-"+
                            formatUUID(inst.UUID_7) + formatUUID(inst.UUID_8) + "-"+
                            formatUUID(inst.UUID_9) + formatUUID(inst.UUID_10) + formatUUID(inst.UUID_11) + formatUUID(inst.UUID_12)+
                            formatUUID(inst.UUID_13) + formatUUID(inst.UUID_14) + formatUUID(inst.UUID_15) + formatUUID(inst.UUID_16);
                }
            },
            {
                name: "GROUP_DATA_1",
                displayName: "Data 1",
                description: "",
                longDescription: ``,
                collapsed: true,
                config: [
                    {
                        name: "UUID_1",
                        displayName: "Universal Unique Identifier [0]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                    {
                        name: "UUID_2",
                        displayName: "Universal Unique Identifier [1]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                    {
                        name: "UUID_3",
                        displayName: "Universal Unique Identifier [2]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                    {
                        name: "UUID_4",
                        displayName: "Universal Unique Identifier [3]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                ],
            },
            {
                name: "GROUP_DATA_2",
                displayName: "Data 2",
                description: "",
                longDescription: ``,
                collapsed: true,
                config:
                [
                    {
                        name: "UUID_5",
                        displayName: "Universal Unique Identifier [0]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                    {
                        name: "UUID_6",
                        displayName: "Universal Unique Identifier [1]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                ],
            },
            {
                name: "GROUP_DATA_3",
                displayName: "Data 3",
                description: "",
                longDescription: ``,
                collapsed: true,
                config: [
                    {
                        name: "UUID_7",
                        displayName: "Universal Unique Identifier [0]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                    {
                        name: "UUID_8",
                        displayName: "Universal Unique Identifier [1]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                ],
            },
            {
                name: "GROUP_DATA_4",
                displayName: "Data 4",
                description: "",
                longDescription: ``,
                collapsed: true,
                config: [
                    {
                        name: "UUID_9",
                        displayName: "Universal Unique Identifier [0]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                    {
                        name: "UUID_10",
                        displayName: "Universal Unique Identifier [1]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                    {
                        name: "UUID_11",
                        displayName: "Universal Unique Identifier [2]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                    {
                        name: "UUID_12",
                        displayName: "Universal Unique Identifier [3]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                    {
                        name: "UUID_13",
                        displayName: "Universal Unique Identifier [4]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                    {
                        name: "UUID_14",
                        displayName: "Universal Unique Identifier [5]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                    {
                        name: "UUID_15",
                        displayName: "Universal Unique Identifier [6]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                    {
                        name: "UUID_16",
                        displayName: "Universal Unique Identifier [7]",
                        default     : 0xFF,
                        isInteger: true,
                        displayFormat: "hex",
                        range       : [0, 0xFF],
                    },
                ],
            },
        ],
    },
]
let config = [];
config = config.concat(classStringConfig, genericConfig);

function moduleInstances(inst){
    let modInstances = [];

    let modInst = {
        name: "associated_devBillboardAUM",
        displayName: "Alternate Configuration",
        useArray: true,
        defaultInstanceCount: 1,
        minInstanceCount: 1,
        // longDescription: longDescription,
        collapsed: false,
        moduleName: "/ti/tinyusb_meta/tinyusb/USB_devBillboardAUM",
        hidden: false,
        args: {

        },
        requiredArgs: {

        },
        // group: "GROUP_DEV_BILLBOARD",

    };
    modInstances.push(modInst);

    return modInstances;
}


let base = {
    name: "devBillboard",
    displayName: "Billboard Configuration",
    longDescription: ``,
    config: config,
    // pinmuxRequirements: pinmuxRequirements,
    validate: validate,
    defaultInstanceName: "BILLBOARD_",
    moduleInstances: moduleInstances,
    maxInstances: 1,
};

exports = base;
