/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== tinyusb.component.js ========
 */

"use strict";

let peripherals = Object.keys(system.deviceData.peripherals);

/* get ti common utility functions */
let Common = system.getScript("/ti/driverlib/Common.js");

let topModules;
let alwaysPresent;
let displayName = "TinyUSB Configuration";
let description = "TinyUSB Configuration";

let migrationModules = [];
let usbTestList = [];
if(/USB/.test(peripherals)){
    migrationModules.push(
        "/ti/tinyusb_meta/tinyusb",
    )
    usbTestList.push(
        // "/ti/tinyusb_meta/endpoint_in/EP_IN",
        // "/ti/tinyusb_meta/endpoint_out/EP_OUT",
        // "/ti/tinyusb_meta/tinyusb/USB_devAudio",
        // "/ti/tinyusb_meta/tinyusb/USB_devCDC",
        // "/ti/tinyusb_meta/tinyusb/USB_devHID",
        // "/ti/tinyusb_meta/tinyusb/USB_devMSC",
        // "/ti/tinyusb_meta/tinyusb/USB_hostCDC",
        // "/ti/tinyusb_meta/tinyusb/USB_hostHID",
        // "/ti/tinyusb_meta/tinyusb/USB_hostMSC",
    )
}


topModules = [
    {
        displayName: displayName,
        description: description,
        modules: migrationModules,
        categories: [
            {
                displayName: "Testing Dependencies",
                modules: usbTestList
            },
        ]
    },
];


let templates = [
    /* USB Templates */
    {
        "name": "/ti/tinyusb_meta/tinyusb/tusb_config.h.xdt",
        "outputPath": "tusb_config.h",
        "alwaysRun": false
    },
    {
        "name": "/ti/tinyusb_meta/tinyusb/usb_descriptors.c.xdt",
        "outputPath": "usb_descriptors.c",
        "alwaysRun": false
    },
    {
        "name": "/ti/tinyusb_meta/tinyusb/usb_descriptors.h.xdt",
        "outputPath": "usb_descriptors.h",
        "alwaysRun": false
    },
];

exports = {
    displayName: displayName,
    topModules: topModules,
    templates: templates,
};
