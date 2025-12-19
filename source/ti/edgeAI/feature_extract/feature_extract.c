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


#ifdef FE_RFFT
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
#endif

#ifdef FE_COMPLEX_MAG_SCALE
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
#endif


#ifdef FE_MAG
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
#endif

#ifdef FE_BIN
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
    uint16_t binOffset = FE_BIN_OFFSET;

    for(int i = 0; i < numBins ; i++)
    {
        uint16_t indx1 = binOffset + i*binSize;
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
#endif

#ifdef SKIP_NORMALIZE
/**
 *  @brief     Converts the data type of the extracted features to signed 8-bit integer.
 *
 *  @param[in]  inputBuffer     A pointer to the input buffer.
 *  @param[in]  varIndex        The index of the variable being processed for feature extraction.
 *  @param[out] outputBuffer    A pointer to the output buffer.
 *
 *  @details    This function converts the data type of the extracted features to signed 8-bit integer.
 *              The function uses value generated from tvmgen_default.h file to scale the data.
 *
 */
void FE_Skip_Normalize(q15_t * inputBuffer, q15_t * outputBuffer, uint8_t varIndex)
{
    for(int i = 0; i < FE_FEATURE_SIZE_PER_FRAME ; i++)
    {
        q31_t inputVal_q31 = (q31_t) inputBuffer[i];

        inputVal_q31 = ((int32_t)((inputVal_q31 + tvmgen_default_bias_data[varIndex]) * tvmgen_default_scale_data[varIndex])) \
                >> tvmgen_default_shift_data[varIndex];

        if(inputVal_q31 > 127)
        {
            inputVal_q31 = 127;
        }
        else if(inputVal_q31 < -128)
        {
            inputVal_q31 = -128;
        }

        outputBuffer[i] = (int8_t) inputVal_q31;
    }
}
#endif

#ifdef FE_PIR
/**
 *  @brief      Calculate features for PIR detection application - zero crossings and slope changes
 *  @param[in]  window          A pointer to the input buffer.
 *  @param[out] ZCRfeature      A pointer to the zero crossings Feature
 *  @param[out] SlopeFeature    A pointer to the slope changes Feature

 *  @details    This function calculates some of the features (Number of zero crossings and slope changes) 
 *              that are needed for PIR detection application. 
 *               
 */
static void FE_PIR_featureSet2(q15_t* window, q15_t* ZCRfeature, q15_t* SlopeFeature)
{
    uint8_t isSlopeInc = (window[1] - window[0]) > 0 ? 1 : 0;
    q15_t zcrVal = 0,slopeVal = 0;

    for(int i = 1 ; i < WINDOW_SIZE;i++)
    {
        /* Zero Crossings */
        int prevSign = (window[i-1] > 0) ? 1 : (window[i-1] < 0 ) ? -1 : 0;
        int currSign = (window[i] > 0) ? 1 : (window[i] < 0 ) ? -1 : 0;

        if(currSign != prevSign)
        {
            zcrVal++;
        }

        /* Slope Changes */
        int dif = (window[i] - window[i-1]);
        uint8_t slope = (dif) > 0 ? 1 : 0;

        if((abs(dif) > 1) && slope != isSlopeInc)
        {
            isSlopeInc = slope;
            slopeVal++;
        }
    }

    /* Scaling the features by 1000 for better accuracy */
    *ZCRfeature = zcrVal * 1000;
    *SlopeFeature = slopeVal * 1000;
}

/**
 *  @brief      Calculate features for PIR detection application - Frequency bins and dominant frequency
 *  @param[in]  window          A pointer to the input buffer.
 *  @param[out] FFTfeatures     A pointer to the FFT Features
 *  @param[out] DomFreqFeatures A pointer to the Dominant Frequency Features

 *  @details    This function calculates some of the features 
 *             (Frequency bins and top 2 index of dominant frequencies per window) 
 *              that are needed for PIR detection application. 
 *               
 */
static void FE_PIR_featureSet1(q15_t* window, q15_t* FFTfeatures, q15_t* DomFreqFeatures)
{
    q15_t windowQ15[WINDOW_SIZE];
    q15_t mirrWinQ15[FE_PIR_RFFT_SIZE];
    q15_t fftArr[FE_PIR_RFFT_SIZE << 1],magFft[FE_PIR_RFFT_SIZE << 1];

    uint8_t freqIndex1 = 0, freqIndex2 = 0;

    /* Convert to Q15 by scaling it to 128 */
    for(int i=0; i < WINDOW_SIZE;i++)
    {
        windowQ15[i] = window[i] << 7;
    }

    /* Make it to FFT size by appending the same array in the end */
    memcpy(mirrWinQ15, windowQ15, WINDOW_SIZE * sizeof(q15_t));
    memcpy(mirrWinQ15 + WINDOW_SIZE, windowQ15, WINDOW_SIZE * sizeof(q15_t));

    arm_rfft_q15(&varInstRfftQ15, mirrWinQ15, fftArr);

    /* Remove DC offset*/
    arm_cmplx_mag_q15(&fftArr[2],magFft,WINDOW_SIZE);

    /* Scaling it by 128 and also finding the top 2 dominant frequency */
    for(int i = 0 ; i < WINDOW_SIZE; i++)
    {
        magFft[i] = magFft[i] << 7;

        if(magFft[freqIndex1] < magFft[i])
        {
            freqIndex2 = freqIndex1;
            freqIndex1 = i;
        }
        else if(magFft[freqIndex2] < magFft[i])
        {
            freqIndex2 = i;
        }
    }

    /* Binning */
    uint8_t numBins = 16;
    for(int i=0;i < numBins; i++)
    {
        q31_t binSum = magFft[i*2] + magFft[i*2 + 1];

        binSum = binSum >> 1;

        if(binSum > 32767)
            binSum = 32767;
        if(binSum < -32768)
            binSum = -32768;

        FFTfeatures[i] = (q15_t) binSum;
    }

    DomFreqFeatures[0] = ((freqIndex1 + 1) * SAMPLING_RATE) / (((FE_PIR_RFFT_SIZE >> 1) + 1) << 1);
    DomFreqFeatures[1] = ((freqIndex2 + 1) * SAMPLING_RATE) / (((FE_PIR_RFFT_SIZE >> 1) + 1) << 1);


    return;
}

/**
 *  @brief      Calculate all the features needed for PIR detection application
 *
 *  @param[in]  inputBuffer     A pointer to the input buffer.
 *  @param[out] outputBuffer    A pointer to the output buffer.
 *
 *  @details    This function calculates all the features needed for PIR detection application.
 *              Total features = 16 FFT, 1 ZCR, 1 Slope, 2 Dominant Frequencies
 *
 */
static void FE_PIR_Features(q15_t * inputBuffer, q15_t * outputBuffer)
{
    q15_t fftFeatures[16];
    q15_t domFreqFeatures[2];
    q15_t zcrFeature;
    q15_t slopeFeature;

    FE_PIR_featureSet1(inputBuffer, fftFeatures, domFreqFeatures);

    FE_PIR_featureSet2(inputBuffer, &zcrFeature, &slopeFeature);

    int featureIndex = 0;
    for(int i = 0 ; i < 16 ;i++)
    {
        outputBuffer[featureIndex++] = fftFeatures[i];
    }

    outputBuffer[featureIndex++] = zcrFeature;
    outputBuffer[featureIndex++] = slopeFeature;

    outputBuffer[featureIndex++] = domFreqFeatures[0];
    outputBuffer[featureIndex++] = domFreqFeatures[1];

    return;
}
#endif

void FE_init()
{
    #ifdef FE_RFFT
    /* Initialize ARM library for real FFT */
    arm_rfft_init_q15(&varInstRfftQ15, FE_FRAME_SIZE, 0, 1);
    #endif

    #ifdef FE_PIR
    /* Initialize ARM library for real FFT */
    arm_rfft_init_64_q15(&varInstRfftQ15, 0, 1);
    #endif
}


void FE_process(q15_t* rawInput, int8_t* extractedFeatures, uint8_t varIndex)
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

    #ifdef FE_PIR
    FE_swapBuffer(&inputBuffer,&outputBuffer);
    FE_PIR_Features(inputBuffer,outputBuffer);
    #endif

    #ifdef SKIP_NORMALIZE
    FE_swapBuffer(&inputBuffer,&outputBuffer);
    FE_Skip_Normalize(inputBuffer, outputBuffer, varIndex);
    #endif

    for(int i = 0; i < FE_FEATURE_SIZE_PER_FRAME; i++)
    {
        extractedFeatures[i] = (int8_t)outputBuffer[i];
    }

    return;
}
