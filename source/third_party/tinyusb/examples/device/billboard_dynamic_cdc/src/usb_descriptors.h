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
 */

#ifndef USB_DESCRIPTORS_H_
#define USB_DESCRIPTORS_H_

enum {
    DISPLAYPORT = 0,
    THUNDERBOLT,
    USB4,
    TOTAL_AUM_COUNT,
};

extern uint8_t descriptorState;

/* Number of Billboard AUM capability descriptors that will be supported */
#define BILLBOARD_AUM_COUNT (1)

/* 
 * Number of separate device capability descriptors in BOS callback, includes Capability and Container ID.
 * Billboard capability + Container ID + # of AUM
 */
#define TUD_TOTAL_CAP_COUNT (2 + BILLBOARD_AUM_COUNT)

/* Total length of all Billboard AUM Capability descriptors */
#define BOS_TOTAL_LEN      (TUD_BOS_DESC_LEN + TUD_BILLBOARD_CONTAINER_ID_LEN +\
                           (TUD_BILLBOARD_CAPABILITY_DESCRIPTOR_BASE_LEN + (BILLBOARD_AUM_COUNT * 4)) +\
                           (BILLBOARD_AUM_COUNT * TUD_BILLBOARD_AUM_CAPABILITY_LEN))

/* Example universal unique identifier required for Container ID descriptor */
#define TUD_BOS_CONTAINER_EXAMPLE_UUID \
        /* Data1 */\
        0x12, 0x3F, 0x5A, 0xF0, \
        /* Data2 */\
        0xD4, 0x78, \
        /* Data3 */\
        0x9A, 0x4A, \
        /* Data4 */\
        0xBC, 0xDE, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66

typedef struct TU_ATTR_PACKED {
    /* BOS descriptor struct length 5 bytes */
    struct TU_ATTR_PACKED {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint16_t wTotalLength;
        uint8_t bNumDeviceCaps;
    } BOS;

    /* Container ID struct length 20 bytes */
    struct TU_ATTR_PACKED {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint8_t bDevCapabilityType;
        uint8_t bReserved;
        uint8_t UUID[16];
    } containerID;

    /* Capability Descriptor struct length 44 bytes + (4 * BILLBOARD_AUM_COUNT) */
    struct TU_ATTR_PACKED {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint8_t bDevCapabilityType;
        uint8_t iAdditionalInfoURL;
        uint8_t bNumberOfAlternateOrUSB4Modes;
        uint8_t bPreferredAlternateOrUSB4Mode;
        uint16_t VCONNPower;
        uint8_t bmConfigured[32];
        uint16_t bcdVersion;
        uint8_t bAdditionalFailureInfo;
        uint8_t bReserved;
        struct TU_ATTR_PACKED {
            uint16_t wSVID;
            uint8_t bAlternateOrUSB4Mode;
            uint8_t iAlternateOrUSB4ModeString;
        } aum[BILLBOARD_AUM_COUNT];
    } billboardCapability;
    /* 
     * This value will grow depending on user requirement, i.e if they have another instance of
     * AUM then they must add another wSVID/bAlt/iAlt instance into the capability struct 
     */

    /* AUM Capability Descriptor struct length 8 bytes */
    struct TU_ATTR_PACKED {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint8_t bDevCapabilityType;
        uint8_t bIndex;
        uint32_t dwAlternateModeVdo;
    } billboardAumCapability[BILLBOARD_AUM_COUNT];
} bos_desc_t;

/* Confirm the size of our Billboard descriptor */
TU_VERIFY_STATIC((sizeof(bos_desc_t) == BOS_TOTAL_LEN), "Total Billboard descriptor length is wrong");

/* Extern the struct so we can use it main.c */
extern bos_desc_t BOS_desc;

#endif /* USB_DESCRIPTORS_H_ */