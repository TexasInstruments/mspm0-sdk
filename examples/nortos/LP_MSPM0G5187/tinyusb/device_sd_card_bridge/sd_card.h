/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2025 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef SD_CARD_H_
#define SD_CARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tusb.h"

/* List of commands for SD Card */
#define GO_IDLE_STATE (0x00U)
#define SEND_OP_CMD (0x01U)
#define READ_CSD (0x09U)
#define READ_CID (0x0AU)
#define SET_BLOCKLEN (0x10U)
#define READ_SINGLE_BLOCK (0x11U)
#define SEND_STATUS (0x13U)
#define WRITE_BLOCK (0x18U)
#define SEND_OP_COND_CMD (0x29U)
#define SEND_IF_COND (0x08U)
#define APP_CMD (0x37U)
#define READ_OCR (0x3AU)

/* Check limit counter */
#define CHECK_LIMIT (65535)
/* Timeout value for SD Card in 5s */
#define TIMEOUT_5S (5000)

typedef struct TU_ATTR_PACKED {
    unsigned int reserved0 : 15;       /* [0,14] Reserved */
    unsigned int v27_28 : 1;           /* [15] 2.7v-2.8v supported */
    unsigned int v28_29 : 1;           /* [16] 2.8v-2.9v supported */
    unsigned int v29_30 : 1;           /* [17] 2.9v-3.0v supported */
    unsigned int v30_31 : 1;           /* [18] 3.0v-3.1v supported */
    unsigned int v31_32 : 1;           /* [19] 3.1v-3.2v supported */
    unsigned int v32_33 : 1;           /* [20] 3.2v-3.3v supported */
    unsigned int v33_34 : 1;           /* [21] 3.3v-3.4v supported */
    unsigned int v34_35 : 1;           /* [22] 3.4v-3.5v supported */
    unsigned int v35_36 : 1;           /* [23] 3.5v-3.6v supported */
    unsigned int vSwitch : 1;          /* [24] Switching to 1.8v accepted */
    unsigned int reserved1 : 5;        /* [25,29] Reserved */
    unsigned int cardCapacityStat : 1; /* [30] Card capacity status */
    unsigned int cardPwrUpStat : 1;    /* [31] Card power up status */
} OCR_BitField;

typedef union {
    /* Raw register access - allows reading/writing the entire 32-bit register at once */
    uint8_t raw[4];
    /* Structured bitfield access - provides named fields for each register bit position */
    OCR_BitField ocrBits;
} OCR_Reg_t;

/* CSD version 1.0 */
typedef struct TU_ATTR_PACKED {
    unsigned int always1 : 1;     /* [0]       Not used, always 1 */
    unsigned int CRC : 7;         /* [1,7]     CRC */
    unsigned int reserved5 : 2;   /* [8,9]     Reserved */
    unsigned int FILE_FORMAT : 2; /* [10,11]   File format */
    unsigned int
        TMP_WRITE_PROTECT : 1; /* [12]      Temporary write protection */
    unsigned int
        PERM_WRITE_PROTECT : 1; /* [13]      Permanent write protection */
    unsigned int COPY : 1;      /* [14]      Copy flag (OTP) */
    unsigned int FILE_FORMAT_GRP : 1; /* [15]      File format group */
    unsigned int reserved4 : 5;       /* [16,20]   Reserved */
    unsigned int
        WRITE_BL_PARTIAL : 1; /* [21]      Partial blocks for write allowed */
    unsigned int WRITE_BL_LEN : 4;  /* [22,25]   Max write data block length */
    unsigned int R2W_FACTOR : 3;    /* [26,28]   Write speed factory */
    unsigned int reserved3 : 2;     /* [29,30]   Reserved */
    unsigned int WP_GRP_ENABLE : 1; /* [31]      Write protect group enable */
    unsigned int WP_GRP_SIZE : 7;   /* [32,38]   Write protect group size */
    unsigned int SECTOR_SIZE : 7;   /* [39,45]   Erase sector size */
    unsigned int ERASE_BLK_EN : 1;  /* [46]      Erase single block enable */
    unsigned int C_SIZE_MULT : 3;   /* [47,49]   Device size multiplier */
    unsigned int
        VDD_W_CURR_MAX : 3; /* [50,52]   Max. write current @VDD max */
    unsigned int
        VDD_W_CURR_MIN : 3; /* [53,55]   Max. write current @VDD min */
    unsigned int VDD_R_CURR_MAX : 3; /* [56,58]   Max. read current @VDD max */
    unsigned int VDD_R_CURR_MIN : 3; /* [59,61]   Max. read current @VDD min */
    unsigned int C_SIZEL : 2;        /* [62,63]   C_SIZE LOW bits */
    unsigned int C_SIZEH : 10;       /* [64,73]   C_SIZE HIGH bits */
    unsigned int reserved1 : 2;      /* [74,75]   Reserved */
    unsigned int DSR_IMP : 1;        /* [76]      DSR implemented */
    unsigned int READ_BLK_MISALIGN : 1; /* [77]      Read block misalignment */
    unsigned int
        WRITE_BLK_MISALIGN : 1; /* [78]      Write block misalignment */
    unsigned int
        READ_BL_PARTIAL : 1; /* [79]      Partial blocks for read allowed */
    unsigned int READ_BL_LEN : 4;   /* [80,83]   Max read data block length */
    unsigned int cardCmdClass : 12; /* [84,95]   Card command class */
    unsigned int maxDataTranRate : 8; /* [96,103]  Max data transfer rate */
    unsigned int
        dataReadAccCyc : 8;       /* [104,111] Data read access time cycle */
    unsigned int dataReadAcc : 8; /* [112,119] Data read access time */
    unsigned int reserved0 : 6;   /* [120,125] Reserved */
    unsigned int csdStruct : 2;   /* [126,127] CSD structure */
} CSD_SD_BitField;

/* CSD version 2.0 */
typedef struct TU_ATTR_PACKED {
    unsigned int always1 : 1;     /* [0]       Not used, always 1 */
    unsigned int CRC : 7;         /* [1,7]     CRC */
    unsigned int reserved5 : 2;   /* [8,9]     Reserved */
    unsigned int FILE_FORMAT : 2; /* [10,11]   File format */
    unsigned int
        TMP_WRITE_PROTECT : 1; /* [12]      Temporary write protection */
    unsigned int
        PERM_WRITE_PROTECT : 1; /* [13]      Permanent write protection */
    unsigned int COPY : 1;      /* [14]      Copy flag (OTP) */
    unsigned int FILE_FORMAT_GRP : 1; /* [15]      File format group */
    unsigned int reserved4 : 5;       /* [16,20]   Reserved */
    unsigned int
        WRITE_BL_PARTIAL : 1; /* [21]   Partial blocks for write allowed */
    unsigned int WRITE_BL_LEN : 4;  /* [22,25]   Max write data block length */
    unsigned int R2W_FACTOR : 3;    /* [26,28]   Write speed factory */
    unsigned int reserved3 : 2;     /* [29,30]   Reserved */
    unsigned int WP_GRP_ENABLE : 1; /* [31]      Write protect group enable */
    unsigned int WP_GRP_SIZE : 7;   /* [32,38]   Write protect group size */
    unsigned int SECTOR_SIZE : 7;   /* [39,45]   Erase sector size */
    unsigned int ERASE_BLK_EN : 1;  /* [46]      Erase single block enable */
    unsigned int reserved2 : 1;     /* [47]      Reserved */
    unsigned int C_SIZEL : 16;      /* [48,63]   Device size low bits */
    unsigned int C_SIZEH : 6;       /* [64,69]   Device size high bits */
    unsigned int reserved1 : 6; /* [70,75]   Reserved */  //23
    unsigned int DSR_IMP : 1;           /* [76]      DSR implemented */
    unsigned int READ_BLK_MISALIGN : 1; /* [77]      Read block misalignment */
    unsigned int
        WRITE_BLK_MISALIGN : 1; /* [78]      Write block misalignment */
    unsigned int READ_BL_PARTIAL : 1;
    /* [79]      Partial blocks for read allowed */  //15
    unsigned int READ_BL_LEN : 4;   /* [80,83]   Max read data block length */
    unsigned int cardCmdClass : 12; /* [84,95]   Card command class */
    unsigned int maxDataTranRate : 8; /* [96,103]  Max data transfer rate */
    unsigned int
        dataReadAccCyc : 8;       /* [104,111] Data read access time cycle */
    unsigned int dataReadAcc : 8; /* [112,119] Data read access time */
    unsigned int reserved0 : 6;   /* [120,125] Reserved */
    unsigned int csdStruct : 2;   /* [126,127] CSD structure */
} CSD_HC_BitField;

typedef union {
    /* Raw register access - allows reading/writing the entire 128-bit register at once */
    uint8_t raw[16];
    /* Structured bitfield access - provides named fields for each register bit position */
    CSD_HC_BitField csdHcBits;
    /* Structured bitfield access - provides named fields for each register bit position */
    CSD_SD_BitField csdSdBits;
} CSD_Reg_t;

/* CID register bitfield */
typedef struct TU_ATTR_PACKED {
    unsigned int always1 : 1;  /* [0]        Not used, always 1 */
    unsigned int CRC : 7;      /* [1,7]      CRC7 checksum */
    unsigned int MDT : 12;     /* [8:19]     Manufacturing date */
    unsigned int reserved : 4; /* [20:23]    Reserved */
    unsigned int PSN : 32;     /* [24,55]    Product serial number */
    unsigned int PRV : 8;      /* [56,63]    Product revision */
    char PNM[5];               /* [64,104]   Product name */
    unsigned int OID : 16;     /* [104,119]  OEM/Application ID */
    unsigned int MID : 8;      /* [120,127]  Manufacturer ID */
} CID_BitField;

typedef union {
    /* Raw register access - allows reading/writing the entire 128-bit register at once */
    uint8_t raw[16];
    /* Structured bitfield access - provides named fields for each register bit position */
    CID_BitField cidBits;
} CID_Reg_t;

/* Confirm size of registers matches expectation */
TU_VERIFY_STATIC(
    (sizeof(CID_Reg_t) == 16), "CID register is not of proper size");
TU_VERIFY_STATIC(
    (sizeof(CSD_Reg_t) == 16), "CSD register is not of proper size");
TU_VERIFY_STATIC(
    (sizeof(OCR_Reg_t) == 4), "OCR register is not of proper size");

typedef struct TU_ATTR_PACKED {
    /* SD Card buffer */
    uint8_t buffer[512];
    /* SD Card OCR data */
    OCR_Reg_t OCR;
    /* SD Card CSD data */
    CSD_Reg_t CSD;
    /* SD Card CID data */
    CID_Reg_t CID;
    /* SD Card block size */
    uint16_t blockSize;
    /* SD Card block count */
    uint32_t blockCount;
    /* SD Card is in slot */
    bool cardIsInSlot;
    /* SD Card is in SPI mode */
    bool cardIsInSpiMode;
    /* SD Card is version 2.0 or later */
    bool cardIsV2_0;
    /* SD Card is SDHC/SDXC */
    bool cardIsSDHC_SDXC;
    /* SD Card is powered up */
    bool cardIsPowered;
    /* SD Card is initialized */
    bool cardIsInitialized;
} SD_Card_t;

extern SD_Card_t mspm0SdCard;

uint8_t sendSdCardCommand(uint8_t cmd, uint32_t arg, uint8_t crc,
    uint8_t token, uint8_t* responseBuffer, uint32_t responseLength);
bool sdCardInit(SD_Card_t* sdCard);
bool sdCardReadBlock(SD_Card_t* sdCard, uint32_t address);
bool sdCardWriteBlock(SD_Card_t* sdCard, uint32_t address, uint8_t* buffer);

#ifdef __cplusplus
}
#endif

#endif /* SD_CARD_H_ */
