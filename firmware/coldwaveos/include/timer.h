/********************************************************************************
  * @file    timer.h
  * @author  ImagineOn cxOS development team
  *          Copyright (c) 2020 ImagineOn GmbH. All rights reserved.
  * @brief   SPI HAL module driver.
  * @copyright  ImagineoOn GmbH
  ********************************************************************************/

#ifndef _COLDWAVEOS_SPI_H
#define _COLDWAVEOS_SPI_H

#include <inttypes.h>
#include <driver.h>

#if defined (__cplusplus)
extern "C" {
#endif


/**
 * @defgroup drv_timer Timer
 * @brief Timer Driver
 * @details Functions for using a hardware timer
 *
 * Sysprops
 * --------
 *
 * sysprop   | description
 * ----------|------------------------------------
 * mode      | the timer mode as per TIMER_MODE_XX define (optional, defaults to TIMER_MODE_COUNT
 * gpio      | the gpio to be used in TIMER_MODE_CC_xx modes
 * channel   | the timer channel (optional, driver dependent)
 *
 * @{
 */


typedef void (*timer_callback_f) (uint32_t value);

#ifndef EXCLUDE_FROM_DOCS
struct timer_driver {
    struct driver drv;
    int (*timer_config) (struct device *, const uint8_t, const uint8_t, const uint32_t);
    int (*timer_start) (struct device *, const uint8_t, const uint32_t, timer_callback_f);
    int (*timer_stop) (struct device *);
};
#endif

#define TIMER_TYPE_PERIODIC   (0)
#define TIMER_TYPE_ONCE       (1)

#define TIMER_MODE_COUNT      (0)
#define TIMER_MODE_CC_RISING  (1)   ///< capture timer on input rising edge
#define TIMER_MODE_CC_FALLING (2)   ///< capture timer on input falling edge
#define TIMER_MODE_CC_OUTPUT  (3)   ///< toggle output at timer frequency

///
/// \param hDev
/// \param type
/// \param freq
/// \param cb callback for input capture or counting mode. set to NULL in CC_OUTPUT mode.
/// \return
CW_DRIVER_FUNC timer_start (int hDev, const uint8_t type, const uint32_t freq, timer_callback_f cb)
CW_DRIVER_CALL(timer, start, hDev, type, freq, cb);

///
/// \param hDev
/// \return
CW_DRIVER_FUNC timer_stop (int hDev)
CW_DRIVER_CALL_VOID(timer, stop, hDev);

///
/// \param hDev
/// \param mode
/// \param channel
/// \param gpio
/// \return
CW_DRIVER_FUNC timer_config (int hDev, const uint8_t mode, const uint8_t channel, const uint32_t gpio)
CW_DRIVER_CALL(timer, config, hDev, mode, channel, gpio);


/**  @} */
 

#if defined (__cplusplus)
}
#endif

#endif //_COLDWAVEOS_SPI_H
