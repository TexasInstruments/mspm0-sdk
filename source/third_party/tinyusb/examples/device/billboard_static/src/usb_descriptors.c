/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "bsp/board_api.h"
#include "tusb.h"
#include "usb_descriptor.h"

/* 
 * A combination of interfaces must have a unique product id, since PC will save device driver after the first plug.
 * Same VID/PID with different interface e.g MSC (first), then CDC (later) will possibly cause system error on PC.
 *
 * Auto ProductID layout's Bitmap:
 *   [MSB]     AUDIO | MIDI | HID | MSC | CDC          [LSB]
 */
#define _PID_MAP(itf, n)  ( (CFG_TUD_##itf) << (n) )
#define USB_PID           (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
                           _PID_MAP(MIDI, 3) | _PID_MAP(AUDIO, 4) | _PID_MAP(VENDOR, 5) )

/* MSPM0 product VID */
#define USB_VID   0x2047
#define USB_BCD   0x0201

//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device =
{
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    /* BCD field for Billboard must be set to 0x0201 minimum */
    .bcdUSB             = USB_BCD,
    /* Billboard class value is 0x11 */
    .bDeviceClass       = TUSB_CLASS_BILLBOARD,
    /* SubClass/Protocol value is 0x00 */
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = USB_VID,
    .idProduct          = USB_PID,
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};

/* Verify that the data struct is of length 18 */
TU_VERIFY_STATIC((sizeof(desc_device)) == sizeof(tusb_desc_device_t),
                  "Device descriptor is not 18-bytes");

/*
 * Invoked when GET DEVICE DESCRIPTOR is received
 * Application return pointer to descriptor
 */
uint8_t const * tud_descriptor_device_cb(void)
{
  return (uint8_t const *) &desc_device;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+

enum
{
  ITF_NUM_BILLBOARD = 0,
  ITF_NUM_TOTAL
};

/* Length of configuration descriptor + billboard descriptor */
#define CONFIG_TOTAL_LEN    (TUD_CONFIG_DESC_LEN + TUD_BILLBOARD_DESC_LEN)

uint8_t const desc_fs_configuration[] =
{
  /* Config number, interface count, string index, total length, attribute, power in mA */
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),

  /*
   * Descriptor Type: Interface
   * Input: String index describing this interface
   */
  TUD_BILLBOARD_DESC_DESCRIPTOR(4)
};

/* Total length of Configuration and Interface descriptor must be 18-bytes */
TU_VERIFY_STATIC((CONFIG_TOTAL_LEN == sizeof(desc_fs_configuration)),
                  "Total length of configuration descriptor is wrong");

/*
 * Invoked when received GET CONFIGURATION DESCRIPTOR
 * Application return pointer to descriptor
 * Descriptor contents must exist long enough for transfer to complete
 */
uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
  /* for multiple configurations */
  (void) index;
  return desc_fs_configuration;
}

//--------------------------------------------------------------------+
// BOS Descriptor
//--------------------------------------------------------------------+
/* BOS and Capability Descriptor is required for Billboard */
uint8_t displayport_desc_bos[] =
{
  /*
   * Descriptor Type: BOS Descriptor
   * Length: 5 bytes
   * Input: wTotalLength ->   Total length of all descriptors in BOS
   *        bNumDeviceCaps -> Number of separate device capability descriptors in BOS.
   *                          This example uses the value of 3 as it contains a Billboard
   *                          Capability, Container ID, and AUM capability descriptor.
   */
  TUD_BOS_DESCRIPTOR(BOS_TOTAL_LEN, TUD_TOTAL_CAP_COUNT),

  /*
   * Descriptor Type: Container ID
   * Length: 20 bytes
   * Input: UUID -> For this example a generic one will be utilized
   */
  TUD_BILLBOARD_CONTAINER_ID_DESCRIPTOR(TUD_BOS_CONTAINER_EXAMPLE_UUID),

  /*
   * Descriptor Type: Capability Descriptor
   * Length: 44 + (4 * # of AUM Capability descriptors) bytes
   * Input: iAdditionalInfoURL            |   Index of string descriptor containing URL to get more details on product and various AUM it supports.
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bNumberOfAlternateOrUSB4Modes |  # of AUM supported, maximum value of this field is MAX_NUM_ALT_OR_USB_4_MODE (0x34h).
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bPreferredAlternateOrUSB4Mode |  Index of preferred AUM, field shall be set to 0xFF to indicate no pref.
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        VCONN Power                   |  Bitmap: [0-2]  Power needed by adapter for full function
   *                                      |          [3-14] Reserved, shall be set to 0
   *                                      |          [15]   Adapter does not require any Vconn Power. If set, bits [0-2] will be ignored
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bmConfigured                  |  Bitmap: Each bit pair represents state of the AUM identified in wSVID[n] & bAlternateOrUSB4Mode[n]
   *                                      |
   *                                      |  |----------------------------------------------------------------------|
   *                                      |  |Value | Description                                                   |
   *                                      |  |----------------------------------------------------------------------|
   *                                      |  |00b   | Unspecified Error                                             |
   *                                      |  |01b   | AUM configuration not attempted or exited                     |
   *                                      |  |10b   | AUM configuration attempted but not succesful and not entered |
   *                                      |  |11b   | AUM configuration succesful                                   |
   *                                      |  |______________________________________________________________________|
   *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bcdVersion                    |  Billboard capability descriptor version number in Binary-Coded Decimal (e.g, 1.10 is 0x0110h).
   *                                      |  This field identifies which version of the Billboard spec implementation is compliant with.
   *                                      |  If 0x0000h then it follows first revision of Billboard spec, for this example it was implemented
   *                                      |  following v1.2.2 of Billboard spec, hence input will be 0x0122h.
   *                                      |  Note: 0x120h is reserved and should not be used
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bAdditionalFailureInfo        |  Bitmap: [0]    bit is set to 1 if Device Container failed due to lack of power.
   *                                      |          [1]    Set to 1 if Device Container failed due to no USB-PD communication. Only valid if
   *                                      |                 bmConfigured field for the preferred AUM is not set to 11b.
   *                                      |          [2-7]  Reserved, for future use.
   *                                      |  Note: Field is only valid if bcdVersion is set to 0x0110h or higher.
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        wSVID[n]                      |  Standard or Vendor ID. Will match one of the SVIDs returned in response to a USBPD DiscoverSVIDs command.
   *                                      |  0xFF00 shall be used as SVID for USB4 devices and used in wSVID[0] location when USBPD Discovery Identity
   *                                      |  response includes USB4 Device Capable in UFP VD01
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bAlternateOrUSB4Mode[n]       |  Index of the AUM within the SVID as returned in response to a Discover Modes command.
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        iAlternateOrUSB4ModeString[n] |  Index of string descriptor describing protocol, optional to support this
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   * 
   * Note: User's are allowed to define as many instances of wSVID/bAlternateOrUSB4Mode/iAlternateOrUSB4ModeString as long as the number of AUM
   *       is less than MAX_NUM_ALT_OR_USB4_MODE_AUM (0x34h). They also must ensure that BOS total length is updated accordingly to ensure proper
   *       total length is passed back to host. MAX_NUM_ALT_OR_USB4_MODE_AUM is also an item that must be taken into consideration when modifying
   *       bmConfigured field, despite the spec defining 32 bytes. Only 104 bits (MAX_NUM_ALT_OR_USB4_MODE_AUM * 2 (bit pair)) is available for user
   *       to configure as of Billboard Spec v1.2.2.
   */
  TUD_BILLBOARD_CAPABILITY_DESCRIPTOR_BASE(5,                       /* iAdditionalInfoURL */\
                                           TUD_BILLBOARD_AUM_COUNT, /* bNumberOfAlternateOrUSB4Modes */\
                                           0x00,                    /* bPreferredAlternateOrUSB4Mode */\
                                           0x0000,                  /* VCONN Power */\
                                           0x00000003,              /* bmConfigured[0] */\
                                           0x00000000,              /* bmConfigured[1] */\
                                           0x00000000,              /* bmConfigured[2] */\
                                           0x00000000,              /* bmConfigured[3] */\
                                           0x00000000,              /* bmConfigured[4] */\
                                           0x00000000,              /* bmConfigured[5] */\
                                           0x00000000,              /* bmConfigured[6] */\
                                           0x00000000,              /* bmConfigured[7] */\
                                           0x0122,                  /* bcdVersion */\
                                           0x00,                    /* bAdditionalFailureInfo */\
                                           U16_TO_U8S_LE(0xFF01),   /* wSVID[0] -> DP_SID value from table 3-8 of Billboard spec v1.2.2 */\
                                           0x00,                    /* bAlternateOrUSB4Mode[0] -> Mode1 in list of modes (DisplayPort) */\
                                           6                        /* iAlternateOrUSB4ModeString[0] -> Index to string describing Displayport AUM */
                                          ),

  /*
   * Descriptor Type: Billboard AUM Capability Descriptor
   * Length: 8 bytes
   * Input: bIndex             -> Index where the AUM will appear in the Capability descriptor
   *        dwAlternateModeVdo -> Content of the Mode VDO for the AUM
   */
  TUD_BILLBOARD_AUM_CAPBILITY_DESCRIPTOR(0x00,        /* Location of alt mode in billboard descriptor */
                                         0x000C000C5)  /* Mode 1 VDO (Fictional Application) - from USBPD Mode response in BIllboard spec v1.2.2 */
};

/* BOS and Capability Descriptor is required for Billboard */
uint8_t thunderbolt_desc_bos[] =
{
  /*
   * Descriptor Type: BOS Descriptor
   * Length: 5 bytes
   * Input: wTotalLength ->   Total length of all descriptors in BOS
   *        bNumDeviceCaps -> Number of separate device capability descriptors in BOS.
   *                          This example uses the value of 3 as it contains a Billboard
   *                          Capability, Container ID, and AUM capability descriptor.
   */
  TUD_BOS_DESCRIPTOR(BOS_TOTAL_LEN, TUD_TOTAL_CAP_COUNT),

  /*
   * Descriptor Type: Container ID
   * Length: 20 bytes
   * Input: UUID -> For this example a generic one will be utilized
   */
  TUD_BILLBOARD_CONTAINER_ID_DESCRIPTOR(TUD_BOS_CONTAINER_EXAMPLE_UUID),

  /*
   * Descriptor Type: Capability Descriptor
   * Length: 44 + (4 * # of AUM Capability descriptors) bytes
   * Input: iAdditionalInfoURL            |   Index of string descriptor containing URL to get more details on product and various AUM it supports.
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bNumberOfAlternateOrUSB4Modes |  # of AUM supported, maximum value of this field is MAX_NUM_ALT_OR_USB_4_MODE (0x34h).
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bPreferredAlternateOrUSB4Mode |  Index of preferred AUM, field shall be set to 0xFF to indicate no pref.
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        VCONN Power                   |  Bitmap: [0-2]  Power needed by adapter for full function
   *                                      |          [3-14] Reserved, shall be set to 0
   *                                      |          [15]   Adapter does not require any Vconn Power. If set, bits [0-2] will be ignored
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bmConfigured                  |  Bitmap: Each bit pair represents state of the AUM identified in wSVID[n] & bAlternateOrUSB4Mode[n]
   *                                      |
   *                                      |  |----------------------------------------------------------------------|
   *                                      |  |Value | Description                                                   |
   *                                      |  |----------------------------------------------------------------------|
   *                                      |  |00b   | Unspecified Error                                             |
   *                                      |  |01b   | AUM configuration not attempted or exited                     |
   *                                      |  |10b   | AUM configuration attempted but not succesful and not entered |
   *                                      |  |11b   | AUM configuration succesful                                   |
   *                                      |  |______________________________________________________________________|
   *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bcdVersion                    |  Billboard capability descriptor version number in Binary-Coded Decimal (e.g, 1.10 is 0x0110h).
   *                                      |  This field identifies which version of the Billboard spec implementation is compliant with.
   *                                      |  If 0x0000h then it follows first revision of Billboard spec, for this example it was implemented
   *                                      |  following v1.2.2 of Billboard spec, hence input will be 0x0122h.
   *                                      |  Note: 0x120h is reserved and should not be used
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bAdditionalFailureInfo        |  Bitmap: [0]    bit is set to 1 if Device Container failed due to lack of power.
   *                                      |          [1]    Set to 1 if Device Container failed due to no USB-PD communication. Only valid if
   *                                      |                 bmConfigured field for the preferred AUM is not set to 11b.
   *                                      |          [2-7]  Reserved, for future use.
   *                                      |  Note: Field is only valid if bcdVersion is set to 0x0110h or higher.
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        wSVID[n]                      |  Standard or Vendor ID. Will match one of the SVIDs returned in response to a USBPD DiscoverSVIDs command.
   *                                      |  0xFF00 shall be used as SVID for USB4 devices and used in wSVID[0] location when USBPD Discovery Identity
   *                                      |  response includes USB4 Device Capable in UFP VD01
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bAlternateOrUSB4Mode[n]       |  Index of the AUM within the SVID as returned in response to a Discover Modes command.
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        iAlternateOrUSB4ModeString[n] |  Index of string descriptor describing protocol, optional to support this
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   * 
   * Note: User's are allowed to define as many instances of wSVID/bAlternateOrUSB4Mode/iAlternateOrUSB4ModeString as long as the number of AUM
   *       is less than MAX_NUM_ALT_OR_USB4_MODE_AUM (0x34h). They also must ensure that BOS total length is updated accordingly to ensure proper
   *       total length is passed back to host. MAX_NUM_ALT_OR_USB4_MODE_AUM is also an item that must be taken into consideration when modifying
   *       bmConfigured field, despite the spec defining 32 bytes. Only 104 bits (MAX_NUM_ALT_OR_USB4_MODE_AUM * 2 (bit pair)) is available for user
   *       to configure as of Billboard Spec v1.2.2.
   */
  TUD_BILLBOARD_CAPABILITY_DESCRIPTOR_BASE(5,                       /* iAdditionalInfoURL */\
                                           TUD_BILLBOARD_AUM_COUNT, /* bNumberOfAlternateOrUSB4Modes */\
                                           0x00,                    /* bPreferredAlternateOrUSB4Mode */\
                                           0x0000,                  /* VCONN Power */\
                                           0x00000003,              /* bmConfigured[0] */\
                                           0x00000000,              /* bmConfigured[1] */\
                                           0x00000000,              /* bmConfigured[2] */\
                                           0x00000000,              /* bmConfigured[3] */\
                                           0x00000000,              /* bmConfigured[4] */\
                                           0x00000000,              /* bmConfigured[5] */\
                                           0x00000000,              /* bmConfigured[6] */\
                                           0x00000000,              /* bmConfigured[7] */\
                                           0x0122,                  /* bcdVersion */\
                                           0x00,                    /* bAdditionalFailureInfo */\
                                           U16_TO_U8S_LE(0x8087),   /* wSVID[0] -> Intel VID value from table 3-8 of Billboard spec v1.2.2 */\
                                           0x01,                    /* bAlternateOrUSB4Mode[0] -> Mode2 in list of modes (Fictional Debug) */\
                                           7                        /* iAlternateOrUSB4ModeString[0] -> Index of string describing Intel Fictional Debug AUM */
                                          ),

  /*
   * Descriptor Type: Billboard AUM Capability Descriptor
   * Length: 8 bytes
   * Input: bIndex             -> Index where the AUM will appear in the Capability descriptor
   *        dwAlternateModeVdo -> Content of the Mode VDO for the AUM
   */
  TUD_BILLBOARD_AUM_CAPBILITY_DESCRIPTOR(0x00,        /* Location of alt mode in billboard descriptor */
                                         0x000000002)  /* Mode 1 VDO (Fictional Application) - from USBPD Mode response in BIllboard spec v1.2.2 */
};

/* BOS and Capability Descriptor is required for Billboard */
uint8_t usb4_desc_bos[] =
{
  /*
   * Descriptor Type: BOS Descriptor
   * Length: 5 bytes
   * Input: wTotalLength ->   Total length of all descriptors in BOS
   *        bNumDeviceCaps -> Number of separate device capability descriptors in BOS.
   *                          This example uses the value of 3 as it contains a Billboard
   *                          Capability, Container ID, and AUM capability descriptor.
   */
  TUD_BOS_DESCRIPTOR(BOS_TOTAL_LEN, TUD_TOTAL_CAP_COUNT),

  /*
   * Descriptor Type: Container ID
   * Length: 20 bytes
   * Input: UUID -> For this example a generic one will be utilized
   */
  TUD_BILLBOARD_CONTAINER_ID_DESCRIPTOR(TUD_BOS_CONTAINER_EXAMPLE_UUID),

  /*
   * Descriptor Type: Capability Descriptor
   * Length: 44 + (4 * # of AUM Capability descriptors) bytes
   * Input: iAdditionalInfoURL            |   Index of string descriptor containing URL to get more details on product and various AUM it supports.
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bNumberOfAlternateOrUSB4Modes |  # of AUM supported, maximum value of this field is MAX_NUM_ALT_OR_USB_4_MODE (0x34h).
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bPreferredAlternateOrUSB4Mode |  Index of preferred AUM, field shall be set to 0xFF to indicate no pref.
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        VCONN Power                   |  Bitmap: [0-2]  Power needed by adapter for full function
   *                                      |          [3-14] Reserved, shall be set to 0
   *                                      |          [15]   Adapter does not require any Vconn Power. If set, bits [0-2] will be ignored
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bmConfigured                  |  Bitmap: Each bit pair represents state of the AUM identified in wSVID[n] & bAlternateOrUSB4Mode[n]
   *                                      |
   *                                      |  |----------------------------------------------------------------------|
   *                                      |  |Value | Description                                                   |
   *                                      |  |----------------------------------------------------------------------|
   *                                      |  |00b   | Unspecified Error                                             |
   *                                      |  |01b   | AUM configuration not attempted or exited                     |
   *                                      |  |10b   | AUM configuration attempted but not succesful and not entered |
   *                                      |  |11b   | AUM configuration succesful                                   |
   *                                      |  |______________________________________________________________________|
   *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bcdVersion                    |  Billboard capability descriptor version number in Binary-Coded Decimal (e.g, 1.10 is 0x0110h).
   *                                      |  This field identifies which version of the Billboard spec implementation is compliant with.
   *                                      |  If 0x0000h then it follows first revision of Billboard spec, for this example it was implemented
   *                                      |  following v1.2.2 of Billboard spec, hence input will be 0x0122h.
   *                                      |  Note: 0x120h is reserved and should not be used
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bAdditionalFailureInfo        |  Bitmap: [0]    bit is set to 1 if Device Container failed due to lack of power.
   *                                      |          [1]    Set to 1 if Device Container failed due to no USB-PD communication. Only valid if
   *                                      |                 bmConfigured field for the preferred AUM is not set to 11b.
   *                                      |          [2-7]  Reserved, for future use.
   *                                      |  Note: Field is only valid if bcdVersion is set to 0x0110h or higher.
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        wSVID[n]                      |  Standard or Vendor ID. Will match one of the SVIDs returned in response to a USBPD DiscoverSVIDs command.
   *                                      |  0xFF00 shall be used as SVID for USB4 devices and used in wSVID[0] location when USBPD Discovery Identity
   *                                      |  response includes USB4 Device Capable in UFP VD01
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        bAlternateOrUSB4Mode[n]       |  Index of the AUM within the SVID as returned in response to a Discover Modes command.
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   *        iAlternateOrUSB4ModeString[n] |  Index of string descriptor describing protocol, optional to support this
   * *------------------------------------------------------------------------------------------------------------------------------------------------
   * 
   * Note: User's are allowed to define as many instances of wSVID/bAlternateOrUSB4Mode/iAlternateOrUSB4ModeString as long as the number of AUM
   *       is less than MAX_NUM_ALT_OR_USB4_MODE_AUM (0x34h). They also must ensure that BOS total length is updated accordingly to ensure proper
   *       total length is passed back to host. MAX_NUM_ALT_OR_USB4_MODE_AUM is also an item that must be taken into consideration when modifying
   *       bmConfigured field, despite the spec defining 32 bytes. Only 104 bits (MAX_NUM_ALT_OR_USB4_MODE_AUM * 2 (bit pair)) is available for user
   *       to configure as of Billboard Spec v1.2.2.
   */
  TUD_BILLBOARD_CAPABILITY_DESCRIPTOR_BASE(5,                       /* iAdditionalInfoURL */\
                                           TUD_BILLBOARD_AUM_COUNT, /* bNumberOfAlternateOrUSB4Modes */\
                                           0x00,                    /* bPreferredAlternateOrUSB4Mode */\
                                           0x0000,                  /* VCONN Power */\
                                           0x00000003,              /* bmConfigured[0] */\
                                           0x00000000,              /* bmConfigured[1] */\
                                           0x00000000,              /* bmConfigured[2] */\
                                           0x00000000,              /* bmConfigured[3] */\
                                           0x00000000,              /* bmConfigured[4] */\
                                           0x00000000,              /* bmConfigured[5] */\
                                           0x00000000,              /* bmConfigured[6] */\
                                           0x00000000,              /* bmConfigured[7] */\
                                           0x0122,                  /* bcdVersion */\
                                           0x00,                    /* bAdditionalFailureInfo */\
                                           U16_TO_U8S_LE(0xFF00),   /* wSVID[0] -> USB PD SID value from table 3-8 of Billboard spec v1.2.2 */\
                                           0x00,                    /* bAlternateOrUSB4Mode[0] -> USB4 Mode */\
                                           8                        /* iAlternateOrUSB4ModeString[0] -> Index of string describing USB4 Mode */
                                          ),

  /*
   * Descriptor Type: Billboard AUM Capability Descriptor
   * Length: 8 bytes
   * Input: bIndex             -> Index where the AUM will appear in the Capability descriptor
   *        dwAlternateModeVdo -> Content of the Mode VDO for the AUM
   */
  TUD_BILLBOARD_AUM_CAPBILITY_DESCRIPTOR(0x00,        /* Location of alt mode in billboard descriptor */
                                         0x2045E000)  /* Mode 1 VDO (Fictional Application) - from USB4 EUDO in Billboard spec v1.2.2 */
};

/* Confirming Displayport BOS contains length of (5 (BOS) + 20 (Container ID) + 48 (Capability) + 8 (1 AUM Capability)) */
TU_VERIFY_STATIC((BOS_TOTAL_LEN == sizeof(displayport_desc_bos)),
                  "Displayport total descriptor length is wrong");

/* Confirming Thunderbolt BOS contains length of (5 (BOS) + 20 (Container ID) + 48 (Capability) + 8 (1 AUM Capability)) */
TU_VERIFY_STATIC((BOS_TOTAL_LEN == sizeof(thunderbolt_desc_bos)),
                  "Thunderbolt total descriptor length is wrong");

/* Confirming USB4 BOS contains length of (5 (BOS) + 20 (Container ID) + 48 (Capability) + 8 (1 AUM Capability)) */
TU_VERIFY_STATIC((BOS_TOTAL_LEN == sizeof(usb4_desc_bos)),
                  "USB4 total descriptor length is wrong");

uint8_t const * tud_descriptor_bos_cb(void)
{
  switch (descriptorState) {
  case THUNDERBOLT:
    return thunderbolt_desc_bos;
  case DISPLAYPORT:
    return displayport_desc_bos;
  case USB4:
    return usb4_desc_bos;
  default:
    break;
  }
  
  return 0;
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

// String Descriptor Index
enum {
  STRID_LANGID = 0,
  STRID_MANUFACTURER,
  STRID_PRODUCT,
  STRID_SERIAL,
};

// array of pointer to string descriptors
char const *string_desc_arr[] =
{
  (const char[]) { 0x09, 0x04 },        /* 0: is supported language is English (0x0409) */
  "Texas Instruments",                  /* 1: Manufacturer */
  "MSPM0 Billboard Device",             /* 2: Product */
   NULL,                                /* 3: Serials will use unique ID if possible */
  "TinyUSB + MSPM0 Billboard",          /* 4: Billboard Interface */
  "www.ti.com",                         /* 5: iAdditionalInfoURL */
  "DisplayPort Alternate Mode",         /* 6: iAlternateOrUSB4ModeString[0] -> DP BOS */
  "Thunderbolt Alternate Mode",         /* 7: iAlternateOrUSB4ModeString[0] -> Intel BOS */
  "USB4 Alternate Mode",                /* 8: iAlternateOrUSB4ModeString[0] -> USB4 BOS */
};

static uint16_t _desc_str[32 + 1];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
  (void) langid;
  size_t chr_count;

  switch ( index ) {
  case STRID_LANGID:
    memcpy(&_desc_str[1], string_desc_arr[0], 2);
    chr_count = 1;
    break;
  case STRID_SERIAL:
    chr_count = board_usb_get_serial(_desc_str + 1, 32);
    break;
  default:
    // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
    // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors
    if ( !(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])) ) return NULL;
      const char *str = string_desc_arr[index];
      // Cap at max char
      chr_count = strlen(str);
      size_t const max_count = sizeof(_desc_str) / sizeof(_desc_str[0]) - 1; // -1 for string type
      if ( chr_count > max_count ) chr_count = max_count;
      // Convert ASCII string into UTF-16
      for ( size_t i = 0; i < chr_count; i++ ) {
        _desc_str[1 + i] = str[i];
      }
      break;
  }
  // first byte is length (including header), second byte is string type
  _desc_str[0] = (uint16_t) ((TUSB_DESC_STRING << 8) | (2 * chr_count + 2));
  return _desc_str;
}
