/*
 * Copyright (c) 2025, Texas Instruments Incorporated
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

#include "feature_extract.h"


/**
 *  @brief      Swap two buffers pointers
 *
 *  This function swaps the pointers of two buffers.
 *
 *  @param[in]  buffer1     A pointer to the pointer to the buffer to swap.
 *  @param[in]  buffer2     A pointer to the pointer to the buffer to swap.
 *
 *  @return     None
 */
static void FE_swapBuffer(q15_t **buffer1, q15_t **buffer2)
{
    q15_t *temp = *buffer1;
    *buffer1 = *buffer2;
    *buffer2 = temp;
}


/**
 *  @brief      Perform real FFT
 *
 *  @param[in]  inputBuffer     A pointer to the input buffer.
 *  @param[out] outputBuffer    A pointer to the output buffer.
 *
 *  @return     None
 *
 *  @details    This function performs a real FFT on the input buffer.
 *              The FFT is performed on the buffer of size FE_FRAME_SIZE.
 *              The output is stored in the output buffer.
 */
static void FE_COMPUTE_RFFT(q15_t * inputBuffer, q15_t * outputBuffer)
{
    /* Perform real FFT Q15 */
    arm_rfft_q15(&varInstRfftQ15, inputBuffer, outputBuffer);
}



/**
 *  @brief      Scale the input buffer
 *
 *  @param[in]  inputBuffer     A pointer to the input buffer.
 *  @param[out] outputBuffer    A pointer to the output buffer.
 *
 *  @details    This function scales the input buffer.
 *              The scaling factor is defined in FE_COMPLEX_MAG_SCALE_FACTOR.
 *              The output is stored in the output buffer.
 */
static void FE_COMPUTE_Scale(q15_t * inputBuffer, q15_t * outputBuffer)
{
    arm_shift_q15(inputBuffer, FE_COMPLEX_MAG_SCALE_FACTOR, outputBuffer, 2*FE_FRAME_SIZE);
}



/**
 *  @brief      Calculate the magnitude of the complex numbers
 *
 *  @param[in]  inputBuffer     A pointer to the input buffer.
 *  @param[out] outputBuffer    A pointer to the output buffer.
 *
 *  @details    This function calculates the magnitude of the complex numbers
 *              in the input buffer.
 *              The output is stored in the output buffer.
 */
static void FE_COMPUTE_MAG(q15_t * inputBuffer, q15_t * outputBuffer)
{
    arm_cmplx_mag_q15(inputBuffer,outputBuffer,FE_FRAME_SIZE >> 1);
}

/**
 *  @brief      Calculate the Bin of the input buffer
 *
 *  @param[in]  inputBuffer     A pointer to the input buffer.
 *  @param[out] outputBuffer    A pointer to the output buffer.
 *
 *  @details    This function calculates the Bin of the input buffer.
 *              The Bin is calculated by summing up the values in each bin
 *              and then dividing it by the bin size.
 *              The output is stored in the output buffer.
 *
 */
static void FE_COMPUTE_Bin(q15_t * inputBuffer, q15_t * outputBuffer)
{
    uint16_t binSize = FE_BIN_SIZE;
    uint16_t numBins = FE_FEATURE_SIZE_PER_FRAME;

    for(int i = 0; i < numBins ; i++)
    {
        uint16_t indx1 = i*binSize;
        q31_t sum = 0;

        for(int j = indx1 ; j < (indx1 + binSize) ; j++)
        {
            sum += inputBuffer[j];
        }

        q31_t avg = sum;

#if defined (FE_BIN_NORMALIZE)
        avg = sum / binSize;
        if(avg > 32767)
            avg = 32767;
        if(avg < -32768)
            avg = -32768;

#endif
        outputBuffer[i] = (q15_t) avg;
    }
}

void FE_init()
{
    #ifdef FE_RFFT
    /* Initialize ARM library for real FFT */
    arm_rfft_init_q15(&varInstRfftQ15, FE_FRAME_SIZE, 0, 1);
    #endif

}


void FE_process(q15_t* rawInput, q15_t* extractedFeatures)
{

    q15_t* inputBuffer =  scratchBuffer1;
    q15_t* outputBuffer =  scratchBuffer2;

    memcpy(outputBuffer, rawInput, FE_FRAME_SIZE * sizeof(q15_t));

    #ifdef FE_RFFT
    FE_swapBuffer(&inputBuffer,&outputBuffer);
    FE_COMPUTE_RFFT(inputBuffer,outputBuffer);
    #endif

    #ifdef FE_COMPLEX_MAG_SCALE
    FE_swapBuffer(&inputBuffer,&outputBuffer);
    FE_COMPUTE_Scale(inputBuffer,outputBuffer);
    #endif

    #ifdef FE_DC_REM
    /* Exclude DC Component from further processing */
    outputBuffer = outputBuffer + 2;
    #endif

    #ifdef FE_MAG
    FE_swapBuffer(&inputBuffer,&outputBuffer);
    FE_COMPUTE_MAG(inputBuffer,outputBuffer);
    #endif

    #ifdef FE_BIN
    FE_swapBuffer(&inputBuffer,&outputBuffer);
    FE_COMPUTE_Bin(inputBuffer,outputBuffer);
    #endif

    memcpy(extractedFeatures, outputBuffer, FE_FEATURE_SIZE_PER_FRAME * sizeof(q15_t));

    return;
}
