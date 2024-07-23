/********************************************************************************
  * @file    dac.h
  * @author  ImagineOn cxOS development team
  *          Copyright (c) 2020 ImagineOn GmbH. All rights reserved.
  * @brief   DAC HAL module driver.
  * @copyright  ImagineoOn GmbH
  ********************************************************************************/

#ifndef COLDWAVEOS_DAC_H
#define COLDWAVEOS_DAC_H

#include <inttypes.h>
#include <driver.h>

#if defined (__cplusplus)
extern "C" {
#endif

/**
 * @defgroup drv_analog_dac Analog Write
 * @brief Simplified API for converting digital values to analog voltages
 * @details While a possible future version of the OS will have a more complex analog API,
 * at the moment the analog APIs are only meant to provide a quick means for reading e.g. a battery voltage
 * or providing a controllable voltage reference for an external analog circuitry.
 * If your application needs to e.g. gather high frequency analog samples from a microphone a
 * vendor-specific implementation for the analog peripheral has to be written in application code.
 *
 * Sysprops
 * --------
 * Sysprops for the ADC are driver dependent. Please refer to the documentation of the specific hardware driver you're using.
 *
 * @{
 */

#ifndef EXCLUDE_FROM_DOCS
struct dac_driver
	{
	struct driver drv;
	int (*dac_convert) (struct device *, uint32_t);
	};
#endif

///
/// \param hDev device handle as returned by \ref open(const char*)
/// \param val set the value according to the HAL peripheral icm20649 translation function.
///     for a 12bit DAC e.g passing 4095 will output the max voltage vRef.
CW_DRIVER_FUNC
dac_convert (int hDev, uint32_t val)
CW_DRIVER_CALL(dac, convert, hDev, val);
/** @} */

#if defined (__cplusplus)
}
#endif

#endif //COLDWAVEOS_DAC_H
