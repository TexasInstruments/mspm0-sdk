/*
 * comp_hal.h
 *
 *  Created on: Jun 4, 2024
 *      Author: a0500536
 */
#include "ti_msp_dl_config.h"

#ifndef COMP_HAL_H_
#define COMP_HAL_H_

//COMP_DACCODE0 set by Sysconfig                                    (39)
#define COMP_DACCODE1                                               (77)
#define COMP_DACCODE2                                               (116)

//IN0+ already defined w/ SysConfig

//IN0-
#define GPIO_COMP_IN0N_PORT                                         (GPIOA)
#define GPIO_COMP_IN0N_PIN                                          (DL_GPIO_PIN_27)
#define GPIO_COMP_IOMUX_IN0N                                        (IOMUX_PINCM28)
#define GPIO_COMP_IOMUX_IN0N_FUNC                                   (IOMUX_PINCM28_PF_UNCONNECTED)

//IN1-
#define GPIO_COMP_IN1N_PORT                                         (GPIOA)
#define GPIO_COMP_IN1N_PIN                                          (DL_GPIO_PIN_23)
#define GPIO_COMP_IOMUX_IN1N                                        (IOMUX_PINCM24)
#define GPIO_COMP_IOMUX_IN1N_FUNC                                   (IOMUX_PINCM24_PF_UNCONNECTED)

//function definitions
void update_comp_configA (void);
void update_comp_configB (void);
void update_comp_configC (void);

#endif /* COMP_HAL_H_ */
