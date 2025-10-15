/* original (disabled) validation function */
// function validate(inst, validation){
// 	if(inst.$name === "SYSPLLMUX"){
// 		let inputFreq = inst[inst.inputSelect];
// 		if(inputFreq < 4 || inputFreq > 48) {
// 			validation.logError("SYSPLL cannot be sourced outside the range of 4 MHz to 48 MHz.");
// 		}
// 	}
// }

function validate(inst, validation){
	let clockTreeEn = false;
	if(system.modules["/ti/driverlib/SYSCTL"]){
		clockTreeEn = system.modules["/ti/driverlib/SYSCTL"].$static.clockTreeEn;
	}

	/*
	 * USBCLK Validation
	 * - must always be 60MHz if USB module is enabled
	 * - if USB module is configured as Host, must be sourced from HFXT via SYSPLL1
	 */
	if(inst.$name === "USBCLKMUX"){
		let USBMod = system.modules["/ti/driverlib/USB"];
		if(USBMod && clockTreeEn){
			// Frequency Validation
			if(inst?.USBCLKMUX_out !== 60){
				validation.logError("USBCLK must be set to 60.000 MHz", inst, "USBCLKMUX_out");
				if(system.clockTree.net_usbclk){
					validation.logError("USBCLK must be set to 60.000 MHz", system.clockTree.net_usbclk, "in");
				}
			}
		}
		else if(clockTreeEn){
        	validation.logInfo("USB Peripheral is not currently enabled", inst);
		}
	}
}

function validatePinmux(inst,validation){
	let clockTreeEn = false;
	if(system.modules["/ti/driverlib/SYSCTL"]){
		clockTreeEn = system.modules["/ti/driverlib/SYSCTL"].$static.clockTreeEn;
	}
	/*
	 * USBCLK Validation
	 * - must always be 60MHz if USB module is enabled
	 * - if USB module is configured as Host, must be sourced from HFXT via SYSPLL1
	 */
	if(inst.$name === "USBCLKMUX"){
		let USBMod = system.modules["/ti/driverlib/USB"];
		if(USBMod && clockTreeEn){
			// Source Validation (Host Mode)
			let USBInsts = USBMod.$instances
			for(let singleInst of USBInsts){
				let USBPeripheral = singleInst?.peripheral?.$solution?.peripheralName;
				if(singleInst.mode == "host"){
					// USBCLK Mux
					if(!(_.endsWith(inst.inputSelect, "PLLCLK1_OUT"))){
						validation.logError("USBCLK must be sourced from HFXT via SYSPLL1 when "+USBPeripheral+" is configured as Host", inst, "inputSelect");
					}
					// SYSPLLMUX
					if(system.clockTree.SYSPLLMUX){
						if(!(_.endsWith(system.clockTree.SYSPLLMUX?.inputSelect, "HFCLK"))){
							validation.logError("SYSPLLMUX must be sourced from HFCLK when "+USBPeripheral+" is configured as Host", system.clockTree.SYSPLLMUX, "inputSelect");
						}
					}
					// EXHFMUX
					if(system.clockTree.EXHFMUX){
						if(!(_.endsWith(system.clockTree.EXHFMUX?.inputSelect, "XTAL"))){
							validation.logError("EXHFMUX must be sourced from XTAL when "+USBPeripheral+" is configured as Host", system.clockTree.EXHFMUX, "inputSelect");
						}
					}
					// HFXT
					if(system.clockTree.HFXT){
						if(!system.clockTree.HFXT?.enable){
							validation.logError("HFXT must be enabled when "+USBPeripheral+" is configured as Host", system.clockTree.HFXT, "enable");
						}
					}
				}
			}
		}
	}
}

exports = {
	displayName: "Mux",
	validate: validate,
	validatePinmux: validatePinmux,
	config: [{
		name: "type",
		displayName: "Type",
		default: "Mux",
		readOnly: true,
	}],
	extendConfig: ({ $ipInstance }) => {
		let config = [
			{
				name: "inputSelect",
				displayName: "Input Select", default: $ipInstance.resetValue,
				options: _.map($ipInstance.inPins, ( name ) => ( name )),
			},
			{
				name: $ipInstance.outPins[0].name,
				default: [0, 0],
				getValue: (inst) => inst[inst.inputSelect],
			},
		];
		return config;
	}
};
