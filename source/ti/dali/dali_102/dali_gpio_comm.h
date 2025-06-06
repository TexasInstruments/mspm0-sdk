/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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

#ifndef DALI_GPIO_COMM_H_
#define DALI_GPIO_COMM_H_

#include "ti_msp_dl_config.h"

/* Receiver Bit Timings */

/*! Minimum DALI Rx Half Bit Time */
#define DALI_RX_HALFBIT_MIN            (333.3f)  //in us
/*! Maximum DALI Rx Half Bit Time */
#define DALI_RX_HALFBIT_MAX            (500.0f)  //in us
/*! Minimum DALI Rx Double Half Bit Time */
#define DALI_RX_BIT_MIN                (666.7f)  //in us
/*! Maximum DALI Rx Double Half Bit Time */
#define DALI_RX_BIT_MAX                (1000.0f) //in us

/*! DALI Rx Timer Frequency */
#define TIMER_CLK_FREQ_MHZ             (32)

/*! Minimum cycles for DALI Rx Half Bit */
#define DALI_RX_HALFBIT_MIN_CYC        (DALI_RX_HALFBIT_MIN * TIMER_CLK_FREQ_MHZ)
/*! Maximum cycles for DALI Rx Half Bit */
#define DALI_RX_HALFBIT_MAX_CYC        (DALI_RX_HALFBIT_MAX * TIMER_CLK_FREQ_MHZ)
/*! Minimum cycles for DALI Rx Double Half Bit */
#define DALI_RX_BIT_MIN_CYC            (DALI_RX_BIT_MIN * TIMER_CLK_FREQ_MHZ)
/*! Maximum cycles for DALI Rx Double Half Bit */
#define DALI_RX_BIT_MAX_CYC            (DALI_RX_BIT_MAX * TIMER_CLK_FREQ_MHZ)

/*! Total Half bits in Forward Frame */
#define FORWARD_FRAME_HALFBIT_SIZE     (34)
/*! Maximum Length of Captured Bits  */
#define MAX_CAPTURE_BITS_LENGTH        (100)

/*!
 * @brief Variable to store DALI Rx information
*/
typedef struct {
    /*! Array to store receiving Rx Bits */
    uint8_t captBits[MAX_CAPTURE_BITS_LENGTH];
    /*! Array to store timer duration of Rx Bits */
    uint32_t captBitTimings[MAX_CAPTURE_BITS_LENGTH];
    /*! Stores number of bits being received */
    uint8_t captIndex;
    /*! Array to store the Received Rx data */
    uint8_t dataArr[2];
} dali_Rx;

/*! Definition of global DALI Rx variable*/
extern volatile dali_Rx gDALI_Rx;


/**
 * @brief   Decode received Half bits from DALI Rx
 *
 * This function decodes the forward frame from the received half bits and duration
 *
 * @return  Whether or not the received bits form a proper forward frame
 *
 * @retval  true    Received half bits form a proper forward frame
 * @retval  false   Received half bits doesn't form a proper forward frame
 */
bool DALI_RX_decodeFrame(void);

#endif /* DALI_GPIO_COMM_H_ */
