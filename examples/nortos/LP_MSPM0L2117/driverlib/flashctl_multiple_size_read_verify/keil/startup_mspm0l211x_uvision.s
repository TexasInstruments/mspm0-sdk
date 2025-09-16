;//*****************************************************************************
;//
;// Copyright (C) 2022 Texas Instruments Incorporated - http://www.ti.com/
;//
;// Redistribution and use in source and binary forms, with or without
;// modification, are permitted provided that the following conditions
;// are met:
;//
;//  Redistributions of source code must retain the above copyright
;//  notice, this list of conditions and the following disclaimer.
;//
;//  Redistributions in binary form must reproduce the above copyright
;//  notice, this list of conditions and the following disclaimer in the
;//  documentation and/or other materials provided with the
;//  distribution.
;//
;//  Neither the name of Texas Instruments Incorporated nor the names of
;//  its contributors may be used to endorse or promote products derived
;//  from this software without specific prior written permission.
;//
;// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
;// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
;// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
;// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
;// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
;// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;//
;// MSPM0L2117 startup file
;//
;//****************************************************************************
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000100

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     SYSCTL_IRQHandler         ;  1:  SYSCTL interrupt handler
                DCD     DEBUGSS_IRQHandler        ;  2:  DEBUGSS interrupt handler
                DCD     UC6_IRQHandler            ;  4:  UC6 interrupt handler
                DCD     ADC0_IRQHandler           ;  5:  ADC0 interrupt handler
                DCD     TIMB0_IRQHandler          ;  6:  TIMB0 interrupt handler
                DCD     TIMB1_IRQHandler          ;  7:  TIMB1 interrupt handler
                DCD     COMP0_IRQHandler          ;  8:  COMP0 interrupt handler
                DCD     UC7_IRQHandler            ;  9:  UC7 interrupt handler
                DCD     UC8_IRQHandler            ;  10:  UC8 interrupt handler
                DCD     TIMB2_IRQHandler          ;  14:  TIMB2 interrupt handler
                DCD     TIMB3_IRQHandler          ;  15:  TIMB3 interrupt handler
                DCD     UC4_IRQHandler            ;  16:  UC4 interrupt handler
                DCD     TIMG14_IRQHandler         ;  17:  TIMG14 interrupt handler
                DCD     TIMG2_IRQHandler          ;  18:  TIMG2 interrupt handler
                DCD     TIMA0_IRQHandler          ;  19:  TIMGA0 interrupt handler
                DCD     TIMG1_IRQHandler          ;  20:  TIMG1 interrupt handler
                DCD     GPIOA_IRQHandler          ;  23:  GPIOA interrupt handler
                DCD     GPIOB_IRQHandler          ;  24:  GPIOB interrupt handler
                DCD     UC11_IRQHandler           ;  25:  UC11 interrupt handler
                DCD     AESADV_IRQHandler         ;  27:  AESADV interrupt handler
                DCD     FLASHCTL_IRQHandler       ;  28:  FLASHCTL interrupt handler
                DCD     LCD_IRQHandler            ;  29:  LCD interrupt handler
                DCD     WWDT0_IRQHandler          ;  30:  WWDT0 interrupt handler
                DCD     LFSS_IRQHandler           ;  31:  LFSS interrupt handler
                DCD     DMA_IRQHandler            ;  32:  DMA interrupt handler

__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                ;IMPORT  SystemInit
                IMPORT  __main
				; SystemInit can be called here, but not necessary for MSPM0
                ;LDR     R0, =SystemInit
                ;BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC
                EXPORT  Default_Handler           [WEAK]
                EXPORT  SYSCTL_IRQHandler         [WEAK]
                EXPORT  DEBUGSS_IRQHandler        [WEAK]
                EXPORT  UC6_IRQHandler            [WEAK]
                EXPORT  ADC0_IRQHandler           [WEAK]
                EXPORT  TIMB0_IRQHandler          [WEAK]
                EXPORT  TIMB1_IRQHandler          [WEAK]
                EXPORT  COMP0_IRQHandler          [WEAK]
                EXPORT  UC7_IRQHandler            [WEAK]
                EXPORT  UC8_IRQHandler            [WEAK]
                EXPORT  TIMB2_IRQHandler          [WEAK]
                EXPORT  TIMB3_IRQHandler          [WEAK]
                EXPORT  UC4_IRQHandler            [WEAK]
                EXPORT  TIMG14_IRQHandler         [WEAK]
                EXPORT  TIMG2_IRQHandler          [WEAK]
                EXPORT  TIMA0_IRQHandler          [WEAK]
                EXPORT  TIMG1_IRQHandler          [WEAK]
                EXPORT  GPIOA_IRQHandler          [WEAK]
                EXPORT  GPIOB_IRQHandler          [WEAK]
                EXPORT  UC11_IRQHandler           [WEAK]
                EXPORT  AESADV_IRQHandler         [WEAK]
                EXPORT  FLASHCTL_IRQHandler       [WEAK]
                EXPORT  LCD_IRQHandler            [WEAK]
                EXPORT  WWDT0_IRQHandler          [WEAK]
                EXPORT  LFSS_IRQHandler           [WEAK]
                EXPORT  DMA_IRQHandler            [WEAK]

SYSCTL_IRQHandler
DEBUGSS_IRQHandler
UC6_IRQHandler
ADC0_IRQHandler
TIMB0_IRQHandler
TIMB1_IRQHandler
COMP0_IRQHandler
UC7_IRQHandler
UC8_IRQHandler
TIMB2_IRQHandler
TIMB3_IRQHandler
UC4_IRQHandler
TIMG14_IRQHandler
TIMG2_IRQHandler
TIMA0_IRQHandler
TIMG1_IRQHandler
GPIOA_IRQHandler
GPIOB_IRQHandler
UC11_IRQHandler
AESADV_IRQHandler
FLASHCTL_IRQHandler
LCD_IRQHandler
WWDT0_IRQHandler
LFSS_IRQHandler
DMA_IRQHandler
                B       .
                ENDP

                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

__initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END
