/*
 * File: defines.h
 * Author: Andrew Klenzman
 * Date: YYYY-MM-DD
 * Description: The file contains macros used for configuring
 * pins and global settings. The source for the pin information
 * comes from the link below, which links to hardware this
 * software is adjusted for, though it certaintly could be tweaked for
 * other systems.
 *
 * https://imagineon1.sharepoint.com/:w:/s/ImagineOnGmbH/ER2K7mGdZTpKjimvvLsX7Y0BIgE5dDRpGYkAl6kdzfx7Tg?e=x068hb
 *
 * I use ColdwaveOS in this project, and it uses a system
 * for GPIO where letters are denoted by a corresponding number. For example,
 * if the pin number in the documentation was A01, it is entered as 001.
 * A = 0 ,
 * B = 1, etc.
 *
 * The original pin description found in the hardware
 * documentation is commented next to the pin.
 */
#pragma once

#define NUM_PIXELS (50)

#define GPIO_PUSH_BTN_1 (100) //PB00
#define GPIO_PUSH_BTN_2 (101) //PB01
#define LEDS_POWER_PIN  (300) //PD00

/* I2C pins*/
#define I2C_DATA_IN (203) //PC03
#define I2C_CLOCK (206) //PC06
#define I2C_BAUDRATE (400000) // 400 kHz
#define I2C_MEMORY_MAPPED_ADDRESS (0x50068000UL) //address found on pg 46 of EFR32xG24 Wireless SoC Reference Manual

/* SPI pins */
#define SPI_MEMORY_MAPPED_ADDRESS (0x5005C000UL)
#define SPI_GPIO_MOSI (102) //PB02
#define SPI_GPIO_CLOCK (206) //PC06
#define SPI_BAUDRATE (2500000) // 2.5 mHz
#define SPI_GPIO_CHIP_SELECT (204) //PC04








