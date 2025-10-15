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
#include "usb_descriptors.h"

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
  .bDeviceClass       = TUSB_CLASS_MISC,
  .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
  .bDeviceProtocol    = MISC_PROTOCOL_IAD,
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
  ITF_NUM_CDC_0,
  ITF_NUM_CDC_0_DATA,
  ITF_NUM_TOTAL
};

/* CDC endpoints */
#define EPNUM_CDC_0_NOTIF   0x81
#define EPNUM_CDC_0_OUT     0x02
#define EPNUM_CDC_0_IN      0x82

/* Length of configuration descriptor + billboard descriptor */
#define CONFIG_TOTAL_LEN    (TUD_CONFIG_DESC_LEN + TUD_BILLBOARD_DESC_LEN + TUD_CDC_DESC_LEN)

uint8_t const desc_fs_configuration[] =
{
  /* Config number, interface count, string index, total length, attribute, power in mA */
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),

  /*
   * Descriptor Type: Billboard Interface
   * Input: String index describing this interface
   */
  TUD_BILLBOARD_DESC_DESCRIPTOR(4),

  /* CDC: Interface number, string index, EP notification address and size, EP data address (out, in) and size */
  TUD_CDC_DESCRIPTOR(ITF_NUM_CDC_0, 9, EPNUM_CDC_0_NOTIF, 8, EPNUM_CDC_0_OUT, EPNUM_CDC_0_IN, 64),
};

/* Total length of Configuration and Interface descriptor must be 84-bytes */
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
/* 
 * Initial Billboard descriptor, will be re-configured at run-time
 * this example will start off in thunderbolt mode.
 */
bos_desc_t BOS_desc =
{
  /* BOS initialization */
  .BOS = {
    .bLength            = (sizeof(BOS_desc.BOS)),
    .bDescriptorType    = TUSB_DESC_BOS,
    .wTotalLength       = (sizeof(bos_desc_t)),
    .bNumDeviceCaps     = TUD_TOTAL_CAP_COUNT,
  },

  /* Container ID initialization */
  .containerID = {
    .bLength            = (sizeof(BOS_desc.containerID)),
    .bDescriptorType    = TUSB_DESC_DEVICE_CAPABILITY,
    .bDevCapabilityType = DEVICE_CAPABILITY_CONTAINER_id,
    .bReserved          = 0x00,
    .UUID               = {TUD_BOS_CONTAINER_EXAMPLE_UUID},
  },

  /* Billboard Capability descriptor initialization */
  .billboardCapability = {
    .bLength                        = (sizeof(BOS_desc.billboardCapability)),
    .bDescriptorType                = TUSB_DESC_DEVICE_CAPABILITY,
    .bDevCapabilityType             = DEVICE_CAPABILITY_BILLBOARD,
    .iAdditionalInfoURL             = 5,
    .bNumberOfAlternateOrUSB4Modes  = 0x01,
    .bPreferredAlternateOrUSB4Mode  = 0x00,
    .VCONNPower                     = 0x0000,
    .bmConfigured                   = {0},
    .bcdVersion                     = 0x0122,
    .bAdditionalFailureInfo         = 0x00,
    .bReserved                      = 0x00,
    .aum = {
      /* Index 0 */
      {
      .wSVID                      = 0x8087,
      .bAlternateOrUSB4Mode       = 0x01,
      .iAlternateOrUSB4ModeString = 7,
      }
    },
  },

  /* AUM capability descriptor initialization */
  .billboardAumCapability = {
    /* Index 0 */
    {
    .bLength            = (sizeof(BOS_desc.billboardAumCapability)),
    .bDescriptorType    = TUSB_DESC_DEVICE_CAPABILITY,
    .bDevCapabilityType = DEVICE_CAPABILITY_BILLBOARD_AUM,
    .bIndex             = 0x00,
    .dwAlternateModeVdo = 0x000000002,
    }
  },
};

/* Verify total length of billboard descriptor is correct */
TU_VERIFY_STATIC((sizeof(BOS_desc) == BOS_TOTAL_LEN),
                  "Billboard length is not correct");
/* Verify BOS descriptor length is correct */
TU_VERIFY_STATIC((sizeof(BOS_desc.BOS) == TUD_BOS_DESC_LEN),
                  "Billboard BOS length is not correct");
/* Verify Container ID descriptor length is correct */
TU_VERIFY_STATIC((sizeof(BOS_desc.containerID) == TUD_BILLBOARD_CONTAINER_ID_LEN),
                  "Billboard Container ID length is not correct");
/* Verify Capability descriptor length is correct */
TU_VERIFY_STATIC((sizeof(BOS_desc.billboardCapability) ==
                  (TUD_BILLBOARD_CAPABILITY_DESCRIPTOR_BASE_LEN + (BILLBOARD_AUM_COUNT * 4))),
                  "Billboard Capability length is not correct");
/* Verify AUM capability descriptor length is correct */
TU_VERIFY_STATIC((sizeof(BOS_desc.billboardAumCapability[0]) ==
                  (BILLBOARD_AUM_COUNT * TUD_BILLBOARD_AUM_CAPABILITY_LEN)),
                  "Billboard AUM Capability length is not correct");

/* Return Billboard descriptor in BOS callback */
uint8_t const * tud_descriptor_bos_cb(void)
{ 
  return (uint8_t const *) &BOS_desc;
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
  "MSPM0 CDC + Billboard",              /* 4: Billboard Interface */
  "www.ti.com",                         /* 5: iAdditionalInfoURL */
  "DisplayPort Alternate Mode",         /* 6: iAlternateOrUSB4ModeString[0] -> DP BOS */
  "Thunderbolt Alternate Mode",         /* 7: iAlternateOrUSB4ModeString[0] -> Intel BOS */
  "USB4 Alternate Mode",                /* 8: iAlternateOrUSB4ModeString[0] -> USB4 BOS */
  "Serial Port",                        /* 9: CDC serial port */
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
