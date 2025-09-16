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

uint8_t descriptorState;

/* Number of Billboard AUM capability descriptors that will be supported */
#define TUD_BILLBOARD_AUM_COUNT (1)

/* 
 * Number of separate device capability descriptors in BOS callback, includes Capability and Container ID.
 * Billboard capability + Container ID + # of AUM
 */
#define TUD_TOTAL_CAP_COUNT (2 + TUD_BILLBOARD_AUM_COUNT)

/* Total length of all Billboard AUM Capability descriptors */
#define BOS_TOTAL_LEN   (TUD_BOS_DESC_LEN + TUD_BILLBOARD_CONTAINER_ID_LEN +\
                        (TUD_BILLBOARD_CAPABILITY_DESCRIPTOR_BASE_LEN + (TUD_BILLBOARD_AUM_COUNT * 4)) +\
                        (TUD_BILLBOARD_AUM_COUNT * TUD_BILLBOARD_AUM_CAPABILITY_LEN))

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

#endif /* USB_DESCRIPTORS_H_ */