/********************************************************************************
  * @file    i2c.h
  * @author  ImagineOn cxOS development team
  *          Copyright (c) 2020 ImagineOn GmbH. All rights reserved.
  * @brief   I2C HAL module driver.
  * @copyright  ImagineoOn GmbH
  ********************************************************************************/

#ifndef _COLDWAVEOS_I2C_H
#define _COLDWAVEOS_I2C_H

#include <inttypes.h>
#include <driver.h>

#if defined (__cplusplus)
extern "C" {
#endif




/**
 * @defgroup drv_serial_i2c I2C
 * \ingroup drv_serial
 * @brief I2C API
 * @details Functions for the Inter-IC-Communication Bus
 *
 * Sysprops
 * --------
 *
 * sysprop   | description
 * ----------|------------------------------------
 * gpio_sda  | SDA GPIO
 * gpio_scl  | SCL GPIO
 *
 * @{
 */
#ifndef EXCLUDE_FROM_DOCS
struct i2c_driver {
    struct driver drv;
    int (*i2c_write) (struct device *, uint8_t, const uint8_t *, uint8_t);
    int (*i2c_write_reg) (struct device *, uint8_t, uint8_t, const uint8_t *, uint8_t);
    int (*i2c_read) (struct device *, uint8_t, uint8_t *, uint8_t);
    int (*i2c_read_reg) (struct device *, uint8_t, uint8_t, uint8_t *, uint8_t);
    int (*i2c_transfer) (struct device *, uint8_t, const uint8_t *, uint8_t, uint8_t *, uint8_t);
    int (*i2c_clear) (struct device *);
    };
#endif

///
/// \param hDev device handle as returned by \ref open(const char*)
/// \param addr the 7bit address of the device to read from.
/// \param data buffer holding the space for the data to be read.
/// \param len length of the data buffer in bytes.
/// \return 0 if the data has successfully been read.
CW_DRIVER_FUNC
i2c_read (int hDev, uint8_t addr, uint8_t *data, uint8_t len)
CW_DRIVER_CALL(i2c, read, hDev, addr, data, len);

///
/// \param hDev device handle as returned by \ref open(const char*)
/// \param addr the 7bit address of the device to read from.
/// \param reg the device register to read data from
/// \param data buffer holding the space for the data to be read.
/// \param len length of the data buffer in bytes.
/// \return 0 if the data has successfully been read.
CW_DRIVER_FUNC
i2c_read_reg (int hDev, uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len)
CW_DRIVER_CALL(i2c, read_reg, hDev, addr, reg, data, len);

///
/// \param hDev device handle as returned by \ref open(const char*)
/// \param addr the 7bit address of the device to write to.
/// \param data buffer containing the data to be written.
/// \param len length of the data buffer in bytes.
/// \param send_stop pass 1 to send a STOP condition after completing the write, for a repeated-start pass 0.
/// \return 0 if the data has been successfully written.
CW_DRIVER_FUNC
i2c_write (int hDev, uint8_t addr, const uint8_t *data, uint8_t len)
CW_DRIVER_CALL(i2c, write, hDev, addr, data, len);

//
/// \param hDev device handle as returned by \ref open(const char*)
/// \param addr the 7bit address of the device to write to.
/// \param reg the device register to be written to
/// \param data buffer containing the data to be written.
/// \param len length of the data buffer in bytes.
/// \return 0 if the data has been successfully written.
CW_DRIVER_FUNC
i2c_write_reg (int hDev, uint8_t addr, uint8_t reg, const uint8_t *data, uint8_t len)
CW_DRIVER_CALL(i2c, write_reg, hDev, addr, reg, data, len);


///
/// \param hDev device handle as returned by \ref open(const char*)
/// \param addr the 7bit address of the device to write to.
/// \param data buffer containing the data to be written.
/// \param len length of the data buffer in bytes.
/// \param send_stop pass 1 to send a STOP condition after completing the write, for a repeated-start pass 0.
/// \return 0 if the data has been successfully written.
CW_DRIVER_FUNC
i2c_transfer (int hDev, uint8_t addr, const uint8_t *data_w, uint8_t len_w, uint8_t *data_r, uint8_t len_r)
CW_DRIVER_CALL(i2c, transfer, hDev, addr, data_w, len_w, data_r, len_r);



///
/// \brief resets a stuck bus by sending 9 clock pulses
/// \param hDev device handle as returned by \ref open(const char*)
CW_DRIVER_FUNC
i2c_clear (int hDev)
CW_DRIVER_CALL_VOID(i2c, clear, hDev);

/**  @} */


#if defined (__cplusplus)
}
#endif

#endif //_COLDWAVEOS_I2C_H
