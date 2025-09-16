/*
 * Copyright (c) 2021, Texas Instruments Incorporated
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

#include "ti_msp_dl_config.h"
#include <stdio.h>
#include <string.h>

/* Number of bytes to write to target for configuration. */
#define I2C_TX_PACKET_SIZE_CONFIGURATION_HDC (3)
#define I2C_TX_PACKET_SIZE_CONFIGURATION_OPT (3)

/* Number of bytes to write for measurement reading. */
#define I2C_TX_PACKET_SIZE_READING (1)

/*
 * Number of bytes to receive from target.
 * Two bytes of result from HDC2010 and OPT3001.
 */
#define I2C_RX_PACKET_SIZE  (2)

/* I2C Target address */
#define I2C_TARGET_ADDRESS_HDC2010 (0x40)
#define I2C_TARGET_ADDRESS_OPT3001 (0x44)

/* Measurement configuration code */
uint8_t gTxPacketConfigurationHDC[I2C_TX_PACKET_SIZE_CONFIGURATION_HDC] = {0x0E, 0x60, 0x01};
uint8_t gTxPacketConfigurationOPT[I2C_TX_PACKET_SIZE_CONFIGURATION_OPT] = {0x01, 0xC4, 0x10};

/* Data sent to the Target - measurement result registers*/
uint8_t gTxPacketTemperatureMeasurement[I2C_TX_PACKET_SIZE_READING] = {0x00};
uint8_t gTxPacketHumidityMeasurement[I2C_TX_PACKET_SIZE_READING] = {0x02};
uint8_t gTxPacketLightMeasurement[I2C_TX_PACKET_SIZE_READING] = {0x00};

/* Data received from Target */
volatile uint8_t gRxPacket[I2C_RX_PACKET_SIZE];

uint8_t gExponent;
uint32_t gResult;
uint16_t gHumidity;
uint8_t gHumidityRH;
uint16_t gLightLux;
uint16_t gTemperature;
float gTemperatureCelsius;
uint8_t gTemperatureCelsiusInt;
uint8_t gTemperatureCelsiusFraction; 
uint8_t gValue;
char gBuffer[64];
uint8_t gSecDelay = 10; // delay 30s default

/* Functions */
void configureTarget(const uint8_t *gBuffer, uint16_t count, uint8_t target_addr);
void getMeasurementFromTarget(const uint8_t *gBuffer, uint8_t target_addr);
float HDC2010_tempToFloatCelsius(uint16_t raw);
uint32_t HDC2010_humToIntRelative(uint16_t x);
void uartTransmit(const char *str);
void setMeasurementConfigurationsForSensors();
void takeMeasurements();
void sendToUARTReceiver();


int main(void)
{   
    SYSCFG_DL_init();

    /* Set LED to indicate start of transfer */
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);

    setMeasurementConfigurationsForSensors();

    while (1) 
    {
        takeMeasurements();
        
        /* Interpret values */
        gHumidityRH = HDC2010_humToIntRelative(gHumidity);   
        gTemperatureCelsius = HDC2010_tempToFloatCelsius(gTemperature); 
        gLightLux = 0.01 * pow(2, gExponent) * gResult;
        delay_cycles(24000);

        sendToUARTReceiver();

        /* If write and read were successful, toggle LED every second till next measurement */
        for (uint8_t i = 0; i < gSecDelay; i++) {
            DL_GPIO_togglePins(GPIO_LEDS_PORT,
            GPIO_LEDS_USER_LED_1_PIN);
            delay_cycles(24000000);
        }
    }
}


/*
 *  ======== uartPrint ========
 *  Transmit string via UART.
 */
void uartTransmit(const char *str)
{
    while(*str)
    {
        DL_UART_transmitDataBlocking(UART_0_INST, *str++);  
    }
}

/*
 *  ======== HDC2010_humToIntRelative ========
 *  Convert raw humidity register value to the relative humidity rounded
 *  to the nearest whole number; a range of 0 to 100.
 */
uint32_t HDC2010_humToIntRelative(uint16_t raw)
{
    /* round relative humidity to nearest whole number */
    return ((25 * (uint32_t)raw + 0x2000) >> 14);
}

/*
 *  ======== HDC2010_tempToFloatCelsius ========
 *  Convert temperature to celsius.
 */
float HDC2010_tempToFloatCelsius(uint16_t x)
{
    return ((float)x * (165.0f / 65536.0f) - 40.0f);
}

/*
 *  ======== configureTarget ========
 *  Send configuration code to target.
 */
void configureTarget(const uint8_t *gBuffer, uint16_t count, uint8_t target_addr)
{   
    /* Fill FIFO with data to write configuration code */
    DL_I2C_fillControllerTXFIFO(I2C_0_INST, gBuffer, count);
            
    /* Wait for I2C to be Idle */
    while (!(
        DL_I2C_getControllerStatus(I2C_0_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));

    /* Send the packet to the Target to configure.
    * This function will send Start + Stop automatically.
    */
    DL_I2C_startControllerTransfer(I2C_0_INST, target_addr,
        DL_I2C_CONTROLLER_DIRECTION_TX, count);
}

void getMeasurementFromTarget(const uint8_t *gBuffer, uint8_t target_addr)
{   
    /* Fill FIFO with data to read measurement */
    DL_I2C_fillControllerTXFIFO(I2C_0_INST, gBuffer,
        I2C_TX_PACKET_SIZE_READING);

    /* Wait for I2C to be Idle */
    while (!(
        DL_I2C_getControllerStatus(I2C_0_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;
    
    /* Send the packet to the Target.
    * This function will send Start + Stop automatically.
    */
    DL_I2C_startControllerTransfer(I2C_0_INST, target_addr,
        DL_I2C_CONTROLLER_DIRECTION_TX, I2C_TX_PACKET_SIZE_READING);

    /* Poll until the Controller writes all bytes */
    while (DL_I2C_getControllerStatus(I2C_0_INST) &
        DL_I2C_CONTROLLER_STATUS_BUSY_BUS)
        ;

    /* Trap if there was an error */
    if (DL_I2C_getControllerStatus(I2C_0_INST) &
        DL_I2C_CONTROLLER_STATUS_ERROR) {
        /* LED will remain high if there is an error */
        DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        __BKPT(0);
    }

    /* Add delay between transfers */
    delay_cycles(1200);

    /* Wait for I2C to be Idle */
    while (!(
        DL_I2C_getControllerStatus(I2C_0_INST) & DL_I2C_CONTROLLER_STATUS_IDLE))
        ;

    /* Send a read request to Target */
    DL_I2C_startControllerTransfer(I2C_0_INST, target_addr,
        DL_I2C_CONTROLLER_DIRECTION_RX, I2C_RX_PACKET_SIZE);

    /*
    * Receive all bytes from target. LED will remain high if not all bytes
    * are received
    */
    DL_GPIO_clearPins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
    for (uint8_t i = 0; i < I2C_RX_PACKET_SIZE; i++) {
        while (DL_I2C_isControllerRXFIFOEmpty(I2C_0_INST))
            ;
        gRxPacket[i] = DL_I2C_receiveControllerData(I2C_0_INST);
        
    }
}

void setMeasurementConfigurationsForSensors()
{   
    /* Measurement configuration for HDC2010 */
    configureTarget(&gTxPacketConfigurationHDC[0],
    I2C_TX_PACKET_SIZE_CONFIGURATION_HDC, I2C_TARGET_ADDRESS_HDC2010);

    delay_cycles(4800);

    /* Measurement configuration for OPT3001 */
    configureTarget(&gTxPacketConfigurationOPT[0],
    I2C_TX_PACKET_SIZE_CONFIGURATION_OPT, I2C_TARGET_ADDRESS_OPT3001);

    delay_cycles(4800);
}

void takeMeasurements()
{   
    /* TEMPERATURE MEASUREMENT */
    getMeasurementFromTarget(&gTxPacketTemperatureMeasurement[0], I2C_TARGET_ADDRESS_HDC2010);
    gTemperature = (gRxPacket[1] << 8) | gRxPacket[0]; 
    delay_cycles(100);
    
    /* HUMIDITY MEASUREMENT */
    getMeasurementFromTarget(&gTxPacketHumidityMeasurement[0], I2C_TARGET_ADDRESS_HDC2010);
    gHumidity = (gRxPacket[1] << 8) | gRxPacket[0];
    delay_cycles(100);

    // /* AMBIENT LIGHT MEASUREMENT */
    getMeasurementFromTarget(&gTxPacketLightMeasurement[0], I2C_TARGET_ADDRESS_OPT3001);
    gExponent = (gRxPacket[0] >> 4) & 0x0F;
    gValue = gRxPacket[0] & 0x0F;
    gResult  = (gValue << 8) | gRxPacket[1];
    delay_cycles(100);
}

void sendToUARTReceiver()
{   
    gTemperatureCelsiusInt = (int)gTemperatureCelsius;
    gTemperatureCelsiusFraction = (int)((gTemperatureCelsius - gTemperatureCelsiusInt) * 10);
    snprintf(gBuffer, sizeof(gBuffer),
        "Temperature: %d.%d C   |   ", gTemperatureCelsiusInt, gTemperatureCelsiusFraction);
    uartTransmit(gBuffer);
    snprintf(gBuffer, sizeof(gBuffer), "Humidity: %d RH   |   ", gHumidityRH);
    uartTransmit(gBuffer);
    snprintf(gBuffer, sizeof(gBuffer),"Ambient light: %d lux\r\n", gLightLux);
    uartTransmit(gBuffer);
}
