/********************************************************************************
  * @file    pwm.h
  * @author  ImagineOn cxOS development team
  *          Copyright (c) 2020 ImagineOn GmbH. All rights reserved.
  * @brief   PWM HAL module driver.
  * @copyright  ImagineoOn GmbH
  ********************************************************************************/

#ifndef _COLDWAVEOS_PWM_H
#define _COLDWAVEOS_PWM_H

#include <inttypes.h>
#include <driver.h>

#if defined (__cplusplus)
extern "C" {
#endif

/**
 * @defgroup drv_pwm PWM
 * @brief Pulse Width Modulation API
 * @details The PWM API can be used to control the duty-cycle of a PWM peripheral
 *  Since the timing-parameters are passed in microseconds,
 *  the maximum meaningful PWM frequency would be in the range of 100kHz, allowing
 *  for 100 different PWM "steps".
 *  Functionality like inverting the PWM signal and setting the maximum frequency,
 *  if available from the HAL driver, is provided via SysConfig parameters.
 *
 * Sysprops
 *
 * sysprop   | description
 * ----------|------------------------------------
 * polarity_invert  | set this to 1 to have the PWM generate an active-low signal
 * @{
 */

typedef float pwm_duty_percent_t;

#ifndef EXCLUDE_FROM_DOCS
struct pwm_driver
    {
    struct driver drv;
    int (*pwm_set_duty_cycle) (struct device *, uint16_t, pwm_duty_percent_t, uint32_t);
    int (*pwm_enable) (struct device *, uint16_t);
    int (*pwm_disable) (struct device *, uint16_t);
    };
#endif

///
/// \brief enable the PWM channel
/// \details when enabled, the pin will output the PWM signal configured by a previous call to \ref pwm_set_duty_cycle
///    or to "0%" if the channel wasn't configured before.
/// \param hDev device handle as returned by \ref open(const char*)
/// \param channel some PWM peripheral icm20649 differentiate different pin outputs via channels.
///     the pin-to-channel assignments will be set in the SysConf.
///     If this is not required by the HAL driver, simply pass 0 here.
CW_DRIVER_FUNC
pwm_enable (int hDev, uint16_t channel)
CW_DRIVER_CALL(pwm, enable, hDev, channel);

///
/// \brief disable the PWM output.
/// \details disabling the output will effectively result in a low level on the pin,
///     or if the driver supports inversion and it is enabled in the SysConf a high level, respectively.
/// \param hDev  device handle as returned by \ref open(const char*)
/// \param channel some PWM peripheral icm20649 differentiate different pin outputs via channels.
///     the pin-to-channel assignments will be set in the SysConf.
///     If this is not required by the HAL driver, simply pass 0 here.
CW_DRIVER_FUNC
pwm_disable (int hDev, uint16_t channel)
CW_DRIVER_CALL(pwm, disable, hDev, channel);

///
/// \brief configure the PWM output ratio.
/// \details the PWM signal will be high for (period_us / duty_us * 100) % of the time.
/// \param hDev  device handle as returned by \ref open(const char*)
/// \param channel channel some PWM peripheral icm20649 differentiate different pin outputs via channels.
///    the pin-to-channel assignments will be set in the SysConf.
///     If this is not required by the HAL driver, simply pass 0 here.
/// \param duty_cycle normalized length of the duty period (0 .. 1).
/// \param period_us length of a pulse period in microseconds, resulting in a PWM frequency of 1,000,000 / period_us Hz.
CW_DRIVER_FUNC
pwm_set_duty_cycle (int hDev, uint16_t channel, pwm_duty_percent_t duty_cycle, uint32_t period_us)
CW_DRIVER_CALL(pwm, set_duty_cycle, hDev, channel, duty_cycle, period_us);

/** @} */

#if defined (__cplusplus)
}
#endif

#endif //_COLDWAVEOS_PWM_H
