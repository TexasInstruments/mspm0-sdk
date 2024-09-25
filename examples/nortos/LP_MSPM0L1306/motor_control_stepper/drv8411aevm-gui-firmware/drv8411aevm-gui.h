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
#ifndef GUI_H
#define GUI_H

#include "drv8411a.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Initial selected step mode
 */
#define GUI_DEFAULT_STEP_MODE              DRV8411A_STEP_FULL_STEP

/*!
 * @brief Initial selected decay mode
 */
#define GUI_DEFAULT_DECAY_MODE             DRV8411A_DECAY_SLOW

/*!
 * @brief Initial selected direction
 */
#define GUI_DEFAULT_DIRECTION              DRV8411A_DIR_FORWARD

/*!
 * @brief Initial set speed in PPS(Pulse Per Second)
 */
#define GUI_DEFAULT_SPEED                  (100)

/*!
 * @brief Initial selected control mode
 */
#define GUI_DEFAULT_CONTROL_MODE           DRV8411A_CONTROL_MODE_SPIN

/*!
 * @brief Initial set sleep state
 */
#define GUI_DEFAULT_SLEEP                  GUI_DRV_STATE_ASLEEP

/*!
 * @brief Initial RIPROPI value in Ohms
 */
#define GUI_DEFAULT_RIPROPI                (1500)

/*!
 * @brief Initial Trip current in Ampere
 */
#define GUI_DEFAULT_ITRIP                  (0.33f)

/*!
 * @brief Initial DRV used ADC channels reference voltage
 */
#define GUI_DEFAULT_DRV_ADC_REF_VOLTAGE    (3.3f)

/*!
 * @brief Initial DRV DAC reference voltage
 */
#define GUI_DEFAULT_DRV_DAC_REF_VOLTAGE    (3.3f)

/*!
 * @brief Initial AIpropi value of the DRV
 */
#define GUI_DEFAULT_DRV_A_IPROPI           DRV8411A_A_IPROPI

/*!
 * @brief Number of consecutive faults to trigger the fault state
 */
#define GUI_FAULT_COUNT_THRES              (10)

/*! @enum GUI_DRV_STATE */
typedef enum {
   /*! Enter sleep mode */
   GUI_DRV_STATE_ASLEEP = 0,
   /*! Enter active mode */
   GUI_DRV_STATE_AWAKE,
}GUI_DRV_STATE;

/*! @enum GUI_MOTOR_STATE */
typedef enum {
   /*! Stop motor */
   GUI_MOTOR_STATE_STOP = 0,
   /*! Start motor */
   GUI_MOTOR_STATE_RUN,
}GUI_MOTOR_STATE;

/*! @brief Define device parameters */
typedef struct
{
   /*!  State of sleep pin. @ref GUI_DRV_STATE */
   volatile GUI_DRV_STATE sleep;
    /*!  Stores the direction of rotation. @ref DRV8411A_DIR */
   volatile DRV8411A_DIR dir;
   /*!  Selected Step mode. @ref DRV8411A_STEP */
   volatile DRV8411A_STEP stepMode;
   /*!  Selected decay mode. @ref DRV8411A_DECAY */
   volatile DRV8411A_DECAY decayMode;
   /*!  Steps to move in step mode */
   volatile uint16_t steps;
   /*!  Selected control mode. @ref DRV8411A_CONTROL_MODE */
   volatile DRV8411A_CONTROL_MODE controlMode;
   /*!  State of motor. @ref GUI_MOTOR_STATE */
   volatile GUI_MOTOR_STATE motorState;
   /*!  Stores the speed in PPS */
   volatile uint32_t speed;
    /*!  Stores the RIPROPI value */
   volatile uint32_t ripropiVal;
   /*!  Stores the trip current */
   volatile float itrip;
   /*!  Reference voltage used by ADC */
   volatile float drvAdcVRef;
   /*!  Reference voltage used by DAC */
   volatile float drvDacVRef;
} gui_var;

/*! @brief Define fault flags */
typedef struct
{  /*!  Drv fault flag */
   volatile bool drvFault;
   /*!  fault clear flag */
   volatile bool clearFault;
}gui_fault;

/**
 * @brief Setup the initial gui values
 */
void gui_setupDefaultValues(void);

/**
 * @brief     Configures the DRV8411A with the initial values
 * @param[in] drv_handle  A pointer to drv instance
 */
void gui_init(DRV8411A_Instance *drv_handle);

/**
 * @brief     Checks for change in GUI variables and executes the changes
 * @param[in] drv_handle  A pointer to drv instance
 */
void gui_loop(DRV8411A_Instance *drv_handle);

/**
 * @brief     Polls for any fault occurrence
 * @param[in] drv_handle  A pointer to drv instance
 */
void gui_checkFaults(DRV8411A_Instance *drv_handle);

/**
 * @brief     Convert and get the phase currents from drv
 * @param[in] drv_handle  A pointer to drv instance
 */
void gui_convertPhaseCurrent(DRV8411A_Instance *drv_handle);

/**
 * @brief     Sets the update plot variable flag
 */
void gui_setUpdatePlotVarFlag(void);

#ifdef __cplusplus
}
#endif
#endif /* GUI_H */
