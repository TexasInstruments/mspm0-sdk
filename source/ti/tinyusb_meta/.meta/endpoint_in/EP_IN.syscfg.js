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
 *  ======== EP_IN.syscfg.js ========
 */
"use strict";

/* get Common /ti/driverlib utility functions */
let Common = system.getScript("/ti/driverlib/Common.js");

/*
 *
 */
function validate(inst, validation){}

function EP_FILTER(peripheral){
    return !((peripheral.name).match("IN0|OUT0") !== null)
}
function QEIFilter(peripheral, inst){
    let eligible = true;
    eligible &= (peripheral.name.match("TIMG(8|9|10|11)") !== null);
    return eligible;
}
function pinmuxRequirements(inst) {
    let epResource = {
        name          : "peripheral",
        displayName   : "Endpoint",
        interfaceName : "USB_EP_IN",
        resources     : [],
        signalTypes   : {

        },
        filter        : EP_FILTER,
    };
    return [epResource];
}

let config = [
    {
        name: "$name",
        // hidden: true,
    },
    {
        name: "epSize",
        displayName: "Endpoint Size",
        default: 64,
        isInteger: true,
        range: [0,64],
    },
];

let base = {
    name: "ep_in",
    displayName: "Endpoint In Configuration",
    longDescription: ``,
    config: config,
    pinmuxRequirements: pinmuxRequirements,
    validate: validate,
    defaultInstanceName: "EP_IN_"
};

exports = base;
