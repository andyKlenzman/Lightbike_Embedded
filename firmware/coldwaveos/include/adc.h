/*******************************************************************************
 * @file       adc.h
 * @brief
 * @copyright  Copyright (c) 2020-2023. ImagineOn GmbH. www.imagineon.de
 *             All rights reserved.
 ******************************************************************************/

#ifndef _COLDWAVEOS_ADC_H
#define _COLDWAVEOS_ADC_H

#include <inttypes.h>
#include <driver.h>

#if defined (__cplusplus)
extern "C" {
#endif

/**
 * @defgroup drv_analog_adc Analog Read
 * \ingroup drv
 * @brief Simplified API for reading analog values from external voltage sources
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
/**
 * @brief ADC driver API
 * minimum API for ADC icm20649.
 */
struct adc_driver {
    struct driver drv;
    int (*adc_read) (struct device *, uint8_t channel);
    int (*adc_read_buf) (struct device *, uint8_t channel, uint32_t *buf);
};
#endif

///
/// \param hdev device handle as returned by \ref open(const char*)
/// \param channel some analog peripheral icm20649 differentiate different pin outputs via channels.
///     if this is not required by the HAL driver, simply pass 0 here.
/// \return the value returned by the driver
/// \return -ENODEV if the device handle is invalid
CW_DRIVER_FUNC adc_read (int hdev, uint8_t channel)
CW_DRIVER_CALL(adc, read, hdev, channel);

CW_DRIVER_FUNC adc_read_buf (int hdev, uint8_t channel, uint32_t *buf)
CW_DRIVER_CALL(adc, read_buf, hdev, channel, buf);

/** @} */

#if defined (__cplusplus)
}
#endif

#endif //_COLDWAVEOS_ADC_H
