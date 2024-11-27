/*
 * Copyright (c) 2024, Texas Instruments Incorporated
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
/*!****************************************************************************
 *  @file       appInterface.h
 *  @brief      Application Interface
 *
 * 
 *  @anchor appInterface_h
 *  # Overview
 *
 *  defines application Interface structures
 *
 *  <hr>
 ******************************************************************************/

#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include "appInputCtrlInterface.h"
#include "stdint.h"
#include "appUserInputs.h"
#include "appUserInputsInterface.h"
#include "appUserOutputs.h"

/*! @brief Interval for service of low priority tasks */
#define SERVICE_INTERVAL                    200

/*! @brief Interval for service of HV die */
#define HVDIE_SERVICE_COUNT                 100

/*! @brief Interval for service of faults */
#define FAULT_SERVICE_COUNT                 190

/*! @brief Structure for app interface flags */
typedef struct
{
    _Bool
    /*! flag to trigger shadow variables to load to algorithm variables */
    shadowToAlgorithmSet: 1,
    /*! flag to set hv die configs */
    hvDieConfigSet: 1,
    /*! flag to set fault response */
    faultsResponseSet: 1;
}appInterfaceFlags;

/*! @brief flags for app interface */
typedef union
{
    /*! Bitwise access */
    appInterfaceFlags b;
    /*! Access in block */
    uint16_t w;
}APPINTERFACE_FLAGS_T;

/*! @brief App interface Structure */
typedef struct
{
    /*! user inputs */
    USER_INPUTS_T
        userInputs;
    /*! user input interface */
    USER_INPUTS_INTERFACE_T
        userInputsInterface;
    /*! user output */
    USER_STATUS_INTERFACE_T *
        pUserOutputs;                /* User Outputs */
    /*! App interface flags */
    volatile APPINTERFACE_FLAGS_T
        flags;
    /*! variable is used to count Number of ms the befor service of low priority task */
     volatile uint16_t servicesCount;
}APP_INTERFACE_T;

/** @brief App interface pointer */
extern APP_INTERFACE_T *g_pAppInterface;
#endif /* APPINTERFACE_H */
