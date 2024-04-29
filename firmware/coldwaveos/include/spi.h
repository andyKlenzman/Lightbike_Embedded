/********************************************************************************
  * @file    spi.h
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
 * @defgroup drv_serial_spi SPI
 * \ingroup drv_serial
 * @brief SPI Driver
 * @details Functions for interacting with peripheral devices over a serial peripheral bus (SPI)
 *
 * Sysprops
 * --------
 *
 * sysprop   | description
 * ----------|------------------------------------
 * gpio_mosi | the master out GPIO
 * gpio_miso | the master in GPIO
 * gpio_cs   | the chip select (low active) GPIO
 * gpio_clk  | the master clock GPIO
 * baudrate  | the desired baudrate
 *
 * @{
 */

#ifndef EXCLUDE_FROM_DOCS
struct spi_driver {
    struct driver drv;
    int (*spi_transfer) (struct device *, const uint8_t *, uint8_t *, size_t);
};
#endif

///
/// \brief send to and receive from the SPI at the same time.
/// \param hDev device handle as returned by \ref open(const char*)
/// \param data_out
/// \param data_in
/// \param len
/// \return
CW_DRIVER_FUNC
spi_transfer (int hDev, const uint8_t *data_out, uint8_t *data_in, size_t len)
CW_DRIVER_CALL(spi, transfer, hDev, data_out, data_in, len);


///
/// \brief read data from the SPI
/// \details convenience function that simply transmits 0xff characters to the communications partner
/// \param hDev device handle as returned by \ref open(const char*)
/// \param data
/// \param len
/// \return
CW_DRIVER_FUNC
spi_read (int hDev, uint8_t *data, uint32_t len)
  {
  return spi_transfer (hDev, NULL, data, len);
  }

///
/// \brief write data to the SPI
/// \details convenience function that simply discards received bytes during the transfer
/// \param hDev device handle as returned by \ref open(const char*)
/// \param data buffer holding the data to be written
/// \param len length of the buffer
/// \return
CW_DRIVER_FUNC
spi_write (int hDev, const uint8_t *data, uint32_t len)
  {
  return spi_transfer (hDev, data, NULL, len);
  }



/**  @} */

#if defined (__cplusplus)
}
#endif

#endif //_COLDWAVEOS_SPI_H
