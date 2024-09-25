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

/********* Common Functions *********/

// Set delay in microseconds
void delayUS(uint16_t us);

// Copy Array
void CopyArray(uint8_t *source, uint8_t *dest, uint8_t count);

/********* Communication Functions *********/

// See the TRM or the BQ7690x header file for a full list of Direct Commands
void DirectCommands(uint8_t command, uint16_t data, uint8_t type);

// See the TRM or the BQ7690x header file for a full list of Subcommands
void Subcommands(uint16_t command, uint16_t data, uint8_t type);

// See the TRM or the BQ769x0 header file for a full list of Command-only subcommands
void CommandSubcommands(uint16_t command);

/********* BQ7690x Command Functions *********/

uint16_t BQ7690x_Device_Number();

uint16_t BQ7690x_FW_Version();

uint16_t BQ7690x_HW_Version();

void BQ7690x_Reset_PassQ();

void BQ7690x_Deepsleep();

void BQ7690x_Exit_Deepsleep();

void BQ7690x_Shutdown();

void BQ7690x_Reset();

void BQ7690x_FET_Enable();

void BQ7690x_Seal();

void BQ7690x_SetConfigUpdate();

void BQ7690x_ExitConfigUpdate();

void BQ7690x_Sleep_Enable();

void BQ7690x_Sleep_Disable();

/********* BQ7690x Measurement Commands *********/

int16_t BQ7690x_ReadVoltage(uint8_t command);

void BQ7690x_ReadAllVoltages();

void BQ7690x_ReadCurrent();

void BQ7690x_ReadCC1Current();

void BQ7690x_ReadIntTemperature();

/********* BQ7690x Getter Functions *********/

// Measurements

void BQ7690x_Get_CellVoltages(int16_t *voltages);  // Cell Voltages

int16_t BQ7690x_Get_Stack_Voltage();  // Stack Voltage

int16_t BQ7690x_Get_VSS_Voltage();  // VSS Voltage

int16_t BQ7690x_Get_REG18_Voltage();  // REG18 LDO Voltage

int16_t BQ7690x_Get_CC2_Current();  // CC2 Current

int16_t BQ7690x_Get_CC1_Current();  // CC1 Current

uint16_t BQ7690x_Get_IntTemp();  // Internal Temperature

uint16_t BQ7690x_Get_RX_data();  // Read buffer for direct commands
