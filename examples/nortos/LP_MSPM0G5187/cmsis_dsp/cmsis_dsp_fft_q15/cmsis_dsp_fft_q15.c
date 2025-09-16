/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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

#include "arm_const_structs.h"
#include "arm_math.h"
#include "ti_msp_dl_config.h"

/* Number of samples */
#define NUM_SAMPLES 256
/* Global variables and defines for FFT */
#define IFFTFLAG 0
#define BITREVERSE 1

/*
 * This sample array contains a reference signal with two frequency components:
 *   sine wave of amplitude of 0.5 and 2kHz frequency, and
 *   sine wave of amplitude of 0.25 and 4kHz frequency
 * The sample frequency of the signal is 100kHz.
 */
static uint32_t gDstBuffer[NUM_SAMPLES] = {0, 4091, 8021, 11639, 14810, 17421,
    19391, 20671, 21246, 21137, 20397, 19109, 17378, 15325, 13078, 10767, 8513,
    6421, 4577, 3040, 1839, 976, 424, 128, 16, 0, -16, -128, -424, -976, -1839,
    -3040, -4577, -6421, -8513, -10767, -13078, -15325, -17378, -19109, -20397,
    -21137, -21246, -20671, -19391, -17421, -14810, -11639, -8021, -4091, 0,
    4091, 8021, 11639, 14810, 17421, 19391, 20671, 21246, 21137, 20397, 19109,
    17378, 15325, 13078, 10767, 8513, 6421, 4577, 3040, 1839, 976, 424, 128,
    16, 0, -16, -128, -424, -976, -1839, -3040, -4577, -6421, -8513, -10767,
    -13078, -15325, -17378, -19109, -20397, -21137, -21246, -20671, -19391,
    -17421, -14810, -11639, -8021, -4091, 0, 4091, 8021, 11639, 14810, 17421,
    19391, 20671, 21246, 21137, 20397, 19109, 17378, 15325, 13078, 10767, 8513,
    6421, 4577, 3040, 1839, 976, 424, 128, 16, 0, -16, -128, -424, -976, -1839,
    -3040, -4577, -6421, -8513, -10767, -13078, -15325, -17378, -19109, -20397,
    -21137, -21246, -20671, -19391, -17421, -14810, -11639, -8021, -4091, 0,
    4091, 8021, 11639, 14810, 17421, 19391, 20671, 21246, 21137, 20397, 19109,
    17378, 15325, 13078, 10767, 8513, 6421, 4577, 3040, 1839, 976, 424, 128,
    16, 0, -16, -128, -424, -976, -1839, -3040, -4577, -6421, -8513, -10767,
    -13078, -15325, -17378, -19109, -20397, -21137, -21246, -20671, -19391,
    -17421, -14810, -11639, -8021, -4091, 0, 4091, 8021, 11639, 14810, 17421,
    19391, 20671, 21246, 21137, 20397, 19109, 17378, 15325, 13078, 10767, 8513,
    6421, 4577, 3040, 1839, 976, 424, 128, 16, 0, -16, -128, -424, -976, -1839,
    -3040, -4577, -6421, -8513, -10767, -13078, -15325, -17378, -19109, -20397,
    -21137, -21246, -20671, -19391, -17421, -14810, -11639, -8021, -4091, 0,
    4091, 8021, 11639, 14810, 17421};

/* Expected Maximum amplitude and frequency index of FFT */
#define GOLDEN_FFT_MAX_FREQ_INDEX (5)

/* Array with FFT results */
volatile int16_t gFFTOutput[NUM_SAMPLES * 2];
/* Maximum amplitude of FFT */
volatile uint32_t gFFTmaxValue;
/* Index of maximum frequency */
volatile uint32_t gFFTmaxFreqIndex;

int main(void)
{
    SYSCFG_DL_init();

    /* Compute the FFT on the sampled data and then find the
     * FFT point for maximum energy and the energy value */
    arm_cfft_q15(
        &arm_cfft_sR_q15_len256, (q15_t *) gDstBuffer, IFFTFLAG, BITREVERSE);
    arm_cmplx_mag_q15((q15_t *) gDstBuffer, (q15_t *) gFFTOutput, NUM_SAMPLES);
    arm_max_q15((q15_t *) gFFTOutput, NUM_SAMPLES, (q15_t *) &gFFTmaxValue,
        (uint32_t *) &gFFTmaxFreqIndex);

    /*
     * The following results are expected:
     * gFFTOutput = FFT output of the signal
     * gFFTmaxFreqIndex = 5
     *  Note that bin 5 corresponds to ~1,953Hz +- ~195Hz, which represents
     *  the 2kHz major component.
     *  The next higher component in the FFT is bin 10 which corresponds to
     *  the ~3,906Hz +- ~195Hz, representing the 4kHz component.
     */
    if (gFFTmaxFreqIndex != GOLDEN_FFT_MAX_FREQ_INDEX) {
        __BKPT(0);
    }

    while (1) {
    }
}
