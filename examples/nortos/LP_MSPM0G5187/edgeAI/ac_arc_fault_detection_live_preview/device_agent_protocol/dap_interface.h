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

/*! DAP API Version */
#define API_VERSION                  0x01
/* Note: Changing SDK version to 0x02 and 0x08 causes issue */
/*! SDK Version (Major)*/
#define SDK_MAJOR_VERSION            0x05
/*! SDK Version (Minor)*/
#define SDK_MINOR_VERSION            0x04

/*! Capability bits flag */
/*! Capability - CRC Enabled bit */
#define CAP_CRC_ENABLED               0
/*! Capability - Bidirectional Enabled bit */
#define CAP_BIDIRECTIONAL_ENABLED     1
/*! Capability - Big Endian Enabled bit */
#define CAP_BIG_ENDIAN_ENABLED        1
/*! Capability - Big Upload Enabled bit */
#define CAP_BIG_UPLOAD_ENABLED        1
/*! Capability - Big Inference Enabled bit */
#define CAP_BIG_INFERENCE_ENABLED     1

/*! Capability bits offset */
/*! Capability - CRC Enabled bit offset */
#define CAP_CRC_BIT_OFFSET            4
/*! Capability - Bidirectional Enabled bit offset */
#define CAP_BIDIRECTIONAL_BIT_OFFSET  3
/*! Capability - Big Endian Enabled bit offset */
#define CAP_BIG_ENDIAN_BIT_OFFSET     2
/*! Capability - Big Upload Enabled bit offset */
#define CAP_BIG_UPLOAD_BIT_OFFSET     1
/*! Capability - Big Inference Enabled bit offset */
#define CAP_BIG_INFERENCE_BIT_OFFSET  0

/*! Total number of sensors */
#define TOTAL_SENSOR_COUNT            1
/*! Total number of models */
#define TOTAL_MODEL_COUNT             1
/*! Total number of classes  */
#define TOTAL_CLASS_COUNT             2
/*! Total number of inference values */
#define TOTAL_INFERENCE_VALUES        (1 + TOTAL_CLASS_COUNT)
/*! Inference data type */
#define INFERENCE_DATA_TYPE           (DATA_FORMAT_INT8)
/*! Total number of properties */
#define TOTAL_PROPERTY_COUNT          1

/*! Buffer Size (Note : Change if required) */
/*! Max size of buffer used for Sensor Info */
#define SENSOR_INFO_MAX_SIZE          (400)
/*! Max size of buffer used for Inference Value Info*/
#define INFERENCE_VALUE_MAX_SIZE      (100)
/*! Max size of buffer used for Property name */
#define PROPERTY_NAME_MAX_SIZE        (50)
/*! Max size of buffer used for Model Info */
#define MODEL_INFO_MAX_SIZE           (400)

/*!
 *  @brief      Receive Data Response Command byte Channels
 *
 *  This enumeration defines the Receive Data Response Command byte Channel types.
*/
typedef enum {
    /*! Response Command byte channel - Sensor Signal*/ 
    Resp_CMD_Channel_sensor_signal = 0x01,
    /*! Response Command byte channel - Inferencing Signal*/
    Resp_CMD_Channel_inferencing_signal = 0x03,
    /*! Response Command byte channel - Inferencing Result*/
    Resp_CMD_Channel_inferencing_result = 0x04,
    /*! Response Command byte channel - Inferencing Value*/
    Resp_CMD_Channel_inferencing_value = 0x05,
    /*! Response Command byte channel - Inferencing Log*/
    Resp_CMD_Channel_inferencing_log = 0x06
} Resp_CMD_Channel;

/*!
 *  @brief      Serial Commands
 *
 *  This enumeration defines all the Serial Commands supported in Device Agent Protocol.
*/
typedef enum {
    /*! Serial Command - Get Capabilities*/
    Serial_CMD_getCapabilities = 0x01,
    /*! Serial Command - List Sensors*/
    Serial_CMD_listSensors = 0x02,
    /*! Serial Command - Configure Pipeline*/
    Serial_CMD_configurePipeline = 0x03,
    /*! Serial Command - List Models*/
    Serial_CMD_listModels = 0x04,
    /*! Serial Command - Remove Model*/
    Serial_CMD_removeModel = 0x05,
    /*! Serial Command - Start of Model Upload*/
    Serial_CMD_startOfModelUpload = 0x06,
    /*! Serial Command - End of Model Upload*/
    Serial_CMD_endOfModelUpload = 0x07,
    /*! Serial Command - Start Streaming*/
    Serial_CMD_startStreaming = 0x08,
    /*! Serial Command - Stop Streaming*/
    Serial_CMD_stopStreaming = 0x09,
    /*! Serial Command - List Inferencing Values*/
    Serial_CMD_listInferencingValues = 0x0a,
    /*! Serial Command - Read Property*/
    Serial_CMD_readProperty = 0x0c,
    /*! Serial Command - Write Property*/
    Serial_CMD_writeProperty = 0x0d,
    /*! Serial Command - List Properties*/
    Serial_CMD_listProperties = 0x0e,
    /*! Serial Command - Send Data*/
    Serial_CMD_sendData = 0x10
} Serial_CMD;

/*!
 *  @brief      Serial Responses
 *
 *  This enumeration defines all the Serial Responses for the commands supported in Device Agent Protocol.
*/
typedef enum {
    /*! Serial Response - Error */
    Serial_Resp_Error = 0x00,
    /*! Serial Response - Get Capabilities*/
    Serial_Resp_getCapabilities = 0x01,
    /*! Serial Response - List Sensors*/
    Serial_Resp_listSensors = 0x02,
    /*! Serial Response - Configure Pipeline*/
    Serial_Resp_configurePipeline = 0x03,
    /*! Serial Response - List Models*/
    Serial_Resp_listModels = 0x04,
    /*! Serial Response - Remove Model*/
    Serial_Resp_removeModel = 0x05,
    /*! Serial Response - Start of Model Upload*/
    Serial_Resp_startOfModelUpload = 0x06,
    /*! Serial Response - End of Model Upload*/
    Serial_Resp_endOfModelUpload = 0x07,
    /*! Serial Response - Start Streaming*/
    Serial_Resp_startStreaming = 0x08,
    /*! Serial Response - Stop Streaming*/
    Serial_Resp_stopStreaming = 0x09,
    /*! Serial Response - List Inferencing Values*/
    Serial_Resp_listInferencingValues = 0x0a,
    /*! Serial Response - Read Property*/
    Serial_Resp_readProperty = 0x0c,
    /*! Serial Response - Write Property*/
    Serial_Resp_writeProperty = 0x0d,
    /*! Serial Response - List Properties*/
    Serial_Resp_listProperties = 0x0e,
    /*! Serial Response - Receive Data*/
    Serial_Resp_receiveData = 0x10
} Serial_Resp;

/*!
 *  @brief      Sensor Type
 *
 *  This enumeration defines all the Sensor Types supported in Device Agent Protocol.
*/
typedef enum {
    /*! Sensor Type - Image */
    SENSOR_TYPE_IMAGE = 0x01,
    /*! Sensor Type - Audio */
    SENSOR_TYPE_AUDIO = 0x02,
    /*! Sensor Type - Scalar */
    SENSOR_TYPE_SCALAR = 0x03,
    /*! Sensor Type - Vector */
    SENSOR_TYPE_VECTOR = 0x04,
    /*! Sensor Type - Array */
    SENSOR_TYPE_ARRAY = 0x05,
    /*! Sensor Type - Arc Fault */
    SENSOR_TYPE_ARC_FAULT = 0x06
} SensorType;

/*!
 *  @brief      Data Format
 *
 *  This enumeration defines all the Data Formats supported in Device Agent Protocol.
*/
typedef enum {
    /*! Data Format - Int8 */
    DATA_FORMAT_INT8 = 0x01,
    /*! Data Format - Int16 */
    DATA_FORMAT_INT16 = 0x02,
    /*! Data Format - Int32 */
    DATA_FORMAT_INT32 = 0x03,
    /*! Data Format - UInt8 */
    DATA_FORMAT_UINT8 = 0x04,
    /*! Data Format - UInt16 */
    DATA_FORMAT_UINT16 = 0x05,
    /*! Data Format - UInt32 */
    DATA_FORMAT_UINT32 = 0x06,
    /*! Data Format - Float16 */
    DATA_FORMAT_FLOAT16 = 0x07,
    /*! Data Format - Float32 */
    DATA_FORMAT_FLOAT32 = 0x08,
    /*! Data Format - Float64 */
    DATA_FORMAT_FLOAT64 = 0x09,
    /*! Data Format - JPEG */
    DATA_FORMAT_JPEG = 0x0a    
} dataFormat;

/*!
 *  @brief      Sensor Info
 *
 *  This structure defines all the Sensor Info supported in Device Agent Protocol.
*/
typedef struct {
    /*! Name of the sensor */
    char name[50];
    /*! Type of the sensor */
    SensorType type;
    /*! Data format of the sensor */
    dataFormat format;
    /*! Data label */
    char label[50];
    /*! Misc details for the sensor (Optional details can be added in this field)*/
    char misc[100];
} SensorInfo;

/*!
 *  @brief      Model Info
 *
 *  This structure defines all the Model Info supported in Device Agent Protocol.
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

/*!
 *  @brief      Inference Value Info
 *
 *  This structure defines all the Inference Value Info supported in Device Agent Protocol.
*/
typedef struct {
    /* Name of the inference value */
    char name[100];
} InferenceValueInfo;

typedef union {    
    uint8_t  u8;
    uint16_t u16;
    uint32_t u32;    
} PropertyValue;

/*!
 *  @brief      Property Info
 *
 *  This structure defines all the Property Info supported in Device Agent Protocol.
*/
typedef struct {
    /*! Name of the property */
    char name[40];
    /*! Data format of the property */
    dataFormat type;
    /*! Value of the property */
    PropertyValue value;
} PropertyInfo;

/*!
 *  @brief      Pipeline Modes
 *
 *  This enumeration defines all the Pipeline Modes supported in Device Agent Protocol.
*/
typedef enum {
    /*! Pipeline Mode - Data Acquisition */
    PIPELINE_MODE_DATA_ACQUISITION = 0x01,
    /*! Pipeline Mode - Sensor Inference */
    PIPELINE_MODE_SENSOR_INFERENCE = 0x02,
    /*! Pipeline Mode - Host Inference */
    PIPELINE_MODE_HOST_INFERENCE = 0x03,
    /*! Pipeline Mode - Loopback */
    PIPELINE_MODE_LOOPBACK = 0x04
} PipelineMode;

/*!
 *  @brief      Pipeline Config
 *
 *  This structure defines all the Pipeline Config supported in Device Agent Protocol.
*/
typedef struct {
    /* Mode of the pipeline */
    PipelineMode mode;
    /* Model Index */
    uint8_t modelIndex;
    /* Sensor Index */
    uint8_t sensorIndex[TOTAL_SENSOR_COUNT];
} PipelineConfig;


/*! Array of Sensor Info */
extern const SensorInfo* const gSensors[TOTAL_SENSOR_COUNT];
/*! Array of Model Info */
extern const ModelInfo* const gModels[TOTAL_MODEL_COUNT];
/*! Array of Inference Values */
extern const char *const gInferenceValues[TOTAL_INFERENCE_VALUES];
/*! Array of Properties */
extern PropertyInfo* gProperties[TOTAL_PROPERTY_COUNT];
/*!  Defines PipelineConfig struct*/
extern PipelineConfig gPipelineConfig;
/*! Device Name */
extern const char deviceName[];
/*! Flag to start streaming data */
extern uint8_t gStartStream;

#endif /* DAP_INTERFACE_H_ */
