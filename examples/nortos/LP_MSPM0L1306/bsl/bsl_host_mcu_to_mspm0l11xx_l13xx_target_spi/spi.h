#ifndef SPI_H__
#define SPI_H__

#include <stdbool.h>
#include <stdint.h>

void SPI_Initialize(void);
void SPI_SendByte(uint8_t data);
void SPI_SendByteArray(uint8_t *data, uint8_t dataLength);
uint8_t SPI_ReadByte();
void SPI_ReadByteArray(uint8_t *data, uint16_t dataLength);
uint8_t Status_check(void);
void SPI_Close();

#endif
