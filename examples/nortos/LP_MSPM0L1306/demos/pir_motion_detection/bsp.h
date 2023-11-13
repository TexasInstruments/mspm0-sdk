// Board Support Package (BSP)
// is part of Hardware Abstraction Layer (HAL). The primary objective of
// the BSP module is to provide one access point for users to modify the
// hardware mapping details without requiring an extensive search and replace
// operation. In order to achieve this objective, the BSP is divided into two
// inter-dependent interfaces: Application Interface and Peripheral Interface.

#ifndef SOURCE_MSPM0PIR_HAL_BSP_H_
#define SOURCE_MSPM0PIR_HAL_BSP_H_

//#   include "gpio.h"
#include "ti_msp_dl_config.h"

// Define on-board LED status, available on MSPM0
//
#   define Red_LED_off     DL_GPIO_setPins(GPIO_RED_LED_PORT, GPIO_RED_LED_PIN_0_PIN)  // PORTA.4
#   define Red_LED_on      DL_GPIO_clearPins(GPIO_RED_LED_PORT, GPIO_RED_LED_PIN_0_PIN)

#   define Green_LED_off   DL_GPIO_setPins(GPIO_GREEN_LED_PORT, GPIO_GREEN_LED_PIN_1_PIN)   // PORTA.23
#   define Green_LED_on    DL_GPIO_clearPins(GPIO_GREEN_LED_PORT, GPIO_GREEN_LED_PIN_1_PIN)
#   define Green_LED_toggle    DL_GPIO_togglePins(GPIO_GREEN_LED_PORT, GPIO_GREEN_LED_PIN_1_PIN)


#define false   0
#define true    1

#endif /* SOURCE_MSPM0PIR_HAL_BSP_H_ */

/*** end of file ***/
