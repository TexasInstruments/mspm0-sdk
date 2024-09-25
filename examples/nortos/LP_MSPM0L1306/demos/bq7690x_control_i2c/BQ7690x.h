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

//BQ7690x General Program Header File
#define CRC_Mode 0     // 0 for disabled, 1 for enabled
#define DEV_ADDR 0x08  // BQ7690x i2c address is 0x08
#define R 0            //Read
#define W 1            //Write
#define W2 2           //write data with two bytes

//-----Direct Commands BQ7690x TRM---------------------------------------------
#define SafetyAlertA \
    0x02  //Provides individual alert signals when enabled safety alerts have triggered.
#define SafetyStatusA \
    0x03  //Provides individual fault signals when enabled safety alerts have triggered.
#define SafetyAlertB \
    0x04  //Provides individual alert signals when enabled safety alerts have triggered.
#define SafetyStatusB \
    0x05  //Provides individual fault signals when enabled safety alerts have triggered.
#define BatteryStatus 0x12  //Provides flags related to battery status.
#define Cell1Voltage 0x14   //16-bit voltage on Cell1.
#define Cell2Voltage 0x16   //16-bit voltage on Cell2.
#define Cell3Voltage 0x18   //16-bit voltage on Cell3.
#define Cell4Voltage 0x1A   //16-bit voltage on Cell4.
#define Cell5Voltage 0x1C   //16-bit voltage on Cell5.
#define Cell6Voltage 0x1E   //16-bit voltage on Cell6.
#define Cell7Voltage 0x20   //16-bit voltage on Cell7.
#define REG18Voltage \
    0x22  //Internal 1.8V regulator voltage measured using bandgap ref. used to diagnose VREF1 vs VREF2.
#define VSSVoltage \
    0x24  //Measurement of VSS using ADC, used for diagnostic of ADC input mux.
#define StackVoltage 0x26  //16-bit voltage on top of stack.
#define IntTemperature \
    0x28  //The most recently measured internal die temperature.
#define TS1Temperature 0x2A  //ADC measurement of TS pin.
#define RawCurrent 0x36      //32-bit raw current measurement.
#define Current 0x3A         //16-bit CC2 current measurement.
#define CC1Current 0x3C      //16-bit CC1 current measurement.
#define AlarmStatus 0x62     //Latched signal used to assert the ALERT pin.
#define AlarmRawStatus \
    0x64  //Unlatched value of flags which can be selected to be latched and used to assert the ALERT pin.
#define AlarmEnable \
    0x66  //Mask for AlarmStatus , can be written during operation to change which alarm sources are enabled.
#define FETControl \
    0x68  //Allows host control of individual FET drivers. "FETCONTROL in TRM"
#define REGOUTControl \
    0x69  //Changes voltage regulator settings. "REGOUTCONTROL in TRM"
#define DSGFETDriverPWMControl \
    0x6A  //Controls the PWM mode of the DSG FET driver.
#define CHGFETDriverPWMControl \
    0x6C  //Controls the PWM mode of the CHG FET driver

//----Command-only Subcommands -----------------------------------------------
#define RESET_PASSQ \
    0x0005  //This command resets the accumulated charge and timer.
#define EXIT_DEEPSLEEP 0x000E  //This command is sent to exit DEEPSLEEP mode.
#define DEEPSLEEP \
    0x000F  //This command is sent to enter DEEPSLEEP mode, sent twice in <4s
#define SHUTDOWN \
    0x0010  //This command is sent to start SHUTDOWN sequence, sent twice <4s
#define RESET 0x0012       //This command is sent to reset the device.
#define FET_ENABLE 0x0022  //This command is sent to toggle the FET_EN bit.
#define SEAL 0x0030  //This command is sent to place the device in SEALED mode.
#define SET_CFGUPDATE \
    0x0090  //This command is sent to place the device in CONFIG_UPDATE mode.
#define EXIT_CFGUPDATE \
    0x0092  //This command is sent to exit CONFIG_UPDATE mode.
#define SLEEP_ENABLE \
    0x0099  //This command is sent to allow the device to enter SLEEP mode.
#define SLEEP_DISABLE \
    0x009A  //This command is sent to block the device from entering SLEEP mode.

//-------Subcommands ----------------------------------------------------------
#define DEVICE_NUMBER \
    0x0001  //Reports the device number that identifies the product. Data is returned in little-endian format.
#define FW_VERSION \
    0x0002  //Returns three 16-bit word values. See TRM for full details.
#define HW_VERSION 0x0003  //Reports device hardware version number.
#define PASSQ 0x0004       //See TRM for full details.
#define SECURITY_KEYS \
    0x0035  //Security key that must be sent to transition from SEALED to FULLACCESS mode.
#define CB_ACTIVE_CELLS \
    0x0083  //When read, reports bit mask of which cells are being actively balanced.
#define PROG_TIMER \
    0x0094  //Programmable timer, which allows the REGOUT LDO to be disabled and wakened after a programmed time or by alarm.
#define PROT_RECOVERY \
    0x009B  //This command enables the host to allow recovery of selected protection faults.

//-Device Register Addresses as in the BQ7690x Technical Reference Manual (TRM)-
#define CellGain 0x9000          //Calibration:Voltage:Cell Gain
#define StackGain 0x9002         //Calibration:Voltage:Stack Gain
#define CellOffset 0x9004        //Calibration:Voltage:Cell Offset
#define CurrGain 0x9006          //Calibration:Current:Curr Gain
#define CurrOffset 0x9008        //Calibration:Current:Curr Offset
#define CC1Gain 0x900a           //Calibration:Current:CC1 Gain
#define CC1Offset 0x900c         //Calibration:Current:CC1 Offset
#define TSOffset 0x900e          //Calibration:Temperature:TS Offset
#define IntTempGain 0x9010       //Calibration:Temperature:Int Temp Gain
#define IntTempOffset 0x9012     //Calibration:Temperature:Int Temp Offset
#define PowerConfig 0x9014       //Settings:Configuration:Power Config
#define REGOUTConfig 0x9015      //Settings:Configuration:REGOUT Config
#define I2CAddress 0x9016        //Settings:Configuration:I2C Address
#define I2CConfig 0x9017         //Settings:Configuration:I2C Config
#define DAConfig 0x9019          //Settings:Configuration:DA Config
#define VcellMode 0x901b         //Settings:Configuration:Vcell Mode
#define DefaultAlarmMask 0x901c  //Settings:Configuration:Default Alarm Mask
#define FETOptions 0x901e        //Settings:Configuration:FET Options
#define ChargeDetectorTime \
    0x901f  //Settings:Configuration:Charge Detector Time
#define BalancingConfiguration \
    0x9020  //Settings:Cell Balancing:Balancing Configuration
#define MinTempThreshold 0x9021  //Settings:Cell Balancing:Min Temp Threshold
#define MaxTempThreshold 0x9022  //Settings:Cell Balancing:Max Temp Threshold
#define MaxInternalTemp 0x9023   //Settings:Cell Balancing:Max Internal Temp
#define EnabledProtectionsA 0x9024  //Settings:Protection:Enabled Protections A
#define EnabledProtectionsB 0x9025  //Settings:Protection:Enabled Protections B
#define DSGFETProtectionsA 0x9026   //Settings:Protection:DSG FET Protections A
#define CHGFETProtectionsA 0x9027   //Settings:Protection:CHG FET Protections A
#define BothFETProtectionsB \
    0x9028                         //Settings:Protection:Both FET Protections B
#define BodyDiodeThreshold 0x9029  //Settings:Protection:Body Diode Threshold
#define CellOpenWireNORMALCheckTime \
    0x902b  //Settings:Protection:Cell Open Wire NORMAL Check Time
#define CellOpenWireSLEEPCheckTime \
    0x902c  //Settings:Protection:Cell Open Wire SLEEP Check Time
#define HostWatchdogTimeout 0x902d  //Settings:Protection:Host Watchdog Timeout
#define CellUndervoltageProtectionThreshold \
    0x902e  //Protections:Cell Voltage:Cell Undervoltage Protection Threshold
#define CellUndervoltageProtectionDelay \
    0x9030  //Protections:Cell Voltage:Cell Undervoltage Protection Delay
#define CellUndervoltageProtectionRecoveryHysteresis \
    0x9031  //Protections:Cell Voltage:Cell Undervoltage Protection Recovery Hysteresis
#define CellOvervoltageProtectionThreshold \
    0x9032  //Protections:Cell Voltage:Cell Overvoltage Protection Threshold
#define CellOvervoltageProtectionDelay \
    0x9034  //Protections:Cell Voltage:Cell Overvoltage Protection Delay
#define CellOvervoltageProtectionRecoveryHysteresis \
    0x9035  //Protections:Cell Voltage:Cell Overvoltage Protection Recovery Hysteresis
#define OvercurrentinChargeProtectionThreshold \
    0x9036  //Protections:Current:Overcurrent in Charge Protection Threshold
#define OvercurrentinChargeProtectionDelay \
    0x9037  //Protections:Current:Overcurrent in Charge Protection Delay
#define OvercurrentinDischarge1ProtectionThreshold \
    0x9038  //Protections:Current:Overcurrent in Discharge 1 Protection Threshold
#define OvercurrentinDischarge1ProtectionDelay \
    0x9039  //Protections:Current:Overcurrent in Discharge 1 Protection Delay
#define OvercurrentinDischarge2ProtectionThreshold \
    0x903a  //Protections:Current:Overcurrent in Discharge 2 Protection Threshold
#define OvercurrentinDischarge2ProtectionDelay \
    0x903b  //Protections:Current:Overcurrent in Discharge 2 Protection Delay
#define ShortCircuitinDischargeProtectionThreshold \
    0x903c  //Protections:Current:Short Circuit in Discharge Protection Threshold
#define ShortCircuitinDischargeProtectionDelay \
    0x903d  //Protections:Current:Short Circuit in Discharge Protection Delay
#define LatchLimit 0x903e    //Protections:Current:Latch Limit
#define RecoveryTime 0x903f  //Protections:Current:Recovery Time
#define OvertemperatureinChargeProtectionThreshold \
    0x9040  //Protections:Temperature:Overtemperature in Charge Protection Threshold
#define OvertemperatureinChargeProtectionDelay \
    0x9041  //Protections:Temperature:Overtemperature in Charge Protection Delay
#define OvertemperatureinChargeProtectionRecovery \
    0x9042  //Protections:Temperature:Overtemperature in Charge Protection Recovery
#define UndertemperatureinChargeProtectionThreshold \
    0x9043  //Protections:Temperature:Undertemperature in Charge Protection Threshold
#define UndertemperatureinChargeProtectionDelay \
    0x9044  //Protections:Temperature:Undertemperature in Charge Protection Delay
#define UndertemperatureinChargeProtectionRecovery \
    0x9045  //Protections:Temperature:Undertemperature in Charge Protection Recovery
#define OvertemperatureinDischargeProtectionThreshold \
    0x9046  //Protections:Temperature:Overtemperature in Discharge Protection Threshold
#define OvertemperatureinDischargeProtectionDelay \
    0x9047  //Protections:Temperature:Overtemperature in Discharge Protection Delay
#define OvertemperatureinDischargeProtectionRecovery \
    0x9048  //Protections:Temperature:Overtemperature in Discharge Protection Recovery
#define UndertemperatureinDischargeProtectionThreshold \
    0x9049  //Protections:Temperature:Undertemperature in Discharge Protection Threshold
#define UndertemperatureinDischargeProtectionDelay \
    0x904a  //Protections:Temperature:Undertemperature in Discharge Protection Delay
#define UndertemperatureinDischargeProtectionRecovery \
    0x904b  //Protections:Temperature:Undertemperature in Discharge Protection Recovery
#define InternalOvertemperatureProtectionThreshold \
    0x904c  //Protections:Temperature:Internal Overtemperature Protection Threshold
#define InternalOvertemperatureProtectionDelay \
    0x904d  //Protections:Temperature:Internal Overtemperature Protection Delay
#define InternalOvertemperatureProtectionRecovery \
    0x904e  //Protections:Temperature:Internal Overtemperature Protection Recovery
#define SleepCurrent 0x904f           //Power:Sleep:Sleep Current
#define VoltageTime 0x9051            //Power:Sleep:Voltage Time
#define WakeComparatorCurrent 0x9052  //Power:Sleep:Wake Comparator Current
#define ShutdownCellVoltage 0x9053    //Power:Shutdown:Shutdown Cell Voltage
#define ShutdownStackVoltage 0x9055   //Power:Shutdown:Shutdown Stack Voltage
#define ShutdownTemperature 0x9057    //Power:Shutdown:Shutdown Temperature
#define AutoShutdownTime 0x9058       //Power:Shutdown:Auto Shutdown Time
#define SecuritySettings 0x9059       //Security:Settings:Security Settings
#define FullAccessKeyStep1 0x905a  //Security:Settings:Full Access Key Step 1
#define FullAccessKeyStep2 0x905c  //Security:Settings:Full Access Key Step 2
