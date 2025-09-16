/*
 *  ======== TIMERBMSPM0.syscfg.js ========
 */

"use strict";

/* get Common /ti/driverlib utility functions */
let Common = system.getScript("/ti/driverlib/Common.js");
let EVENT = system.getScript("/ti/driverlib/EVENT.syscfg.js");

function onChangeTIMERBProfile(inst, ui) {

}

function pinmuxRequirements(inst) {
    let timerb = {
        name          : "peripheral",
        displayName   : "Basic Timer Peripheral",
        interfaceName : "BTIMER",
        resources     : [],
        signalTypes   : {
        }
    };
    return [timerb];
}

let TIMERBConfig = [
    /****** PROFILES CONFIGURATION *******/
    {
        name: "GROUP_PROFILE",
        displayName: "Quick Profiles",
        description: "",
        collapsed: false,
        config: [
            {
                name        : "profile",
                displayName : "TIMERB Profiles",
                description : 'Pre-defined profiles for typical TIMERB configurations',
                longDescription: `
The Basic Timer (TIMERB) module is an array of counters which can generate periodic
interrupts, be concatenated to create longer time durations, and measure timing
properties of external events.

The Quick Profile Options are:
* **Basic Timer with 1 counter**:
    * Counter 0 in use with 1 milisecond period
* **Cascaded counters for long duration**:
    * Counter 0 and 1 in use, with Counter 1 using Counter 0's overflow as clock source
    * Creates a 1-second period on Counter 1
* **Custom**:
    * Allows custom configuration.`,
                hidden      : false,
                default     : "CUSTOM",
                options     : [
                    {name: "CONFIG_PROFILE_1", displayName: "Basic Timer with 1 counter"},
                    {name: "CONFIG_PROFILE_2", displayName: "Cascaded counters for long duration"},
                    {name: "CUSTOM", displayName: "Custom"},
                ],
                onChange    : onChangeTIMERBProfile,
            },
			{
				name: "maxCounters",
				displayName: "Available Counters",
				description: "",
				hidden: true,
				default: 0,
				getValue: (inst) => {
					try {
						return  parseInt(system.deviceData.peripherals[inst.peripheral.$solution.peripheralName].attributes.SYS_NUM_COUNTERS);
					}
					catch(e) {
						return 0;
					}
				}
			}
        ],
    },
	{
		name: "GROUP_COUNTERS",
        displayName: "Counter Configurations",
        description: "",
        collapsed: false,
		config: [

		]
	}
];

function getEnabledOptionsEvents(inst) {
	let enabledOptions = [];
    let maxCounters = parseInt(system.deviceData.peripherals[inst.peripheral.$solution.peripheralName].attributes.SYS_NUM_COUNTERS);
	let timerModules = system.modules["/ti/driverlib/timerb/TIMERBCounter"];
	let num_enabled = 0;
	if(timerModules) num_enabled = timerModules.$instances.length;

	for(let i = 0; i < num_enabled; i++) {
		enabledOptions.push({name: "CNT" + i + "_OVF_EVENT", displayName: timerModules.$instances[i].$name + " Overflow Event"});
		enabledOptions.push({name: "CNT" + i + "_START_EVENT", displayName: timerModules.$instances[i].$name + " Start Event"});
		enabledOptions.push({name: "CNT" + i + "_STOP_EVENT", displayName: timerModules.$instances[i].$name + " Stop Event"});
	}

	for(let i = num_enabled; i < maxCounters; i++) {
		enabledOptions.push({name: "CNT" + i + "_OVF_EVENT", displayName: "Counter " + i + " Overflow Event"});
		enabledOptions.push({name: "CNT" + i + "_START_EVENT", displayName: "Counter " + i + " Start Event"});
		enabledOptions.push({name: "CNT" + i + "_STOP_EVENT", displayName: "Counter " + i + " Stop Event"});
	}
	return enabledOptions;
}

function getDisabledOptionsEvents(inst) {
	let disabledOptions = [];
    let maxCounters = parseInt(system.deviceData.peripherals[inst.peripheral.$solution.peripheralName].attributes.SYS_NUM_COUNTERS);
	let currentEnabledCounters = 0;
	if(system.modules["/ti/driverlib/timerb/TIMERBCounter"]) {
		currentEnabledCounters = system.modules["/ti/driverlib/timerb/TIMERBCounter"].$instances.length;
	}

	for(let i = currentEnabledCounters; i < maxCounters; i++) {
		disabledOptions.push({name: "CNT" + i + "_OVF_EVENT", displayName: "Counter " + i + " Overflow",  reason: "Timer Counter " + i + " not in use"});
		disabledOptions.push({name: "CNT" + i + "_START_EVENT", displayName: "Counter " + i + " Start", reason: "Timer Counter " + i + " not in use"});
		disabledOptions.push({name: "CNT" + i + "_STOP_EVENT", displayName: "Counter " + i + " Stop", reason: "Timer Counter " + i + " not in use"});
	}

	return disabledOptions;
}

// Add Interrupt and Event configuration
TIMERBConfig = TIMERBConfig.concat([
    {
        name: "GROUP_INTERRUPTS",
        displayName: "Interrupt Configuration",
        longDescription: "This section allows enabling/disabling interrupts for each counter",
        collapsed: true,
        config: [
			{
				name: "enabledInterrupts",
				displayName: "Enable Interrupts",
				hidden: false,
				default: [],
                options: (inst) => getEnabledOptionsEvents(inst),
                minSelections: 0,
                getDisabledOptions: (inst) => getDisabledOptionsEvents(inst)
			}
        ]
    },
	{
        name: "eventConfig",
        displayName: "Event Configuration",
        longDescription: `Specifies the Timer Events that are to trigger an Event Condition and where the events are to be published`,
        collapsed: true,
        config: [
            {
                name: "event1PublisherChannel",
                displayName: "Event Publisher Channel ID",
                description: "Specifies the Channel For Events",
                longDescription: `Specifies the Channel to which Timer Events will be published for use by other modules`,
                default: 0,
                options: (inst) => {
                    return EVENT.getPublisherOptions(inst);
                },
                getDisabledOptions: (inst) => {
                    return EVENT.getPublisherDisabledOptions(inst);
                },
                onChange: (inst,ui) => {
                    ui.event1ControllerInterruptEn.hidden = (inst.event1PublisherChannel == 0);
                }
            },
            {
                name: "event1ControllerInterruptEn",
                displayName: "Event Controller Interrupts",
                description: "Determines the Timer Events that will trigger an Event condition",
                longDescription: `Selects all possible timer Events that will create an Event condition`,
                hidden: true,
                default: [],
                options: (inst) => getEnabledOptionsEvents(inst),
                minSelections: 0,
                getDisabledOptions: (inst) => getDisabledOptionsEvents(inst)
            },
            {
                name: "subscriberPort",
                displayName: "Enable Subscriber Port",
                description: "Enables the subscriber port to be used",
                longDescription: `Specifies the subscriber port to be used`,
                default: false,
                onChange: (inst, ui) => {
                    ui.subscriberChannel.hidden = (inst.subscriberPort == "Disabled");
                }
            },
            {
                name: "subscriberChannel",
                displayName: "Subscriber Port Channel Selection",
                description: "Specifies the channel for a subscriber port",
                longDescription: `Specifies the event subscriber channel`,
                hidden: true,
                default: 0,
                options: (inst) => {
                    return EVENT.getSubscriberOptions(inst);
                },
                getDisabledOptions: (inst) => {
                    return EVENT.getSubscriberDisabledOptions(inst);
                },
			},
			{
                name: "eventInfo",
                description: "information about configured events that's passed on to the EVENT module",
                default: [""],
                hidden: true,
                options: (inst) => {
                    let options = [{name: ""}]
                    if(inst.event1PublisherChannel != 0){
                        options.push({name: inst.event1PublisherChannel.toString() + "pub"});
                    }
                    if((inst.subscriberPort) && (inst.subscriberChannel != 0)){
                        options.push({name: inst.subscriberChannel.toString() + "sub"});
                    }
                    return options;
                },
                getValue: (inst) => {
                    let result = [];
                    if(inst.event1PublisherChannel != 0){
                        result.push(inst.event1PublisherChannel.toString() + "pub");
                    }
                    if((inst.subscriberPort) && (inst.subscriberChannel != 0)){
                        result.push(inst.subscriberChannel.toString() + "sub");
                    }
                    if(result.length == 0){
                        result = [""];
                    }
                    return result;
                }
            }
		]
	}
]);

function validate(inst, validation)
{
	let timerModules = system.modules["/ti/driverlib/timerb/TIMERBCounter"];
	// Don't validate until inst.maxCounters has a non-zero value
	if(timerModules && timerModules.$instances.length > inst.maxCounters && inst.maxCounters != 0) {
		validation.logError(`Selected peripheral supports up to ${inst.maxCounters} basic counters.` , inst, "associatedTimer");
	}
}

/*
 *  ======== moduleInstances ========
 */
function moduleInstances(inst) {
	let modInstances = [];

	if(inst.profile == "CONFIG_PROFILE_1") {
		let modInst = {
			name: "associatedTimer",
			displayName: "Counter Configuration",
			useArray: true,
			defaultInstanceCount: 1,
			minInstanceCount: 0,
			collapsed: false,
			group: "GROUP_COUNTERS",
			moduleName: "/ti/driverlib/timerb/TIMERBCounter",
			hidden: false,
			args: {
				desiredPeriod: "1 ms"
			},
			requiredArgs: {
			}
		};
		modInstances.push(modInst);
	}
	else if(inst.profile == "CONFIG_PROFILE_2") {
		let modInst = {
			name: "associatedTimer1",
			displayName: "Counter Configuration",
			useArray: true,
			defaultInstanceCount: 2,
			minInstanceCount: 2,
			collapsed: false,
			moduleName: "/ti/driverlib/timerb/TIMERBCounter",
			hidden: false,
			group: "GROUP_COUNTERS",
			fixedArgs: [{desiredPeriod : "1 ms", }],
			args: {
				clockSource: "CNT0_OVERFLOW",
				desiredPeriod: "1 s"
			},
			requiredArgs: {
			}
		};
		modInstances.push(modInst);
	}
	/* Custom Profile */
	else {
		let modInst = {
			name: "associatedTimer",
			displayName: "",
			useArray: true,
			defaultInstanceCount: 0,
			minInstanceCount: 0,
			collapsed: false,
			moduleName: "/ti/driverlib/timerb/TIMERBCounter",
			group: "GROUP_COUNTERS",
			hidden: false,
			args: {
			},
			requiredArgs: {
			}
		};
		modInstances.push(modInst);
	}

	return modInstances;
}

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base module configuration
 */
let devSpecific = {
    config: TIMERBConfig,
    validate: validate,
    pinmuxRequirements: pinmuxRequirements,
    moduleInstances: moduleInstances,
    templates: {
        boardc: "/ti/driverlib/timerb/TIMERB.Board.c.xdt",
        boardh: "/ti/driverlib/timerb/TIMERB.Board.h.xdt",
        Call: true,
        Reset: true,
        Power: true,
        GPIO: false,
        Function: true
    },
};

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic module module to
 *  allow us to augment/override as needed for the MSPM0.
 */
function extend(base)
{
    /* merge and overwrite base module attributes */
    Object.assign(base, devSpecific);
    return (base);
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base module module */
    extend: extend
};
