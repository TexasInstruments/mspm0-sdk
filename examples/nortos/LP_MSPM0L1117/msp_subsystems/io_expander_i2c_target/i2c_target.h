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
#ifndef I2C_H_
#define I2C_H_

#include "ti_msp_dl_config.h"

/*=============================== EXTERNS =======================================*/

/*===================== TYPEDEFS / ENUMERATIONS =================================*/
/* Indicates status of I2C */
enum I2cControllerStatus {
    I2C_STATUS_IDLE = 0,
    I2C_STATUS_TX_STARTED,
    I2C_STATUS_TX_INPROGRESS,
    I2C_STATUS_TX_COMPLETE,
    I2C_STATUS_RX_STARTED,
    I2C_STATUS_RX_INPROGRESS,
    I2C_STATUS_RX_COMPLETE,
    I2C_STATUS_ERROR,
} gI2cTargetStatus;

/*
 * User-defined data type. Represents 32-bit value as
 * int, short and char. Makes is easier to access portions
 * of 32-bit word by short or byte operations.
 */
typedef union{

    unsigned int ui32Full;

    struct
    {
      short int upper;
      short int lower;

    };

    struct
    {
      unsigned char B0;
      unsigned char B1;
      unsigned char B2;
      unsigned char B3;
    };

}tData;

/*
 * User-defined data type
 * This is the I2C protocol data structure
 */
typedef struct
{
    uint8_t PortNum;
    uint8_t Register;
    tData Data;

}tPacket;

typedef enum{
    eDIN,
    eDOUT,
    eDOUT_CLEAR,
    eDOUT_SET,
    eDOUT_TOGGLE,
}eRegisters;

/*
 * Enumeration of possible ports
 *
 */
typedef enum{
    ePortA,
    ePortB,
    ePortC,
}ePorts;

/*====================== DEFINES/MACROS ====================================*/

/*
 *  Note: Target device I2C Address is defined in Sysconfig
 */

/* Number of bytes to send to controller device */
#define I2C_TARGET_TX_MAX_PACKET_SIZE               (6)
#define I2C_TARGET_TX_PACKET_SIZE                   (6)

/* Number of bytes to received from controller device */
#define I2C_TARGET_RX_MAX_PACKET_SIZE               (6)
#define I2C_TARGET_RX_PACKET_SIZE                   (6)

/*============================ EXTERNS ===================================*/
extern bool g_bMessageReady;
extern void processMessage(void);
extern void deSerializePacket(tPacket* pPacket, uint8_t* pBuffer);
extern void serializePacket(tPacket* pPacket, uint8_t* pBuffer);

#endif /* I2C_H_ */
