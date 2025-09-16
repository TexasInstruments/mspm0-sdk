/*
 * Copyright (c) 2023, Texas Instruments Incorporated
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

typedef enum {
    CMD_CONNECTION = 0x12,
    CMD_UNLOCK_BSL = 0x21,
    CMD_FLASH_RANGE_ERASE = 0x23,
    CMD_MASS_ERASE = 0x15,
    CMD_PROGRAM_DATA = 0x20,
    CMD_MEMORY_READ_BACK = 0x29,
    CMD_FACTORY_RESET = 0x30,
    CMD_GET_IDENTITY = 0x19,
    CMD_STANDALONE_VERIFICATION = 0x26,
    CMD_START_APPLICATION = 0x40,
    
} Command;
extern Command currentCommand;

unsigned char g_pcDFUPrefix_CMD_CONNECTION[] ={ 0x80, 0x01, 0x00, CMD_CONNECTION };

unsigned char g_pcDFUPrefix_CMD_UNLOCK_BSL[]=  
{ 
0x80, 
0x21, 
0x00, 
0x21, 
};

unsigned char g_pcDFUPrefix_CMD_GET_IDENTITY[] = 
{ 
0x80, 
0x01, 
0x00, 
CMD_GET_IDENTITY, 
} ;
unsigned char g_pcDFUPrefix_CMD_MASS_ERASE[] =
{ 
0x80, 
0x01, 
0x00, 
CMD_MASS_ERASE, 
}  ;

unsigned char g_pcDFUPrefix_CMD_FACTORY_RESET[]  =
{ 
0x80, 
0x11, 
0x00, 
CMD_FACTORY_RESET, 
 
} ;


unsigned char  g_pcDFUPrefix_CMD_STANDALONE_VERIFICATION[] =
{ 
0x80, 
0x01, 
0x00, 
CMD_STANDALONE_VERIFICATION, 
0x00, 
0x00, 
0x00, 
0x00, 
0x00, 
0x06, 
0x00, 
0x00, 
} ;

unsigned char  g_pcDFUPrefix_CMD_START_APPLICATION[] =
{ 
0x80, 
0x01, 
0x00, 
CMD_START_APPLICATION, 
} ;
unsigned char  g_pcDFUPrefix_CMD_FLASH_RANGE_ERASE[] =
{ 
0x80, 
0x09, 
0x00, 
CMD_FLASH_RANGE_ERASE, 
0x00, 
0x01, 
0x00, 
0x00, 
0x00, 
0x01, 
0x00, 
0x00, 
} ;

unsigned char g_pcDFUPrefix_CMD_PROGRAM_DATA[] =  { 
0x80, 0x00, 0x00, CMD_PROGRAM_DATA,
0x00, 0x00, 0x00, 0x00, //Address
0x00, 0x00, 0x00, 0x00, //Length
0x00, 0x00, 0x00, 0x00, //Reserved
}; 

unsigned char g_pcDFUPrefix_CMD_MEMORY_READ_BACK[]  = { 0x80, 0x09, 0x00, CMD_MEMORY_READ_BACK, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };