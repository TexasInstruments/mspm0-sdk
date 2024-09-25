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

#include <stdint.h>
#include <stdio.h>
#include "BQ7690x.h"
#include "BQ7690x_i2c.h"
#include "ti_msp_dl_config.h"

/* Global Variables BQ7690x voltages, temperatures, currents, and status */

// Measurements
int16_t CellVoltage[7] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // Cell Voltages
int16_t Stack_Voltage = 0x00;                   // Stack Voltage
int16_t VSS_Voltage   = 0x00;                   // VSS Voltage
int16_t REG18_Voltage = 0x00;                   // REG18 LDO Voltage
int16_t CC2_Current   = 0x00;                   // CC2 Current
int16_t CC1_Current   = 0x00;                   // CC1 Current
uint16_t IntTemp      = 0x00;                   // Internal Temperature

// Alarm and Status
uint16_t AlarmBits = 0x00;
uint8_t value_SafetyStatusA;  // Safety Status Register A
uint8_t value_SafetyStatusB;  // Safety Status Register B
uint8_t Battery_Status;       // FET Status Register
uint16_t CB_ActiveCells;      // Cell Balancing Active Cells

// Coulomb Counter
uint32_t PASSQLSB;   // CC PASSQ LSB
uint32_t PASSQMSB;   // CC PASSQ MSB
uint32_t PASSQTIME;  // CC PASSQ TIME

// System Faults
uint8_t UV_Fault             = 0;  // under-voltage fault state
uint8_t OV_Fault             = 0;  // over-voltage fault state
uint8_t SCD_Fault            = 0;  // short-circuit fault state
uint8_t OCD_Fault            = 0;  // over-current fault state
uint8_t ProtectionsTriggered = 0;  // Set to 1 if any protection triggers

// Battery Status Fields
uint8_t DSG              = 0;  // discharge FET state
uint8_t CHG              = 0;  // charge FET state
uint8_t SLEEP            = 0;  // SLEEP state
uint8_t DEEPSLEEP_Status = 0;  // DEEPSLEEP state
uint8_t SafetyAlert      = 0;  // SafetyAlert SA state
uint8_t SafetyFault      = 0;  // Safety Fault SS state
uint8_t Security_Mode    = 0;  // SecurityMode SEC1:0 state
uint8_t FET_Control_Mode =
    0;  // FET_EN status; autonomous or host controlled mode.
uint8_t POR_Flag       = 0;  // POR status flag
uint8_t SLEEP_Mode     = 0;  // SLEEP_EN state
uint8_t CFGUPDATE_Mode = 0;  // CFGUPDATE mode state
uint8_t ALERTPIN       = 0;  // ALERTPIN state, 0 = Hi-Z , 1=Low
uint8_t CHGDETFLAG_Mode =
    0;  // CHGDETFLAG charge detector signal debounce state

/*Communication Buffers*/
uint8_t RX_32Byte[32] = {0x00};  // Read buffer for Subcommands
uint8_t RX_data[2]    = {0x00};  // Read buffer for direct commands

/********* Common Functions *********/

// Set delay in microseconds
void delayUS(uint16_t us)
{
    uint16_t ms;
    char i;
    ms = us / 1000;
    for (i = 0; i < ms; i++) delay_cycles(32000);
}

// Copy Array
void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count)
{
    uint8_t copyIndex = 0;
    for (copyIndex = 0; copyIndex < count; copyIndex++) {
        dest[copyIndex] = source[copyIndex];
    }
}

/********* Communication Functions *********/

// See the TRM or the BQ7690x header file for a full list of Direct Commands
void DirectCommands(uint8_t command, uint16_t data, uint8_t type)
{
    // type: R = read, W = write
    uint8_t TX_data[2] = {0x00, 0x00};

    // little endian format
    TX_data[0] = data & 0xff;
    TX_data[1] = (data >> 8) & 0xff;

    if (type == R) {                    // Read
        I2C_Read(command, RX_data, 2);  // RX_data is a global variable
        delayUS(2000);
    }
    if (type == W) {  // write
        I2C_Write(command, TX_data, 2);
        delayUS(2000);
    }
}

// See the TRM or the BQ7690x header file for a full list of Subcommands
void Subcommands(uint16_t command, uint16_t data, uint8_t type)
{
    // type: R = read, W = write 1 byte, W2 = write 2 bytes
    // max readback size is 32 bytes
    uint8_t TX_Reg[4]    = {0x00, 0x00, 0x00, 0x00};
    uint8_t TX_Buffer[2] = {0x00, 0x00};

    // TX_Reg in little endian format
    TX_Reg[0] = command & 0xff;
    TX_Reg[1] = (command >> 8) & 0xff;

    if (type == R) {  // read
        I2C_Write(0x3E, TX_Reg, 2);
        delayUS(2000);
        I2C_Read(0x40, RX_32Byte,
            12);  // RX_32Byte is a global variable. 12Byte maximum for BQ7690x Read.
    }

    else if (type == W) {  // write
        // CB_Active_Cells, PROT_RECOVERY
        TX_Reg[2] = data & 0xff;
        I2C_Write(0x3E, TX_Reg, 3);
        delayUS(2000);
        TX_Buffer[0] = Checksum(TX_Reg, 3);
        TX_Buffer[1] = 0x05;  // combined length of registers address and data
        I2C_Write(0x60, TX_Buffer, 2);
        delayUS(2000);
    }

    else if (type == W2) {  // write data with 2 bytes
        TX_Reg[2] = data & 0xff;
        TX_Reg[3] = (data >> 8) & 0xff;
        I2C_Write(0x3E, TX_Reg, 4);
        delayUS(2000);
        TX_Buffer[0] = Checksum(TX_Reg, 4);
        TX_Buffer[1] = 0x06;  // combined length of registers address and data
        I2C_Write(0x60, TX_Buffer, 2);
        delayUS(2000);
    }
}

// See the TRM or the BQ769x0 header file for a full list of Command-only subcommands
void CommandSubcommands(uint16_t command)
{
    //Note: For DEEPSLEEP/SHUTDOWN subcommand you will need to call this function twice consecutively
    uint8_t TX_Reg[2] = {0x00, 0x00};

    //TX_Reg in little endian format
    TX_Reg[0] = command & 0xff;
    TX_Reg[1] = (command >> 8) & 0xff;

    I2C_Write(0x3E, TX_Reg, 2);
    delayUS(2000);
}

/********* BQ7690x Command Functions *********/

uint16_t BQ7690x_Device_Number()
// This function is used to read the Device Number
{
    DirectCommands(DEVICE_NUMBER, 0x00, R);
    return (RX_data[1] * 256 + RX_data[0]);
}

uint16_t BQ7690x_FW_Version()
// This function is used to read the Firmware Version
{
    DirectCommands(FW_VERSION, 0x00, R);
    return (RX_data[1] * 256 + RX_data[0]);
}

uint16_t BQ7690x_HW_Version()
// This function is used to read the Hardware Version
{
    DirectCommands(HW_VERSION, 0x00, R);
    return (RX_data[1] * 256 + RX_data[0]);
}

void BQ7690x_Reset_PassQ()
// This function resets the accumulated charge and timer
{
    CommandSubcommands(RESET_PASSQ);
}

void BQ7690x_Deepsleep()
// This function is used to command the device to enter DEEPSLEEP mode
{
    CommandSubcommands(DEEPSLEEP);
}

void BQ7690x_Exit_Deepsleep()
// This function is used to command the device to exit DEEPSLEEP mode
{
    CommandSubcommands(EXIT_DEEPSLEEP);
}

void BQ7690x_Shutdown()
// This function is used to command the device to enter SHUTDOWN mode
{
    CommandSubcommands(SHUTDOWN);
    delay_cycles(1000);
    CommandSubcommands(SHUTDOWN);
}

void BQ7690x_Reset()
// This function resets the device
{
    CommandSubcommands(RESET);
    delay_cycles(50000);
}

void BQ7690x_FET_Enable()
// This function is used to toggle the FET_EN bit
{
    CommandSubcommands(FET_ENABLE);
}

void BQ7690x_Seal()
// This function is used to place the device in SEALED mode
{
    CommandSubcommands(SEAL);
}

void BQ7690x_SetConfigUpdate()
// This function is used to enter CONFIG_UPDATE mode
{
    CommandSubcommands(SET_CFGUPDATE);
}

void BQ7690x_ExitConfigUpdate()
// This function is used to exit CONFIG_UPDATE mode
{
    CommandSubcommands(EXIT_CFGUPDATE);
}

void BQ7690x_Sleep_Enable()
// This function is used to allow the device to enter SLEEP mode
{
    CommandSubcommands(SLEEP_ENABLE);
}

void BQ7690x_Sleep_Disable()
// This function is used to disallow the device from entering SLEEP mode
{
    CommandSubcommands(SLEEP_DISABLE);
}

/********* BQ7690x Measurement Commands *********/

int16_t BQ7690x_ReadVoltage(uint8_t command)
// This function can be used to read a specific cell voltage or stack / pack / LD voltage
{
    //RX_data is global var
    DirectCommands(command, 0x00, R);
    switch (command) {
        case REG18Voltage:  // Voltage is reported in 16-bit ADC Code: convert to mV
            return (RX_data[1] * 256 + RX_data[0]) * 5 * 1215 / (2 * 32768);
            break;
        case VSSVoltage:  // Voltage is reported in 16-bit ADC Code: convert to mV
            return (RX_data[1] * 256 + RX_data[0]) * 5 * 1.215 / (3 * 32767);
            break;
        default:  // Voltage is reported in mV
            return (RX_data[1] * 256 + RX_data[0]);
            break;
    }
}

void BQ7690x_ReadAllVoltages()
// Reads all cell voltages, Stack voltage, REG18 pin voltage, and VSS voltage
{
    unsigned char x;
    int cellvoltageholder = Cell1Voltage;
    for (x = 0; x < 7; x++) {
        CellVoltage[x] = BQ7690x_ReadVoltage(cellvoltageholder);
        cellvoltageholder += 2;
    }
    Stack_Voltage = BQ7690x_ReadVoltage(StackVoltage);
    REG18_Voltage = BQ7690x_ReadVoltage(REG18Voltage);
    VSS_Voltage   = BQ7690x_ReadVoltage(VSSVoltage);
}

void BQ7690x_ReadCurrent()
// This function can be used to read CC2 current
{
    DirectCommands(Current, 0x00, R);
    CC2_Current =
        (RX_data[1] * 256 + RX_data[0]);  // current is reported in mA
}

void BQ7690x_ReadCC1Current()
// This function can be used to read CC1 current
{
    DirectCommands(CC1Current, 0x00, R);
    CC1_Current =
        (RX_data[1] * 256 + RX_data[0]);  // current is reported in mA
}

void BQ7690x_ReadIntTemperature()
{
    DirectCommands(IntTemperature, 0x00, R);
    //RX_data is a global var
    IntTemp = (RX_data[1] * 256 + RX_data[0]);
}

/********* BQ7690x Getter Functions *********/

// Measurements
void BQ7690x_Get_CellVoltages(int16_t *voltages)  // Cell Voltages
{
    for (int i = 0; i < 7; i++) {
        voltages[i] = CellVoltage[i];
    }
}

int16_t BQ7690x_Get_Stack_Voltage()  // Stack Voltage
{
    return Stack_Voltage;
}

int16_t BQ7690x_Get_VSS_Voltage()  // VSS Voltage
{
    return VSS_Voltage;
}

int16_t BQ7690x_Get_REG18_Voltage()  // REG18 LDO Voltage
{
    return REG18_Voltage;
}

int16_t BQ7690x_Get_CC2_Current()  // CC2 Current
{
    return CC2_Current;
}

int16_t BQ7690x_Get_CC1_Current()  // CC1 Current
{
    return CC1_Current;
}

uint16_t BQ7690x_Get_IntTemp()  // Internal Temperature
{
    return IntTemp;
}

uint16_t BQ7690x_Get_RX_data()  // Read buffer for direct commands
{
    return (RX_data[1] << 8) + RX_data[0];
}
