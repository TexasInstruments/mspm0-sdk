#include "hal.h"
#include "drv8706-q1.h"
#include <string.h>

#define DEFAULT_GUI_FREQ        (20000)
#define DEFAULT_GUI_DUTY_CYCLE  (0)

#define DRV8706_EVM_RSENSE      (6.8)
#define DRV8706_CSA_REF         (3.3)

#define HALF_BRIDGE_CONNECT_UNK     (0)
#define HALF_BRIDGE_CONNECT_PVDD    (1)
#define HALF_BRIDGE_CONNECT_GND     (2)


/* IC Status Bits (13-8) from SPI Read */
#define SPI_READ_FAULT_BIT  (1 << 13)
#define SPI_READ_WARN_BIT   (1 << 12)
#define SPI_READ_DS_BIT     (1 << 11)
#define SPI_READ_UV_BIT     (1 << 10)
#define SPI_READ_OV_BIT     (1 << 9)
#define SPI_READ_OT_BIT     (1 << 8)

/* We need a way to determine if the user changed something of importance in
 * the gui. Using the XDS model that way we do that is take a look at the
 * variable mapped to a GUI element and see if its different than what we
 * previously saw. These structures as well as defines help reduce the amount
 * of repetitive code.
 */
typedef struct {
    uint16_t gui;
    uint16_t firmware;
} GUI_VAR_UINT16;

typedef struct {
    uint8_t gui;
    uint8_t firmware;
} GUI_VAR_UINT8;

typedef struct {
    bool gui;
    bool firmware;
} GUI_VAR_BOOL;

typedef struct {
    float gui;
    float firmware;
} GUI_VAR_FLOAT;

/* Checks to see if the gui variable is different than the last time
 * we checked.
 */
#define hasGUIVarChanged(X) (X.gui != X.firmware)

/* This sets the firmware variable to the gui variable. Therefore,
 * next time we know that nothing has changed unless the gui updates
 * it again */
#define syncFirmwareVar(X) (X.firmware = X.gui)

/* This function sets the gui variable to the default value specified
 * while also making sure the firmware value is different so during
 * the first loop everything will be configured properly.
 */
#define setGUIVarDefault(X,Y) \
    X.gui = Y; \
    X.firmware = (Y+1)

typedef enum {
    GUI_HALF_BRIDGE_MODE = 0,
    GUI_PHASE_MODE,
    GUI_PWM_MODE,
    GUI_SOLENOID_MODE,
    GUI_MAX_MODES
} GUI_MODES;

GUI_VAR_UINT16 guiHalfModeChan1Freq;
GUI_VAR_UINT8 guiHalfModeChan1DutyCycle;
GUI_VAR_BOOL guiHalfModeChan1Hiz;
GUI_VAR_UINT8 guiHalfConnectChan1;

GUI_VAR_UINT16 guiHalfModeChan2Freq;
GUI_VAR_UINT8 guiHalfModeChan2DutyCycle;
GUI_VAR_BOOL guiHalfModeChan2Hiz;
GUI_VAR_UINT8 guiHalfConnectChan2;

GUI_VAR_UINT16 guiPhaseModeFreq;
GUI_VAR_UINT8 guiPhaseModeDutyCycle;
GUI_VAR_BOOL guiPhaseModeDir;

GUI_VAR_UINT16 guiPWMModeFreq;
GUI_VAR_UINT8 guiPWMModeDutyCycle;
GUI_VAR_BOOL guiPWMModeDir;
GUI_VAR_BOOL guiPWMModeFreewheel;

GUI_VAR_UINT16 guiSolenoidModeFreq;
GUI_VAR_UINT8 guiSolenoidModeDutyCycle;
GUI_VAR_BOOL guiSolenoidModeEnable;

GUI_VAR_BOOL guiSleepPin;
GUI_VAR_BOOL guiDrvOffPin;

GUI_VAR_UINT8 guiDrvModeIndex;

GUI_VAR_UINT8 guiDrvCSAGain;
GUI_VAR_FLOAT guiRSense;
GUI_VAR_FLOAT guiARef;

/* These are variables mapped to the GUI but for various reasons we don't
 * care what this value was previously set as.
 */
bool guiWriteBitfieldRegBTN;
uint8_t guiWriteBitfieldAddr;
uint8_t guiBitfieldPosData;
uint8_t guiBitfieldMaskData;
uint8_t guiWriteBitfieldData;

bool guiWriteRegBTN;
uint8_t guiWriteAddr;
uint8_t guiWriteData;

bool guiReadRegBTN;
uint8_t guiReadAddr;
uint8_t guiReadData;


bool guiHalfModeContHide;
bool guiPhaseModeContHide;
bool guiPWMModeContHide;
bool guiSolenoidModeContHide;

bool guiFaultLED;
bool guiWarnLED;
bool guiDsLED;
bool guiUvLED;
bool guiOvLED;
bool guiOtLED;

bool guiFaultPin;

float guiPlotVoltageValue;
float guiPlotMotorValue;

volatile bool updateLoop = false;
DRV8706Q1_INSTANCE drv8706q1;

void gui_loop() {

    bool pwmFreeWheelChange = false;
    bool driveModeChanged = false;
    /* Determine if SPI was read during read commands */
    bool spiRead = false;
    uint16_t spiReadValue = 0;
    uint8_t modeSelection = guiDrvModeIndex.gui;

    if(hasGUIVarChanged(guiDrvModeIndex)) {
        syncFirmwareVar(guiDrvModeIndex);
       driveModeChanged = true;

        if(modeSelection == GUI_HALF_BRIDGE_MODE) {
            guiPhaseModeContHide = true;
            guiPWMModeContHide = true;
            guiSolenoidModeContHide = true;
            guiHalfModeContHide = false;
        } else if(modeSelection == GUI_PHASE_MODE) {
            guiHalfModeContHide = true;
            guiPWMModeContHide = true;
            guiSolenoidModeContHide = true;
            guiPhaseModeContHide = false;
        } else if(modeSelection == GUI_PWM_MODE) {
            guiHalfModeContHide = true;
            guiPhaseModeContHide = true;
            guiSolenoidModeContHide = true;
            guiPWMModeContHide = false;
        } else if(modeSelection == GUI_SOLENOID_MODE) {
            guiHalfModeContHide = true;
            guiPhaseModeContHide = true;
            guiPWMModeContHide = true;
            guiSolenoidModeContHide = false;
        }
    }

    if(driveModeChanged) {
        /* A lot of settings are changed when drive mode is updated. So to avoid
         * undesirable behavior from the motor we can just disable the outputs
         * to the motor, make changes and if required re-enable the outputs to the
         * motor.
         */
        DRV8706Q1_setDrvOff(drv8706q1,HIGH);
    }

    /* We frequently need to determine if the gui variable has changed and have
     * to react accordingly. But also we need to know if the drive mode has
     * changed. If the drive mode has changed then we need to reconfigure each
     * setting back to the value the user previously set.
     */
    if(modeSelection == GUI_HALF_BRIDGE_MODE) {
        if(driveModeChanged) {
            spiReadValue = DRV8706Q1_switchToHalfBridgeDrive(&drv8706q1,DRV8706Q1_HALF_BRIDGE_HI_Z,DRV8706Q1_HALF_BRIDGE_HI_Z);
            /* If mode has changed reset duty cycle to 0 to avoid issues */
            guiHalfModeChan1DutyCycle.gui = DEFAULT_GUI_DUTY_CYCLE;
            guiHalfModeChan2DutyCycle.gui = DEFAULT_GUI_DUTY_CYCLE;

            guiDrvOffPin.gui = 1;
            syncFirmwareVar(guiDrvOffPin);
            DRV8706Q1_setDrvOff(drv8706q1,guiDrvOffPin.gui);

            spiRead = true;

        }

        if(driveModeChanged || hasGUIVarChanged(guiHalfModeChan1Freq)) {
            syncFirmwareVar(guiHalfModeChan1Freq);
            DRV8706Q1_setDrivePWMFreq(drv8706q1,DRV8706Q1_PWM_CHANNEL_0,guiHalfModeChan1Freq.gui);

            #if defined(DeviceFamily_MSPM0L130X) || defined (DeviceFamily_MSPM0C110X) || defined (DeviceFamily_MSPM0H321X)
            /* Both PWM channels are on the same timer so changing frequency
             * affects both channels.
             */
            guiHalfModeChan2Freq.gui = guiHalfModeChan1Freq.gui;
            #endif
        }

        if(driveModeChanged || hasGUIVarChanged(guiHalfModeChan1DutyCycle)) {
            syncFirmwareVar(guiHalfModeChan1DutyCycle);

            uint8_t dutyCycle = guiHalfModeChan1DutyCycle.gui;

            /* When motor in half-bridge mode is connected to PVDD then duty
             * cycle behaves in the opposite manner. So compensate for that.
             */
            if(guiHalfConnectChan1.gui == HALF_BRIDGE_CONNECT_PVDD)
                dutyCycle = 100 - dutyCycle;

            DRV8706Q1_setDriveDutyCycle(drv8706q1,DRV8706Q1_PWM_CHANNEL_0,dutyCycle);
        }

        if(driveModeChanged || hasGUIVarChanged(guiHalfConnectChan1)) {
            syncFirmwareVar(guiHalfConnectChan1);

            /* HiZ the output anytime we change the connection setting */
            guiHalfModeChan1Hiz.gui = 1;

            /* Force Duty Cycle to bet reprogrammed since how dutyCycle is set
             * for PVDD is different for GND.
             */
            setGUIVarDefault(guiHalfModeChan1DutyCycle,
                             guiHalfModeChan1DutyCycle.gui);

        }

        /* If the user hasn't selected how the specific half-bridge channel is
         * connected then HiZ the channel.
         */
        if(guiHalfConnectChan1.gui == HALF_BRIDGE_CONNECT_UNK) {
            guiHalfModeChan1Hiz.gui = 1;
        }

        if(driveModeChanged || hasGUIVarChanged(guiHalfModeChan1Hiz)) {
            syncFirmwareVar(guiHalfModeChan1Hiz);
            if(guiHalfModeChan1Hiz.gui == 0) {
                DRV8706Q1_changeHalfBridgeDrive(drv8706q1,DRV8706Q1_PWM_CHANNEL_0,DRV8706Q1_HALF_BRIDGE_PWM);
            }
            else {
                DRV8706Q1_changeHalfBridgeDrive(drv8706q1,DRV8706Q1_PWM_CHANNEL_0,DRV8706Q1_HALF_BRIDGE_HI_Z);
            }
        }

        if(driveModeChanged || hasGUIVarChanged(guiHalfModeChan2Freq)) {
            syncFirmwareVar(guiHalfModeChan2Freq);
            DRV8706Q1_setDrivePWMFreq(drv8706q1,DRV8706Q1_PWM_CHANNEL_1,guiHalfModeChan2Freq.gui);

            #if defined(DeviceFamily_MSPM0L130X) || defined (DeviceFamily_MSPM0C110X) || defined (DeviceFamily_MSPM0H321X)
            /* Both PWM channels are on the same timer so changing frequency
             * affects both channels.
             */
            guiHalfModeChan1Freq.gui = guiHalfModeChan2Freq.gui;
            #endif
        }

        if(driveModeChanged || hasGUIVarChanged(guiHalfModeChan2DutyCycle)) {
            syncFirmwareVar(guiHalfModeChan2DutyCycle);
            uint8_t dutyCycle = guiHalfModeChan2DutyCycle.gui;

            /* When motor in half-bridge mode is connected to PVDD then duty
             * cycle behaves in the opposite manner. So compensate for that.
             */
            if(guiHalfConnectChan2.gui == HALF_BRIDGE_CONNECT_PVDD)
                dutyCycle = 100 - dutyCycle;

            DRV8706Q1_setDriveDutyCycle(drv8706q1,DRV8706Q1_PWM_CHANNEL_1,dutyCycle);
        }

        if(driveModeChanged || hasGUIVarChanged(guiHalfConnectChan2)) {
            syncFirmwareVar(guiHalfConnectChan2);

            /* HiZ the output anytime we change the connection setting */
            guiHalfModeChan2Hiz.gui = 1;

            /* Force Duty Cycle to bet reprogrammed since how dutyCycle is set
             * for PVDD is different for GND.
             */
            setGUIVarDefault(guiHalfModeChan2DutyCycle,
                             guiHalfModeChan2DutyCycle.gui);
        }

        /* If the user hasn't selected how the specific half-bridge channel is
         * connected then HiZ the channel.
         */
        if(guiHalfConnectChan2.gui == HALF_BRIDGE_CONNECT_UNK) {
            guiHalfModeChan2Hiz.gui = 1;
        }

        if(driveModeChanged || hasGUIVarChanged(guiHalfModeChan2Hiz)) {
            syncFirmwareVar(guiHalfModeChan2Hiz);
            if(guiHalfModeChan2Hiz.gui == 0) {
                DRV8706Q1_changeHalfBridgeDrive(drv8706q1,DRV8706Q1_PWM_CHANNEL_1,DRV8706Q1_HALF_BRIDGE_PWM);
            }
            else {
                DRV8706Q1_changeHalfBridgeDrive(drv8706q1,DRV8706Q1_PWM_CHANNEL_1,DRV8706Q1_HALF_BRIDGE_HI_Z);
            }
        }

    } else if(modeSelection == GUI_PHASE_MODE) {
        if(driveModeChanged) {
            spiReadValue = DRV8706Q1_switchToPhaseDrive(&drv8706q1,DRV8706Q1_PHASE_ACTIVE_FREEWHEEL);

            /* If mode has changed reset duty cycle to 0 to avoid issues */
            guiPhaseModeDutyCycle.gui = DEFAULT_GUI_DUTY_CYCLE;

            guiDrvOffPin.gui = 1;
            syncFirmwareVar(guiDrvOffPin);
            DRV8706Q1_setDrvOff(drv8706q1,guiDrvOffPin.gui);

            spiRead = true;
        }

        if(driveModeChanged || hasGUIVarChanged(guiPhaseModeDir)) {
            syncFirmwareVar(guiPhaseModeDir);

            /* If the direction changes lets set duty cycle back to zero since
             * we really should be waiting until the motor has stopped before
             * changing directions.
             */
            guiPhaseModeDutyCycle.gui = 0;

            if(guiPhaseModeDir.gui == 1) {
                DRV8706Q1_changePhaseDrive(drv8706q1,DRV8706Q1_PHASE_FORWARD);
            } else {
                DRV8706Q1_changePhaseDrive(drv8706q1,DRV8706Q1_PHASE_REVERSE);
            }
        }

        if(driveModeChanged || hasGUIVarChanged(guiPhaseModeFreq)) {
            syncFirmwareVar(guiPhaseModeFreq);
            DRV8706Q1_setDrivePWMFreq(drv8706q1,DRV8706Q1_PWM_CHANNEL_0,guiPhaseModeFreq.gui);
        }

        if(driveModeChanged || hasGUIVarChanged(guiPhaseModeDutyCycle)) {
            syncFirmwareVar(guiPhaseModeDutyCycle);
            DRV8706Q1_setDriveDutyCycle(drv8706q1,DRV8706Q1_PWM_CHANNEL_0,guiPhaseModeDutyCycle.gui);
        }



    } else if(modeSelection == GUI_PWM_MODE) {
        if(driveModeChanged) {
            spiReadValue = DRV8706Q1_switchToHBridgeDrive(&drv8706q1,DRV8706Q1_H_BRIDGE_COAST);
            /* If mode has changed reset duty cycle to 0 to avoid issues */
            guiPWMModeDutyCycle.gui = DEFAULT_GUI_DUTY_CYCLE;

            guiDrvOffPin.gui = 1;
            syncFirmwareVar(guiDrvOffPin);
            DRV8706Q1_setDrvOff(drv8706q1,guiDrvOffPin.gui);

            spiRead = true;
        }




        if(driveModeChanged || hasGUIVarChanged(guiPWMModeFreewheel)) {
            syncFirmwareVar(guiPWMModeFreewheel);

            if(guiPWMModeFreewheel.firmware == 1) {
                DRV8706Q1_changeHBridgeDrive(drv8706q1,DRV8706Q1_H_BRIDGE_ACTIVE_FREEWHEEL);
            }
            else
                pwmFreeWheelChange = true;

        }

        /* Ignore forward/reverse selection if freewheel is enabled */
        if (guiPWMModeFreewheel.firmware == 0) {
            /* If freewheel was just disabled then we need to change the drive back to the mode we selected */
            if(driveModeChanged || hasGUIVarChanged(guiPWMModeDir) || pwmFreeWheelChange == true) {
                syncFirmwareVar(guiPWMModeDir);

                /* If the direction changes lets set duty cycle back to zero since
                 * we really should be waiting until the motor has stopped before
                 * changing directions.
                 */
                guiPWMModeDutyCycle.gui = 0;

                if(guiPWMModeDir.gui == 1) {
                    DRV8706Q1_changeHBridgeDrive(drv8706q1,DRV8706Q1_H_BRIDGE_FORWARD);
                } else {
                    DRV8706Q1_changeHBridgeDrive(drv8706q1,DRV8706Q1_H_BRIDGE_REVERSE);
                }

            }
        }

        if(driveModeChanged || hasGUIVarChanged(guiPWMModeFreq)) {
            syncFirmwareVar(guiPhaseModeFreq);
            DRV8706Q1_setDrivePWMFreq(drv8706q1,DRV8706Q1_PWM_CHANNEL_0,guiPWMModeFreq.gui);
        }
        if(driveModeChanged || hasGUIVarChanged(guiPWMModeDutyCycle)) {
            syncFirmwareVar(guiPWMModeDutyCycle);
            DRV8706Q1_setDriveDutyCycle(drv8706q1,DRV8706Q1_PWM_CHANNEL_0,guiPWMModeDutyCycle.gui);

        }

    } else if(modeSelection == GUI_SOLENOID_MODE) {
        if(driveModeChanged) {
            spiReadValue = DRV8706Q1_switchToSolenoidDrive(&drv8706q1,DRV8706Q1_SOLENOID_DISABLE);
            /* If mode has changed reset duty cycle to 0 to avoid issues */
            guiSolenoidModeDutyCycle.gui = 0;

            guiDrvOffPin.gui = 1;
            syncFirmwareVar(guiDrvOffPin);
            DRV8706Q1_setDrvOff(drv8706q1,guiDrvOffPin.gui);

            spiRead = true;
        }

        if(driveModeChanged || hasGUIVarChanged(guiSolenoidModeEnable)) {
            syncFirmwareVar(guiSolenoidModeEnable);

            if(guiSolenoidModeEnable.gui) {
                DRV8706Q1_changeSolenoidDrive(drv8706q1,DRV8706Q1_SOLENOID_PWM);
            } else {
                DRV8706Q1_changeSolenoidDrive(drv8706q1,DRV8706Q1_SOLENOID_DISABLE);
            }
        }

        if(driveModeChanged || hasGUIVarChanged(guiSolenoidModeFreq)) {
            syncFirmwareVar(guiSolenoidModeFreq);
            DRV8706Q1_setDrivePWMFreq(drv8706q1,DRV8706Q1_PWM_CHANNEL_0,guiSolenoidModeFreq.gui);
        }

        if(driveModeChanged || hasGUIVarChanged(guiSolenoidModeDutyCycle)) {
            syncFirmwareVar(guiSolenoidModeDutyCycle);
            DRV8706Q1_setDriveDutyCycle(drv8706q1,DRV8706Q1_PWM_CHANNEL_0,guiSolenoidModeDutyCycle.gui);
        }
    }

    if(hasGUIVarChanged(guiSleepPin)) {
        syncFirmwareVar(guiSleepPin);
        DRV8706Q1_setNSleep(drv8706q1,guiSleepPin.gui);

        /* When the device has been asleep for more than 1 ms then all
         * SPI registers are set to their power on reset value. So we
         * need to reprogram all the settings.
         */
        if(guiSleepPin.gui) {
            setGUIVarDefault(guiDrvModeIndex,guiDrvModeIndex.gui);
            setGUIVarDefault(guiDrvCSAGain,guiDrvCSAGain.gui);
        }
    }

    if(guiWriteRegBTN) {
        DRV8706Q1_spiWriteCmd(drv8706q1,guiWriteAddr,guiWriteData);

        guiReadAddr = guiWriteAddr;
        spiReadValue = DRV8706Q1_spiReadCmd(drv8706q1,guiReadAddr);
        guiReadData = (uint8_t)(0xFF & spiReadValue);

        spiRead = true;
        guiWriteRegBTN = 0;
    }

    if(guiReadRegBTN) {
        spiReadValue = DRV8706Q1_spiReadCmd(drv8706q1,guiReadAddr);
        guiReadData = (uint8_t)(0xFF & spiReadValue);
        guiReadRegBTN = 0;
        spiRead = true;

    }

    if(guiWriteBitfieldRegBTN) {
        uint8_t mask = (guiBitfieldMaskData<<guiBitfieldPosData);
        uint8_t data = (guiWriteBitfieldData<<guiBitfieldPosData);
        DRV8706Q1_spiUpdateRegister(drv8706q1,guiWriteBitfieldAddr,mask,data);

        guiReadAddr = guiWriteBitfieldAddr;
        spiReadValue = DRV8706Q1_spiReadCmd(drv8706q1,guiReadAddr);
        guiReadData = (uint8_t)(0xFF & spiReadValue);

        guiWriteBitfieldRegBTN = 0;
        spiRead = true;
    }


    /* In the situation we changed drive mode and set the drv off pin
     * we want to reconfigure the pin back to the value the user specified.
     */
    if(driveModeChanged || hasGUIVarChanged(guiDrvOffPin)) {
        syncFirmwareVar(guiDrvOffPin);
        DRV8706Q1_setDrvOff(drv8706q1,guiDrvOffPin.gui);
    }


    if(hasGUIVarChanged(guiRSense)) {
        syncFirmwareVar(guiRSense);
        DRV8706Q1_setCurrentShuntResistor(&drv8706q1,_IQ20(guiRSense.gui));
    }

    if(hasGUIVarChanged(guiARef)) {
        syncFirmwareVar(guiARef);
        DRV8706Q1_setAnalogReference(&drv8706q1,_IQ20(guiARef.gui));
    }

    if(hasGUIVarChanged(guiDrvCSAGain)) {
        syncFirmwareVar(guiDrvCSAGain);
        DRV8706Q1_setCurrentSenseGain(&drv8706q1,guiDrvCSAGain.gui);
    }


    if(updateLoop) {
        guiPlotVoltageValue = _IQ20toF(HAL_getIQ20VoltageFromADC(drv8706q1.adcChan));
        updateLoop = false;
    }

    if(spiRead) {
        if( SPI_READ_FAULT_BIT & spiReadValue) {
            guiFaultLED = true;
        } else {
            guiFaultLED = false;
        }

        if( SPI_READ_WARN_BIT & spiReadValue) {
            guiWarnLED = true;
        } else {
            guiWarnLED = false;
        }

        if( SPI_READ_DS_BIT & spiReadValue) {
            guiDsLED = true;
        } else {
            guiDsLED = false;
        }

        if( SPI_READ_UV_BIT & spiReadValue) {
            guiUvLED = true;
        } else {
            guiUvLED = false;
        }

        if( SPI_READ_OV_BIT & spiReadValue) {
            guiOvLED = true;
        } else {
            guiOvLED = false;
        }

        if( SPI_READ_OT_BIT & spiReadValue) {
            guiOtLED = true;
        } else {
            guiOtLED = false;
        }
    }
}

void defaultGUIValues() {

    setGUIVarDefault(guiDrvModeIndex,GUI_PWM_MODE);

    guiHalfModeContHide = true;
    setGUIVarDefault(guiHalfModeChan1Freq,DEFAULT_GUI_FREQ);
    setGUIVarDefault(guiHalfModeChan1DutyCycle,DEFAULT_GUI_DUTY_CYCLE);
    setGUIVarDefault(guiHalfModeChan1Hiz,false);
    setGUIVarDefault(guiHalfConnectChan1,HALF_BRIDGE_CONNECT_UNK);

    setGUIVarDefault(guiHalfModeChan2Freq,DEFAULT_GUI_FREQ);
    setGUIVarDefault(guiHalfModeChan2DutyCycle,DEFAULT_GUI_DUTY_CYCLE);
    setGUIVarDefault(guiHalfModeChan2Hiz,false);
    setGUIVarDefault(guiHalfConnectChan2,HALF_BRIDGE_CONNECT_UNK);

    guiPhaseModeContHide = true;
    setGUIVarDefault(guiPhaseModeFreq,DEFAULT_GUI_FREQ);
    setGUIVarDefault(guiPhaseModeDutyCycle,DEFAULT_GUI_DUTY_CYCLE);
    setGUIVarDefault(guiPhaseModeDir,true);

    guiPWMModeContHide = true;
    setGUIVarDefault(guiPWMModeFreq,DEFAULT_GUI_FREQ);
    setGUIVarDefault(guiPWMModeDutyCycle,DEFAULT_GUI_DUTY_CYCLE);
    setGUIVarDefault(guiPWMModeDir,true);
    setGUIVarDefault(guiPWMModeFreewheel,false);

    guiSolenoidModeContHide = true;
    setGUIVarDefault(guiSolenoidModeFreq,DEFAULT_GUI_FREQ);
    setGUIVarDefault(guiSolenoidModeDutyCycle,DEFAULT_GUI_DUTY_CYCLE);
    setGUIVarDefault(guiSolenoidModeEnable,false);


    setGUIVarDefault(guiSleepPin,false);
    setGUIVarDefault(guiDrvOffPin,true);

    setGUIVarDefault(guiRSense,DRV8706_EVM_RSENSE);
    setGUIVarDefault(guiARef,DRV8706_CSA_REF);
    setGUIVarDefault(guiDrvCSAGain,DRV8706Q1_CSA_GAIN_20);


    /* We don't have to worry about detecting if these variables were changed
     * by the gui unexpectedly
     */

    guiFaultPin = false;

    guiWriteRegBTN = 0;
    guiWriteBitfieldRegBTN = 0;
    guiReadRegBTN = 0;

    guiFaultLED = 0;
    guiWarnLED = 0;
    guiDsLED = 0;
    guiUvLED = 0;
    guiOvLED = 0;
    guiOtLED = 0;

    guiWriteAddr = DRV8706Q1_SPI_ADDR_IC_CTRL;
    guiWriteData = 0;

    guiReadAddr = 0;
    guiReadData = 0;

    guiWriteBitfieldAddr = DRV8706Q1_SPI_ADDR_IC_CTRL;
    guiBitfieldPosData = 0;
    guiBitfieldMaskData = 0;
    guiWriteBitfieldData = 0;

    guiPlotVoltageValue = 0;
    guiPlotMotorValue = 0;
}



void drvConfiguration() {
    NVIC_EnableIRQ(GUI_TIMER_INST_INT_IRQN);
    NVIC_EnableIRQ(GENERIC_ADC_CHAN_0_INST_INT_IRQN);


    HAL_Init();
    drv8706q1.in1En = PWM_CHANNEL_0;
    drv8706q1.in2Ph = PWM_CHANNEL_1;
    drv8706q1.nFault = GPIO_INPUT_PIN_0;
    drv8706q1.nHiz1 = GPIO_OUTPUT_PIN_2;
    drv8706q1.nHiz2 = GPIO_OUTPUT_PIN_3;
    drv8706q1.nSleep = GPIO_OUTPUT_PIN_0;
    drv8706q1.drvOff = GPIO_OUTPUT_PIN_1;
    drv8706q1.spiChan = SPI_CHANNEL_0;
    drv8706q1.spiCS = SPI_CS_2;
    drv8706q1.adcChan = ADC_CHAN_0;

    DRV8706Q1_init(&drv8706q1);
}
int main(void)
{
    drvConfiguration();
    HAL_delayMilliSeconds(500);
    defaultGUIValues();


    while(1) {
        gui_loop();
    }
}


void GUI_TIMER_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(GUI_TIMER_INST)) {
        case DL_TIMER_IIDX_ZERO:
            updateLoop = true;

            break;
        default:
            break;
    }
}

void GENERIC_ADC_CHAN_0_INST_IRQHandler(void)
{
    DL_ADC12_IIDX pend_irq = HAL_processADCIRQ(GENERIC_ADC_CHAN_0_INST);
}
