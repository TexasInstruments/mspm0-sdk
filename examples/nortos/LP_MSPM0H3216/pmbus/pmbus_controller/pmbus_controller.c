/* --COPYRIGHT--,BSD
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
 * --/COPYRIGHT--*/

/*
 * ======== main.c ========
 * PMBus_controller
 *
 * This example communicates with the target device using PMBus showing
 *  basic calls of PMBusLib functions.
 * ---------------------------------------------------------------------------*/

#include <stdint.h>
#include <ti/pmbus/pmbus.h>

#include "ti_msp_dl_config.h"

#define TARGET_ADDRESS 0x1A      /*! Slave address of target device */

/* Number of bytes */
#define DATA_SIZE_0 0
#define DATA_SIZE_1 1
#define DATA_SIZE_2 2
#define DATA_SIZE_3 3
#define DATA_SIZE_4 4
#define DATA_SIZE_5 5
#define DATA_SIZE_6 6
#define DATA_SIZE_7 7
#define DATA_SIZE_8 8
#define DATA_SIZE_9 9

/* Extended commands */
#define EXTENDED_BYTE_CMD 0x01
#define EXTENDED_WORD_CMD 0x02

/*! SMBus Controller Struct  */
static SMBus SMB;
/*! Buffer for command to slave */
static uint8_t Command_Buff[PMB_MAX_PACKET_SIZE + 4];
/*! Buffer for response from slave device */
static uint8_t Resp_Buff[PMB_MAX_PACKET_SIZE + 4];
/*! Buffer size for response */
static uint8_t Resp_Size;

/*! Step-by-step Tests performed by this demo */
enum Tests
{
    QUICK_CMD_TEST = 0,       /*! Test Quick command                      */
    DEVICE_ID_TEST,           /*! Test device id command                  */
    SEND_BYTE_TEST,           /*! Test Send Byte command                  */
    RECEIVE_BYTE_TEST,        /*! Test Receive Byte command               */
    WRITE_BYTE_TEST,          /*! Test Write Byte Command                 */
    READ_BYTE_TEST,           /*! Test Read Byte command                  */
    WRITE_WORD_TEST,          /*! Test Write 2 Byte - Word command        */
    READ_WORD_TEST,           /*! Test Read 2 Byte - Word command         */
    BLOCK_WRITE_TEST,         /*! Test Block Write command                */
    BLOCK_READ_TEST,          /*! Test Block Read command                 */
    READ_ADC_TEST,            /*! Test Read Byte command                  */
    EXTENDED_WRITE_BYTE_TEST, /*! Test Extended Write Byte command        */
    EXTENDED_READ_BYTE_TEST,  /*! Test Extended Read Byte command         */
    EXTENDED_WRITE_WORD_TEST, /*! Test Extended Write Word command        */
    EXTENDED_READ_WORD_TEST,  /*! Test Extended Read Word command         */
    PROCESS_CALL_TEST,        /*! Test Process Call command               */
    BLOCK_PROC_CALL_TEST,     /*! Test BlockWBlockR Process Call command  */
    END_OF_TESTS              /*! delimiter                               */
} Tests_e;

//*****************************************************************************
//
//! Main Function
//!
//!  This example communicates with the target device using PMBus showing
//!  basic calls of PMBusLib functions. The LED toggles if all communications
//!  work correctly. The LED is set on if there is a failure.
//! \return none
//
// *****************************************************************************

/*! Macro used to check the output of each command.
 *  For demo purposes, the device will stay in a loop with both LEDs ON if an
 *  error was detected (i.e. timeout, NACK, etc)
 */
void CHECK_RET(int8_t ret)
{
    int8_t valid = PMBUS_RET_OK;
    if(ret != PMBUS_RET_OK)
    {
        valid = PMBUS_RET_ERROR;
    }

    switch(Tests_e)
    {
        case DEVICE_ID_TEST:
            /*
             * Verify the ID of the device (MSPM0)
             * Device id command will function as block read
             */
            if(!(Resp_Buff[0]=='M'&&Resp_Buff[1]=='S'&&Resp_Buff[2]=='P'&&
                Resp_Buff[3]=='M'&&Resp_Buff[4]=='0'))
            {
                valid = PMBUS_RET_ERROR;
            }
            if(Resp_Size != DATA_SIZE_5)
            {
                valid = PMBUS_RET_ERROR;
            }
            break;

        case RECEIVE_BYTE_TEST:
            if(Resp_Buff[0] != 0x00)
            {
                valid = PMBUS_RET_ERROR;
            }
            break;

        case READ_BYTE_TEST:
            /* Read VIN_OUT acts as a read byte command */
            if(Resp_Buff[0] != 0x01)
            {
                valid = PMBUS_RET_ERROR;
            }
            if(Resp_Size != DATA_SIZE_1)
            {
                valid = PMBUS_RET_ERROR;
            }
            break;

        case READ_WORD_TEST:
            /* Read VOUT_OV_WARN_LIMIT acts as read word command */
            if(!(Resp_Buff[0]==0xCD&&Resp_Buff[1]==0xAB))
            {
                valid = PMBUS_RET_ERROR;
            }
            if(Resp_Size != DATA_SIZE_2)
            {
                valid = PMBUS_RET_ERROR;
            }
            break;

        case BLOCK_READ_TEST:
            /* Read PMB_MFR_ID acts as a block read command */
            if(!(Resp_Buff[0]=='M'&&Resp_Buff[1]=='F'&&Resp_Buff[2]=='R'&&Resp_Buff[3]=='_'&&Resp_Buff[4]=='I'&&
                Resp_Buff[5]=='D'))
            {
                valid = PMBUS_RET_ERROR;
            }
            if(Resp_Size != DATA_SIZE_6)
            {
                valid = PMBUS_RET_ERROR;
            }
            break;

        case EXTENDED_READ_BYTE_TEST:
            /* Extended read byte command */
            if(Resp_Buff[0] != 0x02)
            {
                valid = PMBUS_RET_ERROR;
            }
            break;

        case EXTENDED_READ_WORD_TEST:
            /* Extended read word command */
            if(!(Resp_Buff[0]==0x34&&Resp_Buff[1]==0x12))
            {
                valid = PMBUS_RET_ERROR;
            }
            break;

        case PROCESS_CALL_TEST:
            /* Process Call word command */
            if(!(Resp_Buff[0]==0x02&&Resp_Buff[1]==0x00))
            {
                valid = PMBUS_RET_ERROR;
            }
            if(Resp_Size != DATA_SIZE_2)
            {
                valid = PMBUS_RET_ERROR;
            }
            break;

        case BLOCK_PROC_CALL_TEST:
            /* Extended read word command */
            if(!(Resp_Buff[0]==0x06&&Resp_Buff[1]==0x00))
            {
                valid = PMBUS_RET_ERROR;
            }
            if(Resp_Size != DATA_SIZE_2)
            {
                valid = PMBUS_RET_ERROR;
            }
            break;

        default:
            break;
    }

    if(valid == PMBUS_RET_ERROR)
    {
        DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_D0_PIN);
        while(1);
    }
}

int main(void)
{
    uint32_t timeout = 800000; /* 10ms */
    int8_t ret;

    SYSCFG_DL_init();

    NVIC_ClearPendingIRQ(SMB_I2C_INST_INT_IRQN);
    NVIC_EnableIRQ(SMB_I2C_INST_INT_IRQN);

    /* Initialize PMBus interface */
    PMBus_controllerInit(&SMB,
                         SMB_I2C_INST,
                         (uint32_t)CPUCLK_FREQ);

    /*
     * If desired, enable PEC. The target device will accept commands with or
     * without PEC, but it will only check PEC if it's sent/requested
     */
    PMBus_enablePEC(&SMB);

    Tests_e = QUICK_CMD_TEST;    /* Start with Quick Command test */

    while(1)
    {
        switch(Tests_e)
        {
            case QUICK_CMD_TEST:
                /* Quick command */
                ret = PMBus_cmdWrite(&SMB,
                                     TARGET_ADDRESS,
                                     PMB_QUICK_COMMAND,
                                     Command_Buff,
                                     DATA_SIZE_0,
                                     false,
                                     timeout);
                break;

            case DEVICE_ID_TEST:
                /*
                 * Verify the ID of the device (MSPM0)
                 * Device id command will function as block read
                 */
                PMBus_cmdRead(&SMB,
                              TARGET_ADDRESS,
                              PMB_IC_DEVICE_ID,
                              Resp_Buff,
                              &Resp_Size,
                              timeout);
                break;

            case SEND_BYTE_TEST:
                /* Clear faults command functions as send bytes command */
                ret = PMBus_cmdWrite(&SMB,
                                    TARGET_ADDRESS,
                                    PMB_CLEAR_FAULTS,
                                    Command_Buff,
                                    DATA_SIZE_0,
                                    false,
                                    timeout);
                break;

            case RECEIVE_BYTE_TEST:
                ret = PMBus_cmdReceiveByte(&SMB,
                                           TARGET_ADDRESS,
                                           Resp_Buff,
                                           timeout);
                break;

            case WRITE_BYTE_TEST:
                /* Write VIN_OUT acts as write byte command */
                Command_Buff[0] = 0x01; /* HSB */
                ret = PMBus_cmdWrite(&SMB,
                                     TARGET_ADDRESS,
                                     PMB_VOUT_MODE,
                                     Command_Buff,
                                     DATA_SIZE_1,
                                     false,
                                     timeout);
                break;

            case READ_BYTE_TEST:
                /* Read VIN_OUT acts as a read byte command */
                ret = PMBus_cmdRead(&SMB,
                                    TARGET_ADDRESS,
                                    PMB_VOUT_MODE,
                                    Resp_Buff,
                                    &Resp_Size,
                                    timeout);
                break;

            case WRITE_WORD_TEST:
                /* Write VOUT_OV_WARN_LIMIT acts as write word command */
                Command_Buff[0] = 0xCD;
                Command_Buff[1] = 0xAB;
                ret = PMBus_cmdWrite(&SMB,
                                    TARGET_ADDRESS,
                                    PMB_VOUT_OV_WARN_LIMIT,
                                    Command_Buff,
                                    DATA_SIZE_2,
                                    false,
                                    timeout);
                break;

            case READ_WORD_TEST:
                /* Read VOUT_OV_WARN_LIMIT acts as read word command */
                ret = PMBus_cmdRead(&SMB,
                                    TARGET_ADDRESS,
                                    PMB_VOUT_OV_WARN_LIMIT,
                                    Resp_Buff,
                                    &Resp_Size,
                                    timeout);
                break;

            case BLOCK_WRITE_TEST:
                /* Write PMB_MFR_ID acts as a block write command */
                Command_Buff[0]='M';Command_Buff[1]='F';Command_Buff[2]='R';Command_Buff[3]='_';
                Command_Buff[4]='I';Command_Buff[5]='D';
                ret = PMBus_cmdWrite(&SMB,
                                     TARGET_ADDRESS,
                                     PMB_MFR_ID,
                                     Command_Buff,
                                     DATA_SIZE_6,
                                     false,
                                     timeout);
                break;

            case BLOCK_READ_TEST:
                /* Read PMB_MFR_ID acts as a block read command */
                ret = PMBus_cmdRead(&SMB,
                                    TARGET_ADDRESS,
                                    PMB_MFR_ID,
                                    Resp_Buff,
                                    &Resp_Size,
                                    timeout);
                break;

            case READ_ADC_TEST:
                /* Read PMB_READ_VOUT acts as read byte command containing ADC data */
                ret = PMBus_cmdRead(&SMB,
                                    TARGET_ADDRESS,
                                    PMB_READ_VOUT,
                                    Resp_Buff,
                                    &Resp_Size,
                                    timeout);
                break;

            case EXTENDED_WRITE_BYTE_TEST:
                /* Extended write byte command */
                Command_Buff[0] = 0x02;
                ret = PMBus_cmdExtendedWriteByte(&SMB,
                                                TARGET_ADDRESS,
                                                PMBUS_COMMAND_EXT,
                                                EXTENDED_BYTE_CMD,
                                                Command_Buff,
                                                DATA_SIZE_1,
                                                timeout);
                break;

            case EXTENDED_READ_BYTE_TEST:
                /* Extended read byte command */
                ret = PMBus_cmdExtendedReadByte(&SMB,
                                                TARGET_ADDRESS,
                                                PMBUS_COMMAND_EXT,
                                                EXTENDED_BYTE_CMD,
                                                Resp_Buff,
                                                DATA_SIZE_1,
                                                timeout);
                break;

            case EXTENDED_WRITE_WORD_TEST:
                /* Extended write word command */
                Command_Buff[0] = 0x34;
                Command_Buff[1] = 0x12;
                ret = PMBus_cmdExtendedWriteWord(&SMB,
                                                TARGET_ADDRESS,
                                                PMBUS_COMMAND_EXT,
                                                EXTENDED_WORD_CMD,
                                                Command_Buff,
                                                DATA_SIZE_2,
                                                timeout);
                break;

            case EXTENDED_READ_WORD_TEST:
                /* Extended read word command */
                ret = PMBus_cmdExtendedReadWord(&SMB,
                                                TARGET_ADDRESS,
                                                PMBUS_COMMAND_EXT,
                                                EXTENDED_WORD_CMD,
                                                Resp_Buff,
                                                DATA_SIZE_2,
                                                timeout);
                break;

            case PROCESS_CALL_TEST:
                /* Process Call word command */
                Command_Buff[0] = 0x02;
                Command_Buff[1] = 0x01;
                ret = PMBus_cmdProcessCall(&SMB,
                                           TARGET_ADDRESS,
                                           PMB_PROCESS_CALL_WORD,
                                           Command_Buff,
                                           Resp_Buff,
                                           timeout);
                break;

            case BLOCK_PROC_CALL_TEST:
                /* Extended read word command */
                Command_Buff[0] = 0x03;
                Command_Buff[1] = 0x02;
                Command_Buff[2] = 0x01;
                ret = PMBus_cmdBlockWriteBlockReadProcessCall(&SMB,
                                                              TARGET_ADDRESS,
                                                              PMB_PROCESS_CALL_BLOCK,
                                                              Command_Buff,
                                                              DATA_SIZE_3,
                                                              Resp_Buff,
                                                              &Resp_Size,
                                                              timeout);
                break;

            default:
                Tests_e = QUICK_CMD_TEST;
                break;
        }

        CHECK_RET(ret);
        SMB.status.u8byte = 0x00;        /* Clear status  */
        Tests_e++;                       /* Run next test */
        if(Tests_e >= END_OF_TESTS)
        {
                while(1)
                {
                    /* toggle LED0 if everything executed OK */
                    DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_D0_PIN);
                    delay_cycles(16000000); /* 200ms */
                }
        }
        delay_cycles(80000); /* 1ms delay between commands  */
    }
}

//
// Events
//
//*****************************************************************************
//
//! I2C Event
//!
//! Event called when I2C TX, RX, Start, Stop and errors are detected
//!
//
// *****************************************************************************
void SMB_I2C_INST_IRQHandler(void) {
    PMBus_controllerProcessInt(&SMB);
}
