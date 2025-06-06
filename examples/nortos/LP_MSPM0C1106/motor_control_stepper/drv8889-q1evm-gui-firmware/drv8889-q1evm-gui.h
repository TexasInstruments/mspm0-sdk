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

#include "drv8889-q1.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Initial selected step mode
 */
#define GUI_DEFAULT_STEP_MODE                DRV8889Q1_STEP_FULL_STEP

/*!
 * @brief Initial selected decay mode
 */
#define GUI_DEFAULT_DECAY_MODE               DRV8889Q1_DECAY_INC_SLOW_DEC_SLOW

/*!
 * @brief Initial selected decay mode
 */
#define GUI_DEFAULT_SLEW_RATE                DRV8889Q1_SLEW_RATE_10VUS

/*!
 * @brief Initial selected direction
 */
#define GUI_DEFAULT_DIRECTION                DRV8889Q1_DIR_FORWARD

/*!
 * @brief Initial set speed in PPS(Pulse Per Second)
 */
#define GUI_DEFAULT_SPEED                    (100)

/*!
 * @brief Initial selected control mode
 */
#define GUI_DEFAULT_CONTROL_MODE             DRV8889Q1_CONTROL_MODE_SPIN

/*!
 * @brief Initial torque percentage
 */
#define GUI_DEFAULT_TRQ_DAC                  DRV8889Q1_TRQ_DAC_100

/*!
 * @brief Initial toff
 */
#define GUI_DEFAULT_TOFF                     DRV8889Q1_TOFF_7US

/*!
 * @brief Initial set sleep state
 */
#define GUI_DEFAULT_SLEEP                    GUI_DRV_STATE_ASLEEP

/*!
 * @brief Initial drv ouput state
 */
#define GUI_DEFAULT_DRV_OUTPUT               GUI_DRV_OUTPUT_DISABLED

/*!
 * @brief Initial stall detection state
 */
#define GUI_DEFAULT_STALL_DET_STATE          GUI_STALL_DISABLED

/*!
 * @brief Initial open load detection state
 */
#define GUI_DEFAULT_OL_STATE                 GUI_OL_DISABLED

/*!
 * @brief Initial reverse on stall state
 */
#define GUI_DEFAULT_REVERSE_ON_STALL         GUI_REVERSE_ON_STALL_DISABLED

/*!
 * @brief Initial full scale current in Ampere
 */
#define GUI_DEFAULT_FS_CURR                        (0.33f)

/*!
 * @brief Initial DRV VREF reference voltage
 */
#define GUI_DEFAULT_DRV_VREF_REF_VOLTAGE           (3.3f)

/*!
 * @brief Default stall threshold
 */
#define GUI_DEFAULT_STALL_THRES                    (0)

/*!
 * @brief Number of consecutive faults to trigger the fault state
 */
#define GUI_FAULT_COUNT_THRES                      (10)

/*!
 * @brief Time in millisecond to wait before reverse on stall
 */
#define GUI_WAIT_BEFORE_REVERSE_ON_STALL           (500)

/*! @enum GUI_DRV_STATE */
typedef enum {
   /*! Enter sleep mode */
   GUI_DRV_STATE_ASLEEP = 0,
   /*! Enter active mode */
   GUI_DRV_STATE_AWAKE,
}GUI_DRV_STATE;

/*! @enum GUI_DRV_OUTPUT */
typedef enum {
   /*! DRV output disabled */
   GUI_DRV_OUTPUT_DISABLED = 0,
   /*! DRV output enabled */
   GUI_DRV_OUTPUT_ENABLED,
}GUI_DRV_OUTPUT;

/*! @enum GUI_MOTOR_STATE */
typedef enum {
   /*! Stop motor */
   GUI_MOTOR_STATE_STOP = 0,
   /*! Start motor */
   GUI_MOTOR_STATE_RUN,
}GUI_MOTOR_STATE;

/*! @enum GUI_STALL */
typedef enum {
   /*! Stall detection disabled */
   GUI_STALL_DISABLED = 0,
   /*! Stall detection enabled */
   GUI_STALL_ENABLED,
}GUI_STALL;

/*! @enum GUI_OL */
typedef enum {
   /*! Open load detection disabled */
   GUI_OL_DISABLED = 0,
   /*! Open load detection enabled */
   GUI_OL_ENABLED,
}GUI_OL;

/*! @enum GUI_REVERSE_ON_STALL */
typedef enum {
   /*! Open load detection disabled */
   GUI_REVERSE_ON_STALL_DISABLED = 0,
   /*! Open load detection enabled */
   GUI_REVERSE_ON_STALL_ENABLED,
}GUI_REVERSE_ON_STALL;

/*! @brief Define device parameters */
typedef struct
{
   /*!  State of drvOff pin. @ref GUI_DRV_OUTPUT */
   volatile GUI_DRV_OUTPUT drvOutput;
   /*!  State of sleep pin. @ref GUI_DRV_STATE */
   volatile GUI_DRV_STATE sleep;
    /*!  Stores the direction of rotation. @ref DRV8889Q1_DIR */
   volatile DRV8889Q1_DIR dir;
   /*!  Selected Step mode. @ref DRV8889Q1_STEP */
   volatile DRV8889Q1_STEP stepMode;
   /*!  Selected decay mode. @ref DRV8889Q1_DECAY */
   volatile DRV8889Q1_DECAY decayMode;
   /*!  Selected slew rate. @ref DRV8889Q1_SLEW_RATE */
   volatile DRV8889Q1_SLEW_RATE slewRate;
   /*!  Steps to move in step mode */
   volatile uint16_t steps;
   /*!  Stores the speed in PPS */
   volatile uint32_t speed;
   /*!  Selected control mode. @ref DRV8889Q1_CONTROL_MODE */
   volatile DRV8889Q1_CONTROL_MODE controlMode;
   /*!  State of motor. @ref GUI_MOTOR_STATE */
   volatile GUI_MOTOR_STATE motorState;
   /*!  Selected torque percentage. @ref DRV8889Q1_TRQ_DAC */
   volatile DRV8889Q1_TRQ_DAC trqDac;
   /*!  Selected toff. @ref DRV8889Q1_TOFF */
   volatile DRV8889Q1_TOFF toff;
   /*!  Stores the full scale current */
   volatile float fsCurr;
   /*!  Stores the stall Threshold value in manual stall detection mode */
   volatile uint8_t stallThres;
   /*!  Stall detection state. @ref GUI_STALL */
   volatile GUI_STALL stallDetState;
   /*!  Open load detection state. @ref GUI_OL */
   volatile GUI_OL olState;
   /*!  Reverse on stall state. @ref GUI_REVERSE_ON_STALL */
   volatile GUI_REVERSE_ON_STALL reverseOnStall;
   /*!  Reference voltage used by DAC */
   volatile float drvDacVRef;
}gui_var;

/*! @brief Define fault flags */
typedef struct
{  /*!  Drv fault flag */
   volatile bool drvFault;
   /*!  fault clear flag */
   volatile bool clearFault;
   /*!  Undervoltage lockout fault flag */
   volatile bool uvlo;
   /*!  Charge pump undervotlage fault flag */
   volatile bool cpuv;
   /*!  Overcurrent fault flag */
   volatile bool ocp;
   /*!  Motor stall fault flag */
   volatile bool stl;
   /*!  Overtemp warning or undertemp warning
          or over temp shutdown fault flag */
   volatile bool tf;
   /*!  Openload condition fault flag */
   volatile bool ol;
}gui_fault;

/*! @brief Define gui spi variables */
typedef struct
{  /*!  Write Bitfield flag */
   volatile bool writeBitfieldRegFlag;
   /*!  Stores register address to write Bitfield. @ref DRV8889Q1_REG_ADDR */
   volatile DRV8889Q1_REG_ADDR writeBitfieldRegAddr;
   /*!  Stores position of Bitfield to write */
   volatile uint8_t bitfieldRegPosData;
   /*!  Stores mask of Bitfield to write */
   volatile uint8_t bitfieldRegMaskData;
   /*!  Stores data of Bitfield to write */
   volatile uint8_t writeBitfieldRegData;
   /*!  Write register flag */
   volatile bool writeRegFlag;
   /*!  Stores address of register to write. @ref DRV8889Q1_REG_ADDR */
   volatile DRV8889Q1_REG_ADDR writeRegAddr;
   /*!  Stores data to write to register */
   volatile uint8_t writeRegData;
   /*!  Read register flag */
   volatile bool readRegFlag;
   /*!  Stores address of register to read. @ref DRV8889Q1_REG_ADDR */
   volatile DRV8889Q1_REG_ADDR readRegAddr;
   /*!  Stores data read from register */
   volatile uint8_t readRegData;
}gui_spi_var;

/**
 * @brief Setup the initial gui values
 */
void gui_setupDefaultValues(void);

/**
 * @brief     Configurures the DRV8889Q1 with the initial values
 * @param[in] drv_handle  A pointer to drv instance
 */
void gui_init(DRV8889Q1_Instance *drv_handle);

/**
 * @brief     Checks for change in GUI variabes and executes the changes
 * @param[in] drv_handle  A pointer to drv instance
 */
void gui_loop(DRV8889Q1_Instance *drv_handle);

/**
 * @brief     Polls for any fault Occurance
 * @param[in] drv_handle  A pointer to drv instance
 */
void gui_checkFaults(DRV8889Q1_Instance *drv_handle);

/**
 * @brief     Clears the fault variables and the status in drv
 * @param[in] drv_handle  A pointer to drv instance
 */
void gui_clearFaults(DRV8889Q1_Instance *drv_handle);

/**
 * @brief     Reads the DRV fault registers and sets the gui faults
 * @param[in] drv_handle  A pointer to drv instance
 */
void gui_monitorFaultStatus(DRV8889Q1_Instance *drv_handle);

/**
 * @brief     Checks if the reverse on stall condition occurs
 * @param[in] drv_handle  A pointer to drv instance
 */
void gui_CheckStallReverse(DRV8889Q1_Instance *drv_handle);

/**
 * @brief     Handles the SPI register request from GUI
 * @param[in] drv_handle  A pointer to drv instance
 */
void gui_spiCommands(DRV8889Q1_Instance *drv_handle);

#ifdef __cplusplus
}
#endif
#endif /* GUI_H */
