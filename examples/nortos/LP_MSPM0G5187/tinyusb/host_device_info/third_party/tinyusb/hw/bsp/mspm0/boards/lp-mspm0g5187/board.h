/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Ha Thach (tinyusb.org)
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
 * This file is part of the TinyUSB stack.
 */

#ifndef BOARD_H_
#define BOARD_H_

/* System header for MSPM0G518x */
#include "system_mspm0g518x.h"

/* Defines utilized for board LED */
#define GPIO_LED_TUSB              GPIOA
#define GPIO_LED_PIN_TUSB          (0x01000000U)
#define GPIO_LED_IOMUX_TUSB        (53)

/* Defines utilized for board button */
#define GPIO_BUTTON_TUSB           GPIOA
#define GPIO_BUTTON_PIN_TUSB       (0x00000080U)
#define GPIO_BUTTON_IOMUX_TUSB     (13)

/* Defines utilized for HFXT pin */
#define GPIO_HFXIN_IOMUX_TUSB      (9)
#define GPIO_HFXOUT_IOMUX_TUSB     (10)

/* Defines utilized for UART instance in SPGSS0*/
#define UNICOMM_TUSB               UC0->inst
#define UART_TUSB                  UC0->uart
#define GPIO_UART_TX_IO_FUNC_TUSB  IOMUX_PINCM21_PF_UC0_TX
#define GPIO_UART_TX_IOMUX_TUSB    (20)
#define GPIO_UART_RX_IO_FUNC_TUSB  IOMUX_PINCM22_PF_UC0_RX
#define GPIO_UART_RX_IOMUX_TUSB    (21)

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef __cplusplus
 }
#endif

#endif