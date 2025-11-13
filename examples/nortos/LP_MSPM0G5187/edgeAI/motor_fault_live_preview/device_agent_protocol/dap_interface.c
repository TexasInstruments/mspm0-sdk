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

#include "dap_interface.h"

// Inner macro: Converts its argument to a string literal.
#define STRINGIFY_HELPER(x) #x

// Outer macro: Expands its argument before passing it to STRINGIFY_HELPER.
#define STRINGIFY(x) STRINGIFY_HELPER(x)

#ifdef DEVICE_NAME
const char deviceName[] = STRINGIFY(DEVICE_NAME);
#endif

const SensorInfo funcGen = {
    .name = "Vib Sensor",
    .type = SENSOR_TYPE_VECTOR,
    .format = DATA_FORMAT_UINT16,
    .label = "[\"x\", \"y\", \"z\"]",
    .misc = "\"sampleRate\":1024,\"columns\":3"
};

/**
 * @brief Array of pointers to SensorInfo structures.
 *
 * This array contains pointers to all sensors exposed by the device agent.
 * The order and count are defined by TOTAL_SENSOR_COUNT.
 */
const SensorInfo* const gSensors[TOTAL_SENSOR_COUNT] = {&funcGen};

/**
 * @brief Model metadata for the onboard inference model.
 *
 * name      : Unique model instance name.
 * task      : Logical task name associated with the model.
 * projectId : Project identifier string.
 * misc      : JSON style string mapping class IDs to human readable names.
 */
const ModelInfo model1 = {
    .name = "MotorFault_model_1_t",
    .task = "MotorFault_model",
    .projectId = "Project_Name",
    .misc = "\"classIdToName\":{\"0\": \"Normal\", \"1\": \"Blade Damage\", \"2\": \"Blade Imbalance\", \"3\": \"others\"}"
};

/**
 * @brief Array of pointers to ModelInfo structures.
 *
 * Contains metadata for all models available on the device.
 */
const ModelInfo* const gModels[TOTAL_MODEL_COUNT] = {&model1};

/**
 * @brief Names of the inference output values provided by the device.
 *
 * The order here must match the inference payload layout produced by the
 * device agent (e.g. inference_result followed by per-class probabilities).
 */
const char *const gInferenceValues[TOTAL_INFERENCE_VALUES] = {
    "inference_result",
    "class0_probability",
    "class1_probability",
    "class2_probability",
    "class3_probability"
};

/**
 * @brief Property describing the number of samples to acquire or currently held.
 *
 * name  : Property identifier "samples".
 * type  : DATA_FORMAT_INT16 indicates 16-bit signed integer value.
 * value : Initial value (0).
 */
PropertyInfo property1 = {
    .name = "samples",
    .type = DATA_FORMAT_UINT16,
    .value.u16 = 0,
};

/**
 * @brief Array of pointers to PropertyInfo structures.
 *
 * Lists the configurable/queryable properties exposed by the device agent.
 */
PropertyInfo* gProperties[TOTAL_PROPERTY_COUNT] = {&property1};

/**
 * @brief Pipeline configuration describing current acquisition and inference mode.
 *
 * mode       : PIPELINE_MODE_DATA_ACQUISITION (data acquisition mode).
 * modelIndex : Index of model to use for inference (0 = first model).
 */
PipelineConfig gPipelineConfig = {
    .mode = PIPELINE_MODE_DATA_ACQUISITION,
    .modelIndex = 0
};

const char *const gInferenceValues[TOTAL_INFERENCE_VALUES];