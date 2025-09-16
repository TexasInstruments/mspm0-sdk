#include "aes_gcm.h"
#include <string.h>
#include "lockable_storage_public.h"

/* TODO: Remove when keystore support added */
//static uint32_t gcm_key[] = {0x92e9fffe, 0x1c736586, 0x948f6a6d, 0x08833067};

typedef struct gcm_header {
    uint32_t *keyHashPtr;
    uint32_t aadLength;
    uint32_t dataLength;
    uint32_t *aadPtr;
    uint32_t tag[4];
    uint32_t nonce[4];
} GCM_Header;

static GCM_Header header;

static DL_AESADV_Config gAESADV_genConfig = {
    .mode              = DL_AESADV_MODE_GCM_AUTONOMOUS,
    .direction         = DL_AESADV_DIR_DECRYPT,
    .ctr_ctrWidth      = DL_AESADV_CTR_WIDTH_32_BIT, /* Dont care */
    .cfb_fbWidth       = DL_AESADV_FB_WIDTH_128,     /* Dont care */
    .ccm_ctrWidth      = DL_AESADV_CCM_CTR_WIDTH_2_BYTES,
    .ccm_tagWidth      = DL_AESADV_CCM_TAG_WIDTH_1_BYTE,
    .iv                = NULL,
    .nonce             = NULL,
    .k1                = NULL,
    .k2                = NULL,
    .lowerCryptoLength = 0,
    .upperCryptoLength = 0,
    .aadLength         = 0,
};

static GCM_status GCM_parseHeader(uintptr_t headerPtr);
/* TODO: transfer this to use the keystore */
static GCM_status GCM_keySearch(uint32_t *hashPtr);

static void GCM_loadDataAligned(uint32_t *dataPtr, uint32_t lengthBytes);
static void GCM_processDataAligned(
    uint32_t *dataPtr, uint32_t *outPtr, uint32_t lengthBytes);

GCM_status GCM_init(uintptr_t headerPtr)
{
    GCM_status status;
    status = GCM_parseHeader(headerPtr);
    /* TODO: declare as a keyslot in the Keystore */
    uint32_t aesKeySize;
    uint32_t i;

    DL_KEYSTORECTL_Config keyStrConfig;
    // TODO: check for key hash in effective keys
    if (status == GCM_OK) {
        // TODO: pickup here
        if (Lock_keySearch(header.keyHashPtr, &keyStrConfig) == 0) {
            if (keyStrConfig.keySize == DL_KEYSTORECTL_KEY_SIZE_128_BITS) {
                aesKeySize = DL_AESADV_KEY_SIZE_128_BIT;
            } else {
                aesKeySize = DL_AESADV_KEY_SIZE_256_BIT;
            }
            DL_AESADV_setKeySize(AESADV, aesKeySize);
            DL_KEYSTORECTL_transferKey(KEYSTORECTL, &keyStrConfig);
        } else {
            status = GCM_INIT_FAILED;
        }
    }

    //DL_AESADV_setKeyAligned(AESADV, gcm_key, DL_AESADV_KEY_SIZE_128_BIT);

    if (status == GCM_OK) {
        // set nonce with counter value 0x01 as part of the nonce
        header.nonce[3] = 0x01000000;

        gAESADV_genConfig.direction         = DL_AESADV_DIR_DECRYPT;
        gAESADV_genConfig.iv                = (uint8_t *) header.nonce;
        gAESADV_genConfig.aadLength         = header.aadLength;
        gAESADV_genConfig.lowerCryptoLength = header.dataLength;

        // init the GCM device
        DL_AESADV_initGCM(AESADV, &gAESADV_genConfig);

        GCM_loadDataAligned(header.aadPtr, header.aadLength);
    }

    return status;
}

GCM_status GCM_streamDataToFlash(
    uint32_t startAddr, uint32_t *inputPtr, uint32_t lengthBytes)
{
    uint32_t processedLength =
        (lengthBytes < header.dataLength) ? lengthBytes : header.dataLength;

    uint32_t outputBuffer[4];

    uint32_t i;

    for (i = 0; i < (processedLength / 16); i++) {
        while (!DL_AESADV_isInputReady(AESADV)) {
            ;
        }
        DL_AESADV_loadInputDataAligned(AESADV, inputPtr + (i * 4));

        if (i != 0) {
            // program new value into flash while next value is being computed
            DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(
                FLASHCTL, (uint32_t)(startAddr + ((i - 1) * 16)), outputBuffer,
                4, DL_FLASHCTL_REGION_SELECT_MAIN);
        }

        while (!DL_AESADV_isOutputReady(AESADV)) {
            ;
        }
        DL_AESADV_readOutputDataAligned(AESADV, outputBuffer);
    }

    /* final program of a full block */
    DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(FLASHCTL,
        (startAddr + ((i - 1) * 16)), outputBuffer, 4,
        DL_FLASHCTL_REGION_SELECT_MAIN);

    // update with any additional values if there is any additional AAD that needs
    // padding with 0
    uint32_t leftoverBytes = processedLength % 16;
    if (leftoverBytes != 0x00) {
        uint32_t paddedValue[] = {0x00, 0x00, 0x00, 0x00};
        memcpy((void *) paddedValue, (void *) (inputPtr + (i * 4)),
            leftoverBytes);
        while (!DL_AESADV_isInputReady(AESADV)) {
            ;
        }
        DL_AESADV_loadInputDataAligned(AESADV, paddedValue);

        while (!DL_AESADV_isOutputReady(AESADV)) {
            ;
        }
        DL_AESADV_readOutputDataAligned(AESADV, paddedValue);
        memset(((uint8_t *) paddedValue + leftoverBytes), 0xFF,
            (16 - leftoverBytes));

        /* final program if partial block */

        DL_FlashCTL_programMemoryBlockingFromRAM64WithECCGenerated(FLASHCTL,
            (startAddr + (i * 16)), paddedValue, 4,
            DL_FLASHCTL_REGION_SELECT_MAIN);
    }

    header.dataLength -= processedLength;

    if (header.dataLength <= 0) {
        return GCM_finishOperation();
    } else {
        return GCM_OPERATION_CONTINUES;
    }
}

GCM_status GCM_finishOperation(void)
{
    uint32_t tagOut[4];
    while (!DL_AESADV_isSavedOutputContextReady(AESADV)) {
        ;
    }
    DL_AESADV_readTAGAligned(AESADV, tagOut);

    if (0 == memcmp(tagOut, header.tag, 16)) {
        return GCM_OK;
    }
    return GCM_OPERATION_FAILED;
}

static GCM_status GCM_parseHeader(uintptr_t headerPtr)
{
    /* reset old header */
    header.keyHashPtr = NULL;
    memset(header.tag, 0x00, sizeof(header.tag));
    header.aadLength = 0;
    header.aadPtr    = NULL;
    memset(header.nonce, 0x00, sizeof(header.nonce));

    /* walk over TLV buffer */
    uint16_t *ptr = (uint16_t *) (headerPtr);

    uint16_t type;
    uint16_t length;
    uint16_t *value;
    uint32_t error = 0x00;

    type  = *ptr++;
    value = ptr++;

    if (type != ENCRYPTION_HEADER_MAGIC ||
        *(value) != ENCRYPTION_TYPE_OPERATION_GCM) {
        // Not set up for GCM
        return GCM_INVALID_HEADER;
    }

    type   = *ptr++;
    length = *ptr++;
    value  = ptr;
    while (type != ENCRYPTION_HEADER_END_OF_HEADER && error == 0x00) {
        switch (type) {
            case 0x55:
                // Key Hash
                if (length != 32) {
                    error = 0x1;
                } else {
                    header.keyHashPtr = (uint32_t *) value;
                }
                break;
            case 0x50:
                // TAG
                if (length != 16) {
                    error = 0x2;
                } else {
                    for (int i = 0; i < 4; i++) {
                        header.tag[i] = *((uint32_t *) value + i);
                    }
                }
                break;
            case 0x51:
                // NONCE/IV
                if (length != 12) {
                    error = 0x3;
                } else {
                    for (int i = 0; i < 3; i++) {
                        header.nonce[i] = *((uint32_t *) value + i);
                    }
                }
                break;
            case 0x52:
                // Data Length
                if (length != 4) {
                    error = 0x04;
                }
                header.dataLength = *((uint32_t *) value);
            case 0x53:
                // Additional Authentication Data (AAD)
                if ((length % 4) != 0) {
                    // Currently aad lengths must be 4-byte aligned
                    error = 0x05;
                }
                header.aadLength = length;
                header.aadPtr    = (uint32_t *) value;
                break;
            default:
                // type not recognized
                error = 0x06;
                break;
        }
        ptr += length / 2;
        type   = *ptr++;
        length = *ptr++;
        value  = ptr;
    }
    return (error == 0x00) ? GCM_OK : error;
}

static void GCM_loadDataAligned(uint32_t *dataPtr, uint32_t lengthBytes)
{
    // update with any and all data in 128-bit increments
    uint32_t i;
    for (i = 0; i < (lengthBytes / 16); i++) {
        while (!DL_AESADV_isInputReady(AESADV)) {
            ;
        }
        DL_AESADV_loadInputDataAligned(AESADV, dataPtr + (i * 4));
    }

    // update with any additional values if there is any additional AAD that needs
    // padding with 0
    uint32_t leftoverBytes = lengthBytes % 16;
    if (leftoverBytes != 0x00) {
        uint32_t paddedValue[] = {0x00, 0x00, 0x00, 0x00};
        memcpy(
            (void *) paddedValue, (void *) (dataPtr + (i * 4)), leftoverBytes);
        while (!DL_AESADV_isInputReady(AESADV)) {
            ;
        }
        DL_AESADV_loadInputDataAligned(AESADV, paddedValue);
    }
}
