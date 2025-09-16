
function validateGates(inst, validation) {
	let clockTreeEn = false;
	if(system.modules["/ti/driverlib/SYSCTL"]){
		clockTreeEn = system.modules["/ti/driverlib/SYSCTL"].$static.clockTreeEn;
	}

	if(inst.$name === "MFPCLKGATE"){
		if(inst[inst.$ipInstance.outPins[0].name] > 4){
			// this is only possible if sourced from the HFCLK and the divider needs to be increased.
			let divMod = system.modules["/ti/clockTree/divider.js"];
			let divInst = _.find(divMod.$instances, ['$name', 'HFCLK4MFPCLKDIV']);
			if(_.isUndefined(divInst)){ throw "cannot find instance HFCLK4MFPCLKDIV"; }
			validation.logError("MFPCLK is limited to below 4MHz.", system.clockTree["net_mfpclk"])
			validation.logError("MFPCLK is limited to below 4MHz.", divInst, "divideValue");
		}
		/* Special Case: DAC12 Validation */
		if(system.modules["/ti/driverlib/SYSCTL"]){
			if(system.modules["/ti/driverlib/SYSCTL"].$static.clockTreeEn){
				let DACMod = system.modules["/ti/driverlib/DAC12"];
				if (DACMod){
					if (DACMod.$static.dacEnable && !inst.enable) {
						validation.logError("MFPCLK must be enabled when DAC12 peripheral is added.", inst, ["enable"]);
					}
				}
			}
		}

	}
	if(inst.$name === "EXCLKGATE"){
		let modPF = system.modules["/ti/clockTree/pinFunction.js"];
		let clkOutInst = _.find(modPF.$instances, ['$name', 'CLKOUT']);

		if(_.isUndefined(clkOutInst)){
			throw 'could not find the CLKOUT pin function instance';
		}
		if(inst.enable & !clkOutInst.enable){
			validation.logWarning("The CLKOUT gate is enabled but the CLKOUT pin is not enabling the GPIO pin", clkOutInst, "enable");
		}
	}

	if(inst.$name === "MFCLKGATE"){
		if(!inst.enable && clockTreeEn){
			let vrefInstance = system.modules["/ti/driverlib/VREF"];
			if (vrefInstance){
				if(vrefInstance.$static.advClockConfigEnable){
					if((vrefInstance.$static.advClkSrc == "DL_VREF_CLOCK_MFCLK")){
						validation.logError("MFCLK must be enabled for the current configuration.", inst, ["enable"]);
					}
				}
			}
		}
	}
	if(inst.$name === "EXCLKGATE"){
		let modPF = system.modules["/ti/clockTree/pinFunction.js"];
		let clkOutInst = _.find(modPF.$instances, ['$name', 'CLKOUT']);

		if(_.isUndefined(clkOutInst)){
			throw 'could not find the CLKOUT pin function instance';
		}
		if(inst.enable & !clkOutInst.enable){
			validation.logWarning("The CLKOUT gate is enabled but the CLKOUT pin is not enabling the GPIO pin", clkOutInst, "enable");
		}
	}
	if(inst.$name === "EXCLKGATE"){
		let modPF = system.modules["/ti/clockTree/pinFunction.js"];
		let clkOutInst = _.find(modPF.$instances, ['$name', 'CLKOUT']);

		if(_.isUndefined(clkOutInst)){
			throw 'could not find the CLKOUT pin function instance';
		}
		if(inst.enable & !clkOutInst.enable){
			validation.logWarning("The CLKOUT gate is enabled but the CLKOUT pin is not enabling the GPIO pin", clkOutInst, "enable");
		}
	}
	if(inst.$name === "USBFLLGATE"){
		if(clockTreeEn){
			// USB Module
			let USBMod = system.modules["/ti/driverlib/USB"];
			// Check LFCLK is enabled
			/*
				LFCLK is enabled by default, but LFXT sources can be disabled
				Check if LFFCLKEXT / LFXT are properly enabled as needed
			*/
			if(inst.enable){
				if(_.endsWith(system.clockTree?.LFXTMUX?.inputSelect, "TRUE")){
					if(_.endsWith(system.clockTree?.EXLFMUX?.inputSelect, "DIG")){
						if(!system.clockTree?.LFCLKEXT?.enable){
							let lfclkextInst = system.clockTree.LFCLKEXT;
							validation.logError("LFCLK Source must be enabled when used as USBFLL reference", inst, "enable");
							validation.logError("LFCLK Source must be enabled when used as USBFLL reference", lfclkextInst, "enable");
						}
					}
					else if(_.endsWith(system.clockTree?.EXLFMUX?.inputSelect, "XTAL")){
						if(!system.clockTree?.LFXT?.enable){
							let lfxtInst = system.clockTree.LFXT;
							validation.logError("LFCLK Source must be enabled when used as USBFLL reference", inst, "enable");
							validation.logError("LFCLK Source must be enabled when used as USBFLL reference", lfxtInst, "enable");
						}
					}
				}
			}
			// USBFLL source validation
			/*
				1) If used as HSCLK source, check if enabled
				2) If used as USBCLK reference, check if enabled [ may be automatic by frequency check ]
			*/
			if(!inst.enable){
				if(_.endsWith(system.clockTree?.HSCLKMUX?.inputSelect, "USBFLL")){
					validation.logError("USBFLL must be enabled when used as HSCLK Source", inst, "enable");
				}
				if(USBMod && _.endsWith(system.clockTree?.USBCLKMUX?.inputSelect, "USBFLL")){
					validation.logError("USBFLL must be enabled when used as USBCLK Source", inst, "enable");
				}
			}
		}
	}

}

let USBFLL_extendedConfig = [
	{
		name: "referenceSource",
		default: "LFCLK",
		options: [
			{name: "LFCLK"},
			{name: "SOF"}
		]
	}
];

exports = {
	displayName: "Gate",
	validate: validateGates,
	config: [{
		name: "type",
		displayName: "Type",
		default: "Gate",
		readOnly: true,
	}],
	extendConfig: ({ $ipInstance }) => {
		let isFixed = false;
		if($ipInstance.fixed == true){
			/* option to remove enable */
			isFixed = true;
		}

		const enable = {
			name: "enable",
			displayName: "Enable",
			default: false,
		};

		const pin = {
			name: $ipInstance.outPins[0].name,
			default: [0, 0],
			getValue: (inst) => {
				if (inst.enable !== false) {
					return inst[$ipInstance.inPins[0].name];
				}
				return 0;
			},
		};
		let config = isFixed?[pin]:[enable, pin];
		if($ipInstance.name === "USBFLLGATE"){
			config = [enable,pin]; // Exception for USBFLL since Fixed property is not added
			config = config.concat(USBFLL_extendedConfig);
		}
		return config;
	},
};
