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
 *  ======== moduleTimerMSP432.js ========
 */

"use strict";

/* get Common /ti/driverlib utility functions */
let Common = system.getScript("/ti/driverlib/Common.js");
let deviceOptions = system.getScript("/ti/driverlib/nonmain/NONMAINOptions.js");

/*
 * Get sfc32 PRNG function for A2 and A3_CAN random PW generation.
 * TODO: Remove after SysConfig 1.23, where node path issue with crypto is
 * fixed
 */
let customPRNG = system.getScript("./sfc32.js");

/*
 * Keep this array to prevent compatibility breaks/errors from being generated
 * due to debug disable workaround
 */
let enableOptions = [
    {name: "enabled", displayName: "Enabled"},
    {name: "enabledWithPW", displayName: "Enabled with password match"},
    {name: "disabled", displayName: "Disabled"},
];

let warningNoteM0C = "";
if(Common.isDeviceFamily_PARENT_MSPM0C110X()){
    warningNoteM0C= `
**Note: This feature is not available on the following devices: MSPM0C1103, MSPM0C1104, MSPS003Fx.**
    `;
}

let sha256Note = "";
if (deviceOptions.SUPPORT_PW_HASH == true)
{
    sha256Note = `
**Note**: Password fields for this device **require** the user to provide the SHA256
hash of the 128-bit password. Refer to the following steps on correctly converting
between 128-bit plaintext password and hash:

1. Determine the 128-bit password to use. These steps will use the following
example password:

0x12345678\n
0xCAFECAFE\n
0xDEADBEEF\n
0xCAFEBABE\n

2. Reverse the password endianness. Combine it into one string.
0x78563412\n
0xFECAFECA\n
0xEFBEADDE\n
0xBEBAFECA\n

Combined String: 78563412FECAFECAEFBEADDEBEBAFECA

3. Calculate the SHA256 of the string. Break the output into 8 32-bit words.
The user can use online tools like the one provided here:
[SHA256 online tool](https://emn178.github.io/online-tools/sha256.html)

Output String: 2738a5682ad52550177227468dd2b55f552c34bd25560b9067b0a3f3d5c648f9

Result:\n
0x2738a568\n
0x2ad52550\n
0x17722746\n
0x8dd2b55f\n
0x552c34bd\n
0x25560b90\n
0x67b0a3f3\n
0xd5c648f9\n

4. Reverse endianness of the 32-bit words.

Original   -> Converted\n
0x2738a568 -> 0x68A53827\n
0x2ad52550 -> 0x5025D52A\n
0x17722746 -> 0x46277217\n
0x8dd2b55f -> 0x5FB5D28D\n
0x552c34bd -> 0xBD342C55\n
0x25560b90 -> 0x900B5625\n
0x67b0a3f3 -> 0xF3A3B067\n
0xd5c648f9 -> 0xF948C6D5\n

5. Enter the converted password into the BCR (boot configuration routine) structure
for the desired function e.g. mass erase, factory reset, or debug lock.
`;
}

function getAPEnableOptions(inst)
{
    let apEnableOptions = [
        {name: "enabled", displayName: "Enabled"},
        {name: "enabledWithPW", displayName: "Enabled with password match"},
    ];

    if (deviceOptions.SUPPORT_PW_HASH != false)
    {
        apEnableOptions.push({name: "disabled", displayName: "Disabled"});
    }

    return apEnableOptions;
}

function getDisabledEnableOptions(inst)
{
    /* Password authentication for any SWD commands isn't supported on M0C */
    let disabledOption = [
        {name: "enabledWithPW",
         displayName: "Enabled with password match",
         reason: "Not available on M0C1103/M0C1104 devices"}
    ];

    if (Common.isDeviceFamily_PARENT_MSPM0C110X())
    {
        return disabledOption;
    }
    else
    {
        return [];
    }
}

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

/*
 *  ======== validate ========
 *  Validate this inst's configuration
 *
 *  param inst       - module instance to be validated
 *  param validation - object to hold detected validation issues
 */
function validateNONMAIN(inst, validation)
{
    /*
     * User must acknowledge NONMAIN configuration risks before proceeding with
     * configuration and programming
     */
    if (!inst.nonmainWarning)
    {
        validation.logError(
            "User must accept Configuration NVM (NONMAIN) configuration risks",
            inst, "nonmainWarning"
        );

        validation.logInfo(
            "Using the Configuration NVM (NONMAIN) has certain risks. Please " +
            "refer to the configuration risks fields's long description by " +
            "clicking the question mark next to the field for more details " +
            "on the risks", inst
        );
    }

    /* Validate BCR region */
    validateBCR(inst, validation);

    /* Some devices don't support BSL */
    if (deviceOptions.SUPPORT_BSL == true)
    {
        validateBSL(inst, validation);
    }
}

function validateBCR(inst, validation)
{
    /* Debug security port validation */
    if (!inst.swdEnable)
    {
        validation.logWarning(
            "When the physical debug port (SW-DP) is disabled, all of the " +
            "SWD-accessible functions (application debug, mass erase, factory " +
            "reset, and TI failure analysis) are not accessible through SWD, " +
            "regardless of their individual configuration.",
            inst, "swdEnable"
        );
    }

    /* MAIN flash static write protection validation */
    if (inst.staticWriteProtectionMainLow || inst.staticWriteProtectionMainHigh)
    {
        validation.logInfo(
            "Mass erase and factory reset commands sent to the BCR via the " +
            "DSSM will override the specified static write protection policy. " +
            "If this behavior is not desired, configure the mass erase and " +
            "factory reset commands to be enabled with password or disabled. " +
            "Note that mass erase and factory reset commands sent to the BSL " +
            "will respect the static write protection policy as the BSL has the " +
            "same permissions as application code.",
            inst, ["staticWriteProtectionMainLow", "staticWriteProtectionMainHigh"]
        );
    }
    if (deviceOptions.BCR_SUPPORT_DUAL_BANK == true)
    {
        if (inst.staticWriteProtectionMainHigh_2)
        {
            validation.logInfo(
                "Mass erase and factory reset commands sent to the BCR via the " +
                "DSSM will override the specified static write protection policy. " +
                "If this behavior is not desired, configure the mass erase and " +
                "factory reset commands to be enabled with password or disabled. " +
                "Note that mass erase and factory reset commands sent to the BSL " +
                "will respect the static write protection policy as the BSL has the " +
                "same permissions as application code.",
                inst, ["staticWriteProtectionMainHigh_2"]
            );
        }
    }

    /* NONMAIN flash static write protection validation */
    if (inst.staticWriteProtectionNonMain)
    {
        validation.logWarning(
            "This locks the configuration permanently unless BCR factory reset " +
            "is enabled with or without password.",
            inst, "staticWriteProtectionNonMain"
        );
    }

    /* Fast boot mode and CRC check validation */
    if (inst.fastBootMode)
    {
        validation.logWarning(
            "If enabled, the application CRC check will be bypassed even if " +
            "the CRC check is enabled.",
            inst, "fastBootMode"
        );
    }

    /* C1104_C1103-specific BCR validation */
    if (Common.isDeviceFamily_PARENT_MSPM0C110X())
    {
        if (inst.debugAccessEnable == "enabledWithPW")
        {
            validation.logError(
                warningNoteM0C + " Please reselect.",
                inst, "debugAccessEnable"
            );
        }

        if (inst.frMode == "enabledWithPW")
        {
            validation.logError(
                warningNoteM0C + " Please reselect.",
                inst, "frMode"
            );
        }
    }

    /* Check that user provides SHA-2 256 hash of password */
    if (deviceOptions.SUPPORT_PW_HASH == true)
    {
        validation.logInfo("For password fields in this device, user should " +
            "provide SHA256 hash of 128-bit password. See field descriptions " +
            "for more details.", inst)
    }
}

function validateBSL(inst, validation)
{
    /*
     * Devices that don't support ROM BSL should only need to validate the
     * custom BSL and BSL Invoke Pin configurations
     */
    if (deviceOptions.SUPPORT_ROM_BSL == true)
    {
        /*
         * Throw meaningful error message when no default BSL UART or I2C pins
         * are available for the package
         */
        if (inst.uartRXPin == "")
        {
            validation.logError(
                "Package does not have default BSL UART RX pin. Select a valid pin option.",
                inst, "uartRXPin"
            );
        }

        if (inst.uartTXPin == "")
        {
            validation.logError(
                "Package does not have default BSL UART TX pin. Select a valid pin option.",
                inst, "uartTXPin"
            );
        }

        if (inst.i2cSCLPin == "")
        {
            validation.logError(
                "Package does not have default BSL I2C SCL pin. Select a valid pin option.",
                inst, "i2cSCLPin"
            );
        }

        if (inst.i2cSDAPin == "")
        {
            validation.logError(
                "Package does not have default BSL I2C SDA pin. Select a valid pin option.",
                inst, "i2cSDAPin"
            );
        }

        /* Validate dynamic enums to pass sanity tests */
        let dynamicEnums = ["uartRXPin", "uartTXPin", "i2cSCLPin", "i2cSDAPin"];

        for(let dE of dynamicEnums)
        {
            let validOptions = inst.$module.$configByName[dE].options(inst);
            let selectedOption = inst[dE];
            let found = _.find(validOptions, (o) => o.name === selectedOption);

            if(!found)
            {
                validation.logError("Invalid Option, please Reselect", inst, dE);
            }
        }
    }

    if (deviceOptions.BSL_SUPPORT_FLASH_PLUGIN)
    {
        /* Check that function pointer names are valid in C */
        if (inst.bslPluginHookInit.length > 0)
        {
            if (!Common.isCName(inst.bslPluginHookInit))
            {
                validation.logError(
                    "Function name must be a valid C identifier.",
                    inst, "bslPluginHookInit"
                );
            }
        }

        if (inst.bslPluginHookInitAddress != 0xFFFFFFFF)
        {
            validation.logInfo(
                "Make sure that the plugin address matches the linker file.",
                inst, "bslPluginHookInitAddress"
            );
        }

        if (inst.bslPluginHookReceive.length > 0)
        {
            if (!Common.isCName(inst.bslPluginHookReceive))
            {
                validation.logError(
                    "Function name must be a valid C identifier.",
                    inst, "bslPluginHookReceive"
                );
            }
        }

        if (inst.bslPluginHookReceiveAddress != 0xFFFFFFFF)
        {
            validation.logInfo(
                "Make sure that the plugin address matches the linker file.",
                inst, "bslPluginHookReceiveAddress"
            );
        }

        if (inst.bslPluginHookTransmit.length > 0)
        {
            if (!Common.isCName(inst.bslPluginHookTransmit))
            {
                validation.logError(
                    "Function name must be a valid C identifier.",
                    inst, "bslPluginHookTransmit"
                );
            }
        }

        if (inst.bslPluginHookTransmitAddress != 0xFFFFFFFF)
        {
            validation.logInfo(
                "Make sure that the plugin address matches the linker file.",
                inst, "bslPluginHookTransmitAddress"
            );
        }

        if (inst.bslPluginHookDeInit.length > 0)
        {
            if (!Common.isCName(inst.bslPluginHookDeInit))
            {
                validation.logError(
                    "Function name must be a valid C identifier.",
                    inst, "bslPluginHookDeInit"
                );
            }
        }

        if (inst.bslPluginHookDeInitAddress != 0xFFFFFFFF)
        {
            validation.logInfo(
                "Make sure that the plugin address matches the linker file.",
                inst, "bslPluginHookDeInitAddress"
            );
        }
    }

    /* BSL alternate address validation */
    if (inst.bslAltConfig)
    {
        let MAIN_CODE_ADDRESS_SPACE_END = 0x40000000;

        if (inst.bslAltAddress >= MAIN_CODE_ADDRESS_SPACE_END)
        {
            validation.logWarning(
                "The alternate BSL address must be located in the MAIN flash region.",
                inst, "bslAltAddress"
            );
        }

        if (!(inst.bslAltAddress & 1))
        {
            validation.logWarning(
                "The LSB of the alternate BSL address should be set since " +
                "Cortex-M type ARM processors only support thumb mode.",
                inst, "bslAltAddress"
            );
        }
    }
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
    return ([]);
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
    return (true);
}

/*
 * Create password configs for SWD access, SWD commands (factory reset,
 * mass erase) - 128-bit passwords, and BSL access (256-bit).
 */
function createPWConfig(pwType, hiddenStatus, numPWRegs)
{
    let pwConfig = [];
    let pwMap = {
        "swdPW": "SWD Password",
        "frPW": "Factory Reset",
        "mePW": "Mass Erase",
        "bslPW": "BSL Access",
    };

    let pwDefaults = [0x761396AF,0x5F63720F,0x5A4AB4BD,0x9FC3630A,0xF930AF12,0x5CEEA650,0x88E11B97,0x51409CE8]
    for (let idx = 0; idx < numPWRegs; idx++)
    {
        /* BSL PW has device-specific configuration involving hash */
        if ((pwType == "bslPW") && (deviceOptions.SUPPORT_PW_HASH == true)){
            pwConfig.push(
                {
                    name        : pwType + idx,
                    displayName : pwMap[pwType] + "[" + idx + "]",
                    description : "",
                    longDescription: ``,
                    hidden      : hiddenStatus,
                    displayFormat: "hex",
                    default     : pwDefaults[idx],
                    range       : [0, 0xFFFFFFFF]
                },
            )
        }
        else{
            pwConfig.push(
                {
                    name        : pwType + idx,
                    displayName : pwMap[pwType] + "[" + idx + "]",
                    description : "",
                    longDescription: ``,
                    hidden      : hiddenStatus,
                    displayFormat: "hex",
                    default     : 0xFFFFFFFF,
                    range       : [0, 0xFFFFFFFF]
                },
            )
        }
    }

    return (pwConfig);
}

function onChangeAPEnable(inst, ui)
{
    onChangeSetCustomProfile(inst, ui);
    updateGUIAPEnable(inst, ui);
    updateGUIFactoryReset(inst, ui);

    /* C1104_C1103 doesn't support mass erase */
    if (!Common.isDeviceFamily_PARENT_MSPM0C110X())
    {
        updateGUIMassErase(inst, ui);
    }
}

function updateGUIAPEnable(inst, ui)
{
    if (inst.debugAccessEnable == "enabledWithPW")
    {
        system.utils.showGroupConfig("GROUP_SWD_PW", inst, ui);
    }
    else
    {
        system.utils.hideGroupConfig("GROUP_SWD_PW", inst, ui);
    }

    updateGUISWDPassword(inst, ui);
}

function onChangeSWDEnable(inst, ui)
{
    onChangeSetCustomProfile(inst, ui);
}

function onChangeTIFAEnable(inst, ui)
{
    onChangeSetCustomProfile(inst, ui);
}

function generateRandom128BitPW()
{
    /*
     * Seed is system timestamp XOR'd with rand value ranging [0, 2^32 - 1].
     * The right shift by 0 produces integer value
     */
    let seed = Date.now() ^ ((Math.random() * 2**32) >>> 0);
    /* Feed in sfc32. Pad seed value with Phi, Pi, and E */
    var randFunc = customPRNG.sfc32(0x9E3779B9, 0x243F6A88, 0xB7E15162, seed);
    /* Run generator 20 times to mix initial state thoroughly */
    for (var i = 0; i < 20; i++)
    {
        randFunc();
    }

    return [randFunc(), randFunc(), randFunc(), randFunc()];
}

function updateGUISWDPassword(inst, ui)
{
    /*
     * Reset password field whenever the debug access option changes. This is
     * especially important for the Debug Access Disable workaround, where we
     * want to minimize customer impact/questions
     */
    inst.swdPW0 = 0xFFFFFFFF;
    inst.swdPW1 = 0xFFFFFFFF;
    inst.swdPW2 = 0xFFFFFFFF;
    inst.swdPW3 = 0xFFFFFFFF;
    if (deviceOptions.SUPPORT_PW_HASH == true)
    {
        inst.swdPW4 = 0xFFFFFFFF;
        inst.swdPW5 = 0xFFFFFFFF;
        inst.swdPW6 = 0xFFFFFFFF;
        inst.swdPW7 = 0xFFFFFFFF;
    }
}

function updateGUIFactoryReset(inst, ui)
{
    if (inst.frMode == "enabledWithPW")
    {
        system.utils.showGroupConfig("GROUP_FACTORY_RESET_PW", inst, ui);
    }
    else
    {
        system.utils.hideGroupConfig("GROUP_FACTORY_RESET_PW", inst, ui);
    }

    updateGUIFRPassword(inst, ui);
}

function updateGUIFRPassword(inst, ui)
{
    if (inst.frMode != "enabledWithPW")
    {
        /* Reset password field */
        inst.frPW0 = 0xFFFFFFFF;
        inst.frPW1 = 0xFFFFFFFF;
        inst.frPW2 = 0xFFFFFFFF;
        inst.frPW3 = 0xFFFFFFFF;
        if (deviceOptions.SUPPORT_PW_HASH == true)
        {
            inst.frPW4 = 0xFFFFFFFF;
            inst.frPW5 = 0xFFFFFFFF;
            inst.frPW6 = 0xFFFFFFFF;
            inst.frPW7 = 0xFFFFFFFF;
        }
    }
}

function updateGUIMassErase(inst, ui)
{
    if (inst.meMode == "enabledWithPW" && !(Common.isDeviceFamily_PARENT_MSPM0C110X()))
    {
        system.utils.showGroupConfig("GROUP_MASS_ERASE_PW", inst, ui);
    }
    else
    {
        system.utils.hideGroupConfig("GROUP_MASS_ERASE_PW", inst, ui);
    }

    updateGUIMEPassword(inst, ui);
}

function updateGUIMEPassword(inst, ui)
{
    if (inst.meMode != "enabledWithPW")
    {
        /* Reset password field */
        inst.mePW0 = 0xFFFFFFFF;
        inst.mePW1 = 0xFFFFFFFF;
        inst.mePW2 = 0xFFFFFFFF;
        inst.mePW3 = 0xFFFFFFFF;
        if (deviceOptions.SUPPORT_PW_HASH == true)
        {
            inst.mePW4 = 0xFFFFFFFF;
            inst.mePW5 = 0xFFFFFFFF;
            inst.mePW6 = 0xFFFFFFFF;
            inst.mePW7 = 0xFFFFFFFF;
        }
    }
}

function onChangeFactoryReset(inst, ui)
{
    onChangeSetCustomProfile(inst, ui);
    updateGUIFactoryReset(inst, ui);
}

function onChangeMassErase(inst, ui)
{
    onChangeSetCustomProfile(inst, ui);
    updateGUIMassErase(inst, ui);
}

function onChangeBSLEnable(inst, ui)
{
    updateGUIBSLEnable(inst, ui);
    updateGUIGPIOInvoke(inst, ui);
    if (deviceOptions.BSL_SUPPORT_FLASH_PLUGIN)
    {
        updateGUIPluginConfig(inst, ui);
    }
    updateGUIAltBSLConfig(inst, ui);
}

function updateGUIBSLEnable(inst, ui)
{
    if (inst.bslMode)
    {
        system.utils.showGroupConfig("GROUP_BSL", inst, ui);
    }
    else
    {
        system.utils.hideGroupConfig("GROUP_BSL", inst, ui);
    }

    /* Reset fields to default */
    /* Enable/disable ROM BSL not tied to invoke pin functionality */
    inst.bslInvokePinCheck = true;
    inst.bslDefaultInvokePin = true;

    ui.bslInvokePinData0.hidden = true;
    ui.bslInvokePinData1.hidden = true;

    if (deviceOptions.SUPPORT_ROM_BSL == true)
    {
        inst.uartTXPin = defaultBSLUARTTX;
        inst.uartRXPin = defaultBSLUARTRX;
        if (deviceOptions.BSL_SUPPORT_UART_BAUD == true)
        {
            inst.uartBaudDefault = "FFFF";
        }

        inst.i2cSCLPin = defaultBSLI2CSCL;
        inst.i2cSDAPin = defaultBSLI2CSDA;
        inst.i2cSlaveAddress = 0x0048;
        inst.bslAppVersion = 0xFFFFFFFF;
        inst.bslEnableReadOut = false;
        inst.bslSecurityConfig = "ignoreAlert";
    }

    if (deviceOptions.BSL_SUPPORT_FLASH_PLUGIN)
    {
        inst.bslFlashPluginEnable = false;
        inst.bslPluginSRAMSize = 0xFF;
    }

    if (deviceOptions.BSL_DISABLE_NRST == true)
    {
        inst.disableNRST = false;
    }

    inst.bslAltConfig = false;
    inst.bslAltAddress = 0xFFFFFFFF;
    inst.bslConfigID = deviceOptions.BSL_CONFIG_ID;

    updateGUIBSLPassword(inst, ui);
}

function updateGUIBSLPassword(inst, ui)
{
    if (!inst.bslMode)
    {
        /* Reset password */
        if (deviceOptions.SUPPORT_ROM_BSL == true)
        {
            if (deviceOptions.SUPPORT_PW_HASH == true)
            {
                inst.bslPW0 = 0x761396AF;
                inst.bslPW1 = 0x5F63720F;
                inst.bslPW2 = 0x5A4AB4BD;
                inst.bslPW3 = 0x9FC3630A;
                inst.bslPW4 = 0xF930AF12;
                inst.bslPW5 = 0x5CEEA650;
                inst.bslPW6 = 0x88E11B97;
                inst.bslPW7 = 0x51409CE8;
            }
            else
            {
                inst.bslPW0 = 0xFFFFFFFF;
                inst.bslPW1 = 0xFFFFFFFF;
                inst.bslPW2 = 0xFFFFFFFF;
                inst.bslPW3 = 0xFFFFFFFF;
                inst.bslPW4 = 0xFFFFFFFF;
                inst.bslPW5 = 0xFFFFFFFF;
                inst.bslPW6 = 0xFFFFFFFF;
                inst.bslPW7 = 0xFFFFFFFF;
            }
        }
    }
}

function updateGUIGPIOInvoke(inst, ui)
{
    updateGUIGPIOInvokePinCheck(inst, ui);
    updateGUIGPIODefaultInvokePin(inst, ui);
}

function updateGUIGPIOInvokePinCheck(inst, ui)
{
    ui.bslDefaultInvokePin.hidden = !(inst.bslInvokePinCheck);

    if (!inst.bslInvokePinCheck)
    {
        /* If the invoke pin check is disabled, reset values to default */
        inst.bslDefaultInvokePin = true;
        inst.bslInvokePin = filteredPinOptions[0].name;
        inst.bslInvokePinLevel = 0;
    }
}

function updateGUIGPIODefaultInvokePin(inst, ui)
{
    if (inst.bslInvokePinCheck && !inst.bslDefaultInvokePin)
    {
        ui.bslInvokePin.hidden = false;
        ui.bslInvokePINCM.hidden = false;
        ui.bslInvokePinLevel.hidden = false;
    }
    else
    {
        ui.bslInvokePin.hidden = true;
        ui.bslInvokePINCM.hidden = true;
        ui.bslInvokePinLevel.hidden = true;

        /* Reset GPIO pin details to default */
        inst.bslInvokePin = filteredPinOptions[0].name;
        inst.bslInvokePinLevel = 0;
    }
}

function updateGUIPluginConfig(inst, ui)
{
    ui.bslPluginType.hidden = !(inst.bslFlashPluginEnable);
    ui.bslPluginSRAMSize.hidden = !(inst.bslFlashPluginEnable);
    ui.bslPluginHookInit.hidden = !(inst.bslFlashPluginEnable);
    ui.bslPluginHookReceive.hidden = !(inst.bslFlashPluginEnable);
    ui.bslPluginHookTransmit.hidden = !(inst.bslFlashPluginEnable);
    ui.bslPluginHookDeInit.hidden = !(inst.bslFlashPluginEnable);

    updateGUIPluginHooks(inst, ui);
}

function updateGUIPluginHooks(inst, ui)
{
    if (!inst.bslFlashPluginEnable)
    {
        inst.bslPluginType = "bslPluginAny";
        inst.bslPluginSRAMSize = 0xFF;
        inst.bslPluginHookInit = "";
        inst.bslPluginHookInitAddress = 0xFFFFFFFF;
        inst.bslPluginHookReceive = "";
        inst.bslPluginHookReceiveAddress = 0xFFFFFFFF;
        inst.bslPluginHookTransmit = "";
        inst.bslPluginHookTransmitAddress = 0xFFFFFFFF;
        inst.bslPluginHookDeInit = "";
        inst.bslPluginHookDeInitAddress = 0xFFFFFFFF;
        ui.bslPluginHookInitAddress.hidden = true;
        ui.bslPluginHookReceiveAddress.hidden = true;
        ui.bslPluginHookTransmitAddress.hidden = true;
        ui.bslPluginHookDeInitAddress.hidden = true;
    }
}

function updateGUIAltBSLConfig(inst, ui)
{
    ui.bslAltAddress.hidden = !(inst.bslAltConfig);

    if (!inst.bslAltConfig)
    {
        inst.bslAltAddress = 0xFFFFFFFF;
    }
}

function onChangePluginConfig(inst, ui)
{
    updateGUIPluginConfig(inst, ui);
}

function onChangeAltBSLConfig(inst, ui)
{
    updateGUIAltBSLConfig(inst, ui);
}

/************************* Profiles functions *******************************/
let securityLevel0Profile = {
    name : "LEVEL_0",
    debugAccessEnable: "enabled",
    swdEnable: true,
    frMode: "enabled",
};

if (!Common.isDeviceFamily_PARENT_MSPM0C110X())
{
    securityLevel0Profile = {...securityLevel0Profile, ...{tifaEnable: true}, ...{meMode: "enabled"}}
}

let securityLevel2Profile = {
    name      : "LEVEL_2",
    swdEnable : false,
    /* Other fields are don't care */
};

const profilesDebugSecurity = [
    securityLevel0Profile,
    securityLevel2Profile,
];

function onChangeNONMAINProfile(inst, ui)
{
    if (inst.profile != "CUSTOM")
    {
        let selectedProfileArray = profilesDebugSecurity.filter(obj =>
                                        {return obj.name === inst.profile});
        if (selectedProfileArray.length != 0)
        {
            const selectedProfile = Object.assign({}, selectedProfileArray[0]);
            delete selectedProfile.name;
            Object.assign(inst, selectedProfile)
        }
    }
    else
    {
        inst.swdEnable = true;
    }
}

function onChangeSetCustomProfile(inst,ui)
{
    if (inst.swdEnable)
    {
        inst.profile = "CUSTOM";
    }
    else
    {
        /* If swdEnable is set to false, that is security level 2 */
        inst.profile = "LEVEL_2";
    }
}

/* Device-Specific Configurables */
/* BCR Configurables */
let extendedConfigBCR = [];
if (deviceOptions.BCR_SUPPORT_CSC == true) {
    extendedConfigBCR = [
        {
            // .CSCexist
            name: "cscExists",
            displayName: "Enable CSC Policy",
            description: "Enable / disable the CSC policy in SYSCTL",
            default: false,
        },
    ]

    if (deviceOptions.BCR_SUPPORT_BANK_SWAP == true)
    {
        extendedConfigBCR = extendedConfigBCR.concat([
            {
                // .flashBankSwapPolicy
                name: "flashBankSwap",
                displayName: "Enable Flash Bank Swap Policy",
                description: "Enable / Disable the Flash Bank Swap Policy in SYSCTL",
                default: false,
            }
        ]);
    }
};
let extendedConfigBCRDebug = [];
if (deviceOptions.BCR_SUPPORT_CSC == true) {
    extendedConfigBCRDebug = [
        {
            // .debugHold
            name: "debugHold",
            displayName: "Debug Hold",
            description: "Controls the release of debug access until INITDONE is issued.",
            default: false,
        },
    ];
};
let extendedConfigBCRCRC = [];
if (deviceOptions.BCR_SUPPORT_USER_APP_HASH == true){
    extendedConfigBCRCRC = [
        {
            // NOTE: this feature is currently disabled/hidden
            // .userSecureAppHash
            // TODO: this will most likely require separate fields
            //          for each of the required values depending on
            //          the chosen addresses/size. Leaving as
            //          placeholder for now.
            // TODO: this would be replaced by a GROUP of configurables similar
            //          to the ones used for password, can create/repurpose
            //          similar function. However, this is tied to re-enabling
            //          the user app checking feature, so this is less priority.
            name        : "appCRCHash",
            displayName : "Application CRC Check Application Hash",
            description : "",
            longDescription: ``,
            hidden      : true,
            displayFormat: "hex",
            default     : 0xFFFFFFFF,
            range       : [0, 0xFFFFFFFF]
        },
    ];
};

let bslExtendedConfigUART = [];
if (deviceOptions.BSL_SUPPORT_UART_BAUD == true){
    bslExtendedConfigUART = [
        {
            name: "uartBaudDefault",
            displayName: "Default UART Baud Rate",
            default: "FFFF",
            options: [
                { name: "0001", displayName: "4800"},
                { name: "FFFF", displayName: "9600"},
                { name: "0003", displayName: "19200"},
                { name: "0004", displayName: "38400"},
                { name: "0005", displayName: "576000"},
                { name: "0006", displayName: "115200"},
                { name: "0007", displayName: "1000000"},
                { name: "0008", displayName: "2000000"},
                { name: "0009", displayName: "3000000"},
            ]
        }
    ]
}

let bslExtendedConfigNRST = [];
if (deviceOptions.BSL_DISABLE_NRST == true){
    bslExtendedConfigNRST = [
        {
            name: "disableNRST",
            displayName: "Disable NRST",
            longDescription: `Disable NRST during Bootloader execution.`,
            default: false,
        }
    ]
}

let extendedConfigWriteProtection = [];
if(deviceOptions.BCR_SUPPORT_DUAL_BANK == true){
    extendedConfigWriteProtection = [
        {
            name: "GROUP_FLASH_STATIC_WRITE_PROTECTION_CONFIG_2",
            displayName: "Flash Memory Static Write Protection (SWP) Configuration (Bank 1)",
            description: "",
            config: [
                {
                    // .staticWriteProtectionMainHigh 2
                    /* FLASHSWP1 */
                    name        : "staticWriteProtectionMainHigh_2",
                    displayName : "MAIN SWP (Upper Banks)",
                    description : "Sets the static write protection of the remaining sectors",
                    longDescription: `
One bit corresponds to eight sectors. Setting a bit to 0 enables write
protection, and setting a bit to 1 disables write protection. Bits 0-3 are ignored.

This is used to protect Flash memory from (256kb to 512kb)

A value of 0xFFFFFFEx (bit 4 cleared) would make all sectors writable,
except for sector 39-32 (0x0000_8000-0x0000_9FFF).

The value of bits corresponding to unimplemented sectors is ignored.

Refer to the TRM and the device datasheet for more information.
`,
                    hidden      : false,
                    displayFormat: "hex",
                    default     : 0xFFFFFFFF,
                    range       : [0, 0xFFFFFFFF]
                },
            ],
        }
    ];
}

/* Display Flash Bank Numbering */
let flashBank0Display = "";
if(deviceOptions.BCR_SUPPORT_DUAL_BANK == true){
    flashBank0Display = " (Bank 0)";
}

/* Password Configurables: Device Specific */
const passwordWordLen = ((deviceOptions.SUPPORT_PW_HASH == true) ? 8:4);
let mePWConfig = createPWConfig("mePW", true, passwordWordLen);
let frPWConfig = createPWConfig("frPW", true, passwordWordLen);
let swdPWConfig = createPWConfig("swdPW", true, passwordWordLen);

let nonMainConfig = [
    /****** PROFILES CONFIGURATION *******/
    {
        name: "GROUP_PROFILE",
        displayName: "Quick Profiles",
        description: "",
        collapsed: false,
        config: [
            {
                name: "nonmainWarning",
                displayName: "Accept configuration risks",
                description: "Acknowledge Configuration NVM (NONMAIN) configuration risks",
                longDescription: `
For security purposes, MSPM0 devices require a valid Configuration NVM (NONMAIN)
at all times. When updating Configuration NVM, the old Configuration NVM
configuration is erased, and the new configuration is programmed. Any
interruption during the erase or re-program operations i.e. unplugging device,
removing SWD jumpers, hitting reset by accident, cancelling the code download,
an IDE crash, etc. can brick the device permanently. Improper configuration of
Configuration NVM can also lead to permanent locking of the device.

To minimize these risks, TI **highly recommends** adding and configuring
this module only when one is ready to go to production. Click the checkbox to
acknowledge the risks when using Configuration NVM, and refer to the relevant
IDE guides in the Tools documentation directory in the SDK on how to properly
program Configuration NVM.
`,
                default: false,
            },
            {
                name        : "profile",
                displayName : "Debug Security Profiles",
                description : 'Pre-defined profiles for debug security configurations',
                longDescription: `
To accommodate a variety of needs while keeping the configuration process
simple, MSPM0 devices support three generic security levels: no restrictions (level 0),
custom restrictions (level 1), and fully restricted (level 2).

Refer to the Boot Configuration Routine (BCR) Security Policies section of the
[Cybersecurity Enablers in MSPM0 MCUs app note](https://www.ti.com/lit/slaae29)
for more details on the security levels.

The Quick Profile Options are:
* **Security Level 0**:
    * Physical debug port/SW-DP: enabled
    * Application debug: enabled
    * Mass erase policy: enabled
    * Factory reset policy: enabled
    * TI failure analysis policy: enabled
* **Security Level 2**:
    * Physical debug port/SW-DP: disabled
    * Application debug: don't care, access not possible through SWD
    * Mass erase policy: don't care from SWD, but can be done through BSL if
      BSL is enabled
    * Factory reset policy: don't care from SWD, but can be done through BSL if
      BSL is enabled
    * TI failure analysis policy: don't care, access not possible through SWD
* **Security Level 1 (Custom)**:
    * Allows custom configuration.`,
                hidden      : false,
                default     : "LEVEL_0",
                options     : [
                    {name: "LEVEL_0", displayName: "Security Level 0 - No restrictions"},
                    {name: "LEVEL_2", displayName: "Security Level 2 - Fully restrictive"},
                    {name: "CUSTOM", displayName: "Security Level 1 - Custom restrictions"},
                ],
                onChange    : onChangeNONMAINProfile,
            },
        ],
    },
    /****** BCR CONFIGURATION *******/
    {
        name: "GROUP_BCR",
        displayName: "Boot Configuration Routine (BCR) Configuration",
        description: "",
        longDescription: `
The BCR is the first firmware to run on the device after a BOOTRST. The BCR manages the
following at boot time:

* Configuring the debug interface security policy
* Configuring the flash memory static write protection policy
* Optionally:
    * Executing a mass erase
    * Executing a factory reset
    * Verifying the integrity of some/all of the application firmware with
      32-bit CRC
    * Enabling fast boot mode
    * Starting the BSL
`,
        collapsed: true,
        config: [
            {
                name: "GROUP_DEBUG_SECURITY_POLICY_CONFIG",
                displayName: "Debug Security Policy Configuration",
                description: "",
                collapsed: true,
                config: [
                    // .swdpMode
                    {
                        name        : "swdEnable",
                        displayName : "Enable Physical Debug Port (SW-DP)",
                        description : "Selects if the physical debug port (SW-DP) is enabled or completely disabled.",
                        longDescription: `
When this field is disabled, the physical debug port (SW-DP) is completely
disabled, and **all** of the SWD-accessible functions (application debug, mass
erase, factory reset, and TI failure analysis) are not accessible through SWD,
regardless of their individual configuration.

**Note**: If the BSL is enabled, the mass erase and factory reset configuration
fields are still used by the BSL to authorize mass erase or factory reset
commands originating from the BSL interface.

This is the **fully restrictive** state or **SWD Security Level 2**.
Level 2 should be used for mass production when no further access to any SWD
functions is required and a fully secure state is desired for the device.

Refer to the Boot Configuration Routine (BCR) Security Policies section of the
[Cybersecurity Enablers in MSPM0 MCUs app note](https://www.ti.com/lit/slaae29)
for more details on the security levels.
`,
                        hidden      : false,
                        default     : true,
                        onChange    : onChangeSWDEnable,
                    },
                    // .debugAccess
                    {
                        name        : "apEnable",
                        displayName : "Enable Application Debug Access",
                        description : "Selects if application debug access is enabled, disabled, or enabled with password.",
                        longDescription: `
Application debug access includes:

* Full access to the processor, memory map, and peripherals through the AHB-AP
* Access to the device EnergyTrace+ state information through the ET-AP
* Access to the device power state controls for debug through the PWR-AP

This field can be modified to implement **SWD Security Level 1**, which allows
for a customized security configuration. Level 1 is well suited for restricted
prototyping/development scenarios and for mass production scenarios where the
the desire is to retain certain SWD functions such as factory reset and TI
failure analysis while disabling other functions (such as application debug).

Refer to the Boot Configuration Routine (BCR) Security Policies section of the
[Cybersecurity Enablers in MSPM0 MCUs app note](https://www.ti.com/lit/slaae29)
for more details on the security levels.
`,
                        /* Hidden to preserve backwards compatibility */
                        hidden      : true,
                        default     : "enabled",
                        options: enableOptions,
                        getDisabledOptions: getDisabledEnableOptions,
                        onChange: onChangeAPEnable,
                        onChange : (inst, ui) => {
                            if ((deviceOptions.SUPPORT_PW_HASH == false) && (inst.apEnable == "disabled"))
                            {
                                /*
                                 * If device using plaintext password and
                                 * disabling debug, set the value to enabled
                                 */
                                inst.debugAccessEnable = "enabledWithPW";
                            }
                            else
                            {
                                /* Otherwise keep value */
                                inst.debugAccessEnable = inst.apEnable;
                            }
                            /*
                             * Catch case where debugAccessEnable is set with
                             * password
                             */
                            if (inst.debugAccessEnable == "enabledWithPW")
                            {
                                system.utils.showGroupConfig("GROUP_SWD_PW", inst, ui);
                            }
                        },
                    },
                    {
                        name        : "debugAccessEnable",
                        displayName : "Enable Application Debug Access",
                        description : "Selects if application debug access is enabled, disabled, or enabled with password.",
                        longDescription: `
Application debug access includes:

* Full access to the processor, memory map, and peripherals through the AHB-AP
* Access to the device EnergyTrace+ state information through the ET-AP
* Access to the device power state controls for debug through the PWR-AP

This field can be modified to implement **SWD Security Level 1**, which allows
for a customized security configuration. Level 1 is well suited for restricted
prototyping/development scenarios and for mass production scenarios where the
the desire is to retain certain SWD functions such as factory reset and TI
failure analysis while disabling other functions (such as application debug).

Refer to the Boot Configuration Routine (BCR) Security Policies section of the
[Cybersecurity Enablers in MSPM0 MCUs app note](https://www.ti.com/lit/slaae29)
for more details on the security levels.
`,
                        hidden      : false,
                        default     : "enabled",
                        /* Special AP enable options due to debug disable */
                        options: getAPEnableOptions,
                        getDisabledOptions: getDisabledEnableOptions,
                        onChange    : onChangeAPEnable,
                    },
                    {
                        // .passwordDebugLock
                        name: "GROUP_SWD_PW",
                        displayName: "SWD Password",
                        longDescription: sha256Note,
                        config: swdPWConfig,
                    },
                    // .tifaMode
                    {
                        name        : "tifaEnable",
                        displayName : "Enable TI Failure Analysis",
                        description : "Allows TI failure analysis access",
                        longDescription: warningNoteM0C + `
TI failure analysis access includes the ability for TI to initiate a failure
analysis return flow through SWD.

**Note**: This flow always forces a factory reset before FA access is given to
TI to ensure that TI does not have any mechanism to read proprietary customer
information stored in the device flash memory when a failure analysis flow is
initiated.
`,
                        hidden      : false,
                        default     : !(Common.isDeviceFamily_PARENT_MSPM0C110X()),
                        readOnly    : Common.isDeviceFamily_PARENT_MSPM0C110X(),
                        onChange    : onChangeTIFAEnable,
                    },
                ].concat(extendedConfigBCRDebug),
            },
            {
                name: "GROUP_SWD_MASS_ERASE_FACTORY_RESET_CONFIG",
                displayName: "SWD Mass Erase and Factory Reset Configuration",
                description: "",
                collapsed: true,
                config: [
                    {
                        // .factoryResetMode
                        /* BOOTCFG2.FRMODE */
                        name        : "frMode",
                        displayName : "Factory Reset Mode Policy",
                        description : "Sets the factory reset mode policy",
                        longDescription: `
The BCR provides factory reset functionality through commands sent to the
device over SWD from a debug probe using the debug subsystem mailbox (DSSM).
This command is not available in SWD security level 2 but is optionally
available in security levels 0 and 1.

The factory reset command can be configured to be enabled, enabled with a
password, or disabled.

A SWD factory reset is an erase of the MAIN flash regions followed by a reset
of the NONMAIN flash region to default values. This erase is useful for
completely resetting the BCR and BSL device boot policies while also erasing
application code and data.
`,
                        hidden      : false,
                        default     : "enabled",
                        options: enableOptions,
                        getDisabledOptions: getDisabledEnableOptions,
                        onChange    : onChangeFactoryReset,
                    },
                    {
                        // .passwordFactoryReset
                        name: "GROUP_FACTORY_RESET_PW",
                        displayName: "Factory Reset Password",
                        longDescription: sha256Note,
                        config: frPWConfig,
                    },
                    {
                        // .massEraseMode
                        /* BOOTCFG2.MEMODE */
                        name        : "meMode",
                        displayName : "Mass Erase Mode Policy",
                        description : "Sets the mass erase mode policy",
                        longDescription: warningNoteM0C + `
The BCR provides mass erase functionality through commands sent to the
device over SWD from a debug probe using the debug subsystem mailbox (DSSM).
This command is not available in SWD security level 2 but is optionally
available in security levels 0 and 1.

The mass erase command can be configured to be enabled, enabled with a
password, or disabled.

A SWD mass erase is an erase of the MAIN flash regions only, which typically
includes the user application. The BCR and BSL policies stored in NONMAIN
flash are not affected. This erase is useful for erasing all application code
and data while leaving the device configuration intact.
`,
                        hidden      : false,
                        default     : "enabled",
                        readOnly    : Common.isDeviceFamily_PARENT_MSPM0C110X(),
                        options: enableOptions,
                        getDisabledOptions: getDisabledEnableOptions,
                        onChange    : onChangeMassErase,
                    },
                    {
                        // .passwordMassErase
                        name: "GROUP_MASS_ERASE_PW",
                        displayName: "Mass Erase Password",
                        longDescription: sha256Note,
                        config: mePWConfig,
                    },
                ],
            },
            {
                name: "GROUP_FLASH_STATIC_WRITE_PROTECTION_CONFIG",
                displayName: "Flash Memory Static Write Protection (SWP) Configuration"+flashBank0Display,
                description: "",
                config: [
                    {
                        // .staticWriteProtectionMainLow
                        /* FLASHSWP0 */
                        name        : "staticWriteProtectionMainLow",
                        displayName : "MAIN SWP (Lower Sectors)",
                        description : "Sets the static write protection of the lower 32 sectors",
                        longDescription: `
One bit corresponds to one sector with the LSB being Sector 0. Setting a bit to
0 enables write protection, and setting a bit to 1 disables write protection.

For example, on an MSPM0 device with 32kB of flash or more and a
sector size of 1kB, this setting defines the protection of the lower 32kB.

A value of 0x7FFFFFFE (bits 31 and 0 cleared) would make all sectors writable,
except for sector 0 (0x0000_0000-0x0000_003FF) and sector 31 (0x0000_7C00-0x0000_7FFF).

On a device with less than 32KB, the value of bits corresponding to
unimplemented sectors is ignored.

Refer to the TRM and the device datasheet for more information.
`,
                        hidden      : false,
                        displayFormat: "hex",
                        default     : 0xFFFFFFFF,
                        range       : [0, 0xFFFFFFFF]
                    },
                    {
                        // .staticWriteProtectionMainHigh
                        /* FLASHSWP1 */
                        name        : "staticWriteProtectionMainHigh",
                        displayName : "MAIN SWP (Remaining Sectors)",
                        description : "Sets the static write protection of the remaining sectors",
                        longDescription: `
One bit corresponds to eight sectors. Setting a bit to 0 enables write
protection, and setting a bit to 1 disables write protection. Bits 0-3 are ignored.

For example, on an MSPM0 device with more than 32kB of flash and a
sector size of 1kB, this setting defines the protection of memory above 32kB.

A value of 0xFFFFFFEx (bit 4 cleared) would make all sectors writable,
except for sector 39-32 (0x0000_8000-0x0000_9FFF).

The value of bits corresponding to unimplemented sectors is ignored.

Refer to the TRM and the device datasheet for more information.
`,
                        hidden      : false,
                        displayFormat: "hex",
                        default     : 0xFFFFFFFF,
                        range       : [0, 0xFFFFFFFF]
                    },
                    {
                        // .staticWriteProtectionNonMain
                        /* BOOTCFG3.NONMAINLOCK */
                        name        : "staticWriteProtectionNonMain",
                        displayName : "NONMAIN Static Write Protection",
                        description : "Configures whether to lock device configuration",
                        longDescription: `
Statically write protecting NONMAIN prevents any further erase or program
operations on the region. Application code and the BSL have no mechanism to
change the configuration data.
`,
                        hidden      : false,
                        default     : false,
                    },]
            },].concat(extendedConfigWriteProtection).concat([
            {
                name: "GROUP_APP_CRC_CHECK_CONFIG",
                displayName: "User Application CRC Check Configuration",
                description: "",
                config: [
                    {
                        // .secureBootMode
                        /* BOOTCFG3.APPCRCMODE */
                        name        : "appCRCCheck",
                        displayName : "Enable Application CRC Check",
                        description : "Enable Application CRC Check",
                        longDescription: `
The BCR supports executing a complete CRC32 integrity check of the application
code and data contained in the MAIN flash regions during the boot process
before starting the user application. This is useful in ensuring the integrity
of some or all of the application code and data before execution.

If the application CRC check fails at boot, the application in MAIN flash won't
be started. If the BSL is enabled, it is entered. If not, then the boot fails.
`,
                        /* This feature is a nice-to-have. Marking as hidden for now */
                        hidden      : true,
                        default     : false,
                        onChange    : (inst, ui) => {
                            ui.appCRCCheckStartAddress.hidden = !(inst.appCRCCheck);
                            ui.appCRCCheckLength.hidden = !(inst.appCRCCheck);
                            ui.appCRC.hidden = !(inst.appCRCCheck);
                            if(deviceOptions.BCR_SUPPORT_USER_APP_HASH == true){
                                ui.appCRCHash.hidden = !(inst.appCRCCheck);
                            }
                        },
                    },
                    {
                        // .userSecureAppStartAddr
                        /* APPCRCSTART */
                        name        : "appCRCCheckStartAddress",
                        displayName : "Application CRC Check Start Address",
                        description : "The starting address of the application CRC check (must be an address in a MAIN flash region)",
                        longDescription: ``,
                        hidden      : true,
                        displayFormat: "hex",
                        default     : 0xFFFFFFFF,
                        range       : [0, 0xFFFFFFFF]
                    },
                    {
                        // .userSecureAppLength
                        /* APPCRCLEN */
                        name        : "appCRCCheckLength",
                        displayName : "Application CRC Check Application Length",
                        description : "The length of the application for which the CRC is calculated",
                        longDescription: ``,
                        hidden      : true,
                        displayFormat: "hex",
                        default     : 0xFFFFFFFF,
                        range       : [0, 0xFFFFFFFF]
                    },
                    {
                        // .userSecureAppCrc
                        /* APPCRC */
                        name        : "appCRC",
                        displayName : "Expected Application CRC Check Digest (CRC-32)",
                        description : "The expected CRC value to test against during boot",
                        longDescription: ``,
                        hidden      : true,
                        displayFormat: "hex",
                        default     : 0xFFFFFFFF,
                        range       : [0, 0xFFFFFFFF]
                    },
                ].concat(extendedConfigBCRCRC),
            },
        ]).concat(extendedConfigBCR).concat([
            {
                // .fastBootMode
                /* BOOTCFG1.FASTBOOTMODE */
                name        : "fastBootMode",
                displayName : "Enable Fast Boot Mode",
                description : "Enable Fast Boot Mode",
                longDescription: warningNoteM0C + `
The execution time of the BCR can be reduced by enabling fast boot mode. Fast
boot mode speeds up the boot process via the following methods:

* Limiting the BSL entry methods. Only SYSCTL register invoke and the DSSM
  invoke methods may be used to enter the bootloader. Other BSL invoke conditions
  such as pin-based invoke are not tested.

* Bypassing the application CRC check, even if the application CRC check is
  enabled.
`,
                hidden      : false,
                default     : false,
                readOnly    : Common.isDeviceFamily_PARENT_MSPM0C110X(),
            },
            // .bcrConfigID
            {
                name        : "bcrConfigID",
                displayName : "BCR Configuration ID",
                description : "Predetermined Config Signature Magic ID",
                longDescription: ``,
                hidden      : false,
                readOnly    : true,
                displayFormat: "hex",
                default     : deviceOptions.BCR_CONFIG_ID,
            },
            {
                // .userCfgCRC
                /* BOOTCRC */
                name        : "bootCRC",
                displayName : "Expected BCR Configuration CRC",
                description : "The expected CRC value to test against during boot",
                longDescription: warningNoteM0C + `
The BCR configuration data structure is protected by a 32-bit CRC to improve
security. The BCR configuration CRC is stored in the BOOTCRC field of NONMAIN.

If the stored CRC of the BCR configuration doesn't match the calculated CRC
during boot, the result is a boot error. BSL will not be invoked, the user
application is not started, and no application debug access is enabled. Pending
SWD factory reset commands and TI failure analysis flow entry will be honored.
See the device TRM for more details.
`,
                hidden      : false,
                displayFormat: "hex",
                default     : 0,
                getValue    : (inst) => {
                    if (Common.isDeviceFamily_PARENT_MSPM0L122X_L222X() ||
                        Common.isDeviceFamily_PARENT_MSPM0GX51X() ||
                        Common.isDeviceFamily_PARENT_MSPM0L111X())
                    {
                        return calculateBCRCRC_Advanced_32Bit(inst);
                    }
                    else if (Common.isDeviceFamily_PARENT_MSPM0H321X() ||
                             Common.isDeviceFamily_PARENT_MSPM0C1105_C1106())
                    {
                        return calculateBCRCRC_Advanced_16Bit(inst);
                    }
                    else if (!Common.isDeviceFamily_PARENT_MSPM0C110X())
                    {
                        return calculateBCRCRC(inst);
                    }
                    else
                    {
                        /* M0C doesn't support BCR CRC validation */
                        return 0xFFFFFFFF;
                    }
                },
            },
            {
                name: "bootCRCString",
                /* Test and debug only. Used in crc output .txt file */
                hidden: true,
                default: "",
                getValue: (inst) => {
                    if (Common.isDeviceFamily_PARENT_MSPM0L122X_L222X() ||
                        Common.isDeviceFamily_PARENT_MSPM0GX51X() ||
                        Common.isDeviceFamily_PARENT_MSPM0L111X())
                    {
                        return createAdvancedBCRConfigString(inst);
                    }
                    else if (Common.isDeviceFamily_PARENT_MSPM0H321X() ||
                             Common.isDeviceFamily_PARENT_MSPM0C1105_C1106())
                    {
                        return createAdvancedBCRConfigString(inst);
                    }
                    else if (!Common.isDeviceFamily_PARENT_MSPM0C110X())
                    {
                        return createBCRString(inst);
                    }
                    else
                    {
                        /* M0C doesn't support BCR CRC validation */
                        return "FFFFFFFF";
                    }
                }
            },
            {
                // .bootloaderMode
                /* BOOTCFG1.BSLMODE */
                name        : "bslMode",
                displayName : "Enable BSL",
                description : "Enable BSL",
                longDescription: warningNoteM0C + `
**Note**, when the BSL is disabled, it is not possible to enter the BSL through
any invocation mechanism.
`,
                hidden      : false,
                default     : !(deviceOptions.SUPPORT_ROM_BSL != true),
                readOnly    : Common.isDeviceFamily_PARENT_MSPM0C110X(),
                onChange    : onChangeBSLEnable,
            },
        ])
    },
]


const CRC32 = require('./crc/cjs/calculators/crc32').default;
/*
 * BSL CRC for N1_48 uses the CRC16-CCITT polynomial with a custom
 * configuration (input and output reflected with initial seed value of
 * 0xFFFF). This custom configuration isn't supported in the CRC package, so
 * use CRC16-Kermit, which uses same polynomial and input/output reflection,
 * and specify a seed value of 0xFFFF
 */
const CRC16 = require('./crc/cjs/calculators/crc16kermit').default;

function createBCRString(inst)
{
    /*
     * Data is stored, LSB first. To emulate, create a string of the BCR struct
     * values, starting from the last struct field (excluding the BCR struct CRC).
     *
     * Once the string is created, convert to an array and reverse the values.
     */
    let bcrConfigStr = "";

    /*
     * Strings should be padded appropriately so that 0s are included in CRC
     * calculations
     */
    bcrConfigStr += (inst.appCRC).toString(16).padStart(8, "0");
    bcrConfigStr += (inst.appCRCCheckLength).toString(16).padStart(8, "0");
    bcrConfigStr += (inst.appCRCCheckStartAddress).toString(16).padStart(8, "0");
    bcrConfigStr += (inst.appCRCCheck) ? "AABB" : "FFFF";

    bcrConfigStr += (inst.staticWriteProtectionNonMain) ? "FFFE" : "FFFF";
    bcrConfigStr += (inst.staticWriteProtectionMainHigh).toString(16).padStart(8, "0");
    bcrConfigStr += (inst.staticWriteProtectionMainLow).toString(16).padStart(8, "0");

    bcrConfigStr += (inst.frPW3).toString(16).padStart(8, "0") +
        (inst.frPW2 ).toString(16).padStart(8, "0") +
        (inst.frPW1).toString(16).padStart(8, "0") +
        (inst.frPW0).toString(16).padStart(8, "0");
    bcrConfigStr += (inst.mePW3).toString(16).padStart(8, "0") +
        (inst.mePW2).toString(16).padStart(8, "0") +
        (inst.mePW1).toString(16).padStart(8, "0") +
        (inst.mePW0).toString(16).padStart(8, "0");

    let meModeStr = "FFFF";
    if (inst.meMode == "enabled")
    {
        meModeStr = "AABB";
    }
    else if (inst.meMode == "enabledWithPW")
    {
        meModeStr = "CCDD";
    }

    let frModeStr = "FFFF";
    if (inst.frMode == "enabled")
    {
        frModeStr = "AABB";
    }
    else if (inst.frMode == "enabledWithPW")
    {
        frModeStr = "CCDD";
    }

    bcrConfigStr += frModeStr + meModeStr;

    bcrConfigStr += (inst.bslMode) ? "AABB" : "FFFF";
    bcrConfigStr += (inst.fastBootMode) ? "AABB" : "FFFF";

    bcrConfigStr += (inst.swdPW3).toString(16).padStart(8, "0") +
        (inst.swdPW2).toString(16).padStart(8, "0") +
        (inst.swdPW1).toString(16).padStart(8, "0") +
        (inst.swdPW0).toString(16).padStart(8, "0");

    bcrConfigStr += (inst.bslInvokePinCheck) ? "AABB" : "FFFF";
    bcrConfigStr += (inst.tifaEnable) ? "AABB" : "FFFF";
    bcrConfigStr += (inst.swdEnable) ? "AABB" : "FFFF";

    let debugAccessStr = "FFFF"
    if (inst.debugAccessEnable == "enabled")
    {
        debugAccessStr = "AABB";
    }
    else if (inst.debugAccessEnable == "enabledWithPW")
    {
        debugAccessStr = "CCDD";
    }

    bcrConfigStr += debugAccessStr;
    bcrConfigStr += (inst.bcrConfigID).toString(16).padStart(8, "0");

    return (bcrConfigStr);
}

function calculateBCRCRC(inst)
{
    let bcrConfigStr = createBCRString(inst);

    /* Convert into an array and reverse the values */
    let bcrConfigArr = chunkString(bcrConfigStr, 2).reverse();

    /* Convert array of strings into array of hex numbers */
    let crc = CRC32(bcrConfigArr.map(el => parseInt(el, 16)));

    /* Calculate the JAMCRC and get the unsigned value */
    crc = (~crc) >>> 0

    return (crc);
}

/* Create config string to calculate CRC over */
function createAdvancedBCRConfigString(inst)
{
    /*
     * Data is stored, LSB first. To emulate, create a string of the BCR struct
     * values, starting from the last struct field (excluding the BCR struct CRC).
     *
     * Once the string is created, convert to an array and reverse the values.
     */
    let bcrConfigStr = "";

    /*
     * Strings should be padded appropriately so that 0s are included in CRC
     * calculations
     */

    if (Common.isDeviceFamily_PARENT_MSPM0L122X_L222X() ||
        Common.isDeviceFamily_PARENT_MSPM0L111X() ||
        Common.isDeviceFamily_PARENT_MSPM0H321X() ||
        Common.isDeviceFamily_PARENT_MSPM0C1105_C1106())
    {
    /* Reserved, 32-bits (bc_reserved_1 or bc_reserved_2 for H321x) */
        bcrConfigStr +=  "FFFFFFFF"
    }
    else if (Common.isDeviceFamily_PARENT_MSPM0GX51X())
    {
        bcrConfigStr += (inst.staticWriteProtectionMainHigh_2).toString(16).padStart(8, "0");
    }

    /*
     * App CRC Hash: option currently not handled by SysConfig, have default
     * value in the meantime.
     *
     * Commented out the user-provided option for now.
     */
    // bcrConfigStr += (appCRCHash7).toString(16).padStart(8, "0") +
    // (appCRCHash6).toString(16).padStart(8, "0") +
    // (appCRCHash5).toString(16).padStart(8, "0") +
    // (appCRCHash4).toString(16).padStart(8, "0") +
    // (appCRCHash3).toString(16).padStart(8, "0") +
    // (appCRCHash2).toString(16).padStart(8, "0") +
    // (appCRCHash1).toString(16).padStart(8, "0") +
    // (appCRCHash0).toString(16).padStart(8, "0");
    //
    /*
     * App CRC Hash: option currently not handled by SysConfig, have default
     * value in the meantime
     *
     * Using 0xFFFFFFFF (CFG_DEFAULT_VALUE) default value for now
     */
    bcrConfigStr += (0xFFFFFFFF).toString(16).padStart(8, "0") +
    (0xFFFFFFFF).toString(16).padStart(8, "0") +
    (0xFFFFFFFF).toString(16).padStart(8, "0") +
    (0xFFFFFFFF).toString(16).padStart(8, "0") +
    (0xFFFFFFFF).toString(16).padStart(8, "0") +
    (0xFFFFFFFF).toString(16).padStart(8, "0") +
    (0xFFFFFFFF).toString(16).padStart(8, "0") +
    (0xFFFFFFFF).toString(16).padStart(8, "0");

    bcrConfigStr += (inst.appCRCCheckLength).toString(16).padStart(8, "0");
    bcrConfigStr += (inst.appCRCCheckStartAddress).toString(16).padStart(8, "0");
    bcrConfigStr += (inst.appCRCCheck) ? "AABB" : "FFFF";

    /* Reserved, 16-bits (bc_reserved_0) or bc_reserved_1 for H321x */
    bcrConfigStr += "FFFF";

    bcrConfigStr += (inst.swdPW7).toString(16).padStart(8, "0") +
        (inst.swdPW6).toString(16).padStart(8, "0") +
        (inst.swdPW5).toString(16).padStart(8, "0") +
        (inst.swdPW4).toString(16).padStart(8, "0") +
        (inst.swdPW3).toString(16).padStart(8, "0") +
        (inst.swdPW2).toString(16).padStart(8, "0") +
        (inst.swdPW1).toString(16).padStart(8, "0") +
        (inst.swdPW0).toString(16).padStart(8, "0");
    bcrConfigStr += (inst.frPW7).toString(16).padStart(8, "0") +
        (inst.frPW6).toString(16).padStart(8, "0") +
        (inst.frPW5).toString(16).padStart(8, "0") +
        (inst.frPW4).toString(16).padStart(8, "0") +
        (inst.frPW3).toString(16).padStart(8, "0") +
        (inst.frPW2).toString(16).padStart(8, "0") +
        (inst.frPW1).toString(16).padStart(8, "0") +
        (inst.frPW0).toString(16).padStart(8, "0");
    bcrConfigStr += (inst.mePW7).toString(16).padStart(8, "0") +
        (inst.mePW6).toString(16).padStart(8, "0") +
        (inst.mePW5).toString(16).padStart(8, "0") +
        (inst.mePW4).toString(16).padStart(8, "0") +
        (inst.mePW3).toString(16).padStart(8, "0") +
        (inst.mePW2).toString(16).padStart(8, "0") +
        (inst.mePW1).toString(16).padStart(8, "0") +
        (inst.mePW0).toString(16).padStart(8, "0");

    let meModeStr = "FFFF";
    if (inst.meMode == "enabled")
    {
        meModeStr = "AABB";
    }
    else if (inst.meMode == "enabledWithPW")
    {
        meModeStr = "CCDD";
    }

    let frModeStr = "FFFF";
    if (inst.frMode == "enabled")
    {
        frModeStr = "AABB";
    }
    else if (inst.frMode == "enabledWithPW")
    {
        frModeStr = "CCDD";
    }

    bcrConfigStr += frModeStr + meModeStr;

    bcrConfigStr += (inst.bslMode) ? "AABB" : "FFFF";
    bcrConfigStr += (inst.fastBootMode) ? "AABB" : "FFFF";

    /* H321x does not have flash bank swap. Has 16-bit bc_reserved_0 instead */
    if (Common.isDeviceFamily_PARENT_MSPM0H321X() ||
        Common.isDeviceFamily_PARENT_MSPM0C1105_C1106())
    {
        bcrConfigStr += "FFFF";
    }
    else
    {
        bcrConfigStr += (inst.flashBankSwap) ? "FFFF" : "AABB";
    }

    bcrConfigStr += (inst.cscExists) ? "FFFF" : "AABB";
    bcrConfigStr += (inst.debugHold) ? "FFFF" : "AABB";

    bcrConfigStr += (inst.staticWriteProtectionNonMain) ? "FFFF" : "AABB";
    bcrConfigStr += (inst.staticWriteProtectionMainHigh).toString(16).padStart(8, "0");
    bcrConfigStr += (inst.staticWriteProtectionMainLow).toString(16).padStart(8, "0");

    bcrConfigStr += (inst.bslInvokePinCheck) ? "AABB" : "FFFF";
    bcrConfigStr += (inst.tifaEnable) ? "AABB" : "FFFF";
    bcrConfigStr += (inst.swdEnable) ? "AABB" : "FFFF";

    let debugAccessStr = "FFFF"
    if (inst.debugAccessEnable == "enabled")
    {
        debugAccessStr = "AABB";
    }
    else if (inst.debugAccessEnable == "enabledWithPW")
    {
        debugAccessStr = "CCDD";
    }

    bcrConfigStr += debugAccessStr;
    bcrConfigStr += (inst.bcrConfigID).toString(16).padStart(8, "0");

    return (bcrConfigStr);
}

/*
 * Splits a string into substrings of a particular length. Helper function for
 * CRC calculations
 */
function chunkString(str, length)
{
    return str.match(new RegExp('.{1,' + length + '}', 'g'));
}

/* 32-bit CRCJAM Calculation for L122x_L222x, Gx51x, and L111x devices BCR */
function calculateBCRCRC_Advanced_32Bit(inst)
{
    let bcrConfigStr = createAdvancedBCRConfigString(inst);

    /* Convert into an array and reverse the values */
    let bcrConfigArr = chunkString(bcrConfigStr, 2).reverse();

    /* Convert array of strings into array of hex numbers */
    let crc = CRC32(bcrConfigArr.map(el => parseInt(el, 16)));

    /* Calculate the JAMCRC and get the unsigned value */
    crc = (~crc) >>> 0

    return (crc);
}

/* 16-bit CRC Calculation for H321x BCR */
function calculateBCRCRC_Advanced_16Bit(inst)
{
    let bcrConfigStr = createAdvancedBCRConfigString(inst);

    /* Convert into an array and reverse the values */
    let bcrConfigArr = chunkString(bcrConfigStr, 2).reverse();

    /* Convert array of strings into hex numbers. CRC16-Kermit + FFFF seed */
    let crc = CRC16(bcrConfigArr.map(el => parseInt(el, 16)), 0xFFFF);

    return (crc);
}

/* Create BSL config string (supporting ROM BSL) to calculate CRC over */
function createROMBSLString(inst)
{
    /*
     * Data is stored, LSB first. To emulate, create a string of the BSL struct
     * values, starting from the last struct field (excluding the BSL struct CRC).
     *
     * Once the string is created, convert to an array and reverse the values.
     */
    let bslConfigStr = "";

    if(Common.isDeviceFamily_PARENT_MSPM0L111X()){
        /* Reserved, 32-bits (bl_reserved_1) */
        bslConfigStr +=  "FFFFFFFF"
        /* Reserved, 16-bits (bl_reserved_0) */
        bslConfigStr += "FFFF";
        /* disableNRST (AABB - Disables NRST, all other values - enabled) */
        bslConfigStr += (inst.disableNRST) ? "AABB" : "FFFF";
    }

    bslConfigStr += (inst.i2cSlaveAddress).toString(16).padStart(4, "0");

    let securityAlertStr = "FFFF";
    if (inst.bslSecurityConfig == "triggerFactoryReset")
    {
        securityAlertStr = "AABB";
    }
    else if (inst.bslSecurityConfig == "disableBSL")
    {
        securityAlertStr = "CCDD";
    }

    bslConfigStr += securityAlertStr;
    bslConfigStr += (inst.bslAppVersion).toString(16).padStart(8, "0");
    /*
     * Alternate BSL address is at a fixed location (secondary_bsl example).
     * Note, Cortex-M type ARM processors only support thumb mode. Any pointers
     * to functions must have the LSB set. When taking the address of a
     * function, the toolchain automatically handles this.
     *
     * Therefore, the alternate BSL address is actually located at 0x00001041
     */
    bslConfigStr += (inst.bslAltAddress).toString(16).padStart(8, "0");

    if(!Common.isDeviceFamily_PARENT_MSPM0GX51X() && !Common.isDeviceFamily_PARENT_MSPM0L111X()){
        /* Reserved, 16-bits */
        bslConfigStr += "FFFF";
    }

    bslConfigStr += (inst.bslAltConfig) ? "AABB" : "FFFF";

    if(Common.isDeviceFamily_PARENT_MSPM0GX51X() || Common.isDeviceFamily_PARENT_MSPM0L111X()){
        bslConfigStr += inst.uartBaudDefault;
    }

    /*
     * Flash plugins are at fixed addresses  (see linker cmd file).
     * Note, Cortex-M type ARM processors only support thumb mode. Any pointers
     * to functions must have the LSB set. When taking the address of a
     * function, the toolchain automatically handles this.
     *
     * Therefore, these plugin functions should actually be provided address + 1
     */
    let deInitPluginAddress = (inst.bslPluginHookDeInitAddress != 0xFFFFFFFF) ? (inst.bslPluginHookDeInitAddress + 1) : 0xFFFFFFFF;
    let transmitPluginAddress = (inst.bslPluginHookTransmitAddress != 0xFFFFFFFF) ? (inst.bslPluginHookTransmitAddress + 1) : 0xFFFFFFFF;
    let receivePluginAddress = (inst.bslPluginHookReceiveAddress != 0xFFFFFFFF) ? (inst.bslPluginHookReceiveAddress + 1) : 0xFFFFFFFF;
    let initPluginAddress = (inst.bslPluginHookInitAddress != 0xFFFFFFFF) ? (inst.bslPluginHookInitAddress + 1) : 0xFFFFFFFF;

    bslConfigStr += deInitPluginAddress.toString(16).padStart(8, "0");
    bslConfigStr += transmitPluginAddress.toString(16).padStart(8, "0");
    bslConfigStr += receivePluginAddress.toString(16).padStart(8, "0");
    bslConfigStr += initPluginAddress.toString(16).padStart(8, "0");

    bslConfigStr += (inst.bslPluginSRAMSize).toString(16).padStart(2, "0");

    bslConfigStr += (inst.bslFlashPluginEnable) ? "BB" : "FF";

    let pluginTypeStr = "FFFF";
    if (inst.bslPluginType == "bslPluginUART")
    {
        pluginTypeStr = "1000";
    }
    else if (inst.bslPluginType == "bslPluginI2C")
    {
        pluginTypeStr = "2000";
    }

    bslConfigStr += pluginTypeStr;

    bslConfigStr += (inst.bslPW7).toString(16).padStart(8, "0") +
        (inst.bslPW6).toString(16).padStart(8, "0") +
        (inst.bslPW5).toString(16).padStart(8, "0") +
        (inst.bslPW4).toString(16).padStart(8, "0") +
        (inst.bslPW3).toString(16).padStart(8, "0") +
        (inst.bslPW2).toString(16).padStart(8, "0") +
        (inst.bslPW1).toString(16).padStart(8, "0") +
        (inst.bslPW0).toString(16).padStart(8, "0");

    bslConfigStr += (inst.bslEnableReadOut) ? "AABB" : "FFFF";

    bslConfigStr += (inst.bslInvokePinData1).toString(16).padStart(2, "0");
    bslConfigStr += (inst.bslInvokePinData0).toString(16).padStart(2, "0");

    bslConfigStr += (inst.i2cSCLMux).toString(16).padStart(2, "0") +
        (inst.i2cSCLPadNum).toString(16).padStart(2, "0");
    bslConfigStr += (inst.i2cSDAMux).toString(16).padStart(2, "0") +
        (inst.i2cSDAPadNum).toString(16).padStart(2, "0");
    bslConfigStr += (inst.uartTXMux).toString(16).padStart(2, "0") +
        (inst.uartTXPadNum).toString(16).padStart(2, "0");
    bslConfigStr += (inst.uartRXMux).toString(16).padStart(2, "0") +
        (inst.uartRXPadNum).toString(16).padStart(2, "0");

    bslConfigStr += (inst.bslConfigID).toString(16).padStart(8, "0");

    return (bslConfigStr);
}

/* Create no-Flash BSL config string for CRC calculation */
function createNoFlashBSLConfigString(inst)
{
    /*
     * Data is stored, LSB first. To emulate, create a string of the BSL struct
     * values, starting from the last struct field (excluding the BSL struct CRC).
     *
     * Once the string is created, convert to an array and reverse the values.
     */
    let bslConfigStr = "";

    /* Reserved, 32-bits (bl_reserved_1) */
    bslConfigStr +=  "FFFFFFFF"
    /* Reserved, 16-bits (bl_reserved_0) */
    bslConfigStr += "FFFF";
    /* disableNRST */
    bslConfigStr += "FFFF";

    /* I2C Address string */
    bslConfigStr += "FFFF";
    /* Security Alert string */
    bslConfigStr += "FFFF";
    /* Application Version string */
    bslConfigStr += "FFFFFFFF";
    /*
     * Alternate BSL address is at a fixed location (secondary_bsl example).
     * Note, Cortex-M type ARM processors only support thumb mode. Any pointers
     * to functions must have the LSB set. When taking the address of a
     * function, the toolchain automatically handles this.
     *
     * Therefore, the alternate BSL address is actually located at 0x00001041
     */
    bslConfigStr += (inst.bslAltAddress).toString(16).padStart(8, "0");
    bslConfigStr += (inst.bslAltConfig) ? "AABB" : "FFFF";

    /* Total 56 reserved bytes filled with F's in this chunk */
    /* UART baud string */
    bslConfigStr += "FFFF";

    /* Flash plugin strings */
    bslConfigStr += "FFFFFFFF"; /* De-init */
    bslConfigStr += "FFFFFFFF"; /* Transmit */
    bslConfigStr += "FFFFFFFF"; /* Receive */
    bslConfigStr += "FFFFFFFF"; /* Init */
    bslConfigStr += "FF"; /* Flash plugin SRAM size */
    bslConfigStr += "FF"; /* Flash plugin enable */
    bslConfigStr += "FFFF"; /* Plugin Type */

    /* BSL PW string */
    bslConfigStr += "FFFFFFFF" + "FFFFFFFF" + "FFFFFFFF" + "FFFFFFFF" +
    "FFFFFFFF" + "FFFFFFFF" + "FFFFFFFF" + "FFFFFFFF";

    /* BSL Memory Readout string */
    bslConfigStr += "FFFF";
    /* End reserved chunk */

    bslConfigStr += (inst.bslInvokePinData1).toString(16).padStart(2, "0");
    bslConfigStr += (inst.bslInvokePinData0).toString(16).padStart(2, "0");

    /* BSL UART and I2C pins */
    bslConfigStr += "FFFFFFFF" + "FFFFFFFF";

    bslConfigStr += (inst.bslConfigID).toString(16).padStart(8, "0");

    return (bslConfigStr);
}

/* Calculate 32-bit CRCJAM over BSL config string (supporting ROM BSL) */
function calculateBSLCRC_32Bit(inst)
{
    let bslConfigStr = createROMBSLString(inst);

    /* Convert into an array and reverse the values */
    let bslConfigArr = chunkString(bslConfigStr, 2).reverse();

    /* Convert array of strings into array of hex numbers */
    let crc = CRC32(bslConfigArr.map(el => parseInt(el, 16)));

    /* Calculate the JAMCRC and get the unsigned value */
    crc = (~crc) >>> 0

    return (crc);
}

/*
 * Calculate 16-bit CRC over BSL config string (not supporting Flash BSL)
 * Currently only devices that don't support Flash BSL require 16-bit CRC
 * calculations
 */
function calculateBSLCRC_NoFlash_16Bit(inst)
{
    let bslConfigStr = createNoFlashBSLConfigString(inst);

    /* Convert into an array and reverse the values */
    let bslConfigArr = chunkString(bslConfigStr, 2).reverse();

    /* Convert array of strings into hex numbers. CRC16-Kermit + FFFF seed */
    let crc = CRC16(bslConfigArr.map(el => parseInt(el, 16)), 0xFFFF);

    return (crc);
}

/* Get device data pins with a valid PINCM */
let validDeviceDataPins = _.filter(system.deviceData.devicePins, (pin) =>
    pin.attributes.iomux_pincm != "None");

/* Create array of GPIO pin names */
const pinOptions = _.map(validDeviceDataPins, (pin) =>
    ({name: pin.mux.muxSetting[0].peripheralPin.peripheralName}));

/* BSL Invoke Pin will never be on GPIOC */
const filteredPinOptions = pinOptions.filter((pin) =>
    {return Common.getGPIOPort(pin.name) !== "GPIOC"});

/* Create array of GPIO pins with their associated PINCM */
const iomuxPincmOptions = _.map(validDeviceDataPins, (pin) =>
    ({name: pin.designSignalName, num: pin.attributes.iomux_pincm}));

/* Device-specific BSL configurables */
let extendedConfigBSLPW = [];
if (deviceOptions.SUPPORT_ROM_BSL == true)
{
    extendedConfigBSLPW = [
        {
            // .password
            /* BSLPW[0-7] */
            name: "GROUP_BSL_ACCESS_PW",
            displayName: "BSL Access Password",
            longDescription: sha256Note,
            config: createPWConfig("bslPW", false, 8),
        },
    ];
}

/*
 * Default UART/I2C pin names should NOT go in NONMAINOptions.js because the
 * actual pin name can change depending on the particular package. For example,
 * on some pin-constrained packages, the pin name in SysConfig has the format
 * "Px/ABC" rather than "Px"
 */
let defaultBSLUARTRX = "";
let defaultBSLUARTTX = "";
let defaultBSLI2CSCL = "";
let defaultBSLI2CSDA = "";
let uartTXPinData;
let uartRXPinData;
let i2cSCLPinData;
let i2cSDAPinData;
let extendedConfigROMBSL = [];

function isStandaloneNumberPresent(number, str)
{
    const regex = new RegExp(`(?:^|,\s*)(${number})(?=\s*,|$)`);
    const match = str.match(regex);
    /* Match returns the match and captured groups, return the captured group */
    return match ? match[1] : null;
}
if (deviceOptions.SUPPORT_ROM_BSL == true)
{
    /* Derive UART/I2C interface pins */
    /* Create array of pins with UART TX functionality */
    uartTXPinData = validDeviceDataPins.map((obj) => ({
        ...obj,
        muxOptions: obj.mux.muxSetting.filter((muxOption) => muxOption.peripheralPin.name.match(/UART\d.[T]X$/)),
    }));
    uartTXPinData = _.filter(uartTXPinData, (pin) => pin.muxOptions.length);

    /* Create array of pins with UART RX functionality */
    uartRXPinData = validDeviceDataPins.map((obj) => ({
        ...obj,
        muxOptions: obj.mux.muxSetting.filter((muxOption) => muxOption.peripheralPin.name.match(/UART\d.[R]X$/)),
    }));
    uartRXPinData = _.filter(uartRXPinData, (pin) => pin.muxOptions.length);

    /* Create array of pins with I2C SCL functionality */
    i2cSCLPinData = validDeviceDataPins.map((obj) => ({
        ...obj,
        muxOptions: obj.mux.muxSetting.filter((muxOption) => muxOption.peripheralPin.name.match(/I2C\d.SCL$/)),
    }));
    i2cSCLPinData = _.filter(i2cSCLPinData, (pin) => pin.muxOptions.length);

    /* Create array of pins with I2C SDA functionality */
    i2cSDAPinData = validDeviceDataPins.map((obj) => ({
        ...obj,
        muxOptions: obj.mux.muxSetting.filter((muxOption) => muxOption.peripheralPin.name.match(/I2C\d.SDA$/)),
    }));
    i2cSDAPinData = _.filter(i2cSDAPinData, (pin) => pin.muxOptions.length);

    /*
     * Get default BSL UART/I2C pins (based on device). num field can be more
     * than one entry e.g. "A,B" because of dual-purpose pins
     */
    defaultBSLUARTRX = iomuxPincmOptions.find(x => isStandaloneNumberPresent(deviceOptions.BSL_UART_RX_PINCM, x.num));
    defaultBSLUARTTX = iomuxPincmOptions.find(x => isStandaloneNumberPresent(deviceOptions.BSL_UART_TX_PINCM, x.num));
    defaultBSLI2CSCL = iomuxPincmOptions.find(x => isStandaloneNumberPresent(deviceOptions.BSL_I2C_SCL_PINCM, x.num));
    defaultBSLI2CSDA = iomuxPincmOptions.find(x => isStandaloneNumberPresent(deviceOptions.BSL_I2C_SDA_PINCM, x.num));

    defaultBSLUARTRX = (!defaultBSLUARTRX) ? "0xFF" : defaultBSLUARTRX.name;
    defaultBSLUARTTX = (!defaultBSLUARTTX) ? "0xFF" : defaultBSLUARTTX.name;
    defaultBSLI2CSCL = (!defaultBSLI2CSCL) ? "0xFF" : defaultBSLI2CSCL.name;
    defaultBSLI2CSDA = (!defaultBSLI2CSDA) ? "0xFF" : defaultBSLI2CSDA.name;

    extendedConfigROMBSL = [
        {
            name: "GROUP_BSL_UART_PIN_CONFIG",
            displayName: "BSL UART Pin Configuration",
            description: "",
            config: [
                {
                    name: "uartPeripheral",
                    displayName: "UART Peripheral",
                    description: "Select the UART peripheral",
                    hidden: false,
                    /* BSL UART instance is tied to UART0 */
                    readOnly: true,
                    default: "UART0",
                },
            ].concat(bslExtendedConfigUART).concat([
                {
                    name: "uartTXPin",
                    displayName: "UART TX Pin",
                    description: "Select the UART TX pin",
                    hidden: false,
                    default: defaultBSLUARTTX,
                    options: createUARTTXPinOptions,
                },
                {
                    // .interfacePins.UART_TXD_pad_num
                    name: "uartTXPadNum",
                    displayName: "UART TX Pad Number",
                    description: "Pad number represents the IOMUX PINCM value",
                    default: 0,
                    getValue: (inst) => {
                        let selectedPin = uartTXPinData.find(pin => pin.designSignalName == inst.uartTXPin);
                        let iomuxPINCM = 0;

                        if (selectedPin)
                        {
                            /* Convert to integer with radix 10 */
                            iomuxPINCM = parseInt(selectedPin.attributes.iomux_pincm, 10);
                        }

                        return iomuxPINCM;
                    }
                },
                {
                    // .interfacePins.UART_TXD_PF_mux_sel
                    name: "uartTXMux",
                    displayName: "UART TX Mux",
                    description: "The mux value represents the pin function",
                    default: 0,
                    getValue: (inst) => {
                        let selectedPin = uartTXPinData.find(pin => pin.designSignalName == inst.uartTXPin);
                        let muxSetting = 0;

                        if (selectedPin)
                        {
                            muxSetting = parseInt(selectedPin.muxOptions[0].mode, 10);
                        }

                        return muxSetting;
                    }
                },
                {
                    name: "uartRXPin",
                    displayName: "UART RX Pin",
                    description: "Select the UART RX pin",
                    hidden: false,
                    default: defaultBSLUARTRX,
                    options: createUARTRXPinOptions,
                },
                {
                    // .interfacePins.UART_RXD_pad_num
                    name: "uartRXPadNum",
                    displayName: "UART RX Pad Number",
                    description: "Pad number represents the IOMUX PINCM value",
                    default: 0,
                    getValue: (inst) => {
                        let selectedPin = uartRXPinData.find(pin => pin.designSignalName == inst.uartRXPin);
                        let iomuxPINCM = 0;

                        if (selectedPin)
                        {
                            /* Convert to integer with radix 10 */
                            iomuxPINCM = parseInt(selectedPin.attributes.iomux_pincm, 10);
                        }

                        return iomuxPINCM;
                    }
                },
                {
                    // .interfacePins.UART_RXD_PF_mux_sel
                    name: "uartRXMux",
                    displayName: "UART RX Mux",
                    description: "The mux value represents the pin function",
                    default: 0,
                    getValue: (inst) => {
                        let selectedPin = uartRXPinData.find(pin => pin.designSignalName == inst.uartRXPin);
                        let muxSetting = 0;

                        if (selectedPin)
                        {
                            muxSetting = parseInt(selectedPin.muxOptions[0].mode, 10);
                        }

                        return muxSetting;
                    }
                },
            ],)
        },
        {
            name: "GROUP_BSL_I2C_PIN_CONFIG",
            displayName: "BSL I2C Pin Configuration",
            description: "",
            config: [
                {
                    name: "i2cPeripheral",
                    displayName: "I2C Peripheral",
                    description: "Select the I2C peripheral",
                    hidden: false,
                    /* BSL I2C instance is tied to I2C0 */
                    readOnly: true,
                    default: "I2C0",
                },
                {
                    name: "i2cSCLPin",
                    displayName: "I2C SCL Pin",
                    description: "Select the I2C SCL pin",
                    hidden: false,
                    default: defaultBSLI2CSCL,
                    options: createI2CSCLPinOptions,
                },
                {
                    // .interfacePins.I2C_SCL_pad_num
                    name: "i2cSCLPadNum",
                    displayName: "I2C SCL Pad Number",
                    description: "Pad number represents the IOMUX PINCM value",
                    default: 0,
                    getValue: (inst) => {
                        let selectedPin = i2cSCLPinData.find(pin => pin.designSignalName == inst.i2cSCLPin);
                        let iomuxPINCM = 0;

                        if (selectedPin)
                        {
                            /* Convert to integer with radix 10 */
                            iomuxPINCM = parseInt(selectedPin.attributes.iomux_pincm, 10);
                        }

                        return iomuxPINCM;
                    }
                },
                {
                    // .interfacePins.I2C_SCL_PF_mux_sel
                    name: "i2cSCLMux",
                    displayName: "I2C SCL Mux",
                    description: "The mux value represents the pin function",
                    default: 0,
                    getValue: (inst) => {
                        let selectedPin = i2cSCLPinData.find(pin => pin.designSignalName == inst.i2cSCLPin);
                        let muxSetting = 0;

                        if (selectedPin)
                        {
                            muxSetting = parseInt(selectedPin.muxOptions[0].mode, 10);
                        }

                        return muxSetting;
                    }
                },
                {
                    name: "i2cSDAPin",
                    displayName: "I2C SDA Pin",
                    description: "Select the I2C SDA pin",
                    hidden: false,
                    default: defaultBSLI2CSDA,
                    options: createI2CSDAPinOptions,
                },
                {
                    // .interfacePins.I2C_SDA_pad_num
                    name: "i2cSDAPadNum",
                    displayName: "I2C SDA Pad Number",
                    description: "Pad number represents the IOMUX PINCM value",
                    default: 0,
                    getValue: (inst) => {
                        let selectedPin = i2cSDAPinData.find(pin => pin.designSignalName == inst.i2cSDAPin);
                        let iomuxPINCM = 0;

                        if (selectedPin)
                        {
                            /* Convert to integer with radix 10 */
                            iomuxPINCM = parseInt(selectedPin.attributes.iomux_pincm, 10);
                        }

                        return iomuxPINCM;
                    }
                },
                {
                    // .interfacePins.I2C_SDA_PF_mux_sel
                    name: "i2cSDAMux",
                    displayName: "I2C SDA Mux",
                    description: "The mux value represents the pin function",
                    default: 0,
                    getValue: (inst) => {
                        let selectedPin = i2cSDAPinData.find(pin => pin.designSignalName == inst.i2cSDAPin);
                        let muxSetting = 0;

                        if (selectedPin)
                        {
                            muxSetting = parseInt(selectedPin.muxOptions[0].mode, 10);
                        }

                        return muxSetting;
                    }
                },
                {
                    // .i2cSlaveAddress
                    name: "i2cSlaveAddress",
                    displayName: "I2C Target Address (7-bit)",
                    default: 0x0048,
                    displayFormat: "hex",
                    range: [0, 0x007F],
                },
            ],
        },
    ];
}

let extendedConfigBSLFlashPlugins = [];
if (deviceOptions.BSL_SUPPORT_FLASH_PLUGIN == true)
{
    extendedConfigBSLFlashPlugins = [
        {
            name: "GROUP_BSL_PLUGIN_CONFIG",
            displayName: "BSL Plugin Configuration",
            description: "",
            config: [
                {
                    // .flashPluginEnable
                    /* BSLPLUGINCFG.PLUGINEXISTS */
                    name        : "bslFlashPluginEnable",
                    displayName : "BSL Flash Plugin Enable",
                    description : "Enables use of Flash or ROM plugin",
                    longDescription: `
    The BSL supports MAIN flash plugins to enable additional interfaces beyond
    UART and I2C. When this feature is enabled, the BSL core will call the
    corresponding plugin functions (Init, Receive, Transmit and De-Init).

    The MSPM0 SDK includes code examples illustrating the implementation of plugins.`,
                    hidden      : false,
                    default     : false,
                    onChange    : onChangePluginConfig,
                },
                {
                    // .pluginType
                    /* BSLPLUGINCFG.PLUGINTYPE */
                    name        : "bslPluginType",
                    displayName : "BSL Plugin Type",
                    description : "",
                    longDescription: ``,
                    hidden      : true,
                    default     : "bslPluginAny",
                    options:    [
                        {name: "bslPluginUART", displayName: "Plugin is for UART"},
                        {name: "bslPluginI2C", displayName: "Plugin is for I2C"},
                        {name: "bslPluginAny", displayName: "Any other interface with valid hooks"},
                    ],
                },
                {
                    // .pluginSramSize
                    /* BSL Plugin SRAM size */
                    name: "bslPluginSRAMSize",
                    displayName: "Plugin SRAM Size",
                    description: "SRAM memory consumed by the flash plugin",
                    longDescription: `
    SRAM memory consumed by flash plugin. The size should be derived from the .map file
    output of the flash plugin code, primarily the .data and .bss sections.

    The methods to calculate the SRAM memory size in different IDEs are listed below:

    **CCS**:
    - Build the project in CCS
    - Navigate to the Debug directory in the project
    - Open the .map file, and find the .bss and .data sections
    - Add the section lengths together to get the plugin SRAM size
    - Input the calculated value into SysConfig
    - Save the changes, and rebuild the project

    **IAR**:
    - Build the project in IAR
    - Navigate to the Output directory in the project
    - Open the .map file, and find the .bss and .data sections
    - Add the section lengths together to get the plugin SRAM size
    - Input the calculated value into SysConfig
    - Save the changes, and rebuild the project

    **Keil**:
    - Build the project in Keil
    - Right-click the project, select "Open Map File", and find the .bss and .data sections
    - Add the section lengths together to get the plugin SRAM size
    - Input the calculated value into SysConfig
    - Save the changes, and rebuild the project
    `,
                    hidden: true,
                    default: 0xFF,
                    displayFormat: "hex",
                    range: [0, 0xFF],
                },
                {
                    // .pluginHook[0]
                    /* BSLPLUGINHOOK[0] */
                    name        : "bslPluginHookInit",
                    displayName : "Function Pointer for Plugin Init",
                    description : "Flash plugin init function",
                    longDescription: ``,
                    hidden      : true,
                    default: "",
                    onChange: (inst, ui) => {
                        ui.bslPluginHookInitAddress.hidden = !(inst.bslPluginHookInit.length > 0);
                    }
                },
                {
                    /* Allow user to customize function pointer address */
                    name        : "bslPluginHookInitAddress",
                    displayName : "Plugin Init Address",
                    description : "Address to the flash plugin init function",
                    longDescription: ``,
                    hidden      : true,
                    default: 0xFFFFFFFF,
                    displayFormat: "hex",
                    range: [0, 0xFFFFFFFF],
                },
                {
                    // .pluginHook[1]
                    /* BSLPLUGINHOOK[1] */
                    name        : "bslPluginHookReceive",
                    displayName : "Function Pointer for Plugin Receive",
                    description : "Flash plugin receive function",
                    longDescription: ``,
                    hidden      : true,
                    default: "",
                    onChange: (inst, ui) => {
                        ui.bslPluginHookReceiveAddress.hidden = !(inst.bslPluginHookReceive.length > 0);
                    }
                },
                {
                    /* Allow user to customize function pointer address */
                    name        : "bslPluginHookReceiveAddress",
                    displayName : "Plugin Receive Address",
                    description : "Address to the flash plugin receive function",
                    longDescription: ``,
                    hidden      : true,
                    default: 0xFFFFFFFF,
                    displayFormat: "hex",
                    range: [0, 0xFFFFFFFF],
                },
                {
                    // .pluginHook[2]
                    /* BSLPLUGINHOOK[2] */
                    name        : "bslPluginHookTransmit",
                    displayName : "Function Pointer for Plugin Transmit",
                    description : "Flash plugin transmit function",
                    longDescription: ``,
                    hidden      : true,
                    default: "",
                    onChange: (inst, ui) => {
                        ui.bslPluginHookTransmitAddress.hidden = !(inst.bslPluginHookTransmit.length > 0);
                    }
                },
                {
                    /* Allow user to customize function pointer address */
                    name        : "bslPluginHookTransmitAddress",
                    displayName : "Plugin Transmit Address",
                    description : "Address to the flash plugin transmit function",
                    longDescription: ``,
                    hidden      : true,
                    default: 0xFFFFFFFF,
                    displayFormat: "hex",
                    range: [0, 0xFFFFFFFF],
                },
                {
                    // .pluginHook[3]
                    /* BSLPLUGINHOOK[3] */
                    name        : "bslPluginHookDeInit",
                    displayName : "Function Pointer for Plugin De-Init",
                    description : "Flash plugin de-init function",
                    longDescription: ``,
                    hidden      : true,
                    default: "",
                    onChange: (inst, ui) => {
                        ui.bslPluginHookDeInitAddress.hidden = !(inst.bslPluginHookDeInit.length > 0);
                    }
                },
                {
                    /* Allow user to customize function pointer address */
                    name        : "bslPluginHookDeInitAddress",
                    displayName : "Plugin De-Init Address",
                    description : "Address to the flash plugin de-init function",
                    longDescription: ``,
                    hidden      : true,
                    default: 0xFFFFFFFF,
                    displayFormat: "hex",
                    range: [0, 0xFFFFFFFF],
                },
            ],
        }
    ];
}

let extendedConfigBSLAdvanced = [];
if (deviceOptions.SUPPORT_ROM_BSL == true)
{
    extendedConfigBSLAdvanced = [
        {
            // .appRev
            /* BSLAPPVER */
            name        : "bslAppVersion",
            displayName : "BSL App Version",
            description : "BSL App Version",
            longDescription: `
The BSL supports returning an application version number through the BSL serial
interface. This allows the BSL host to interrogate the firmware version without
being able to read the firmware. The version field is 32 bits in length.

To link the application version command to a version number programmed in the
main flash, program the address of the version number in this field.

The version data is only returned if the specified address corresponds to a
valid flash memory address.
`,
            hidden      : false,
            displayFormat: "hex",
            default     : 0xFFFFFFFF,
            range       : [0, 0xFFFFFFFF]
        },
        {
            // .memoryRead
            /* BSLCFG.READOUTEN */
            name        : "bslEnableReadOut",
            displayName : "BSL Read Out Enable",
            description : "Enables memory read-out via BSL",
            longDescription: `
The BSL can be configured to allow read-out of memory locations through the BSL
serial interface. The default BSL configuration on new devices does not enable
read-out.
`,
            hidden      : false,
            default     : false,
        },
        {
            // .securityAlert
            /* BSLSECCFG */
            name        : "bslSecurityConfig",
            displayName : "BSL Security Alert Configuration",
            description : "BSL Security Alert Configuration",
            longDescription: `
A brute force attack security alert occurs if three consecutive invalid
passwords are provided to the BSL in an attempt to gain access to the BSL. The
BSL can be configured to respond to the security alert in one of the following
ways:

* Issue a factory reset where MAIN and NONMAIN flash regions are erased. Only
memory regions which are not protected by static write protection settings will
be erased/reset.

* Disable the bootloader (leaves MAIN intact but reconfigures NONMAIN to
block BSL access). This option is only available if NONMAIN is not statically
write protected.

* Ignore (do not modify the configuration and allow password attempts to
continue).
`,
            hidden      : false,
            default     : "ignoreAlert",
            options:    [
                {name: "triggerFactoryReset", displayName: "Trigger a factory reset"},
                {name: "disableBSL", displayName: "Disable BSL"},
                {name: "ignoreAlert", displayName: "Ignore security alert"},
            ],
        },
    ];
}

if (deviceOptions.SUPPORT_BSL == true)
{
    nonMainConfig = nonMainConfig.concat([
        /****** BSL CONFIGURATION *******/
        {
            name: "GROUP_BSL",
            displayName: "Bootstrap Loader (BSL) Configuration",
            description: "",
            longDescription: `
    The BSL provides a method to program and/or verify the device memory through a
    standard serial interface (UART or I2C).
    Key features of the BSL which are accessible through the serial interface include:

    * Programming and erase of flash memory
    * Ability to return a firmware version number through a pointer to the MAIN flash
    * Ability to specify a hardware invoke GPIO
    * Ability to enable code/data read-out (disabled by default)
    * Ability to return a 32-bit CRC of a code/data region (1kB minimum region size)
    to verify programming
    * Access is always protected with a 256-bit password
    * Configurable security alert handling for resisting brute force attacks
    * Support for MAIN flash plugins to enable additional interfaces beyond UART and I2C

    For a complete description of the BSL features, see the BSL User's Guide.

    The BSL can be completely disabled if desired by properly configuring the BSL
    mode in the BCR configuration in NONMAIN flash.
    `,
            collapsed: true,
            config: extendedConfigBSLPW.concat([
                {
                    name: "GROUP_BSL_GPIO_INVOKE_CONFIG",
                    displayName: "BSL GPIO Invoke Configuration",
                    description: "",
                    config: [
                        {
                            // .bslPinInvokeEnable
                            /* BOOTCFG0.BSLIVKPINEN */
                            name        : "bslInvokePinCheck",
                            displayName : "Enable BSL Invoke Pin Check",
                            description : "Enable BSL Invoke Pin Check",
                            longDescription: ``,
                            hidden      : false,
                            default     : true,
                            onChange    : updateGUIGPIOInvoke,
                        },
                        {
                            name        : "bslDefaultInvokePin",
                            displayName : "Use Default BSL Invoke Pin",
                            description : "Use Default BSL Invoke Pin",
                            longDescription: `
    Refer to the device datasheet for details on the default BSL invoke pin.
    `,
                            hidden      : false,
                            default     : true,
                            onChange    : updateGUIGPIOInvoke,
                        },
                        {
                            /* BSLCFG.BSLIVK_GPIOPORT. BSLCFG.BSLIVK_GPIOPIN */
                            name        : "bslInvokePin",
                            displayName : "BSL Invoke Pin",
                            description : "Select BSL Invoke Pin",
                            longDescription: ``,
                            hidden      : true,
                            default     : filteredPinOptions[0].name,
                            options     : filteredPinOptions,
                        },
                        {
                            /* BSLCFG.BSLIVK_PINCM */
                            name        : "bslInvokePINCM",
                            displayName : "BSL Invoke Pin PINCM",
                            description : "BSL Invoke Pin PINCM",
                            longDescription: ``,
                            hidden      : true,
                            readOnly    : true,
                            default     : "1",
                            getValue    : (inst) => {
                                let matchPinCM = iomuxPincmOptions.find(x => x.name.includes(inst.bslInvokePin)).num;
                                /*
                                 * Handle case of dual purpose pins. This
                                 * assumes that the first function is used for
                                 * BSL GPIO invoke
                                 */
                                return ((matchPinCM.split(","))[0]);
                            }
                        },
                        {
                            /* BSLCFG.BSLIVK_LVL */
                            name        : "bslInvokePinLevel",
                            displayName : "BSL Invoke Pin Level",
                            description : "BSL Invoke Pin Level",
                            longDescription: ``,
                            hidden      : true,
                            default     : 0,
                            options:    [
                                {name: 0, displayName: "Low"},
                                {name: 1, displayName: "High"},
                            ],
                        },
                        {
                            // .pin.pinData0
                            /* For background calculation only */
                            name        : "bslInvokePinData0",
                            displayName : "BSL Invoke Pin Details: Data0",
                            description : "BSL Invoke Pin Details: Data0",
                            longDescription: ``,
                            hidden      : true,
                            readOnly    : true,
                            displayFormat: "hex",
                            default     : 0,
                            getValue    : (inst) => calculatePinData0(inst),
                        },
                        {
                            // .pin.pinData1
                            /* For background calculation only */
                            name        : "bslInvokePinData1",
                            displayName : "BSL Invoke Pin Details: Data1",
                            description : "BSL Invoke Pin Details: Data1",
                            longDescription: ``,
                            hidden      : true,
                            readOnly    : true,
                            displayFormat: "hex",
                            default     : 0,
                            getValue    : (inst) => calculatePinData1(inst),
                        },
                    ],
                },
            ]).concat(extendedConfigROMBSL).concat(extendedConfigBSLFlashPlugins).concat([
                {
                    name: "GROUP_BSL_ALT_CONFIG",
                    displayName: "Alternate BSL Configuration",
                    description: "",
                    config: [
                        {
                            // .BSLAlternateConfig
                            /* BSLALTCFG */
                            name        : "bslAltConfig",
                            displayName : "Use Alternate BSL Configuration",
                            description : "Selects whether to use alternate BSL config",
                            longDescription: `
    The MSPM0 bootcode can jump to an alternate BSL if enabled, allowing developers
    to implement custom bootloaders in MAIN flash. The specified address of the
    alternate BSL must be valid.

    The MSPM0 SDK includes code examples illustrating the implementation of alternate bootloaders.
    `,
                            hidden      : false,
                            default     : false,
                            onChange    : onChangeAltBSLConfig,
                        },
                        {
                            // .BSLAlternateAddr
                            /* BSLALTADDR */
                            name        : "bslAltAddress",
                            displayName : "Alternate BSL Address",
                            description : "Alternate BSL Address",
                            longDescription: `
    The alternate BSL address should be the address of the first function to get
    executed in the secondary BSL code. This function should be placed in a fixed
    location. Refer to the secondary_bsl SDK example for more details.

    Note, Cortex-M type ARM processors only support thumb mode. Any pointers
    to functions must have the LSB set. **Therefore, when inputting the address of the
    function pointer into SysConfig, set the LSB to 1.**

    For example, in the secondary_bsl SDK example, the alternate BSL address is
    pointing to Reset_Handler, which is placed at 0x00001040. However, due to thumb
    mode, the alternate BSL address is actually located at 0x00001041.
    `,
                            displayFormat: "hex",
                            hidden      : true,
                            default     : 0xFFFFFFFF,
                            range       : [0, 0xFFFFFFFF],
                        },
                    ],
                },
                {
                    // .configID
                    /* BSLID */
                    name        : "bslConfigID",
                    displayName : "BSL Configuration ID",
                    description : "Predetermined Config Signature Magic ID",
                    longDescription: ``,
                    hidden      : false,
                    readOnly    : true,
                    displayFormat: "hex",
                    default     : deviceOptions.BSL_CONFIG_ID,
                },
            ]).concat(extendedConfigBSLAdvanced).concat(bslExtendedConfigNRST).concat([
                {
                    // .userCfgCRC
                    /* BSLCRC */
                    name        : "bslCRC",
                    displayName : "Expected BSL Configuration CRC",
                    description : "The expected CRC value to test against during boot",
                    longDescription: ``,
                    hidden      : false,
                    displayFormat: "hex",
                    default     : 0,
                    getValue    : (inst) =>  {
                        if (Common.isDeviceFamily_PARENT_MSPM0H321X() ||
                            Common.isDeviceFamily_PARENT_MSPM0C1105_C1106())
                        {
                            return calculateBSLCRC_NoFlash_16Bit(inst);
                        }
                        else
                        {
                            return calculateBSLCRC_32Bit(inst);
                        }
                    },
                },
                {
                    name        : "bslCRCString",
                    /* Test and debug only. Used for crc output .txt */
                    hidden      : true,
                    default     : "",
                    getValue    : (inst) =>  {
                        if (Common.isDeviceFamily_PARENT_MSPM0H321X() ||
                            Common.isDeviceFamily_PARENT_MSPM0C1105_C1106())
                        {
                            return createNoFlashBSLConfigString(inst);
                        }
                        else
                        {
                            return createROMBSLString(inst);
                        }
                    },
                },
            ])
        },
    ]);
};

function calculatePinData0(inst)
{
    let pinData0;

    if (!inst.bslDefaultInvokePin)
    {
        pinData0 = (inst.bslInvokePINCM) | (inst.bslInvokePinLevel << 7);
    }
    else
    {
        /* Default pin level is high */
        pinData0 = (deviceOptions.BSL_INVOKE_PIN_PINCM) | (1 << 7);
    }

    return (pinData0);
}

function calculatePinData1(inst)
{
    let pinData1;
    let pin;
    let gpioBaseIndex;

    if (!inst.bslDefaultInvokePin)
    {
        pin = inst.bslInvokePin;
    }
    else
    {

        pin = iomuxPincmOptions.find(x => isStandaloneNumberPresent(deviceOptions.BSL_INVOKE_PIN_PINCM, x.num)).name;
    }

    /* Note: This assumes that BSL invoke pin will never be on GPIOC */
    gpioBaseIndex = (Common.getGPIOPort(pin) == "GPIOA") ? 0 : 1;
    pinData1 = Common.getGPIONumber(pin) | (gpioBaseIndex << 5);

    return (pinData1);
}

function createUARTTXPinOptions(inst, ui)
{
    let pinOptions = [];

    for (let i in uartTXPinData)
    {
        let peripheralPinName = uartTXPinData[i].muxOptions[0].peripheralPin.name;

        if (peripheralPinName.match(inst.uartPeripheral + ".TX"))
        {
            pinOptions.push(
                {name: uartTXPinData[i].designSignalName}
            )
        }
    }

    return pinOptions;
}

function createUARTRXPinOptions(inst, ui)
{
    let pinOptions = [];

    for (let i in uartRXPinData)
    {
        let peripheralPinName = uartRXPinData[i].muxOptions[0].peripheralPin.name;

        if (peripheralPinName.includes(inst.uartPeripheral + ".RX"))
        {
            pinOptions.push(
                {name: uartRXPinData[i].designSignalName}
            )
        }
    }

    return pinOptions;
}

function createI2CSCLPinOptions(inst, ui)
{
    let pinOptions = [];

    for (let i in i2cSCLPinData)
    {
        let peripheralPinName = i2cSCLPinData[i].muxOptions[0].peripheralPin.name;

        if (peripheralPinName.match(inst.i2cPeripheral + ".SCL"))
        {
            pinOptions.push(
                {name: i2cSCLPinData[i].designSignalName}
            )
        }
    }

    return pinOptions;
}

function createI2CSDAPinOptions(inst, ui)
{
    let pinOptions = [];

    for (let i in i2cSDAPinData)
    {
        let peripheralPinName = i2cSDAPinData[i].muxOptions[0].peripheralPin.name;

        if (peripheralPinName.includes(inst.i2cPeripheral + ".SDA"))
        {
            pinOptions.push(
                {name: i2cSDAPinData[i].designSignalName}
            )
        }
    }

    return pinOptions;
}

/*
 *  ======== devSpecific ========
 *  Device-specific extensions to be added to base module configuration
 */
let devSpecific = {

    /* This is where you should be putting all of your configurable information.
    If you want a configurable or variable that is not visible, you can make it
    hidden
     */

    config: nonMainConfig,

    pinmuxRequirements: pinmuxRequirements,

    filterHardware: filterHardware,

    validate: validateNONMAIN,

    _getPinResources: _getPinResources,
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
    let result = Object.assign({}, base.moduleStatic, devSpecific);

    /* concatenate device-specific configs */
    // moduleStatic specific to NONMAIN as it's statically defined
    result.config = base.moduleStatic.config.concat(devSpecific.config);
    base.moduleStatic = result;

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
