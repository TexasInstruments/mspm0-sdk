/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

#include "bsp/board_api.h"
#ifdef __USE_SYSCONFIG__
#include "ti_msp_dl_config.h"
#else
#include "msp.h"
#include "dl_common.h"
#include "board.h"
#endif

#define POWER_STARTUP_DELAY                    (16)
/* Defines for UART at 115200 */
#define UNICOMMUART_IBRD_32_MHZ_115200_BAUD    (21)
#define UNICOMMUART_FBRD_32_MHZ_115200_BAUD    (45)
//--------------------------------------------------------------------+
// Forward USB interrupt events to TinyUSB IRQ Handler
//--------------------------------------------------------------------+
void USBFS0_IRQHandler(void)
{

/*
 * Reading IIDX register to clear the RIS and MIS register
 * TBD is there any other use
 */
  USBFS0->CPU_INT.IIDX;
#if CFG_TUH_ENABLED
  tuh_int_handler(0, true);
#endif
#if CFG_TUD_ENABLED
  tud_int_handler(0);
#endif
}

//--------------------------------------------------------------------+
// MACRO TYPEDEF CONSTANT ENUM
//--------------------------------------------------------------------+
//*****************************************************************************
//
//! Initialize the pins used for LED and button functions.
//! \return None.
//
//*****************************************************************************
void ConfigureGPIO(void)
{
  /* Reset GPIO_LED_TUSB port */
  GPIO_LED_TUSB->GPRCM.RSTCTL = (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETSTKYCLR_CLR |
                                 GPIO_RSTCTL_RESETASSERT_ASSERT);

  /* Enable power to GPIO_LED_TUSB port */
  GPIO_LED_TUSB->GPRCM.PWREN = (GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE);

  /* Delay startup by 16 cycles */
  DL_Common_delayCycles(POWER_STARTUP_DELAY);

  /* Initialize GPIO_LED_TUSB as an output */
  IOMUX->SECCFG.PINCM[GPIO_LED_IOMUX_TUSB] = (IOMUX_PINCM_PC_CONNECTED | ((uint32_t) 0x00000001));

  /* Initialize GPIO_BUTTON_TUSB as an input */
  IOMUX->SECCFG.PINCM[GPIO_BUTTON_IOMUX_TUSB] = (IOMUX_PINCM_PC_CONNECTED | ((uint32_t) 0x00000001) |
                                                 IOMUX_PINCM_INENA_ENABLE | IOMUX_PINCM_INV_DISABLE |
                                                 IOMUX_PINCM_PIPU_ENABLE | IOMUX_PINCM_PIPD_DISABLE |
                                                 IOMUX_PINCM_HYSTEN_DISABLE | IOMUX_PINCM_WUEN_DISABLE);

  /* Clearing GPIO_LED_TUSB pin */
  GPIO_LED_TUSB->DOUTCLR31_0 = GPIO_LED_PIN_TUSB;

  /* Enabling GPIO_LED_TUSB pin as an output */
  GPIO_LED_TUSB->DOESET31_0 = GPIO_LED_PIN_TUSB;

#ifdef CFG_TUH_ENABLED
  /*
   * When HOST mode is enabled the device must source the USB clock from
   * an external crystal in combination with the PLL. To utilize the
   * external crystal we must configure our HFXT pins.
   */

  /* Configure HFXIN pin */
  IOMUX->SECCFG.PINCM[GPIO_HFXIN_IOMUX_TUSB] = IOMUX_PINCM_PC_UNCONNECTED;

  /* Configure HFXOUT pin*/
  IOMUX->SECCFG.PINCM[GPIO_HFXOUT_IOMUX_TUSB] = IOMUX_PINCM_PC_UNCONNECTED;
#endif
}

//*****************************************************************************
//
//! Initialize UNICOMM instance for UART functionality.
//! \return None.
//
//*****************************************************************************
void ConfigureUART(void)
{
  /* Reset the UNICOMM instance */
  UNICOMM_TUSB->GPRCM.RSTCTL = (UNICOMM_RSTCTL_KEY_UNLOCK_W | UNICOMM_RSTCTL_RESETSTKYCLR_CLR |
                                UNICOMM_RSTCTL_RESETASSERT_ASSERT);

  /* Enable power to the UNICOMM instance */
  UNICOMM_TUSB->GPRCM.PWREN = (UNICOMM_PWREN_ENABLE_ENABLE | UNICOMM_PWREN_KEY_UNLOCK_W);

  DL_Common_delayCycles(POWER_STARTUP_DELAY);

  /* Configure the UNICOMM instance to use UART */
  UNICOMM_TUSB->IPMODE = UNICOMM_IPMODE_SELECT_UART;

  /* Configure UART TX pin */
  IOMUX->SECCFG.PINCM[GPIO_UART_TX_IOMUX_TUSB] = (GPIO_UART_TX_IO_FUNC_TUSB | IOMUX_PINCM_PC_CONNECTED);

  /* Configure UART RX pin */
  IOMUX->SECCFG.PINCM[GPIO_UART_RX_IOMUX_TUSB] = (GPIO_UART_RX_IO_FUNC_TUSB | IOMUX_PINCM_PC_CONNECTED|
                                                  IOMUX_PINCM_INENA_ENABLE);

  /* Configure UART clock to use BUSCLK 32 MHz*/
  UART_TUSB->CLKSEL = (uint32_t) UNICOMMUART_CLKSEL_BUSCLK_SEL_ENABLE;

  /* Divide the UART clock by 1 */
  UART_TUSB->CLKDIV = (uint32_t) UNICOMMUART_CLKDIV_RATIO_DIV_BY_1;

  /* Disable UART before initialization */
  UART_TUSB->CTL0 &= ~(UNICOMMUART_CTL0_ENABLE_ENABLE);

  /*
  * Updating UART control register
  * MODE: NORMAL
  * RX: ENABLE
  * TX: ENABLE
  * CTS: DISABLE
  * RTS: DISABLE
  */
  DL_Common_updateReg(&UART_TUSB->CTL0, (UNICOMMUART_CTL0_MODE_UART | UNICOMMUART_CTL0_RXE_ENABLE |
                      UNICOMMUART_CTL0_TXE_ENABLE | UNICOMMUART_CTL0_CTSEN_DISABLE | UNICOMMUART_CTL0_RTSEN_DISABLE),
                      (UNICOMMUART_CTL0_RXE_MASK | UNICOMMUART_CTL0_TXE_MASK | UNICOMMUART_CTL0_MODE_MASK |
                      UNICOMMUART_CTL0_RTSEN_MASK | UNICOMMUART_CTL0_CTSEN_MASK));

  /*
  * Updating UART line control register
  * PARITY: NORMAL
  * WORDLENGTH: RX_TX
  * STOPBITS: ONE
  */
  DL_Common_updateReg(&UART_TUSB->LCRH, (UNICOMMUART_LCRH_PEN_DISABLE | UNICOMMUART_LCRH_WLEN_DATABIT8 |
                      UNICOMMUART_LCRH_STP2_DISABLE), (UNICOMMUART_LCRH_PEN_DISABLE | UNICOMMUART_LCRH_EPS_MASK |
                      UNICOMMUART_LCRH_SPS_MASK | UNICOMMUART_LCRH_WLEN_MASK | UNICOMMUART_LCRH_STP2_MASK));

  /* Configure oversampling rate, it will be configured at 16x*/
  DL_Common_updateReg(&UART_TUSB->CTL0, UNICOMMUART_CTL0_HSE_OVS16,
                      UNICOMMUART_CTL0_HSE_MASK);

  /* Configure integer baud rate divisor */
  DL_Common_updateReg(&UART_TUSB->IBRD, UNICOMMUART_IBRD_32_MHZ_115200_BAUD,
                      UNICOMMUART_IBRD_DIVINT_MASK);

  /* Configure fractional baud rate divisor */
  DL_Common_updateReg(&UART_TUSB->FBRD, UNICOMMUART_FBRD_32_MHZ_115200_BAUD,
                      UNICOMMUART_FBRD_DIVFRAC_MASK);

  /* When configuring baud-rate divisor the LCRH must also be modified */
  DL_Common_updateReg(&UART_TUSB->LCRH, (UART_TUSB->LCRH & UNICOMMUART_LCRH_BRK_MASK),
                      UNICOMMUART_LCRH_BRK_MASK);

  /* Enable the UART peripheral */
  UART_TUSB->CTL0 |= UNICOMMUART_CTL0_ENABLE_ENABLE;
}

void board_init(void)
{
  /* Reset the USB peripheral */
  USBFS0->GPRCM.RSTCTL = (USB_RSTCTL_KEY_UNLOCK_W | USB_RSTCTL_RESETSTKYCLR_CLR |
                          USB_RSTCTL_RESETASSERT_ASSERT);

#ifdef __USE_SYSCONFIG__
  SYSCFG_DL_init();
#else
  ConfigureGPIO();

  SystemCoreClockUpdate();

  ConfigureUART();

#if CFG_TUSB_OS == OPT_OS_NONE
  /*
   * Once we have initialized the clock lets initialize Systick.
   * The device will be running at 80 MHz by default so we will
   * set the value of SysTick_Config to 80000 so the period will be 1ms.
   */
  SysTick_Config(80000);
#endif
#endif

  /* Enable power to the USB peripheral */
  USBFS0->GPRCM.PWREN = (USB_PWREN_ENABLE_ENABLE |  USB_PWREN_KEY_UNLOCK_W);

  /*
   * Polling until USB peripheral has been powered on
   * TODO: MSPMSDKUSB-15 - Confirm SYSCTL_RCGCUSB_R0 is implemented within SYSCTL
   */
  while (( USBFS0->GPRCM.PWREN & USB_PWREN_ENABLE_ENABLE ) == 0)
    ;

  /* Polling for status of USB peripheral from SYSCTL module */
  while (((SYSCTL->SOCLOCK.SYSSTATUS) & SYSCTL_SYSSTATUS_USBFS0READY_MASK) !=
          SYSCTL_SYSSTATUS_USBFS0READY_TRUE)
          ;

  /*
   * Depending on whether the USB peripheral is configured as device (CFG_TUD_ENABLED) or
   * host (CFG_TUH_ENABLED) mode we will write their respective enable bit alongside the
   * PHY bit as well to configure the PHY. By configuring the PHY, IOMUX for the DP and DM
   * pins are automatically configured and no further configuration is required.
   */
#if CFG_TUD_ENABLED
  USBFS0->USBMODE |= (USB_USBMODE_DEVICEONLY_ENABLE | USB_USBMODE_PHYMODE_USB);
#else
  USBFS0->USBMODE |= (USB_USBMODE_HOST_ENABLE | USB_USBMODE_PHYMODE_USB);
#endif

  /*
   * TinyUSB will periodically enable/disable interrupts through NVIC which can cause issues with our architecture.
   * To workaround this we will enable the interrupts through IMASK and NVIC will remain enabled throughout.
   */

  /* Clear any pending USB interrupt */
  NVIC_ClearPendingIRQ(USBFS0_INT_IRQn);

  /* Clearing USB interrupts */
  USBFS0->CPU_INT.ICLR = (USB_ICLR_INTRUSB_CLR | USB_ICLR_VUSBPWRDN_CLR);

  /* Clearing out USB interrupt status with read */
  USBFS0->REGISTERS.USBIS;

  /* Enable USB interrupts */
  NVIC_EnableIRQ(USBFS0_INT_IRQn);
}

//--------------------------------------------------------------------+
// Board porting API
//--------------------------------------------------------------------+

void board_led_write(bool state) {
  if (state)
    GPIO_LED_TUSB->DOUTSET31_0 = GPIO_LED_PIN_TUSB;
  else
    GPIO_LED_TUSB->DOUTCLR31_0 = GPIO_LED_PIN_TUSB;
}

int board_uart_read(uint8_t * buf, int len)
{
  /*
   * This api will read the status to see if the RXFIFO
   * is empty, if empty then return. Otherwise if there
   * is something add it to the buffer.
   */
  if (UART_TUSB->STAT & UNICOMMUART_STAT_RXFE_MASK)
    return 0;

  for (int i = 0; i < len; ++i) {
    *buf++ = (UART_TUSB->RXDATA & UNICOMMUART_RXDATA_DATA_MASK);
  }

  return len;
}

int board_uart_write(void const * buf, int len)
{
  /*
   * This api will poll until the TXDATA register is
   * empty. Once empty only then will data be written
   * to the TXDATA register. Upon completion length will
   * be returned.
   */
  uint8_t const *p = (uint8_t const *)buf;
  for (int i = 0; i < len; ++i) {
    while (UART_TUSB->STAT & UNICOMMUART_STAT_TXFF_MASK);
    UART_TUSB->TXDATA = *p++;
  }
  return len;
}

#if CFG_TUSB_OS  == OPT_OS_NONE
volatile uint32_t system_ticks = 0u;
void SysTick_Handler(void)
{
  system_ticks++;
}

uint32_t board_millis(void) {
  return system_ticks;
}
#endif

uint32_t board_button_read(void)
{
  /*
   * The button pin is configured as high when idle and low
   * when there is user input. So this function will return
   * 0u when button is idle and 1u when button is pressed.
   */
  return (GPIO_BUTTON_TUSB->DIN31_0 & GPIO_BUTTON_PIN_TUSB) ? 0u : 1u;
}