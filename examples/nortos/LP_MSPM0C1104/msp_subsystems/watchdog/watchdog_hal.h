/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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
 */


#ifndef WATCHDOG_FUNCTION_H_
#define WATCHDOG_FUNCTION_H_

#include "ti_msp_dl_config.h"

#define WATCHDOG_1s             (32768)
#define WATCHDOG_500ms          (WATCHDOG_1s/2)
#define WATCHDOG_250ms          (WATCHDOG_1s/4)
#define WATCHDOG_125ms          (WATCHDOG_1s/8)
#define WATCHDOG_100ms          (WATCHDOG_1s/10)
#define WATCHDOG_50ms           (WATCHDOG_1s/20)
#define WATCHDOG_25ms           (WATCHDOG_1s/40)
#define WATCHDOG_12D5ms         (WATCHDOG_1s/80)
#define WATCHDOG_10ms           (WATCHDOG_1s/100)

#define delay_1s                (24000000)
#define delay_20ms              (delay_1s/50)
#define delay_10ms              (delay_20ms/2)
#define delay_5ms               (delay_20ms/4)
#define delay_2D5ms             (delay_20ms/8)
#define delay_2ms               (delay_20ms/10)


/* User defined */
#define WATCHDOG_PERIOD    (WATCHDOG_1s)
#define WDO_RESET_TIME          (delay_10ms)

void watchdog_start(void);
void watchdog_timeout(void);
void watchdog_reload(void);

#endif /* WATCHDOG_FUNCTION_H_ */
