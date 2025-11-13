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
 *  ======== PWM.common.js ========
 */

/* get Common /ti/driverlib utility functions */
let Common = system.getScript("/ti/driverlib/Common.js");

exports = {
    // allEventOptions     : allEventOptions,
    getDisabledEvents   : getDisabledEvents,
    getEventOptions     : getEventOptions,
};

let allEventOptions = [
    { name: "ZERO_EVENT", displayName: "Zero event", description: "PWM Timer reaches zero" },
    { name: "LOAD_EVENT", displayName: "Load event", description: "PWM Timer loads Timer Count Value" },
    { name: "FAULT_EVENT", displayName: "Fault event", reason: "Fault not Capable" },
    { name: "OVERFLOW_EVENT", displayName: "Overflow event" },
    { name: "CC0_DN_EVENT", displayName: "Channel 0 capture or compare down event", reason: "Channel Not In Use" },
    { name: "CC0_UP_EVENT", displayName: "Channel 0 capture or compare up event", reason: "Channel Not In Use" },
    { name: "CC1_DN_EVENT", displayName: "Channel 1 capture or compare down event", reason: "Channel Not In Use" },
    { name: "CC1_UP_EVENT", displayName: "Channel 1 capture or compare up event", reason: "Channel Not In Use" },
    { name: "CC2_DN_EVENT", displayName: "Channel 2 capture or compare down event", reason: "Channel Not In Use" },
    { name: "CC2_UP_EVENT", displayName: "Channel 2 capture or compare up event", reason: "Channel Not In Use" },
    { name: "CC3_DN_EVENT", displayName: "Channel 3 capture or compare down event", reason: "Channel Not In Use" },
    { name: "CC3_UP_EVENT", displayName: "Channel 3 capture or compare up event", reason: "Channel Not In Use" },
    { name: "CC4_DN_EVENT", displayName: "Channel 4 capture or compare down event", reason: "Channel Not In Use" },
    { name: "CC4_UP_EVENT", displayName: "Channel 4 capture or compare up event", reason: "Channel Not In Use" },
    { name: "CC5_DN_EVENT", displayName: "Channel 5 capture or compare down event", reason: "Channel Not In Use" },
    { name: "CC5_UP_EVENT", displayName: "Channel 5 capture or compare up event", reason: "Channel Not In Use" },
];

function getEventOptions(inst)
{
    let options = [];
    //let pubs = system.modules["/ti/driverlib/EVENT"].$static.channel2Pub;
    for(let option of allEventOptions){
        options.push({name: option["name"], displayName: option["displayName"]});
    }

    if(!inst.fourChannelCapable) {
        options = options.filter( (el) => !el.name.match("CC2|3") );
        if(!inst.timerACapable) {
             options.splice(2,1); // remove fault
        }
    }
    if(!Common.hasTimerA()) {
        options = options.filter( (el) => !el.name.match("CC4|5") );
    }
    return options;
}

function getDisabledEvents(inst)
{
    let allOptions = allEventOptions;

    if(!inst.fourChannelCapable) {
        allOptions = _.filter( allOptions, (el) => !el.name.match("CC2|3") );
    }

    let channels = [0,1,2,3];
    for (let cc of inst.ccIndex) {
        // remove the channels that are active, since these are not disabled
        channels.splice(channels.indexOf(cc),1);
    }

    var str = "CC"
    if(channels.length != 0) {
        for (let chan of channels){
            str += chan + '|';
        }
        // remove last '|' before matching
        return allOptions.filter( (el) => el.name.match(str.slice(0,-1)) );
    }
    return [];
}
