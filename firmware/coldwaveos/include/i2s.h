/********************************************************************************
  * @file    i2s.h
  * @author  ImagineOn cxOS development team
  *          Copyright (c) 2020 ImagineOn GmbH. All rights reserved.
  * @brief   SPI HAL module driver.
  * @copyright  ImagineoOn GmbH
  ********************************************************************************/

#ifndef _COLDWAVEOS_I2S_H
#define _COLDWAVEOS_I2S_H

#include <inttypes.h>
#include <driver.h>

#if defined (__cplusplus)
extern "C" {
#endif



/**
 * @defgroup drv_i2s I2S
 * @brief I2S Driver
 * @details Functions for interacting with peripheral devices over a I2S interface
 * @{
 */

#ifndef EXCLUDE_FROM_DOCS
struct i2s_driver {
    struct driver drv;
    int (*i2s_read)(struct device *, uint8_t *, uint32_t);
    int (*i2s_write)(struct device *, const uint8_t *, uint32_t);
};
#endif


CW_DRIVER_FUNC
i2s_read(int hDev, uint8_t *data_in, size_t len) CW_DRIVER_CALL(i2s, read, hDev, data_in, len);

CW_DRIVER_FUNC
i2s_write(int hDev, const uint8_t *data_out, size_t len) CW_DRIVER_CALL(i2s, write, hDev, data_out, len);

/**  @} */

#if defined (__cplusplus)
}
#endif

#endif //_COLDWAVEOS_I2S_H
