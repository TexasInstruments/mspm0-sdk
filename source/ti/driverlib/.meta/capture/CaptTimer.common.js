/*
 * Copyright (c) 2018-2019 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== CaptTimer.common.js ========
 */

/* get Common /ti/driverlib utility functions */
let Common = system.getScript("/ti/driverlib/Common.js");

exports = {
    getDisabledOptionsInterrupts: getDisabledOptionsInterrupts,
    isFourCCCapable             : isFourCCCapable,
};

function getDisabledOptionsInterrupts(inst,ui)
{
    let disabled = [];

    if((Common.hasTimerA()) && !isFourCCCapable(inst))
    {
        disabled.push({ name: "CC2_DN", displayName: "Channel 2 compare down event", reason: "Not supported by Timer instance"});
        disabled.push({ name: "CC2_UP", displayName: "Channel 2 compare up event",   reason: "Not supported by Timer instance"});
        disabled.push({ name: "CC3_DN", displayName: "Channel 3 compare down event", reason: "Not supported by Timer instance"});
        disabled.push({ name: "CC3_UP", displayName: "Channel 3 compare up event",   reason: "Not supported by Timer instance"});
    }

    return disabled;
}

// Returns true if the device is TIMA0 and thus has four capture compare channels
function isFourCCCapable(inst)
{
    try{
        return (inst.peripheral.$solution.peripheralName.match(/TIMA0|TIMG14/) != null);
    }catch (e) {
        return false;
    }
}
