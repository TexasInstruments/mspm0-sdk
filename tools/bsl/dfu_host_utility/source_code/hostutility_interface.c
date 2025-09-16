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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include "commands.h"
#include <windows.h>


#define fileTypeUnknown 0    
#define fileTypeTxt 1
#define fileTypeBin 2

#define WRITE_LONG(num, ptr)                                                  \
{                                                                             \
    *((unsigned char *)(ptr)) = ((num) & 0xFF);                               \
    *(((unsigned char *)(ptr)) + 1) = (((num) >> 8) & 0xFF);                  \
    *(((unsigned char *)(ptr)) + 2) = (((num) >> 16) & 0xFF);                 \
    *(((unsigned char *)(ptr)) + 3) = (((num) >> 24) & 0xFF);                 \
}

#define WRITE_SHORT(num, ptr)                                                 \
{                                                                             \
    *((unsigned char *)(ptr)) = ((num) & 0xFF);                               \
    *(((unsigned char *)(ptr)) + 1) = (((num) >> 8) & 0xFF);                  \
}



//structure to get command code from command string
typedef struct {
    char *name;
    Command command;
} CommandMap;

//A structure which will will hold data regarding Txt files
typedef struct {
    long int address;  //address
    unsigned char *data;   //pointer to the hex data 
    int dataSize;           //hex data 
    int dataCapacity;      //only for dynamic allocation
} InputBlock;



CommandMap commandMap[] = {
    {"CMD_MEMORY_READ_BACK", CMD_MEMORY_READ_BACK}, 
    {"CMD_CONNECTION", CMD_CONNECTION},
    {"CMD_UNLOCK_BSL", CMD_UNLOCK_BSL},
    {"CMD_GET_IDENTITY", CMD_GET_IDENTITY},
    {"CMD_MASS_ERASE", CMD_MASS_ERASE},
    {"CMD_FACTORY_RESET", CMD_FACTORY_RESET},
    {"CMD_STANDALONE_VERIFICATION", CMD_STANDALONE_VERIFICATION},
    {"CMD_START_APPLICATION", CMD_START_APPLICATION},
    {"CMD_FLASH_RANGE_ERASE",CMD_FLASH_RANGE_ERASE},
    {"CMD_PROGRAM_DATA", CMD_PROGRAM_DATA},
    {NULL, 0}
};


InputBlock *txtInputDict = NULL;
int blockCount = 0;
int blockCapacity = 0;

//USB identifiers
unsigned short g_usVendorID  = 0x2047;    
unsigned short g_usProductID = 0x0210;    
unsigned short g_usDeviceID  = 0x0001;

char *inputFile    = "empty.txt";   //input name(image file)
unsigned long inputFileSize = 0;
int fileType=0;  //0 - not supported 1 - txt 2 - bin

//Debuuging variblers:
int g_logsEnabled = 0;    //disabled by default
int sendFile=1;          //send file to device after processing, enabled by default pass -s flag to just wrap
int sendStatus=0;        //flag to check if the download request successful then only do upload request

//DFU prefix and suffix
unsigned char *g_pcDFUPrefix=NULL;        
unsigned long int sizeofDFUPrefix = 0;   

//suffix 
unsigned char g_pcDFUSuffix[] =
{
    0x00, // bcdDevice LSB
    0x00, // bcdDevice MSB
    0x00, // idProduct LSB
    0x00, // idProduct MSB
    0x00, // idVendor LSB
    0x00, // idVendor MSB
    0x00, // bcdDFU LSB
    0x01, // bcdDFU MSB
    'U',  // ucDfuSignature LSB
    'F',  // ucDfuSignature
    'D',  // ucDfuSignature MSB
    16,   // bLength
    0x00, // dwCRC LSB
    0x00, // dwCRC byte 2
    0x00, // dwCRC byte 3
    0x00  // dwCRC MSB
};


char *g_pszOutput = "image.dfu";      //output image name
unsigned long  g_ulAddress   = -1;   //check for bin files if address provided used by txt too
unsigned long g_ulLength = -1;       //any command that requires length
unsigned long g_ulAddress1 = -1;    //this variable is used for flash range erase command


//command specific variables
long int readBackAddress=-1;
long int flashRangeEraseAddress1=-1;
long int flashRangeEraseAddress2=-1;
long int readBackLength=-1;
long int verificationAddress=-1;
long int verificationLength=-1; 

Command currentCommand;

// Converts bin to text format
int binConv(char *input_file, char *output_file) {

    printf("Bin file path : %s\n", input_file);
    printf("Output file path : %s\n", output_file);

    // Open input binary file
    FILE *fin = fopen(input_file, "rb");
    if (!fin) {
        perror("Error opening input file");
        return 1;
    }

    // Get file size
    fseek(fin, 0, SEEK_END);
    long filesize = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    // Allocate buffer and read file
    unsigned char *buffer = (unsigned char *)malloc(filesize);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(fin);
        return 1;
    }
    fread(buffer, 1, filesize, fin);
    fclose(fin);

    // Open output text file
    FILE *fout = fopen(output_file, "w");
    if (!fout) {
        perror("Error opening output file");
        free(buffer);
        return 1;
    }

    // Write hex data, 16 bytes per line, space-separated
    for (long i = 0; i < filesize; i += 16) {
        int line_len = (filesize - i >= 16) ? 16 : (filesize - i);
        for (int j = 0; j < line_len; ++j) {
            fprintf(fout, "%02X", buffer[i + j]);
            if (j < line_len - 1) fprintf(fout, " ");
        }
        fprintf(fout, "\n");
    }

    printf("Hex data of given .bin file has been written to %s\n", output_file);

    free(buffer);
    fclose(fout);
    return 0;
}

// Copy file from one location to other
int copyFile(char *sourceFileName, char *destFileName){
    FILE *sourceFile, *destFile;
    char buffer[1024]; // Buffer to hold data temporarily
    size_t bytesRead;

    // Open the source file in binary read mode
    sourceFile = fopen(sourceFileName, "rb");
    if (sourceFile == NULL) {
        perror("Error opening source file");
        return 1;
    }

    // Open the destination file in binary write mode
    destFile = fopen(destFileName, "wb");
    if (destFile == NULL) {
        perror("Error opening destination file");
        fclose(sourceFile);
        return 1;
    }

    // Read from source and write to destination
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
        fwrite(buffer, 1, bytesRead, destFile);
    }

    // Close the files
    fclose(sourceFile);
    fclose(destFile);

    printf("File copied successfully.\n");
    return 0;
}

void setPrefix(Command C){
    switch (C)
    {

    case CMD_CONNECTION:
        g_pcDFUPrefix = g_pcDFUPrefix_CMD_CONNECTION;
        sizeofDFUPrefix = sizeof(g_pcDFUPrefix_CMD_CONNECTION); 
        break;
    
    case CMD_UNLOCK_BSL:
        g_pcDFUPrefix = g_pcDFUPrefix_CMD_UNLOCK_BSL;
        sizeofDFUPrefix = sizeof(g_pcDFUPrefix_CMD_UNLOCK_BSL);
        break;
    
    case CMD_GET_IDENTITY:
        g_pcDFUPrefix = g_pcDFUPrefix_CMD_GET_IDENTITY;
        sizeofDFUPrefix = sizeof(g_pcDFUPrefix_CMD_GET_IDENTITY);
        break;
    case CMD_FLASH_RANGE_ERASE:
        g_pcDFUPrefix = g_pcDFUPrefix_CMD_FLASH_RANGE_ERASE;
        sizeofDFUPrefix = sizeof(g_pcDFUPrefix_CMD_FLASH_RANGE_ERASE);
        break;
    case CMD_MASS_ERASE:
        g_pcDFUPrefix = g_pcDFUPrefix_CMD_MASS_ERASE;
        sizeofDFUPrefix = sizeof(g_pcDFUPrefix_CMD_MASS_ERASE);
        break;

     case CMD_PROGRAM_DATA:
        g_pcDFUPrefix = g_pcDFUPrefix_CMD_PROGRAM_DATA;
        sizeofDFUPrefix = sizeof(g_pcDFUPrefix_CMD_PROGRAM_DATA);
        break;
    case CMD_MEMORY_READ_BACK:
        g_pcDFUPrefix = g_pcDFUPrefix_CMD_MEMORY_READ_BACK;
        sizeofDFUPrefix = sizeof(g_pcDFUPrefix_CMD_MEMORY_READ_BACK);
        break;
    case CMD_FACTORY_RESET:
        g_pcDFUPrefix = g_pcDFUPrefix_CMD_FACTORY_RESET;
        sizeofDFUPrefix = sizeof(g_pcDFUPrefix_CMD_FACTORY_RESET);
        break;
    case CMD_STANDALONE_VERIFICATION:
        g_pcDFUPrefix = g_pcDFUPrefix_CMD_STANDALONE_VERIFICATION;
        sizeofDFUPrefix = sizeof(g_pcDFUPrefix_CMD_STANDALONE_VERIFICATION);
        break;
    case CMD_START_APPLICATION:
        g_pcDFUPrefix = g_pcDFUPrefix_CMD_START_APPLICATION;
        sizeofDFUPrefix = sizeof(g_pcDFUPrefix_CMD_START_APPLICATION);
        break;
    default:
        printf("Unknown command: 0x%02x\n", C);
        break;
    }
    
}


unsigned long
CalculateCRC32(unsigned char* data, unsigned long length)
{
    unsigned long ii, jj, byte, crc, mask;;
    unsigned long CRC32_POLY = 0xEDB88320; 

    crc = 0xFFFFFFFF;

    for(ii=0;ii<length;ii++)
    {
        byte = data[ii];
        crc = crc ^ byte;

        for (jj = 0; jj < 8; jj++)
        {
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (CRC32_POLY & mask);
        }

    }

    return crc;
}

Command getCommand(char *str) {
    for (int i = 0; commandMap[i].name != NULL; i++) {
        if (strcmp(str, commandMap[i].name) == 0) {
            return commandMap[i].command;
        }
    }
    printf("Unknown command\n");
    exit(1);
}

// find the extension
int checkFileExtension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) {
        printf("Format not supported\n");
        return fileTypeUnknown;
    }
    if (strcmp(dot, ".txt") == 0) {
        return fileTypeTxt;
    } else if (strcmp(dot, ".bin") == 0) {
        return fileTypeBin;
    } else {
        printf("Format not supported\n");
        return fileTypeUnknown;
    }
}


int sendDFUFile(char *filename){
    char send[256];
    char move[256];
    int found = 0;

    // Move file to dfu-util directory
    if (g_logsEnabled)
        snprintf(move, sizeof(move), "move %s dfu-util/", filename);
    else
        snprintf(move, sizeof(move), "move %s dfu-util/ > NUL 2>&1", filename);

    system(move);

    if (chdir("dfu-util/") != 0) {
        printf("Error: Could not change to dfu-util directory.\n");
        return 0;
    }

    if (g_logsEnabled) {
        // Show dfu-util output in terminal and check for status(0)
        snprintf(send, sizeof(send), "dfu-util.exe -D %s -d %04x:%04x", filename, g_usVendorID, g_usProductID);
        FILE *fp = _popen(send, "r");
        if (fp) {
            char line[256];
            while (fgets(line, sizeof(line), fp)) {
                fputs(line, stdout);
                if (strstr(line, "status(0)")) {
                    found = 1;
                }
            }
            _pclose(fp);
        }
    } else {
        // Redirect output to debug log file
        snprintf(send, sizeof(send), "dfu-util.exe -D %s -d %04x:%04x > ../debug/dfu_log.txt 2>&1", filename, g_usVendorID, g_usProductID);
        system(send);

        // Check log for "status(0)"
        FILE *log = fopen("../debug/dfu_log.txt", "r");
        if (log) {
            char line[256];
            while (fgets(line, sizeof(line), log)) {
                if (strstr(line, "status(0)")) {
                    found = 1;
                    break;
                }
            }
            fclose(log);
        }
        if (!found) {
            printf("DFU download may have failed. See debug/dfu_log.txt for details.\n");
        } else {
            remove("../debug/dfu_log.txt");
        }
    }

    if (found) {
        sendStatus = 1;
    } else {
        sendStatus = 0;
    }

    chdir("..");
    return 0;
}


void uploadRequest(char *filename, int file_type) {
    char upload[256];
    char conv[256];
    char binfile[256];
    char tempbinfile[256];
    int found = 0;

    if (file_type == 0) return;
    if (chdir("dfu-util/") != 0) {
        printf("Error: Could not change to dfu-util directory.\n");
        return;
    }

    // Temporary readback.bin file
    snprintf(tempbinfile, sizeof(tempbinfile), "output/readback.bin");
    snprintf(binfile, sizeof(binfile), "../output/readback.bin");

    // Truncate readback.bin in output folder
    FILE *fp = fopen(binfile, "wb");
    if (fp) fclose(fp);

    if (g_logsEnabled) {
        // Show dfu-util output in terminal and check for finished!
        snprintf(upload, sizeof(upload), "dfu-util.exe -U %s -d %04x:%04x", binfile, g_usVendorID, g_usProductID);
        FILE *fpup = _popen(upload, "r");
        if (fpup) {
            char line[256];
            while (fgets(line, sizeof(line), fpup)) {
                fputs(line, stdout);
                if (strstr(line, "finished!")) {
                    found = 1;
                }
            }
            _pclose(fpup);
        }
    } else {
        // Log to debug folder
        snprintf(upload, sizeof(upload), "dfu-util.exe -U %s -d %04x:%04x > ../debug/dfu_upload_log.txt 2>&1", binfile, g_usVendorID, g_usProductID);
        system(upload);

        // Check log for "finished!"
        FILE *log = fopen("../debug/dfu_upload_log.txt", "r");
        if (log) {
            char line[256];
            while (fgets(line, sizeof(line), log)) {
                if (strstr(line, "finished!")) {
                    found = 1;
                    break;
                }
            }
            fclose(log);
        }
        if (!found) {
            printf("DFU upload may have failed. See debug/dfu_upload_log.txt for details.\n");
        } else {
            remove("../debug/dfu_upload_log.txt");
        }
    }

    if (found) {
        sendStatus = 1;
    } else {
        sendStatus = 0;
    }
    chdir("..");

    // Convert to user output file in output folder
    if(file_type == 1){ // User wants .txt output
        binConv(tempbinfile, filename);
    }
    else { // User wants .bin output
        char copycommand[256];
        // Copy only if the file is not same as the temporarily generated bin file
        if(strcmp(filename, tempbinfile) != 0){
            copyFile(tempbinfile, filename);
        }
    }
}
void addBlock(long int address) {
    // Resize the array if necessary
    if (blockCount >= blockCapacity) {
        blockCapacity = (blockCapacity == 0) ? 4 : blockCapacity * 2;
        txtInputDict = realloc(txtInputDict, blockCapacity * sizeof(InputBlock));
    }

    // Initialize the new block
    txtInputDict[blockCount].address = address;
    txtInputDict[blockCount].data = NULL;
    txtInputDict[blockCount].dataSize = 0;
    txtInputDict[blockCount].dataCapacity = 0;

    // Preallocate for prefix
    if (sizeofDFUPrefix > 0) {
        txtInputDict[blockCount].dataCapacity = sizeofDFUPrefix * 2;
        txtInputDict[blockCount].data = malloc(txtInputDict[blockCount].dataCapacity);
        for (int i = 0; i < sizeofDFUPrefix; i++) {
            txtInputDict[blockCount].data[txtInputDict[blockCount].dataSize++] = g_pcDFUPrefix[i];
        }
    }
    blockCount++;
    //printf("size of block count %d\n", blockCount);
}

void addDataToCurrentBlock(unsigned char byte) {
    InputBlock *current = &txtInputDict[blockCount - 1];
    // Grow buffer if needed
    if (current->dataSize >= current->dataCapacity) {
        current->dataCapacity = (current->dataCapacity == 0) ? 64 : current->dataCapacity * 2;
        current->data = realloc(current->data, current->dataCapacity);
    }
    current->data[current->dataSize++] = byte;
}

void parseInputFile(char *filename) {
    // Open the input file
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Cannot open file");
        return;
    }

    // Read the input file line by line
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        // Check if the line starts with '@'
        if (line[0] == '@') {
            // Extract the address
            long int addr;
            sscanf(line + 1, "%lx", &addr);

            // Add a new block
            addBlock(addr);
        } else if (blockCount > 0) {
            // Tokenize the line
            char *token = strtok(line, " \n\r");
            while (token) {
                // Convert the token to a byte
                unsigned int byte;
                if (sscanf(token, "%x", &byte) == 1) {
                    // Add the byte to the current block
                    addDataToCurrentBlock((unsigned char)byte);
                }

                // Get the next token
                token = strtok(NULL, " \n\r");
            }
        }
    }

    // Close the input file
    fclose(fp);
}
//1-error opening output file
//2-error writing data to output file
//0-success

int writeOutputTxt(char *pszFile, unsigned char *pcData, unsigned long ulLength){
    
    FILE *fh;
    unsigned long ulWritten;
    int flag=1;
    fh=fopen(pszFile, "wb");  //overwrite condition here
    if (fh == NULL) 
    {
        printf("Error opening output file for writing\n");
        return 1;  // Error opening file
    }
    
    if(fh)
    {
        ulWritten = fwrite(pcData, ulLength, 1, fh);
        fclose(fh);
        
        
    }
   
    while (access(pszFile, F_OK) == -1) {
        // File does not exist, wait and try again
        sleep(1); // Wait for 1 second before trying again
    }

    printf("File written successfully: %s\n", pszFile);
    flag=0;
    
    
    return flag;
    
}
//Bin handling
//reading gives buffer ptr to the data and size as well as parameters

unsigned char *
ReadInputFileBin(char *pcFilename, long *pulLength){
    char *pcFileBuffer;
    int iRead;
    int iSize;
    int iSizeAlloc;
    FILE *fhFile;

    fhFile = fopen(pcFilename, "rb");
    if (fhFile == NULL)
    {
        printf("Cannot open file\n");
        return NULL;
    }
    

    fseek(fhFile, 0, SEEK_END);
    iSize = ftell(fhFile);
    fseek(fhFile, 0, SEEK_SET);

    inputFileSize = iSize;

    iSizeAlloc=iSize+sizeofDFUPrefix+sizeof(g_pcDFUSuffix);
    pcFileBuffer = malloc(iSizeAlloc);
    if(pcFileBuffer==NULL){
        printf("Cannot allocate memory\n");
        return NULL;
    }
    
    iRead = fread(pcFileBuffer,1, iSize, fhFile);
    //WRITE PREFIX
    memcpy(pcFileBuffer, g_pcDFUPrefix,  sizeofDFUPrefix);
    //WRITE SUFFIX
    memcpy(&pcFileBuffer[iSizeAlloc] - sizeof(g_pcDFUSuffix), g_pcDFUSuffix,sizeof(g_pcDFUSuffix));
    fclose(fhFile);
    *pulLength = iSizeAlloc;
    return pcFileBuffer;

}

//output file name , buffer ptr where the data was written in readinputbinfile,and length of the file+suffix+prefix
//error codes:
//0-success
//1-open file error
//2- couldnt write the entire file


int
WriteOutputFileBin(char *pszFile, unsigned char *pcData, unsigned long ulLength){
    FILE *fh;
    int iResponse;
    unsigned long ulWritten;

    fh = fopen(pszFile, "wb");
    if(!fh)
    {
        printf("Error opening output file for writing\n");
        return(1);
    }
    ulWritten = fwrite(pcData, 1, ulLength, fh);
    printf("data written to output file: %s\n", pszFile);

    if(ulWritten != ulLength)
    {
        printf("Error writing data to output file! Wrote %ld, "
                   "requested %ld\n", ulWritten, ulLength);
        return(2);
    }

    
    fclose(fh);
    return 0;

}


void handleTxt(void) {
    char filename[50] = {'\0'};
    parseInputFile(inputFile);
    int num = 0;
    
    while (num < blockCount) {
        txtInputDict[num].data = (unsigned char *)realloc(txtInputDict[num].data, txtInputDict[num].dataSize + sizeof(g_pcDFUSuffix));
        unsigned char *pcInput = txtInputDict[num].data;
        readBackAddress = g_ulAddress;
        readBackLength = g_ulLength;
        g_ulAddress = txtInputDict[num].address;
        inputFileSize += txtInputDict[num].dataSize;
        memcpy(pcInput + txtInputDict[num].dataSize, g_pcDFUSuffix, sizeof(g_pcDFUSuffix));
        sprintf(filename, "firmware_%ld_%x.dfu", txtInputDict[num].address, currentCommand);
        unsigned char *pcSuffix = txtInputDict[num].data + txtInputDict[num].dataSize + sizeof(g_pcDFUSuffix);
        WRITE_SHORT(g_usDeviceID, pcSuffix - 16);
        WRITE_SHORT(g_usProductID, pcSuffix - 14);
        WRITE_SHORT(g_usVendorID, pcSuffix - 12);
        unsigned long lenofdata = txtInputDict[num].dataSize - sizeofDFUPrefix;
        unsigned long ulFileLen = txtInputDict[num].dataSize + sizeof(g_pcDFUSuffix);

        

        if ((g_pcDFUPrefix[3] == CMD_PROGRAM_DATA )) {
                WRITE_LONG(g_ulAddress, pcInput + 4);
                WRITE_LONG(lenofdata, pcInput + 8);
            }
        printf("writing to file %s with address %ld \n", filename, txtInputDict[num].address);
        
        unsigned long ulCRC = CalculateCRC32(pcInput, ulFileLen - 4);
        WRITE_LONG(ulCRC, pcSuffix - 4);
        writeOutputTxt(filename, txtInputDict[num].data, txtInputDict[num].dataSize + sizeof(g_pcDFUSuffix));

        if (sendFile) {
            printf("sending file: %s\n", filename);
            sendDFUFile(filename);
            //remove these lines 
            if (currentCommand == CMD_MEMORY_READ_BACK || currentCommand == CMD_GET_IDENTITY || currentCommand == CMD_STANDALONE_VERIFICATION) {
                
                if (strcmp(g_pszOutput, "image.dfu") == 0) {
                    printf("please provide outputfile name for the response file\n");
                    return;
                }
                printf("output file %s\n", g_pszOutput);
                int outputFileType = checkFileExtension(g_pszOutput);
                uploadRequest(g_pszOutput, outputFileType);


            }
        }
        num++;
        free(pcInput);
    }
}

//This handles the binary file as well as all other commands other than program data with txt file
//This function only calls sendDFUFile if sendFile is true and upload request
void handleBin(void) {
    // Check for required address for program data command with binary file
    if (g_ulAddress == -1 && fileType == fileTypeBin && currentCommand == CMD_PROGRAM_DATA) {
        printf("You need to provide address\n");
        return;
    }

    unsigned long ulFileLen;
    unsigned long ulCRC;

    unsigned char *pcInput;
    if (currentCommand == CMD_PROGRAM_DATA )
    {  
        pcInput= ReadInputFileBin(inputFile, &ulFileLen);
    }
    else{
       
        ulFileLen = sizeofDFUPrefix + sizeof(g_pcDFUSuffix);
        pcInput = (unsigned char *)malloc(ulFileLen);
        if (!pcInput) {
            printf("Error: Could not allocate memory for command buffer.\n");
            return;
        }
        memcpy(pcInput, g_pcDFUPrefix, sizeofDFUPrefix);
        memcpy(pcInput + sizeofDFUPrefix, g_pcDFUSuffix, sizeof(g_pcDFUSuffix));
    }
     
    if (!pcInput) {
        printf("Error: Could not read input file.\n");
        return;
    }
    unsigned char *pcSuffix = pcInput + ulFileLen;

    // Write device, product, vendor IDs into suffix
    WRITE_SHORT(g_usDeviceID, pcSuffix - 16);
    WRITE_SHORT(g_usProductID, pcSuffix - 14);
    WRITE_SHORT(g_usVendorID, pcSuffix - 12);

    // For program data, write address and length
    if (inputFileSize != 0 && g_pcDFUPrefix[3] == CMD_PROGRAM_DATA) {
        WRITE_LONG(g_ulAddress, pcInput + 4);
        WRITE_LONG(ulFileLen - (sizeofDFUPrefix + sizeof(g_pcDFUSuffix)), pcInput + 8);
    }

    // Handle memory read back command
    if (g_pcDFUPrefix[3] == CMD_MEMORY_READ_BACK) {
        readBackAddress = g_ulAddress;
        readBackLength = g_ulLength;
        if (readBackAddress == -1 || readBackLength == -1) {
            printf("Please provide readBackAddress and readBackLength\n");
            free(pcInput);
            return;
        }
        WRITE_LONG(readBackAddress, pcInput + 4);
        WRITE_LONG(readBackLength, pcInput + 8);
    }
    // Handle flash range erase command
    else if (g_pcDFUPrefix[3] == CMD_FLASH_RANGE_ERASE) {
        flashRangeEraseAddress1 = g_ulAddress;
        flashRangeEraseAddress2 = g_ulAddress1;
        if (flashRangeEraseAddress1 == -1 || flashRangeEraseAddress2 == -1) {
            printf("Please provide flashRangeEraseAddress1 and flashRangeEraseAddress2\n");
            free(pcInput);
            return;
        }

        WRITE_LONG(flashRangeEraseAddress1, pcInput + 4);
        WRITE_LONG(flashRangeEraseAddress2, pcInput + 8);
    }
    // Handle standalone verification command
    else if (g_pcDFUPrefix[3] == CMD_STANDALONE_VERIFICATION) {
        verificationAddress = g_ulAddress;
        verificationLength = g_ulLength;
        if (verificationAddress == -1 || verificationLength == -1) {
            printf("Please provide verification address and verification length\n");
            free(pcInput);
            return;
        }
        WRITE_LONG(verificationAddress, pcInput + 4);
        WRITE_LONG(verificationLength, pcInput + 8);
    }

    // Calculate and write CRC
    ulCRC = CalculateCRC32(pcInput, ulFileLen - 4);
    WRITE_LONG(ulCRC, pcInput + ulFileLen - 4);

    // Determine output filename
    char *imagefilename = "image.dfu";
    // User can choose to wrap only, else can do readback
    if (currentCommand == CMD_PROGRAM_DATA || currentCommand == CMD_CONNECTION ||
        currentCommand == CMD_MASS_ERASE || currentCommand == CMD_FLASH_RANGE_ERASE) {
        imagefilename = g_pszOutput;
    }

    // Write output file
    WriteOutputFileBin(imagefilename, pcInput, ulFileLen);
    printf("the output data written in %s\n", imagefilename);
    printf("\n");

    // Send file if required
    if (sendFile) {
        sendDFUFile(imagefilename);
        if (currentCommand == CMD_MEMORY_READ_BACK ||
            currentCommand == CMD_GET_IDENTITY ||
            currentCommand == CMD_STANDALONE_VERIFICATION) {
            if (strcmp(g_pszOutput, "image.dfu") == 0) {
                printf("please provide outputfile name for the response file\n");
                free(pcInput);
                return;
            }
            printf("output file %s\n", g_pszOutput);
            int outputfiletype = checkFileExtension(g_pszOutput);
            printf("the send staus is %d\n", sendStatus);
            if(sendStatus){
                
                uploadRequest(g_pszOutput, outputfiletype);
            }
           
           
        }
    }
    free(pcInput);
}

int getInputsHex(const char *prompt, unsigned long *val, unsigned long def) {
    char buf[64];
    printf("%s [0x%lX]: ", prompt, def);
    fgets(buf, sizeof(buf), stdin);
    if (buf[0] == '\n' || buf[0] == '\0') {
        *val = def;
        return 1;
    }
    *val = strtoul(buf, NULL, 0);
    return 1;
}

void getInputs(const char *prompt, char *out, size_t outsz, const char *def) {
    printf("%s [%s]: ", prompt, def ? def : "");
    fgets(out, outsz, stdin);
    out[strcspn(out, "\r\n")] = 0;
    if (strlen(out) == 0 && def) strcpy(out, def);
}

int interactive_menu() {
    printf("\nEnter the choice based on the command mapping shown below:\n");
    printf("1: Program Device\n");
    printf("2: Readback Memory\n");
    printf("3: Standalone Verification\n");
    printf("4: Mass Erase\n");
    printf("5: Get Device Info\n");
    printf("6: Factory Reset\n");
    printf("7: Flash Range Erase\n");
    printf("8: Start Application\n");
    printf("9: Unlock BSL\n");
    printf("10: Toggle logs (currently %s)\n", g_logsEnabled ? "ON" : "OFF");
    printf("11: Toggle send file (currently %s)\n", sendFile ? "ON" : "OFF"); 
    printf("0: Exit\n");
    printf("Choice: ");
    char buf[16];
    fgets(buf, sizeof(buf), stdin);
    int choice = atoi(buf);

    char inputfile[256] = "";
    char outputfile[256] = "";
    unsigned long addr = 0, addr1 = 0, len = 0;

    switch (choice) {
        case 1: // Program Device
            getInputs("Input file (.txt or .bin)", inputfile, sizeof(inputfile), "firmware.txt");
            fileType = checkFileExtension(inputfile);
            if (fileType == fileTypeTxt) { // TXT
                inputFile= strdup(inputfile);
                currentCommand = CMD_PROGRAM_DATA;
                setPrefix(currentCommand);
                handleTxt();
            } else if (fileType == fileTypeBin) { // BIN
                getInputsHex("Start address (hex)", &addr, 0x8000);
                getInputs("Output file name", outputfile, sizeof(outputfile), "image.dfu");
                inputFile= strdup(inputfile);
                g_ulAddress = addr;
                g_pszOutput = strdup(outputfile);
                currentCommand = CMD_PROGRAM_DATA;
                setPrefix(currentCommand);
                handleBin();
            } else {
                printf("Unsupported file type.\n");
            }
            break;
        case 2: // Readback
            getInputsHex("Start address (hex)", &addr, 0x0000);
            getInputsHex("Length (hex)", &len, 0x064);
            getInputs("Output file name", outputfile, sizeof(outputfile), "output/readback.bin");
            g_ulAddress = addr;
            g_ulLength = len;
            g_pszOutput = strdup(outputfile);
            currentCommand = CMD_MEMORY_READ_BACK;
            setPrefix(currentCommand);
            handleBin();
            break;
        case 3: // Standalone Verification
            getInputsHex("Start address (hex)", &addr, 0x0);
            getInputsHex("Length (hex)", &len, 0x1000);
            getInputs("Output file name", outputfile, sizeof(outputfile), "output/verification.bin");
            g_ulAddress = addr;
            g_ulLength = len;
            g_pszOutput = strdup(outputfile);
            currentCommand = CMD_STANDALONE_VERIFICATION;
            setPrefix(currentCommand);
            handleBin();
            break;
        case 4: // Mass Erase
            g_pszOutput = "image.dfu";
            currentCommand = CMD_MASS_ERASE;
            setPrefix(currentCommand);
            handleBin();
            break;
        case 5: // Get Device Info
            g_pszOutput = "output/device_info.txt";
            currentCommand = CMD_GET_IDENTITY;
            setPrefix(currentCommand);
            handleBin();
            break;
        case 6: // Factory Reset with password
            printf("Type 'null' if you want to operate factory reset without password\n");
            getInputs("Password file (.txt)", inputfile, sizeof(inputfile), "password.txt");
            //Without password mode
            if(strcmp(inputfile, "null") == 0){
                g_pszOutput = "image.dfu";
                currentCommand = CMD_FACTORY_RESET;
                setPrefix(currentCommand);
                handleBin();
                break;
            }
            inputFile= strdup(inputfile);
            currentCommand = CMD_FACTORY_RESET;
            setPrefix(currentCommand);
            fileType = checkFileExtension(inputFile);
            if (fileType == fileTypeTxt) handleTxt();
            else printf("Password must be in .txt format.\n");
            break;
        case 7: // Flash Range Erase
            getInputsHex("Start address (hex)", &addr, 0x8000);
            getInputsHex("End address (hex)", &addr1, 0x9000);
            g_ulAddress = addr;
            g_ulAddress1 = addr1;
            g_pszOutput = "image.dfu";
            currentCommand = CMD_FLASH_RANGE_ERASE;
            setPrefix(currentCommand);
            handleBin();
            break;
        case 8: // Start Application
            g_pszOutput = "image.dfu";
            currentCommand = CMD_START_APPLICATION;
            setPrefix(currentCommand);
            handleBin();
            break;
        case 9: // Unlock BSL
            getInputs("Password file (.txt)", inputfile, sizeof(inputfile), "password.txt");
            inputFile= strdup(inputfile);
            currentCommand = CMD_UNLOCK_BSL;
            setPrefix(currentCommand);
            fileType = checkFileExtension(inputFile);
            if (fileType == fileTypeTxt) handleTxt();
            else printf("Password must be in .txt format.\n");
            break;
        case 10:
            g_logsEnabled = !g_logsEnabled;
            printf("Logs are now %s.\n", g_logsEnabled ? "ENABLED" : "DISABLED");
            break;
        case 11:
            sendFile = !sendFile;
            printf("Send file is now %s.\n", sendFile ? "ENABLED" : "DISABLED");
            break;
        case 0:
            return 0;
        default:
            printf("Invalid choice.\n");
            break;
    }
    printf("\nDo you want to continue? (y/n): ");
    fgets(buf, sizeof(buf), stdin);
    if (buf[0] == 'n' || buf[0] == 'N') return 0;
    return 1;
}

void InitialSetup() {
    printf("Performing initial setup...\n");
    // 1.connection to device
    printf("Checking device connection...\n");
    currentCommand = CMD_CONNECTION;
    setPrefix(currentCommand);
    handleBin();

    // 2. Get device info
    printf("Getting device info...\n");
    inputFile= "empty.txt";
    g_pszOutput = "output/device_info.txt"; // Default output file for device info
    currentCommand = CMD_GET_IDENTITY;
    setPrefix(currentCommand);
    handleBin();
    printf("Initial setup complete.\n");
}
int main() {
    printf("Welcome to Interface Utility!\n");
    InitialSetup();
    while (interactive_menu());
    return 0;
}