/********************************************************************************
  * @file    touch.h
  * @author  ImagineOn cxOS development team
  *          Copyright (c) 2020 ImagineOn GmbH. All rights reserved.
  * @brief   Cacacitive touch HAL module driver.
  ********************************************************************************/

#ifndef _COLDWAVEOS_TOUCH_H
#define _COLDWAVEOS_TOUCH_H

#include <inttypes.h>
#include <driver.h>

#if defined (__cplusplus)
extern "C" {
#endif

/**
 * @defgroup drv_touch Touch
 * @brief Capacitive Touch Driver
 *
 * Sysprops
 * --------
 * The capacitive touch driver does not have any sysprops defined.
 *
 * @{
 */

#ifndef EXCLUDE_FROM_DOCS
struct touch_driver
{
	struct driver drv;
    int (*touch_sample_gpio) (struct device *, int);
    int (*touch_reset_gpio) (struct device *, int);
    };
#endif

///
/// \param hDev device handle as returned by \ref open(const char*)
/// \param gpio_nr the gpio to sample
/// \return
CW_DRIVER_FUNC touch_sample_gpio (int hDev, int gpio_nr)
CW_DRIVER_CALL(touch, sample_gpio, hDev, gpio_nr);

///
/// \param hDev device handle as returned by \ref open(const char*)
/// \return
CW_DRIVER_FUNC touch_reset_gpio (int hDev, int gpio_nr)
CW_DRIVER_CALL(touch, reset_gpio, hDev, gpio_nr);

/**  @} */


#if defined (__cplusplus)
}
#endif

#endif //_COLDWAVEOS_TOUCH_H
