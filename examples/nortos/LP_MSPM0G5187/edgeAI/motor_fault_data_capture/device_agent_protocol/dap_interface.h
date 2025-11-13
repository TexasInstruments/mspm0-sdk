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

#ifndef DAP_INTERFACE_H_
#define DAP_INTERFACE_H_

#include <stdint.h>

/**
 * @file dap_interface.h
 * @brief Device Agent Protocol (DAP) interface definitions and metadata.
 *
 * This header declares protocol constants, data types and global metadata
 * used by the device agent that communicates with the host (e.g. live-preview
 * and data capture tools). The definitions here control capabilities, command
 * encodings, sensor/model metadata and pipeline configuration.
 */

/** @name API / SDK version information
 * @{
 */
/** @brief DAP API version supported by this firmware. */
#define API_VERSION                  0x01
/** @brief SDK major version (note: changing may affect host compatibility). */
#define SDK_MAJOR_VERSION            0x05
/** @brief SDK minor version. */
#define SDK_MINOR_VERSION            0x04
/** @} */

/** @name Capability bit flags and offsets
 * @{
 */
/** @brief Capability bit: CRC support enabled. */
#define CAP_CRC_ENABLED               0
/** @brief Capability bit: Bidirectional host/device interactions enabled. */
#define CAP_BIDIRECTIONAL_ENABLED     1
/** @brief Capability bit: Big-endian encoding support. */
#define CAP_BIG_ENDIAN_ENABLED        1
/** @brief Capability bit: Big uploads supported. */
#define CAP_BIG_UPLOAD_ENABLED        1
/** @brief Capability bit: Big inference payloads supported. */
#define CAP_BIG_INFERENCE_ENABLED     1

/** @brief Bit offset for CRC capability. */
#define CAP_CRC_BIT_OFFSET            4
/** @brief Bit offset for bidirectional capability. */
#define CAP_BIDIRECTIONAL_BIT_OFFSET  3
/** @brief Bit offset for big endian capability. */
#define CAP_BIG_ENDIAN_BIT_OFFSET     2
/** @brief Bit offset for big upload capability. */
#define CAP_BIG_UPLOAD_BIT_OFFSET     1
/** @brief Bit offset for big inference capability. */
#define CAP_BIG_INFERENCE_BIT_OFFSET  0
/** @} */

/** @name Counts and limits
 * @{
 */
/** @brief Number of sensors exposed by this device. */
#define TOTAL_SENSOR_COUNT            1
/** @brief Number of models available on device (0 if none). */
#define TOTAL_MODEL_COUNT             0
/** @brief Number of inference value entries (per model output). */
#define TOTAL_INFERENCE_VALUES        0
/** @brief Default inference data type. */
#define INFERENCE_DATA_TYPE           (DATA_FORMAT_INT8)
/** @brief Number of properties exposed by device. */
#define TOTAL_PROPERTY_COUNT          1

/** @brief Maximum bytes for a serialized SensorInfo string. */
#define SENSOR_INFO_MAX_SIZE          (400)
/** @brief Maximum bytes for an inference value name. */
#define INFERENCE_VALUE_MAX_SIZE      (100)
/** @brief Maximum length for a property name. */
#define PROPERTY_NAME_MAX_SIZE        (50)
/** @brief Maximum bytes reserved for ModelInfo misc string. */
#define MODEL_INFO_MAX_SIZE           (50)
/** @} */

/**
 * @enum Resp_CMD_Channel
 * @brief DAP receive-data channel identifiers used in response framing.
 *
 * These values map to the channel field used in the host/device framing for
 * streamed payloads (sensor samples, inference results, logs, etc).
 */
typedef enum { 
    Resp_CMD_Channel_sensor_signal = 0x01,       /**< Sensor streaming channel */
    Resp_CMD_Channel_inferencing_signal = 0x03,  /**< Inference streaming channel */
    Resp_CMD_Channel_inferencing_result = 0x04,  /**< Inference result channel */
    Resp_CMD_Channel_inferencing_value = 0x05,   /**< Inference value channel */
    Resp_CMD_Channel_inferencing_log = 0x06      /**< Logging / textual output channel */
} Resp_CMD_Channel;

/**
 * @enum Serial_CMD
 * @brief Command bytes supported by the Device Agent Protocol.
 *
 * Host sends one of these command bytes to request capabilities, list sensors,
 * start/stop streaming, upload models, read/write properties, etc.
 */
typedef enum {
    Serial_CMD_getCapabilities      = 0x01, /**< Request device capability bitmap */
    Serial_CMD_listSensors          = 0x02, /**< Request list of available sensors */
    Serial_CMD_configurePipeline    = 0x03, /**< Configure pipeline (mode, model selection, etc) */
    Serial_CMD_listModels           = 0x04, /**< Request list of uploaded models on device */
    Serial_CMD_removeModel          = 0x05, /**< Remove a model from device storage */
    Serial_CMD_startOfModelUpload   = 0x06, /**< Begin model upload sequence */
    Serial_CMD_endOfModelUpload     = 0x07, /**< End model upload sequence */
    Serial_CMD_startStreaming       = 0x08, /**< Command to start streaming sensor or inference data */
    Serial_CMD_stopStreaming        = 0x09, /**< Command to stop streaming */
    Serial_CMD_listInferencingValues= 0x0a, /**< Request list of inferencing output value names */
    Serial_CMD_readProperty         = 0x0c, /**< Read a device property */
    Serial_CMD_writeProperty        = 0x0d, /**< Write/update a device property */
    Serial_CMD_listProperties       = 0x0e, /**< Request list of properties */
    Serial_CMD_sendData             = 0x10  /**< Send raw data payload to device (for loopback / uploads) */
} Serial_CMD;

/**
 * @enum Serial_Resp
 * @brief Response bytes returned by the Device Agent Protocol.
 *
 * Device uses these response codes to acknowledge commands or return results.
 */
typedef enum {
    Serial_Resp_Error                 = 0x00, /**< Generic error / negative acknowledgement */
    Serial_Resp_getCapabilities       = 0x01, /**< Response containing capability bitmap */
    Serial_Resp_listSensors           = 0x02, /**< Response listing sensors */
    Serial_Resp_configurePipeline     = 0x03, /**< Acknowledge pipeline configuration */
    Serial_Resp_listModels            = 0x04, /**< Response listing models */
    Serial_Resp_removeModel           = 0x05, /**< Acknowledge model removal */
    Serial_Resp_startOfModelUpload    = 0x06, /**< Acknowledge start of model upload */
    Serial_Resp_endOfModelUpload      = 0x07, /**< Acknowledge end of model upload */
    Serial_Resp_startStreaming        = 0x08, /**< Acknowledge start streaming */
    Serial_Resp_stopStreaming         = 0x09, /**< Acknowledge stop streaming */
    Serial_Resp_listInferencingValues = 0x0a, /**< Response listing inference value names */
    Serial_Resp_readProperty          = 0x0c, /**< Response with property value */
    Serial_Resp_writeProperty         = 0x0d, /**< Acknowledge property write */
    Serial_Resp_listProperties        = 0x0e, /**< Response listing properties */
    Serial_Resp_receiveData           = 0x10  /**< Frame containing received data payload */
} Serial_Resp;

/**
 * @enum SensorType
 * @brief Types of sensors that can be described by SensorInfo.
 */
typedef enum {
    SENSOR_TYPE_IMAGE = 0x01,   /**< Image sensor (frames) */
    SENSOR_TYPE_AUDIO = 0x02,   /**< Audio sensor (waveforms) */
    SENSOR_TYPE_SCALAR = 0x03,  /**< Scalar sensor (single value) */
    SENSOR_TYPE_VECTOR = 0x04,  /**< Vector sensor (multiple channels per sample) */
    SENSOR_TYPE_ARRAY = 0x05,   /**< Array formatted data */
    SENSOR_TYPE_ARC_FAULT = 0x06/**< Arc-fault specific sensor type */
} SensorType;

/**
 * @enum dataFormat
 * @brief Supported data formats for sensor, property and inference values.
 */
typedef enum {
    DATA_FORMAT_INT8    = 0x01, /**< 8-bit signed integer */
    DATA_FORMAT_INT16   = 0x02, /**< 16-bit signed integer */
    DATA_FORMAT_INT32   = 0x03, /**< 32-bit signed integer */
    DATA_FORMAT_UINT8   = 0x04, /**< 8-bit unsigned integer */
    DATA_FORMAT_UINT16  = 0x05, /**< 16-bit unsigned integer */
    DATA_FORMAT_UINT32  = 0x06, /**< 32-bit unsigned integer */
    DATA_FORMAT_FLOAT16 = 0x07, /**< 16-bit floating point (half) */
    DATA_FORMAT_FLOAT32 = 0x08, /**< 32-bit floating point (single) */
    DATA_FORMAT_FLOAT64 = 0x09, /**< 64-bit floating point (double) */
    DATA_FORMAT_JPEG    = 0x0a  /**< JPEG-encoded image payload */
} dataFormat;

/**
 * @struct SensorInfo
 * @brief Metadata describing a sensor exposed by the device agent.
 *
 * Fields:
 *  - name: human readable name for the sensor
 *  - type: sensor type (SensorType)
 *  - format: data element format (dataFormat)
 *  - label: optional labels (e.g. axis names) serialized as a small string
 *  - misc: optional JSON-like metadata (sample rate, units, etc)
 */
typedef struct {
    /* Name of the sensor */
    char name[50];
    /* Type of the sensor */
    SensorType type;
    /* Data format of the sensor */
    dataFormat format;
    /* Data label */
    char label[50];
    /* Misc details for the sensor (Optional details can be added in this field)*/
    char misc[100];
} SensorInfo;

/**
 * @struct ModelInfo
 * @brief Metadata describing a model available on the device.
 *
 * Fields:
 *  - name: model name
 *  - task: model task description
 *  - projectId: project identifier
 *  - misc: optional metadata (class maps, thresholds, version etc)
 */
typedef struct {
    /* Name of the model */
    char name[50];
    /* Task that this model was trained for */
    char task[50];
    /* Idenitifier for the project */
    char projectId[50];
    /* Misc details for the model (Optional details can be added in this field)*/    
    char misc[250];
} ModelInfo;

/**
 * @struct InferenceValueInfo
 * @brief Metadata describing a single inference value (name only).
 */
typedef struct {
    /* Name of the inference value */
    char name[100];
} InferenceValueInfo;

/**
 * @union PropertyValue
 * @brief Union to hold various sized integer property values.
 *
 * Use the appropriate field according to the PropertyInfo.type.
 */
typedef union {
    uint8_t  u8;   /**< 8-bit unsigned */
    uint16_t u16;  /**< 16-bit unsigned */
    uint32_t u32;  /**< 32-bit unsigned */
    uint64_t u64;  /**< 64-bit unsigned */
} PropertyValue;

/**
 * @struct PropertyInfo
 * @brief Describe a configurable/queryable property exposed by the device.
 *
 * Fields:
 *  - name: property identifier string
 *  - type: data format for this property
 *  - value: current value (use union to store correct width)
 */
typedef struct {
    /* Name of the property */
    char name[40];
    /* Data format of the property */
    dataFormat type;
    /* Value of the property */
    PropertyValue value;
} PropertyInfo;

/**
 * @enum PipelineMode
 * @brief Operation modes for the device pipeline.
 *
 * - PIPELINE_MODE_DATA_ACQUISITION: stream raw sensor data to host
 * - PIPELINE_MODE_SENSOR_INFERENCE: perform inference on device from sensor data
 * - PIPELINE_MODE_HOST_INFERENCE: stream features to host for remote inference
 * - PIPELINE_MODE_LOOPBACK: return received data for test/debug
 */
typedef enum {
    PIPELINE_MODE_DATA_ACQUISITION   = 0x01, /**< Stream raw sensor samples to host */
    PIPELINE_MODE_SENSOR_INFERENCE   = 0x02, /**< Run inference on-device and stream results */
    PIPELINE_MODE_HOST_INFERENCE     = 0x03, /**< Stream features to host for inference there */
    PIPELINE_MODE_LOOPBACK           = 0x04  /**< Loopback mode: echo received data for testing */
} PipelineMode;

/**
 * @struct PipelineConfig
 * @brief Current pipeline configuration used by device agent.
 *
 * Fields:
 *  - mode: pipeline operation mode (PipelineMode)
 *  - modelIndex: index of the active model for inference
 *  - sensorIndex: mapping of sensor indices used by pipeline
 */
typedef struct {
    /* Mode of the pipeline */
    PipelineMode mode;
    /* Model Index */
    uint8_t modelIndex;
    /* Sensor Index */
    uint8_t sensorIndex[TOTAL_SENSOR_COUNT];
} PipelineConfig;

/* Externs exported for use by application and protocol handlers */

/** @brief Array of pointers to SensorInfo for all sensors on device. */
extern const SensorInfo* const gSensors[TOTAL_SENSOR_COUNT];
/** @brief Array of pointers to ModelInfo for all models on device. */
extern const ModelInfo* const gModels[TOTAL_MODEL_COUNT];
/** @brief Array of inference value names (strings). */
extern const char *const gInferenceValues[TOTAL_INFERENCE_VALUES];
/** @brief Array of pointers to PropertyInfo instances exposed by device. */
extern PropertyInfo* gProperties[TOTAL_PROPERTY_COUNT];
/** @brief Current pipeline configuration instance. */
extern PipelineConfig gPipelineConfig;
/** @brief Optional device name string (defined when DEVICE_NAME is set). */
extern const char deviceName[];
/** @brief Flag indicating start of streaming requested by host. */
extern uint8_t gStartStream;

#endif /* USER_INTERFACE_H_ */