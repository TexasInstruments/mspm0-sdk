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

#include "dali_gpio_comm.h"

#define IS_IN_RANGE(data, min, max) ((data) >= (min) && (data) <= (max))


bool DALI_RX_decodeFrame(void)
{
    uint8_t halfBits[MAX_CAPTURE_BITS_LENGTH] = {0}, halfBitIndex= 0;

       /* Check if start bit is proper */
       if(!(gDALI_Rx.captBits[0] == 1 && gDALI_Rx.captBits[1] == 0))
           return false;

       /* Check if Bit timings are proper and form Manchester Encoded data */
       for(int i=0; i < gDALI_Rx.captIndex ; i++)
       {
           if(i > 0 && (gDALI_Rx.captBits[i] == gDALI_Rx.captBits[i-1]))
               return false;

           if(IS_IN_RANGE(gDALI_Rx.captBitTimings[i], DALI_RX_HALFBIT_MIN_CYC,DALI_RX_HALFBIT_MAX_CYC))
           {
               halfBits[halfBitIndex++] = gDALI_Rx.captBits[i];
           }

           else if(IS_IN_RANGE(gDALI_Rx.captBitTimings[i], DALI_RX_BIT_MIN_CYC,DALI_RX_BIT_MAX_CYC))
           {
               halfBits[halfBitIndex++] = gDALI_Rx.captBits[i];
               halfBits[halfBitIndex++] = gDALI_Rx.captBits[i];
           }

           else
           {
               return false;
           }
       }

       /* Last bit is not counted if it is high */
       if(halfBitIndex % 2 == 1)
       {
           halfBitIndex++;
       }

       /* Check if it contains all the bits of Forward Frame (start bit + 2 bytes )*/
       if(halfBitIndex != FORWARD_FRAME_HALFBIT_SIZE)
       {
           return false;
       }


       /* Decode Rx Data from Manchester Encoded data*/
       uint16_t RxData = 0;

       for(int i = 2 ; i < halfBitIndex; i += 2)
       {
          if(halfBits[i] == 0 && halfBits[i + 1] == 1)
          {
              RxData = (RxData << 1) | 0 ;
          }

          else
          {
              RxData = (RxData << 1) | 1 ;
          }
       }

       gDALI_Rx.dataArr[1] = RxData & 0xFF ;
       gDALI_Rx.dataArr[0] = (RxData >> 8) & 0xFF;

       return true;
}




