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

#include <stdbool.h>
#include "foc.h"
#include "drv8323rs.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Initial selected VDS comparator threshold */
#define GUI_DEFAULT_DRV_VDS_LVL         DRV8323RS_OCP_CONTROL_VDS_LVL_1P700_V

/** @brief Initial selected setting for OCP level */
#define GUI_DEFAULT_DRV_SEN_LVL         DRV8323RS_CSA_CONTROL_SEN_LVL_1P00_V

/** @brief Initial selected gain of CSA */
#define GUI_DEFAULT_DRV_CSA_GAIN        DRV8323RS_CSA_CONTROL_CSA_GAIN_10VPV

/** @brief Initial value for shunt resistor in ohms */
#define GUI_DEFAULT_DRV_RSHUNT          DRV8323RS_DEFAULT_R_SHUNT

/** @brief Initial value of stator resistance */
#define GUI_DEFAULT_FOC_RS             (MOTOR_PARA_RS)

/** @brief Initial value of stator inductance */
#define GUI_DEFAULT_FOC_LS             (MOTOR_PARA_LS)

/** @brief Initial value of motor poles */
#define GUI_DEFAULT_FOC_POLES          (MOTOR_PARA_POLES)

/** @brief Initial value of base rpm for the per unit reference */
#define GUI_DEFAULT_FOC_BASE_RPM       (MOTOR_PARA_BASE_RPM)

/** @brief Initial value of pwm frequency in Hz */
#define GUI_DEFAULT_FOC_PWMFREQ        (10000)

/** @brief Initial value of pwm cycles spend in adc sampling */
#define GUI_DEFAULT_FOC_PWMADCSAMPLE   (20)

/** @brief Initial value of control divider to derive foc frequency 
 * from pwm frequency, currently supports only 1 */
#define GUI_DEFAULT_FOC_CNTRLDIV       (1)

/** @brief Initial value of deadband in ns */
#define GUI_DEFAULT_FOC_DEADBAND       DRV8323RS_DEFAULT_PWM_DEADBAND_NS

/** @brief Initial value of rshunt */
#define GUI_DEFAULT_FOC_RSHUNT         DRV8323RS_DEFAULT_R_SHUNT

/** @brief Initial value of voltage divider ratio */
#define GUI_DEFAULT_FOC_VOLT_RATIO     DRV8323RS_DEFAULT_VOLT_R_DIVIDER

/** @brief Initial value of amplifier gain */
#define GUI_DEFAULT_FOC_AMP_GAIN       drvGain[GUI_DEFAULT_DRV_CSA_GAIN]

/** @brief Initial value of estimator kslide */
#define GUI_DEFAULT_FOC_KSLIDE         (0.3)

/** @brief Initial value of KP for PI controller for speed */
#define GUI_DEFAULT_FOC_PISPD_KP       (0.3)

/** @brief Initial value of KI for PI controller for speed */
#define GUI_DEFAULT_FOC_PISPD_KI       (0.01)

/** @brief Initial value of max output for PI controller for speed */
#define GUI_DEFAULT_FOC_PISPD_MAX      (0.95)

/** @brief Initial value of min output for PI controller for speed */
#define GUI_DEFAULT_FOC_PISPD_MIN      (-0.95)

/** @brief Initial value of divider to derive PI speed execution rate from 
 * foc frequency */
#define GUI_DEFAULT_FOC_PISPD_DIV      (10)

/** @brief Initial value of KP for PI controller for Iq */
#define GUI_DEFAULT_FOC_PIIQ_KP        (0.043)

/** @brief Initial value of KI for PI controller for Iq */
#define GUI_DEFAULT_FOC_PIIQ_KI        (0.45)

/** @brief Initial value of max output for PI controller for Iq */
#define GUI_DEFAULT_FOC_PIIQ_MAX       (0.95)

/** @brief Initial value of min output for PI controller for Iq */
#define GUI_DEFAULT_FOC_PIIQ_MIN       (-0.95)

/** @brief Initial value of KP for PI controller for Id */
#define GUI_DEFAULT_FOC_PIID_KP        (0.043)

/** @brief Initial value of KI for PI controller for Id */
#define GUI_DEFAULT_FOC_PIID_KI        (0.45)

/** @brief Initial value of max output for PI controller for Id */
#define GUI_DEFAULT_FOC_PIID_MAX       (0.95)

/** @brief Initial value of min output for PI controller for Id */
#define GUI_DEFAULT_FOC_PIID_MIN       (-0.95)

/*!
 * @brief Number of consecutive faults to trigger the fault state
 */
#define GUI_FAULT_COUNT_THRES                      (10)

/*! @brief Define device parameters */
typedef struct
{
   /*!  Motor phase resistance */
   volatile float rs;
   /*!  Motor phase inductance */
   volatile float ls;
   /*!  Motor poles */
   volatile float poles;
   /*!  Base rpm for motor */
   volatile float baseRpm;
   /*!  PWM frequency */
   volatile float pwmFreq;
   /*!  Deadband for PWM outputs */
   volatile float deadband;
   /*!  Gain set for drv and foc. One of @ref DRV8323RS_CSA_CONTROL_CSA_GAIN */
   volatile DRV8323RS_CSA_CONTROL_CSA_GAIN gain;
   /*!  Shunt resistor value */
   volatile float rshunt;
   /*!  Proportional gain for pi speed controller */
   volatile float piSpdKp;
   /*!  Integral gain for pi speed controller */
   volatile float piSpdKi;
   /*!  Proportional gain for pi id controller */
   volatile float piIdKp;
   /*!  Integral gain for pi id controller */
   volatile float piIdKi;
   /*!  Proportional gain for pi iq controller */
   volatile float piIqKp;
   /*!  Integral gain for pi iq controller */
   volatile float piIqKi;
   /*!  esmo sliding factor for foc */
   volatile float kslide;
}gui_var;

/*! @brief Define gui spi variables */
typedef struct
{  /*!  Write Bitfield flag */
   volatile bool writeBitfieldRegFlag;
   /*!  Stores register address to write Bitfield. @ref DRV8323RS_REG_ADDR */
   volatile DRV8323RS_REG_ADDR writeBitfieldRegAddr;
   /*!  Stores position of Bitfield to write */
   volatile uint16_t bitfieldRegPosData;
   /*!  Stores mask of Bitfield to write */
   volatile uint16_t bitfieldRegMaskData;
   /*!  Stores data of Bitfield to write */
   volatile uint16_t writeBitfieldRegData;
   /*!  Write register flag */
   volatile bool writeRegFlag;
   /*!  Stores address of register to write. @ref DRV8323RS_REG_ADDR */
   volatile DRV8323RS_REG_ADDR writeRegAddr;
   /*!  Stores data to write to register */
   volatile uint16_t writeRegData;
   /*!  Read register flag */
   volatile bool readRegFlag;
   /*!  Stores address of register to read. @ref DRV8323RS_REG_ADDR */
   volatile DRV8323RS_REG_ADDR readRegAddr;
   /*!  Stores data read from register */
   volatile uint16_t readRegData;
}gui_spi_var;

/*! @brief Define fault flags */
typedef struct
{
   /*!  fault clear flag */
   volatile bool clearFault;
   /*!  Drv fault flag */
   volatile bool drvFault;
   /*!  Undervoltage fault flag */
   volatile bool uvlo;
   /*!  Overvoltage fault flag */
   volatile bool ovlo;
   /*!  Overcurrent fault flag */
   volatile bool ocp;
}gui_fault;

/**
 * @brief     Setup the initial gui values
 * @param[in] foc_handle  A pointer to foc instance
 * @param[in] drv_handle  A pointer to drv instance
 */
void GUI_setupDefaultValues(FOC_Instance *foc_handle,
                                                DRV8323RS_Instance *drv_handle);

/**
 * @brief     Configures the foc and drv with initial values
 * @param[in] foc_handle  A pointer to foc instance
 * @param[in] drv_handle  A pointer to drv instance
 */
void GUI_init(FOC_Instance *foc_handle, DRV8323RS_Instance *drv_handle);

/**
 * @brief     Checks for change in GUI variables and executes the changes
 * @param[in] foc_handle  A pointer to foc instance
 * @param[in] drv_handle  A pointer to drv instance
 */
void GUI_loop(FOC_Instance *foc_handle, DRV8323RS_Instance *drv_handle);

/**
 * @brief     Convert and get the phase currents and bus voltage from drv
 * @param[in] drv_handle  A pointer to drv instance
 */
void GUI_getVars(DRV8323RS_Instance *drv_handle);

/**
 * @brief     Polls for any fault occurrence
 * @param[in] foc_handle  A pointer to foc instance
 * @param[in] drv_handle  A pointer to drv instance
 */
void GUI_checkFaults(FOC_Instance *foc_handle, DRV8323RS_Instance *drv_handle);

/**
 * @brief     Clears the fault occurred
 * @param[in] foc_handle  A pointer to foc instance
 * @param[in] drv_handle  A pointer to drv instance
 */
void GUI_clearFaults(FOC_Instance *foc_handle, DRV8323RS_Instance *drv_handle);

/**
 * @brief     Handles the SPI register request from GUI
 * @param[in] drv_handle  A pointer to drv instance
 */
void GUI_spiCommands(DRV8323RS_Instance *drv_handle);

#ifdef __cplusplus
}
#endif
#endif /* GUI_H */
