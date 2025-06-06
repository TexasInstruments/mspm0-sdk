/* --COPYRIGHT--,BSD
 * Copyright (c) 2020, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
//*****************************************************************************
//         Generic GUI Demo using MSP430
//
// Texas Instruments, Inc.
// ******************************************************************************
#include "GUIComm_mpack.h"
#include <stdbool.h>
#include <stdint.h>
#include <ti/gui_composer/HAL.h>
#include "GUI_mpack.h"

extern char data;
extern mpack_writer_t writer;
extern size_t size;

int GUIComm_sendBool(char *cmd, uint8_t cmdLength, bool val)
{
    mpack_start_map(&writer, 1);
    mpack_write_str(&writer, cmd, cmdLength);
    mpack_write_bool(&writer, val);
    mpack_writer_flush_message(&writer);

    if (writer.error != mpack_ok) {
        return -1;
    }

    return 0;
}

int GUIComm_sendUInt(char *cmd, uint8_t cmdLength, uint32_t val)
{
    mpack_start_map(&writer, 1);
    mpack_write_str(&writer, cmd, cmdLength);
    mpack_write_uint(&writer, val);
    mpack_writer_flush_message(&writer);

    if (writer.error != mpack_ok) {
        return -1;
    }

    return 0;
}

int GUIComm_sendInt(char *cmd, uint8_t cmdLength, uint32_t val)
{
    mpack_start_map(&writer, 1);
    mpack_write_str(&writer, cmd, cmdLength);
    mpack_write_int(&writer, val);
    mpack_writer_flush_message(&writer);

    if (writer.error != mpack_ok) {
        return -1;
    }

    return 0;
}

int GUIComm_sendUInt8(char *cmd, uint8_t cmdLength, uint8_t val)
{
    mpack_start_map(&writer, 1);
    mpack_write_str(&writer, cmd, cmdLength);
    mpack_write_u8(&writer, val);
    mpack_finish_map(&writer);
    mpack_writer_flush_message(&writer);

    if (writer.error != mpack_ok) {
        return -1;
    }

    return 0;
}

int GUIComm_sendInt8(char *cmd, uint8_t cmdLength, int8_t val)
{
    mpack_start_map(&writer, 1);
    mpack_write_str(&writer, cmd, cmdLength);
    mpack_write_i8(&writer, val);
    mpack_finish_map(&writer);
    mpack_writer_flush_message(&writer);

    if (writer.error != mpack_ok) {
        return -1;
    }

    return 0;
}

int GUIComm_sendUInt16(char *cmd, uint8_t cmdLength, uint16_t val)
{
    mpack_start_map(&writer, 1);
    mpack_write_str(&writer, cmd, cmdLength);
    mpack_write_u16(&writer, val);
    mpack_finish_map(&writer);
    mpack_writer_flush_message(&writer);

    if (writer.error != mpack_ok) {
        return -1;
    }

    return 0;
}

int GUIComm_sendInt16(char *cmd, uint8_t cmdLength, int16_t val)
{
    mpack_start_map(&writer, 1);
    mpack_write_str(&writer, cmd, cmdLength);
    mpack_write_i16(&writer, val);
    mpack_finish_map(&writer);
    mpack_writer_flush_message(&writer);

    if (writer.error != mpack_ok) {
        return -1;
    }

    return 0;
}

int GUIComm_sendUInt32(char *cmd, uint8_t cmdLength, uint32_t val)
{
    mpack_start_map(&writer, 1);
    mpack_write_str(&writer, cmd, cmdLength);
    mpack_write_u32(&writer, val);
    mpack_finish_map(&writer);
    mpack_writer_flush_message(&writer);

    if (writer.error != mpack_ok) {
        return -1;
    }

    return 0;
}

int GUIComm_sendInt32(char *cmd, uint8_t cmdLength, int32_t val)
{
    mpack_start_map(&writer, 1);
    mpack_write_str(&writer, cmd, cmdLength);
    mpack_write_i32(&writer, val);
    mpack_finish_map(&writer);
    mpack_writer_flush_message(&writer);

    if (writer.error != mpack_ok) {
        return -1;
    }

    return 0;
}

int GUIComm_sendUInt64(char *cmd, uint8_t cmdLength, uint64_t val)
{
    mpack_start_map(&writer, 1);
    mpack_write_str(&writer, cmd, cmdLength);
    mpack_write_u64(&writer, val);
    mpack_finish_map(&writer);
    mpack_writer_flush_message(&writer);

    if (writer.error != mpack_ok) {
        return -1;
    }

    return 0;
}

int GUIComm_sendInt64(char *cmd, uint8_t cmdLength, int64_t val)
{
    mpack_start_map(&writer, 1);
    mpack_write_str(&writer, cmd, cmdLength);
    mpack_write_i64(&writer, val);
    mpack_finish_map(&writer);
    mpack_writer_flush_message(&writer);

    if (writer.error != mpack_ok) {
        return -1;
    }

    return 0;
}

uint8_t GUIComm_ReadUInt8(mpack_tag_t *tag)
{
    return (uint8_t) mpack_tag_uint_value(tag);
}

uint16_t GUIComm_ReadUInt16(mpack_tag_t *tag)
{
    return (uint16_t) mpack_tag_uint_value(tag);
}

uint32_t GUIComm_ReadUInt32(mpack_tag_t *tag)
{
    return (uint32_t) mpack_tag_uint_value(tag);
}

uint64_t GUIComm_ReadUInt64(mpack_tag_t *tag)
{
    return (uint64_t) mpack_tag_uint_value(tag);
}

int8_t GUIComm_ReadInt8(mpack_tag_t *tag)
{
    return (int8_t) mpack_tag_int_value(tag);
}

int16_t GUIComm_ReadInt16(mpack_tag_t *tag)
{
    return (int16_t) mpack_tag_int_value(tag);
}

int32_t GUIComm_ReadInt32(mpack_tag_t *tag)
{
    return (int32_t) mpack_tag_int_value(tag);
}

int64_t GUIComm_ReadInt64(mpack_tag_t *tag)
{
    return (int64_t) mpack_tag_int_value(tag);
}

bool GUIComm_ReadBool(mpack_tag_t *tag)
{
    return mpack_tag_bool_value(tag);
}

float GUIComm_ReadFloat(mpack_tag_t *tag)
{
    return mpack_tag_float_value(tag);
}

double GUIComm_ReadDouble(mpack_tag_t *tag)
{
    return mpack_tag_double_value(tag);
}
