/*****************************************************************************

  Copyright (C) 2021 Texas Instruments Incorporated - http://www.ti.com/

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

   Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the
   distribution.

   Neither the name of Texas Instruments Incorporated nor the names of
   its contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

#include <stdint.h>

/*****************************************************************************

 Enable the IAR extensions for this source file.

*****************************************************************************/
#pragma language = extended
#pragma segment  = "CSTACK"

/*****************************************************************************

 Forward declaration of the default fault handlers.

*****************************************************************************/
extern void Reset_Handler(void);
static void Default_Handler(void);

extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void SVC_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);

/* Device specific interrupt handler */
extern void SYSCTL_IRQHandler(void);
extern void DEBUGSS_IRQHandler(void);
extern void UC6_IRQHandler(void);
extern void ADC0_IRQHandler(void);
extern void TIMB0_IRQHandler(void);
extern void TIMB1_IRQHandler(void);
extern void COMP0_IRQHandler(void);
extern void UC7_IRQHandler(void);
extern void UC8_IRQHandler(void);
extern void TIMB2_IRQHandler(void);
extern void TIMB3_IRQHandler(void);
extern void UC4_IRQHandler(void);
extern void TIMG14_IRQHandler(void);
extern void TIMG2_IRQHandler(void);
extern void TIMA0_IRQHandler(void);
extern void TIMG1_IRQHandler(void);
extern void GPIOA_IRQHandler(void);
extern void GPIOB_IRQHandler(void);
extern void UC11_IRQHandler(void);
extern void AESADV_IRQHandler(void);
extern void FLASHCTL_IRQHandler(void);
extern void LCD_IRQHandler(void);
extern void WWDT0_IRQHandler(void);
extern void LFSS_IRQHandler(void);
extern void DMA_IRQHandler(void);

/* Cortex-M0+ Processor Exceptions */
#pragma weak NMI_Handler       = Default_Handler
#pragma weak HardFault_Handler = Default_Handler
#pragma weak SVC_Handler       = Default_Handler
#pragma weak PendSV_Handler    = Default_Handler
#pragma weak SysTick_Handler   = Default_Handler

/* Device specific interrupt handler */
#pragma weak GROUP0_IRQHandler   = Default_Handler
#pragma weak SYSCTL_IRQHandler   = Default_Handler
#pragma weak DEBUGSS_IRQHandler  = Default_Handler
#pragma weak UC6_IRQHandler      = Default_Handler
#pragma weak ADC0_IRQHandler     = Default_Handler
#pragma weak TIMB0_IRQHandler    = Default_Handler
#pragma weak TIMB1_IRQHandler    = Default_Handler
#pragma weak COMP0_IRQHandler    = Default_Handler
#pragma weak UC7_IRQHandler      = Default_Handler
#pragma weak UC8_IRQHandler      = Default_Handler
#pragma weak TIMB2_IRQHandler    = Default_Handler
#pragma weak TIMB3_IRQHandler    = Default_Handler
#pragma weak UC4_IRQHandler      = Default_Handler
#pragma weak TIMG14_IRQHandler   = Default_Handler
#pragma weak TIMG2_IRQHandler    = Default_Handler
#pragma weak TIMA0_IRQHandler    = Default_Handler
#pragma weak TIMG1_IRQHandler    = Default_Handler
#pragma weak GPIOA_IRQHandler    = Default_Handler
#pragma weak GPIOB_IRQHandler    = Default_Handler
#pragma weak UC11_IRQHandler     = Default_Handler
#pragma weak AESADV_IRQHandler   = Default_Handler
#pragma weak FLASHCTL_IRQHandler = Default_Handler
#pragma weak LCD_IRQHandler      = Default_Handler
#pragma weak WWDT0_IRQHandler    = Default_Handler
#pragma weak LFSS_IRQHandler     = Default_Handler
#pragma weak DMA_IRQHandler      = Default_Handler

//*****************************************************************************
//
// The reference for the device initialization.
//
//*****************************************************************************
extern void SystemInit(void);

/*****************************************************************************

 The entry point for the application startup code.

*****************************************************************************/
extern void __iar_program_start(void);

/*****************************************************************************

 A union that describes the entries of the vector table.  The union is needed
 since the first entry is the stack pointer and the remainder are function
 pointers.

*****************************************************************************/
typedef union {
    void (*handler)(void);
    uint32_t ptr;
} uVectorEntry;

/*****************************************************************************

 The vector table.  Note that the proper constructs must be placed on this to
 ensure that it ends up at physical address 0x0000.0000.

*****************************************************************************/
__root const uVectorEntry __vector_table[] @ ".intvec" = {
    {.ptr = (uint32_t) __sfe("CSTACK")}, /* The initial stack pointer */
    Reset_Handler,                       /* The reset handler         */
    NMI_Handler,                         /* The NMI handler           */
    HardFault_Handler,                   /* The hard fault handler    */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    SVC_Handler,                         /* SVCall handler            */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    PendSV_Handler,                      /* The PendSV handler        */
    SysTick_Handler,                     /* SysTick handler           */
    SYSCTL_IRQHandler,                   /* SYSCTL interrupt handler  */
    DEBUGSS_IRQHandler,                  /* DEBUGSS interrupt handler */
    0,                                   /* Reserved                  */
    UC6_IRQHandler,                      /* UC6 interrupt handler     */
    ADC0_IRQHandler,                     /* ADC0 interrupt handler    */
    TIMB0_IRQHandler,                    /* TIMB0 interrupt handler   */
    TIMB1_IRQHandler,                    /* TIMB1 interrupt handler   */
    COMP0_IRQHandler,                    /* COMP0 interrupt handler   */
    UC7_IRQHandler,                      /* UC7 interrupt handler     */
    UC8_IRQHandler,                      /* UC8 interrupt handler     */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    TIMB2_IRQHandler,                    /* TIMB2 interrupt handler   */
    TIMB3_IRQHandler,                    /* TIMB3 interrupt handler   */
    UC4_IRQHandler,                      /* UC4 interrupt handler     */
    TIMG14_IRQHandler,                   /* TIMG14 interrupt handler  */
    TIMG2_IRQHandler,                    /* TIMG2 interrupt handler   */
    TIMA0_IRQHandler,                    /* TIMA0 interrupt handler   */
    TIMG1_IRQHandler,                    /* TIMG1 interrupt handler   */
    0,                                   /* Reserved                  */
    0,                                   /* Reserved                  */
    GPIOA_IRQHandler,                    /* GPIOA interrupt handler   */
    GPIOB_IRQHandler,                    /* GPIOB interrupt handler   */
    UC11_IRQHandler,                     /* UC11 interrupt handler    */
    0,                                   /* Reserved                  */
    AESADV_IRQHandler,                   /* AESADV interrupt handler  */
    FLASHCTL_IRQHandler,                 /* FLASHCTL interrupt handler */
    LCD_IRQHandler,                      /* LCD interrupt handler     */
    WWDT0_IRQHandler,                    /* WWDT0 interrupt handler   */
    LFSS_IRQHandler,                     /* LFSS interrupt handler    */
    DMA_IRQHandler,                      /* DMA interrupt handler     */
};

/*****************************************************************************

 This is the code that gets called when the processor first starts execution
 following a reset event.  Only the absolutely necessary set is performed,
 after which the application supplied entry() routine is called.  Any fancy
 actions (such as making decisions based on the reset cause register, and
 resetting the bits in that register) are left solely in the hands of the
 application.

*****************************************************************************/
__weak void Reset_Handler(void)
{
    /* Initialize the device */
    // SystemInit();

    /* Call the application's entry point. */
    __iar_program_start();
}

/*****************************************************************************

 This is the code that gets called when the processor receives an unexpected
 interrupt.  This simply enters an infinite loop, preserving the system state
 for examination by a debugger.

*****************************************************************************/
static void Default_Handler(void)
{
    /* Go into an infinite loop. */
    while (1) {
    }
}
