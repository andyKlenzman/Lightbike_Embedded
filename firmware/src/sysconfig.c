#include <sysconfig.h>

sysconf_use_driver(spi_gecko)
sysconf_use_driver(i2c_gecko)

/********************************************//**
 *  pin mappings coming from page 12 of the SiLabs EVK user guide
 *
 *
 ***********************************************/
sysconf_create_device("silabs-gecko-spi", spi0, 0x5005C000UL,
                      sysconf_set_int_param (gpio_mosi, 201),
                      sysconf_set_int_param (gpio_clk, 203),
                      sysconf_set_int_param (gpio_miso, 202),
                      sysconf_set_int_param (baudrate, 2500000), //2.5 mHz
                      sysconf_set_int_param (gpio_cs, 200))



// address found on pg 46 of EFR32xG24 Wireless SoC Reference Manual
sysconf_create_device("silabs-gecko-i2c", i2c1, 0x50068000UL,
                      sysconf_set_int_param (gpio_scl, 205),
                      sysconf_set_int_param (gpio_sda, 207),
                      sysconf_set_int_param (baudrate, 400000), //400 kHz
                      )