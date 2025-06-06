/* --COPYRIGHT--,BSD
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
 * --/COPYRIGHT--*/

#include "SMBus_Commands_List.h"

/*! List of implemented commands for this demo
    This demo implements a list of commands but the approach using the t_SMB_Cmd
    structure. This list can be customized to support new commands, or the
    approach to validate commands and execute instructiong can be different.
    For example, a switch-case or conditional statements can be used to
    simplify the code

   SMBus Command types
   - QUICK_COMMAND: (Note: only Write is supported in this implementation )
    S + Target_Addr+R/W*+A + P
   - SEND_BYTE
     S + Target_Addr+W+A + DataByte+A + {PEC+A} + P
   - RECEIVE_BYTE
     S + Target_Addr+R+A + DataByte+A + {PEC+A} + P
   - WRITE_BYTE
     S + Target_Addr+W+A + Cmd+A + DataByte+A + {PEC+A} + P
   - WRITE_WORD
     S + Target_Addr+W+A + Cmd+A + DataByteL+A + DataByteH+A + {PEC+A} + P
   - READ_BYTE
     S + Target_Addr+W+A + Cmd+A +
     Sr + Target_Addr+R+A + DataByte+A + {PEC+A} + P
   - READ_WORD has the following format:
      S + Target_Addr+W+A + Cmd+A +
      Sr + Target_Addr+R+A + DataByteL+A + DataByteH+A + {PEC+A} + P
   - PROCESS_CALL has the following format:
      S + Target_Addr+W+A + Cmd+A + DataByteL+A + DataByteH+A +
      Sr + Target_Addr+R+A + DataByteL+A + DataByteH+A + {PEC+A} + P
   - BLOCK_WRITE has the following format:
      S + Target_Addr+W+A + Cmd+A + ByteCountN+A + DataByte1+A +...+ DataByteN+A + {PEC+A} +P
   - BLOCK_READ has the following format:
      S + Target_Addr+W+A + Cmd+A +
      Sr + Target_Addr+R+A + ByteCountN+A + DataByte1+A +...+ DataByteN+A + {PEC+A} +P
   - BLOCK_WRITE_-BLOCK_READ_PROCESS_CALL has the following format:
      S + Target_Addr+W+A + Cmd+A + ByteCountN+A + DataByte1+A +...+ DataByteN+A +
      Sr + Target_Addr+R+A + ByteCountM+A + DataByte1+A +...+ DataByteM+A + {PEC+A} + P
 */
const t_SMB_Cmd SMB_CMD_List[CMD_LIST_LENGTH] =
{
    /* Command          ControlLED
        Type            WRITE_BYTE
        CMD             0x12
        Length          1 (Fixed)
        Description     Sets LED if 1, Clears LED if 0
     */
    {0x12, 0x00, 0x01, Control_LED_hdlr},
};


