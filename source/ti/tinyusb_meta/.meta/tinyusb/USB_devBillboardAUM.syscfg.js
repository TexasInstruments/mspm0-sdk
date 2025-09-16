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
    if(!Common.isValidInputStr(inst.AUMStr)){
        validation.logError("Invalid String", inst, "AUMStr");
    }
}

function pinmuxRequirements(inst) {

}

let genericConfig = [
    {
        name: "dwAlternateModeVdo",
        displayName: "Alternate Mode VDO",
        default: 0,
        isInteger: true,
        range: [0,0xFFFFFFFF],
    },
    {
        name: "AUMStr",
        displayName: "Alternate Mode String",
        default: "Billboard AUM",
    },
    // TODO: ADD these
    //       .wSVID                      = 0x8087,
    {
        name: "wSVID",
        displayName: "wSVID",
        default: 0x0,
        isInteger: true,
        displayFormat: "hex",
        range: [0x0000,0xFFFF],
    },
    //  .bAlternateOrUSB4Mode       = 0x01,
    {
        name: "bAlternateOrUSB4Mode",
        displayName: "bAlternateOrUSB4Mode",
        default: 0x00,
        isInteger: true,
        displayFormat: "hex",
        range: [0x00,0xFF],
    },
]
let config = [].concat(genericConfig);
// config = config.concat();

function moduleInstances(inst){
    let modInstances = [];

    // let mod = undefined;
    // modInstances.push(mod);

    return modInstances;
}


let base = {
    name: "devBillboardAUM",
    displayName: "ALternate Billboard Configuration",
    longDescription: ``,
    config: config,
    // pinmuxRequirements: pinmuxRequirements,
    validate: validate,
    defaultInstanceName: "AUM_",
    moduleInstances: moduleInstances,
};

exports = base;
