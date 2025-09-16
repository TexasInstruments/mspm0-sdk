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

//A structure which will hold data regarding Txt files
typedef struct {
    long int address;  //address
    unsigned char *data;   //pointer to the hex data 
    int dataSize;           //hex data 
    int dataCapacity;      //only for dynamic allocation
} InputBlock;

Command currentCommand;

//in case of addition any command add it here and provide prefix
//Command strings
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

//USB identifiers
unsigned short g_usVendorID  = 0x2047;    
unsigned short g_usProductID = 0x0210;    
unsigned short g_usDeviceID  = 0x0001;

char *g_inputFile    = "empty.bin";   //input name(image file)
unsigned long g_inputFileSize = 0;
int fileType=0;  //0 - not supported 1 - txt 2 - bin

//Debuuging varibles:
int g_logsEnabled = 0;    //disabled by default
int sendFile=1;          //send file to device after processing, enabled by default pass -s flag to just wrap
int sendStatus=0;       //flag to stop readback if the download request fails

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

char *g_pszOutput = "image.dfu";      //output image name only for bin files
unsigned long  g_ulAddress   = -1;   //check for bin files if address provided
unsigned long g_ulLength = -1;       //any command that requires length
unsigned long g_ulAddress1 = -1;    //this variable is used for flash range erase command

//command specific variables
long int readBackAddress=-1;   
long int flashRangeEraseAddress1=-1;
long int flashRangeEraseAddress2=-1;
long int readBackLength=-1;
long int verificationAddress=-1;
long int verificationLength=-1; 

// Creating a dictinary based on address values|pointer to the hex data | size of data including prefix | Capacity of block in case of reallocation of memory
InputBlock *txtInputDict = NULL;
int blockCount = 0;
int blockCapacity = 0;

//help function to show usage of the utility
void showHelp(void) {
    printf("\nHost Utility CLI Tool\n");
    printf("Usage:\n");
    printf("  hostutility -c <command> -i <input_file> [options]\n\n");
    printf("Options:\n");
    printf("  -c <command>      Command to execute (see below for supported commands)\n");
    printf("  -i <input_file>   Input file (.txt for hex/text, .bin for binary, or password file for unlock/factory reset)\n");
    printf("  -a <address>      Start address (required for some commands)\n");
    printf("  -e <address>      End address (used for range erase commands)\n");
    printf("  -l <length>       Length (used for readback, verification, etc.)\n");
    printf("  -o <output_file>  Output file name (for response/readback or wrapped image)\n");
    printf("  -d <vendor_id>    Vendor ID in hex format (default: 0x2047)\n");
    printf("  -p <product_id>   Product ID in hex format (default: 0x0210)\n");
    printf("  -v                Enable verbose logging\n");
    printf("  -s                Only wrap file, do not send to device\n");
    printf("  -h                Show this help message\n\n");

    printf("Supported Commands:\n");
    printf("  CMD_PROGRAM_DATA            Program data to device (requires address and input file)\n");
    printf("  CMD_MEMORY_READ_BACK        Read memory from device (requires address and length)\n");
    printf("  CMD_CONNECTION              Test connection to device\n");
    printf("  CMD_UNLOCK_BSL              Unlock BSL (requires password file in .txt format)\n");
    printf("  CMD_GET_IDENTITY            Get device identity (requires output file)\n");
    printf("  CMD_MASS_ERASE              Mass erase device\n");
    printf("  CMD_FACTORY_RESET           Factory reset (supports both with and without password file in .txt format)\n");
    printf("  CMD_STANDALONE_VERIFICATION Standalone verification (requires address and length)\n");
    printf("  CMD_START_APPLICATION       Start application on device\n");
    printf("  CMD_FLASH_RANGE_ERASE       Erase flash range (requires start and end address)\n\n");

    printf("Examples:\n");
    printf("  main -c CMD_PROGRAM_DATA -i firmware.txt\n");
    printf("  main -c CMD_PROGRAM_DATA -i firmware.bin -a 0x8000 -o image.dfu\n");
    printf("  main -c CMD_MEMORY_READ_BACK -a 0x8000 -l 0x1000 -o readback.bin\n");
    printf("  main -c CMD_MEMORY_READ_BACK -a 0x8000 -l 0x1000 -o readback.txt\n");
    printf("  main -c CMD_UNLOCK_BSL -i password.txt\n");
    printf("  main -c CMD_FACTORY_RESET -i password.txt (if using with password)\n");
    printf("  main -c CMD_FACTORY_RESET (without password)\n");
    printf("  main -c CMD_CONNECTION\n");
    printf("  main -c CMD_MASS_ERASE\n");
    printf("  main -c CMD_START_APPLICATION\n");
    printf("  main -c CMD_STANDALONE_VERIFICATION -a 0x0 -l 0x1000 -o verification.bin\n");
    printf("  main -c CMD_STANDALONE_VERIFICATION -a 0x0 -l 0x1000 -o verification.txt\n");
    printf("  main -c CMD_FLASH_RANGE_ERASE -a 0x8000 -e 0x9000\n");
    printf("  main -c CMD_GET_IDENTITY -o device_info.txt\n\n");

    printf("Notes:\n");
    printf("  - For unlock/factory reset (with password), password must be provided in a .txt file.\n");
    printf("  - For .bin input, address (-a) is required for programming.\n");
    printf("  - Output file (-o) is required for readback and recommended for other commands.\n");
    printf("  - Use -s to only wrap the file without sending to the device.\n");
    printf("  - Verbose output can be enabled with -v for debugging.\n\n");
}

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

// Set the global DFU prefix and its size according to the selected command
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



//CRC
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



//Get command code from command string
Command getCommand(char *str) {
    for (int i = 0; commandMap[i].name != NULL; i++) {
        if (strcmp(str, commandMap[i].name) == 0) {
            return commandMap[i].command;
        }
    }
    printf("Unknown command\n");
    exit(1);
}

// find the extension of file
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

//collect inputs
int
ParseCommandLine(int argc, char *argv[])
{
    int iRetcode;

    
    while(1)
    {   

        if (argc == 1) {
        showHelp();
        return 0;
    }
        iRetcode = getopt(argc, argv, "c:i:a:e:o:d:p:l:hvs");

        if(iRetcode == -1)
        {
            break;
        }

        switch(iRetcode)
        {   
            case 'c':   //command flag-compulsory
                currentCommand = getCommand(optarg);
                setPrefix(currentCommand);
                break; 
            case 'i':  //image file name or {password file name(only in txt format)}
                g_inputFile = optarg;
                break;
            case 'a':  //start address for bin file or in case of other commands which require address
                g_ulAddress = (unsigned long)strtol(optarg, NULL, 0);
                break;
            case 'e':  //incase a command requires additional address (onky for mass erase)
                g_ulAddress1= (unsigned long)strtol(optarg, NULL, 0);
                break;
            case 'l':  //length
                g_ulLength = (unsigned long)strtol(optarg, NULL, 0);
                break;
            case 'o':   //final output file name //this doesn't apply to txt files img file in case of program data
                g_pszOutput = optarg;
                break;
            case 'v':  //track error in case of dfu-util errors
                g_logsEnabled = 1;
                break;
            case 's':   //disable sending file to device after processing
                sendFile = 0;
                break;
            case 'h':  
                showHelp();
                break;
            case 'p':  //product id
                g_usProductID = (unsigned short)strtol(optarg, NULL, 16);
                break;
            case 'd':  //vendor id 
                g_usVendorID = (unsigned short)strtol(optarg, NULL, 16);
                break;
            default:
                printf("Unknown option: %c\n", iRetcode);
                showHelp();
                break;
            
        }
           
    }


    
}

//Function to send returns 1 if the download request is successful, 0 otherwise
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
//Upload Request
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
        // Show dfu-util output in terminal
        snprintf(upload, sizeof(upload), "dfu-util.exe -U %s -d %04x:%04x", binfile, g_usVendorID, g_usProductID);
    } else {
        // Log to debug folder
        snprintf(upload, sizeof(upload), "dfu-util.exe -U %s -d %04x:%04x > ../debug/dfu_upload_log.txt 2>&1", binfile, g_usVendorID, g_usProductID);
    }
    system(upload);

    // Check log for "finished!" if logs are disabled
    if (!g_logsEnabled) {
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

//Parsing the txt file functions
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
//0-success

int writeOutputTxt(char *pszFile, unsigned char *pcData, unsigned long ulLength){
    
    FILE *fh;
    unsigned long ulWritten;
    
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
    return 0;
    
}


//Bin handling
//reading gives buffer ptr to the data
unsigned char *
ReadInputFileBin(char *pcFilename, long *pulLength) {
    unsigned char *pcFileBuffer;
    int iRead;
    int iSize=0;
    int iSizeAlloc;
    FILE *fhFile;
    
    fhFile = fopen(pcFilename, "rb");
    if (fhFile == NULL) {
        printf("Cannot open file\n");
        return NULL;
    }

    fseek(fhFile, 0, SEEK_END);
    iSize = ftell(fhFile);
    fseek(fhFile, 0, SEEK_SET);

    g_inputFileSize = iSize;

    iSizeAlloc = iSize + sizeofDFUPrefix + sizeof(g_pcDFUSuffix);
    pcFileBuffer = (unsigned char *)malloc(iSizeAlloc);
    if (pcFileBuffer == NULL) {
        printf("Cannot allocate memory\n");
        fclose(fhFile);
        return NULL;
    }

    // Write prefix
    memcpy(pcFileBuffer, g_pcDFUPrefix, sizeofDFUPrefix);

    // Read file data after prefix
    iRead = fread(pcFileBuffer + sizeofDFUPrefix, 1, iSize, fhFile);
    if (iRead != iSize) {
        printf("Error reading file data\n");
        free(pcFileBuffer);
        fclose(fhFile);
        return NULL;
    }

    // Write suffix at the end
    memcpy(pcFileBuffer + sizeofDFUPrefix + iSize, g_pcDFUSuffix, sizeof(g_pcDFUSuffix));

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
    unsigned char *pcInput;
    unsigned long ulCRC;
    unsigned long lenofdata;
    unsigned long ulFileLen;
    int num = 0;

    parseInputFile(g_inputFile);

    // For password commands, only one block should be present
    if (g_pcDFUPrefix[3] == CMD_FACTORY_RESET || g_pcDFUPrefix[3] == CMD_UNLOCK_BSL) {
        if (blockCount != 1) {
            printf("You might have given wrong password file\n");
            return;
        }
    }

    while (num < blockCount) {
        // Reallocate to fit suffix
        txtInputDict[num].data = (unsigned char *)realloc(
            txtInputDict[num].data,
            txtInputDict[num].dataSize + sizeof(g_pcDFUSuffix)
        );
        pcInput = txtInputDict[num].data;

        // Update addresses and sizes
        readBackAddress = g_ulAddress;
        readBackLength = g_ulLength;
        g_ulAddress = txtInputDict[num].address;
        g_inputFileSize += txtInputDict[num].dataSize;

        // Append DFU suffix
        memcpy(pcInput + txtInputDict[num].dataSize, g_pcDFUSuffix, sizeof(g_pcDFUSuffix));

        // Prepare output filename
        sprintf(filename, "firmware_%ld_%x.dfu", txtInputDict[num].address, currentCommand);

        // Suffix pointer for writing device/product/vendor IDs and CRC
        unsigned char *pcSuffix = pcInput + txtInputDict[num].dataSize + sizeof(g_pcDFUSuffix);

        WRITE_SHORT(g_usDeviceID, pcSuffix - 16);
        WRITE_SHORT(g_usProductID, pcSuffix - 14);
        WRITE_SHORT(g_usVendorID, pcSuffix - 12);

        lenofdata = txtInputDict[num].dataSize - sizeofDFUPrefix;
        ulFileLen = txtInputDict[num].dataSize + sizeof(g_pcDFUSuffix);

        // Write address and length for program data command
        if (g_pcDFUPrefix[3] == CMD_PROGRAM_DATA) {
            WRITE_LONG(g_ulAddress, pcInput + 4);
            WRITE_LONG(lenofdata, pcInput + 8);
        }

        printf("writing to file %s with address %ld\n", filename, txtInputDict[num].address);

        // Calculate and write CRC
        ulCRC = CalculateCRC32(pcInput, ulFileLen - 4);
        WRITE_LONG(ulCRC, pcSuffix - 4);

        // Write output file
        writeOutputTxt(filename, pcInput, ulFileLen);

        printf("\n");

        // Send file if required
        if (sendFile) {
            printf("sending file: %s\n", filename);
            sendDFUFile(filename);

            // For certain commands, handle response file
            if (currentCommand == CMD_MEMORY_READ_BACK ||
                currentCommand == CMD_GET_IDENTITY ||
                currentCommand == CMD_STANDALONE_VERIFICATION) {

                if (strcmp(g_pszOutput, "image.dfu") == 0) {
                    printf("Please provide output file name for the response file\n");
                    return;
                }
                printf("output file %s\n", g_pszOutput);
                int outputfiletype = checkFileExtension(g_pszOutput);
                uploadRequest(g_pszOutput, outputfiletype);
            }
        }

        num++;
        free(pcInput);
        free(pcSuffix);
    }
}
// Handle binary file as well as commands other than program data with txt file
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
        pcInput= ReadInputFileBin(g_inputFile, &ulFileLen);
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
    if (g_inputFileSize != 0 && g_pcDFUPrefix[3] == CMD_PROGRAM_DATA) {
        WRITE_LONG(g_ulAddress, pcInput + 4);
        WRITE_LONG(ulFileLen - (sizeofDFUPrefix + sizeof(g_pcDFUSuffix)), pcInput + 8);
    }

    // Handle memory read back command
    if (g_pcDFUPrefix[3] == CMD_MEMORY_READ_BACK) {
        readBackAddress = g_ulAddress;
        readBackLength = g_ulLength;
        if (readBackAddress == -1 || readBackLength == -1) {
            printf("Error: Please provide both start address (-a) and length (-l) for memory readback.\n");
            showHelp();
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
            printf("Error: Please provide both start address (-a) and end address (-e) for flash range erase.\n");
            showHelp();
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
            printf("Error: Please provide both verification address (-a) and verification length (-l) for standalone verification.\n");
            showHelp();
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
    printf("The output data written in %s\n", imagefilename);
    for (int i = 0; i < ulFileLen; i++) {
        printf("0x%x ", pcInput[i]);
    }
    printf("\n");

    // Send file if required
    if (sendFile) {
        sendDFUFile(imagefilename);
        if (currentCommand == CMD_MEMORY_READ_BACK ||
            currentCommand == CMD_GET_IDENTITY ||
            currentCommand == CMD_STANDALONE_VERIFICATION) {
            if (strcmp(g_pszOutput, "image.dfu") == 0) {
                printf("Error: Please provide an output file name for the response file using the -o option.\n");
                free(pcInput);
                return;
            }
            printf("Output file %s\n", g_pszOutput);
            int outputfiletype = checkFileExtension(g_pszOutput);
            uploadRequest(g_pszOutput, outputfiletype);
        }
    }
    free(pcInput);
}

int main(int argc, char *argv[]) {
    int iRetcode;

    iRetcode = ParseCommandLine(argc, argv);
    if (argc == 1) return 0;

    fileType = checkFileExtension(g_inputFile);
    // Check for required input file for specific commands
    if ((currentCommand == CMD_PROGRAM_DATA || currentCommand == CMD_UNLOCK_BSL)
        && (g_inputFile == NULL || strlen(g_inputFile) == 0)) {
        printf("Error: You must provide an input file with -i for this command.\n");
        showHelp();
        return 1;
    }

    if (currentCommand == CMD_PROGRAM_DATA || currentCommand == CMD_UNLOCK_BSL || currentCommand == CMD_FACTORY_RESET) {
        if (fileType == fileTypeTxt) {
            handleTxt();
        } else if (fileType == fileTypeBin) {
            if ((strcmp(g_inputFile, "empty.bin") != 0 && currentCommand == CMD_FACTORY_RESET) || currentCommand == CMD_UNLOCK_BSL) {
                printf("You need to provide password in txt format only\n");
                return 1; 
            }
            handleBin();
        } else {
            printf("Unsupported file type.\n");
            return 1;
        }
    } else {      
        handleBin();
    }

    return 0;
}