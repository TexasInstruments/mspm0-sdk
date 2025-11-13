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
#include "dap_core.h"
#include <string.h>
#include <stdio.h>

/**
 * @brief Prepares frame header for transmission
 * @param[in] txMsg    A pointer to transmit buffer
 * @param[in] cmdByte  Command byte
 */
static void DAP_prepareTxHeader(BufferInfo *txMsg, uint8_t cmdByte);

/**
 * @brief Prepares frame trailer for transmission
 * @param[in] txMsg    A pointer to transmit buffer
 * @param[in] isCrc    Flag to check if CRC is enabled 
 */
static void DAP_prepareTxTrailer(BufferInfo *txMsg,_Bool isCrc);

/**
 * @brief Prepares payload length for transmission
 * @param[in] txMsg    A pointer to transmit buffer
 * @param[in] len      Payload length
 */
static void DAP_preparePayloadLen(BufferInfo *txMsg,uint32_t len);

/**
 * @brief Prepares payload for transmission
 * @param[in] txMsg         A pointer to transmit buffer
 * @param[in] payloadData   A pointer to payload data
 * @param[in] len           payload length
 */
static void DAP_preparePayload(BufferInfo *txMsg, uint8_t* payloadData, uint32_t len);

/**
 * @brief Prepares a string with sensor information
 * @param[in] sensorIndex    Sensor index
 * @param[in] sensorInfoString    A pointer to sensor information string
 */
static void DAP_getSensorInfo(uint8_t sensorIndex,char* sensorInfoString);

/**
 * @brief Sends Error Type 
 * @param[in] UART_handle    A pointer to UART_Instance
 */
static void DAP_sendErrorType(UART_Instance *UART_handle);

/**
 * @brief Verifies if the received buffer is valid
 * @param[in] UART_handle    A pointer to UART_Instance
 */
static void DAP_verifyBuffer(UART_Instance *UART_handle);

/**
 * @brief Executes the received command
 * @param[in] UART_handle    A pointer to UART_Instance
 */
static void DAP_serviceCommand(UART_Instance *UART_handle);

/**
 * @brief Sends response to Get Capabilities command
 * @param[in] UART_handle    A pointer to UART_Instance
 */
static void DAP_getCapabilities(UART_Instance* UART_handle);

/**
 * @brief Configures pipeline
 * @param[in] UART_handle    A pointer to UART_Instance
 */
static void DAP_configurePipeline(UART_Instance* UART_handle);

/**
 * @brief Sends sensor information 
 * @param[in] UART_handle    A pointer to UART_Instance
 * @param[in] sensorIndex    Sensor index
 */
static void DAP_listSensor(UART_Instance* UART_handle, uint8_t sensorIndex);

/**
 * @brief Sends response to List Sensor command
 * @param[in] UART_handle    A pointer to UART_Instance
 */
static void DAP_listAllSensors(UART_Instance* UART_handle);

/**
 * @brief Prepares a string with sensor information
 * @param[in] modelIndex    Model index
 * @param[in] modelInfoString    A pointer to model information string
*/
static void DAP_getModelInfo(uint8_t modelIndex, char* modelInfoString);
/**
 * @brief Sends model information
 * @param[in] UART_handle    A pointer to UART_Instance
 * @param[in] modelIndex    Model index
 */
static void DAP_listModel(UART_Instance* UART_handle,uint8_t modelIndex);   

/**
 * @brief Sends response to List Models command
 * @param[in] UART_handle    A pointer to UART_Instance
 */
static void DAP_listAllModels(UART_Instance* UART_handle);


/**
 * @brief Sets a flag to start streaming data
 * @param[in] UART_handle    A pointer to UART_Instance
 */
static void DAP_startStreaming(UART_Instance* UART_handle);

/**
 * @brief Sets a flag to stop streaming data
 * @param[in] UART_handle    A pointer to UART_Instance
 */
static void DAP_stopStreaming(UART_Instance* UART_handle);

/**
 * @brief Sends Inference Value information
 * @param[in] UART_handle    A pointer to UART_Instance
 * @param[in] inferenceValueIndex    Inference value index
 */
static void DAP_listInference(UART_Instance* UART_handle, uint8_t inferenceValueIndex);

/**
 * @brief Sends response to List Inferencing Values command
 * @param[in] UART_handle    A pointer to UART_Instance
 */
static void DAP_listInferenceValues(UART_Instance* UART_handle);

/**
 * @brief Sends response to Read Property command
 * @param[in] UART_handle    A pointer to UART_Instance
 */
static void DAP_readProperty(UART_Instance* UART_handle);

/**
 * @brief Sends response to Write Property command
 * @param[in] UART_handle    A pointer to UART_Instance
 */
static void DAP_writeProperty(UART_Instance* UART_handle);

/**
 * @brief Sends property data
 * @param[in] UART_handle    A pointer to UART_Instance
 * @param[in] propertyIndex    Property index
 */
static void DAP_listProperty(UART_Instance* UART_handle, uint8_t propertyIndex);

/**
 * @brief Sends response to List Properties command
 * @param[in] UART_handle    A pointer to UART_Instance
 */
static void DAP_listProperties(UART_Instance* UART_handle);


/* Flag to start streaming data */
uint8_t gStartStream = 0;

/* Helper Function*/
uint8_t getDataLen(dataFormat dataType)
{
    if((dataType == DATA_FORMAT_UINT8) || (dataType == DATA_FORMAT_INT8))
    {
        return 1;
    }

    if((dataType == DATA_FORMAT_UINT16) || (dataType == DATA_FORMAT_INT16) || (dataType == DATA_FORMAT_FLOAT16))
    {
        return 2;
    } 

    if((dataType == DATA_FORMAT_UINT32) || (dataType == DATA_FORMAT_INT32) || (dataType == DATA_FORMAT_FLOAT32))
    {
        return 4;
    }

    if(dataType == DATA_FORMAT_FLOAT64)
    {
        return 8;
    }

    return 0;
}

void DAP_getFrameLength(UART_Instance *UART_handle)
{
    /* Check for Start Byte */
    if(UART_handle->rxMsg.buffer[START_IDX] != FRAME_START_BYTE)
    {
        UART_handle->rxMsg.len = 0;
        return;
    }

    /* Calculate Frame Length */
    UART_handle->rxMsg.len = UART_handle->rxMsg.buffer[LEN_IDX] + PACKET_FIXED_PART_SIZE;

    return;
}

static void DAP_prepareTxHeader(BufferInfo *txMsg, uint8_t cmdByte)
{
    /* Add Start Byte */
    txMsg->buffer[txMsg->ptr++] = FRAME_START_BYTE;

    /* Add Command Byte */
    txMsg->buffer[txMsg->ptr++] = cmdByte;

   return;
}

static void DAP_prepareTxTrailer(BufferInfo *txMsg,_Bool isCrc)
{
    if(isCrc)
    {
        /* Calculate CRC16 */
        /* From Start byte to end of data byte */
        uint8_t size = txMsg->ptr - PREAMBLE_BYTE_SIZE;
        /* Send address of startbyte */
        uint8_t checkSum = CRC_calc8(&txMsg->buffer[PREAMBLE_BYTE_SIZE],size);
        
        txMsg->buffer[txMsg->ptr++] = checkSum;
    }

    /* Add End byte */
    txMsg->buffer[txMsg->ptr++] = FRAME_END_BYTE;

    txMsg->len = txMsg->ptr;
    txMsg->ptr = 0;

    return;
}

static void DAP_preparePayloadLen(BufferInfo *txMsg,uint32_t len)
{    
    if(IN_RANGE(len, PAYLOAD_FIELD_1B_MIN_LEN, PAYLOAD_FIELD_1B_MAX_LEN))        
    {
        txMsg->buffer[txMsg->ptr++] = PAYLOAD_FIELD_1B_OFFSET + len;
    }

    else if(IN_RANGE(len, PAYLOAD_FIELD_2B_MIN_LEN, PAYLOAD_FIELD_2B_MAX_LEN))
    {
        uint16_t dapLen = PAYLOAD_FIELD_2B_OFFSET + len;        
        txMsg->buffer[txMsg->ptr++] = (dapLen>>8) & 0xFF;
        txMsg->buffer[txMsg->ptr++] = dapLen & 0xFF;
    }

    else if(IN_RANGE(len, PAYLOAD_FIELD_3B_MIN_LEN, PAYLOAD_FIELD_3B_MAX_LEN))
    {
        uint32_t dapLen = PAYLOAD_FIELD_3B_OFFSET + len;
        txMsg->buffer[txMsg->ptr++] = (dapLen>>16) & 0xFF;
        txMsg->buffer[txMsg->ptr++] = (dapLen>>8) & 0xFF;
        txMsg->buffer[txMsg->ptr++] = dapLen & 0xFF;        
    }
    
    return;
}

static void DAP_preparePayload(BufferInfo *txMsg, uint8_t* payloadData, uint32_t len)
{
    memcpy(txMsg->buffer + txMsg->ptr, payloadData, len);

    txMsg->ptr += len;

    return;
}

static void DAP_getSensorInfo(uint8_t sensorIndex,char* sensorInfoString)
{
    if(strlen(gSensors[sensorIndex]->misc) != 0)
    {
        sprintf(sensorInfoString, "{\"name\":\"%s\",\"type\":%d,\"dataFormat\":%d,\"labels\":%s,%s}", \
                gSensors[sensorIndex]->name, gSensors[sensorIndex]->type, gSensors[sensorIndex]->format, \
                gSensors[sensorIndex]->label, gSensors[sensorIndex]->misc);
    }

    else
    {
        sprintf(sensorInfoString, "{\"name\":\"%s\",\"type\":%d,\"dataFormat\":%d,\"labels\":%s}", \
                gSensors[sensorIndex]->name, gSensors[sensorIndex]->type, gSensors[sensorIndex]->format, \
                gSensors[sensorIndex]->label);
    }
  
    return ;
}
void DAP_processBuffer(UART_Instance *UART_handle)
{
    if( UART_handle->RxStatus == UART_STATUS_PROCESS_FRAME )
    {
        DAP_verifyBuffer(UART_handle);

        if( UART_handle->error == ERROR_TYPE_NONE )
        {
            DAP_serviceCommand(UART_handle);
        }

        /* Waits till end byte is stored in the buffer if buffering is out of sync */
        else if(UART_handle->error == ERROR_TYPE_MISMATCH_START_BYTE
                            || UART_handle->error == ERROR_TYPE_MISMATCH_END_BYTE )
        {
            UART_reSync(UART_handle);
            DAP_sendErrorType(UART_handle);
        }

        else
        {
            DAP_sendErrorType(UART_handle);
        }
    }
}

static void DAP_sendErrorType(UART_Instance *UART_handle)
{
    while(UART_handle->TxStatus == UART_STATUS_TX_TRANSMITTING)
    {};
    /* Clear Tx Buffer */
    UART_clearBuffer(&UART_handle->txMsg);

    /* Prepare Tx Header */
    DAP_prepareTxHeader(&UART_handle->txMsg, (Serial_Resp_Error));

    /* Add payload length */
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = 2;

    /* Add Error Type */
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++]
                                                    = UART_handle->rxMsg.buffer[CMD_IDX];

    /* Add Error Type */
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++]
                                                    = UART_handle->error;

    /* Prepare Tx Trailer */
    DAP_prepareTxTrailer(&UART_handle->txMsg,UART_handle->isCrc);


    UART_handle->TxStatus = UART_STATUS_TX_TRANSMITTING;

    /* Send Tx Buffer */
    UART_sendBuffer(&UART_handle->txMsg);
}

static void DAP_verifyBuffer(UART_Instance *UART_handle)
{
    /* Verify start byte */
    if(UART_handle->rxMsg.buffer[START_IDX] != FRAME_START_BYTE)
    {
        UART_handle->error = ERROR_TYPE_MISMATCH_START_BYTE;
        return ;
    }

    /* Verify end byte */
    if(UART_handle->rxMsg.buffer[UART_handle->rxMsg.len - END_OFFSET] != FRAME_END_BYTE)
    {
        UART_handle->error = ERROR_TYPE_MISMATCH_END_BYTE;
        return ;
    }

    UART_handle->error = ERROR_TYPE_NONE;
    return;
}

static void DAP_serviceCommand(UART_Instance *UART_handle)
{
    switch(UART_handle->rxMsg.buffer[CMD_IDX])
    {
        case Serial_CMD_getCapabilities: DAP_getCapabilities(UART_handle);
                                         break;

        case Serial_CMD_listSensors: DAP_listAllSensors(UART_handle);
                                     break;

        case Serial_CMD_configurePipeline: DAP_configurePipeline(UART_handle);
                                           break;

        case Serial_CMD_listModels: DAP_listAllModels(UART_handle);
                                    break;

        case Serial_CMD_startStreaming: DAP_startStreaming(UART_handle);
                                        break;

        case Serial_CMD_stopStreaming: DAP_stopStreaming(UART_handle);
                                       break;

        case Serial_CMD_listInferencingValues: DAP_listInferenceValues(UART_handle);
                                               break;

        case Serial_CMD_readProperty: DAP_readProperty(UART_handle);
                                      break;

        case Serial_CMD_writeProperty: DAP_writeProperty(UART_handle);
                                       break;

        case Serial_CMD_listProperties: DAP_listProperties(UART_handle);
                                        break;
    }
}

static void DAP_getCapabilities(UART_Instance* UART_handle)
{
    while(UART_handle->TxStatus == UART_STATUS_TX_TRANSMITTING)
    {};
    /* Clear Tx Buffer */
    UART_clearBuffer(&UART_handle->txMsg);

    /* Prepare Tx Header */
    DAP_prepareTxHeader(&UART_handle->txMsg, Serial_Resp_getCapabilities);

    /* Size of device information (API version, capability byte, SDK Major and Minor Version)
     * plus device name */
    uint16_t payloadLen = 4 + strlen(deviceName);

    DAP_preparePayloadLen(&UART_handle->txMsg,payloadLen);

    /* Capability Byte */
    uint8_t capabilityByte = ((CAP_CRC_ENABLED) << (CAP_CRC_BIT_OFFSET)) | 
                             ((CAP_BIDIRECTIONAL_ENABLED) << (CAP_BIDIRECTIONAL_BIT_OFFSET)) | 
                             ((CAP_BIG_ENDIAN_ENABLED) << (CAP_BIG_ENDIAN_BIT_OFFSET)) | 
                             ((CAP_BIG_UPLOAD_ENABLED) << (CAP_BIG_UPLOAD_BIT_OFFSET)) | 
                             ((CAP_BIG_INFERENCE_ENABLED) << (CAP_BIG_INFERENCE_BIT_OFFSET));

    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = API_VERSION;     
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = capabilityByte;
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = SDK_MAJOR_VERSION; 
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = SDK_MINOR_VERSION; 

    DAP_preparePayload(&UART_handle->txMsg,(uint8_t*)deviceName,strlen(deviceName));

    /* Prepare Tx Trailer */
    DAP_prepareTxTrailer(&UART_handle->txMsg,UART_handle->isCrc);


    UART_handle->TxStatus = UART_STATUS_TX_TRANSMITTING;

    /* Send Tx Buffer */
    UART_sendBuffer(&UART_handle->txMsg);
}


static void DAP_listSensor(UART_Instance* UART_handle, uint8_t sensorIndex)
{
    /* Clear Tx Buffer */
    while(UART_handle->TxStatus == UART_STATUS_TX_TRANSMITTING)
    {};

    UART_clearBuffer(&UART_handle->txMsg);

    /* Prepare Tx Header */
    DAP_prepareTxHeader(&UART_handle->txMsg, (Serial_Resp_listSensors));

    char sensorInfoString[SENSOR_INFO_MAX_SIZE];
    DAP_getSensorInfo(sensorIndex, sensorInfoString);

    /* Payload size is 2 bytes for sensor count and index plus sensor info */
    uint32_t payloadLen = 2 + strlen(sensorInfoString);

    DAP_preparePayloadLen(&UART_handle->txMsg,payloadLen);
    
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = TOTAL_SENSOR_COUNT;
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = sensorIndex;        

    DAP_preparePayload(&UART_handle->txMsg,(uint8_t *)sensorInfoString,strlen(sensorInfoString));
    
    /* Prepare Tx Trailer */
    DAP_prepareTxTrailer(&UART_handle->txMsg,UART_handle->isCrc);

    UART_handle->TxStatus = UART_STATUS_TX_TRANSMITTING;

    /* Send Tx Buffer */
    UART_sendBuffer(&UART_handle->txMsg);

}

static void DAP_listNone(UART_Instance* UART_handle, Serial_Resp cmd)
{
    /* Clear Tx Buffer */
    while(UART_handle->TxStatus == UART_STATUS_TX_TRANSMITTING)
    {};

    UART_clearBuffer(&UART_handle->txMsg);

    /* Prepare Tx Header */
    DAP_prepareTxHeader(&UART_handle->txMsg, (cmd));

    /* Payload size is 1 byte for count */
    uint32_t payloadLen = 1;

    DAP_preparePayloadLen(&UART_handle->txMsg,payloadLen);

    /* No data to send */
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = 0;

    /* Prepare Tx Trailer */
    DAP_prepareTxTrailer(&UART_handle->txMsg,UART_handle->isCrc);

    UART_handle->TxStatus = UART_STATUS_TX_TRANSMITTING;

    /* Send Tx Buffer */
    UART_sendBuffer(&UART_handle->txMsg);
}

static void DAP_listAllSensors(UART_Instance* UART_handle)
{
    if(TOTAL_SENSOR_COUNT == 0)
    {
        DAP_listNone(UART_handle,Serial_Resp_listSensors);
        return;
    }

    for(int i = 0; i < TOTAL_SENSOR_COUNT; i++)
    {
        DAP_listSensor(UART_handle, i);
    }
}

void DAP_receivedDataResponse(UART_Instance *UART_handle,Resp_CMD_Channel channel,uint32_t dataLen,uint8_t* dataPtr)
{
    /* Clear Tx Buffer */
    while(UART_handle->TxStatus == UART_STATUS_TX_TRANSMITTING)
    {};
    UART_clearBuffer(&UART_handle->txMsg);

    /* Prepare Tx Header */
    /**/
    DAP_prepareTxHeader(&UART_handle->txMsg, (Serial_Resp_receiveData | channel));

    DAP_preparePayloadLen(&UART_handle->txMsg,dataLen);

    DAP_preparePayload(&UART_handle->txMsg,dataPtr,dataLen);

    /* Prepare Tx Trailer */
    DAP_prepareTxTrailer(&UART_handle->txMsg,UART_handle->isCrc);


    UART_handle->TxStatus = UART_STATUS_TX_TRANSMITTING;

    /* Send Tx Buffer */
    UART_sendBuffer(&UART_handle->txMsg);
}

static void DAP_configurePipeline(UART_Instance* UART_handle)
{
    /* Configure Pipeline */
    uint8_t sensorIndexLen = UART_handle->rxMsg.buffer[LEN_IDX] - 2;

    gPipelineConfig.mode = UART_handle->rxMsg.buffer[3];
    gPipelineConfig.modelIndex = UART_handle->rxMsg.buffer[4];
    
    if(sensorIndexLen > TOTAL_SENSOR_COUNT)
        {
            return;
        }

    for(int i = 0 ; i < sensorIndexLen ; i++)
    {
        gPipelineConfig.sensorIndex[i] = UART_handle->rxMsg.buffer[5 + i];
    }

    while(UART_handle->TxStatus == UART_STATUS_TX_TRANSMITTING)
    {};

    /* Clear Tx Buffer */
    UART_clearBuffer(&UART_handle->txMsg);

    /* Prepare Tx Header */
    DAP_prepareTxHeader(&UART_handle->txMsg, Serial_CMD_configurePipeline);

    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = 0;

    /* Prepare Tx Trailer */
    DAP_prepareTxTrailer(&UART_handle->txMsg,UART_handle->isCrc);


    UART_handle->TxStatus = UART_STATUS_TX_TRANSMITTING;

    /* Send Tx Buffer */
    UART_sendBuffer(&UART_handle->txMsg);
}


static void DAP_getModelInfo(uint8_t modelIndex, char* modelInfoString)
{
    if(strlen(gModels[modelIndex]->misc) != 0)
    {
        sprintf(modelInfoString, "{\"name\":\"%s\",\"task\":\"%s\",\"projectId\":\"%s\",%s}", \
                gModels[modelIndex]->name, gModels[modelIndex]->task, gModels[modelIndex]->projectId, \
                gModels[modelIndex]->misc);
    }

    else
    {
        sprintf(modelInfoString, "{\"name\":\"%s\",\"task\":\"%s\",\"projectId\":\"%s\"}", \
                gModels[modelIndex]->name, gModels[modelIndex]->task, gModels[modelIndex]->projectId);        
    }
}

static void DAP_listModel(UART_Instance* UART_handle,uint8_t modelIndex)
{
    while(UART_handle->TxStatus == UART_STATUS_TX_TRANSMITTING)
    {};
    /* Clear Tx Buffer */
    UART_clearBuffer(&UART_handle->txMsg);

    /* Prepare Tx Header */
    DAP_prepareTxHeader(&UART_handle->txMsg, (Serial_Resp_listModels));

    char modelInfoString[MODEL_INFO_MAX_SIZE];
    DAP_getModelInfo(modelIndex, modelInfoString);

    /* Payload size is 2 bytes for model count and index plus model info */
    uint32_t payloadLen = 2 + strlen(modelInfoString);

    DAP_preparePayloadLen(&UART_handle->txMsg,payloadLen);

    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = TOTAL_MODEL_COUNT;
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = modelIndex;

    DAP_preparePayload(&UART_handle->txMsg,(uint8_t *)modelInfoString,strlen(modelInfoString));

    /* Prepare Tx Trailer */
    DAP_prepareTxTrailer(&UART_handle->txMsg,UART_handle->isCrc);


    UART_handle->TxStatus = UART_STATUS_TX_TRANSMITTING;

    /* Send Tx Buffer */
    UART_sendBuffer(&UART_handle->txMsg);
}

static void DAP_listAllModels(UART_Instance* UART_handle)
{
    if(TOTAL_MODEL_COUNT == 0)
    {
        DAP_listNone(UART_handle,Serial_Resp_listModels);
        return;
    }

    for(int i = 0; i < TOTAL_MODEL_COUNT; i++)
    {
        DAP_listModel(UART_handle, i);
    }
}

static void DAP_listInference(UART_Instance* UART_handle, uint8_t inferenceValueIndex)
{
    while(UART_handle->TxStatus == UART_STATUS_TX_TRANSMITTING)
    {};
    /* Clear Tx Buffer */
    UART_clearBuffer(&UART_handle->txMsg);

    /* Prepare Tx Header */
    DAP_prepareTxHeader(&UART_handle->txMsg, (Serial_Resp_listInferencingValues));

    char inferenceValue[INFERENCE_VALUE_MAX_SIZE];
    strcpy(inferenceValue, gInferenceValues[inferenceValueIndex]);

    /* Payload size is 3 bytes for count,index and data format plus size of inference value */
    uint32_t payloadLen = 3 + strlen(inferenceValue);
    DAP_preparePayloadLen(&UART_handle->txMsg,payloadLen);
    
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = TOTAL_INFERENCE_VALUES;
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = inferenceValueIndex;
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = INFERENCE_DATA_TYPE;
    
    DAP_preparePayload(&UART_handle->txMsg,(uint8_t *)inferenceValue,strlen(inferenceValue));

    /* Prepare Tx Trailer */
    DAP_prepareTxTrailer(&UART_handle->txMsg,UART_handle->isCrc);


    UART_handle->TxStatus = UART_STATUS_TX_TRANSMITTING;

    /* Send Tx Buffer */
    UART_sendBuffer(&UART_handle->txMsg);
}

static void DAP_listInferenceValues(UART_Instance* UART_handle)
{
    if(TOTAL_INFERENCE_VALUES == 0)
    {
        DAP_listNone(UART_handle,Serial_Resp_listInferencingValues);
        return;
    }

    for(int i = 0; i < TOTAL_INFERENCE_VALUES ;i++)
    {
        DAP_listInference(UART_handle, i);
    }
}

static void DAP_readProperty(UART_Instance* UART_handle)
{
    uint8_t propertyIndex = UART_handle->rxMsg.buffer[3];
    
    /* Check if the property exists */
    if(propertyIndex >= TOTAL_PROPERTY_COUNT)
    {    
        return;
    }

    while(UART_handle->TxStatus == UART_STATUS_TX_TRANSMITTING)
    {};

    /* Clear Tx Buffer */
    UART_clearBuffer(&UART_handle->txMsg);

    /* Prepare Tx Header */
    DAP_prepareTxHeader(&UART_handle->txMsg, (Serial_Resp_writeProperty));
    
    dataFormat dataType = gProperties[propertyIndex]->type;
    uint8_t dataLen = getDataLen(dataType);

    /* Payload size is 1 byte of property index plus data length */
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = 1 + dataLen;

    /* Property Index */
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = propertyIndex;
    
    if(dataLen == 1)
    {
        UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = gProperties[propertyIndex]->value.u8;
    }

    else if(dataLen == 2)
    {
        UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = (gProperties[propertyIndex]->value.u16 >> 8);
        UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = gProperties[propertyIndex]->value.u16;
    }

    else if(dataLen == 4)
    {
        UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = (gProperties[propertyIndex]->value.u32 >> 24);
        UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = (gProperties[propertyIndex]->value.u32 >> 16);
        UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = (gProperties[propertyIndex]->value.u32 >> 8);
        UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = gProperties[propertyIndex]->value.u32;
    }
    
    /* Prepare Tx Trailer */
    DAP_prepareTxTrailer(&UART_handle->txMsg,UART_handle->isCrc);

    UART_handle->TxStatus = UART_STATUS_TX_TRANSMITTING;

    /* Send Tx Buffer */
    UART_sendBuffer(&UART_handle->txMsg);
}

static void DAP_writeProperty(UART_Instance* UART_handle)
{           
    uint8_t propertyIndex = UART_handle->rxMsg.buffer[3];
    
    /* Check if the property exists */
    if(propertyIndex >= TOTAL_PROPERTY_COUNT)
    {
        return;
    }
    
    uint8_t dataLen = UART_handle->rxMsg.buffer[LEN_IDX] - 1;
    dataFormat dataType = gProperties[propertyIndex]->type;

    /* Check if the data received is of proper type */
    if(dataLen != getDataLen(dataType))
    {
        return;
    }
    
    if(dataLen == 1)
    {
        gProperties[propertyIndex]->value.u8 = UART_handle->rxMsg.buffer[4];
    }

    else if(dataLen == 2)
    {
        gProperties[propertyIndex]->value.u16 = ((UART_handle->rxMsg.buffer[4] << 8) \
                                          | UART_handle->rxMsg.buffer[5]);
    }

    else if(dataLen == 4)
    {
        gProperties[propertyIndex]->value.u32 = ((UART_handle->rxMsg.buffer[4] << 24) \
                                         | (UART_handle->rxMsg.buffer[5] << 16) \
                                         | (UART_handle->rxMsg.buffer[6] << 8) \
                                         |  UART_handle->rxMsg.buffer[7]);
    }
        
    while(UART_handle->TxStatus == UART_STATUS_TX_TRANSMITTING)
    {};

    /* Clear Tx Buffer */
    UART_clearBuffer(&UART_handle->txMsg);

    /* Prepare Tx Header */
    DAP_prepareTxHeader(&UART_handle->txMsg, (Serial_Resp_writeProperty));

    /* Payload size is 1 byte for count */
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = 1;
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = propertyIndex;

    DAP_prepareTxTrailer(&UART_handle->txMsg,UART_handle->isCrc);


    UART_handle->TxStatus = UART_STATUS_TX_TRANSMITTING;

    /* Send Tx Buffer */
    UART_sendBuffer(&UART_handle->txMsg);
}


static void DAP_listProperty(UART_Instance* UART_handle, uint8_t propertyIndex)
{
    while(UART_handle->TxStatus == UART_STATUS_TX_TRANSMITTING)
    {};
    /* Clear Tx Buffer */
    UART_clearBuffer(&UART_handle->txMsg);

    /* Prepare Tx Header */
    DAP_prepareTxHeader(&UART_handle->txMsg, (Serial_Resp_listProperties));

    char propertyName[PROPERTY_NAME_MAX_SIZE];
    strcpy(propertyName, gProperties[propertyIndex]->name);

    /* Payload size is 3 bytes for count,index and data format plus size of property name*/
    uint32_t payloadLen = 3 + strlen(propertyName);
    DAP_preparePayloadLen(&UART_handle->txMsg,payloadLen);

    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = TOTAL_PROPERTY_COUNT;
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = propertyIndex;
    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = gProperties[propertyIndex]->type;

    DAP_preparePayload(&UART_handle->txMsg,(uint8_t *)propertyName,strlen(propertyName));

    /* Prepare Tx Trailer */
    DAP_prepareTxTrailer(&UART_handle->txMsg,UART_handle->isCrc);


    UART_handle->TxStatus = UART_STATUS_TX_TRANSMITTING;

    /* Send Tx Buffer */
    UART_sendBuffer(&UART_handle->txMsg);    
}


static void DAP_listProperties(UART_Instance* UART_handle)
{
    if(TOTAL_PROPERTY_COUNT == 0)
    {
        DAP_listNone(UART_handle,Serial_Resp_listProperties);
        return;
    }


    for(int i = 0; i < TOTAL_PROPERTY_COUNT ; i++)
    {
        DAP_listProperty(UART_handle, i);
    }
    
}
static void DAP_startStreaming(UART_Instance* UART_handle)
{
    while(UART_handle->TxStatus == UART_STATUS_TX_TRANSMITTING)
    {};
    /* Clear Tx Buffer */
    UART_clearBuffer(&UART_handle->txMsg);

    /* Prepare Tx Header */
    DAP_prepareTxHeader(&UART_handle->txMsg, (Serial_Resp_startStreaming));

    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = 0x00;

    /* Prepare Tx Trailer */
    DAP_prepareTxTrailer(&UART_handle->txMsg,UART_handle->isCrc);


    UART_handle->TxStatus = UART_STATUS_TX_TRANSMITTING;

    /* Send Tx Buffer */
    UART_sendBuffer(&UART_handle->txMsg);

    /* Set the flag to start streaming data */
    gStartStream = 1;
}

static void DAP_stopStreaming(UART_Instance* UART_handle)
{
    while(UART_handle->TxStatus == UART_STATUS_TX_TRANSMITTING)
    {};
    /* Clear Tx Buffer */
    UART_clearBuffer(&UART_handle->txMsg);

    /* Prepare Tx Header */
    DAP_prepareTxHeader(&UART_handle->txMsg, (Serial_Resp_stopStreaming));

    UART_handle->txMsg.buffer[UART_handle->txMsg.ptr++] = 0x00;

    /* Prepare Tx Trailer */
    DAP_prepareTxTrailer(&UART_handle->txMsg,UART_handle->isCrc);


    UART_handle->TxStatus = UART_STATUS_TX_TRANSMITTING;

    /* Send Tx Buffer */
    UART_sendBuffer(&UART_handle->txMsg);

    /* Clear the flag to stop streaming data */
    gStartStream = 0;
}
