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

#include "BQ769x2_Configs/BQ769x2_protocol.h"

//******************************************************************************
// BQ Parameters ***************************************************************
//******************************************************************************

#define Current_0mA 1
#define Current_3A 0
#define Current_6A 0
#define CCA_CCB_Ready 0

volatile uint16_t Battery_status = 0;
volatile uint8_t ProtectionsTriggered =
    0;  // Set to 1 if any protection triggers
volatile uint32_t CFETOFF_Count = 0x0000;
volatile uint32_t DFETOFF_Count = 0x0000;
volatile uint32_t ALERT_Count   = 0x0000;
volatile uint32_t TS1_Count     = 0x0000;
volatile uint32_t TS2_Count     = 0x0000;
volatile uint32_t TS3_Count     = 0x0000;
volatile uint32_t HDQ_Count     = 0x0000;
volatile uint32_t DCHG_Count    = 0x0000;
volatile uint32_t DDSG_Count    = 0x0000;

// Global Variables for cell voltages, temperatures, Stack voltage, PACK Pin voltage, LD Pin voltage, CC2 current
uint16_t CellVoltage[16] = {0x01, 0x02, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint16_t Stack_Voltage = 0x00;
uint16_t Pack_Voltage  = 0x00;
uint16_t LD_Voltage    = 0x00;
int16_t Pack_Current   = 0x00;
//int16_t Pack_Current_filtered = 0x00;
uint16_t AlarmBits = 0x00;

uint8_t value_SafetyStatusA;  // Safety Status Register A
uint8_t value_SafetyStatusB;  // Safety Status Register B
uint8_t value_SafetyStatusC;  // Safety Status Register C
uint8_t value_PFStatusA;      // Permanent Fail Status Register A
uint8_t value_PFStatusB;      // Permanent Fail Status Register B
uint8_t value_PFStatusC;      // Permanent Fail Status Register C
uint8_t FET_Status;  // FET Status register contents  - Shows states of FETs
uint16_t CB_ActiveCells;  // Cell Balancing Active Cells

uint8_t UV_Fault     = 0;  // under-voltage fault state
uint8_t OV_Fault     = 0;  // over-voltage fault state
uint8_t SCD_Fault    = 0;  // short-circuit fault state
uint8_t OCD2_Fault   = 0;  // over-current fault state
uint8_t FULLSCAN     = 0x00;
uint8_t ADCSCAN      = 0x00;
uint8_t TOP_FULLSCAN = 0x00;
uint8_t BOT_FULLSCAN = 0x00;
uint8_t TOP_ADCSCAN  = 0x00;
uint8_t BOT_ADCSCAN  = 0x00;

uint8_t LD_ON = 0;  // Load Detect status bit
uint8_t DSG   = 0;  // discharge FET state
uint8_t CHG   = 0;  // charge FET state
uint8_t PCHG  = 0;  // pre-charge FET state
uint8_t PDSG  = 0;  // pre-discharge FET state

uint32_t AccumulatedCharge_Int;   // in AFE_READPASSQ func
uint32_t AccumulatedCharge_Frac;  // in AFE_READPASSQ func
uint32_t AccumulatedCharge_Time;  // in AFE_READPASSQ func

uint8_t OTP_condition          = 0;
uint8_t OTP_write_check        = 0;
uint8_t OTP_write_success_flag = 0;
uint8_t FULLACCESS             = 0;
uint8_t FET_EN;

int16_t CC2_Counts_Raw[10]    = {0x0000};
int16_t CC2_Counts_Calibrated = 0x00;
int16_t CC2_Counts_Average    = 0x00;
int16_t CC_Offset_Samples     = 0x00;
int16_t Board_Offset          = 0x00;
int16_t CC2_Counts_A          = 0x00;
int16_t CC2_Counts_B          = 0x00;
float CC_Gain                 = 0x00;

// Arrays
unsigned char RX_32Byte[32] = {0x00};
unsigned char RX_data[2]    = {0x00};
unsigned char Result        = 1;

//**********************************BQ Parameters *********************************

//**********************************Function prototypes **********************************
void delayUS(uint16_t us)
{  // Sets the delay in microseconds.
    uint16_t ms;
    char i;
    ms = us / 1000;
    for (i = 0; i < ms; i++) delay_cycles(80000);
}

unsigned char Checksum(unsigned char *ptr, unsigned char len)
// Calculates the checksum when writing to a RAM register. The checksum is the inverse of the sum of the bytes.
{
    unsigned char i;
    unsigned char checksum = 0;

    for (i = 0; i < len; i++) checksum += ptr[i];

    checksum = 0xff & ~checksum;

    return (checksum);
}

unsigned char CRC8(unsigned char *ptr, unsigned char len)
//Calculates CRC8 for passed bytes. Used in i2c read and write functions
{
    unsigned char i;
    unsigned char crc = 0;
    while (len-- != 0) {
        for (i = 0x80; i != 0; i /= 2) {
            if ((crc & 0x80) != 0) {
                crc *= 2;
                crc ^= 0x107;
            } else
                crc *= 2;

            if ((*ptr & i) != 0) crc ^= 0x107;
        }
        ptr++;
    }
    return (crc);
}

void DirectCommands(uint8_t command, uint16_t data, uint8_t type)
// See the TRM or the BQ76952 header file for a full list of Direct Commands
{  //type: R = read, W = write
    uint8_t TX_data[4] = {0x00, 0x00};

    //little endian format
    TX_data[0] = data & 0xff;
    TX_data[1] = (data >> 8) & 0xff;

    if (type == R) {                       //Read
        I2C_ReadReg(command, RX_data, 2);  //RX_data is a global variable
    }
    if (type == W) {  //write
        //Control_status, alarm_status, alarm_enable all 2 bytes long
        I2C_WriteReg(command, TX_data, 2);
    }
}

void CommandSubcommands(uint16_t command)  //For Command only Subcommands
// See the TRM or the BQ76952 header file for a full list of Command-only subcommands
{  //For DEEPSLEEP/SHUTDOWN subcommand you will need to call this function twice consecutively

    uint8_t TX_Reg[2] = {0x00, 0x00};

    //TX_Reg in little endian format
    TX_Reg[0] = command & 0xff;
    TX_Reg[1] = (command >> 8) & 0xff;

    I2C_WriteReg(0x3E, TX_Reg, 2);
    delayUS(2000);
}

void Subcommands(uint16_t command, uint16_t data, uint8_t type)
// See the TRM or the BQ76952 header file for a full list of Subcommands
{
    //security keys and Manu_data writes dont work with this function (reading these commands works)
    //max readback size is 32 bytes i.e. DASTATUS, CUV/COV snapshot
    uint8_t TX_Reg[4]    = {0x00, 0x00, 0x00, 0x00};
    uint8_t TX_Buffer[2] = {0x00, 0x00};

    //TX_Reg in little endian format
    TX_Reg[0] = command & 0xff;
    TX_Reg[1] = (command >> 8) & 0xff;

    if (type == R) {  //read
        I2C_WriteReg(0x3E, TX_Reg, 2);
        delayUS(
            2000);  //Need be longer than 500us, otherwise, return 0xFFFFFFFF
        I2C_ReadReg(0x40, RX_32Byte, 32);  //RX_32Byte is a global variable
    } else if (type == W) {
        //FET_Control, REG12_Control
        TX_Reg[2] = data & 0xff;
        I2C_WriteReg(0x3E, TX_Reg, 3);
        delayUS(1000);
        TX_Buffer[0] = Checksum(TX_Reg, 3);
        TX_Buffer[1] = 0x05;  //combined length of registers address and data
        I2C_WriteReg(0x60, TX_Buffer, 2);
        delayUS(1000);
    } else if (type == W2) {  //write data with 2 bytes
        //CB_Active_Cells, CB_SET_LVL
        TX_Reg[2] = data & 0xff;
        TX_Reg[3] = (data >> 8) & 0xff;
        I2C_WriteReg(0x3E, TX_Reg, 4);
        delayUS(1000);
        TX_Buffer[0] = Checksum(TX_Reg, 4);
        TX_Buffer[1] = 0x06;  //combined length of registers address and data
        I2C_WriteReg(0x60, TX_Buffer, 2);
        delayUS(1000);
    }
}

void BQ769x2_SetRegister(uint16_t reg_addr, uint32_t reg_data, uint8_t datalen)
{
    uint8_t TX_Buffer[2]  = {0x00, 0x00};
    uint8_t TX_RegData[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    //TX_RegData in little endian format
    TX_RegData[0] = reg_addr & 0xff;
    TX_RegData[1] = (reg_addr >> 8) & 0xff;
    TX_RegData[2] = reg_data & 0xff;  //1st byte of data

    switch (datalen) {
        case 1:  //1 byte datalength
            I2C_WriteReg(0x3E, TX_RegData, 3);
            delayUS(2000);
            TX_Buffer[0] = Checksum(TX_RegData, 3);
            TX_Buffer[1] =
                0x05;  //combined length of register address and data
            I2C_WriteReg(0x60, TX_Buffer, 2);  // Write the checksum and length
            delayUS(2000);
            break;
        case 2:  //2 byte datalength
            TX_RegData[3] = (reg_data >> 8) & 0xff;
            I2C_WriteReg(0x3E, TX_RegData, 4);
            delayUS(2000);
            TX_Buffer[0] = Checksum(TX_RegData, 4);
            TX_Buffer[1] =
                0x06;  //combined length of register address and data
            I2C_WriteReg(0x60, TX_Buffer, 2);  // Write the checksum and length
            delayUS(2000);
            break;
        case 4:  //4 byte datalength, Only used for CCGain and Capacity Gain
            TX_RegData[3] = (reg_data >> 8) & 0xff;
            TX_RegData[4] = (reg_data >> 16) & 0xff;
            TX_RegData[5] = (reg_data >> 24) & 0xff;
            I2C_WriteReg(0x3E, TX_RegData, 6);
            delayUS(2000);
            TX_Buffer[0] = Checksum(TX_RegData, 6);
            TX_Buffer[1] =
                0x08;  //combined length of register address and data
            I2C_WriteReg(0x60, TX_Buffer, 2);  // Write the checksum and length
            delayUS(2000);
            break;
    }
}

//***********************************BQ769X2 Functions*********************************

// ********************************* BQ769x2 Status and Fault Commands   *****************************************
void BQ769x2_ReadAlarmStatus()
{
    // Read this register to find out why the ALERT pin was asserted
    DirectCommands(AlarmStatus, 0x00, R);
    AlarmBits = (uint16_t) RX_data[1] * 256 + (uint16_t) RX_data[0];
}

void BQ769x2_ReadFULLSCAN()
{
    I2C_TARGET_ADDRESS = BOTAFE_I2C_ADDR;
    BQ769x2_ReadAlarmStatus();
    //    BOT_ADCSCAN =((0x02 & RX_data[0]) >> 1);//bit2
    BOT_FULLSCAN = ((0x80 & RX_data[0]) >> 7);  //bit8

    I2C_TARGET_ADDRESS = TOPAFE_I2C_ADDR;
    BQ769x2_ReadAlarmStatus();
    //    TOP_ADCSCAN =((0x02 & RX_data[0]) >> 1);//bit2
    TOP_FULLSCAN = ((0x80 & RX_data[0]) >> 7);  //bit8
}

uint16_t BQ769x2_ReadSafetyStatus()
{
    // Read Safety Status A/B/C and find which bits are set
    // This shows which primary protections have been triggered
    DirectCommands(SafetyStatusA, 0x00, R);
    value_SafetyStatusA = RX_data[0];
    //Example Fault Flags
    UV_Fault   = ((0x04 & RX_data[0]) >> 2);
    OV_Fault   = ((0x08 & RX_data[0]) >> 3);
    SCD_Fault  = ((0x80 & RX_data[0]) >> 7);
    OCD2_Fault = ((0x40 & RX_data[0]) >> 6);
    DirectCommands(SafetyStatusB, 0x00, R);
    value_SafetyStatusB = RX_data[0];
    DirectCommands(SafetyStatusC, 0x00, R);
    value_SafetyStatusC = RX_data[0];

    if ((value_SafetyStatusA + value_SafetyStatusB + value_SafetyStatusC) >
        1) {
        ProtectionsTriggered = 1;
    } else {
        ProtectionsTriggered = 0;
    }
    return ProtectionsTriggered;
}

void BQ769x2_ReadPFStatus()
{
    // Read Permanent Fail Status A/B/C and find which bits are set
    // This shows which permanent failures have been triggered
    DirectCommands(PFStatusA, 0x00, R);
    value_PFStatusA = ((RX_data[1] << 8) + RX_data[0]);
    DirectCommands(PFStatusB, 0x00, R);
    value_PFStatusB = ((RX_data[1] << 8) + RX_data[0]);
    DirectCommands(PFStatusC, 0x00, R);
    value_PFStatusC = ((RX_data[1] << 8) + RX_data[0]);
}

uint16_t BQ769x2_ReadBatteryStatus()
{
    DirectCommands(BatteryStatus, 0x00, R);
    return Battery_status = (RX_data[1] << 8) + RX_data[0];
}

void BQ769x2_ReadFETStatus()
{
    DirectCommands(FETStatus, 0x00, R);
    FET_Status = (RX_data[1] << 8) + RX_data[0];
}

void BQ769x2_ReadMANUFACTURINGStatus()
{
    Subcommands(MANUFACTURINGSTATUS, 0x00, R);
    FET_EN = ((0x10 & RX_32Byte[0]) >> 4);
}

void BQ769x2_ReadOtpCondition()
{
    Subcommands(OTP_WR_CHECK, 0x00, R);
    OTP_condition = RX_32Byte[0];
}

void BQ769x2_OtpWriteCheck()
{
    DirectCommands(0x40, 0x00, R);
    OTP_write_check = RX_data[0];
}

void BQ769x2_ReadAllTemperature_Count()
{
    //Thermistors pins are measured in the sequence of CFETOFF, DFETOFF, ALERT, TS1, TS2, TS3, HDQ, DCHG, DDSG
    Subcommands(DASTATUS6, 0x00, R);
    CFETOFF_Count = (RX_32Byte[15] << 24) + (RX_32Byte[14] << 16) +
                    (RX_32Byte[13] << 8) + RX_32Byte[12];
    DFETOFF_Count = (RX_32Byte[19] << 24) + (RX_32Byte[18] << 16) +
                    (RX_32Byte[17] << 8) + RX_32Byte[16];
    ALERT_Count = (RX_32Byte[23] << 24) + (RX_32Byte[22] << 16) +
                  (RX_32Byte[21] << 8) + RX_32Byte[20];
    TS1_Count = (RX_32Byte[27] << 24) + (RX_32Byte[26] << 16) +
                (RX_32Byte[25] << 8) + RX_32Byte[24];
    TS2_Count = (RX_32Byte[31] << 24) + (RX_32Byte[30] << 16) +
                (RX_32Byte[29] << 8) + RX_32Byte[28];

    Subcommands(DASTATUS7, 0x00, R);

    TS3_Count = (RX_32Byte[3] << 24) + (RX_32Byte[2] << 16) +
                (RX_32Byte[1] << 8) + RX_32Byte[0];
    HDQ_Count = (RX_32Byte[7] << 24) + (RX_32Byte[6] << 16) +
                (RX_32Byte[5] << 8) + RX_32Byte[4];
    DCHG_Count = (RX_32Byte[11] << 24) + (RX_32Byte[10] << 16) +
                 (RX_32Byte[9] << 8) + RX_32Byte[8];
    DDSG_Count = (RX_32Byte[15] << 24) + (RX_32Byte[14] << 16) +
                 (RX_32Byte[13] << 8) + RX_32Byte[12];
}

uint16_t pad_res     = 0x00;
uint16_t pullup_res1 = 0x00;
uint16_t pullup_res2 = 0x00;

void BQ769x2_Readpullup_pad_RES()
{
    Subcommands(0x9063, 0x00, R);

    pad_res     = (RX_32Byte[1] << 8) + RX_32Byte[0];
    pullup_res1 = (RX_32Byte[3] << 8) + RX_32Byte[2];
    pullup_res2 = (RX_32Byte[5] << 8) + RX_32Byte[4];
}

void BQ769x2_Reset()
{
    //Partial Reset

    //Full Reset
}

// ********************************* BQ769x2 Measurement Commands   *****************************************

uint16_t BQ769x2_ReadVoltage(uint8_t command)
// This function can be used to read a specific cell voltage or stack / pack / LD voltage
{
    //RX_data is global var
    DirectCommands(command, 0x00, R);
    if (command >= Cell1Voltage &&
        command <= Cell16Voltage) {  //Cells 1 through 16 (0x14 to 0x32)
        return (RX_data[1] * 256 + RX_data[0]);  //voltage is reported in mV
    } else {                                     //stack, Pack, LD
        return 10 * (RX_data[1] * 256 +
                        RX_data[0]);  //voltage is reported in 0.01V units
    }
}

void BQ769x2_ReadCurrent()
// Reads PACK current
{
    //    DirectCommands(CC2Current, 0x00, R); //read CC2 current
    //    Pack_Current =
    //        (int16_t)((uint16_t) RX_data[1] * 256 +
    //                  (uint16_t) RX_data[0]);  // current is reported in mA

    Subcommands(DASTATUS5, 0x00, R);
    Pack_Current =
        (int16_t)((uint16_t) RX_32Byte[21] * 256 +
                  (uint16_t) RX_32Byte[20]);  // current is reported in mA
}

float BQ769x2_ReadTemperature(uint8_t command)
{
    DirectCommands(command, 0x00, R);
    //RX_data is a global var
    return (0.1 * (float) (RX_data[1] * 256 + RX_data[0])) -
           273.15;  // converts from 0.1K to Celcius
}

void BQ769x2_ReadPassQ()
{  // Read Accumulated Charge and Time from DASTATUS6
    Subcommands(DASTATUS6, 0x00, R);
    AccumulatedCharge_Int  = ((RX_32Byte[3] << 24) + (RX_32Byte[2] << 16) +
                             (RX_32Byte[1] << 8) + RX_32Byte[0]);  //Bytes 0-3
    AccumulatedCharge_Frac = ((RX_32Byte[7] << 24) + (RX_32Byte[6] << 16) +
                              (RX_32Byte[5] << 8) + RX_32Byte[4]);  //Bytes 4-7
    AccumulatedCharge_Time =
        ((RX_32Byte[11] << 24) + (RX_32Byte[10] << 16) + (RX_32Byte[9] << 8) +
            RX_32Byte[8]);  //Bytes 8-11
}

void BQ769x2_EnableAllFETs()  // Enable FET and Trun on all FET
{
    CommandSubcommands(FET_ENABLE);  // Need check if FET_EN=1
    delayUS(500);
    CommandSubcommands(ALL_FETS_ON);
    delayUS(500);
}
//************************************End of BQ769x2 Measurement Commands******************************************

/******************* BOT AFE OTP Programming**************************/
/* OTP BOT AFE I2C Address to 0x20 -- 8 bit format
 * Set Blue LED to indicate OTP Status
 * Apply 10-12V to BAT at OTP,only OTP BOT AFE, ensure no I2C signal sent to TOP AFE
 */
void BQ769x2_OTP_Programming()
{
    I2C_TARGET_ADDRESS = AFE_Default_I2C_ADDR;
    DL_GPIO_setPins(LED_PORT, LED_LED1_Blue_PIN);

    BQ769x2_ReadBatteryStatus();  //Read the 0x12 Battery Status[SEC1,SEC0] bits to verify the device is in FULL ACCESS mode (0x01).

    if ((Battery_status & 0x0100) ==
        0x0100)  // Check if device is in FULL ACCESS mode
    {
        FULLACCESS = 1;
        CommandSubcommands(
            SET_CFGUPDATE);  // Enter CONFIGUPDATE mode (Subcommand 0x0090) - It is required to be in CONFIG_UPDATE mode to program the device RAM settings
        while (1) {
            BQ769x2_ReadBatteryStatus();
            if (Battery_status & 0x0001) break;
            //               delay_cycles(2400);
        }

        BQ769x2_SetRegister(I2CAddress, 0x20,
            1);  // set bottom AFE I2C address to 0x20(W), 0x21(R)
        BQ769x2_SetRegister(REG0Config, 0x01, 1);  // Enable REG0
        BQ769x2_SetRegister(REG12Config, 0x0D,
            1);  // Enable REG1 with 3.3V output (0x0D for 3.3V)

        CommandSubcommands(
            EXIT_CFGUPDATE);  // Exit CONFIGUPDATE mode  - Subcommand 0x0092
        while (1) {
            BQ769x2_ReadBatteryStatus();
            if (!(Battery_status & 0x0001)) break;
            //               delay_cycles(2400);
        }
        // User can read the data memory registers to verify all parameters were written successfully to double check the RAM settings.
        /* OTP Check */
        CommandSubcommands(SET_CFGUPDATE);
        while (1) {
            BQ769x2_ReadBatteryStatus();
            if (Battery_status & 0x0001) break;
            //               delay_cycles(2400);
        }

        if (!((Battery_status >> 7) &
                1))  // Check the Battery Status[OTPB] bit is clear to verify OTP programming conditions are met.
        {
            BQ769x2_ReadOtpCondition();
            if ((OTP_condition >> 7) & 1) {
                CommandSubcommands(OTP_WRITE);
                delayUS(50000);
                while (1)  // Wait write OTP
                {
                    delayUS(50000);
                    BQ769x2_OtpWriteCheck();
                    if (OTP_write_check & 0x80) {
                        OTP_write_success_flag = 1;
                        break;
                    }
                }
            }
        }

        CommandSubcommands(EXIT_CFGUPDATE);
        while (1) {
            BQ769x2_ReadBatteryStatus();
            if (!(Battery_status & 0x0001)) break;
            //               delay_cycles(2400);
        }
    }

    else {
        FULLACCESS = 0;
        while (1)
            ;
    }

    delayUS(60000);
    DL_GPIO_clearPins(LED_PORT,
        LED_LED1_Blue_PIN);  //Blue LED blinking indicate OTP successfully
    delayUS(60000);
    delayUS(60000);
    delayUS(60000);
    DL_GPIO_setPins(LED_PORT, LED_LED1_Blue_PIN);
    delayUS(60000);
    delayUS(60000);
    delayUS(60000);
    DL_GPIO_clearPins(LED_PORT, LED_LED1_Blue_PIN);
    delayUS(60000);
    delayUS(60000);
    delayUS(60000);
    DL_GPIO_setPins(LED_PORT, LED_LED1_Blue_PIN);
    delayUS(60000);
    delayUS(60000);
    delayUS(60000);
    DL_GPIO_clearPins(LED_PORT, LED_LED1_Blue_PIN);
    delayUS(60000);
    delayUS(60000);
    delayUS(60000);
    DL_GPIO_setPins(LED_PORT, LED_LED1_Blue_PIN);
}

//***************************BQ769x2 Current,Voltage,Temperature Calibration*****************************************************

uint16_t BQ769x2_Current_BoardOffset_Calibration()
{
    //board offset calibration, ensure no current is flowing through the sense resistor
#if Current_0mA

    CommandSubcommands(SLEEP_DISABLE);
    int32_t sum = 0;

    delayUS(50000);
    delayUS(50000);
    for (int i = 0; i < 10; i++) {
        Subcommands(READ_CAL1, 0x00, R);
        CC2_Counts_Raw[i] =
            (RX_32Byte[4] << 8) + RX_32Byte[3];  //middle 16bit value
        sum += CC2_Counts_Raw[i];
    }

    CC2_Counts_Average = sum / 10;
    Subcommands(CoulombCounterOffsetSamples, 0x00, R);
    CC_Offset_Samples = (RX_32Byte[1] << 8) + RX_32Byte[0];
    Board_Offset      = CC2_Counts_Average * CC_Offset_Samples;  // got 0 @0mA

    //Re-check the CC2 Counts reading. If the reading is not close to zero, repeat above steps.

#endif

    return CC2_Counts_Calibrated;
}

void BQ769x2_Current_Gain_Calibration()
{
    /* @brief Below is further calibration
 * @note Disable Sleep Mode (Subcommand 0x009A) to ensure voltage counts update quickly
 */
    CommandSubcommands(SLEEP_DISABLE);
    int32_t sum = 0;

#if Current_3A

    delayUS(50000);
    delayUS(50000);

    for (int i = 0; i < 10; i++) {
        Subcommands(READ_CAL1, 0x00, R);
        CC2_Counts_Raw[i] = (RX_32Byte[4] << 8) + RX_32Byte[3];
        sum += CC2_Counts_Raw[i];
    }
    CC2_Counts_A = sum / 10;

#endif

//CC Gain calibration, ensure apply 6A to the sense resistor
#if Current_6A

    delayUS(50000);
    delayUS(50000);

    for (int i = 0; i < 10; i++) {
        Subcommands(READ_CAL1, 0x00, R);
        CC2_Counts_Raw[i] = (RX_32Byte[4] << 8) + RX_32Byte[3];
        sum += CC2_Counts_Raw[i];
        delayUS(50000);
        delayUS(50000);
    }
    CC2_Counts_B = sum / 10;
#endif

#if CCA_CCB_Ready

    CC2_Counts_A = 0xFF89;  //Type in the CCA value tested in 3A
    CC2_Counts_B = 0xFF11;  //Type in the CCA value tested in 6A
    CC_Gain =
        (-6000 + 3000) / (float) (CC2_Counts_B - CC2_Counts_A);  //got 25.0

#endif
}

//***************************BQ769x2 initiation*****************************************************

/* Configures all parameters in device RAM
 * Separate bottom and top AFE initiation function in case of different settings
 * @brief After entering CONFIG_UPDATE mode, RAM registers can be programmed. When programming RAM, checksum and length must also be
 * programmed for the change to take effect. All of the RAM registers are described in detail in the BQ769x2 TRM.
 * An easier way to find the descriptions is in the BQStudio Data Memory screen. When you move the mouse over the register name,
 * a full description of the register and the bits will pop up on the screen.
 */
void BQ769x2_BOT_Init()
{
    I2C_TARGET_ADDRESS = BOTAFE_I2C_ADDR;
    // Enter CONFIGUPDATE mode (Subcommand 0x0090) - It is required to be in CONFIG_UPDATE mode to program the device RAM settings
    // See TRM for full description of CONFIG_UPDATE mode
    DL_GPIO_setPins(WAKE_BOT_AFE_TS2_PORT, WAKE_BOT_AFE_TS2_PIN);
    delayUS(60000);
    delayUS(60000);
    delayUS(60000);
    delayUS(60000);
    delayUS(60000);

    CommandSubcommands(SET_CFGUPDATE);
    delayUS(2000);
    while (1) {
        BOTBatteryStatus = BQ769x2_ReadBatteryStatus();
        if (BOTBatteryStatus & 0x0001) break;
        CommandSubcommands(SET_CFGUPDATE);
        delayUS(2000);
    }
    /* Calibration
 * @brief Current can simply be adjusted based on the sense resistor value in the system (CC Gain = 7.4768 / (Rsense in mOhm)).
 * If further calibration is required, need to calibrate board offset and CC gain based on APP NOTE SLUAA32A
 */
    //Simple Current Calibration; CC Gain = 7.4768/Rcs(0.300)=24.9227= 0x41C76BEE -> 32bit IEEE-754 format
    // CC_Gain_Calibrated=25=0x41C80000
    BQ769x2_SetRegister(CCGain, 0x41C80000, 4);
    // Capacity Gain = CC Gain_calibrated x 298261.6178 = 7,433,474.88= 0x4AE2DA06
    // Capacity Gain = CC Gain_calibrated x 298261.6178 = 7,456,540.445=0x4AE38E39
    BQ769x2_SetRegister(CapacityGain, 0x4AE38E39, 4);

    BQ769x2_SetRegister(BoardOffset, 0x00, 1);  //0x91C8 default 0

#if !TMP_MUX_Enabled
    //bestA [A1 A2 A3 A4 A5] =  [-11279 -16652 -22025 -11282 272]
    //bestB [B1 B2 B3 B4] =  [ 7956  22863  25932  13143]
    //Adc0 = 11703
    BQ769x2_SetRegister(T18kCoeffa1, 0xD3F1, 2);
    BQ769x2_SetRegister(T18kCoeffa2, 0xBEF4, 2);
    BQ769x2_SetRegister(T18kCoeffa3, 0xA9F7, 2);
    BQ769x2_SetRegister(T18kCoeffa4, 0xD3EE, 2);
    BQ769x2_SetRegister(T18kCoeffa5, 0x0110, 2);

    BQ769x2_SetRegister(T18kCoeffb1, 0x1F14, 2);
    BQ769x2_SetRegister(T18kCoeffb2, 0x594F, 2);
    BQ769x2_SetRegister(T18kCoeffb3, 0x654C, 2);
    BQ769x2_SetRegister(T18kCoeffb4, 0x3357, 2);
    BQ769x2_SetRegister(T18kAdc0, 0x2DB7, 2);
#endif

    /*
 *  Settings->Configurations
 */
    // 'Power Config' - 0x9234 = 0x2DC4 = FASTADC=1, Loop_slow=1
    // 0x2D84=FASTADC=0,Loop_slow=1 ;  0x2DC0 = FASTADC=1, Loop_slow=0
    // Setting the DSLP_LDO bit allows the LDOs to remain active when the device goes into Deep Sleep mode
    // Set wake speed bits to 00 for best performance
    BQ769x2_SetRegister(PowerConfig, 0x2DC0, 2);
    //     'REG0 Config' - set REG0_EN bit to enable pre-regulator
    BQ769x2_SetRegister(REG0Config, 0x01, 1);
    //     'REG12 Config' - Enable REG1 with 3.3V output, REG2 with 2.5V output 0X9D
    BQ769x2_SetRegister(REG12Config, 0x9D, 1);
    //     'Comm Type' - 0x9239 = 0x08(fast I2C mode)
    BQ769x2_SetRegister(CommType, 0x08, 1);
    //     'SleepCurrent' - 0x9248 = 0x64(100mA)
    BQ769x2_SetRegister(SleepCurrent, 0x64, 1);

#if TMP_MUX_Enabled

    BQ769x2_SetRegister(DFETOFFPinConfig, 0x3B, 1);
    BQ769x2_SetRegister(DCHGPinConfig, 0x3B, 1);
    BQ769x2_SetRegister(DDSGPinConfig, 0x3B, 1);
    BQ769x2_SetRegister(TS2Config, 0x3B, 1);

#else
    // Set DFETOFF pin to control BOTH CHG and DSG FET - 0x92FB = 0x46, active high (set to 0x00 to disable)
    BQ769x2_SetRegister(
        DFETOFFPinConfig, 0x46, 1);  // must enable weak pulldown
    // Set DCHG to DCHG - 0x9301 = 0xAA, active low
    BQ769x2_SetRegister(DCHGPinConfig, 0xAA, 1);
    // Set DDSG to DDSG - 0x9302 = 0x2A, active high
    BQ769x2_SetRegister(DDSGPinConfig, 0x2A, 1);
#endif
    // Set CFETOFF pin to measure Cell Temperature - 0x92FA = 0x07 (set to 0x00 to disable)
    BQ769x2_SetRegister(CFETOFFPinConfig, 0x3B, 1);
    // Set ALERT pin to 0x07 to measure Cell Temperature
    BQ769x2_SetRegister(ALERTPinConfig, 0x3B, 1);
    // Set TS1 to measure Cell Temperature - 0x92FD
    BQ769x2_SetRegister(TS1Config, 0x3B, 1);
    // Set TS3 to measure Cell Temperature - 0x92FF
    BQ769x2_SetRegister(TS3Config, 0x3B, 1);
    // Set HDQ to measure Cell Temperature - 0x9300
    BQ769x2_SetRegister(HDQPinConfig, 0x3B, 1);

    // 'VCell Mode' - Enable 16 cells - 0x9304 = 0xFFFF;
    BQ769x2_SetRegister(VCellMode, 0xFFFF, 2);
    // Set USER_AMPs to 10mA
    BQ769x2_SetRegister(DAConfiguration, 0x06, 1);

    /*
 *  Settings->Protections
 */
    // Enable protections in 'Enabled Protections A' 0x9261 = 0xBC, Enables SCD,OCD1,OCC,COV,CUV
    BQ769x2_SetRegister(EnabledProtectionsA, 0xBC, 1);
    // Enable all protections in 'Enabled Protections B' 0x9262 = 0xF7
    // Enables OTF, OTINT, OTD , OTC , UTINT, UTD , UTC
    BQ769x2_SetRegister(EnabledProtectionsB, 0xF7, 1);

    /*
 *  Settings->Alarm
 */
    // 'Default Alarm Mask' - 0x..82 Enables the FullScan and ADScan bits, default value = 0xF800
    BQ769x2_SetRegister(DefaultAlarmMask, 0xF882, 2);

    /*
 *  Settings->FET
 */
    // Set FET_INIT_OFF bit
    BQ769x2_SetRegister(FETOptions, 0x2F, 1);
    // Disable BOTAFE charge pump
    BQ769x2_SetRegister(ChgPumpControl, 0x00, 1);

    /*
 *  Settings->Cell Balancing Config
 */
    // Set up Cell Balancing Configuration - 0x9335 = 0x03   -  Automated balancing while in Relax or Charge modes
    // Also see "Cell Balancing with BQ769x2 Battery Monitors" document on ti.com
    BQ769x2_SetRegister(BalancingConfiguration, 0x00,
        1);  //Kian: Set to 0x00 to disable cell balancing
    // Set the minimum cell balance voltage in charge - 0x933B = 3300mv
    BQ769x2_SetRegister(CellBalanceMinCellVCharge, 0x0CE4, 2);
    // Set the minimum cell balance voltage in rest - 0x933F = 3300mv
    BQ769x2_SetRegister(CellBalanceMinCellVRelax, 0x0CE4, 2);

    /*
 * Protections
 */
    // Set up CUV (under-voltage) Threshold - 0x9275 = 0x32 (2530mV)
    // CUV Threshold is this value multiplied by 50.6mV
    BQ769x2_SetRegister(CUVThreshold, 0x14, 1);  //set to 0x14=1012mv for tests
    // Set up COV (over-voltage) Threshold - 0x9278 = 0x48 (3642mV)
    // COV Threshold is this value multiplied by 50.6mV
    BQ769x2_SetRegister(COVThreshold, 0x50, 1);
    // Set up OCC (over-current in charge) Threshold - 0x9280 = 0x06(12mv = 40A across 0.3mOhm sense resistor) Units in 2mV
    BQ769x2_SetRegister(OCCThreshold, 0x6, 1);
    // Set up OCD1 (over-current in discharge) Threshold - 0x9282 = -10(32 mV = 106.6A across 0.3mOhm sense resistor) units of 2mV
    BQ769x2_SetRegister(OCD1Threshold, 0x10, 1);
    // Set up OCD1 (over-current in discharge) Threshold - 0x9282 = -14(40 mV = 133.3A across 0.3mOhm sense resistor) units of 2mV
    BQ769x2_SetRegister(OCD2Threshold, 0x14, 1);
    // Set up SCD Threshold - 0x9286 = 0x04 (80 mV = 266.7A across 0.3mOhm sense resistor)
    BQ769x2_SetRegister(SCDThreshold, 0x04, 1);
    // Set up SCD Delay - 0x9287 = 0x03 (30us) Enabled with a delay of (value - 1) * 15 us; min value of 1
    BQ769x2_SetRegister(SCDDelay, 0x03, 1);
    // Set up SCDL Latch Limit to 1 to set SCD recovery only with load removal 0x9295 = 0x01
    // If this is not set, then SCD will recover based on time (SCD Recovery Time parameter).
    BQ769x2_SetRegister(SCDLLatchLimit, 0x01, 1);

    /*
 * Power -> Shutdown
 */
    // shutdown cell voltage 2000mV - 0x923F = 0x07D0;
    BQ769x2_SetRegister(
        ShutdownCellVoltage, 0x0258, 2);  // set to 600mv for tests
    // shutdown pack voltage 20000mV - 0x9241 = 0x07D0;
    BQ769x2_SetRegister(
        ShutdownStackVoltage, 0x0258, 2);  // set to 6000mv for tests
    // shutdown delay 6s - 0x9243 = 0x06;
    BQ769x2_SetRegister(LowVShutdownDelay, 0x06, 1);

    // Exit CONFIGUPDATE mode  - Subcommand 0x0092
    CommandSubcommands(EXIT_CFGUPDATE);
    delayUS(2000);

    while (1) {
        BOTBatteryStatus = BQ769x2_ReadBatteryStatus();
        if (!(BOTBatteryStatus & 0x0001)) break;
        CommandSubcommands(EXIT_CFGUPDATE);
        delayUS(2000);
    }

    DL_GPIO_clearPins(WAKE_BOT_AFE_TS2_PORT, WAKE_BOT_AFE_TS2_PIN);
    delayUS(8000);
    CommandSubcommands(FET_ENABLE);
    delayUS(8000);
    CommandSubcommands(ALL_FETS_ON);
    delayUS(8000);
    CommandSubcommands(SLEEP_DISABLE);
    delayUS(8000);
    delayUS(10000);
}

void BQ769x2_TOP_Init()
{
    I2C_TARGET_ADDRESS = TOPAFE_I2C_ADDR;
    DL_GPIO_setPins(WAKE_TOP_AFE_TS2_PORT, WAKE_TOP_AFE_TS2_PIN);
    delayUS(60000);
    delayUS(60000);
    delayUS(60000);
    delayUS(60000);
    delayUS(60000);

    // Same process as bottom AFE initiation, see annotation in BQ769x2_BOT_Init() function for more details
    CommandSubcommands(SET_CFGUPDATE);
    delayUS(2000);
    while (1) {
        TOPBatteryStatus = BQ769x2_ReadBatteryStatus();
        if (TOPBatteryStatus & 0x0001) break;
        CommandSubcommands(SET_CFGUPDATE);
        delayUS(2000);
    }
    //Calibrations: only need calibrate temperature and voltage

    //Settings:Configurations
    BQ769x2_SetRegister(PowerConfig, 0x2DC0,
        2);  // 'Power Config' - 0x9234 = 0x2DC4 = FASTADC=1, Loop_slow=1
    BQ769x2_SetRegister(REG0Config, 0x01, 1);
    BQ769x2_SetRegister(REG12Config, 0x9D, 1);

    BQ769x2_SetRegister(CommType, 0x08, 1);
    BQ769x2_SetRegister(SleepCurrent, 0x64, 1);

#if TMP_MUX_Enabled
    BQ769x2_SetRegister(CFETOFFPinConfig, 0x3B, 1);
    BQ769x2_SetRegister(DFETOFFPinConfig, 0x3B, 1);  // 0x42 BOTHOFF
    BQ769x2_SetRegister(TS2Config, 0x3B, 1);
#else
    BQ769x2_SetRegister(CFETOFFPinConfig, 0x82, 1);
    BQ769x2_SetRegister(DFETOFFPinConfig, 0x82, 1);  // 0x42 BOTHOFF
#endif

    BQ769x2_SetRegister(ALERTPinConfig, 0x3B, 1);
    BQ769x2_SetRegister(TS1Config, 0x3B, 1);
    BQ769x2_SetRegister(TS3Config, 0x3B, 1);
    BQ769x2_SetRegister(HDQPinConfig, 0x3B, 1);
    BQ769x2_SetRegister(DCHGPinConfig, 0x3B, 1);
    BQ769x2_SetRegister(DDSGPinConfig, 0x3B, 1);

#if !TMP_MUX_Enabled
    BQ769x2_SetRegister(T18kCoeffa1, 0xD3F1, 2);
    BQ769x2_SetRegister(T18kCoeffa2, 0xBEF4, 2);
    BQ769x2_SetRegister(T18kCoeffa3, 0xA9F7, 2);
    BQ769x2_SetRegister(T18kCoeffa4, 0xD3EE, 2);
    BQ769x2_SetRegister(T18kCoeffa5, 0x0110, 2);

    BQ769x2_SetRegister(T18kCoeffb1, 0x1F14, 2);
    BQ769x2_SetRegister(T18kCoeffb2, 0x594F, 2);
    BQ769x2_SetRegister(T18kCoeffb3, 0x654C, 2);
    BQ769x2_SetRegister(T18kCoeffb4, 0x3357, 2);
    BQ769x2_SetRegister(T18kAdc0, 0x2DB7, 2);
#endif

    //Settings:Protections
    BQ769x2_SetRegister(EnabledProtectionsA, 0xBC, 1);
    BQ769x2_SetRegister(EnabledProtectionsB, 0xF7, 1);
    //Settings:Alarm
    BQ769x2_SetRegister(DefaultAlarmMask, 0xF882, 2);
    //Settings->Cell Balancing Config
    BQ769x2_SetRegister(BalancingConfiguration, 0x00,
        1);  //Kian: Set to 0x00 to disable cell balancing
    BQ769x2_SetRegister(CellBalanceMinCellVCharge, 0x0CE4, 2);
    BQ769x2_SetRegister(CellBalanceMinCellVRelax, 0x0CE4, 2);
    //Settings->FET
    BQ769x2_SetRegister(FETOptions, 0x2F, 1);
    BQ769x2_SetRegister(ChgPumpControl, 0x01, 1);
    //Protections
    BQ769x2_SetRegister(CUVThreshold, 0x14, 1);  // 0x14=1012mv  default 0x32
    BQ769x2_SetRegister(COVThreshold, 0x50, 1);  //0x48
    BQ769x2_SetRegister(OCCThreshold, 0x6, 1);
    BQ769x2_SetRegister(OCD1Threshold, 0x10, 1);
    BQ769x2_SetRegister(OCD2Threshold, 0x14, 1);
    BQ769x2_SetRegister(SCDThreshold, 0x04, 1);
    BQ769x2_SetRegister(SCDDelay, 0x03, 1);
    BQ769x2_SetRegister(SCDLLatchLimit, 0x01, 1);

    //Power -> Shutdown
    BQ769x2_SetRegister(
        ShutdownCellVoltage, 0x0258, 2);  // set to 600mv for tests 0x7D0
    BQ769x2_SetRegister(
        ShutdownStackVoltage, 0x0258, 2);  // set to 6000mv for tests 0x7D0
    BQ769x2_SetRegister(LowVShutdownDelay, 0x06, 1);

    CommandSubcommands(EXIT_CFGUPDATE);
    delayUS(2000);

    while (1) {
        TOPBatteryStatus = BQ769x2_ReadBatteryStatus();
        if (!(TOPBatteryStatus & 0x0001)) break;
        CommandSubcommands(EXIT_CFGUPDATE);
        delayUS(2000);
    }

    DL_GPIO_clearPins(WAKE_TOP_AFE_TS2_PORT, WAKE_TOP_AFE_TS2_PIN);
    delayUS(8000);

    CommandSubcommands(FET_ENABLE);
    delayUS(8000);
    CommandSubcommands(ALL_FETS_ON);
    delayUS(8000);
    CommandSubcommands(SLEEP_DISABLE);
    delayUS(8000);
    delayUS(10000);
}
