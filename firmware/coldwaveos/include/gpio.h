/********************************************************************************
  * @file    gpio.h
  * @author  ImagineOn cxOS development team
  *          Copyright (c) 2020 ImagineOn GmbH. All rights reserved.
  * @brief   GPIO HAL module driver.
  * @copyright  ImagineoOn GmbH
  ********************************************************************************/

#ifndef _COLDWAVEOS_GPIO_H
#define _COLDWAVEOS_GPIO_H

#include <inttypes.h>
#include <stdbool.h>
#include <driver.h>

#if defined (__cplusplus)
extern "C" {
#endif

/**
* @defgroup drv_gpio GPIO
* @brief GPIO API
* @details A simple interface for setting and reading pin states on the MCU.
 *  It allows to define pin direction and weak pull-up or pull-down as
 *  well as the registration of GPIO interrupts.
 *  Be aware, that the API at the moment does not check, if the pin has been assigned to a
 *  peripheral (like I2C, ADC, ...). So if you access a pin that is otherwise used in
 *  an alternate context you might mess things up.
 *  For the pin identifier, the OS's HAL requirements specify that common Pin Names like "PA3" or "PB5"
 *  shall be translated into pin numbers as follows:
 *  100 * Port (A=0, B=1,...) + Pin. So PA3 becomes 3, PB5 becomes 105.
 *  This is only a recommendation, so check the HAL implementation's docs for compliance.
 *
 * Sysprops
 * --------
 * The GPIO driver does not have any sysprops defined.
 *
 * @{
 */

#define GPIO_PIN(p)   (p % 100U)
#define GPIO_PORT(p)  (p / 100U)

typedef enum {
    gpioIrqFallingEdge = 0,
    gpioIrqRisingEdge,
    gpioIrqBothEdges
} gpio_irq_edge_t;

typedef enum {
    gpioPinDirInput = 0,
    gpioPinDirOutput,
    gpioPinDirOutputOpenDrain
} gpio_pin_dir_t;

typedef enum {
    gpioActiveLow = 0,
    gpioActiveHigh
} gpio_pin_logic_state_t;

typedef enum {
    gpioLogicLow = 0,
    gpioLogicHigh
} gpio_pin_state_t;

typedef enum {
    gpioModePullOff = 0,
    gpioModePullUp,
    gpioModePullDown
} gpio_pin_mode_t;

typedef void (*gpio_irq_cb_t) (void);

#ifndef EXCLUDE_FROM_DOCS
struct gpio_driver {
    struct driver drv;
    cw_driver_return_t (*gpio_get_pin) (struct device *, uint16_t);
    cw_driver_return_t (*gpio_set_pin) (struct device *, uint16_t, gpio_pin_state_t);
    cw_driver_return_t (*gpio_set_pin_mode) (struct device *, uint16_t, gpio_pin_mode_t);
    cw_driver_return_t (*gpio_set_pin_dir) (struct device *, uint16_t, gpio_pin_dir_t);
    cw_driver_return_t (*gpio_register_pin_interrupt) (struct device *, uint16_t, gpio_irq_edge_t, gpio_irq_cb_t);
    cw_driver_return_t (*gpio_unregister_pin_interrupt) (struct device *, uint16_t);
    cw_driver_return_t (*gpio_enable_pin_interrupt) (struct device *, uint16_t);
    cw_driver_return_t (*gpio_disable_pin_interrupt) (struct device *, uint16_t);
    };
#endif

CW_DRIVER_FUNC
gpio_get_pin (int hDev, uint16_t gpio)
CW_DRIVER_CALL(gpio, get_pin, hDev, gpio);

CW_DRIVER_FUNC
gpio_set_pin (int hDev, uint16_t gpio, gpio_pin_state_t state)
CW_DRIVER_CALL(gpio, set_pin, hDev, gpio, state);

CW_DRIVER_FUNC
gpio_set_pin_mode (int hDev, uint16_t gpio, gpio_pin_mode_t mode)
CW_DRIVER_CALL(gpio, set_pin_mode, hDev, gpio, mode);

CW_DRIVER_FUNC
gpio_set_pin_dir (int hDev, uint16_t gpio, gpio_pin_dir_t dir)
CW_DRIVER_CALL(gpio, set_pin_dir, hDev, gpio, dir);

CW_DRIVER_FUNC
gpio_register_pin_interrupt (int hDev, uint16_t gpio, gpio_irq_edge_t edge_type, gpio_irq_cb_t callback)
CW_DRIVER_CALL(gpio, register_pin_interrupt, hDev, gpio, edge_type, callback);

CW_DRIVER_FUNC
gpio_unregister_pin_interrupt (int hDev, uint16_t gpio)
CW_DRIVER_CALL(gpio, unregister_pin_interrupt, hDev, gpio);

CW_DRIVER_FUNC
gpio_enable_pin_interrupt (int hDev, uint16_t gpio)
CW_DRIVER_CALL(gpio, enable_pin_interrupt, hDev, gpio);

CW_DRIVER_FUNC
gpio_disable_pin_interrupt (int hDev, uint16_t gpio)
CW_DRIVER_CALL(gpio, disable_pin_interrupt, hDev, gpio);


///----
/// GPIO convenience functions for accessing the system GPIO device directly
///

cw_driver_return_t gpio_get (uint16_t pin);

cw_driver_return_t gpio_set (uint16_t pin, gpio_pin_state_t state);

cw_driver_return_t gpio_set_dir (uint16_t gpio, gpio_pin_dir_t dir); // so this declaration returns a simple it, but it has no declaration?? What is going on here.


cw_driver_return_t gpio_set_mode (uint16_t gpio, gpio_pin_mode_t mode);

cw_driver_return_t gpio_register_interrupt (uint16_t gpio, gpio_irq_edge_t edge_type, gpio_irq_cb_t callback);

cw_driver_return_t gpio_unregister_interrupt (uint16_t gpio);

cw_driver_return_t gpio_enable_interrupt (uint16_t gpio);

cw_driver_return_t gpio_disable_interrupt (uint16_t gpio);

/** @} */

#if defined (__cplusplus)
}
#endif

#endif //_COLDWAVEOS_GPIO_H
