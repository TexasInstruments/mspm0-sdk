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

#ifndef DRV8329REVM_GUI_H_
#define DRV8329REVM_GUI_H_

//*****************************************************************************
// the includes
//*****************************************************************************
#include "hall_trap.h"
#include "hal.h"
#include "drv8329.h"

//*****************************************************************************
//defines
//*****************************************************************************
#define TRUE    (1)
#define FALSE   (0)
/* @brief Defines motor state run    */
#define GUI_MOTOR_RUN                         MOTOR_STATE_RUN

/* @brief Defines motor state stop   */
#define GUI_MOTOR_STOP                        MOTOR_STATE_STOP

/* @brief Defines Fault status low   */
#define GUI_FAULT_STATUS_LOW                  (0)

/* @brief Defines Fault status high  */
#define GUI_FAULT_STATUS_HIGH                 (1)

/* @brief Defines Fault reset low    */
#define GUI_FAULT_RESET_LOW                   (0)

/* @brief Defines Fault reset high   */
#define GUI_FAULT_RESET_HIGH                  (1)

/* @brief Defines initial motor direction (forward or reverse) */
#define GUI_INITIAL_MOTOR_DIRECTION           MOTOR_DIR_FORWARD

/* @brief Defines initial DRV mode (sleep or awake)  */
#define GUI_INITIAL_NSLEEP                    DRV8329_DRV_NSLEEP_AWAKE

/* @brief Defines initial DRV (ON or OFF)            */
#define GUI_INITIAL_DRVOFF                    DRV8329_DRVOFF_PIN_LOW

/* @brief Defines initial motor state (RUN or STOP)  */
#define GUI_INITIAL_STOPMOTOR                 GUI_MOTOR_RUN

/* @brief Defines initial CSAGAIN                    */
#define GUI_INITIAL_CSAGAIN                   DRV8329_CSAGAIN_40

/* @brief Defines initial fault reset bit status     */
#define GUI_INITIAL_FAULT_RESET               GUI_FAULT_RESET_LOW

/* @brief Defines initial motor brake type           */
#define GUI_INITIAL_BRAKETYPE                 MOTOR_BRAKE_TYPE_COAST

/* @brief Defines initial CSA reference */
#define GUI_INITIAL_CSA_REF                   DRV8329_CSA_REF_VOLTAGE

/* @brief Defines initial adc ref in milli volts     */
#define GUI_INITIAL_ADC_REF                   (3300)

/* @brief Defines initial PWM Frequency      */
#define GUI_INITIAL_PWM_FREQ                  (20000)

/* @brief Defines initial dead time in ns    */
#define GUI_INITIAL_DEAD_TIME_NS              (320)

/* @brief Defines initial acceleration rate
 *        increment/decrement of pulse width by 1 percent
 *        for every 20 pwm cycles(1ms for 20kHz freq)
 */
#define GUI_INITIAL_ACCRATE                   (20)

/* @brief Defines initial pwm cycle          */
#define GUI_INITIAL_PULSE_WIDTH               (0)

/* @brief Defines fault count limit          */
#define GUI_FAULT_LIMIT                       (10)

/* @brief Defines maximum pwm percentage           */
#define GUI_MAX_PULSEWIDTH                    (100)

/* @brief Defines the shunt resistace in milli ohms */
#define GUI_INITIAL_SHUNT_RES                 (1)

/* @brief Defines initial adc voltage reference */
#define GUI_INITIAL_ADCVREF                   HAL_ADC_VREF_VDDA

/* @brief Defines initial adc internal voltage reference */
#define GUI_INITIAL_ADC_INTVREF               HAL_ADC_INT_VREF_2P5V

/* @brief Defines initial adc external voltage reference */
#define GUI_INITIAL_ADC_EXTVREF               (3300)

/* @brief Defines the ADC reset status */
#define GUI_ADC_STATUS_RESET                  0x00

/* @brief Defines the status when adc data is ready */
#define GUI_ADC0_DATA_READY                   0x01

#ifdef GEN_ADC_CHAN_1_INST_IRQHandler
/* @brief Defines the status when adc data is ready */
#define GUI_ADC1_DATA_READY                   0x02

/* @brief Defines the status when adc data is ready */
#define GUI_ADC_READY                         0x03

#else
/* @brief Defines the status when adc data is ready */
#define GUI_ADC_READY                         0x01
#endif

/* @brief Defines number of poles in motor  */
#define GUI_MOTOR_NUMBER_OF_POLES             (8)

/* @brief Defines the over voltage limit    */
#define GUI_DEFAULT_OVER_VOLTAGE_LIMIT        (24)

/* @brief Defines the under voltage limit    */
#define GUI_DEFAULT_UNDER_VOLTAGE_LIMIT       (6)

/* @brief Defines the over current limit    */
#define GUI_DEFAULT_OVER_CURRENT_LIMIT        (30)

//*****************************************************************************
// typedefs
//*****************************************************************************
/*! @enum VAR */
typedef enum {
    /*! @brief Defines GUI Variable */
    VAR_GUI = 0,
    /*! @brief Defines Local Variable */
    VAR_LOCAL = 1,
    /*! @brief Maximum number of variables*/
    VAR_MAX
} VAR;

/*! @brief Defines the GUI parameters    */
typedef struct _GUIParams_
{
    /*! Stores the direction of rotation    */
    volatile MOTOR_DIR          motorDirection;
    /*! Stores the state of nsleep pin      */
    volatile DRV8329_DRV_NSLEEP_STAT    nsleepSignal;
    /*! Stores the state of DRVOFF pin      */
    volatile DRV8329_DRVOFF_PIN_STAT    drvoffSignal;
    /*! Stores the state of motor           */
    volatile MOTOR_STATE        stopMotor;
    /*! Stores the selected brake type      */
    volatile MOTOR_BRAKE_TYPE   motorBraketype;
    /*! Stores the selected CSA gain        */
    volatile DRV8329_CSAGAIN    CSAGain;
    /*! Stores the current reference voltage*/
    volatile uint16_t           csaRef;
    /*! Stores the PWM frequency input      */
    volatile uint16_t           pwmFreq;
    /*! Stores the deadtime value           */
    volatile uint16_t           deadTime_ns;
    /*! Stores the acceleration rate value  */
    volatile uint16_t           accRate;
    /*! Stores the pwm duty cycle           */
    volatile uint16_t           pulseWidth;
    /*! Stores the adc voltage reference    */
    volatile HAL_ADC_VREF       adcVRef;
    /*! Stores the internal voltage reference value */
    volatile HAL_ADC_INT_VREF   adcInternalVRef;
    /*! Stores the external voltage reference value */
    volatile uint16_t           adcExternalVRef;
    /*! Stores the shunt resistance value in milli ohm  */
    volatile uint16_t           rShunt_mohm;
}GUI_Params;

/*! @brief Defines the fault conditions    */
typedef struct _faults_
{
    /*!  fault reset bit */
    volatile bool Reset;
    /*!  Overvoltage fault flag  */
    volatile bool overVoltage;
    /*!  Undervoltage fault flag */
    volatile bool underVoltage;
    /*!  Drv fault flag */
    volatile bool drvFault;
    /*!  Overcurrent fault flag */
    volatile bool overCurrent;
    /*!  fault indication flag  */
    volatile bool ledIndiaction;
}GUI_faults;

/*! @brief Defines the voltage and current readings to GUI */
typedef struct
{
    /*! @brief Stores bus voltage */
    float busVoltage;
    /*! @brief Stores phase A voltage */
    float voltagePhaseA;
    /*! @brief Stores phase B voltage */
    float voltagePhaseB;
    /*! @brief Stores phase C voltage */
    float voltagePhaseC;
    /*! @brief Stores bus current */
    float busCurrent;
}GUI_readings;

//*****************************************************************************
//global variables
//*****************************************************************************
/** @brief Structure to store the firmware values */
extern GUI_Params       firmVar;

/** @brief Stores the status of ADC data availability   */
extern uint8_t          guiADCStatus;

/** @brief Stores the motor speed in rpm    */
extern uint32_t         guiMotorSpeed;

/** @brief Speed timeout    */
extern uint32_t         guiSpeedTimeout;

/** @brief Stores the number of poles in motor  */
extern uint32_t         guiMotorPoles;

/** @brief Structure to store voltage and current readings  */
extern GUI_readings     reading;

/** @brief Flag to start monitoring if motor stops */
extern MOTOR_STATE      motorState;

//*****************************************************************************
// the function prototypes
//*****************************************************************************
/**
 * @brief     setup initial GUI values
 * @param[in] drvHandle  The drv8329 instance
 */
void GUI_setupGuiValues(DRV8329_Instance *drv8329);

/**
 * @brief     checks for any change in GUI parameters
 * @param[in] drvHandle   The drv8329 instance
 * @param[in] hallTrap    The hallTrap instance
 */
void GUI_changeCheck(DRV8329_Instance *drv8329, Halltrap_Instance *hallTrap);

/**
 * @brief     accelerate motor
 * @param[in] hallTrap    The hallTrap instance
 */
void GUI_accMotor(Halltrap_Instance *hallTrap);

/**
 * @ brief converts IQ data to float
 */
void GUI_convertReadingsFromIQtoFloat();

#ifdef __cplusplus
}
#endif
#endif /* DRV8329REVM_GUI_H_ */
