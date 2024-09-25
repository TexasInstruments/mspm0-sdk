#include "ti_msp_dl_config.h"

typedef unsigned char byte;

// Pin mapping of BOOSTXL-PGA460 to LaunchPad by pin name
#define DECPL_A 2
#define RXD_LP 3
#define TXD_LP 4
#define DECPL_D 5
#define TEST_A 6
#define TCI_CLK 7
#define TEST_D 8
#define MEM_SOMI 9
#define MEM_SIMO 10
#define TCI_RX 14
#define TCI_TX 15
#define COM_SEL 17
#define COM_PD 18
#define SPI_CS 33
#define SCLK_CLK 34
#define MEM_HOLD 36
#define MEM_CS 37
#define DS1_LED 38
#define F_DIAG_LED 39
#define V_DIAG_LED 40

// Define UART commands by name
// Single Address
byte P1BL  = 0x00;
byte P2BL  = 0x01;
byte P1LO  = 0x02;
byte P2LO  = 0x03;
byte TNLM  = 0x04;
byte UMR   = 0x05;
byte TNLR  = 0x06;
byte TEDD  = 0x07;
byte SD    = 0x08;
byte SRR   = 0x09;
byte SRW   = 0x0A;
byte EEBR  = 0x0B;
byte EEBW  = 0x0C;
byte TVGBR = 0x0D;
byte TVGBW = 0x0E;
byte THRBR = 0x0F;
byte THRBW = 0x10;
//Broadcast
byte BC_P1BL  = 0x11;
byte BC_P2BL  = 0x12;
byte BC_P1LO  = 0x13;
byte BC_P2LO  = 0x14;
byte BC_TNLM  = 0x15;
byte BC_RW    = 0x16;
byte BC_EEBW  = 0x17;
byte BC_TVGBW = 0x18;
byte BC_THRBW = 0x19;
//CMDs 26-31 are reserved

// List user registers by name with default settings from TI factory
byte USER_DATA1   = 0x00;
byte USER_DATA2   = 0x00;
byte USER_DATA3   = 0x00;
byte USER_DATA4   = 0x00;
byte USER_DATA5   = 0x00;
byte USER_DATA6   = 0x00;
byte USER_DATA7   = 0x00;
byte USER_DATA8   = 0x00;
byte USER_DATA9   = 0x00;
byte USER_DATA10  = 0x00;
byte USER_DATA11  = 0x00;
byte USER_DATA12  = 0x00;
byte USER_DATA13  = 0x00;
byte USER_DATA14  = 0x00;
byte USER_DATA15  = 0x00;
byte USER_DATA16  = 0x00;
byte USER_DATA17  = 0x00;
byte USER_DATA18  = 0x00;
byte USER_DATA19  = 0x00;
byte USER_DATA20  = 0x00;
byte TVGAIN0      = 0xAF;
byte TVGAIN1      = 0xFF;
byte TVGAIN2      = 0xFF;
byte TVGAIN3      = 0x2D;
byte TVGAIN4      = 0x68;
byte TVGAIN5      = 0x36;
byte TVGAIN6      = 0xFC;
byte INIT_GAIN    = 0xC0;
byte FREQUENCY    = 0x8C;
byte DEADTIME     = 0x00;
byte PULSE_P1     = 0x01;
byte PULSE_P2     = 0x12;
byte CURR_LIM_P1  = 0x47;
byte CURR_LIM_P2  = 0xFF;
byte REC_LENGTH   = 0x1C;
byte FREQ_DIAG    = 0x00;
byte SAT_FDIAG_TH = 0xEE;
byte FVOLT_DEC    = 0x7C;
byte DECPL_TEMP   = 0x0A;
byte DSP_SCALE    = 0x00;
byte TEMP_TRIM    = 0x00;
byte P1_GAIN_CTRL = 0x00;
byte P2_GAIN_CTRL = 0x00;
byte EE_CRC       = 0xFF;
byte EE_CNTRL     = 0x00;
byte P1_THR_0     = 0x88;
byte P1_THR_1     = 0x88;
byte P1_THR_2     = 0x88;
byte P1_THR_3     = 0x88;
byte P1_THR_4     = 0x88;
byte P1_THR_5     = 0x88;
byte P1_THR_6     = 0x84;
byte P1_THR_7     = 0x21;
byte P1_THR_8     = 0x08;
byte P1_THR_9     = 0x42;
byte P1_THR_10    = 0x10;
byte P1_THR_11    = 0x80;
byte P1_THR_12    = 0x80;
byte P1_THR_13    = 0x80;
byte P1_THR_14    = 0x80;
byte P1_THR_15    = 0x80;
byte P2_THR_0     = 0x88;
byte P2_THR_1     = 0x88;
byte P2_THR_2     = 0x88;
byte P2_THR_3     = 0x88;
byte P2_THR_4     = 0x88;
byte P2_THR_5     = 0x88;
byte P2_THR_6     = 0x84;
byte P2_THR_7     = 0x21;
byte P2_THR_8     = 0x08;
byte P2_THR_9     = 0x42;
byte P2_THR_10    = 0x10;
byte P2_THR_11    = 0x80;
byte P2_THR_12    = 0x80;
byte P2_THR_13    = 0x80;
byte P2_THR_14    = 0x80;
byte P2_THR_15    = 0x80;

// Miscellaneous variables; (+) indicates OWU transmitted byte offset
byte checksum = 0x00;          // UART checksum value
byte ChecksumInput[44];        // data byte array for checksum calculator
byte ultraMeasResult[34 + 3];  // data byte array for cmd5 and tciB+L return
byte diagMeasResult[5 + 3];    // data byte array for cmd8 and index1 return
byte tempNoiseMeasResult[4 +
                         3];  // data byte array for cmd6 and index0&1 return
byte echoDataDump[130 + 3];   // data byte array for cmd7 and index12 return
byte tempOrNoise =
    0;  // data byte to determine if temp or noise measurement is to be performed
byte comm = 0;  // indicates UART (0), TCI (1), OWU (2) communication mode
unsigned long starttime;  // used for function time out
byte bulkThr[34 + 3];     // data byte array for bulk threhsold commands
                          //UART & OWU exclusive variables
byte syncByte = 0x55;  // data byte for Sync field set UART baud rate of PGA460
byte regAddr  = 0x00;  // data byte for Register Address
byte regData  = 0x00;  // data byte for Register Data
byte uartAddr =
    0;  // PGA460 UART device address (0-7). '0' is factory default address
byte numObj = 1;  // number of objects to detect
                  //OWU exclusive variables
signed int owuShift =
    0;  // accoutns for OWU receiver buffer offset for capturing master transmitted data - always 0 for standard two-wire UART
        //SPI exclusive variables
byte misoBuf[131];  // SPI MISO receive data buffer for all commands

/*------------------------------------------------- calcChecksum -----
     |  Function calcChecksum
     |
     |  Purpose:  Calculates the SPI checksum value based on the selected command and the user EERPOM values associated with the command
     |
     |  Parameters:
     |      cmd (IN) -- the UART command for which the checksum should be calculated for
     |
     |  Returns: byte representation of calculated checksum value
     *-------------------------------------------------------------------*/
byte calcChecksum(byte cmd)
{
    int checksumLoops = 0;

    cmd =
        cmd &
        0x001F;  // zero-mask command address of cmd to select correct switch-case statement

    switch (cmd) {
        case 0:   //P1BL
        case 1:   //P2BL
        case 2:   //P1LO
        case 3:   //P2LO
        case 17:  //BC_P1BL
        case 18:  //BC_P2BL
        case 19:  //BC_P1LO
        case 20:  //BC_P2LO
            ChecksumInput[0] = cmd;
            ChecksumInput[1] = numObj;
            checksumLoops    = 2;
            break;
        case 4:   //TNLM
        case 21:  //TNLM
            ChecksumInput[0] = cmd;
            ChecksumInput[1] = tempOrNoise;
            checksumLoops    = 2;
            break;
        case 5:   //UMR
        case 6:   //TNLR
        case 7:   //TEDD
        case 8:   //SD
        case 11:  //EEBR
        case 13:  //TVGBR
        case 15:  //THRBR
            ChecksumInput[0] = cmd;
            checksumLoops    = 1;
            break;
        case 9:  //RR
            ChecksumInput[0] = cmd;
            ChecksumInput[1] = regAddr;
            checksumLoops    = 2;
            break;
        case 10:  //RW
        case 22:  //BC_RW
            ChecksumInput[0] = cmd;
            ChecksumInput[1] = regAddr;
            ChecksumInput[2] = regData;
            checksumLoops    = 3;
            break;
        case 14:  //TVGBW
        case 24:  //BC_TVGBW
            ChecksumInput[0] = cmd;
            ChecksumInput[1] = TVGAIN0;
            ChecksumInput[2] = TVGAIN1;
            ChecksumInput[3] = TVGAIN2;
            ChecksumInput[4] = TVGAIN3;
            ChecksumInput[5] = TVGAIN4;
            ChecksumInput[6] = TVGAIN5;
            ChecksumInput[7] = TVGAIN6;
            checksumLoops    = 8;
            break;
        case 16:  //THRBW
        case 25:  //BC_THRBW
            ChecksumInput[0]  = cmd;
            ChecksumInput[1]  = P1_THR_0;
            ChecksumInput[2]  = P1_THR_1;
            ChecksumInput[3]  = P1_THR_2;
            ChecksumInput[4]  = P1_THR_3;
            ChecksumInput[5]  = P1_THR_4;
            ChecksumInput[6]  = P1_THR_5;
            ChecksumInput[7]  = P1_THR_6;
            ChecksumInput[8]  = P1_THR_7;
            ChecksumInput[9]  = P1_THR_8;
            ChecksumInput[10] = P1_THR_9;
            ChecksumInput[11] = P1_THR_10;
            ChecksumInput[12] = P1_THR_11;
            ChecksumInput[13] = P1_THR_12;
            ChecksumInput[14] = P1_THR_13;
            ChecksumInput[15] = P1_THR_14;
            ChecksumInput[16] = P1_THR_15;
            ChecksumInput[17] = P2_THR_0;
            ChecksumInput[18] = P2_THR_1;
            ChecksumInput[19] = P2_THR_2;
            ChecksumInput[20] = P2_THR_3;
            ChecksumInput[21] = P2_THR_4;
            ChecksumInput[22] = P2_THR_5;
            ChecksumInput[23] = P2_THR_6;
            ChecksumInput[24] = P2_THR_7;
            ChecksumInput[25] = P2_THR_8;
            ChecksumInput[26] = P2_THR_9;
            ChecksumInput[27] = P2_THR_10;
            ChecksumInput[28] = P2_THR_11;
            ChecksumInput[29] = P2_THR_12;
            ChecksumInput[30] = P2_THR_13;
            ChecksumInput[31] = P2_THR_14;
            ChecksumInput[32] = P2_THR_15;
            checksumLoops     = 33;
            break;
        case 12:  //EEBW
        case 23:  //BC_EEBW
            ChecksumInput[0]  = cmd;
            ChecksumInput[1]  = USER_DATA1;
            ChecksumInput[2]  = USER_DATA2;
            ChecksumInput[3]  = USER_DATA3;
            ChecksumInput[4]  = USER_DATA4;
            ChecksumInput[5]  = USER_DATA5;
            ChecksumInput[6]  = USER_DATA6;
            ChecksumInput[7]  = USER_DATA7;
            ChecksumInput[8]  = USER_DATA8;
            ChecksumInput[9]  = USER_DATA9;
            ChecksumInput[10] = USER_DATA10;
            ChecksumInput[11] = USER_DATA11;
            ChecksumInput[12] = USER_DATA12;
            ChecksumInput[13] = USER_DATA13;
            ChecksumInput[14] = USER_DATA14;
            ChecksumInput[15] = USER_DATA15;
            ChecksumInput[16] = USER_DATA16;
            ChecksumInput[17] = USER_DATA17;
            ChecksumInput[18] = USER_DATA18;
            ChecksumInput[19] = USER_DATA19;
            ChecksumInput[20] = USER_DATA20;
            ChecksumInput[21] = TVGAIN0;
            ChecksumInput[22] = TVGAIN1;
            ChecksumInput[23] = TVGAIN2;
            ChecksumInput[24] = TVGAIN3;
            ChecksumInput[25] = TVGAIN4;
            ChecksumInput[26] = TVGAIN5;
            ChecksumInput[27] = TVGAIN6;
            ChecksumInput[28] = INIT_GAIN;
            ChecksumInput[29] = FREQUENCY;
            ChecksumInput[30] = DEADTIME;
            ChecksumInput[31] = PULSE_P1;
            ChecksumInput[32] = PULSE_P2;
            ChecksumInput[33] = CURR_LIM_P1;
            ChecksumInput[34] = CURR_LIM_P2;
            ChecksumInput[35] = REC_LENGTH;
            ChecksumInput[36] = FREQ_DIAG;
            ChecksumInput[37] = SAT_FDIAG_TH;
            ChecksumInput[38] = FVOLT_DEC;
            ChecksumInput[39] = DECPL_TEMP;
            ChecksumInput[40] = DSP_SCALE;
            ChecksumInput[41] = TEMP_TRIM;
            ChecksumInput[42] = P1_GAIN_CTRL;
            ChecksumInput[43] = P2_GAIN_CTRL;
            checksumLoops     = 44;
            break;
        default:
            break;
    }

    if (ChecksumInput[0] <
        17)  //only re-append command address for non-broadcast commands.
    {
        ChecksumInput[0] = ChecksumInput[0] + (uartAddr << 5);
    }

    uint16_t carry = 0;

    for (int i = 0; i < checksumLoops; i++) {
        if ((ChecksumInput[i] + carry) < carry) {
            carry = carry + ChecksumInput[i] + 1;
        } else {
            carry = carry + ChecksumInput[i];
        }

        if (carry > 0xFF) {
            carry = carry - 255;
        }
    }

    carry = (~carry & 0x00FF);
    return carry;
}

/*------------------------------------------------- spiSendData -----
     |  Function receive data
     |
     |  Purpose: receive data
     |
     |  Parameters:
     |      size (IN) -- number of transmitting data
     |      arr[] (IN) -- data
     |  Returns:  none
     *-------------------------------------------------------------------*/
void spiSendData(byte arr[], byte size)
{
    for (int i = 0; i < size; i++) {
        DL_SPI_transmitData8(SPI_0_INST, arr[i]);
        delay_cycles(500);
    }
    return;
}

/*------------------------------------------------- spiMosiIdle -----
      |  Function receive data
      |
      |  Purpose: receive data
      |
      |  Parameters:
      |      size (IN) -- number of receiving data
      |  Returns:  none
      *-------------------------------------------------------------------*/
void spiMosiIdle(byte size)
{
    //clear FIFO
    DL_SPI_receiveData8(SPI_0_INST);
    DL_SPI_receiveData8(SPI_0_INST);
    DL_SPI_receiveData8(SPI_0_INST);
    DL_SPI_receiveData8(SPI_0_INST);
    //transmit 0xFE for opening CLK to receive DATA
    for (int i = 0; i < size; i++) {
        DL_SPI_transmitData8(SPI_0_INST, 0xFE);
        delay_cycles(500);
        misoBuf[i] = DL_SPI_receiveData8(SPI_0_INST);
    }
    return;
}

/*------------------------------------------------- registerWrite -----
     |  Function registerWrite
     |
     |  Purpose:  Write single register data to PGA460
     |
     |  Parameters:
     |      addr (IN) -- PGA460 register address to write data to
     |      data (IN) -- 8-bit data value to write into register
     |
     |  Returns:  none
     *-------------------------------------------------------------------*/
void registerWrite(byte addr, byte data)
{
    byte bufRR[5] = {syncByte, SRW, addr, data, calcChecksum(SRW)};
    spiSendData(bufRR, 5);
    delay_cycles(240000);
}

/*------------------------------------------------- registerRead -----
     |  Function registerRead
     |
     |  Purpose:  Read single register data from PGA460
     |
     |  Parameters:
     |      addr (IN) -- PGA460 register address to read data from
     |
     |  Returns:  8-bit data read from register
     *-------------------------------------------------------------------*/
byte registerRead(byte addr)
{
    regAddr      = addr;
    byte data    = 0x00;
    byte buf8[4] = {syncByte, SRR, regAddr, calcChecksum(SRR)};
    spiSendData(buf8, 4);
    delay_cycles(240000);
    spiMosiIdle(3);
    data = misoBuf[1];
    return data;
}

/*------------------------------------------------- initThresholds -----
     |  Function initThresholds
     |
     |  Purpose:  Updates threshold mapping for both presets, and performs bulk threshold write
     |
     |  Parameters:
     |      thr (IN) -- updates all threshold levels to a fixed level based on specific percentage of the maximum level.
     |          All times are mid-code (1.4ms intervals).
     |          Modify existing case statements, or append additional case-statement for custom user threshold configurations.
     |          • 0 = 25% Levels 64 of 255
     |          • 1 = 50% Levels 128 of 255
     |          • 2 = 75% Levels 192 of 255
     |          • 3 = customized
     |  Returns:  none
     *-------------------------------------------------------------------*/
void initThresholds(byte thr)
{
    switch (thr) {
        case 0:  //25% Levels 64 of 255
            P1_THR_0  = 0x88;
            P1_THR_1  = 0x88;
            P1_THR_2  = 0x88;
            P1_THR_3  = 0x88;
            P1_THR_4  = 0x88;
            P1_THR_5  = 0x88;
            P1_THR_6  = 0x42;
            P1_THR_7  = 0x10;
            P1_THR_8  = 0x84;
            P1_THR_9  = 0x21;
            P1_THR_10 = 0x08;
            P1_THR_11 = 0x40;
            P1_THR_12 = 0x40;
            P1_THR_13 = 0x40;
            P1_THR_14 = 0x40;
            P1_THR_15 = 0x00;
            P2_THR_0  = 0x88;
            P2_THR_1  = 0x88;
            P2_THR_2  = 0x88;
            P2_THR_3  = 0x88;
            P2_THR_4  = 0x88;
            P2_THR_5  = 0x88;
            P2_THR_6  = 0x42;
            P2_THR_7  = 0x10;
            P2_THR_8  = 0x84;
            P2_THR_9  = 0x21;
            P2_THR_10 = 0x08;
            P2_THR_11 = 0x40;
            P2_THR_12 = 0x40;
            P2_THR_13 = 0x40;
            P2_THR_14 = 0x40;
            P2_THR_15 = 0x00;
            break;

        case 1:  //50% Level (midcode) 128 of 255
            P1_THR_0  = 0x88;
            P1_THR_1  = 0x88;
            P1_THR_2  = 0x88;
            P1_THR_3  = 0x88;
            P1_THR_4  = 0x88;
            P1_THR_5  = 0x88;
            P1_THR_6  = 0x84;
            P1_THR_7  = 0x21;
            P1_THR_8  = 0x08;
            P1_THR_9  = 0x42;
            P1_THR_10 = 0x10;
            P1_THR_11 = 0x80;
            P1_THR_12 = 0x80;
            P1_THR_13 = 0x80;
            P1_THR_14 = 0x80;
            P1_THR_15 = 0x00;
            P2_THR_0  = 0x88;
            P2_THR_1  = 0x88;
            P2_THR_2  = 0x88;
            P2_THR_3  = 0x88;
            P2_THR_4  = 0x88;
            P2_THR_5  = 0x88;
            P2_THR_6  = 0x84;
            P2_THR_7  = 0x21;
            P2_THR_8  = 0x08;
            P2_THR_9  = 0x42;
            P2_THR_10 = 0x10;
            P2_THR_11 = 0x80;
            P2_THR_12 = 0x80;
            P2_THR_13 = 0x80;
            P2_THR_14 = 0x80;
            P2_THR_15 = 0x00;
            break;

        case 2:  //75% Levels 192 of 255
            P1_THR_0  = 0x88;
            P1_THR_1  = 0x88;
            P1_THR_2  = 0x88;
            P1_THR_3  = 0x88;
            P1_THR_4  = 0x88;
            P1_THR_5  = 0x88;
            P1_THR_6  = 0xC6;
            P1_THR_7  = 0x31;
            P1_THR_8  = 0x8C;
            P1_THR_9  = 0x63;
            P1_THR_10 = 0x18;
            P1_THR_11 = 0xC0;
            P1_THR_12 = 0xC0;
            P1_THR_13 = 0xC0;
            P1_THR_14 = 0xC0;
            P1_THR_15 = 0x00;
            P2_THR_0  = 0x88;
            P2_THR_1  = 0x88;
            P2_THR_2  = 0x88;
            P2_THR_3  = 0x88;
            P2_THR_4  = 0x88;
            P2_THR_5  = 0x88;
            P2_THR_6  = 0xC6;
            P2_THR_7  = 0x31;
            P2_THR_8  = 0x8C;
            P2_THR_9  = 0x63;
            P2_THR_10 = 0x18;
            P2_THR_11 = 0xC0;
            P2_THR_12 = 0xC0;
            P2_THR_13 = 0xC0;
            P2_THR_14 = 0xC0;
            P2_THR_15 = 0x00;
            break;

        case 3:  //Custom
            P1_THR_0  = 0x00;
            P1_THR_1  = 0x11;
            P1_THR_2  = 0x11;
            P1_THR_3  = 0x11;
            P1_THR_4  = 0x11;
            P1_THR_5  = 0x11;
            P1_THR_6  = 0xC9;
            P1_THR_7  = 0x8C;
            P1_THR_8  = 0x63;
            P1_THR_9  = 0x18;
            P1_THR_10 = 0xC6;
            P1_THR_11 = 0x30;
            P1_THR_12 = 0x30;
            P1_THR_13 = 0x30;
            P1_THR_14 = 0x30;
            P1_THR_15 = 0x00;
            P2_THR_0  = 0x11;
            P2_THR_1  = 0x11;
            P2_THR_2  = 0x11;
            P2_THR_3  = 0x11;
            P2_THR_4  = 0x11;
            P2_THR_5  = 0x11;
            P2_THR_6  = 0xC9;
            P2_THR_7  = 0x8C;
            P2_THR_8  = 0x63;
            P2_THR_9  = 0x18;
            P2_THR_10 = 0xC6;
            P2_THR_11 = 0x30;
            P2_THR_12 = 0x30;
            P2_THR_13 = 0x30;
            P2_THR_14 = 0x30;
            P2_THR_15 = 0x00;
            break;

        default:
            break;
    }
    byte buf16[35] = {syncByte, THRBW, P1_THR_0, P1_THR_1, P1_THR_2, P1_THR_3,
        P1_THR_4, P1_THR_5, P1_THR_6, P1_THR_7, P1_THR_8, P1_THR_9, P1_THR_10,
        P1_THR_11, P1_THR_12, P1_THR_13, P1_THR_14, P1_THR_15, P2_THR_0,
        P2_THR_1, P2_THR_2, P2_THR_3, P2_THR_4, P2_THR_5, P2_THR_6, P2_THR_7,
        P2_THR_8, P2_THR_9, P2_THR_10, P2_THR_11, P2_THR_12, P2_THR_13,
        P2_THR_14, P2_THR_15, calcChecksum(THRBW)};

    spiSendData(buf16, 35);

    delay_cycles(2400000);
    return;
}

/*------------------------------------------------- defaultPGA460 -----
     |  Function defaultPGA460
     |
     |  Purpose:  Updates user EEPROM values, and performs bulk EEPROM write.
     |
     |  Parameters:
     |      xdcr (IN) -- updates user EEPROM based on predefined listing for a specific transducer.
     |          Modify existing case statements, or append additional case-statement for custom user EEPROM configurations.
     |          • 0 = Murata MA58MF14-7N
     |          • 1 = Murata MA40H1S-R
     |          • 2 = PUI Audio UTR-1440K-TT-R
     |          • 3 = Customized
     |  Returns:  none
     *-------------------------------------------------------------------*/
void defaultPGA460(byte xdcr)
{
    switch (xdcr) {
        case 0:  // Murata MA58MF14-7N
            USER_DATA1   = 0x00;
            USER_DATA2   = 0x00;
            USER_DATA3   = 0x00;
            USER_DATA4   = 0x00;
            USER_DATA5   = 0x00;
            USER_DATA6   = 0x00;
            USER_DATA7   = 0x00;
            USER_DATA8   = 0x00;
            USER_DATA9   = 0x00;
            USER_DATA10  = 0x00;
            USER_DATA11  = 0x00;
            USER_DATA12  = 0x00;
            USER_DATA13  = 0x00;
            USER_DATA14  = 0x00;
            USER_DATA15  = 0x00;
            USER_DATA16  = 0x00;
            USER_DATA17  = 0x00;
            USER_DATA18  = 0x00;
            USER_DATA19  = 0x00;
            USER_DATA20  = 0x00;
            TVGAIN0      = 0x00;
            TVGAIN1      = 0x00;
            TVGAIN2      = 0x00;
            TVGAIN3      = 0x71;
            TVGAIN4      = 0xC7;
            TVGAIN5      = 0x1C;
            TVGAIN6      = 0x70;
            INIT_GAIN    = 0x5C;
            FREQUENCY    = 0x8F;
            DEADTIME     = 0x00;
            PULSE_P1     = 0x10;
            PULSE_P2     = 0x12;
            CURR_LIM_P1  = 0x5C;
            CURR_LIM_P2  = 0x7F;
            REC_LENGTH   = 0x8C;
            FREQ_DIAG    = 0x00;
            SAT_FDIAG_TH = 0xEE;
            FVOLT_DEC    = 0x7C;
            DECPL_TEMP   = 0x0A;
            DSP_SCALE    = 0x00;
            TEMP_TRIM    = 0x00;
            P1_GAIN_CTRL = 0x08;
            P2_GAIN_CTRL = 0x00;
            break;
        case 1:  // Murata MA40H1SR
            USER_DATA1   = 0x00;
            USER_DATA2   = 0x00;
            USER_DATA3   = 0x00;
            USER_DATA4   = 0x00;
            USER_DATA5   = 0x00;
            USER_DATA6   = 0x00;
            USER_DATA7   = 0x00;
            USER_DATA8   = 0x00;
            USER_DATA9   = 0x00;
            USER_DATA10  = 0x00;
            USER_DATA11  = 0x00;
            USER_DATA12  = 0x00;
            USER_DATA13  = 0x00;
            USER_DATA14  = 0x00;
            USER_DATA15  = 0x00;
            USER_DATA16  = 0x00;
            USER_DATA17  = 0x00;
            USER_DATA18  = 0x00;
            USER_DATA19  = 0x00;
            USER_DATA20  = 0x00;
            TVGAIN0      = 0xAA;
            TVGAIN1      = 0xAA;
            TVGAIN2      = 0xAA;
            TVGAIN3      = 0x51;
            TVGAIN4      = 0x45;
            TVGAIN5      = 0x14;
            TVGAIN6      = 0x50;
            INIT_GAIN    = 0x54;
            FREQUENCY    = 0x32;
            DEADTIME     = 0xA0;
            PULSE_P1     = 0x08;
            PULSE_P2     = 0x10;
            CURR_LIM_P1  = 0x40;
            CURR_LIM_P2  = 0x40;
            REC_LENGTH   = 0x19;
            FREQ_DIAG    = 0x33;
            SAT_FDIAG_TH = 0xFE;
            FVOLT_DEC    = 0x7C;
            DECPL_TEMP   = 0x4F;
            DSP_SCALE    = 0x00;
            TEMP_TRIM    = 0x00;
            P1_GAIN_CTRL = 0x09;
            P2_GAIN_CTRL = 0x09;
            break;

        case 5:  // Murata MA4S4S/R
            USER_DATA1   = 0x00;
            USER_DATA2   = 0x00;
            USER_DATA3   = 0x00;
            USER_DATA4   = 0x00;
            USER_DATA5   = 0x00;
            USER_DATA6   = 0x00;
            USER_DATA7   = 0x00;
            USER_DATA8   = 0x00;
            USER_DATA9   = 0x00;
            USER_DATA10  = 0x00;
            USER_DATA11  = 0x00;
            USER_DATA12  = 0x00;
            USER_DATA13  = 0x00;
            USER_DATA14  = 0x00;
            USER_DATA15  = 0x00;
            USER_DATA16  = 0x00;
            USER_DATA17  = 0x00;
            USER_DATA18  = 0x00;
            USER_DATA19  = 0x00;
            USER_DATA20  = 0x00;
            TVGAIN0      = 0xAA;
            TVGAIN1      = 0xAA;
            TVGAIN2      = 0xAA;
            TVGAIN3      = 0x51;
            TVGAIN4      = 0x45;
            TVGAIN5      = 0x14;
            TVGAIN6      = 0x50;
            INIT_GAIN    = 0x54;
            FREQUENCY    = 0x32;
            DEADTIME     = 0xA0;
            PULSE_P1     = 0x02;
            PULSE_P2     = 0x10;
            CURR_LIM_P1  = 0x40;
            CURR_LIM_P2  = 0x40;
            REC_LENGTH   = 0x19;
            FREQ_DIAG    = 0x33;
            SAT_FDIAG_TH = 0xFE;
            FVOLT_DEC    = 0x7C;
            DECPL_TEMP   = 0x4F;
            DSP_SCALE    = 0x00;
            TEMP_TRIM    = 0x00;
            P1_GAIN_CTRL = 0x00;
            P2_GAIN_CTRL = 0x00;
            break;

        case 2:  // PUI Audio UTR-1440K-TT-R
        {
            USER_DATA1   = 0x00;
            USER_DATA2   = 0x00;
            USER_DATA3   = 0x00;
            USER_DATA4   = 0x00;
            USER_DATA5   = 0x00;
            USER_DATA6   = 0x00;
            USER_DATA7   = 0x00;
            USER_DATA8   = 0x00;
            USER_DATA9   = 0x00;
            USER_DATA10  = 0x00;
            USER_DATA11  = 0x00;
            USER_DATA12  = 0x00;
            USER_DATA13  = 0x00;
            USER_DATA14  = 0x00;
            USER_DATA15  = 0x00;
            USER_DATA16  = 0x00;
            USER_DATA17  = 0x00;
            USER_DATA18  = 0x00;
            USER_DATA19  = 0x00;
            USER_DATA20  = 0x00;
            TVGAIN0      = 0x9D;
            TVGAIN1      = 0xEE;
            TVGAIN2      = 0xEF;
            TVGAIN3      = 0x2D;
            TVGAIN4      = 0xB9;
            TVGAIN5      = 0xEF;
            TVGAIN6      = 0xDC;
            INIT_GAIN    = 0x03;
            FREQUENCY    = 0x32;
            DEADTIME     = 0x80;
            PULSE_P1     = 0x08;
            PULSE_P2     = 0x12;
            CURR_LIM_P1  = 0x72;
            CURR_LIM_P2  = 0x32;
            REC_LENGTH   = 0x09;
            FREQ_DIAG    = 0x33;
            SAT_FDIAG_TH = 0xEE;
            FVOLT_DEC    = 0x7C;
            DECPL_TEMP   = 0x8F;
            DSP_SCALE    = 0x00;
            TEMP_TRIM    = 0x00;
            P1_GAIN_CTRL = 0x09;
            P2_GAIN_CTRL = 0x29;
            break;
        }
        case 3:  // User Custom XDCR
        {
            USER_DATA1   = 0x02;
            USER_DATA2   = 0x00;
            USER_DATA3   = 0x00;
            USER_DATA4   = 0x00;
            USER_DATA5   = 0x00;
            USER_DATA6   = 0x00;
            USER_DATA7   = 0x00;
            USER_DATA8   = 0x00;
            USER_DATA9   = 0x00;
            USER_DATA10  = 0x01;
            USER_DATA11  = 0x00;
            USER_DATA12  = 0x00;
            USER_DATA13  = 0x00;
            USER_DATA14  = 0x00;
            USER_DATA15  = 0x00;
            USER_DATA16  = 0x00;
            USER_DATA17  = 0x00;
            USER_DATA18  = 0x00;
            USER_DATA19  = 0x00;
            USER_DATA20  = 0x00;
            TVGAIN0      = 0x22;
            TVGAIN1      = 0x22;
            TVGAIN2      = 0x22;
            TVGAIN3      = 0x10;
            TVGAIN4      = 0xA5;
            TVGAIN5      = 0x20;
            TVGAIN6      = 0xC1;
            INIT_GAIN    = 0x40;
            FREQUENCY    = 0x11;
            DEADTIME     = 0x20;
            PULSE_P1     = 0x04;
            PULSE_P2     = 0x12;
            CURR_LIM_P1  = 0x5C;
            CURR_LIM_P2  = 0x7F;
            REC_LENGTH   = 0x19;
            FREQ_DIAG    = 0x00;
            SAT_FDIAG_TH = 0xEE;
            FVOLT_DEC    = 0x7C;
            DECPL_TEMP   = 0x0A;
            DSP_SCALE    = 0x00;
            TEMP_TRIM    = 0x00;
            P1_GAIN_CTRL = 0x0A;
            P2_GAIN_CTRL = 0x09;
            break;
        }
        default:
            break;
    }

    byte buf12[46] = {syncByte, EEBW, USER_DATA1, USER_DATA2, USER_DATA3,
        USER_DATA4, USER_DATA5, USER_DATA6, USER_DATA7, USER_DATA8, USER_DATA9,
        USER_DATA10, USER_DATA11, USER_DATA12, USER_DATA13, USER_DATA14,
        USER_DATA15, USER_DATA16, USER_DATA17, USER_DATA18, USER_DATA19,
        USER_DATA20, TVGAIN0, TVGAIN1, TVGAIN2, TVGAIN3, TVGAIN4, TVGAIN5,
        TVGAIN6, INIT_GAIN, FREQUENCY, DEADTIME, PULSE_P1, PULSE_P2,
        CURR_LIM_P1, CURR_LIM_P2, REC_LENGTH, FREQ_DIAG, SAT_FDIAG_TH,
        FVOLT_DEC, DECPL_TEMP, DSP_SCALE, TEMP_TRIM, P1_GAIN_CTRL,
        P2_GAIN_CTRL, calcChecksum(EEBW)};

    spiSendData(buf12, 46);

    delay_cycles(1200000);
    return;
}

/*------------------------------------------------- initTVG -----
     |  Function initTVG
     |
     |  Purpose:  Updates time varying gain (TVG) range and mapping, and performs bulk TVG write
     |
     |  Parameters:
     |      agr (IN) -- updates the analog gain range for the TVG.
     |          • 0 = 32-64dB
     |          • 1 = 46-78dB
     |          • 2 = 52-84dB
     |          • 3 = 58-90dB
     |      tvg (IN) -- updates all TVG levels to a fixed level based on specific percentage of the maximum level.
     |          All times are mid-code (2.4ms intervals).
     |          Modify existing case statements, or append additional case-statement for custom user TVG configurations
     |          • 0 = 25% Levels of range
     |          • 1 = 50% Levels of range
     |          • 2 = 75% Levels of range
     |
     |  Returns:  none
     *-------------------------------------------------------------------*/

void initTVG(byte agr, byte tvg)
{
    byte gain_range = 0x4F;
    // set AFE gain range
    switch (agr) {
        case 3:  //58-90dB
            gain_range = 0x0F;
            break;
        case 2:  //52-84dB
            gain_range = 0x4F;
            break;
        case 1:  //46-78dB
            gain_range = 0x8F;
            break;
        case 0:  //32-64dB
            gain_range = 0xCF;
            break;
        default:
            break;
    }

    regAddr       = 0x26;
    regData       = gain_range;
    byte buf10[5] = {syncByte, SRW, regAddr, regData, calcChecksum(SRW)};
    spiSendData(buf10, 5);

    switch (tvg) {
        case 0:  //25% Level
            TVGAIN0 = 0x88;
            TVGAIN1 = 0x88;
            TVGAIN2 = 0x88;
            TVGAIN3 = 0x41;
            TVGAIN4 = 0x04;
            TVGAIN5 = 0x10;
            TVGAIN6 = 0x40;
            break;

        case 1:  //50% Levels
            TVGAIN0 = 0x88;
            TVGAIN1 = 0x88;
            TVGAIN2 = 0x88;
            TVGAIN3 = 0x82;
            TVGAIN4 = 0x08;
            TVGAIN5 = 0x20;
            TVGAIN6 = 0x80;
            break;

        case 2:  //75% Levels
            TVGAIN0 = 0x88;
            TVGAIN1 = 0x88;
            TVGAIN2 = 0x88;
            TVGAIN3 = 0xC3;
            TVGAIN4 = 0x0C;
            TVGAIN5 = 0x30;
            TVGAIN6 = 0xC0;
            break;

        case 3:  //customer
            TVGAIN0 = 0xAA;
            TVGAIN1 = 0xAA;
            TVGAIN2 = 0xAA;
            TVGAIN3 = 0x82;
            TVGAIN4 = 0x08;
            TVGAIN5 = 0x20;
            TVGAIN6 = 0x80;
            break;
        default:
            break;
    }

    byte buf14[10] = {syncByte, TVGBW, TVGAIN0, TVGAIN1, TVGAIN2, TVGAIN3,
        TVGAIN4, TVGAIN5, TVGAIN6, calcChecksum(TVGBW)};
    spiSendData(buf14, 10);
    delay_cycles(2400000);

    return;
}

/*------------------------------------------------- ultrasonicCmd -----
     |  Function ultrasonicCmd
     |
     |  Purpose:  Issues a burst-and-listen or listen-only command based on the number of objects to be detected.
     |
     |  Parameters:
     |      cmd (IN) -- determines which preset command is run
     |          • 0 = Preset 1 Burst + Listen command
     |          • 1 = Preset 2 Burst + Listen command
     |          • 2 = Preset 1 Listen Only command
     |          • 3 = Preset 2 Listen Only command
     |          • 17 = Preset 1 Burst + Listen broadcast command
     |          • 18 = Preset 2 Burst + Listen broadcast command
     |          • 19 = Preset 1 Listen Only broadcast command
     |          • 20 = Preset 2 Listen Only broadcast command
     |      numObjUpdate (IN) -- PGA460 can capture time-of-flight, width, and amplitude for 1 to 8 objects.
     |          TCI is limited to time-of-flight measurement data only.
     |
     |  Returns:  none
     *-------------------------------------------------------------------*/
void ultrasonicCmd(byte cmd, byte numObjUpdate)
{
    numObj         = numObjUpdate;  // number of objects to detect
    byte bufCmd[4] = {syncByte, 0xFF, numObj,
        0xFF};  // prepare bufCmd with 0xFF placeholders

    switch (cmd) {
        // SINGLE ADDRESS
        case 0:  // Send Preset 1 Burst + Listen command
        {
            bufCmd[1] = P1BL;
            bufCmd[3] = calcChecksum(P1BL);
            break;
        }
        case 1:  // Send Preset 2 Burst + Listen command
        {
            bufCmd[1] = P2BL;
            bufCmd[3] = calcChecksum(P2BL);
            break;
        }
        case 2:  // Send Preset 1 Listen Only command
        {
            bufCmd[1] = P1LO;
            bufCmd[3] = calcChecksum(P1LO);
            break;
        }
        case 3:  // Send Preset 2 Listen Only command
        {
            bufCmd[1] = P2LO;
            bufCmd[3] = calcChecksum(P2LO);
            break;
        }

        // BROADCAST
        case 17:  // Send Preset 1 Burst + Listen Broadcast command
        {
            bufCmd[1] = BC_P1BL;
            bufCmd[3] = calcChecksum(BC_P1BL);
            break;
        }
        case 18:  // Send Preset 2 Burst + Listen Broadcast command
        {
            bufCmd[1] = BC_P2BL;
            bufCmd[3] = calcChecksum(BC_P2BL);
            break;
        }
        case 19:  // Send Preset 1 Listen Only Broadcast command
        {
            bufCmd[1] = BC_P1LO;
            bufCmd[3] = calcChecksum(BC_P1LO);
            break;
        }
        case 20:  // Send Preset 2 Listen Only Broadcast command
        {
            bufCmd[1] = BC_P2LO;
            bufCmd[3] = calcChecksum(BC_P2LO);
            break;
        }

        default:
            return;
    }

    spiSendData(bufCmd, 4);
    // maximum record length is 65ms, so delay with margin

    return;
}

/*------------------------------------------------- runEchoDataDump -----
     |  Function runEchoDataDump
     |
     |  Purpose:  Runs a preset 1 or 2 burst and or listen command to capture 128 bytes of echo data dump.
     |      Toggle echo data dump enable bit to enable/disable echo data dump mode.
     |
     |  Parameters:
     |      preset (IN) -- determines which preset command is run:
     |          • 0 = Preset 1 Burst + Listen command
     |          • 1 = Preset 2 Burst + Listen command
     |          • 2 = Preset 1 Listen Only command
     |          • 3 = Preset 2 Listen Only command
     |          • 17 = Preset 1 Burst + Listen broadcast command
     |          • 18 = Preset 2 Burst + Listen broadcast command
     |          • 19 = Preset 1 Listen Only broadcast command
     |          • 20 = Preset 2 Listen Only broadcast command
     |
     |  Returns:  none
     *-------------------------------------------------------------------*/
void runEchoDataDump(byte preset)
{
    // enable Echo Data Dump bit
    regAddr        = 0x40;
    regData        = 0x80;
    byte writeType = SRW;  // default to single address register write (cmd10)
    if (preset >
        16)  // update to broadcast register write if broadcast TOF preset command given
    {
        writeType = BC_RW;  // cmd22
    }

    byte buf10[5] = {
        syncByte, writeType, regAddr, regData, calcChecksum(writeType)};

    spiSendData(buf10, 5);

    delay_cycles(10000);

    // run preset 1 or 2 burst and or listen command
    ultrasonicCmd(preset, 1);

    // disbale Echo Data Dump bit
    regData  = 0x00;
    buf10[3] = regData;
    buf10[4] = calcChecksum(writeType);

    for (int i = 0; i < 3; i++) {
        DL_SPI_transmitData8(SPI_0_INST, buf10[i]);
        delay_cycles(500);
    }

    return;
}
/*------------------------------------------------- pullEchoDataDumpBulk -----
     |  Function pullEchoDataDumpBulk
     |
     |  Purpose:  Bulk read out 128 bytes of echo data dump (EDD) from latest burst and or listen command.
     |      For UART and OWU, readout bulk echo data dump register values.
     |      For TCI, perform index 12 read of all echo data dump values in bulk.
     |
     |  Parameters:
     |      none
     |
     |  Returns:  comma delimited string of all EDD values
     *-------------------------------------------------------------------*/
//    char* pullEchoDataDumpBulk()
//    {
//        char bulkString[128];
//
//            // run read bulk transducer echo data dump command on first iteration
//            byte buf7[3] = {syncByte, TEDD, calcChecksum(TEDD)};
//
//            for (int i = 0; i<3; i++)
//                                                {
//                                                    DL_SPI_transmitData8(SPI_0_INST, buf7[i]);
//                                                    delay_cycles(500);
//                                                }
//
//            // MOSI transmit 0xFF to pull MISO return data
//            //clear FIFO
//
//
//            return ;
//    }

/*------------------------------------------------- pullUltrasonicMeasResult -----
     |  Function pullUltrasonicMeasResult
     |
     |  Purpose:  Read the ultrasonic measurement result data based on the last busrt and/or listen command issued.
     *-------------------------------------------------------------------*/
void pullUltrasonicMeasResult()
{
    byte buf5[3] = {syncByte, UMR, calcChecksum(UMR)};
    spiSendData(buf5, sizeof(buf5));
    // MOSI transmit 0xFF to pull MISO return data
    spiMosiIdle(numObj * 4 + 1);
    // copy MISO global array data to local array based on number of objects
    for (int n = 0; n < ((2 + (numObj * 4)) + owuShift); n++) {
        ultraMeasResult[n + 1] = misoBuf[n];
    }
    return;
}

double printUltrasonicMeasResult(byte umr)
{
    int speedSound   = 343;  // speed of sound in air at room temperature
    double objReturn = 0;
    double digitalDelay =
        0;  // TODO: compensates the burst time calculated as number_of_pulses/frequency.
    uint16_t objDist  = 0;
    uint16_t objWidth = 0;
    uint16_t objAmp   = 0;
    switch (umr) {
        case 0:  //Obj1 Distance (m)
        {
            objDist   = (ultraMeasResult[1] << 8) + ultraMeasResult[2];
            objReturn = (objDist / 2 * 0.000001 * speedSound) - digitalDelay;
            break;
        }
        case 1:  //Obj1 Width (us)
        {
            objWidth  = ultraMeasResult[3];
            objReturn = objWidth * 16;
            break;
        }
        case 2:  //Obj1 Peak Amplitude
        {
            objAmp    = ultraMeasResult[4];
            objReturn = objAmp;
            break;
        }

        case 3:  //Obj2 Distance (m)
        {
            objDist   = (ultraMeasResult[5] << 8) + ultraMeasResult[6];
            objReturn = (objDist / 2 * 0.000001 * speedSound) - digitalDelay;
            break;
        }
        case 4:  //Obj2 Width (us)
        {
            objWidth  = ultraMeasResult[7];
            objReturn = objWidth * 16;
            break;
        }
        case 5:  //Obj2 Peak Amplitude
        {
            objAmp    = ultraMeasResult[8];
            objReturn = objAmp;
            break;
        }

        case 6:  //Obj3 Distance (m)
        {
            objDist   = (ultraMeasResult[9] << 8) + ultraMeasResult[10];
            objReturn = (objDist / 2 * 0.000001 * speedSound) - digitalDelay;
            break;
        }
        case 7:  //Obj3 Width (us)
        {
            objWidth  = ultraMeasResult[11];
            objReturn = objWidth * 16;
            break;
        }
        case 8:  //Obj3 Peak Amplitude
        {
            objAmp    = ultraMeasResult[12];
            objReturn = objAmp;
            break;
        }
        case 9:  //Obj4 Distance (m)
        {
            objDist   = (ultraMeasResult[13] << 8) + ultraMeasResult[14];
            objReturn = (objDist / 2 * 0.000001 * speedSound) - digitalDelay;
            break;
        }
        case 10:  //Obj4 Width (us)
        {
            objWidth  = ultraMeasResult[15];
            objReturn = objWidth * 16;
            break;
        }
        case 11:  //Obj4 Peak Amplitude
        {
            objAmp    = ultraMeasResult[16];
            objReturn = objAmp;
            break;
        }
        case 12:  //Obj5 Distance (m)
        {
            objDist   = (ultraMeasResult[17] << 8) + ultraMeasResult[18];
            objReturn = (objDist / 2 * 0.000001 * speedSound) - digitalDelay;
            break;
        }
        case 13:  //Obj5 Width (us)
        {
            objWidth  = ultraMeasResult[19];
            objReturn = objWidth * 16;
            break;
        }
        case 14:  //Obj5 Peak Amplitude
        {
            objAmp    = ultraMeasResult[20];
            objReturn = objAmp;
            break;
        }
        case 15:  //Obj6 Distance (m)
        {
            objDist   = (ultraMeasResult[21] << 8) + ultraMeasResult[22];
            objReturn = (objDist / 2 * 0.000001 * speedSound) - digitalDelay;
            break;
        }
        case 16:  //Obj6 Width (us)
        {
            objWidth  = ultraMeasResult[23];
            objReturn = objWidth * 16;
            break;
        }
        case 17:  //Obj6 Peak Amplitude
        {
            objAmp    = ultraMeasResult[24];
            objReturn = objAmp;
            break;
        }
        case 18:  //Obj7 Distance (m)
        {
            objDist   = (ultraMeasResult[25] << 8) + ultraMeasResult[26];
            objReturn = (objDist / 2 * 0.000001 * speedSound) - digitalDelay;
            break;
        }
        case 19:  //Obj7 Width (us)
        {
            objWidth  = ultraMeasResult[27];
            objReturn = objWidth * 16;
            break;
        }
        case 20:  //Obj7 Peak Amplitude
        {
            objAmp    = ultraMeasResult[28];
            objReturn = objAmp;
            break;
        }
        case 21:  //Obj8 Distance (m)
        {
            objDist   = (ultraMeasResult[29] << 8) + ultraMeasResult[30];
            objReturn = (objDist / 2 * 0.000001 * speedSound) - digitalDelay;
            break;
        }
        case 22:  //Obj8 Width (us)
        {
            objWidth  = ultraMeasResult[31];
            objReturn = objWidth * 16;
            break;
        }
        case 23:  //Obj8 Peak Amplitude
        {
            objAmp    = ultraMeasResult[32];
            objReturn = objAmp;
            break;
        }
    }
    return objReturn;
}

void doubleToString(double num, char* str)
{
    int whole       = (int) num;
    double fraction = num - whole;

    int i = 0;
    if (whole == 0) {
        str[i++] = '0';
    } else {
        while (whole != 0) {
            str[i++] = '0' + whole % 10;
            whole    = whole / 10;
        }
    }
    int n = i - 1;
    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j]    = str[n];
        str[n]    = temp;
        n--;
    }

    str[i++] = '.';

    for (int k = 0; k < 5; k++) {
        fraction *= 10;
        int digit = (int) fraction;
        str[i++]  = '0' + digit;
        fraction -= digit;
    }
    return;
}

void uartSend(double num)
{
    char letterbuf[30] = "\r\n P1 Obj1 Distance (m): ";

    for (int i = 0; i < 30; i++) {
        DL_UART_Main_transmitData(UART_0_INST, letterbuf[i]);
        delay_cycles(1000000);
    }

    char resultbuf[20];
    doubleToString(num, resultbuf);
    for (int i = 0; i < 7; i++) {
        DL_UART_Main_transmitData(UART_0_INST, resultbuf[i]);
        delay_cycles(1000000);
    }
    return;
}
