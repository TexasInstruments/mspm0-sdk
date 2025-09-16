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
 *  ======== I2SMSPM0.syscfg.js ========
 */

"use strict";

/* get Common /ti/driverlib utility functions */
let Common = system.getScript("/ti/driverlib/Common.js");


/************************* General functions **********************************/

/*
 *  ======== validatePinmux ========
 *  Validation run after solution
 *
 *  param inst       - module instance to be validated
 *  param validation - object to hold detected validation issues
 */
function validatePinmux(inst, validation) {

}

/*
 *  ======== validate ========
 *  Validate this inst's configuration
 *
 *  param inst       - module instance to be validated
 *  param validation - object to hold detected validation issues
 */
function validate(inst, validation){

}

function onChangeI2SMode(inst,ui){
    // MCLK Signal can only be enabled in CONTROLLER mode
    ui.enableMCLK.hidden = (inst.mode !== "CONTROLLER");
    ui.mclkDivider.hidden = (inst.mode !== "CONTROLLER") || (!inst.enableMCLK);
}

function getIntOptions(inst)
{
    let allOptions = [
        {name: "WCLK_ERROR", displayName: "WCLK Error"},
        {name: "RXFIFO_TRIGGER", displayName: "RXFIFO Trigger when >= trigger level"},
        {name: "TXFIFO_TRIGGER", displayName: "TXFIFO Trigger when <= trigger level"},
        {name: "DMA_DONE_RX", displayName: "DMA Done on RX Event Channel"},
        {name: "DMA_DONE_TX", displayName: "DMA Done on TX Event Channel"},
        {name: "RX_FIFO_OVERFLOW", displayName: "TXFIFO Overflow Event"},
        {name: "TX_FIFO_UNDERFLOW", displayName: "TXFIFO Underflow Event"},
    ];

    return allOptions;
}
function updateGUIInterrupts(inst, ui){
    ui.interruptPriority.hidden = (inst.enabledInterrupts.length === 0);
}

let clockConfig = [
    /* DL_I2S_setClockConfig */
    {
        // DL_I2S_CLOCK_SOURCE_
        name: "clockSel",
        displayName: "Clock Source",
        default: "SYSOSC",
        options: [
            {name: "SYSOSC", displayName: "SYSOSC"},
            {name: "HFXT", displayName: "HFXT"},
            {name: "PLL", displayName: "PLL"},
        ]
    },
    {
        // DL_I2S_WORD_BAUD_CLOCK_SOURCE_
        name: "wordBaudClockSource",
        displayName: "Word Baud Clock Source",
        default: "NONE",
        options: [
            {name: "NONE", displayName: "None"},
            {name: "EXTERNAL", displayName: "External"},
            {name: "INTERNAL", displayName: "Internal"},
        ]
    },
    {
        // DL_I2S_WCLK_PHASE_
        name: "wclkPhase",
        displayName: "WCLK Phase",
        default: "SINGLE",
        options: [
            {name: "SINGLE", displayName: "Single-Phase"},
            {name: "DUAL", displayName: "Dual-Phase"},
            {name: "CUSTOM", displayName: "Custom"},
        ]
    },
    {
        // [0x1, 0xFFFF]
        name: "wclkDivider",
        displayName: "WCLK Divider",
        default: 1,
        isInteger: true,
        range: [1, 0xFFFF],
    },
    {
        // [ DL_I2S_BDIV_MINIMUM, DL_I2S_BDIV_MAXIMUM ]
        name: "bclkDivider",
        displayName: "BCLK Divider",
        default: 2,
        isInteger: true,
        range: [2, 1024],
    },
    {
        //
        name: "enableMCLK",
        displayName: "Enable MCLK",
        description: "Enable MCLK signal",
        longDescription: "Enable the optional MCLK signal. Controller mode only.",
        default: false,
        hidden: false,
        onChange: onChangeI2SMode,
    },
    {
        // [ DL_I2S_MDIV_MINIMUM, DL_I2S_MDIV_MAXIMUM]
        name: "mclkDivider",
        displayName: "MCLK Divider",
        default: 2,
        isInteger: true,
        range: [2, 1024],
        hidden: true,
    },
]
let initConfig =[
    /* DL_I2S_INIT */
    {
        // DL_I2S_MODE_
        name: "mode",
        displayName: "Mode",
        default: "CONTROLLER",
        options: [
            {name: "CONTROLLER", displayName: "Controller"},
            {name: "TARGET", displayName: "Target"},
        ],
        onChange: onChangeI2SMode,
    },
    {
        // DL_I2S_WCLK_INVERSION_
        name: "wclkInvert",
        displayName: "Invert WCLK",
        default: false,
    },
    {
        // DL_I2S_PHASE_
        name: "phase",
        displayName: "Phase",
        default: "SINGLE",
        options: [
            {name: "SINGLE", displayName: "Single-Phase (DSP)"},
            {name: "DUAL", displayName: "Dual-Phase (I2S, LJF and RJF)"},
        ]
    },
    {
        // DL_I2S_SAMPLE_EDGE_
        name: "samplingEdge",
        displayName: "Sampling Edge",
        default: "NEG",
        options: [
            {name: "POS", displayName: "Data sampled on negative edge, clocked out on positive edge"},
            {name: "NEG", displayName: "Data sampled on positive edge, clocked out on negative edge"},
        ]
    },
    {
        // DL_I2S_setSampleWordLength_
        name: "sampleWordLength",
        displayName: "Sample Word Length (Bits)",
        default: 8,
        isInteger: true,
        range: [8, 32],
    },
    {
        // DL_I2S_DATA_DELAY_
        name: "dataDelay",
        displayName: "Data Delay",
        default: "ZERO",
        description: "Delay in BCLK periods",
        longDescription: "Set the delay in BCLK periods between WCLK edge and MSB of first word",
        options: [
            {name: "ZERO", displayName: "0 BCLK periods"},
            {name: "ONE", displayName: "1 BCLK period"},
            {name: "TWO", displayName: "2 BCLK periods"},
            {name: "MAX", displayName: "Max BCLK period"},
        ],
    },
    {
        // DL_I2S_EMPTY_SLOT_OUTPUT_
        name: "emptySlotOutput",
        displayName: "Empty Slot Output",
        default: "ZERO",
        options: [
            {name: "ZERO", displayName: "Send out zeroes"},
            {name: "ONE", displayName: "Send out ones"},
            {name: "TRISTATE", displayName: "Tri-State data pin line"},
        ],
    },
    {
        // DL_I2S_MEMORY_LENGTH_
        name: "memoryAccessLength",
        displayName: "Memory Access Length",
        default: "16",
        options: [
            {name: "16", displayName: "16-Bit"},
            {name: "32", displayName: "32-Bit"},
        ]
    },
    {
        name: "dataPin0Enable",
        displayName: "Enable Data Pin 0",
        default: false,
        onChange: (inst, ui) => {
            ui.dataPin0Direction.hidden = !inst.dataPin0Enable;
            ui.dataPin0ValidChannelMask.hidden = !inst.dataPin0Enable;
        }
    },
    {
        // DL_I2S_DATA_PIN_DIRECTION_
        name: "dataPin0Direction",
        displayName: "Data Pin 0 Direction",
        default: "INPUT",
        options: [
            {name: "INPUT", displayName: "Input"},
            {name: "OUTPUT", displayName: "Output"},
        ],
        hidden: true,
    },
    {
        // [0, 0xFF]
        name: "dataPin0ValidChannelMask",
        displayName: "Data Pin 0 Valid Channel Mask",
        default: 0x1,
        options: [
            {name: 0x1, displayName: "Mono"},
            {name: 0x3, displayName: "Stereo"},
        ],
        hidden: true,
    },
    {
        name: "dataPin1Enable",
        displayName: "Enable Data Pin 1",
        default: false,
        onChange: (inst, ui) => {
            ui.dataPin1Direction.hidden = !inst.dataPin1Enable;
            ui.dataPin1ValidChannelMask.hidden = !inst.dataPin1Enable;
        }
    },
    {
        // DL_I2S_DATA_PIN_DIRECTION_
        name: "dataPin1Direction",
        displayName: "Data Pin 1 Direction",
        default: "INPUT",
        options: [
            {name: "INPUT", displayName: "Input"},
            {name: "OUTPUT", displayName: "Output"},
        ],
        hidden: true,
    },
    {
        // [0, 0xFF]
        name: "dataPin1ValidChannelMask",
        displayName: "Data Pin 1 Valid Channel Mask",
        default: 0x1,
        options: [
            {name: 0x1, displayName: "Mono"},
            {name: 0x3, displayName: "Stereo"},
        ],
        hidden: true,
    },
];

let miscConfig = [
    {
        // DL_I2S_enableFreeRun(I2S_Regs *)
        name: "enableFreeRun",
        displayName: "Enable Free Run",
        default: false,
    },
    {
        // DL_I2S_setTXFIFOThreshold(I2S_Regs *, DL_I2S_TX_FIFO_LEVEL)
        // DL_I2S_TX_FIFO_LEVEL_
        name: "txFIFOThreshold",
        displayName: "Set TX FIFO Threshold",
        default: "1_2_EMPTY",
        options: [
            {name: "3_4_EMPTY", displayName: "TX FIFO <= 3/4 empty"},
            {name: "1_2_EMPTY", displayName: "TX FIFO <= 1/2 empty"},
            {name: "1_4_EMPTY", displayName: "TX FIFO <= 1/4 empty"},
            {name: "EMPTY", displayName: "TX FIFO is empty"},
            {name: "ONE_ENTRY", displayName: "TX FIFO <= 1 entry"},
            // TODO: Improve naming of this option, see other COMM modules
            {name: "ALMOST_FULL", displayName: "TX FIFO >= 3 entries"},
        ]
    },
    {
        // DL_I2S_setRXFIFOThreshold(I2S_Regs *, DL_I2S_RX_FIFO_LEVEL)
        // DL_I2S_RX_FIFO_LEVEL
        name: "rxFIFOThreshold",
        displayName: "Set RX FIFO Threshold",
        default: "1_2_FULL",
        options: [
            {name: "1_4_FULL", displayName: "RX FIFO >= 1/4 full"},
            {name: "1_2_FULL", displayName: "RX FIFO >= 1/2 full"},
            {name: "3_4_FULL", displayName: "RX FIFO >= 3/4 full"},
            {name: "FULL", displayName: "RX FIFO is full"},
            // TODO: Improve naming of this option, see other COMM modules
            {name: "ALMOST_FULL", displayName: "RX FIFO >= 3 entries"},
            {name: "ONE_ENTRY", displayName: "RX FIFO <= 1 entry"},

        ]
    },
]


let clockConfigGroup = {
    name: "GROUP_CLOCK_CONFIG",
    displayName: "Clock Configuration",
    description: "",
    longDescription: ``,
    collapsed: false,
    config: clockConfig,
};

let basicConfig = [].concat(initConfig,[clockConfigGroup]);
let basicGroup = {
        name: "GROUP_BASIC",
        displayName: "Basic Configuration",
        description: "",
        longDescription:
`Basic I2S options allow configuration of the most common options to enable
basic communication and configure the serial interface.
`,
        collapsed: false,
        config: basicConfig,

};

let advancedConfig = [].concat(miscConfig);
let advancedGroup = {
        name: "GROUP_ADVANCED",
        displayName: "Advanced Configuration",
        description: "",
        longDescription: "",
        collapsed: true,
        config: advancedConfig,
};
let interruptConfig = [
    /* DL_I2S_enableInterrupt */
    {
        name        : "enabledInterrupts",
        displayName : "Enable Interrupts",
        description : 'Enable I2S interrupt',
        default     : [],
        options     : getIntOptions,
        onChange    : (inst, ui) => {
            updateGUIInterrupts(inst, ui);
        }
    },
    {
        name: "interruptPriority",
        displayName: "Interrupt Priority",
        description: "Specifies interrupt priority",
        longDescription: '',
        hidden: true,
        default: "DEFAULT",
        options: Common.InterruptPriorityOptions,
    },
]
let interruptGroup = {
        name: "GROUP_INTERRUPTS",
        displayName: "Interrupt Configuration",
        longDescription: "This section allows enabling/disabling I2S interrupts",
        collapsed: true,
        config: interruptConfig,
};

/* DL_I2S_DMA_INTERRUPT_RX */
let dmaEvt1Options = [
    {name: "None", displayName: "None"},
    {name: "DL_I2S_DMA_INTERRUPT_RX_TRIGGER", displayName: "I2S RX interrupt"},
]
/* DL_I2S_DMA_INTERRUPT_TX */
let dmaEvt2Options = [
    {name: "None", displayName: "None"},
    {name: "DL_I2S_DMA_INTERRUPT_TX_TRIGGER", displayName: "I2S TX interrupt"},
]

let dmaConfig = [
    /* DL_I2S_enableDMAReceiveEvent */
    {
        name        : "enabledDMAEvent1Triggers",
        displayName : "Configure DMA Event 1 Triggers",
        description : 'Configure the I2S interrupts to use as triggers for the DMA RX event',
        hidden      : false,
        default     : "None",
        options     : dmaEvt1Options,
        onChange    : (inst,ui) => {
            ui.enableDMAEvent1.hidden = (inst.enabledDMAEvent1Triggers == "None");
        },
    },
    {
        name        : "enableDMAEvent1",
        displayName : "Enable DMA Event 1 Trigger",
        longDescription: `Only one DMA receive trigger can be enabled
        at the same time. If left unchecked, the module-specific enable
        DMA API should be called in the application code.`,
        hidden      : true,
        default     : true,
    },
    /* DL_I2S_enableDMATransmitEvent */
    {
        name        : "enabledDMAEvent2Triggers",
        displayName : "Configure DMA Event 2 Trigger",
        description : 'Configure the I2S interrupt to use as triggers for the DMA TX event',
        hidden      : false,
        default     : "None",
        options     : dmaEvt2Options,
        onChange    : (inst,ui) => {
            ui.enableDMAEvent2.hidden = (inst.enabledDMAEvent1Triggers == "None");
        },
    },
    {
        name        : "enableDMAEvent2",
        displayName : "Enable DMA Event 2 Trigger",
        longDescription: `If left unchecked, the module-specific enable
        DMA API should be called in the application code.`,
        hidden      : true,
        default     : true,
    },
]
let dmaGroup = {
        name: "GROUP_DMA",
        displayName: "DMA Configuration",
        longDescription: `
This section allows configuring DMA Event triggers.\n
`,
        collapsed: true,
        config: dmaConfig,
};

let config = [
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
    basicGroup,
    advancedGroup,
    interruptGroup,
    dmaGroup,
]


/* Add Pinmux Peripheral Configuration group */
config = config.concat(Common.getGPIOGroupConfig());

function moduleInstances(inst){
    let modInstances = []

    /*
     * Gets a DMA module if available
     */
    if(!["None"].includes(inst.enabledDMAEvent1Triggers)){
        let mod = {
            name: "DMA_CHANNEL_EVENT1",
            displayName: "DMA Channel Event 1",
            moduleName: '/ti/driverlib/DMAChannel',
            group: "GROUP_DMA",
            args: {

            },
            requiredArgs: {
                hideTriggerSelect: true,
                passedTriggerSelect: 0,

            },

        }
        modInstances.push(mod);
    }
    if(!["None"].includes(inst.enabledDMAEvent2Triggers)){
        let mod = {
            name: "DMA_CHANNEL_EVENT2",
            displayName: "DMA Channel Event 2",
            moduleName: '/ti/driverlib/DMAChannel',
            group: "GROUP_DMA",
            args: {
            },
            requiredArgs: {
                hideTriggerSelect: true,
                passedTriggerSelect: 1,

            },

        }
        modInstances.push(mod);
    }

    return modInstances;

}

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base module configuration
 */
let devSpecific = {

    config: config,


    maxInstances: Common.peripheralCount("I2S"),

    /* override generic requirements with  device-specific reqs (if any) */
    validate: validate,

    validatePinmux: validatePinmux,

    pinmuxRequirements: pinmuxRequirements,

    moduleInstances: moduleInstances,

    filterHardware: filterHardware,

    /* override device-specific templates */
    templates: {
        boardc: "/ti/driverlib/i2s/I2S.Board.c.xdt",
        boardh: "/ti/driverlib/i2s/I2S.Board.h.xdt",
        Call: true,
        Reset: true,
        Power: true,
        GPIO: true,
        Function: true,
    },
    modules: setRequiredModules,
    _getPinResources: _getPinResources
};

function setRequiredModules(inst){
    let theModules = ["Board", "SYSCTL"]    /* DMA Dependency */
    if(!["None"].includes(inst.enabledDMAEvent1Triggers) || !["None"].includes(inst.enabledDMAEvent2Triggers)){
        theModules.push("DMA");
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

    return modArray
};


/*
 *  ======== _getPinResources ========
 */
function _getPinResources(inst)
{
    let pin;

    if (inst.module) {
        let modPin = "DIO" + inst.module.modulePin.$solution.devicePinName;
        modPin = modPin.match(/P\d+\.\d+/)[0];

        pin = "\nMODPIN: " + modPin;

        if (inst.$hardware && inst.$hardware.displayName) {
            pin += "\n" + inst.$hardware.displayName;
        }
    }

    return (pin);


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
    let AD0Pin = {
        name: "AD0Pin",
        displayName: "Data Pin 0",
        interfaceNames: ["AD0"]
    }

    let AD1Pin = {
        name: "AD1Pin",
        displayName: "Data Pin 1",
        interfaceNames: ["AD1"]
    }

    let BCLKPin = {
        name: "BCLKPin",
        displayName: "BCLK Pin",
        interfaceNames: ["BCLK"]
    }

    let MCLKPin = {
        name: "MCLKPin",
        displayName: "MCLK Pin",
        interfaceNames: ["MCLK"]
    }

    let WCLKPin = {
        name: "WCLKPin",
        displayName: "WCLK Pin",
        interfaceNames: ["WCLK"]
    }

    let resources = [];
    if(inst.dataPin0Enable){
        resources.push(AD0Pin);
    }
    if(inst.dataPin1Enable){
        resources.push(AD1Pin);
    }
    resources.push(BCLKPin);
    if(inst.enableMCLK){
        resources.push(MCLKPin);
    }
    resources.push(WCLKPin);
    // resources.push(sclk);

    let i2s = {
        name          : "peripheral",
        displayName   : "I2S Peripheral",
        interfaceName : "I2S",
        resources     : resources,
        signalTypes   : {
            AD0Pin       : ['AD0'],
            AD1Pin       : ['AD1'],
            BCLKPin      : ['BCLK'],
            MCLKPin      : ['MCLK'],
            WCLKPin      : ['WCLK'],
        }
    };
    return [i2s];
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
    let modulePin;

    for (let sig in component.signals) {
        let type = component.signals[sig].type;
        if (Common.typeMatches(type, ["I2S"])) modulePin = sig;
    }
    let result = modulePin ? true : false;

    return (result);
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
