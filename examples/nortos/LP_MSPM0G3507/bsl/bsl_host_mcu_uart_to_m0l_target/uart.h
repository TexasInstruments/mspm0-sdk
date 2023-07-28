#include "stdint.h"

#define TIMEOUT_COUNT 1500000

void Host_BSL_entry_software(void);
uint8_t Status_check(void);
void BSL_sendSingleByte(uint8_t ui8Byte);
uint8_t BSL_getResponse(void);
uint8_t UART_writeBuffer(uint8_t *pData, uint8_t ui8Cnt);
void UART_readBuffer(uint8_t *pData, uint8_t ui8Cnt);
