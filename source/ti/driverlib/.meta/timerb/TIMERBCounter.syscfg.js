let Common = system.getScript("/ti/driverlib/Common.js");
let EVENT = system.getScript("/ti/driverlib/EVENT.syscfg.js");

/**
 * Parses a time string with units into a value in seconds
 * Supports s, ms, us, ns and scientific notation
 *
 * @param {String} timeStr - Time string with units (e.g., "500 ms", "1.5 s", "100e-6 s")
 * @return {Number} Time in seconds, or null if invalid format
 */
function parseTimeWithUnits(timeStr) {
    if (!timeStr || typeof timeStr !== 'string') {
        return null;
    }

    // Regex to match number followed by optional units
    // Supports scientific notation and various time units
    const regex = /^([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)\s*([a-zA-Z]+)?$/;
    const match = timeStr.trim().match(regex);

    if (!match) {
        return null;
    }

    const value = parseFloat(match[1]);
    const unit = (match[2] || 's').toLowerCase();

    // Convert to seconds based on unit
    switch (unit) {
        case 's':  return value;
        case 'ms': return value * 1e-3;
        case 'us':
        case 'µs': return value * 1e-6;
        case 'ns': return value * 1e-9;
        default:   return null; // Unrecognized unit
    }
}

/**
 * Formats a time value in seconds to a human-readable string with appropriate units
 *
 * @param {Number} timeInSeconds - Time value in seconds
 * @return {String} Formatted time string with units
 */
function formatTimeWithUnits(timeInSeconds) {
    if (timeInSeconds === null || timeInSeconds === undefined || isNaN(timeInSeconds) || timeInSeconds === 0) {
        return "0 ms";
    }

    if (timeInSeconds >= 1) {
        return `${timeInSeconds.toFixed(6)} s`.replace(/\.?0+\s/, ' ');
    } else if (timeInSeconds >= 1e-3) {
        return `${(timeInSeconds * 1e3).toFixed(6)} ms`.replace(/\.?0+\s/, ' ');
    } else if (timeInSeconds >= 1e-6) {
        return `${(timeInSeconds * 1e6).toFixed(6)} µs`.replace(/\.?0+\s/, ' ');
    } else {
        return `${(timeInSeconds * 1e9).toFixed(6)} ns`.replace(/\.?0+\s/, ' ');
    }
}

function getSourceFrequency(inst) {
	const BUSCLK_FREQUENCY = Common.getBUSCLKFreq(inst, "BTIMER");
	let index = inst.index;
	let sourceFrequency = "N/A";

	if(inst.clockSource === "BUSCLK") {
		sourceFrequency = BUSCLK_FREQUENCY;
	}
	else if (inst.clockSource.includes("CNT") && !inst.clockSource.includes(index)) {
        // Extract source counter index from clockSource string (e.g., "CNT0_OVERFLOW" -> 0)
        const sourceCounterIdx = parseInt(inst.clockSource.match(/CNT(\d+)_OVERFLOW/)[1]);
		let allCounters = system.modules["/ti/driverlib/timerb/TIMERBCounter"].$instances;

		// Find the source counter instance
		const sourceCounter = allCounters.find(counter => {
			// Get the counter's index value
			let counterIdx = 0;
			for(let i = 0; i < allCounters.length; i++) {
				if(allCounters[i].$name === counter.$name) {
					counterIdx = i;
					break;
				}
			}
			return counterIdx === sourceCounterIdx;
		});

        if(sourceCounter) {
        	sourceFrequency = sourceCounter.calculatedFrequency;
		}
    }
	return sourceFrequency;
}

/**
 * Calculates the frequency of a timer counter based on its configuration
 * Takes into account cascading from clock source, start source, and reset source
 *
 * @param {Object} inst - The counter instance
 * @param {Array} allCounters - Array of all counter instances
 * @return {Number} The calculated frequency in Hz, or null if cannot be determined
 */
function calculateCounterFrequency(inst) {
	let sourceFrequency = getSourceFrequency(inst);

	if(sourceFrequency == "N/A") {
		return "N/A";
	}

	sourceFrequency = parseInt(sourceFrequency);

	if(inst.loadValue == 0) {
		return 0;
	}

    // Calculate this counter's overflow frequency
    let overflowFrequency = sourceFrequency / inst.loadValue;
    return overflowFrequency;
}

/**
 * Calculates the optimal load value for a counter to achieve a desired period
 *
 * @param {Object} inst - The counter instance
 * @return {Integer} loadValue - The calculated load value
 */
function calculateLoadValueForPeriod(inst) {
    const allCounters = system.modules["/ti/driverlib/timerb/TIMERBCounter"].$instances;
	const desiredPeriod = inst.desiredPeriod;

	// Parse the desired period to seconds
    const desiredPeriodInSeconds = parseTimeWithUnits(desiredPeriod);
    if (desiredPeriodInSeconds === null) {
        return 0;
    }

	let sourceFrequency = getSourceFrequency(inst);

    // Calculate the ideal load value: desired period * input clock frequency
    let idealLoadValue = desiredPeriodInSeconds * sourceFrequency;

    // Check if the desired period is achievable
    if (idealLoadValue < 1) {
        return 0;
    }
    if (idealLoadValue > 65535) {
        return 0;
    }

    // Load value must be a positive integer between 1 and 65535
    let loadValue = Math.round(idealLoadValue);
    loadValue = Math.max(1, Math.min(65535, loadValue));

	return loadValue;
}

function calculateActualPeriod(inst) {
    // Get the same source frequency used for load value calculation
    const sourceFrequency = getSourceFrequency(inst);

	if(sourceFrequency == "N/A") return "N/A";

    // Calculate the actual period using the same frequency source
    const actualPeriodInSeconds = inst.loadValue / sourceFrequency;

    const actualPeriod = formatTimeWithUnits(actualPeriodInSeconds);
    return actualPeriod;
}

function getClockSourceOptions(inst) {
	let timerModules = system.modules["/ti/driverlib/timerb/TIMERBCounter"];
	let source_options = [];
	source_options.push({name: "BUSCLK", displayName: "Bus Clock"});
	for(let i = 0; i < timerModules.$instances.length && i <= inst.index; i++) {
		source_options.push({name: "CNT" + i + "_OVERFLOW", displayName: timerModules.$instances[i].$name + " Overflow"});
	}

	source_options.push({name: "EVENT2", displayName: "GPIO0 Publisher 1"});
	source_options.push({name: "EVENT3", displayName: "GPIO0 Publisher 2"});
	source_options.push({name: "EVENT4", displayName: "GPIO1 Publisher 1"});
	source_options.push({name: "EVENT5", displayName: "GPIO1 Publisher 2"});
	source_options.push({name: "EVENT6", displayName: "DMA Interrupt"});
	source_options.push({name: "SUBSCRIBER_EVENT", displayName: "Generic Subscriber Event"});
	return source_options;
}

function getStartSourceOptions(inst) {
	let timerModules = system.modules["/ti/driverlib/timerb/TIMERBCounter"];
	let start_source_options = [];
	start_source_options.push({name: "UNUSED", displayName: "Unused"});

	if(timerModules) {
		for(let i = 0; (i < timerModules.$instances.length && i < 7); i++) {
			start_source_options.push({name: "CNT" + i + "_OVERFLOW", displayName: timerModules.$instances[i].$name + " Overflow"});
		}
	}

	start_source_options.push({name: "EVENT2", displayName: "GPIO0 Publisher 1"});
	start_source_options.push({name: "EVENT3", displayName: "GPIO0 Publisher 2"});
	start_source_options.push({name: "EVENT4", displayName: "GPIO1 Publisher 1"});
	start_source_options.push({name: "EVENT5", displayName: "GPIO1 Publisher 2"});
	start_source_options.push({name: "EVENT6", displayName: "DMA Interrupt"});
	start_source_options.push({name: "SUBSCRIBER_EVENT", displayName: "Generic Subscriber Event"});
	return start_source_options;
}

function getStopSourceOptions(inst) {
	let timerModules = system.modules["/ti/driverlib/timerb/TIMERBCounter"];

	let stop_source_options = [];
	stop_source_options.push({name: "UNUSED", displayName: "Unused"});

	if(timerModules) {
		for(let i = 0; (i < timerModules.$instances.length && i < 7); i++) {
			stop_source_options.push({name: "CNT" + i + "_OVERFLOW", displayName: timerModules.$instances[i].$name + " Overflow"});
		}
	}

	stop_source_options.push({name: "EVENT2", displayName: "GPIO0 Publisher 1"});
	stop_source_options.push({name: "EVENT3", displayName: "GPIO0 Publisher 2"});
	stop_source_options.push({name: "EVENT4", displayName: "GPIO1 Publisher 1"});
	stop_source_options.push({name: "EVENT5", displayName: "GPIO1 Publisher 2"});
	stop_source_options.push({name: "EVENT6", displayName: "DMA Interrupt"});
	stop_source_options.push({name: "SUBSCRIBER_EVENT", displayName: "Generic Subscriber Event"});
	return stop_source_options;
}

function getResetSourceOptions(inst) {
	let timerModules = system.modules["/ti/driverlib/timerb/TIMERBCounter"];

	let reset_source_options = [];
	reset_source_options.push({name: "UNUSED", displayName: "Unused"});

	for(let i = 0; i < timerModules.$instances.length && i <= inst.index; i++) {
		reset_source_options.push({name: "CNT" + i + "_OVERFLOW", displayName: timerModules.$instances[i].$name + " Overflow"});
	}

	reset_source_options.push({name: "EVENT2", displayName: "GPIO0 Publisher 1"});
	reset_source_options.push({name: "EVENT3", displayName: "GPIO0 Publisher 2"});
	reset_source_options.push({name: "EVENT4", displayName: "GPIO1 Publisher 1"});
	reset_source_options.push({name: "EVENT5", displayName: "GPIO1 Publisher 2"});
	reset_source_options.push({name: "EVENT6", displayName: "DMA Interrupt"});
	reset_source_options.push({name: "SUBSCRIBER_EVENT", displayName: "Generic Subscriber Event"});
	return reset_source_options;
}

function onChangeClockSource(inst, ui) {
	if(inst.clockSource != "BUSCLK") {
		ui.sourceClockFrequency.hidden = true;
		ui.periodRange.hidden = true;
		ui.loadValue.hidden = true;
		ui.desiredPeriod.hidden = true;
		ui.actualPeriod.hidden = true;
		ui.calculatedFrequency.hidden = true;
		ui.userLoadValue.hidden = false;
	}
	else {
		ui.sourceClockFrequency.hidden = false;
		ui.periodRange.hidden = false;
		ui.loadValue.hidden = false;
		ui.desiredPeriod.hidden = false;
		ui.actualPeriod.hidden = false;
		ui.calculatedFrequency.hidden = false;
		ui.userLoadValue.hidden = true;
	}
}

let basicConfig = [
	{
		name: "index",
		displayName: "Counter Index",
		hidden: false,
		default: 0,
		readOnly: true,
		getValue: (inst) => {
			let parent = inst.$ownedBy.$name;
			let counters = system.modules["/ti/driverlib/timerb/TIMERBCounter"].$instances;
			counters = counters.filter((counter) => counter.$ownedBy.$name == parent);
			for(let i = 0; i < counters.length; i++) {
				if(counters[i].$name == inst.$name && counters[i].$ownedBy.$name === parent) {
					return i;
				}
			}
			return -1;
		}
	},
    {
		name: "clockSource",
		displayName: "Clock Selection",
		hidden: false,
		default: "BUSCLK",
		options: getClockSourceOptions,
		onChange: onChangeClockSource
    },
	{
		name: "sourceClockFrequency",
		displayName: "Source Clock Frequency (Hz)",
		description: "The frequency of the selected clock source",
		longDescription: "Displays the frequency of the selected clock source. If using another timer's overflow as the source, this will show that timer's calculated overflow frequency.",
		hidden: false,
		readOnly: true,
		default: "N/A",
		getValue: (inst) => {
			const frequency = getSourceFrequency(inst);
			if(frequency == "N/A") {
				return frequency;
			}
			const sourceFrequency = parseInt(frequency);
			if (sourceFrequency >= 1e6) {
				return `${(sourceFrequency / 1e6).toFixed(2)} MHz`;
			} else if (sourceFrequency >= 1e3) {
				return `${(sourceFrequency / 1e3).toFixed(2)} kHz`;
			} else {
				return `${sourceFrequency.toFixed(2)} Hz`;
			}
		},
	},
	{
		name: "periodRange",
		displayName: "Valid Period Range",
		description: "The minimum and maximum period values possible with the current source clock",
		longDescription: "Shows the valid range of periods that can be achieved with the current source clock frequency. The minimum period is 1 clock cycle and the maximum is 65535 clock cycles.",
		hidden: false,
		readOnly: true,
		default: "",
		getValue: (inst) => {
			const frequency = getSourceFrequency(inst);
			if(frequency == "N/A") {
				return frequency;
			}
			const sourceFrequency = parseInt(frequency);
			if(sourceFrequency == 0) {
				return `N/A`;
			}
			else {
				// Calculate min and max periods
				const minPeriod = 1 / sourceFrequency;  // Time for 1 clock cycle
				const maxPeriod = 65535 / sourceFrequency;  // Time for 65535 clock cycles (max timer value)
				const resolution = minPeriod;  // Resolution is equal to one clock cycle
				const formatWithRounding = (timeInSeconds) => {
					if (timeInSeconds >= 1) {
						return `${Math.floor(timeInSeconds * 100) / 100} s`;
					} else if (timeInSeconds >= 1e-3) {
						return `${Math.floor(timeInSeconds * 1e5) / 100} ms`;
					} else if (timeInSeconds >= 1e-6) {
						return `${Math.floor(timeInSeconds * 1e8) / 100} µs`;
					} else {
						return `${Math.floor(timeInSeconds * 1e11) / 100} ns`;
					}
				};
				return `Min: ${formatWithRounding(minPeriod)}, Max: ${formatWithRounding(maxPeriod)}, Resolution: ${formatWithRounding(resolution)}`;
			}
		}
	},
    {
        name: "startSource",
		displayName: "Start Source",
		hidden: false,
		default: "UNUSED",
		options: getStartSourceOptions
    },
    {
        name: "stopSource",
		displayName: "Stop Source",
		hidden: false,
		default: "UNUSED",
		options: getStopSourceOptions
    },
	{
        name: "resetSource",
		displayName: "Reset Source",
		hidden: false,
		default: "UNUSED",
		options: getResetSourceOptions
    },
	{
		name: "desiredPeriod",
		displayName: "Desired Timer Period",
		description: "Desired Timer Period in seconds",
		longDescription: `The desired timer period is an input string that can set the period of the timer.
						  Valid formats:
	* Units with supported prefixes (ms, µs, ns):
		* 234.56 ms, 150ns
	* Units without a prefix:
		* 0.0123 s, 1.0 s
	* Exponential form with units
		* 123.45e-8 s`,
		hidden: false,
		default: "0 ms",
	},
	{
		name: "actualPeriod",
		displayName: "Actual Timer Period",
		hidden: false,
		readOnly: true,
		default: "",
		getValue: (inst) => { return calculateActualPeriod(inst) }
	},
	{
		name: "loadValue",
		displayName: "Load Value",
		hidden: false,
		default: 0,
		getValue: (inst) => { return calculateLoadValueForPeriod(inst) }
	},
	{
		name: "userLoadValue",
		displayName: "Load Value",
		hidden: true,
		default: 0,
	},
	{
		name: "calculatedFrequency",
		displayName: "Counter Overflow Frequency (Hz)",
		hidden: false,
		readOnly: true,
		default: "0",
		getValue: (inst) => {
			let counterFrequency =  calculateCounterFrequency(inst)
			if(counterFrequency == "N/A") {
				return counterFrequency;
			} else if (counterFrequency >= 1e6) {
				return `${(counterFrequency / 1e6).toFixed(2)} MHz`;
			} else if (counterFrequency >= 1e3) {
				return `${(counterFrequency / 1e3).toFixed(2)} kHz`;
			} else {
				return `${counterFrequency.toFixed(2)} Hz`;
			}
		}
	},
	{
		name: "startCounter",
		displayName: "Start Counter",
		hidden: false,
		default: false
	}
]

let config = [
    {
        name: "$name",
        defaultPrefix: "COUNTER_",
        hidden: false
    },
	{
        name: "GROUP_BASIC",
        displayName: "Basic Configuration",
        description: "",
        longDescription: "",
        collapsed: false,
        config: basicConfig,
    },
];

/*  ======== pinmuxRequirements ========
 *  Returns peripheral pin requirements of the specified instance
 *
 *  param inst    - a fully configured module instance
 *
 *  returns req[] - an array of pin requirements needed by inst
 */
function pinmuxRequirements(inst)
{
	return [];
}

function validate(inst, validation, ui)
{
	let desiredPeriodInSeconds = parseTimeWithUnits(inst.desiredPeriod);
	if(desiredPeriodInSeconds != 0 && inst.loadValue == 0) {
		let sourceFrequency = getSourceFrequency(inst);
		validation.logError(`Desired period (${inst.desiredPeriod}) is out of range for the input clock frequency (${sourceFrequency} Hz). Minimum period: ${formatTimeWithUnits(1/sourceFrequency)}, Maximum period: ${formatTimeWithUnits(65535/sourceFrequency)}` , inst, "desiredPeriod");
	}
	if(inst.sourceClockFrequency == "N/A" && inst.userLoadValue == 0) {
		validation.logWarning(`Source clock frequency is unknown, please manually enter desired load value for counter.` , inst, "userLoadValue");
	}
}

function validatePinmux(inst, validation)
{
	//Module has no pins
}

function moduleInstances(inst) {
    let modules = [];
    return modules;
}

exports = {
    config: config,
    pinmuxRequirements: pinmuxRequirements,
    validate: validate,
    validatePinmux: validatePinmux,
    /* Added for cleaner error message */
    displayName: "MSPM0 TimerB Counter",
};
