#ifndef PGA460_H_
#define PGA460_H_

typedef unsigned char byte;

// UART and SPI communication
byte calcChecksum(byte cmd);
void spiSendData(byte arr[], byte size);
void spiMosiIdle(byte size);
void uartSend(double num);
void doubleToString(double num, char* str);

//PGA460 REG read and write
void registerWrite(byte addr, byte data);
byte registerRead(byte addr);

//PGA460  initialization and configuration
void defaultPGA460(byte xdcr);
void initTVG(byte agr, byte tvg);
void initThresholds(byte thr);

//PGA460  run function
void ultrasonicCmd(byte cmd, byte numObjUpdate);
void runEchoDataDump(byte preset);
char* pullEchoDataDumpBulk();
double printUltrasonicMeasResult(byte umr);
void pullUltrasonicMeasResult();

#endif /* PGA460_H_ */
