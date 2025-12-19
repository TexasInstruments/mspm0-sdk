let Common = system.getScript("/ti/driverlib/Common.js");

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
		if($ipInstance.name === "SYSPLLMUX" && Common.isdeviceAffected_SYSPLL_ERR_01()) {
			config.push(
				{
					name: "GROUP_WORKAROUND_SYSPLL_ERR_01",
					displayName: "SYSPLL Validation",
					description: "(SYSPLL_ERR_01)",
					longDescription:
`Use FCC to check for the validity of the SYSPLL frequency. This configuration
serves as a workaround for SYSPLL_ERR_01.
\n* SYSPLL_ERR_01: When setting the SYSPLLEN bit to 1 in SYSCTL HSCLKEN register,
the SYSPLL will run the phase locked loop search. The search can potentially
fail where the frequency will not be set to the correct value, instead the
resultant frequency will be drastically different than the configured
frequency.`,
					collapsed: false,
					config: [
						{
							name: "enableWorkaround_SYSPLL_ERR_01",
							displayName: "Validate SYSPLL Frequency Lock",
							default: true,
							description: "Use FCC to check for the validity of the SYSPLL frequency (SYSPLL_ERR_01)",
							longDescription:
`This option serves as a workaround for SYSPLL_ERR_01, when enabled it will use
FCC to check for the validity of the SYSPLL frequency
`,
							hidden: false,
						},
						// FCC Limitation boundary
						{
							name: "workaroundfccBoundaryStr_SYSPLL_ERR_01",
							displayName: "FCC Limitation Boundary (%)",
							description: "Set the limitation boundary percentage used for validating the FCC Ratio",
							longDescription:
`Set the limitation boundary percentage used for validating the FCC Ratio`,
							default: 0.3,
							range  : [0,100],
						},
						// Default Period Calculation
						{
							name: "overrideworkaroundFCCPeriod_SYSPLL_ERR_01",
							displayName: "Override Calculated FCC Period",
							default: false,
							onChange: (inst,ui)=>{
								ui.workaroundFCCPeriod_SYSPLL_ERR_01.hidden = !inst.overrideworkaroundFCCPeriod_SYSPLL_ERR_01;
								ui.workaroundFCCPeriodCalculated_SYSPLL_ERR_01.hidden = inst.overrideworkaroundFCCPeriod_SYSPLL_ERR_01;
							}
						},
						{
							name: "workaroundFCCPeriodCalculated_SYSPLL_ERR_01",
							displayName: "Calculated FCC Period",
							description: "Set the number of rising-edge to rising-edge period for FCC",
							longDescription: `Set the number of rising-edge to rising-edge period for FCC (for validation only)`,
							default: 1,
							range  : [1,32],
							getValue: (inst)=>{
								/*
								 * FCC Periods = (4 * 1.6 / SYSPLL_SOURCE_FREQ / FCC Boundary) (Round the result up)
								 * - 1.6% is max error percentage
								 * - SYSPLL Source Frequency in MHz
								 * - FCC Boundary is user input
								 */
								let fccPeriod = Math.ceil(4 * 1.6 / (inst.SYSPLLMUX_out ) / inst.workaroundfccBoundaryStr_SYSPLL_ERR_01);
								if(fccPeriod>32 || fccPeriod<1 || fccPeriod === NaN){ return 1};
								return fccPeriod;
							},
						},
						{
							name: "workaroundFCCPeriod_SYSPLL_ERR_01",
							displayName: "FCC Periods",
							description: "Set the number of rising-edge to rising-edge period for FCC",
							longDescription: `Set the number of rising-edge to rising-edge period for FCC (for validation only)`,
							default: 1,
							range  : [1,32],
							hidden: true,
						},
					],
				}
			)
		}
		return config;
	}
};
