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

function onChangeRXTX(inst,ui){
    inst.enableOneChannelMic    = (inst.mode === "oneChannelMic");
    inst.enableFourChannelMic   = (inst.mode === "fourChannelMic");
    inst.enableHeadset          = (inst.mode === "headset");
    inst.enableSpeaker          = (inst.mode === "speaker");


    ui.interruptEnable.hidden   = !inst.enableHeadset;
    ui.enableFeedback.hidden    = !inst.enableSpeaker;
    ui.enableDebug.hidden       = !inst.enableSpeaker;

    let rxEnabled = (inst.enableHeadset || inst.enableSpeaker);
    let txEnabled = (inst.enableOneChannelMic || inst.enableFourChannelMic || inst.enableHeadset);
    let multiFormatEnabled = (inst.enableHeadset);
    let resolutionEnabled = (inst.enableHeadset || inst.enableSpeaker);

    ui.rxChannels.hidden        = !(rxEnabled);
    ui.bytesPerSampleRX.hidden  = !(rxEnabled);
    ui.rxResolution.hidden      = !(rxEnabled && resolutionEnabled);

    ui.txChannels.hidden        = !(txEnabled);
    ui.bytesPerSampleTX.hidden  = !(txEnabled);
    ui.txResolution.hidden      = !(txEnabled && resolutionEnabled);

    ui.bytesPerSampleRX_F2.hidden  = !(rxEnabled && multiFormatEnabled);
    ui.rxResolution_F2.hidden      = !(rxEnabled && multiFormatEnabled && resolutionEnabled);

    ui.bytesPerSampleTX_F2.hidden  = !(txEnabled && multiFormatEnabled);
    ui.txResolution_F2.hidden      = !(txEnabled && multiFormatEnabled && resolutionEnabled);

    ui.rxChannelsPerFifo.hidden = !inst.enableDecoding || !rxEnabled;
    ui.txChannelsPerFifo.hidden = !inst.enableEncoding || !txEnabled;

}

let modeConfig = [
    {
        name: "mode",
        displayName: "Mode",
        default: "oneChannelMic",
        options: [
            { name: "oneChannelMic",   displayName: "One Channel Microphone"},
            { name: "fourChannelMic",  displayName: "Four Channel Microphone"},
            { name: "headset",         displayName: "Headset"},
            { name: "speaker",         displayName: "Speaker"},
        ],
        onChange: onChangeRXTX,
    },
    {
        name: "enableOneChannelMic",
        displayName: "Enable One Channel Microphone",
        default: true,
        hidden: true,
    },
    {
        name: "enableFourChannelMic",
        displayName: "Enable Four Channel Microphone",
        default: false,
        hidden: true,
    },
    {
        name: "enableHeadset",
        displayName: "Enable Headset",
        default: false,
        hidden: true,
    },
    {
        name: "interruptEnable",
        displayName: "Interrupt Enable",
        default: false,
        hidden:true,
    },
    {
        name: "enableSpeaker",
        displayName: "Enable Speaker",
        default: false,
        hidden: true
    },
    {
        name: "enableFeedback",
        displayName: "Feedback Enable",
        default: false,
        hidden: true,
    },
    {
        name: "enableDebug",
        displayName: "Debug Enable",
        default: false,
        hidden: true,
    }
]


// Format-independent configuration
let genericTXRXConfig = [
    {
        name: "rxChannels",
        displayName: "RX Channels",
        default: "1",
        options: [
            { name: "1", displayName: "1" },
            { name: "2", displayName: "2" },
        ],
        hidden: true,
    },
    //Inherited value from parent
    {
        name: "enableDecoding",
        default: true,
        hidden: true,
        onChange: onChangeRXTX,
    },
    {
        name: "rxChannelsPerFifo",
        displayName: "RX Channels Per FIFO",
        default: 1,
        hidden: true,
        options: [
            { name: 1, displayName: "1" },
            { name: 2, displayName: "2" },
        ]
    },
    {
        name: "txChannels",
        displayName: "TX Channels",
        default: "1",
        getValue: (inst)=>{
            let txChannels = 0;
            if(inst.enableOneChannelMic){ txChannels += 1;};
            if(inst.enableFourChannelMic){ txChannels += 4;};
            if(inst.enableHeadset){ txChannels += 1;};
            return txChannels.toString();
        }
    },
    //Inherited value from parent
    {
        name: "enableEncoding",
        default: true,
        hidden: true,
        onChange: onChangeRXTX,
    },
    {
        name: "txChannelsPerFifo",
        displayName: "TX Channels Per FIFO",
        default: 1,
        hidden: true,
        options: [
            { name: 1, displayName: "1" },
            { name: 2, displayName: "2" },
        ]
    },
]
// Format 1 Configuration
let format1TXRXConfig = [
    {
        name: "rxResolution",
        displayName: "RX Resolution",
        default: "1",
        options: [
            { name: "1", displayName: "8-bit" },
            { name: "2", displayName: "16-bit" },
            { name: "3", displayName: "32-bit" },
            { name: "4", displayName: "64-bit" },

        ]
    },
    {
        name: "bytesPerSampleRX",
        displayName: "Bytes Per Sample (RX)",
        default: 0,
        isInteger: true,
        // range: [0,1],
    },
    {
        name: "txResolution",
        displayName: "TX Resolution",
        default: "1",
        options: [
            { name: "1", displayName: "8-bit" },
            { name: "2", displayName: "16-bit" },
            { name: "3", displayName: "32-bit" },
            { name: "4", displayName: "64-bit" },

        ]
    },
    {
        name: "bytesPerSampleTX",
        displayName: "Bytes Per Sample (TX)",
        default: 0,
        isInteger: true,
        // range: [0,1],
    },
];
// Format 2 Configuration
let format2TXRXConfig = [
    {
        name: "rxResolution_F2",
        displayName: "RX Resolution",
        default: "1",
        options: [
            { name: "1", displayName: "8-bit" },
            { name: "2", displayName: "16-bit" },
            { name: "3", displayName: "32-bit" },
            { name: "4", displayName: "64-bit" },
        ],
        hidden: true,
    },
    {
        name: "bytesPerSampleRX_F2",
        displayName: "Bytes Per Sample (RX)",
        default: 0,
        isInteger: true,
        // range: [0,1],
        hidden: true,
    },
    {
        name: "txResolution_F2",
        displayName: "TX Resolution",
        default: "1",
        options: [
            { name: "1", displayName: "8-bit" },
            { name: "2", displayName: "16-bit" },
            { name: "3", displayName: "32-bit" },
            { name: "4", displayName: "64-bit" },
        ],
        hidden: true,
    },
    {
        name: "bytesPerSampleTX_F2",
        displayName: "Bytes Per Sample (TX)",
        default: 0,
        isInteger: true,
        // range: [0,1],
        hidden: true,
    },
];

// Multi-Format Configuration
let multiFormatTXRXConfig = [
    {
        name: "GROUP_TXRX_FORMAT1",
        displayName: "Format 1 Configuration",
        config: format1TXRXConfig,
    },
    {
        name: "GROUP_TXRX_FORMAT2",
        displayName: "Format 2 Configuration",
        config: format2TXRXConfig,
    }
];

let dynamicTXRXConfig = genericTXRXConfig.concat(multiFormatTXRXConfig);

let genericConfig = [
    {
        name: "GROUP_MODE",
        displayName: "Mode Configuration",
        config: modeConfig,
        collapsed: false,
    },
    {
        name: "GROUP_TXRX",
        displayName: "TX/RX Configuration",
        config: dynamicTXRXConfig,
        collapsed: false,
    },
    {
        name: "sampleRate",
        displayName: "Sample Rate (Hz)",
        default: 48000,
        isInteger: true,
        range: [0,96000],
    },
    {
        name: "sampleRateDisplay",
        displayName: "Sample Rate",
        default: "0 Hz",
        getValue: (inst)=>{
            return (Common.getUnitPrefix(inst.sampleRate)).str+"Hz"
        }
    },
]

let hiddenConfig = tusbCommon.getGenericBufferSizeConfig();
let classStringConfig = tusbCommon.getClassStringConfigTemplate();
let endpointConfig = tusbCommon.getEndpointConfigTemplate();
let config = [];
config = config.concat(genericConfig, classStringConfig, endpointConfig,hiddenConfig);

function moduleInstances(inst){
    let mod = undefined;
    let modInstances = [];
    /*
    * Gets a Endpoint In module if available
    */
    if(inst.enableOneChannelMic){
        mod = {
            name: "AUDIO_EP_IN_ONE_CHANNEL_MIC",
            displayName: "One Channel Microphone Input",
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
    }

    if(inst.enableFourChannelMic){
        mod = {
            name: "AUDIO_EP_IN_FOUR_CHANNEL_MIC",
            displayName: "Four Channel Microphone Input",
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
    }

    if(inst.enableHeadset){
        mod = {
            name: "AUDIO_EP_IN_HEADSET",
            displayName: "Headset Input",
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

        mod = {
            name: "AUDIO_EP_OUT_HEADSET",
            displayName: "Headset Output",
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

        if(inst.interruptEnable){
            mod = {
                name: "AUDIO_EP_IN_INTERRUPT",
                displayName: "Headset Interrupt",
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
        }
    }

    if(inst.enableSpeaker){
        mod = {
            name: "AUDIO_EP_OUT_SPEAKER",
            displayName: "Speaker Output",
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

        if(inst.enableFeedback){
            mod = {
                name: "AUDIO_EP_IN_FEEDBACK",
                displayName: "Speaker Feedback",
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
        }

        if(inst.enableDebug){
            mod = {
                name: "AUDIO_EP_IN_DEBUG",
                displayName: "Speaker Debug",
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
        }
    }


    return modInstances;
}

let base = {
    name: "devAudio",
    displayName: "Audio Configuration",
    longDescription: ``,
    config: config,
    // pinmuxRequirements: pinmuxRequirements,
    validate: validate,
    defaultInstanceName: "AUDIO_",
    maxInstances: 3,
    moduleInstances: moduleInstances,
};

exports = base;
