/*
 * Copyright (c) 2015-2023, Texas Instruments Incorporated
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
/*!****************************************************************************
 *  @file       GPIO.h
 *
 *  @brief      General Purpose I/O driver interface.
 *
 *  @defgroup   GPIO General Purpose Input Output (GPIO)
 *
 *  The GPIO header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/GPIO.h>
 *  @endcode
 *
 *  @anchor ti_drivers_GPIO_Overview
 *  # Overview
 *  The GPIO module allows you to manage General Purpose I/O pins via simple
 *  and portable APIs. GPIO pin behavior is usually configured statically,
 *  but can also be configured or reconfigured at runtime.
 *
 *  Because of its simplicity, the GPIO driver does not follow the model of
 *  other RTOS drivers in which a driver application interface has
 *  separate device-specific implementations. This difference is most
 *  apparent in the GPIOxxx_Config structure, which does not require you to
 *  specify a particular function table or object.
 *
 *  # Usage #
 *  This section provides a basic \ref ti_drivers_GPIO_Synopsis
 *  "usage summary" and a set of \ref ti_drivers_GPIO_Examples "examples"
 *  in the form of commented code fragments.  Detailed descriptions of the
 *  GPIO APIs and their effect are provided in subsequent sections.
 *
 *  @anchor ti_drivers_GPIO_Synopsis
 *  ### Synopsis #
 *  @anchor ti_drivers_GPIO_Synopsis_Code
 *  @code
 *  // Import GPIO Driver definitions.
 *  #include <ti/drivers/GPIO.h>
 *
 *  // Define names for GPIO pin indexes.
 *  #define BUTTON 0
 *  #define LED    1
 *
 *  // One-time init of GPIO driver.
 *  GPIO_init();
 *
 *  // Read GPIO pin
 *  unsigned int state = GPIO_read(BUTTON);
 *
 *  // Write to GPIO pin
 *  GPIO_write(LED, state);
 *  @endcode
 *
 *  @anchor ti_drivers_GPIO_Examples
 *  ### Examples #
 *  * @ref ti_drivers_GPIO_Example_callback "Creating an input callback"
 *  * @ref ti_drivers_GPIO_Example_reconfigure "Runtime pin configuration"
 *
 *  @anchor ti_drivers_GPIO_Example_callback
 *  **Creating an input callback**: The following example demonstrates how
 *  to configure a GPIO pin to generate an interrupt and how to toggle an
 *  an LED on and off within the registered interrupt callback function.
 *  @code
 *  // Driver header file
 *  #include <ti/drivers/GPIO.h>
 *
 *  // TI Drivers Configuration
 *  #include "ti_drivers_config.h"
 *  // Board file
 *  #include <ti/drivers/Board.h>
 *
 *  // GPIO button call back function
 *  void gpioButton0Fxn(uint_least8_t index);
 *
 *  main()
 *  {
 *      // One-time Board initialization.
 *      Board_init();
 *
 *      // One-time init of GPIO driver.
 *      GPIO_init();
 *
 *      // Turn on user LED.
 *      GPIO_write(CONFIG_GPIO_LED0, CONFIG_GPIO_LED_ON);
 *
 *      // install Button callback.
 *      GPIO_setCallback(CONFIG_GPIO_BUTTON0, gpioButton0Fxn);
 *
 *      // Enable interrupts.
 *      GPIO_enableInt(CONFIG_GPIO_BUTTON0);
 *  }
 *
 *  //
 *  //  ======== gpioButton0Fxn ========
 *  //  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON0
 *  //
 *  //  Note: index is the GPIO id for the button which is not used here
 *  //
 *  void gpioButton0Fxn(uint_least8_t index)
 *  {
 *      // Toggle the LED.
 *      GPIO_toggle(CONFIG_GPIO_LED0);
 *  }
 *  @endcode
 *
 *  @anchor ti_drivers_GPIO_Example_reconfigure
 *  **Runtime pin configuration**: The following example demonstrates how
 *  to (re)configure GPIO pins.
 *  @code
 *  // Driver header file.
 *  #include <ti/drivers/GPIO.h>
 *
 *  // TI Driver configuration.
 *  #include "ti_drivers_config.h"
 *
 *  #define LED    CONFIG_GPIO_LED0
 *  #define BUTTON CONFIG_GPIO_BUTTON0
 *
 *  void main()
 *  {
 *      // One-time init of GPIO driver.
 *      GPIO_init();
 *
 *      // Configure a button input pin.
 *      GPIO_setConfig(BUTTON, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING |
 *          BUTTON_IOMUX);
 *
 *      // Configure an LED output pin.
 *      GPIO_setConfig(LED, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW | LED_IOMUX);
 *  }
 *  @endcode
 *
 *  ### GPIO Driver Configuration #
 *
 *  In order to use the GPIO APIs, the application is required
 *  to provide 3 structures in the ti_drivers_config.c file:
 *  1.  An array of @ref GPIO_PinConfig elements that defines the
 *  initial configuration of each pin used by the application. A
 *  pin is referenced in the application by its corresponding index in this
 *  array. The pin type (that is, INPUT/OUTPUT), its initial state (that is
 *  OUTPUT_HIGH or LOW), interrupt behavior (RISING/FALLING edge, etc.), and
 *  device specific pin identification are configured in each element
 *  of this array (see @ref GPIO_PinConfigSettings).
 *  Below is a device specific example of the GPIO_PinConfig array:
 *  @code
 *  //
 *  // Array of Pin configurations.
 *  //
 * GPIO_PinConfig gpioPinConfigs[31] = {
 *     GPIO_CFG_INPUT | PA0_IOMUX, // PA0
 *     GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_NONE | PA1_IOMUX, // PA1
 *     GPIO_CFG_INPUT | PA2_IOMUX, // PA2
 *     GPIO_CFG_INPUT | PA3_IOMUX, // PA3
 *     ...
 * };
 *  @endcode
 *
 *  2.  An array of @ref GPIO_CallbackFxn elements that is used to store
 *  callback function pointers for GPIO pins configured with interrupts.
 *  The indexes for these array elements correspond to the pins defined
 *  in the GPIO_pinConfig array. These function pointers can be defined
 *  statically by referencing the callback function name in the array
 *  element, or dynamically, by setting the array element to NULL and using
 *  GPIO_setCallback() at runtime to plug the callback entry.
 *  Pins not used for interrupts can be omitted from the callback array to
 *  reduce memory usage (if they are placed at the end of GPIO_pinConfig
 *  array). The callback function syntax should match the following:
 *  @code
 *  void (*GPIO_CallbackFxn)(uint_least8_t index);
 *  @endcode
 *  The index parameter is the same index that was passed to
 *  GPIO_setCallback(). This allows the same callback function to be used
 *  for multiple GPIO interrupts, by using the index to identify the GPIO
 *  that caused the interrupt.
 *  @remark Callback functions are called in the context of an interrupt
 *  service routine and should be designed accordingly.
 *
 *  When an interrupt is triggered, the interrupt status of all
 *  (interrupt enabled) pins on a port will be read, cleared, and the
 *  respective callbacks will be executed. Callbacks will be called in order
 *  from least significant bit to most significant bit.
 *  Below is a device specific example of the GPIO_CallbackFxn array:
 *  @code
 *  //
 *  // Array of callback function pointers.
 *  //
 *  GPIO_CallbackFxn gpioCallbackFunctions[31] = {
 *      NULL, // PA0
 *      NULL, // PA1
 *      myGpioCallback, // PA2
 *      NULL, // PA3
 *      ...
 *  };
 *  @endcode
 *
 *  3.  A device specific GPIO_Config structure that tells the GPIO
 *  driver where the two aforementioned arrays are. The interrupt priority of
 *  all pins configured to generate interrupts is also specified here.
 *  Values for the interrupt priority are device-specific. You should be
 *  well-acquainted with the interrupt controller used in your device before
 *  setting this parameter to a non-default value. The sentinel value of
 *  (~0) (the default value) is used to indicate that the lowest possible
 *  priority should be used. Below is a device specific example of a
 *  GPIO_Config structure:
 *  @code
 *  //
 *  // ======== GPIO_config ========
 *  //
 * const GPIO_Config GPIO_config = {
 *     .configs = (GPIO_PinConfig *)gpioPinConfigs,
 *     .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
 *     .intPriority = (~0)
 * };
 *  @endcode
 *
 *  ### Initializing the GPIO Driver #
 *
 *  GPIO_init() must be called before any other GPIO APIs.  This function
 *  configures each GPIO pin in the user-provided @ref GPIO_PinConfig
 *  array according to the defined settings. The user can also reconfigure
 *  a pin dynamically after GPIO_init() is called by using the
 *  GPIO_setConfig(), and GPIO_setCallback() APIs.
 *
 *  # Implementation #
 *
 *  Unlike most other RTOS drivers, the GPIO driver has no generic function
 *  table with pointers to device-specific API implementations. All the generic
 *  GPIO APIs are implemented by the device-specific GPIO driver module.
 *  Additionally, there is no notion of an instance 'handle' with the GPIO
 *  driver.
 *
 *  GPIO pins are referenced by their numeric index in the GPIO_PinConfig
 *  array.  This design approach was used to enhance runtime and memory
 *  efficiency.
 *
 ******************************************************************************
 */
/** @addtogroup GPIO
 * @{
 */
#ifndef ti_drivers_GPIO__include
#define ti_drivers_GPIO__include

#include <stdint.h>

#include <ti/drivers/gpio/GPIOMSPM0.h>

/* clang-format off */

/* Generic functions for converting pin indexes to and from masks. Internal use
 * only. CLZ is an ARM instruction for `count leading zeroes`, so if multiple
 * bits in the pinmask are set MASK_TO_PIN will only return the highest set
 * bit. PIN_TO_MASK is used for setting registers.
 */
#if defined(__IAR_SYSTEMS_ICC__)
    #include <intrinsics.h>
    #define GPIO_MASK_TO_PIN(pinmask) (31 - __CLZ(pinmask))
#elif defined(__TI_COMPILER_VERSION__)
    #include <arm_acle.h>
    #define GPIO_MASK_TO_PIN(pinmask) (31 - __clz(pinmask))
#elif defined(__GNUC__) && !defined(__TI_COMPILER_VERSION__)
    #include <arm_acle.h>
    #define GPIO_MASK_TO_PIN(pinmask) (31 - __builtin_clz(pinmask))
#endif

/*!
 *  @brief      Convert the pin index to mask.
 */
#define GPIO_PIN_TO_MASK(pin) (1 << (pin))

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @defgroup GPIO_STATUS GPIO status codes
 * These macros are general status codes returned by GPIO driver APIs.
 *  @{
 */

/*! @cond */
/*!
 * @brief   Common GPIO status code reservation offset.
 *
 * GPIO driver implementations should offset status codes with
 * GPIO_STATUS_RESERVED growing negatively.
 *
 * Example implementation specific status codes:
 * @code
 * #define GPIOTXYZ_STATUS_ERROR1    GPIO_STATUS_RESERVED - 1
 * #define GPIOTXYZ_STATUS_ERROR0    GPIO_STATUS_RESERVED - 0
 * #define GPIOTXYZ_STATUS_ERROR2    GPIO_STATUS_RESERVED - 2
 * @endcode
 */
#define GPIO_STATUS_RESERVED        (-32)
/*! @endcond */

/*!
 * @brief   Successful status code returned by GPI_setConfig().
 *
 * GPI_setConfig() returns GPIO_STATUS_SUCCESS if the API was executed
 * successfully.
 */
#define GPIO_STATUS_SUCCESS         (0)

/*!
 * @brief   Generic error status code returned by GPI_setConfig().
 *
 * GPI_setConfig() returns GPIO_STATUS_ERROR if the API was not executed
 * successfully.
 */
#define GPIO_STATUS_ERROR           (-1)
/** @}*/

/*!
 *  @brief  GPIO pin configuration settings
 *
 *  The meaning of the bits within PinConfig are entirely device-specific
 *  and are typically one-to-one with the hardware register controlling pin
 *  configuration.
 *
 *  Only create and manipulate these values using GPIO_CFG_* defines.
 *
 *  The upper 16 bits of the 32 bit PinConfig is reserved for pin
 *  configuration settings.
 *
 *  The lower 16 bits are reserved for device-specific port/pin
 *  identifications such as IOMUX PINCM.
 */
typedef uint32_t GPIO_PinConfig;

/*!
 * @brief Dummy value for "this pin is not assigned to a GPIO".
 *
 * Not for use in customer software. Some drivers use this value to manage the
 * behaviour of optional pins (e.g. UART flow control, SPI chip select). If you
 * pass this value to any GPIO methods, it will return immediately and no
 * register writes will be performed.
 */
#define GPIO_INVALID_INDEX 0xFF

/*!
 *  @cond NODOC
 *  Internally used configuration bit access macros.
 */
#define GPIO_CFG_IO_MASK           (0x07ff0000)
#define GPIO_CFG_IO_LSB            (16)
#define GPIO_CFG_OUT_TYPE_MASK     (0x00060000)
#define GPIO_CFG_OUT_TYPE_LSB      (17)
#define GPIO_CFG_IN_TYPE_MASK      (0x00060000)
#define GPIO_CFG_IN_TYPE_LSB       (17)
#define GPIO_CFG_OUT_BIT           (19)
#define GPIO_CFG_OUT_STRENGTH_MASK (0x00300000)
#define GPIO_CFG_OUT_STRENGTH_LSB  (20)

#define GPIO_CFG_INVERSION_BIT     (22)
#define GPIO_CFG_INVERSION_MASK    (0x00400000)

#define GPIO_CFG_HYSTERESIS_BIT    (23)
#define GPIO_CFG_HYSTERESIS_MASK   (0x00800000)

#define GPIO_CFG_HIZ_BIT           (24)
#define GPIO_CFG_HIZ_MASK          (0x01000000)

#define GPIO_CFG_WAKEUP_LSB        (25)
#define GPIO_CFG_WAKEUP_MASK       (0x06000000)

#define GPIO_CFG_INT_LSB           (27)
#define GPIO_CFG_INT_MASK          (0x38000000)

/* Bit 30 of GPIO_PinConfig is unused */
/*! @endcond */

/*!
 *  \defgroup GPIO_PinConfigSettings Macros used to configure GPIO pins
 *  @{
 */
/** @name GPIO_PinConfig output pin configuration macros.
 *  @{
 */
/*! @hideinitializer Pin is an output. */
#define GPIO_CFG_OUTPUT            (((uint32_t) 0) << GPIO_CFG_IO_LSB)
/*! @hideinitializer Output pin is actively driven high and low */
#define GPIO_CFG_OUT_STD           (((uint32_t) 0) << GPIO_CFG_IO_LSB)
/*! @hideinitializer Output pin is Open Drain */
#define GPIO_CFG_OUT_OD_NOPULL     (((uint32_t) 2) << GPIO_CFG_IO_LSB)
/*! @hideinitializer Output pin is Open Drain w/ pull up */
#define GPIO_CFG_OUT_OD_PU         (((uint32_t) 4) << GPIO_CFG_IO_LSB)
/*! @hideinitializer Output pin is Open Drain w/ pull dn */
#define GPIO_CFG_OUT_OD_PD         (((uint32_t) 6) << GPIO_CFG_IO_LSB)

/*! @hideinitializer Set output pin strength to low */
#define GPIO_CFG_OUT_STR_LOW       (((uint32_t) 0) << GPIO_CFG_OUT_STRENGTH_LSB)
/*! @hideinitializer Set output pin strength to medium */
#define GPIO_CFG_OUT_STR_MED       GPIO_CFG_DRVSTR_MED_INTERNAL
/*! @hideinitializer Set output pin strength to high */
#define GPIO_CFG_OUT_STR_HIGH      (((uint32_t) 2) << GPIO_CFG_OUT_STRENGTH_LSB)

/*! @hideinitializer Set pin's output to 1. */
#define GPIO_CFG_OUT_HIGH          (((uint32_t) 1) << GPIO_CFG_OUT_BIT)
/*! @hideinitializer Set pin's output to 0. */
#define GPIO_CFG_OUT_LOW           (((uint32_t) 0) << GPIO_CFG_OUT_BIT)
/** @} */

/** @name GPIO_PinConfig input pin configuration macros
 *  @{
 */
/*! @hideinitializer Pin is an input. */
#define GPIO_CFG_INPUT             (((uint32_t) 1) << GPIO_CFG_IO_LSB)
/*! @hideinitializer Input pin with no internal PU/PD */
#define GPIO_CFG_IN_NOPULL         (((uint32_t) 1) << GPIO_CFG_IO_LSB)
/*! @hideinitializer Input pin with internal PU */
#define GPIO_CFG_IN_PU             (((uint32_t) 3) << GPIO_CFG_IO_LSB)
/*! @hideinitializer Input pin with internal PD */
#define GPIO_CFG_IN_PD             (((uint32_t) 5) << GPIO_CFG_IO_LSB)
/** @} */

/** @name GPIO_PinConfig pin inversion configuration macros.
 *  @{
 */
/*! @hideinitializer Input/output values are normal (default) */
#define GPIO_CFG_INVERT_OFF        (((uint32_t) 0) << GPIO_CFG_INVERSION_BIT)
/*! @hideinitializer Input/output values are inverted */
#define GPIO_CFG_INVERT_ON         (((uint32_t) 1) << GPIO_CFG_INVERSION_BIT)
/** @} */

/** @name GPIO_PinConfig pin hysteresis configuration macros.
 *  @{
 */
/*! @hideinitializer Input hysteresis is disabled (default) */
#define GPIO_CFG_HYSTERESIS_OFF    (((uint32_t) 0) << GPIO_CFG_HYSTERESIS_BIT)
/*! @hideinitializer Input hysteresis is enabled */
#define GPIO_CFG_HYSTERESIS_ON     (((uint32_t) 1) << GPIO_CFG_HYSTERESIS_BIT)
/** @} */

/** @name GPIO_PinConfig pin Hi-Z configuration macros.
 *  @{
 */
/*! @hideinitializer Hi-Z is disabled (default) */
#define GPIO_CFG_HIZ_OFF           (((uint32_t) 0) << GPIO_CFG_HIZ_BIT)
/*! @hideinitializer Hi-Z is enabled */
#define GPIO_CFG_HIZ_ON            (((uint32_t) 1) << GPIO_CFG_HIZ_BIT)
/** @} */

/** @name GPIO_PinConfig interrupt configuration macros.
 *  @{
 */
/*! @hideinitializer No Interrupt */
#define GPIO_CFG_IN_INT_NONE       (((uint32_t) 0) << GPIO_CFG_INT_LSB)
/*! @hideinitializer Interrupt on rising edge */
#define GPIO_CFG_IN_INT_RISING     (((uint32_t) 1) << GPIO_CFG_INT_LSB)
/*! @hideinitializer Interrupt on falling edge */
#define GPIO_CFG_IN_INT_FALLING    (((uint32_t) 2) << GPIO_CFG_INT_LSB)
/*! @hideinitializer Interrupt on both edges */
#define GPIO_CFG_IN_INT_BOTH_EDGES (((uint32_t) 3) << GPIO_CFG_INT_LSB)
/*! @hideinitializer Interrupt on low level */
#define GPIO_CFG_IN_INT_LOW        (GPIO_CFG_INT_LOW_INTERNAL)
/*! @hideinitializer Interrupt on high level */
#define GPIO_CFG_IN_INT_HIGH       (GPIO_CFG_INT_HIGH_INTERNAL)
/** @} */

/** @name GPIO_PinConfig wakeup configuration macros.
 *  @{
 */
/*! @hideinitializer This pin will not wake the device up */
#define GPIO_CFG_WAKEUP_DISABLE    (((uint32_t) 0) << GPIO_CFG_WAKEUP_LSB)
/*! @hideinitializer Wakeup when pin changes to 0 */
#define GPIO_CFG_WAKEUP_ON_0       (((uint32_t) 1) << GPIO_CFG_WAKEUP_LSB)
/*! @hideinitializer Wakeup when pin changes to 1 */
#define GPIO_CFG_WAKEUP_ON_1       (((uint32_t) 2) << GPIO_CFG_WAKEUP_LSB)
/** @} */

/** @name Special GPIO_PinConfig configuration macros.
 *  @{
 */
/*!
 *  @brief Use this @ref GPIO_PinConfig definition to inform GPIO_init()
 *  NOT to configure the corresponding pin.
 */
/*! @hideinitializer Do not configure this Pin */
#define GPIO_DO_NOT_CONFIG         (0x80000000)
/** @} */

/** @name GPIO_Mux configuration macros
 *  @brief For additional muxing options, see the directions in the
 *  device-specific GPIO driver.
 *  @{
 */
/*! @hideinitializer Set this pin to be a GPIO (the default) */
#define GPIO_MUX_GPIO              (0x00000001)
/** @} */
/** @} end of GPIO_PinConfigSettings group */

/* clang-format on */

/*!
 *  @brief  GPIO callback function type.
 *
 *  @param      index       GPIO index.  This is the same index that
 *                          was passed to GPIO_setCallback().  This allows
 *                          you to use the same callback function for multiple
 *                          GPIO interrupts, by using the index to identify
 *                          the GPIO that caused the interrupt.
 */
typedef void (*GPIO_CallbackFxn)(uint_least8_t index);

/*!
 *  @brief  GPIO driver configuration structure.
 *
 *  The GPIO_Config struct contains the defaults for pin configuration.
 *
 *  The interrupt priority of all pins configured to generate interrupts
 *  is also specified here. Values for the interrupt priority are
 *  device-specific. You should be well-acquainted with the interrupt
 *  controller used in your device before setting this parameter to a
 *  non-default value. The sentinel value of (~0) (the default value) is
 *  used to indicate that the lowest possible priority should be used.
 */
typedef struct {
    GPIO_PinConfig *configs; /*!< Pointer to an array of pin configurations */
    GPIO_CallbackFxn
        *callbacks;  /*!< Pointer to an array of callback function pointers */
    void **userArgs; /*!< Pointer to an array of user argument pointers */
    uint32_t intPriority; /*!< Interrupt Priority for all pins */
} GPIO_Config;

/*!
 *  @brief      Clear a GPIO pin interrupt flag.
 *
 *  Clears the GPIO interrupt for the specified index.
 *
 *  Note: It is not necessary to call this API within a
 *  callback assigned to a pin.
 *
 *  @param      index       GPIO index
 */
extern void GPIO_clearInt(uint_least8_t index);

/*!
 *  @brief      Disable a GPIO pin interrupt.
 *
 *  Disables interrupts for the specified GPIO index.
 *
 *  @param      index       GPIO index
 */
extern void GPIO_disableInt(uint_least8_t index);

/*!
 *  @brief      Enable a GPIO pin interrupt.
 *
 *  Enables GPIO interrupts for the selected index to occur.
 *
 *  Note:  Prior to enabling a GPIO pin interrupt, make sure
 *  that a corresponding callback function has been provided.
 *  Use the GPIO_setCallback() API for this purpose at runtime.
 *  Alternatively, the callback function can be statically
 *  configured in the GPIO_CallbackFxn array provided.
 *
 *  @param      index       GPIO index
 */
extern void GPIO_enableInt(uint_least8_t index);

/*!
 *  @brief  Initializes the GPIO module.
 *
 *  The pins defined in the application-provided *GPIOXXX_config* structure
 *  are initialized accordingly.
 *
 *  @pre    The GPIO_config structure must exist and be persistent before this
 *          function can be called. This function must also be called before
 *          any other GPIO driver APIs.
 */
extern void GPIO_init();

/*!
 *  @brief      Reads the value of a GPIO pin.
 *
 *  The value returned will either be zero or one depending on the
 *  state of the pin.
 *
 *  Note, this only reads input pin values.
 *
 *  @param      index  GPIO index
 *
 *  @return     0 or 1, depending on the state of the pin.
 */
extern uint_fast8_t GPIO_read(uint_least8_t index);

/*!
 *  @brief      Toggles the current state of a GPIO.
 *
 *  @param      index  GPIO index
 */
extern void GPIO_toggle(uint_least8_t index);

/*!
 *  @brief     Writes the value to a GPIO pin.
 *
 *  @param      index    GPIO index
 *  @param      value    must be either 0 or 1.
 */
extern void GPIO_write(uint_least8_t index, unsigned int value);

/*!
 *  @brief      Bind a callback function to a GPIO pin interrupt.
 *
 *  Associate a callback function with a particular GPIO pin interrupt.
 *
 *  Callbacks can be changed at any time, making it easy to switch between
 *  efficient, state-specific interrupt handlers.
 *
 *  Note: The callback function is called within the context of an interrupt
 *  handler.
 *
 *  Note: This API does not enable the GPIO pin interrupt.
 *  Use GPIO_enableInt() and GPIO_disableInt() to enable
 *  and disable the pin interrupt as necessary.
 *
 *  Note: it is not necessary to call GPIO_clearInt() within a callback.
 *  That operation is performed internally before the callback is invoked.
 *
 *  @param      index       GPIO index
 *  @param      callback    address of the callback function.
 */
extern void GPIO_setCallback(uint_least8_t index, GPIO_CallbackFxn callback);

/*!
 *  @brief      Gets the callback associated with a GPIO pin.
 *
 *  @param      index  GPIO index.
 *
 *  @return     The callback function for the pin or NULL.
 */
extern GPIO_CallbackFxn GPIO_getCallback(uint_least8_t index);

/*!
 *  @brief      Configure the gpio pin.
 *
 *  Dynamically configure a gpio pin to a device specific setting.
 *  For many applications, the pin configurations provided in the static
 *  GPIO_PinConfig array is sufficient.
 *
 *  For input pins with interrupt configurations, a corresponding interrupt
 *  object will be created as needed.
 *
 *  @param      index       GPIO index
 *  @param      pinConfig   device specific pin configuration settings.
 *
 *  @return     GPIO_STATUS_SUCCESS or an error if the pin cannot be configured.
 */
extern int_fast16_t GPIO_setConfig(
    uint_least8_t index, GPIO_PinConfig pinConfig);

/*!
 *  @brief      Configure the gpio pin.
 *
 *  Dynamically configure a gpio pin to a device specific setting.
 *  This variant only allows configuring the interrupt settings (rising edge,
 *  falling edge, etc.) and enabling or disabling interrupts.
 *
 *  Only GPIO_CFG_IN_INT_XXX macros and GPIO_CFG_INT_ENABLE/DISABLE may be
 *  passed to the config parameter for this function. If you do not pass
 *  GPIO_CFG_INT_ENABLE, this function will disable interrupts.
 *
 *  @param      index     GPIO index.
 *  @param      config    pin configuration settings.
 */
extern void GPIO_setInterruptConfig(
    uint_least8_t index, GPIO_PinConfig config);

/*!
 *  @brief      Get the current configuration for a gpio pin.
 *
 *  The pin configuration is provided in the static GPIO_PinConfig array,
 *  but can be changed with GPIO_setConfig().  GPIO_getConfig() gets the
 *  current pin configuration.
 *
 *  @param      index       GPIO index.
 *  @param      pinConfig   Location to store device specific pin
 *                          configuration settings.
 */
extern void GPIO_getConfig(uint_least8_t index, GPIO_PinConfig *pinConfig);

/*!
 *  @brief      Resets the configuration for a gpio pin to the default value.
 *
 *  The default pin configuration is provided in the static GPIO_PinConfig
 *  array, defined by sysconfig or the board file at compile time. Also clears
 *  the callback and user argument.
 *
 *  @param      index       GPIO index.
 */
extern void GPIO_resetConfig(uint_least8_t index);

/*!
 *  @brief      Get the current mux for a gpio pin.
 *
 *  For details and valid mux options, see the device-specific header file.
 *
 *  @param      index       GPIO index.
 *
 *  @return     A device-specific mux value or GPIO_MUX_GPIO.
 */
extern uint32_t GPIO_getMux(uint_least8_t index);

/*!
 *  @brief      Configure the gpio pin's config and mux in a single write.
 *
 *  Dynamically configure a gpio pin to a device specific setting.
 *  For many applications, the pin configurations provided in the static
 *  GPIO_PinConfig array is sufficient.
 *
 *  For some devices, configuring the pin and then muxing it can create a small
 *  drop on the line, which is enough to trigger some communication protocols.
 *  This helper function sets the pin configuration and the mux in a single access.
 *
 *  @param      index       GPIO index.
 *  @param      pinConfig   device specific pin configuration settings.
 *  @param      mux         Device-specific mux value to use a special mode,
 *                          or GPIO_MUX_GPIO to reset the pin to standard IO.
 *
 *  @return     GPIO_STATUS_SUCCESS or an error if the pin cannot be configured.
 */
extern int_fast16_t GPIO_setConfigAndMux(
    uint_least8_t index, GPIO_PinConfig pinConfig, uint32_t mux);

/*!
 *  @brief      Set the user argument for a gpio pin.
 *
 *  This can be retrieved using GPIO_getUserArg() and can be helpful to share
 *  callback logic across different pins.
 *
 *  @param      index       GPIO index.
 *  @param      arg         Pointer to a user object.
 */
void GPIO_setUserArg(uint_least8_t index, void *arg);

/*!
 *  @brief      Get the user argument for a gpio pin.
 *
 *  @param      index       GPIO index.
 *
 *  @return     Pointer to a user object set by GPIO_setUserArg().
 */
void *GPIO_getUserArg(uint_least8_t index);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_GPIO__include */
/** @}*/
