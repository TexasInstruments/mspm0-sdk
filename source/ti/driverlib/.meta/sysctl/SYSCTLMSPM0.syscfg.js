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
 *  ======== sysctlTimerMSP432.js ========
 */

"use strict";

/* get Common /ti/driverlib utility functions */
let Common = system.getScript("/ti/driverlib/Common.js");
let Options = system.getScript("/ti/driverlib/sysctl/SYSCTLMSPM0options.js")

function validatePowerPolicy(inst, validation){
    if(inst.runPowerPolicy != "RUN0"){
        validation.logInfo("Current Power Policy with CPU Enabled is "+ inst.runPowerPolicy + ".", inst);
    }

    if((inst.runPowerPolicy.slice(-1) !== inst.powerPolicy.slice(-1)) && inst.powerPolicy.match(/SLEEP\d/)){
        validation.logWarning("Incompatible transition between RUN and SLEEP power policies of different numbers. The end numbers must match. Sleep will be changed to match RUN.", inst, ["runPowerPolicy", "powerPolicy"]);
    }

    if(inst.runPowerPolicy == "RUN2" && inst.powerPolicy == "STOP0"){
        validation.logInfo("SYSOSC will remain disabled in STOP0 since jumping from RUN2", inst, ["powerPolicy"]);
    }

    if(inst.runPowerPolicy.match(/RUN[12]/) && inst.powerPolicy == "STOP1"){
        validation.logWarning("Illogical transition between RUN policy and STOP1", inst, ["runPowerPolicy", "powerPolicy"]);
    }


    // check that MF/MFP clocks are disabled in RUN1/2
    if(inst.runPowerPolicy != "RUN0"){
        let errStub = " cannot be enabled and used during RUN1 or RUN2 policy."+
        "\nDisable or change the run policy by sourcing MCLK from something other than LFCLK";
        if(inst.clockTreeEn){
            let mod = system.modules["/ti/clockTree/gate.js"];
            let mfclkgate = _.find(mod.$instances, ['$name', 'MFCLKGATE']);
            let mfpclkgate = _.find(mod.$instances, ['$name', 'MFPCLKGATE']);
            let gates = [ mfclkgate, mfpclkgate ]
            let offendingGates = _.filter(gates, (g) => g.enable === true);
            if(offendingGates.length > 0){
                _.each(offendingGates, (h) => {
                    validation.logError(_.trimEnd(h.$name, "GATE") + errStub, h, "enable");
                })
            }
        } else {
            if(inst.MFPCLKEn){
                validation.logError("MFPCLK" + errStub, inst, "MFPCLKEn");
            }
            if(inst.MFCLKEn){
                validation.logError("MFCLK" + errStub, inst, "MFCLKEn");
            }
        }
    }

    // MFPSourcing in STOP0 mode
    if(Common.isDeviceM0G() && inst.powerPolicy.match(/STOP[01]/)) {
        let errStr = "MFPCLK cannot be sourced from HFCLK and remain operational in any STOP mode";
        if(inst.clockTreeEn) {
            let mod = system.modules["/ti/clockTree/gate.js"];
            let mfpclkGate = _.find(mod.$instances, ['$name', 'MFPCLKGATE']);
            mod = system.modules["/ti/clockTree/mux.js"];
            let mfpclkMux = _.find(mod.$instances, ['$name', 'MFPCLKMUX']);
            if(mfpclkGate.enable && mfpclkMux.inputSelect.match(/HFCLK/)){
                validation.logError(errStr, mfpclkMux, "inputSelect");
            }
        } else {
            if(inst.MFPCLKEn && inst.MFPCLKSource === "HFCLK") {
                validation.logError(errStr, inst, "MFPCLKSource");
            }
        }
    }

    // check for HSCLKs in RUN1/2
    if(Common.isDeviceM0G() && inst.runPowerPolicy != "RUN0"){
        if(inst.clockTreeEn){

            // Checks for remaining HFCLK enabled
            let mod = system.modules["/ti/clockTree/divider.js"];

            let pllClk0Inst = _.find(mod.$instances, ['$name', 'PLL_CLK0_DIV']);
            let pllClk1Inst = _.find(mod.$instances, ['$name', 'PLL_CLK1_DIV']);
            let pllClk2XInst = _.find(mod.$instances, ['$name', 'PLL_CLK2X_DIV']);

            let pllInstances = [ pllClk0Inst, pllClk1Inst, pllClk2XInst ];

            mod = system.modules["/ti/clockTree/mux.js"];
            let hsclkMuxInst = _.find(mod.$instances, ['$name', 'HSCLKMUX']);
            let canclkMuxInst = _.find(mod.$instances, ['$name', 'CANCLKMUX']);
            let fccMux = _.find(mod.$instances, ['$name', 'FCCSELCLKMUX']);
	        let exclkMux = _.find(mod.$instances, ['$name', 'EXCLKMUX']);
        	let exhfclkMux = _.find(mod.$instances, ['$name', 'EXHFMUX']);
            let mfpclkMux = _.find(mod.$instances, ['$name', 'MFPCLKMUX']);

            let pllMuxes = [ hsclkMuxInst, canclkMuxInst, fccMux, exclkMux ];
            // TODO: later add ADC mux if applicable
            // canclk mux is not necessary because code is not generated on behalf of
            // CANCLK
            let hfMuxes = [ hsclkMuxInst, exclkMux, fccMux, mfpclkMux ];

            if(_.isUndefined(pllClk0Inst) || _.isUndefined(pllClk1Inst) ||
               _.isUndefined(pllClk2XInst) ||
               _.isUndefined(hsclkMuxInst) || _.isUndefined(canclkMuxInst) ||
               _.isUndefined(fccMux) || _.isUndefined(exclkMux) ||
               _.isUndefined(exhfclkMux))
            {
                throw 'Not all CLK Power check elements are defined appropriately';
            }


            let offendingPLL = _.filter(pllInstances, (i) => i.enable === true);
            let offendingMux = _.filter(pllMuxes, (m) => !_.isNull(m.inputSelect.match(/PLL/)));
            if(offendingPLL.length > 0){
                let errStr = "SYSPLL cannot be enabled during RUN1 or RUN2 policy.\nChange Run Policy by sourcing MCLK from something other than LFCLK";
                _.each(offendingPLL.concat(offendingMux), (p) => validation.logError(errStr, p));
            }
            let offendingHF = _.filter(hfMuxes, (m) => !_.isNull(m.inputSelect.match(/HFCLK/)))
            if(offendingHF.length > 0){
                let errStr = "HFCLK cannot be enabled and used during RUN1 or RUN2 policy.\nChange Run Policy by sourcing MCLK from something other than LFCLK";
                _.each(offendingHF, (h) => validation.logError(errStr, h))
            }

        } else {

            let potentialInstances = [ "SYSPLL_CLK0En", "SYSPLL_CLK1En", "SYSPLL_CLK2XEn", "useHFCLK_Manual" ];
            let offendingInstances = _.filter(potentialInstances, (x) => inst[x] == true);
            if(offendingInstances.length > 0)
            {
                offendingInstances.push("runPowerPolicy");
                validation.logError("High-Speed clocks cannot be enabled during a the run power policy " + inst.runPowerPolicy +
                ". Will not operate correctly", inst, offendingInstances);
            }

        }
    }
}

/*
 *  ======== validatePinmux ========
 *  Validate this inst's configuration
 *
 *  param inst       - module instance to be validated
 *  param validation - object to hold detected validation issues
 */
function validatePinmux(inst, validation){
    /* CLKOUT Validation */
    if(inst.enableEXCLK){
        try{
            let clkOutPin = inst.peripheral.clkOutPin.$solution.packagePinName;
            let validPinSet = validClkOutPinSet(inst);

            if (!validPinSet.includes(clkOutPin)){
                validation.logError("Currently selected CLK_OUT pin not available when routing a high frequency. Please select a different CLK_OUT source or a different pin. See device documentation for pin limitations.",
                inst, ["EXCLKSource"]);
            }
            if(inst.ClkOutForceHighDrive){
				if(inst.VDD < 2.7){
					validation.logError("When High Drive is enabled VDD supplied voltage cannot be less than 2.7V.",
					inst, ["VDD"]);
				}
			}
        } catch (e){
            // do nothing
        }
    }
    /* NRST / PA1 Validation */
    if(!inst.disableNRSTPin){
        let keys = Common.getModuleKeys();
        let usedPins = Common.getUsedPins(keys);
        let usedNRST = (usedPins.usedPinNames.filter(e => (!_.isNull(e)?e.includes("NRST"):false)));
        let warningCondition = (usedNRST.filter(e => (!_.isNull(e)?e.includes("PA1"):false)));
        if(warningCondition.length > 0){
            validation.logWarning("Enabling NRST functionality while PA1/NRST pin is used by the application can cause unexpected reset.",
                inst, ["disableNRSTPin"]);
        }
    }

}

/*
 *  ======== validateSYSCTL ========
 *  Validate this inst's configuration
 *
 *  param inst       - module instance to be validated
 *  param validation - object to hold detected validation issues
 */
function validateSYSCTL(inst, validation)
{
    if(system.modules["/ti/drivers/Board"]){
        if(!inst.driversEn){
            validation.logError("TI Drivers are being configured, must enable Driver configuration.", inst, ["driversEn"]);
        }
        else if(inst.driversEn){
            validation.logInfo("Power & Systems options are disabled from being configured in SysCtl.", inst, ["driversEn"]);
        }
    }
    else if(inst.driversEn){
        validation.logInfo("TI Drivers are not currently being configured. This option will lock other configurations.", inst, ["driversEn"]);
    }


    validatePowerPolicy(inst, validation);

    if(!inst.clockTreeEn){
    /* HFCLK Validation */
    if(Common.isDeviceM0G()){
        /* Validate case of disabled HFCLK */
        if(!["None"].includes(inst.usesHFCLK) && !inst.useHFCLK_Manual){
            validation.logError("Must enable HFCLK for this configuration", inst, ["useHFCLK_Manual"]);
        }
        if(["None"].includes(inst.usesHFCLK) && inst.useHFCLK_Manual){
            validation.logInfo("HFCLK is not being sourced by anything, initialization code will be generated once it's being used.",
             inst, ["useHFCLK_Manual"]);
        }
    }
    /* HSCLK & Wait State */
    if(Common.isDeviceM0G()){
        if(inst.MCLKSource === "HSCLK"){
            /* Wait state validation */
            /* Validate wait states with frequency ranges */
            if(inst.waitState == "0"){
                if(inst.MCLK_Freq > 24000000){
                    validation.logError("Exceeded maximum frequency for selected wait state", inst, ["waitState","MCLK_Freq_unit"]);
                }
            }
            else if(inst.waitState == "1"){
                if(inst.MCLK_Freq > 48000000){
                    validation.logError("Exceeded maximum frequency for selected wait state", inst, ["waitState","MCLK_Freq_unit"]);
                }
            }
            else if(inst.waitState == "2"){
                if(inst.MCLK_Freq > 80000000){
                    validation.logError("Exceeded maximum frequency for selected wait state", inst, ["waitState","MCLK_Freq_unit"]);
                }
            }
        }
    }
    /* SYSPLL Validation */
    if(Common.isDeviceM0G()){
        if (inst.SYSPLL_VCOFreq != 160000000) {
            validation.logWarning("MSPM0G currently only includes internal parameters to support configuring fVCO at a \
            recommended frequency of 160MHz. Pending characterization, other frequencies might result in reduced \
            accuracy or the PLL locking onto a wrong frequency entirely.", inst, ["SYSPLL_VCOFreq_disp"]);
        }
        /* SYSPLL frequency range must be between [4MHz, 48MHz] */
        if (inst.SYSPLLSource == "HFCLK")
        {
            if ((inst.HFCLK_Freq < 4000000) ||
                (inst.HFCLK_Freq > 48000000))
            {
                if(inst.useSYSPLL){
                    validation.logError("SYSPLL input frequency must be within [4MHz, 48MHz]", inst, ["HFCLK_Freq","SYSPLL_InputCLKFreqRange"]);
                }
            }
        }
        /* SYSOSC frequency must be 32MHz when used as SYSPLL source. */
        else if (inst.SYSPLLSource == "SYSOSC"){
            if(inst.SYSOSC_Freq != 32000000){
                if(inst.useSYSPLL){
                    validation.logError("SYSPLL input frequency must be 32MHz when SYSOSC is source.", inst, ["SYSOSC_Freq","SYSPLL_InputCLKFreqRange"]);
                }
            }
        }
        if(inst.MCLKSource === "HSCLK"){
            /* Validate HSCLK Sources */
            if(inst.HSCLKSource === "SYSPLLCLK0" && !inst.SYSPLL_CLK0En){
                validation.logError("SYSPLL Clock 0 is not currently enabled", inst, "HSCLKSource");
                validation.logError("HSCLK has selected SYSPLL Clock 0 as its source, which is currently disabled.", inst, "SYSPLL_CLK0En");
            }
            else if(inst.HSCLKSource === "SYSPLLCLK2X" && !inst.SYSPLL_CLK2XEn){
                validation.logError("SYSPLL Clock 2 is not currently enabled", inst, "HSCLKSource");
                validation.logError("HSCLK has selected SYSPLL Clock 2 as its source, which is currently disabled.", inst, "SYSPLL_CLK2XEn");
            }
        }
        if (inst.SYSPLL_Freq_CLK0 > 80000000) {
            validation.logError("Calculated frequency is greater than 80MHz, which is a spec violation for this device.", inst, "SYSPLL_Freq_CLK0");
        }
        if (inst.SYSPLL_Freq_CLK1 > 80000000) {
            validation.logError("Calculated frequency is greater than 80MHz, which is a spec violation for this device.", inst, "SYSPLL_Freq_CLK1");
        }
        if (inst.SYSPLL_Freq_CLK2X > 80000000) {
            validation.logError("Calculated frequency is greater than 80MHz, which is a spec violation for this device.", inst, "SYSPLL_Freq_CLK2X");
        }

        // CAN CLK specific validation
        if(inst.CANCLKSource != "None"){
            if((inst.CANCLKSource == "SYSPLLCLK1") && !inst.SYSPLL_CLK1En){
                validation.logError("SYSPLLCLK1 is not enabled.", inst, "SYSPLL_CLK1En");
            }
        }

    }

    /* MSPM0L11XX_L13XX Unavailable Sources Validation */
    if(Common.isDeviceFamily_PARENT_MSPM0L11XX_L13XX()){
        if(["SYSPLLOUT1","HFCLK"].includes(inst.EXCLKSource)){
            validation.logError("Please select a valid source", inst, "EXCLKSource");
        }
        if(inst.MCLKSource === "HSCLK"){
            validation.logError("Please select a valid source", inst, "MCLKSource");
        }
        if(["LFCLK_IN","LFXT"].includes(inst.LFCLKSource)){
            validation.logError("Please select a valid source", inst, "LFCLKSource");
        }
    }

    /* Disable SYSOSC validation */
    if(inst.disableSYSOSC){
        validation.logInfo("SYSOSC has been disabled through MCLK source configuration", inst, ["enableSYSOSCFCL","SYSOSC_Freq"]);
        if(inst.MFCLKEn){
            validation.logError("SYSOSC has been disabled through MCLK source configuration", inst, "MFCLKEn");
        }
        if(inst.MFPCLKEn && (inst.MFPCLKSource === "SYSOSC")){
            validation.logError("SYSOSC has been disabled through MCLK source configuration", inst, "MFPCLKSource");
        }
        if(Common.isDeviceM0G()){
            if(inst.useSYSPLL && inst.SYSPLLSource === "SYSOSC"){
                validation.logError("SYSOSC has been disabled through MCLK source configuration", inst, "SYSPLLSource");
            }
        }
    }
    else{
        /* FCL Validation */
        if(inst.enableSYSOSCFCL && !inst.enableROSC){
            validation.logError("ROSC should be enabled if Frequency Correction Loop is enabled", inst, ["enableSYSOSCFCL", "enableROSC"]);
        }
        else if(!inst.enableSYSOSCFCL && inst.enableROSC){
            validation.logWarning("If ROSC has been enabled, its recommended Frequency Correction Loop is enabled.", inst, ["enableSYSOSCFCL", "enableROSC"]);
        }
    }

    /* CLK_OUT Validation */
    if(inst.enableEXCLK){
        if(Common.isDeviceM0G()){
            if(inst.EXCLKSource === "SYSPLLOUT1" && !inst.SYSPLL_CLK1En){
                validation.logError("SYSPLL Clock 1 is not currently enabled", inst, "EXCLKSource");
                validation.logError("CLK_OUT has selected SYSPLL Clock 1 as its source, which is currently disabled.", inst, "SYSPLL_CLK1En");
            }
        }
        else if(inst.EXCLKSource === "SYSOSC" && inst.disableSYSOSC){
            validation.logError("SYSOSC is currently disabled by MCLK", inst, "EXCLKSource");
        }
        else if(inst.EXCLKSource === "MFPCLK" && !inst.MFPCLKEn){
            validation.logError("MFPCLK is currently disabled.", inst, "EXCLKSource");
        }

        if(inst.EXCLKDivider === "DISABLE"){
            if(inst.EXCLKSource === "ULPCLK" || inst.EXCLKSource === "MFPCLK"){
                validation.logError("CLK_OUT Divider must be enabled for selected source.", inst, ["EXCLKSource", "EXCLKDivider"]);
            }
        }
    }

    /* FCC Clock Source validation */
    if(inst.enableFCC){
        if((inst.fccClkSrc == "SYSOSC")&&(inst.disableSYSOSC)){
            validation.logError("Must enable SYSOSC for this configuration", inst, ["fccClkSrc","disableSYSOSC"]);
        }
        if((inst.fccClkSrc == "CLK_OUT")&&(!inst.enableEXCLK)){
            validation.logError("Must enable CLK_OUT for this configuration", inst, ["fccClkSrc","enableEXCLK"]);
        }
        if(Common.isDeviceM0G()){
            if((inst.fccClkSrc == "HFCLK")&&(!inst.useHFCLK_Manual)){
                validation.logError("Must enable HFCLK for this configuration", inst, ["fccClkSrc"]);
            }
            if((inst.fccClkSrc == "SYSPLLCLK0")&&(!inst.SYSPLL_CLK0En)){
                validation.logError("Must enable SYSPLLCLK0 for this configuration", inst, ["fccClkSrc","SYSPLL_CLK0En"]);
            }
            if((inst.fccClkSrc == "SYSPLLCLK1")&&(!inst.SYSPLL_CLK1En)){
                validation.logError("Must enable SYSPLLCLK1 for this configuration", inst, ["fccClkSrc","SYSPLL_CLK1En"]);
            }
            if((inst.fccClkSrc == "SYSPLLCLK2X")&&(!inst.SYSPLL_CLK2XEn)){
                validation.logError("Must enable SYSPLLCLK2X for this configuration", inst, ["fccClkSrc","SYSPLL_CLK2XEn"]);
            }
        }
    }

    /* Special case MCLK validation */
    if(inst.MCLKSource == "LFCLK"){
        let COMPMod = system.modules["/ti/driverlib/COMP"];
        if (COMPMod){ if(COMPMod.$instances.length>0){
            let COMPerror = false;
            for(let COMPinst of COMPMod.$instances){
                if(COMPinst.compMode == "DL_COMP_MODE_FAST"){
                    COMPerror = true;
                }
            }
            if(COMPerror){
                validation.logError("LFCLK cannot source MCLK if COMP is enabled on Fast Mode.", inst, ["MCLKSource"]);
            }
        }}

        let OPAMod = system.modules["/ti/driverlib/OPA"];
        if (OPAMod){ if(OPAMod.$instances.length>0){
            validation.logError("LFCLK cannot source MCLK if OPA is enabled.", inst, ["MCLKSource"]);
        }}
    }

    /* Special Case: DAC12 Validation */
    let DACMod = system.modules["/ti/driverlib/DAC12"];
    if (DACMod){
        if (DACMod.$static.dacEnable && !inst.MFPCLKEn) {
            validation.logError("MFPCLK must be enabled when DAC12 peripheral is added.", inst, ["MFPCLKEn"]);
        }
    }

    /* MFPCLK Frequency Validation */
    if(inst.MFPCLKEn && inst.MFPCLK_Freq > 4000000){
        validation.logError("MFPCLK frequency must be under 4MHz.", inst, ["HFCLK4MFPCLKDIV","MFPCLK_Freq_unit"]);
    }

    /* VREF/MFCLK Validation */
    if(!inst.MFCLKEn){
        let vrefInstance = system.modules["/ti/driverlib/VREF"];
        if (vrefInstance){
            if(vrefInstance.$static.advClockConfigEnable){
                if((vrefInstance.$static.advClkSrc == "DL_VREF_CLOCK_MFCLK")){
                    validation.logError("MFCLK must be enabled for the current configuration.", inst, ["MFCLKEn"]);
                }
            }
        }
    }

    } // if Clock Tree is enabled

    if(inst.ClkOutHighDriveEn){
        validation.logInfo("High Drive Enabled on the CLK_OUT pin", inst, "enableEXCLK");
    }
}

function onChangeUseDrivers(inst, ui){
    // Power Settings
    // TODO: (H) leaving for now for retention. clarify if need to remove / overlaps with Drivers
    // ui.powerPolicy.readOnly    = inst.driversEn;
    ui.BORThresh.readOnly                       = inst.driversEn;
    ui.enableWriteLock.readOnly                 = inst.driversEn;
    ui.enableSleepOnExit.readOnly               = inst.driversEn;
    ui.enableEventOnPending.readOnly            = inst.driversEn;
    ui.disableNRSTPin.readOnly                  = inst.driversEn;
    ui.vboostMode.readOnly                      = inst.driversEn;
    ui.enableFCC.readOnly                       = inst.driversEn;
        ui.fccTrigLvl.readOnly                  = inst.driversEn;
        ui.fccPeriods.readOnly                  = inst.driversEn;
        ui.fccTrigSrc.readOnly                  = inst.driversEn;
        ui.fccClkSrc.readOnly                   = inst.driversEn;
    ui.enableIntDone.readOnly                   = inst.driversEn;
        ui.flashInterruptPriority.readOnly      = inst.driversEn;
    ui.nmiWWDT0Behav.readOnly                   = inst.driversEn;
    if(Common.isDeviceM0G()){
        ui.waitState.readOnly                   = inst.driversEn;
        ui.powerSysInterrupts.readOnly          = inst.driversEn;
            ui.pmcuInterruptPriority.readOnly   = inst.driversEn;
        ui.nmiWWDT1Behav.readOnly               = inst.driversEn;
        ui.nmiFlashDedBehav.readOnly            = inst.driversEn;
    }
}

function onChangeClockTree(inst, ui){
    if(Common.isDeviceM0G()){
    ui.LFCLKLowCap.readOnly             = inst.clockTreeEn;
    ui.LFCLKMonitor.readOnly            = inst.clockTreeEn;
    ui.LFCLKDriveStrength.readOnly      = inst.clockTreeEn;
    ui.HFXTStartup.readOnly             = inst.clockTreeEn;
    ui.HFCLKMonitor.readOnly            = inst.clockTreeEn;
    ui.SYSPLLSource.readOnly            = inst.clockTreeEn;
    ui.SYSPLL_Pdiv.readOnly             = inst.clockTreeEn;
    ui.SYSPLL_Qdiv.readOnly             = inst.clockTreeEn;
    ui.SYSPLL_CLK0En.readOnly           = inst.clockTreeEn;
    ui.SYSPLL_CLK1En.readOnly           = inst.clockTreeEn;
    ui.SYSPLL_CLK2XEn.readOnly          = inst.clockTreeEn;
    ui.SYSPLL_CLK0Div.readOnly          = inst.clockTreeEn;
    ui.SYSPLL_CLK1Div.readOnly          = inst.clockTreeEn;
    ui.SYSPLL_CLK2XDiv.readOnly         = inst.clockTreeEn;
    ui.useHFCLK_Manual.readOnly         = inst.clockTreeEn;
    ui.HFCLKSource.readOnly             = inst.clockTreeEn;
    ui.HFCLK_Freq.readOnly              = inst.clockTreeEn;
    ui.HSCLKSource.readOnly             = inst.clockTreeEn;
    ui.UDIV.readOnly                    = inst.clockTreeEn;
    ui.validateClkStatus.readOnly       = (inst.LFCLKSource == "LFXT") || (inst.useHFCLK_Manual && inst.HFCLKSource == "HFXT");

    ui.waitState.hidden = !inst.clockTreeEn && !(inst.MCLKSource == "HSCLK");
    }

    ui.MFCLKEn.readOnly                 = inst.clockTreeEn;
    ui.MFPCLKEn.readOnly                = inst.clockTreeEn;
    ui.MFPCLKSource.readOnly            = inst.clockTreeEn;
    ui.MCLKSource.readOnly              = inst.clockTreeEn;
    ui.disableSYSOSC.readOnly           = inst.clockTreeEn;
    ui.MDIV.readOnly                    = inst.clockTreeEn;
    ui.LFCLKSource.readOnly             = inst.clockTreeEn;
    ui.LFCLK_Freq_IN.readOnly           = inst.clockTreeEn;
    ui.enableEXCLK.readOnly             = inst.clockTreeEn;
    ui.EXCLKSource.readOnly             = inst.clockTreeEn;
    ui.EXCLKDivider.readOnly            = inst.clockTreeEn;
    ui.SYSOSC_Freq.readOnly             = inst.clockTreeEn;
    ui.enableSYSOSCFCL.readOnly         = inst.clockTreeEn;
    ui.enableROSC.readOnly              = inst.clockTreeEn;

    ui.enableFCC.readOnly               = inst.clockTreeEn;
    ui.fccTrigLvl.readOnly              = inst.clockTreeEn;
    ui.fccPeriods.readOnly              = inst.clockTreeEn;
    ui.fccTrigSrc.readOnly              = inst.clockTreeEn;
    ui.fccClkSrc.readOnly               = inst.clockTreeEn;
}

function getClockTreeDesc(){
    if(Common.isDeviceM0G()){
        return `
This is the Clock Tree diagram, refer to the TRM for more detailed information.

![Clock Tree Diagram](../docs/english/sysconfig/images/clocktree_M0G.png "Clock Tree Diagram")
`;
    }
    else if(Common.isDeviceM0L()){
        return `
This is the Clock Tree diagram, refer to the TRM for more detailed information.

![Clock Tree Diagram](../docs/english/sysconfig/images/clocktree_M0L.png "Clock Tree Diagram")
`;
    }
    return "Refer to the TRM for more detailed information on Clock Tree";
};

function getLFXTConfig(){
    if(Common.isDeviceM0G()){
        return [
            // DL_SYSCTL_setLFCLKSourceLFXT
            {
                name        : "LFCLKLowCap",
                displayName : "Low Cap Low Power Enable",
                description : 'Enable if CAP is less than 3pF to reduce power consumption',
                longDescription : 'Enable if CAP is less than 3pF to reduce power consumption',
                hidden      : false,
                default     : false
            },
            // DL_SYSCTL_setLFCLKSourceLFXT
            {
                name        : "LFCLKMonitor",
                displayName : "LFCLK Monitor Enable",
                description : 'Enable to use monitor for LFXT, EXLF failure',
                longDescription : 'Enable to use monitor for LFXT, EXLF failure',
                hidden      : false,
                default     : false
            },
            // DL_SYSCTL_setLFCLKSourceLFXT
            // DL_SYSCTL_LFXT_DRIVE_STRENGTH_[...]
            {
                name        : "LFCLKDriveStrength",
                displayName : "Drive Strength",
                description : 'Drive strength and power consumption option',
                hidden      : false,
                default     : "HIGHEST",
                options     : [
                    {name: "LOWEST", displayName: "Lowest Drive and Current"},
                    {name: "LOWER", displayName: "Lower Drive and Current"},
                    {name: "HIGHER", displayName: "Higher Drive and Current"},
                    {name: "HIGHEST", displayName: "Highest Drive and Current"},
                ],
            },
        ]
    };
    return[];
};

function getHFXTConfig(){
    if(Common.isDeviceM0G()){
        return [
            {
                name: "HFXTRange",
                displayName: "HFXT Frequency Range",
                default: "DL_SYSCTL_HFXT_RANGE_4_8_MHZ",
                hidden: false,
                options: [
                    {name: "DL_SYSCTL_HFXT_RANGE_4_8_MHZ", displayName: "4 and 8 MHz"},
                    {name: "DL_SYSCTL_HFXT_RANGE_8_16_MHZ", displayName: "8 and 16 MHz"},
                    {name: "DL_SYSCTL_HFXT_RANGE_16_32_MHZ", displayName: "16 and 32 MHz"},
                    {name: "DL_SYSCTL_HFXT_RANGE_32_48_MHZ", displayName: "32 and 48 MHz"},
                ],
                getValue: (inst) => {
                    if (inst.HFCLK_Freq <= 8000000) { return "DL_SYSCTL_HFXT_RANGE_4_8_MHZ" }
                    else if (inst.HFCLK_Freq > 8000000 && inst.HFCLK_Freq <= 16000000) { return "DL_SYSCTL_HFXT_RANGE_8_16_MHZ" }
                    else if (inst.HFCLK_Freq > 16000000 && inst.HFCLK_Freq <= 32000000) { return "DL_SYSCTL_HFXT_RANGE_16_32_MHZ" }
                    else if (inst.HFCLK_Freq > 32000000) { return "DL_SYSCTL_HFXT_RANGE_32_48_MHZ" }
                    else{ return "DL_SYSCTL_HFXT_RANGE_32_48_MHZ"}
                }
            },
            {
                name        : "HFXTStartup",
                displayName : "HFXT Startup Time",
                description : 'Specify the HFXT startup time in 64us resolution.',
                hidden      : false,
                range       : [0,255],
                default     : 0
            },
            {
                name        : "HFXTStartupCalculated",
                displayName : "Calculated HFXT Startup Time",
                description : 'Specifies the HFXStartup time.',
                hidden      : false,
                default     : "0 s",
                getValue    : (inst) => {
                    return (Common.getUnitPrefix(inst.HFXTStartup * 0.000064)).str+"s";
                },
            },
            {
                name        : "HFCLKMonitor",
                displayName : "HFCLK Startup Monitor",
                description : 'Enable the HFCLK startup monitor',
                hidden      : false,
                default     : false
            },
        ];
    }
    return [];
};

function getSYSPLLConfig(){
    if(Common.isDeviceM0G()){
        return [
            // TODO: once CAN has been added update condition to be included here for SYSPLL use
            {
                name        : "useSYSPLL",
                hidden      : true,
                default     : false,
                getValue    : (inst) => {
                    /* SYSPLL is not available foor M0Lx device family */
                    if(Common.isDeviceM0L() || inst.clockTreeEn){
                        return false;
                    }
                    if((inst.MCLKSource === "HSCLK")&&
                    (["SYSPLLCLK0","SYSPLLCLK2X"].includes(inst.HSCLKSource))){
                        return true;
                    }
                    if(inst.enableEXCLK && inst.EXCLKSource === "SYSPLLOUT1"){
                        return true;
                    }
                    if(inst.enableFCC && (["SYSPLLCLK0","SYSPLLCLK1","SYSPLLCLK2X"].includes(inst.fccClkSrc))){
                        return true;
                    }
                    if(inst.CANCLKSource === "SYSPLLCLK1"){
                        return true;
                    }
                    return false;
                }
            },
            {
                name        : "usesSYSPLL",
                displayName : "Sourced By SYSPLL",
                description : "List of components of the Clock Tree being sourced by SYSPLL",
                hidden      : false,
                default     : "",
                getValue    : (inst) => {
                    /* SYSPLL is not available foor M0Lx device family */
                    if(Common.isDeviceM0L() || inst.clockTreeEn){
                        return "";
                    }
                    let inUse = "";
                    if((inst.MCLKSource === "HSCLK")&&(["SYSPLLCLK0","SYSPLLCLK2X"].includes(inst.HSCLKSource))){
                        inUse += "MCLK, "
                    }
                    if(inst.enableEXCLK && (inst.EXCLKSource === "SYSPLLOUT1")){
                        inUse += "CLK_OUT, ";
                    }
                    if(inst.enableFCC && (["SYSPLLCLK0","SYSPLLCLK1","SYSPLLCLK2X"].includes(inst.fccClkSrc))){
                        inUse+= "FCC, "
                    }
                    if((inst.CANCLKSource === "SYSPLLCLK1")){
                        inUse += "CANCLK, "
                    }
                    if(inUse.length == 0){
                        return "None"
                    }

                    return inUse;
                }
            },
            // DL_SYSCTL_configSYSPLL
            {
                name: "SYSPLLSource",
                displayName: "SYSPLL Reference Clock (SYSPLLREF)",
                description : "The available reference clocks for the SYSPLL include SYSOSC and HFCLK.",
                longDescription : "The available reference clocks for the SYSPLL include SYSOSC and HFCLK.",
                default: "SYSOSC",
                options: [
                    { name: "SYSOSC" }, { name: "HFCLK"},
                ]
            },
            {
                name        : "SYSPLLSource_freq",
                displayName : "SYSPLL Reference Clock Frequency",
                hidden      : true,
                getValue    : (inst) => {
                    let sourceFreq = 0;
                    switch (inst.SYSPLLSource){
                       case "SYSOSC":
                            sourceFreq = inst.SYSOSC_Freq;
                        break;
                        case "HFCLK":
                            sourceFreq = inst.HFCLK_Freq;
                        break;
                    }
                    return sourceFreq;
                },
                default     : 0
            },
            {
                name        : "SYSPLLSource_freq_disp",
                displayName : "SYSPLL Reference Clock Frequency",
                longDescription :
`The output frequency of the SYSPLL reference clock, fSYSPLL. The SYSPLL accepts an input reference clock from 4-48 MHz.`,
                hidden      : false,
                default     : "0 Hz",
                getValue: (inst) => {
                    return (Common.getUnitPrefix(inst.SYSPLLSource_freq)).str+"Hz";
                }
            },
            {
                name: "SYSPLL_Pdiv",
                displayName: "SYSPLL Predivider (PDIV)",
                description: "Scales the selected input reference clock ahead of the feedback loop",
                longDescription:
`
PDIV can be selected as /1, /2, /4, or /8.\n
The SYSPLL reference clock, SYSPLLREF, will be scaled by the value selected by PDIV.
`,
                default: 2,
                options: [
                    { name: 1, displayName: "Divide by 1" },
                    { name: 2, displayName: "Divide by 2" },
                    { name: 4, displayName: "Divide by 4" },
                    { name: 8, displayName: "Divide by 8" },
                ]
            },
            {
                name: "SYSPLL_InputCLKFreqRange_disp",
                displayName: "SYSPLL Input Freq Range (fLOOPIN)",
                longDescription:
`
The predivider PDIV scales the selected input reference clock ahead of the PLL feedback loop.\n
This scaled input reference clock fed into the PLL feedback loop is fLOOPIN, and is calculated as:\n
fLOOPIN = SYSPLLREF / PDIV \n

For example, if the input SYSPLL reference is 32MHz, and PDIV is set to Divide by 2, then fLOOPIN is 16MHz:
fLOOPIN = fSYSPLLREF / PDIV \n
fLOOPIN = 32MHz / 2 \n
fLOOPIN = 16MHz \n
`,

                default: "0 Hz",
                hidden: false,
                getValue: (inst) => {
                    let sourecFreq
                    if (inst.SYSPLLSource == "SYSOSC")
                    {
                        sourecFreq = inst.SYSOSC_Freq;
                    }
                    if (inst.SYSPLLSource == "HFCLK")
                    {
                        sourecFreq = inst.HFCLK_Freq;
                    }
                    let floopin = sourecFreq / inst.SYSPLL_Pdiv;
                    return (Common.getUnitPrefix(floopin)).str+"Hz";
                }
            },
            {
                name: "SYSPLL_InputCLKFreqRange",
                displayName: "SYSPLL Input Clock Frequency Range",
                default: "DL_SYSCTL_SYSPLL_INPUT_FREQ_32_48_MHZ",
                hidden: true,
                options: [
                    {name: "DL_SYSCTL_SYSPLL_INPUT_FREQ_4_8_MHZ", displayName: "4 MHz to 8 MHz"},
                    {name: "DL_SYSCTL_SYSPLL_INPUT_FREQ_8_16_MHZ", displayName: "8 MHz to 16 MHz"},
                    {name: "DL_SYSCTL_SYSPLL_INPUT_FREQ_16_32_MHZ", displayName: "16 MHz to 32 MHz"},
                    {name: "DL_SYSCTL_SYSPLL_INPUT_FREQ_32_48_MHZ", displayName: "32 MHz to 48 MHz"},
                    /* special case for when out of range */
                    {name: "DL_SYSCTL_SYSPLL_INPUT_FREQ_OUT_OF_RANGE", displayName: "OUT OF RANGE"}
                ],
                getValue: (inst) => {
                    let floopin = inst.SYSPLLSource_freq / inst.SYSPLL_Pdiv;
                    if (floopin >= 48000000)
                    {
                        return "DL_SYSCTL_SYSPLL_INPUT_FREQ_OUT_OF_RANGE";
                    }
                    else if (floopin >= 32000000)
                    {
                        return "DL_SYSCTL_SYSPLL_INPUT_FREQ_32_48_MHZ";
                    }
                    else if (floopin >= 16000000)
                    {
                        return "DL_SYSCTL_SYSPLL_INPUT_FREQ_16_32_MHZ";
                    }
                    else if (floopin >= 8000000)
                    {
                        return "DL_SYSCTL_SYSPLL_INPUT_FREQ_8_16_MHZ";
                    }
                    else if (floopin >= 4000000)
                    {
                        return "DL_SYSCTL_SYSPLL_INPUT_FREQ_4_8_MHZ";
                    }
                    else
                    {
                        return "DL_SYSCTL_SYSPLL_INPUT_FREQ_OUT_OF_RANGE";
                    }
                }
            },
            // DL_SYSCTL_configSYSPLL
            {
                name: "SYSPLL_Qdiv",
                displayName: "SYSPLL Feedback Divider (QDIV)",
                description: "Multiplied with the divided input reference clock",
                longDescription:
`
The PLL feedback loop sets the VCO output, fVCO, equal to the divided input reference clock fLOOPIN multiplied by
the QDIV feedback divider. The QDIV divider is an integer divider with a valid range of /2 to /128.
`,
                isInteger: true,
                default: 10,
                range: [2, 128],
            },
            {
                name: "SYSPLL_VCOFreq",
                displayName: "Calculated PLL VCO Freq (Hz)",
                description: "VCO output frequency that is set by the SYSPLL feedback loop",
                longDescription:
`
The PLL feedback loop sets the VCO output, fVCO, equal to the divided input reference clock fLOOPIN multiplied by
the QDIV feedback divider. The QDIV divider is an integer divider with a valid range of /2 to /128.
`,
                default: 0,
                hidden: true,
                getValue: (inst) => {
                    if(inst.SYSPLLSource == "SYSOSC") {
                        /*
                         * SYSOSC must be running at 32MHz (base frequency)
                         * for SYSPLL operation, even if HFCLK is the
                         * reference clock
                         */
                        return (32000000 / inst.SYSPLL_Pdiv) * inst.SYSPLL_Qdiv;
                    } else if (inst.SYSPLLSource == "HFCLK") {
                        return (inst.HFCLK_Freq / inst.SYSPLL_Pdiv) * inst.SYSPLL_Qdiv;
                    } else {
                        return 0;
                    }
                }
            },
            {
                name: "SYSPLL_VCOFreq_disp",
                displayName: "Calculated VCO Freq (fVCO)",
                description: "VCO output frequency that is set by the SYSPLL feedback loop",
                longDescription:
`
The output frequency of the VCO, fVCO is calculated as:\n

fVCO = fSYSPLLREF / PDIV * QDIV\n

For example, if fSYSPLL is 32MHz, PDIV is set to /2, and QDIV is set to 10, then fVCO is calculated to be 160MHz.\n
fVCO = fSYSPLLREF / PDIV * QDIV\n
fVCO = 32MHz / 2 * 10 \n
fVCO = 160MHz \n
`,
                default: "0 Hz",
                hidden: false,
                getValue: (inst) => {
                    return (Common.getUnitPrefix(inst.SYSPLL_VCOFreq)).str+"Hz";
                }
            },
            // DL_SYSCTL_configSYSPLL
            {
                name: "SYSPLL_CLK0En",
                displayName: "Enable SYSPLLCLK0",
                description: "Enables SYSPLL output clock SYSPLLCLK0",
                longDescription:
`
The VCO output sources three separate SYSPLL outputs (SYSPLLCLK0, SYSPLLCLK1, and SYSPLLCLK2X).
Each output has its own divider unit to enable generation of up to 3 different output frequencies for use by
different modules in the device. The third output (SYSPLLCLK2X) also contains a frequency doubler before the
divider unit to provide a wider range of output frequencies and lower power consumption.
`,
                default: false,
                onChange: (inst, ui) => {
                    ui.SYSPLL_CLK0Div.hidden = !inst.SYSPLL_CLK0En;
                }
            },
            // DL_SYSCTL_configSYSPLL
            {
                name: "SYSPLL_CLK0Div",
                displayName: "SYSPLLCLK0 Divider",
                description: "Scales the output frequency of SYSPLLCLK0",
                longDescription:
`
For SYSPLLCLK1 and SYSPLLCLK0, the output dividers can be set from /2 to /32 in steps of 2.
The SYSPLL output clock frequencies are thus set by the combination of fVCO and the respective dividers.\n
`,
                hidden: true,
                default: 0,
                options: [
                    { name: 0, displayName: "Divide by 2" },
                    { name: 1, displayName: "Divide by 4" },
                    { name: 2, displayName: "Divide by 6" },
                    { name: 3, displayName: "Divide by 8" },
                    { name: 4, displayName: "Divide by 10" },
                    { name: 5, displayName: "Divide by 12" },
                    { name: 6, displayName: "Divide by 14" },
                    { name: 7, displayName: "Divide by 16" },
                    { name: 8, displayName: "Divide by 18" },
                    { name: 9, displayName: "Divide by 20" },
                    { name: 10, displayName: "Divide by 22" },
                    { name: 11, displayName: "Divide by 24" },
                    { name: 12, displayName: "Divide by 26" },
                    { name: 13, displayName: "Divide by 28" },
                    { name: 14, displayName: "Divide by 30" },
                    { name: 15, displayName: "Divide by 32" },
                ]
            },
            {
                name: "SYSPLL_Freq_CLK0",
                displayName: "Calculated SYSPLLCLK0 Output Freq (Hz)",
                description: "Clock frequency that is output by the SYSPLLCLK0",
                default: 0,
                hidden: true,
                getValue: (inst) => {
                    let outputFreq = 0;

                    if (inst.SYSPLL_CLK0En == true)
                    {
                        outputFreq = inst.SYSPLL_VCOFreq / ((inst.SYSPLL_CLK0Div + 1) * 2);
                    }

                    return outputFreq;
                }
            },
            {
                name: "SYSPLL_Freq_CLK0_display",
                displayName: "Calculated SYSPLLCLK0 Output Freq",
                description: "Clock frequency that is output by the SYSPLLCLK0",
                longDescription:
`
The SYSPLLCLK0 output is calculated as shown:\n

fSYSPLLCLK0 = fVCO / SYSPLLCLK0DIV\n

For example, if fSYSPLL is 32MHz, PDIV is set to /2, QDIV is set to 10, then fVCO is 160MHz. Then, if SYSPLLCLK0DIV is set to /2, the output frequency of SYSPLLCLK0 is 80MHz.\n
fSYSPLLCLK0 = fVCO / SYSPLLCLK0DIV\n
fSYSPLLCLK0 = 160MHz / 2\n
fSYSPLLCLK0 = 80MHz \n
`,
                default: "0 Hz",
                hidden: false,
                getValue: (inst) => {
                    return (Common.getUnitPrefix(inst.SYSPLL_Freq_CLK0)).str+"Hz";
                }
            },
            // DL_SYSCTL_configSYSPLL
            {
                name: "SYSPLL_CLK1En",
                displayName: "Enable SYSPLLCLK1",
                description: "Enables SYSPLL output clock SYSPLLCLK1",
                longDescription:
`
The VCO output sources three separate SYSPLL outputs (SYSPLLCLK0, SYSPLLCLK1, and SYSPLLCLK2X).
Each output has its own divider unit to enable generation of up to 3 different output frequencies for use by
different modules in the device. The third output (SYSPLLCLK2X) also contains a frequency doubler before the
divider unit to provide a wider range of output frequencies and lower power consumption.
`,
                default: false,
                onChange: (inst, ui) => {
                    ui.SYSPLL_CLK1Div.hidden = !inst.SYSPLL_CLK1En;
                }
            },
            // DL_SYSCTL_configSYSPLL
            {
                name: "SYSPLL_CLK1Div",
                displayName: "SYSPLLCLK1 Divider",
                description: "Scales the output frequency of SYSPLLCLK1",
                longDescription:
`
For SYSPLLCLK1 and SYSPLLCLK0, the output dividers can be set from /2 to /32 in steps of 2.
The SYSPLL output clock frequencies are thus set by the combination of fVCO and the respective dividers.\n
`,
                hidden: true,
                default: 0,
                options: [
                    { name: 0, displayName: "Divide by 2" },
                    { name: 1, displayName: "Divide by 4" },
                    { name: 2, displayName: "Divide by 6" },
                    { name: 3, displayName: "Divide by 8" },
                    { name: 4, displayName: "Divide by 10" },
                    { name: 5, displayName: "Divide by 12" },
                    { name: 6, displayName: "Divide by 14" },
                    { name: 7, displayName: "Divide by 16" },
                    { name: 8, displayName: "Divide by 18" },
                    { name: 9, displayName: "Divide by 20" },
                    { name: 10, displayName: "Divide by 22" },
                    { name: 11, displayName: "Divide by 24" },
                    { name: 12, displayName: "Divide by 26" },
                    { name: 13, displayName: "Divide by 28" },
                    { name: 14, displayName: "Divide by 30" },
                    { name: 15, displayName: "Divide by 32" },
                ]
            },
            {
                name: "SYSPLL_Freq_CLK1",
                displayName: "Calculated SYSPLLCLK1 Output Freq (Hz)",
                description: "Clock frequency that is output by the SYSPLLCLK1",
                default: 0,
                hidden: true,
                getValue: (inst) => {
                    let outputFreq = 0;

                    if (inst.SYSPLL_CLK1En == true)
                    {
                        outputFreq = inst.SYSPLL_VCOFreq / ((inst.SYSPLL_CLK1Div + 1) * 2);
                    }

                    return outputFreq;
                }
            },
            {
                name: "SYSPLL_Freq_CLK1_display",
                displayName: "Calculated SYSPLLCLK1 Output Freq",
                description: "Clock frequency that is output by the SYSPLLCLK1",
                longDescription:
`
The SYSPLLCLK1 output is calculated as shown:\n

fSYSPLLCLK1 = fVCO / SYSPLLCLK1DIV\n

For example, if fSYSPLL is 32MHz, PDIV is set to /2, QDIV is set to 10, then fVCO is 160MHz. Then, if SYSPLLCLK1DIV is set to /2, the output frequency of SYSPLLCLK1 is 80MHz.\n
fSYSPLLCLK1 = fVCO / SYSPLLCLK1DIV\n
fSYSPLLCLK1 = 160MHz / 2\n
fSYSPLLCLK1 = 80MHz \n
`,
                default: "0 Hz",
                hidden: false,
                getValue: (inst) => {
                    return (Common.getUnitPrefix(inst.SYSPLL_Freq_CLK1)).str+"Hz";
                }
            },
            // DL_SYSCTL_configSYSPLL
            {
                name: "SYSPLL_CLK2XEn",
                displayName: "Enable SYSPLLCLK2X",
                description: "Enables SYSPLL output clock SYSPLLCLK2X",
                longDescription:
`
The VCO output sources three separate SYSPLL outputs (SYSPLLCLK0, SYSPLLCLK1, and SYSPLLCLK2X).
Each output has its own divider unit to enable generation of up to 3 different output frequencies for use by
different modules in the device. The third output (SYSPLLCLK2X) also contains a frequency doubler before the
divider unit to provide a wider range of output frequencies and lower power consumption.
`,
                default: false,
                onChange: (inst, ui) => {
                    ui.SYSPLL_CLK2XDiv.hidden = !inst.SYSPLL_CLK2XEn;
                }
            },
            // DL_SYSCTL_configSYSPLL
            {
                name: "SYSPLL_CLK2XDiv",
                displayName: "SYSPLLCLK2X Divider",
                description: "Scales the output frequency of SYSPLLCLK2X",
                longDescription:
`
For SYSPLLCLK2X, the output divider can be set from /1 to /16 in steps of 1.\n

SYSPLLCLK2X also contains a frequency doubler before the
divider unit to provide a wider range of output frequencies and lower power consumption.\n

The SYSPLL output clock frequencies are thus set by the combination of fVCO and the respective dividers.\n
`,
                hidden: true,
                default: 0,
                options: [
                    { name: 0, displayName: "Divide by 1" },
                    { name: 1, displayName: "Divide by 2" },
                    { name: 2, displayName: "Divide by 3" },
                    { name: 3, displayName: "Divide by 4" },
                    { name: 4, displayName: "Divide by 5" },
                    { name: 5, displayName: "Divide by 6" },
                    { name: 6, displayName: "Divide by 7" },
                    { name: 7, displayName: "Divide by 8" },
                    { name: 8, displayName: "Divide by 9" },
                    { name: 9, displayName: "Divide by 10" },
                    { name: 10, displayName: "Divide by 11" },
                    { name: 11, displayName: "Divide by 12" },
                    { name: 12, displayName: "Divide by 13" },
                    { name: 13, displayName: "Divide by 14" },
                    { name: 14, displayName: "Divide by 15" },
                    { name: 15, displayName: "Divide by 16" },
                ]
            },
            {
                name: "SYSPLL_Freq_CLK2X",
                displayName: "Calculated SYSPLLCLK2X Output Freq (Hz)",
                description: "Clock frequency that is output by the SYSPLLCLK2X",
                default: 0,
                hidden: true,
                getValue: (inst) => {
                    let outputFreq = 0;

                    if (inst.SYSPLL_CLK2XEn == true)
                    {
                        outputFreq = 2 * (inst.SYSPLL_VCOFreq / (inst.SYSPLL_CLK2XDiv + 1));
                    }

                    return outputFreq;
                }
            },
            {
                name: "SYSPLL_Freq_CLK2X_display",
                displayName: "Calculated SYSPLLCLK2X Output Freq",
                description: "Clock frequency that is output by the SYSPLLCLK2X",
                longDescription:
`
SYSPLLCLK2X contains a frequency doubler before the divider unit.
Therefore the SYSPLLCLK2 output is calculated as shown:\n

fSYSPLLCLK2 = 2 * fVCO / SYSPLLCLK2DIV\n

For example, if fSYSPLL is 32MHz, PDIV is set to /2, QDIV is set to 10, then fVCO is 160MHz. Then, if SYSPLLCLK2DIV is set to /4, the output frequency of SYSPLLCLK2 is 80MHz.\n
fSYSPLLCLK2 = 2 * fVCO / SYSPLLCLK2DIV\n
fSYSPLLCLK2 = 2 * 160MHz / 4\n
fSYSPLLCLK2 = 80MHz \n
`,
                default: "0 Hz",
                hidden: false,
                getValue: (inst) => {
                    return (Common.getUnitPrefix(inst.SYSPLL_Freq_CLK2X)).str+"Hz";
                }
            },
        ];
    }
    return [];
}

function getHFCLKConfig(){
    if(Common.isDeviceM0G()){
        return [
            {
                name        : "useHFCLK_Manual",
                displayName : "Enable HFCLK",
                longDescription: `This setting is automatically enabled when XTAL configuration is enabled.`,
                hidden      : false,
                default     : false,
                onChange: (inst,ui) =>{
                    if(inst.useHFCLK_Manual && inst.HFCLKSource == "HFXT"){
                        inst.validateClkStatus = true;
                        ui.validateClkStatus.readOnly = true;
                    }
                    else{
                        ui.validateClkStatus.readOnly = (inst.LFCLKSource == "LFXT");
                    }
                },
            },
            {
                name        : "useHFCLK",
                hidden      : true,
                default     : false,
                getValue    : (inst) => {
                    /* HFCLK is not available foor M0Lx device family */
                    if(Common.isDeviceFamily_PARENT_MSPM0L11XX_L13XX() || !inst.useHFCLK_Manual){
                        return false;
                    }
                    let inUse = false;
                    if(inst.MCLKSource === "HSCLK" && "HFCLK" === inst.HSCLKSource){
                        return true;
                    }
                    if(inst.CANCLKSource === "HFCLK"){
                        return true;
                    }
                    if(inst.MCLKSource === "HSCLK" && inst.useSYSPLL && inst.SYSPLLSource === "HFCLK"){
                        return true;
                    }
                    if(inst.MFPCLKEn && inst.MFPCLKSource === "HFCLK"){
                        return true;
                    }
                    if(inst.enableEXCLK && inst.EXCLKSource === "HFCLK"){
                        return true;
                    }
                    if(inst.enableFCC && inst.fccClkSrc == "HFCLK"){
                        return true;
                    }
                    let ADCMod = system.modules["/ti/driverlib/ADC12"];
                    if (ADCMod){
                        let ADCInsts = ADCMod.$instances
                        for(let singleInst of ADCInsts){
                            if(singleInst.sampClkSrc === "DL_ADC12_CLOCK_HFCLK"){
                               inUse = true;
                            }
                        }
                    }
                    return inUse;
                }
            },
            {
                name        : "usesHFCLK",
                displayName : "Sourced By HFCLK",
                description : "List of components of the Clock Tree being sourced by HFCLK",
                hidden      : false,
                default     : "",
                getValue    : (inst) => {
                    /* HFCLK is not available foor M0Lx device family */
                    if(Common.isDeviceM0L() || inst.clockTreeEn){
                        return "None";
                    }
                    let inUse = "";
                    if(inst.MCLKSource === "HSCLK" && "HFCLK" === inst.HSCLKSource){
                        inUse+= "MCLK, ";
                    }
                    if((inst.MCLKSource === "HSCLK")&&
                        (["SYSPLLCLK0","SYSPLLCLK2X"].includes(inst.HSCLKSource))){
                        if(inst.SYSPLLSource === "HFCLK"){
                            inUse+= "SYSPLL, ";
                        }
                    }
                    if(inst.MFPCLKEn && inst.MFPCLKSource === "HFCLK"){
                        inUse+= "MFPCLK, ";
                    }
                    if(inst.enableFCC && inst.fccClkSrc == "HFCLK"){
                        inUse+= "FCC, "
                    }
                    let ADCMod = system.modules["/ti/driverlib/ADC12"];
                    if (ADCMod){
                        let ADCInsts = ADCMod.$instances
                        for(let singleInst of ADCInsts){
                            if(singleInst.sampClkSrc === "DL_ADC12_CLOCK_HFCLK"){
                                if(singleInst.peripheral.$solution){
                                    inUse+= singleInst.peripheral.$solution.peripheralName+" ADCCLK, ";
                                }
                            }
                        }
                    }
                    if(inst.enableEXCLK && inst.EXCLKSource === "HFCLK"){
                        inUse+= "CLK_OUT, ";
                    }
                    if(inst.CANCLKSource === "HFCLK"){
                        inUse+= "CANCLK, ";
                    }

                    if(inUse.length == 0){
                        return "None"
                    }

                    return inUse;
                }
            },
            // setHFCLKSourceHFXT / setHFCLKSourceHFXTParams / setHFCLKSourceHFCLKIN
            {
                name: "HFCLKSource",
                displayName: "HFCLK Source",
                default: "HFCLK_IN",
                options: Options.HFCLKSource,
                onChange: (inst,ui) =>{
                    if(inst.useHFCLK_Manual && inst.HFCLKSource == "HFXT"){
                        inst.validateClkStatus = true;
                        ui.validateClkStatus.readOnly = true;
                    }
                    else{
                        ui.validateClkStatus.readOnly = (inst.LFCLKSource == "LFXT");
                    }
                },
            },
            // DL_SYSCTL_setHFCLKSourceHFXTParams -> range
            {
                name: "HFCLK_Freq",
                displayName: "External HF Clock Frequency (Hz)",
                default: 4000000,
                isInteger: true,
                range: [4000000,48000000]
            },
        ];
    }
    return [];
}
function getCANCLKConfig(){
    if(Common.isDeviceM0G()){
        return [
            // setHSCLKSource
            {
                name: "CANCLKSource",
                displayName: "CANCLK Source",
                description: "Specifies the clock source of the CAN Clock",
                default: "None",
                hidden: (Common.isDeviceM0G() === false),
                options: [
                    { name: "None" },
                    { name: "SYSPLLCLK1" },
                    { name: "HFCLK" },
                ]
            },
        ];
    }

    return [];
}

function getHSCLKConfig(){
    if(Common.isDeviceM0G()){
        return[
            // setHSCLKSource
            {
                name: "HSCLKSource",
                displayName: "HSCLK Source",
                description: "Specifies the clock source of the High Speed Clock",
                default: "SYSPLLCLK0",
                hidden: true,
                options: Options.HSCLKSource,
            },
            {
                name: "HSCLK_Freq",
                default: 0,
                hidden: true,
                getValue: (inst) => {
                    switch(inst.HSCLKSource){
                        case "SYSPLLCLK0":
                            return inst.SYSPLLSource != "Disabled"? inst.SYSPLL_Freq_CLK0 : 0;
                        case "SYSPLLCLK2X":
                            return inst.SYSPLLSource != "Disabled"? inst.SYSPLL_Freq_CLK2X : 0;
                        case "HFCLK":
                            return inst.HFCLK_Freq;
                            break;
                        default:
                            return 0;
                    }
                }
            },
        ];
    }
    return [];
}

function getNMIConfig(){
    let returnConfig = [
        //DL_SYSCTL_setWWDT0ErrorBehavior
        {
            name        : "nmiWWDT0Behav",
            displayName : "WWDT0 Error Behavior",
            description : 'Configures whether a WWDT0 error will trigger a BOOTRST or an NMI (non-maskable interrupt). By default, this error will trigger a BOOTRST.',
            hidden      : false,
            default     : "BOOTRST",
            options     : [
                { name: "BOOTRST" },
                { name: "Non-Maskable Interrupt" }
            ],
        },
    ];
    if(Common.isDeviceM0G()){
        returnConfig = returnConfig.concat([
            // DL_SYSCTL_setWWDT1ErrorBehavior
            {
                name        : "nmiWWDT1Behav",
                displayName : "WWDT1 Error Behavior",
                description : 'Configures whether a WWDT1 error will trigger a SYSRST or an NMI (non-maskable interrupt). By default, this error will trigger a SYSRST.',
                hidden      : false,
                default     : "SYSRST",
                options     : [
                    { name: "SYSRST" },
                    { name: "Non-Maskable Interrupt" }
                ],
            },
            //DL_SYSCTL_setFlashDEDErrorBehavior
            {
                name        : "nmiFlashDedBehav",
                displayName : "Flash DED Error Behavior",
                description : 'Configures whether a Flash DED error will trigger a SYSRST or an NMI (non-maskable interrupt). By default, this error will trigger a SYSRST.',
                hidden      : false,
                default     : "SYSRST",
                options     : [
                    { name: "SYSRST" },
                    { name: "Non-Maskable Interrupt" }
                ],
            },
        ]);
    }
    return returnConfig;
}

function getMFPCLKConfig(){
    let returnConfig = [
        // DL_SYSCTL_enableMFPCLK / DL_SYSCTL_disableMFPCLK
        {
            name        : "MFPCLKEn",
            displayName : "Enable MFPCLK",
            description : '',
            hidden      : false,
            default     : false
        },
    ];

    if(Common.isDeviceM0G()){
        returnConfig = returnConfig.concat([
        // setMFPCLKSource
            {
                name        : "MFPCLKSource",
                displayName : "MFPCLK Source",
                description : 'Source for the Middle Frequency Precision (MFP) CLK',
                longDescription : `
The source for MFPCLK, designed to be accurately set to 4MHz for precision peripherals
such as DAC.
                `,
                hidden      : false,
                default     : "SYSOSC",
                options     : [
                    // DL_SYSCTL_MFPCLK_SOURCE_[...]
                    {name: "SYSOSC", displayName: "SYSOSC"},
                    {name: "HFCLK", displayName: "HFCLK"},
                ],
                onChange: (inst, ui) => {
                    if(inst.MFPCLKSource == "HFCLK")
                    {
                        ui.HFCLK4MFPCLKDIV.hidden = false;
                    } else {
                        ui.HFCLK4MFPCLKDIV.hidden = true;
                    }
                }
            },
            {
                name        : "HFCLK4MFPCLKDIV",
                displayName : "HFCLK Divider for MFPCLK",
                description : "Divider value for HFCLK to MFPCLK to divide down for 4MHz",
                hidden      : true,
                default     : "DISABLE",
                options     : [
                { name: "DISABLE" , displayName: "1"}, {name: "2" }, {name: "3" }, {name: "4" }, {name: "5" },
                {name: "6" }, {name: "7" }, {name: "8" }, {name: "9" }, {name: "10" }, {name: "11" }, {name: "12" },
                {name: "13" }, {name: "14" }, {name: "15"}, {name: "16"}
                ]
            }
        ]);
    }
    /* MFPCLK Source selection is not available in M0Lx */
    else if(Common.isDeviceM0L()){
        returnConfig.push(
        // setMFPCLKSource
        {
            name        : "MFPCLKSource",
            displayName : "MFPCLK Source",
            description : '',
            hidden      : false,
            default     : "SYSOSC",
            readOnly    : false,
            options     : [
                // DL_SYSCTL_MFPCLK_SOURCE_[...]
                {name: "SYSOSC", displayName: "SYSOSC"},
            ],
        },)
    }
    return returnConfig;
}

function getULPCLKConfig(){
    if(Common.isDeviceM0G()){
        return [
            // setULPCLKDivider [!]
            {
                name: "UDIV",
                displayName: "ULPCLK Divider",
                description: "ULPCLK is MCLK divided by this value.",
                default: "1",
                options: [ { name: "1" }, { name: "2" }, { name: "3" } ]
            }
        ];
    }
    return [];
}

function getClkOutFreqConfig(){
    let clkOutConfig = [];
    clkOutConfig.push(
        { name: "ADC0_ADCCLK_Freq", displayName: "ADC0 ADCCLK", default: 32000000, hidden: true,
                getValue: (inst) => {
                    let ADCMod = system.modules["/ti/driverlib/ADC12"];
                    if (ADCMod){
                        let ADCInsts = ADCMod.$instances
                        for(let singleInst of ADCInsts){
                            if(singleInst.peripheral.$solution){
                                if(singleInst.peripheral.$solution.peripheralName == "ADC0"){
                                    return singleInst.inputClock_Freq
                                }
                            }
                        }
                    }
                    return inst.SYSOSC_Freq
                },
            },
            { name: "ADC0_ADCCLK_Freq_unit", displayName: "ADC0 ADCCLK", default: "32 MHz", readOnly: true, getValue: (inst)=>{return (Common.getUnitPrefix(inst.ADC0_ADCCLK_Freq)).str+"Hz"}},
    );
    if(Common.isDeviceM0G()){clkOutConfig.push(
        { name: "ADC1_ADCCLK_Freq", displayName: "ADC1 ADCCLK", default: 32000000, hidden: true,
                getValue: (inst) => {
                    let ADCMod = system.modules["/ti/driverlib/ADC12"];
                    if (ADCMod){
                        let ADCInsts = ADCMod.$instances
                        for(let singleInst of ADCInsts){
                            if(singleInst.peripheral.$solution){
                                if(singleInst.peripheral.$solution.peripheralName == "ADC1"){
                                    return singleInst.inputClock_Freq
                                }
                            }
                        }
                    }
                    return inst.SYSOSC_Freq
                },
            },
            { name: "ADC1_ADCCLK_Freq_unit", displayName: "ADC1 ADCCLK", default: "32 MHz", readOnly: true, getValue: (inst)=>{return (Common.getUnitPrefix(inst.ADC1_ADCCLK_Freq)).str+"Hz"}},
            { name: "CANCLK_Freq", displayName: "CANCLK", default: 48000000, readOnly: true, hidden: true,
                getValue: (inst) => {
                    if(inst.clockTreeEn){
                        return system.clockTree["net_canclk"].in * 1000000;
                    }else{
                        if(inst.CANCLKSource == "SYSPLLCLK1")
                        {
                            return inst.SYSPLL_Freq_CLK1;
                        }else if(inst.CANCLKSource == "HFCLK" && inst.useHFCLK_Manual){
                            return inst.HFCLK_Freq;
                        }else{
                            return 0;
                        }
                    }
                }},
            { name: "CANCLK_Freq_unit", displayName: "CANCLK", default: "48 MHz", readOnly: true, getValue: (inst)=>{return (Common.getUnitPrefix(inst.CANCLK_Freq)).str+"Hz"}},
    );}
    clkOutConfig.push(
        { name: "CPUCLK_Freq", displayName: "CPUCLK", default: 32000000, hidden: true, getValue: (inst) => { return inst.MCLK_Freq; } },
        { name: "CPUCLK_Freq_unit", displayName: "CPUCLK", default: "32 MHz", readOnly: true, getValue: (inst)=>{return (Common.getUnitPrefix(inst.CPUCLK_Freq)).str+"Hz"}},
        { name: "LFCLK_Freq", displayName: "LFCLK", default: 32768, hidden: true,
            getValue: (inst) => {
                if(inst.clockTreeEn){
                    return system.clockTree["net_lfclk"].in * 1000000;
                }
                switch(inst.LFCLKSource){
                    case "LFOSC":
                        return 32768;
                    case "LFXT":
                    case "LFCLK_IN":
                        return inst.LFCLK_Freq_IN;
                }
            }
        },
        { name: "LFCLK_Freq_unit", displayName: "LFCLK", default: "32.7 kHz", readOnly: true, getValue: (inst)=>{return (Common.getUnitPrefix(inst.LFCLK_Freq)).str+"Hz"}},
        { name: "MCLK_Freq",  displayName: "MCLK",  default: 32000000, hidden: true,
            getValue: (inst) => {
                if(inst.clockTreeEn){
                    return system.clockTree["net_mclk"].in * 1000000;
                }
                switch(inst.MCLKSource) {
                    case "SYSOSC":
                        return inst.MDIV == "DISABLE" ? inst.SYSOSC_Freq : inst.SYSOSC_Freq / parseInt(inst.MDIV);
                    case "HSCLK":
                        if(Common.isDeviceM0G()){
                            return inst.MDIV == "DISABLE" ? inst.HSCLK_Freq : inst.HSCLK_Freq / parseInt(inst.MDIV);
                        }
                        else{
                            return 0;
                        }
                    case "LFCLK":
                        return inst.LFCLK_Freq;
                    default:
                        return 0;
                }
            }
        },
        { name: "MCLK_Freq_unit", displayName: "MCLK", default: "32 MHz", readOnly: true, getValue: (inst)=>{return (Common.getUnitPrefix(inst.MCLK_Freq)).str+"Hz"}},
        { name: "MFCLK_Freq", displayName: "MFCLK", default: 4000000, hidden: true,
            getValue: (inst) => {
                if(inst.clockTreeEn){
                    return system.clockTree["net_mfclk"].in * 1000000;
                }
                return inst.MFCLKEn ? 4000000 : 0;
            }
        },
        { name: "MFCLK_Freq_unit", displayName: "MFCLK", default: "4 MHz", readOnly: true, getValue: (inst)=>{return (Common.getUnitPrefix(inst.MFCLK_Freq)).str+"Hz"}},
        { name: "MFPCLK_Freq", displayName: "mfpclk_freq", default: 4000000, hidden: true, readOnly: true,
            getValue: (inst) => {
                if(inst.clockTreeEn){
                    return system.clockTree["net_mfpclk"].in * 1000000;
                } else if(inst.MFPCLKEn && inst.MFPCLKSource === "HFCLK"){
                    return inst.HFCLK_FreqOut / ((inst.HFCLK4MFPCLKDIV == "DISABLE") ? 1 : parseInt(inst.HFCLK4MFPCLKDIV))
                } else {
                    return inst.MFPCLKEn ? 4000000:0;
                }
            }
        },
        { name: "MFPCLK_Freq_unit", displayName: "MFPCLK", default: "4 MHz", readOnly: true, getValue: (inst)=>{ return (Common.getUnitPrefix(inst.MFPCLK_Freq)).str+"Hz"}},
    );
    if(Common.isDeviceM0G()){clkOutConfig.push(
        { name: "RTCCLK_Freq", displayName: "RTCCLK", default: 32768, readOnly: true, hidden: true,},
        { name: "RTCCLK_Freq_unit", displayName: "RTCCLK", default: "32.7 kHz", readOnly: true, getValue: (inst)=>{return (Common.getUnitPrefix(inst.RTCCLK_Freq)).str+"Hz"}},
        { name: "HFCLK_FreqOut", displayName: "HFCLK", default: 0, hidden: true, readOnly: true,
            getValue: (inst) => {
                if(inst.clockTreeEn){
                    return system.clockTree["net_hfclk"].in * 1000000;
                }
                return inst.useHFCLK_Manual?inst.HFCLK_Freq:0;
            }
        },
        { name: "HFCLK_Freq_unit", displayName: "HFCLK", default: "0 MHz", readOnly: true, getValue: (inst)=>{return (Common.getUnitPrefix(inst.HFCLK_FreqOut)).str+"Hz"}},
    );}
    clkOutConfig.push(
        { name: "ULPCLK_Freq", displayName: "ULPCLK", default: 32000000, hidden: true,
            getValue: (inst) => {
                if(inst.clockTreeEn){
                    return system.clockTree["net_ulpclk"].in * 1000000;
                }
                if(Common.isDeviceM0G()){
                    return inst.MCLK_Freq / parseInt(inst.UDIV);
                }
                else if(Common.isDeviceM0L()){
                    return inst.MCLK_Freq;
                }
                else return 0;
            }
        },
        { name: "ULPCLK_Freq_unit", displayName: "ULPCLK", default: "32 MHz", readOnly: true, getValue: (inst)=>{return (Common.getUnitPrefix(inst.ULPCLK_Freq)).str+"Hz"}},
        { name: "SYSOSC_FreqOut", displayName: "SYSOSC", default: 32000000, hidden: true,
            getValue: (inst) => {
                if(inst.clockTreeEn){
                    return system.clockTree["net_sysosc"].in * 1000000;
                } else {
                    return inst.SYSOSC_Freq;
                }
            }
        },
        { name: "SYSOSC_Freq_unit", displayName: "SYSOSC", default: "32 MHz", readOnly: true, getValue: (inst)=>{return (Common.getUnitPrefix(inst.SYSOSC_FreqOut)).str+"Hz"}},
    );

    return clkOutConfig;
}

function getFlashConfig(){
    let flashConfig = [
        // DL_FLASHCTL_INTERRUPT_DONE
        {
            name: "enableIntDone",
            displayName: "Enable FlashCtl Interrupt",
            description: "Enable flash command execution is done interrupt",
            longDescription: "Enable flash command execution is done interrupt",
            default: false,
            hidden: false,
            onChange: (inst, ui) => {
                ui.flashInterruptPriority.hidden = !inst.enableIntDone;
            }
        },
        {
            name: "flashInterruptPriority",
            displayName: "Flash Interrupt Priority",
            description: "Set the interrupt priority of Flash as a part of Group 0",
            longDescription: `
The FLASH interrupt is placed in the INT_GROUP0 peripheral group,
thus their interrupt priority is tied to other members of the group.

INT_GROUP0:
* WWDT (all)
* DEBUGSS
* FLASHCTL
* FSUB1/FSUB0
* Power Management Control Unit (SYSCTL)
            `,
            hidden: true,
            default: "DEFAULT",
            options: Common.InterruptPriorityOptions
        }
    ];
    if(Common.isDeviceM0G()){flashConfig.push(
            //DL_SYSCTL_setFlashWaitState()
            {
                name        : "waitState",
                displayName : "Wait States",
                description : 'Desired number of flash wait states.',
                hidden      : true,
                default     : "2",
                options     : [
                    // DL_SYSCTL_FLASH_WAIT_STATE_
                    {name: "0",},
                    {name: "1",},
                    {name: "2",},
                ],
            },
    );}
    return flashConfig;
}

function getPowerSysIntConfig(){
    if(Common.isDeviceM0G()){
        return [
            //DL_SYSCTL_enableInterrupt
            {
                name        : "powerSysInterrupts",
                displayName : "Enable Interrupts",
                description : '',
                hidden      : false,
                default     : [],
                minSelections: 0,
                options     : getPowerSysInterrupts(),
                onChange    : (inst, ui) => {
                    ui.pmcuInterruptPriority.hidden = (inst.powerSysInterrupts.length === 0)
                }
            },
            {
                name: "pmcuInterruptPriority",
                displayName: "Power Interrupt Priority",
                description: "Interrupt Priority of the Power Interrupts (Group 0)",
                longDescription: `
The power interrupts are placed in the INT_GROUP0 peripheral group,
thus their interrupt priority is tied to other members of the group.

INT_GROUP0:
* WWDT (all)
* DEBUGSS
* FLASHCTL
* FSUB1/FSUB0
* Power Management Control Unit (SYSCTL)
                `,
                hidden: true,
                default: "DEFAULT",
                options: Common.InterruptPriorityOptions
            }
        ]
    }
    return [];
}

function getInterruptGroupConfig(){
    let config = [
        {
            name: "intGroup0Primary",
            hidden: true,
            default: "",
            getValue: (inst) => {
                let priorityArg = 2048; // much larger than any real priority
                let outStr = "";
                let modWWDT = system.modules["/ti/driverlib/WWDT"];
                if(modWWDT && modWWDT.$instances){
                    for(let inst of modWWDT.$instances){
                        // finds the index since WWDT is dynamically assigned by the solver
                        let instIdx = inst.peripheral.$solution?parseInt(inst.peripheral.$solution.peripheralName.slice(-1)):0;
                        if(inst.enabledInterrupts && inst.interruptPriority !== "DEFAULT"){
                            let candidatePriority = parseInt(inst.interruptPriority);
                            if(candidatePriority < priorityArg){
                                priorityArg = candidatePriority;
                                outStr = "/ti/driverlib/WWDT:$instances:" + inst.$name +
                                    ":WWDT"+ instIdx;
                            }
                        }
                    }
                }
                /* SYSCTL interrupts, found in inst */
                /* Disabled when TI Drivers configuration enabled */
                if(!inst.driversEn){
                    if(inst.enableIntDone && inst.flashInterruptPriority !== "DEFAULT"){
                        let candidatePriority = parseInt(inst.flashInterruptPriority);
                        if(candidatePriority < priorityArg){
                            priorityArg = candidatePriority;
                            outStr = "/ti/driverlib/SYSCTL:$static:flashInterruptPriority:FLASHCTL";
                        }
                    }
                    if(Common.isDeviceM0G()){
                        if(inst.powerSysInterrupts.length > 0 && inst.pmcuInterruptPriority !== "DEFAULT"){
                            let candidatePriority = parseInt(inst.pmcuInterruptPriority);
                            if(candidatePriority < priorityArg){
                                priorityArg = candidatePriority;
                                outStr = "/ti/driverlib/SYSCTL:$static:pmcuInterruptPriority:SYSCTL";
                            }
                        }
                    }
                }

                // iterate through all pins
                return outStr;
            }
        },
        {
            name: "intGroup0Priority",
            hidden: true,
            default: "DEFAULT",
            options: Common.InterruptPriorityOptions,
            getValue: (inst) => {
                let priority = "DEFAULT";
                if(inst.intGroup0Primary !== "")
                {
                    let refStr = _.split(inst.intGroup0Primary, ':', 4);
                    if(refStr.length >= 3){
                        try {
                            if(refStr[1] === "$static"){
                                priority = system.modules[refStr[0]][refStr[1]][refStr[2]];
                            } else {
                                priority = _.find(system.modules[refStr[0]][refStr[1]], ['$name', refStr[2]])["interruptPriority"];
                            }
                        } catch {
                            throw "interrupt group 0 didn't exist appropriately";
                        }
                    }
                }
                return priority;
            }
        },
        {
            name: "intGroup0IRQnStr",
            hidden: true,
            default: "SYSCTL_INT_IRQn",
            getValue: (inst) => {
                let str = "SYSCTL_INT_IRQn";
                if(inst.intGroup0Primary !== "")
                {
                    let refStr = _.split(inst.intGroup0Primary, ':', 4);
                    if(refStr.length === 4){
                        try {
                            str = refStr[3] + "_INT_IRQn";
                        } catch {
                            throw "interrupt group 0 didn't exist appropriately";
                        }
                    }
                }
                return str;
            }
        },
        {
            name: "intGroup1Primary",
            hidden: true,
            default: "",
            getValue: (inst) => {
                let priorityArg = 2048; // much larger than any real priority
                let outStr = "";
                let pinmodGPIO = system.modules["/ti/driverlib/gpio/GPIOPin"];
                // iterate through all pins
                if(pinmodGPIO && pinmodGPIO.$instances){
                    for (let pinst of pinmodGPIO.$instances){
                        if(pinst.direction == "INPUT" && pinst.interruptEn){
                            if(pinst.interruptPriority !== "DEFAULT"){
                                let candidatePriority = parseInt(pinst.interruptPriority);
                                priorityArg = (candidatePriority < priorityArg)?candidatePriority:priorityArg;
                                let port = Common.getGPIOPort(pinst.pin.$solution.peripheralPinName);
                                outStr = "/ti/driverlib/gpio/GPIOPin:$instances:" + pinst.$name + ":" + port;
                            }
                        }
                    }
                }
                let modCOMP = system.modules["/ti/driverlib/COMP"];
                if(modCOMP && modCOMP.$instances){
                    for(let inst of modCOMP.$instances){
                        if(inst.enabledInterrupts.length > 0 && inst.interruptPriority !== "DEFAULT"){
                            let candidatePriority = parseInt(inst.interruptPriority);
                            if(candidatePriority < priorityArg){
                                priorityArg = candidatePriority;
                                let solution = inst.peripheral.$solution;
                                let peripheral = system.deviceData.peripherals[solution.peripheralName];
                                outStr = "/ti/driverlib/COMP:$instances:" + inst.$name + ":" + peripheral.name;
                            }
                        }
                    }
                }
                let modTRNG = system.modules["/ti/driverlib/TRNG"];
                if(modTRNG && modTRNG.$static){
                    let stat = modTRNG.$static;
                    if(stat.length !== 0 && stat.enabledInterrupts.length > 0 && stat.interruptPriority !== "DEFAULT"){
                        let candidatePriority = parseInt(stat.interruptPriority);
                        if(candidatePriority < priorityArg){
                            priorityArg = candidatePriority;
                            outStr = "/ti/driverlib/TRNG:$static:interruptPriority:TRNG";
                        }
                    }
                }
                return outStr;
            }
        },
        {
            name: "intGroup1Priority",
            hidden: true,
            default: "DEFAULT",
            options: Common.InterruptPriorityOptions,
            getValue: (inst) => {
                let priority = "DEFAULT";
                if(inst.intGroup1Primary !== "")
                {
                    let refStr = _.split(inst.intGroup1Primary, ':', 3);
                    if(refStr.length === 3){
                        try {
                            if(refStr[1] === "$static"){
                                priority = system.modules[refStr[0]][refStr[1]][refStr[2]];
                            } else {
                                priority = _.find(system.modules[refStr[0]][refStr[1]], ['$name', refStr[2]])["interruptPriority"];
                            }
                        } catch {
                            throw "interrupt group 1 didn't exist appropriately";
                        }
                    }
                }
                return priority;
            }
        },
        {
            name: "intGroup1IRQnStr",
            hidden: true,
            default: "GPIOA_INT_IRQn",
            getValue: (inst) => {
                let str = "GPIOA_INT_IRQn";
                if(inst.intGroup1Primary !== "")
                {
                    let refStr = _.split(inst.intGroup1Primary, ':', 4);
                    if(refStr.length === 4){
                        try {
                            str = refStr[3] + "_INT_IRQn";
                        } catch {
                            throw "interrupt group 1 didn't exist appropriately";
                        }
                    }
                }
                return str;
            }
        },
    ];
    return config;
}

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base sysctl configuration
 */
let devSpecific = {

    /* Important Note about disabling sysctl:
     *  Once software disables SWD functionality, it is not possible to re-
     * enable it other than by triggering a POR. A POR will automatically
     * re-enable the SWD functionality and put the SWD pins into SWD
     * mode with pullup/pulldown resistors enabled. To re-gain sysctl access
     * to a device which contains software that disables the SWD pins at
     * startup, it is necessary to hold the device in a reset state with the
     * NRST pin during a POR.
     */

    config: [
        {
            name: "GROUP_PWR_SYS",
            displayName: "Power & Systems Configuration",
            description: "",
            longDescription: "",
            collapsed: false,
            config: [
                // TODO: [3/8 (H) - work in progress drivers SysCtl behavior]
                {
                    name: "driversEn",
                    displayName: "Use TI Drivers Configuration",
                    default: false,
                    onChange: onChangeUseDrivers,
                    hidden: true,
                },
                // DL_SYSCTL_setPowerPolicy[...]
                {
                    name: "runPowerPolicy",
                    displayName: "Run Power Policy",
                    description: `Set power policy during runtime (CPU up). Defined by clock configuration`,
                    longDescription: `
The run power policy of the device is set by the clocks that are enabled and how they are configured.
This configurable cannot be set but rather is a product of the clock configuration. The different policies are:

* **RUN0**: MCLK and CPUCLK can fun from a fast clock source (SYSOSC or a High-Frequency Clock if applicable)
* **RUN1**: MCLK and CPUCLK run from LFCLK (at 32 kHz), but SYSOSC is left enabled in order to service analog modules
* **RUN2**: MCLK and CPUCLK run from LFCLK, and SYSOSC is disabled. Lowest power state with the CPU running.

**NOTE** RUN1 and RUN2 require all HFCLK peripherals (HFXT, HFCLK_IN, SYSPLL if equipped) to be off, thus it
is not possible to source MCLK from LFCLK and have a high-frequency external clock.

**NOTE** Not every low power state can be reached from every run policy. There are some transitions that are
not possible or logical, such as:

* RUN A -> SLEEP B and A != B : MCLK doesn't change configuration so these are incompatible
* RUN1/2 -> STOP1 : SYSOSC is throttled down for MCLK use, not necessary for RUN1 (4MHz max in stop mode on ULPCLK) and off in RUN2.

                    `,
                    default: "RUN0",
                    options: [
                        { name: "RUN0" },
                        { name: "RUN1" },
                        { name: "RUN2" },
                    ],
                    getValue: (inst) => {
                        let policy = "RUN0";
                        if(inst.clockTreeEn){
                            let mod = system.modules["/ti/clockTree/mux.js"];
                            let mclkSrcInst = _.find(mod.$instances, ['$name', 'SYSCTLMUX']);
                            mod = system.modules["/ti/clockTree/oscillator.js"];
                            let sysoscInst = _.find(mod.$instances, ['$name', 'SYSOSC']);
                            if(_.isUndefined(mclkSrcInst) || _.isUndefined(sysoscInst)){
                                throw "runPowerPolicy cannot find appropriate modules";
                            }
                            if(_.endsWith(mclkSrcInst.inputSelect,"LFCLK")){
                                policy = sysoscInst.disableSYSOSC?"RUN2":"RUN1";
                            }
                        } else {
                            if(inst.MCLKSource === "LFCLK"){
                                policy = inst.disableSYSOSC?"RUN2":"RUN1";
                            }
                        }
                        return policy;
                    }

                },
                {
                    name: "powerPolicy",
                    displayName: "Low Power Policy",
                    description: `Set the low power state of the device when _WFI() called`,
                    longDescription: `
The low power state of the device is entered when _WFI() is called from the main function.
In each of these, the CPU will turn off but some subset of peripherals can remain active.
The severity of the power state goes:
* **SLEEP**: Same as corresponding run mode but CPU disabled
* **STOP**: SLEEP plus PD1 peripherals are disabled and in retention max PD0 frequency is 4MHz
* **STANDBY**: STOP plus max PD0 frequency now 32KHz and SYSOSC always disabled.
Additionally, higher numbers save more power by disabling more features`,
                    default: "SLEEP0",
                    options: [
                        { name: "SLEEP0" },
                        { name: "SLEEP1" },
                        { name: "SLEEP2" },
                        { name: "STOP0" },
                        { name: "STOP1" },
                        { name: "STOP2" },
                        { name: "STANDBY0" },
                        { name: "STANDBY1" }
                    ]
                },
                {
                    name        : "VDD",
                    displayName : "Supplied Operational Voltage (VDD)",
                    description : "Supplied Operational Voltage for the Board supplied by the user",
                    longDescription: "Voltage given for the DC Operation of the Device, used by GPIO pins for internal operating parameters.",
                    default     : 3.3,
                    range       : [1.71, 5]
                },
                //  DL_SYSCTL_setBORThreshold
                // DL_SYSCTL_BOR_THRESHOLD_LEVEL_[...]
                {
                    name        : "BORThresh",
                    displayName : "BOR Threshold",
                    description : ' Set the brown-out reset (BOR) threshold level',
                    longDescription: `Set the brown-out reset (BOR) threshold level.
                    A BOR0 violation will force a re-boot, this is the minimum allowed threshold.
                    A BOR1-BOR3 violation generates an interrupt.
                    `,
                    hidden      : false,
                    default     : "0",
                    options     : [
                        {name: "0"},
                        {name: "1"},
                        {name: "2"},
                        {name: "3"},
                    ],
                },
                // DL_SYSCTL_enableWriteLock / DL_SYSCTL_disableWriteLock
                {
                    name        : "enableWriteLock",
                    displayName : "Enable Write Lock",
                    description : 'Enable write protection of selected SYSCTL registers',
                    longDescription: `Enable write protection of selected SYSCTL registers.
                    Does not protect all SYSCTL registers, see TRM for more detail.
                    `,
                    hidden      : false,
                    default     : false
                },
                // DL_SYSCTL_enableSleepOnExit
                {
                    name        : "enableSleepOnExit",
                    displayName : "Enable Sleep On Exit",
                    description : '',
                    longDescription: `Enables sleep on exit when the CPU moves from handler mode to thread mode.
                    By enabling, allows an interrupt driven application to avoid returning to an empty main application.
                    `,
                    hidden      : false,
                    default     : false
                },
                // DL_SYSCTL_enableEventOnPend
                {
                    name        : "enableEventOnPending",
                    displayName : "Enable Event On Pending",
                    description : 'Enable send event on pending bit',
                    longDescription: `When enabled, any enabled event and all interrupts (including disabled
                    interrupts) can wakeup the processor.
                    `,
                    hidden      : false,
                    default     : false
                },
                // DL_SYSCTL_disableNRSTPin
                {
                    name        : "disableNRSTPin",
                    displayName : "Disable NRST Pin",
                    description : 'Disable the functionality of the NRST pin',
                    longDescription: `When disabled, the NRST pin becomes free to use as a GPIO. This
                    can be advantageous for lower pin count devices.
                    **NOTE:** Once disabled, it can only be re-enabled by a POR.
                    `,
                    hidden      : false,
                    default     : false
                },
                // DL_SYSCTL_setVBOOSTConfig(DL_SYSCTL_VBOOST)
                {
                    name: "vboostMode",
                    displayName: "VBOOST Operating Mode",
                    longDescription: `
Sets operating mode of VBOOST (analog charge pump), the available modes are in the following table:

| Mode | VBOOST Enable | Application Use-Case |
| :------: | :----------: | :--------: |
| ONDEMAND | VBOOST is automatically enabled by SYSCTL only when a COMP, OPA, or the GPAMP is enabled. | This setting provides the lowest power consumption in all modes when fast startup of the COMP, OPA, or GPAMP is not critical. |
| ONACTIVE | VBOOST is forced to be enabled when the device is in RUN or SLEEP mode. VBOOST is also kept enabled in STOP or STANDBY mode if a COMP, OPA, or the GPAMP is enabled. | This setting provides low power consumption by disabling the VBOOST automatically in STOP and STANDBY modes when no peripherals requiring VBOOST are enabled. VBOOST is automatically reenabled upon exit to RUN mode to provide fast startup of the COMP, OPA, or GPAMP in the event that application software enables a COMP, OPA, or GPAMP in RUN mode |
| ONALWAYS | VBOOST is forced to be enabled in all operating modes except SHUTDOWN. | This setting ensures that the COMP, OPA, and GPAMP never incur additional startup latency due to VBOOST startup in applications where fast COMP, OPA, or GPAMP startup is critical. |
                    `,
                    default: "ONDEMAND",
                    hidden: false,
                    options: [
                        {name: "ONDEMAND", displayName: "ONDEMAND"},
                        {name: "ONACTIVE", displayName: "ONACTIVE"},
                        {name: "ONALWAYS", displayName: "ONALWAYS"},
                    ],
                },
                {
                    name: "GROUP_FLASH_CONFIG",
                    displayName: "Flash Controller (FlashCtl) Configuration",
                    description: "",
                    longDescription: "",
                    collapsed: false,
                    config: getFlashConfig(),
                },
                {
                    name: "GROUP_PWR_SYS_Interrupts",
                    displayName: "Power & Systems Interrupts",
                    description: "",
                    longDescription: "",
                    collapsed: false,
                    config: getPowerSysIntConfig()
                },
                {
                    name: "GROUP_NMI_Interrupts",
                    displayName: "Non-Maskable Interrupt (NMI) Configuration",
                    description: "",
                    longDescription:
`
The SYSCTL module supports generation of a non-maskable interrupt (NMI) to the Cortex-M0+ processor for some system error conditions.
The default behavior for some system error conditions can be configured.
`,
                    collapsed: false,
                    config: getNMIConfig(),
                },
            ]
        },
        {
            name: "GROUP_CLK",
            displayName: "Clock Configuration",
            description: "",
            longDescription: getClockTreeDesc(),
            collapsed: false,
            config: [

                {
                    name: "clockTreeEn",
                    displayName: "Use Clock Tree",
                    description: "Use the graphical Clock Tree to configure clocking",
                    longDescription: `
_Beta_: Clock Tree is a new feature supported by SysConfig that allows a graphical configuration of all clocks present in the system. It is found
by clicking the signal icon in the top left of the window. There are two views that allow for the user to configure clocks graphically and
easily see signal flows and connections rather than using menus. Code generation is supported for most cases but is not guaranteed. Validation
of clock configurations is also not checked thoroughly yet, but is planned for future releases.

To get started:
* Enable Clock Tree using this button
* Click the signal icon on the top left of the sysconfig window
* Begin to click blocks of interest in order to manipulate the clock values
                    `,
                    default: false,
                    onChange: onChangeClockTree
                },
                {
                    name: "validateClkStatus",
                    displayName: "Enable Check for Clock Stabilization",
                    description: "Waits for clocks to return correct status",
                    longDescription: "Check For Clock Stabilization from an external source, must be on for external crystal",
                    default: false,
                    readOnly: false
                },
                {
                    name: "MCLK_Config_Group",
                    displayName: "MCLK (Main Clock)",
                    collapsed: false,
                    config: [
                        // DL_SYSCTL_setMCLKSource
                        {
                            name: "MCLKSource",
                            displayName: "MCLK Source",
                            description: "Specifies the clock source",
                            longDescription:`The frequency of the clock sources are
                                defined in their respective configurables`,
                            default: "SYSOSC",
                            options: Options.MCLKSource,
                            onChange: (inst, ui) => {
                                ui.disableSYSOSC.hidden = !(inst.MCLKSource == "LFCLK");
                                inst.disableSYSOSC = false;
                                if(Common.isDeviceM0G()){
                                    ui.HSCLKSource.hidden = !(inst.MCLKSource == "HSCLK");
                                    /* Wait configuration available only when MCLK source is HSCLK */
                                    ui.waitState.hidden = !inst.clockTreeEn && !(inst.MCLKSource == "HSCLK");
                                }
                                /* MDIV disabled when MCLK Source is HSCLK or LFCLK */
                                if(["HSCLK","LFCLK"].includes(inst.MCLKSource)){
                                    inst.MDIV = "DISABLE";
                                    ui.MDIV.hidden = true;
                                }
                                else{
                                    ui.MDIV.hidden = false;
                                }
                            }
                        },
                        // DL_SYSCTL_switchMCLKfromSYSOSCtoLFCLK
                        {
                            name: "disableSYSOSC",
                            displayName: "Disable SYSOSC",
                            description: "SYSOSC can be disabled if LFCLK is the clock used by MCLK",
                            default: false,
                            hidden: true
                        },
                        {
                            name: "MDIV",
                            displayName: "Main Clock Divider",
                            default: "DISABLE",
                            options: [ { name: "DISABLE" , displayName: "1"}, {name: "2" }, {name: "3" }, {name: "4" }, {name: "5" },
                                       {name: "6" }, {name: "7" }, {name: "8" }, {name: "9" }, {name: "10" }, {name: "11" }, {name: "12" },
                                       {name: "13" }, {name: "14" }, {name: "15"}, {name: "16"} ],
                            hidden: false
                        },
                        {
                            name: "HSCLK_Config_Group",
                            displayName: "HSCLK (High Speed Clock)",
                            collapsed: false,
                            config: getHSCLKConfig(),
                        },
                    ]
                },
                // TODO:(H) MFCLK can only be enabled based on certain power policies. throw error otherwise
                {
                    name: "MFCLK_Config_Group",
                    displayName: "MFCLK (Middle Frequency Clock)",
                    description: "",
                    longDescription: "",
                    collapsed: true,
                    config: [
                        // DL_SYSCTL_enableMFCLK
                        {
                            name: "MFCLKEn",
                            displayName: "MFCLK Enable",
                            description: "Enable the Mid-Frequency Clock (4MHz always)",
                            longDescription: `The Mid-Frequency Clock is a constant frequency 4MHz
                            clock that is sourced from the SYSOSC. Peripherals, specifically PD0
                            peripherals which may be used in STOP mode, may select MFCLK as their
                            clock source instead of ULPCLK. MFCLK will be maintained at 4MHz in RUN,
                            SLEEP and STOP, which is ideal for peripherals like UART, I2C, and low
                            power timers that need a consistent clock but require a clock source
                            greater than 32kHz`,
                            default: false
                        },
                    ]
                },
                {
                    name: "LFCLK_Config_Group",
                    displayName: "LFCLK (Low-Frequency Clock)",
                    description: "",
                    longDescription: "",
                    collapsed: true,
                    config: [
                        // DL_SYSCTL_setLFCLKSourceEXLF / setLFCLKSourceLFXT [!]
                        {
                            name: "LFCLKSource",
                            displayName: "LFCLK Source",
                            default: "LFOSC",
                            options: Options.LFCLKSource,
                            onChange: (inst,ui) => {
                                if(inst.LFCLKSource == "LFCLK_IN"){
                                    ui.LFCLK_Freq_IN.hidden = false;
                                } else {
                                    ui.LFCLK_Freq_IN.hidden = true;
                                    inst.LFCLK_Freq_IN = 32768;
                                }

                                if(Common.isDeviceM0G()){
                                    if(inst.LFCLKSource == "LFXT"){
                                        inst.validateClkStatus = true;
                                        ui.validateClkStatus.readOnly = true;
                                    }
                                    else{
                                        ui.validateClkStatus.readOnly = (inst.useHFCLK_Manual && inst.HFCLKSource == "HFXT");
                                    }
                                }
                            }
                        },
                        {
                            name: "LFCLK_Freq_IN",
                            displayName: "External LF Clock Frequency (Hz)",
                            default: 32768,
                            isInteger: true,
                            range: [29491, 36045],
                            hidden: true
                        },
                    ]
                },
                {
                    name: "MFPCLK_Config_Group",
                    displayName: "MFPCLK (Middle Frequency Precision Clock)",
                    description: "",
                    longDescription: "",
                    collapsed: true,
                    config: getMFPCLKConfig(),
                },
                {
                    name: "ULPCLK_Config_Group",
                    displayName: "ULPCLK (Low Power Clock)",
                    description: "",
                    longDescription: "",
                    collapsed: true,
                    config: getULPCLKConfig(),
                },
                {
                    name: "HFCLK_Config_Group",
                    displayName: "HFCLK (High-Frequency External Clock)",
                    collapsed: true,
                    config: getHFCLKConfig(),
                },
                {
                    name: "CANCLK_Config_Group",
                    displayName: "CANCLK (CAN Clock)",
                    description: "",
                    longDescription: "",
                    collapsed: true,
                    config: getCANCLKConfig(),
                },
                {
                    name: "EXCLK_Config_Group",
                    displayName: "External Clock Output (CLK_OUT)",
                    collapsed: true,
                    config: [
                        // DL_SYSCTL_enableExternalClock()
                        {
                            name: "enableEXCLK",
                            displayName: "Enable CLK_OUT",
                            description: "",
                            default: false,
                            hidden: false,
                            onChange: (inst,ui) => {
                                ui.clkOutPinDisplay.hidden  = !inst.enableEXCLK;
                                ui.clkOutPinIO.hidden       = !inst.enableEXCLK;
                            }
                        },
                        {
                            name: "clkOutPinDisplay",
                            displayName: "CLK_OUT Pin",
                            description: "",
                            default: "N/A",
                            hidden: true,
                            getValue: (inst) => {
                                try{
                                    return inst.peripheral.clkOutPin.$solution.devicePinName;
                                } catch(e){
                                    return "N/A"
                                }

                            }
                        },
                        {
                            name: "clkOutPinIO",
                            displayName: "CLK_OUT Pin IO Structure",
                            description: "",
                            default: "N/A",
                            hidden: true,
                            options: [
                                {name: "N/A", displayName: "N/A"},
                                {name: "SD", displayName: "Standard"},
                                {name: "SDL", displayName: "Standard"},
                                {name: "SDW", displayName: "Standard with Wake"},
                                {name: "HS", displayName: "High-Speed"},
                                {name: "HD", displayName: "High-Drive"},
                                {name: "OD", displayName: "5V Tolerant Open Drain"},
                            ],
                            getValue: (inst) => {
                                try{
                                    return system.deviceData.gpio.pinInfo[inst.peripheral.clkOutPin.$solution.packagePinName].devicePin.attributes.io_type;
                                } catch(e){
                                    return "N/A"
                                }
                            }
                        },
                        // DL_SYSCTL_CLK_OUT_SOURCE_[...]
                        {
                            name: "EXCLKSource",
                            displayName: "CLK_OUT Source",
                            default: "SYSOSC",
                            options: Options.EXCLKSource,
                        },
                        //  DL_SYSCTL_CLK_OUT_DIVIDE_[...]
                        {
                            name: "EXCLKDivider",
                            displayName: "CLK_OUT Divider",
                            default: "DISABLE",
                            options: [ { name: "DISABLE"}, {name: "2" }, {name: "4" },
                               {name: "6" }, {name: "8" }, {name: "10" }, {name: "12" },
                               {name: "14" }, {name: "16"} ],
                        },
                        {
                            name: "ClkOut_Freq",
                            default: 32,
                            getValue: (inst, ui) => {
                                let sourceFreq = inst.EXCLKSource + "_Freq";
                                let div = (inst.EXCLKDivider == "DISABLE")?1:parseInt(inst.EXCLKDivider);
                                return inst[sourceFreq] / div;
                            }
                        },
                        {
                            name: "ClkOutForceHighDrive",
                            displayName: "Force High Drive",
                            description: "Force the output pin for the CLK_OUT Pin to High Drive",
                            longDescription: `
Forces the output pin to be of type HDIO or HSIO configured with High-Drive strength, useful when driving other devices.
High Drive is turned on automatically if the CLK_OUT frequency is above 32 MHz when VDD >= 2.7 or above 16 MHz otherwise.
                            `,
                            default: false
                        },
                        {
                            name: "ClkOutHighDriveEn",
                            default: false,
                            hidden: true,
                            getValue: (inst, ui) => {
                                if(inst.enableEXCLK){
                                    if(inst.ClkOutForceHighDrive){
                                        return true;
                                    } else {
                                        // automatic enabling due to the frequency
                                        let clkOutFreq = inst.ClkOut_Freq;
                                        let index = (inst.VDD >= 2.7)?1:0;
                                        if(clkOutFreq >  (maxFrequencyCLKOUTDir["SD"][index] * 1000000) &&
                                           clkOutFreq <= (maxFrequencyCLKOUTDir["HS"][index] * 1000000)){
                                            // in this condition, SD pins will already be filtered out
                                            // so we can be certain the pin in use will be high-speed
                                            return true;
                                        }
                                    }
                                }
                                return false
                            }
                        }
                    ]
                },
                {
                    name: "GROUP_FCC_CONFIG",
                    displayName: "FCC Configuration",
                    description: "",
                    longDescription: `
Configure the Frequency Clock Counter (FCC).
FCC enables flexible in-system testing and calibration of a variety of oscillators
and clocks on the device. The FCC counts the number of clock periods seen on the
selected clock source within a known fixed trigger period (derived from a secondary
reference source) to provide an estimation of the frequency of the source clock.
                    `,
                    collapsed: true,
                    config: [
                        {
                            name        : "enableFCC",
                            displayName : "Enable FCC",
                            description : '',
                            hidden      : false,
                            default     : false,
                            onChange    : (inst,ui) => {
                                ui.fccTrigLvl.hidden = !inst.enableFCC;
                                ui.fccTrigSrc.hidden = !inst.enableFCC;
                                ui.fccClkSrc.hidden = !inst.enableFCC;
                                ui.fccPeriods.hidden = (!inst.enableFCC) || !(inst.fccTrigLvl == "RISE_RISE");
                            }
                        },
                        // DL_SYSCTL_configFCC()
                        // DL_SYSCTL_FCC_TRIG_TYPE_ -> if RISE selected, must set period
                        {
                            name        : "fccTrigLvl",
                            displayName : "FCC Trigger Level",
                            description : 'Determines if active high level trigger or rising-edge to rising-edge',
                            hidden      : true,
                            default     : "RISE_RISE",
                            options     : [
                                {name: "RISE_RISE", displayName: "Rising-edge to rising-edge pulse"},
                                {name: "LEVEL", displayName: "Active-high pulse level"},
                            ],
                            onChange    : (inst,ui) => {
                                ui.fccPeriods.hidden = !(inst.fccTrigLvl == "RISE_RISE");
                            }
                        },
                        // DL_SYSCTL_setFCCPeriods()
                        {
                            name        : "fccPeriods",
                            displayName : "FCC Periods",
                            description : 'Set the number of rising-edge to rising-edge period for FCC',
                            hidden      : true,
                            default     : "01",
                            options     : [
                                // DL_SYSCTL_FCC_TRIG_CNT_
                                {name: "01", displayName: "1"},
                                {name: "02", displayName: "2"},
                                {name: "03", displayName: "3"},
                                {name: "04", displayName: "4"},
                                {name: "05", displayName: "5"},
                                {name: "06", displayName: "6"},
                                {name: "07", displayName: "7"},
                                {name: "08", displayName: "8"},
                                {name: "09", displayName: "9"},
                                {name: "10", displayName: "10"},
                                {name: "11", displayName: "11"},
                                {name: "12", displayName: "12"},
                                {name: "13", displayName: "13"},
                                {name: "14", displayName: "14"},
                                {name: "15", displayName: "15"},
                                {name: "16", displayName: "16"},
                                {name: "17", displayName: "17"},
                                {name: "18", displayName: "18"},
                                {name: "19", displayName: "19"},
                                {name: "20", displayName: "20"},
                                {name: "21", displayName: "21"},
                                {name: "22", displayName: "22"},
                                {name: "23", displayName: "23"},
                                {name: "24", displayName: "24"},
                                {name: "25", displayName: "25"},
                                {name: "26", displayName: "26"},
                                {name: "27", displayName: "27"},
                                {name: "28", displayName: "28"},
                                {name: "29", displayName: "29"},
                                {name: "30", displayName: "30"},
                                {name: "31", displayName: "31"},
                                {name: "32", displayName: "32"},

                            ],
                        },
                        // DL_SYSCTL_FCC_TRIG_SOURCE
                        {
                            name        : "fccTrigSrc",
                            displayName : "FCC Trigger Source",
                            description : 'Determines which clock source to trigger FCC from',
                            hidden      : true,
                            default     : "FCC_IN",
                            options     : [
                                {name: "FCC_IN", displayName: "FCC_IN external pin"},
                                {name: "LFCLK", displayName: "LFCLK"},
                            ],
                        },
                        // DL_SYSCTL_FCC_CLOCK_SOURCE
                        {
                            name        : "fccClkSrc",
                            displayName : "FCC Clock Source",
                            description : 'Which clock source to capture and measure frequency of',
                            hidden      : true,
                            default     : "MCLK",
                            options     : getFCCClkSrcs(),
                        },
                    ],
                },
                {
                    name: "INT_OSC_NAME",
                    displayName: "Internal Oscillators",
                    description: "",
                    longDescription: "",
                    collapsed: true,
                    config: [
                        {
                            name: "SYSOSC_Config_Group",
                            displayName: "SYSOSC (Internal System Oscillator)",
                            collapsed: true,
                            config: [
                                {
                                    name        : "usesSYSOSC",
                                    displayName : "Sourced by SYSOSC",
                                    description : "List of components of the Clock Tree being sourced by SYSOSC",
                                    hidden      : false,
                                    default     : "",
                                    getValue    : (inst) => {
                                        if(inst.clockTreeEn){
                                            return "None";
                                        }
                                        let inUse = "";
                                        if(!inst.disableSYSOSC){
                                            if(inst.MCLKSource === "SYSOSC"){
                                                inUse+= "MCLK, ";
                                            }
                                            if(inst.MFPCLKEn && inst.MFPCLKSource === "SYSOSC"){
                                                inUse+= "MFPCLK, ";
                                            }
                                            if(inst.MFCLKEn){
                                                inUse+= "MFCLK, ";
                                            }
                                            if(inst.enableEXCLK && inst.EXCLKSource === "SYSOSC"){
                                                inUse+= "CLK_OUT, ";
                                            }
                                            let ADCMod = system.modules["/ti/driverlib/ADC12"];
                                            if (ADCMod){
                                                let ADCInsts = ADCMod.$instances
                                                for(let singleInst of ADCInsts){
                                                    if(singleInst.sampClkSrc === "DL_ADC12_CLOCK_SYSOSC"){
                                                        if(singleInst.peripheral.$solution){
                                                            inUse+= singleInst.peripheral.$solution.peripheralName+" ADCCLK, ";
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        if(inUse.length == 0){
                                            return "None"
                                        }

                                        return inUse;
                                    }
                                },
                                {
                                    name: "SYSOSC_Freq",
                                    displayName: "SYSOSC Frequency",
                                    default: 32000000,
                                    options: [
                                        { name: 32000000, displayName: "32 MHz" },
                                        { name: 4000000,  displayName: "4 MHz"  },
                                    ]
                                },
                                // DL_SYSCTL_enableSYSOSCFCL [!]
                                {
                                    name: "enableSYSOSCFCL",
                                    displayName: "Enable Frequency Correction Loop",
                                    description: "Used to increase SYSOSC accuracy.",
                                    default: false,
                                    hidden: false,
                                },
                                {
                                    name: "enableROSC",
                                    displayName: "Enable External Ref. Resistor ROSC",
                                    description: "External Reference Resistor used to increase SYSOSC accuracy",
                                    default: false,
                                    hidden: false,
                                },
                            ]
                        },
                        {
                            name: "SYSPLL_Config_Group",
                            displayName: "SYSPLL (System Phase-Locked Loop)",
                            longDescription:
`
The system phase locked loop (SYSPLL) takes an input reference clock SYSPLLREF and scales the input
frequency to produce user-specified high frequency clocks (SYSPLLCLK0, SYSPLLCLK1, and SYSPLLCLK2X)
for use by the device. Specifically, the SYSPLL clock outputs can be used as sources to MCLK and CANCLK.
![SYSPLL Diagram](../docs/english/sysconfig/images/syspllDiagram.png "SYSPLL Diagram")
`,
                            collapsed: true,
                            config: getSYSPLLConfig(),
                        },
                    ]
                },
                {
                    name: "EXT_OSC_Group",
                    displayName: "External Oscillators",
                    collapsed: true,
                    config: [
                        {
                            name: "HFXT_Config_Group",
                            displayName: "HFXT Configuration",
                            description: "",
                            longDescription: "",
                            collapsed: true,
                            config: getHFXTConfig(),
                        },
                        {
                            name: "LFXT_Config_Group",
                            displayName: "LFXT Configuration",
                            description: "",
                            longDescription: "",
                            collapsed: true,
                            config: getLFXTConfig(),
                        },
                    ]
                },
                {
                    name: "GROUP_CLK_Interrupts",
                    displayName: "Clock Interrupts",
                    description: "",
                    longDescription: "",
                    collapsed: false,
                    config: [
                        //DL_SYSCTL_enableInterrupt
                        {
                            name        : "clockInterrupts",
                            displayName : "Enable Interrupts",
                            description : '',
                            hidden      : false,
                            default     : [],
                            minSelections: 0,
                            options     : getClockInterrupts(),
                        },
                    ]
                },
                {
                    name: "Clock_Output_Group",
                    displayName: "Clock Output Frequencies",
                    collapsed: true,
                    config: getClkOutFreqConfig(),
                },
                {
                    name: "interruptGroup",
                    displayName: "Group Interrupts",
                    collapsed: true,
                    config: getInterruptGroupConfig(),
                }
            ]
        },
    ],

    /* override generic requirements with  device-specific reqs (if any) */
    pinmuxRequirements: pinmuxRequirements,

    filterHardware: filterHardware,

    //moduleInstances: moduleInstances,

    validate: validateSYSCTL,

    validatePinmux: validatePinmux,

    //_getPinResources: _getPinResources
};

function getClockInterrupts(inst){
    if(Common.isDeviceM0G()){
        return [
            // DL_SYSCTL_INTERRUPT_[...]
            {name: "LFOSC_GOOD", displayName: "Low Frequency Oscillator is stabilized and ready to use"},
            {name: "ANALOG_CLOCK_ERROR", displayName: "Analog clocking consistency error"},
            {name: "LFXT_GOOD", displayName: "Low Frequency Crystal is stabilized and ready to use"},
            {name: "HFCLK_GOOD", displayName: "High Frequency Clock is stabilized and ready to use"},
            {name: "SYSPLL_GOOD", displayName: "System PLL is stabilized and ready to use"},
            {name: "HSCLK_GOOD", displayName: "High Speed Clock is stabilized and ready to use"},
        ];
    }
    else if(Common.isDeviceFamily_PARENT_MSPM0L11XX_L13XX()){
        return [
            // DL_SYSCTL_INTERRUPT_[...]
            {name: "LFOSC_GOOD", displayName: "Low Frequency Oscillator is stabilized and ready to use"},
            {name: "ANALOG_CLOCK_ERROR", displayName: "Analog clocking consistency error"},
        ];
    }
    return [];
};

function getPowerSysInterrupts(inst){
    if(Common.isDeviceM0G()){
        return [
            // DL_SYSCTL_INTERRUPT_[...]
            {name: "FLASH_SEC", displayName: "Flash Single Error Correct"},
        ];
    }
    else if(Common.isDeviceM0L()){
        return [
            // DL_SYSCTL_INTERRUPT_[...]
        ];
    }
    return [];
};

function getFCCClkSrcs(inst){
    if(Common.isDeviceM0G()){
        return [
            {name: "MCLK",},
            {name: "SYSOSC",},
            {name: "HFCLK",},
            {name: "CLK_OUT"},
            {name: "SYSPLLCLK0",},
            {name: "SYSPLLCLK1",},
            {name: "SYSPLLCLK2X",},
            {name: "FCC_IN",},
        ];
    }
    else if(Common.isDeviceFamily_PARENT_MSPM0L11XX_L13XX()){
        return [
            {name: "MCLK",},
            {name: "SYSOSC",},
            {name: "CLK_OUT",},
            {name: "FCC_IN",},
        ];
    }
    return [];
}

/*
 *  ======== _getPinResources ========
 */
function _getPinResources(inst)
{


    return ("");


}

const memoizedGPIOFilter = _.memoize(curryGPIOFilter, (validPinSet) => JSON.stringify(validPinSet));

function curryGPIOFilter(validPinSet) {
	return (devicePin, peripheralPin) => GPIOFilter(devicePin, validPinSet);
}

function GPIOFilter(devicePin, validPinSet) {
    return validPinSet.includes(devicePin.name);
}

const maxFrequencyCLKOUTDir = {
    "SD" : [ 16, 32 ],
    "SDW" : [16, 32], // same as SD simply with wake
    "SDL" : [16, 32], // same as SD simply with low leakage
    "HS_DRVLOW" : [ 16, 32 ], // LOW Drive, used later on in pinmux validation
    "HS" : [ 24, 40 ], // High drive enabled
    "HD" : [ 16, 20 ],
    "OD" : [ 1, 1 ]
};

/* Filter valid pins for Fault Pin configuration */
function validClkOutPinSet(inst){
    let validPinSet = [];

    // determine the frequency based on the table:
    /*  Type        | short |  >1.71    |   >2.7
     *  SDIO        |  SD   |  16 MHz   |   32 MHz
     *  HSIO(DRV 0) |  HS   |  16 MHz   |   32 MHz
     *  HSIO(DRV 1) |  HS   |  24 MHz   |   40 MHz
     *  HDIO        |  HD   |  16 MHz   |   20 MHz
     *  ODIO        |  OD   |   1 MHz   |    1 MHz
     *
     */
    let clkOutFreq = inst.ClkOut_Freq;

    for(let pinIdx in system.deviceData.gpio.pinInfo){
        let eligible = true;

        let pinType = system.deviceData.gpio.pinInfo[pinIdx].devicePin.attributes.io_type;

        if(inst.ClkOutForceHighDrive && !["HD", "HS"].includes(pinType)){
            eligible = false;
        } else {
            // determine eligibility based on the frequency value
            let index = 0;
            if(inst.VDD >= 2.7){
                index = 1;
            }

            if(_.isUndefined(maxFrequencyCLKOUTDir[pinType])){
                throw "max frequency not found for pin type" + pinType;
            }
            // if pinType is HSIO;
            let maxFreq = maxFrequencyCLKOUTDir[pinType][index] * 1000000;

            if(clkOutFreq > maxFreq){
            // pin.devicePin.attributes.io_type
                eligible = false;
            }
        }

        if(eligible){
            validPinSet.push(pinIdx);
        }
    }

    return validPinSet;
}

/*  ======== pinmuxRequirements ========
 *  Returns peripheral pin requirements of the specified instance
 *
 *  param inst    - a fully configured sysctl instance
 *
 *  returns req[] - an array of pin requirements needed by inst
 */
function pinmuxRequirements(inst)
{
    let resources = [];
    if(Common.isDeviceM0G()){
        if(inst.LFCLKSource === "LFXT"){
            resources.push({
                name            : "lfxInPin",
                displayName     : "LFXIN",
                interfaceNames  : ["LFXIN"],
            },
            {
                name            : "lfxOutPin",
                displayName     : "LFXOUT",
                interfaceNames  : ["LFXOUT"],
            });
        }
        else if(inst.LFCLKSource === "LFCLK_IN"){
            resources.push({
                name            : "lfclkInPin",
                displayName     : "LFCLK In",
                interfaceNames  : ["LFXOUT"],
            });
        }
        if(inst.useHFCLK_Manual && inst.HFCLKSource === "HFXT"){
            resources.push({
                name            : "hfxInPin",
                displayName     : "HFXIN",
                interfaceNames  : ["HFXIN"],
            },
            {
                name            : "hfxOutPin",
                displayName     : "HFXOUT",
                interfaceNames  : ["HFXOUT"],
            });
        }
        else if(inst.useHFCLK_Manual && inst.HFCLKSource === "HFCLK_IN"){
            resources.push({
                name            : "hfclkInPin",
                displayName     : "HFCLK In",
                interfaceNames  : ["HFXOUT"],
            });
        }
    }
    if(inst.enableROSC){
        resources.push({
            name            : "roscPin",
            displayName     : "ROSC",
            interfaceNames  : ["ROSC"],
        });
    }
    if(inst.enableEXCLK){
        resources.push({
            name            : "clkOutPin",
            displayName     : "CLK OUT",
            interfaceNames  : ["CLK_OUT"],
        });
    }
    let sysctl = {
        name          : "peripheral",
        displayName   : "SYSCTL Peripheral",
        interfaceName : "SYSCTL",
        canShareWith  : "CLOCK_TREE",
        resources     : resources,
        signalTypes   : {
            lfxInPin    : ["LFXIN"],
            lfxOutPin   : ["LFXOUT"],
            lfclkInPin  : ["LFXOUT"],
            hfxInPin    : ["LFXIN"],
            hfxOutPin   : ["HFXOUT"],
            hfclkInPin  : ["HFXOUT"],
            roscPin     : ["ROSC"],
            clkOutPin   : ["CLK_OUT"],
        }
    };
    if(inst.clockTreeEn){
        return [];
    } else {
        return [sysctl];
    }
}

/*
 *  ========= filterHardware ========
 *  Check 'component' signals for compatibility with sysctl
 *
 *  param component - hardware object describing signals and
 *                    resources they're attached to
 *  returns Boolean indicating whether or not to allow the component to
 *           be assigned to an instance's $hardware config
 */
function filterHardware(component)
{
    return true;
}

/*
 *  ======== extend ========
 *  Extends a base exports object to include any device specifics
 *
 *  This function is invoked by the generic sysctl module to
 *  allow us to augment/override as needed for the MSP432.
 */
function extend(base)
{
    // Special static extend

    /* merge and overwrite base module attributes */
    let result = Object.assign({}, base.moduleStatic, devSpecific);

    /* concatenate device-specific configs */
    // moduleStatic specific to SYSCTL as it's statically defined
    result.config = base.moduleStatic.config.concat(devSpecific.config);
    base.moduleStatic = result;

    return (base);
}

/*
 *  ======== exports ========
 *  Export device-specific extensions to base exports
 */
exports = {
    /* required function, called by base sysctl module */
    extend: extend
};
