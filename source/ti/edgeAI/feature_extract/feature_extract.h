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


#ifndef FEATURE_EXTRACT_H_
#define FEATURE_EXTRACT_H_

#include "model/user_input_config.h"
#include "model/tvmgen_default.h"

/* ARM CMSIS-DSP Header files */
#include "arm_const_structs.h"
#include "arm_math.h"


q15_t scratchBuffer1[2*FE_FRAME_SIZE];
q15_t scratchBuffer2[2*FE_FRAME_SIZE];

#if defined(FE_RFFT) || defined(FE_PIR)
arm_rfft_instance_q15 varInstRfftQ15;
#endif



/**
 * @brief Initialize the feature extraction module.
 *
 * This function initializes the feature extraction module based on the defines in user_input_config file.
 *
 * @return void
 */
void FE_init(void);


/**
 * @brief Process the input data to extract the features.
 *
 * This function takes the raw input data and processes it to extract the features.
 * The raw input data is expected to be in the format of a q15_t array.
 * The extracted features are stored in the output array.
 *
 * @param rawInput The raw input data to be processed.
 * @param extractedFeatures The output array to store the extracted features.
 * @param varIndex The index of the variable for which raw data is being processed for feature extraction. 
 *
 * @return void
 */
void FE_process(q15_t* rawInput, int8_t* extractedFeatures, uint8_t varIndex);

#endif /* FEATURE_EXTRACT_H_ */
