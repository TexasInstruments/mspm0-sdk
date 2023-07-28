/*
 * Copyright (c) 2021, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== driverlib.component.js ========
 */

"use strict";

/* get ti/drivers common utility functions */
let Common = system.getScript("/ti/driverlib/Common.js");

let topModules;
let displayName = "MSPM0 Driver Library";
let description = "MSPM0 DriverLib System Configuration";
/* universal modules */
let systemModulesList = [
    "/ti/driverlib/SYSCTL",
    "/ti/driverlib/Board",
    "/ti/driverlib/DMA",
    "/ti/driverlib/GPIO",
    "/ti/driverlib/SYSTICK",
    "/ti/driverlib/WWDT",
    "/ti/driverlib/NONMAIN",
];
/* MSPM0Gxx-specific modules */
if(Common.isDeviceM0G()){
    systemModulesList.push(
        "/ti/driverlib/MATHACL",
        "/ti/driverlib/RTC",
    );
};

let timerModulesList = [
    "/ti/driverlib/PWM",
    "/ti/driverlib/TIMER",
    "/ti/driverlib/CAPTURE",
    "/ti/driverlib/COMPARE",
];
/* MSPM0Gxx-specific modules */
if(Common.isDeviceM0G()){
    timerModulesList.push(
        "/ti/driverlib/QEI",
        "/ti/driverlib/TIMERFault",
    );
};

let commModulesList = [
    "/ti/driverlib/UART",
    "/ti/driverlib/uartLIN",
    "/ti/driverlib/I2C",
    "/ti/driverlib/i2cSMBUS",
    "/ti/driverlib/SPI",
];

let analogModulesList = [
    "/ti/driverlib/ADC12",
    "/ti/driverlib/VREF",
    "/ti/driverlib/GPAMP",
];
/* add device-specific modules - not available on MSPM0G110x, MSPM0L110X */
if((Common.isDeviceM0G() || Common.isDeviceM0L()) && !Common.isDeviceM0x110x()){
    analogModulesList.push(
        "/ti/driverlib/COMP",
    );
};
/* add device-specific modules - not available on MSPM0G110x or MSPM0L110X */
if((Common.isDeviceM0G() || Common.isDeviceFamily_PARENT_MSPM0L11XX_L13XX()) && !Common.isDeviceM0x110x()){
    analogModulesList.push(
    "/ti/driverlib/OPA",
);
};
// TODO: confirm device list for DAC
/* MSPM0Gxx-specific modules - not available for MSPM0G310X, MSPM0G110x */
if(["MSPM0G350X","MSPM0G150X"].includes(system.deviceData.device)){
    analogModulesList.push(
        "/ti/driverlib/DAC12",
    );
};

/* MSPM0Gxx-specific modules - not available for MSPM0G150X, MSPM0G110x */
if(["MSPM0G350X","MSPM0G310X"].includes(system.deviceData.device)){
    commModulesList.push(
        "/ti/driverlib/MCAN",
    );
};

let securityModulesList = [
]
/* MSPM0Gxx-specific modules - not available for MSPM0G110x */
if(Common.isDeviceM0G() && !Common.isDeviceM0x110x()){
    securityModulesList.push(
        "/ti/driverlib/AES",
        "/ti/driverlib/TRNG",
    );
};

let dataIntegrityModulesList = []
/* add device-specific modules - not available on A2LCD */
if(Common.isDeviceM0G() || Common.isDeviceFamily_PARENT_MSPM0L11XX_L13XX()){
    dataIntegrityModulesList.push(
        "/ti/driverlib/CRC",
    );
};

let readOnlyModulesList = [
    "/ti/driverlib/EVENT",
];

topModules = [
    {
        displayName: displayName,
        description: description,
        categories: [
            {
                displayName: "System",
                modules: systemModulesList
            },
            {
                displayName: "Analog",
                modules: analogModulesList
            },
            {
                displayName: "Communications",
                modules: commModulesList
            },
            {
                displayName: "Timers",
                modules: timerModulesList
            },
            {
                displayName: "Security",
                modules: securityModulesList
            },
            {
                displayName: "Data Integrity",
                modules: dataIntegrityModulesList
            },
            {
                displayName: "Read-Only",
                modules: readOnlyModulesList
            }
        ]
    },
];


let templates = [
    {
        "name": "/ti/driverlib/templates/Board.c.xdt",
        "outputPath": "ti_msp_dl_config.c",
        "alwaysRun": false
    },
    {
        "name": "/ti/driverlib/templates/Board.h.xdt",
        "outputPath": "ti_msp_dl_config.h",
        "alwaysRun": false
    },
    {
        "name": "/ti/driverlib/templates/peripheralPinAssignments.txt.xdt",
        "outputPath": "peripheralPinAssignments.txt",
        "alwaysRun": false
    },
    {
        "name": "/ti/driverlib/templates/EVENT.dot.xdt",
        "outputPath": "Event.dot",
        "alwaysRun": false
    },
    /* TODO: Future enhancement: remove nonmain from driverlib area */
    {
        "name": "/ti/driverlib/nonmain/NonMain.c.xdt",
        "outputPath": "boot_config.c",
        "alwaysRun": false
    },
    {
        "name": "/ti/driverlib/nonmain/NonMain.h.xdt",
        "outputPath": "boot_config.h",
        "alwaysRun": false
    },
];

exports = {
    displayName: displayName,
    topModules: topModules,
    templates: templates
};
