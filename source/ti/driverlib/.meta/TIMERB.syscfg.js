/*
 *  ======== TIMERB.syscfg.js ========
 */
"use strict";

/* get Common /ti/driverlib utility functions */
let Common = system.getScript("/ti/driverlib/Common.js");

/*
 *  ======== validate ========
 *  Validate this inst's configuration
 *
 *  param inst       - module instance to be validated
 *  param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    // Common.validateNames(inst, validation);
}

/*
 *  ======== _getPinResources ========
 */
/* istanbul ignore next */
function _getPinResources(inst)
{
    return;
}

/*
 *  ======== exports ========
 */
let base = {
    displayName: "TIMER - BASIC",
    description: "Basic Timer module for MSPM0",
    defaultInstanceName: "TIMERB",
    longDescription: `
The **Basic Timer (TIMERB) module** is an array of counters which can generate
periodic interrupts, be concatenated to create longer time durations, and measure
timing properties of external events.

Under *Basic Configuration* users can:
- Configure the clock source and frequency
- Configure individual counter settings
- Enable specific interrupts for each counter

The TIMERB module provides multiple interrupt sources which can be configured to
source CPU interrupt events. These interrupts include counter overflow, start, and
stop events for each counter.

Check the datasheet of your selected device for more detailed information.
    `,
    validate: validate,
    modules: Common.autoForceModules(["SYSCTL", "Board", "EVENT"]),
    templates: {
        boardc: "/ti/driverlib/timerb/TIMERB.Board.c.xdt",
        boardh: "/ti/driverlib/timerb/TIMERB.Board.h.xdt"
    },
    _getPinResources: _getPinResources,
};

/* get device-specific TIMERB module */
let deviceTIMERB = system.getScript("/ti/driverlib/timerb/TIMERBMSPM0");
exports = deviceTIMERB.extend(base);
