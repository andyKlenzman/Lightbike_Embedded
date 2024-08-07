/*
 * File: sysconfig.c
 * Author: Andrew Klenzman
 * Date:
 * Description: This file contains the configuration of the SPI and
 * i2c icm20649.
 * Deficiencies:
 */

#include <sysconfig.h>
#include "globals.h"


sysconf_use_driver(spi_gecko)
sysconf_use_driver(i2c_gecko)


sysconf_create_device("silabs-gecko-spi", spi0, SPI_MEMORY_MAPPED_ADDRESS,
                      sysconf_set_int_param (gpio_mosi, SPI_GPIO_MOSI),
                      sysconf_set_int_param (gpio_clk, SPI_GPIO_CLOCK),
                      sysconf_set_int_param (gpio_miso, 202),
                      sysconf_set_int_param (baudrate, SPI_BAUDRATE), //2.5 mHz
//                      sysconf_set_int_param (gpio_cs, 200)
)


sysconf_create_device("silabs-gecko-i2c", i2c1, I2C_MEMORY_MAPPED_ADDRESS,
                      sysconf_set_int_param (gpio_scl, I2C_CLOCK),
                      sysconf_set_int_param (gpio_sda, I2C_DATA_IN),
                      sysconf_set_int_param (baudrate, I2C_BAUDRATE), //400 kHz
                      )