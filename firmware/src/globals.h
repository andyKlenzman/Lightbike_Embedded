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
#include <stdbool.h>


extern volatile bool flag_toggle_system_power;  // Flag variable to signal events
extern volatile bool is_system_on;



#define NUM_PIXELS (50)
#define MAX_REGISTER_READ_RETRIES (50)
#define DEBUG_PRINT_ICM20649 // uncomment to enable debug statements

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


/* LEDFilter Defines */
/* General Settings*/
#define FILTER_SMOOTHING_FACTOR (47.9)

/* Sensor mapping settings*/

#define ACCEL_MAP_IN_MAX (2)
#define ACCEL_MAP_IN_MIN (-2)

#define ACCEL_MAP_OUT_MAX (255)
#define ACCEL_MAP_OUT_MIN (0)

#define GYRO_MAP_IN_MAX (50)
#define GYRO_MAP_IN_MIN (-50)

#define GYRO_MAP_OUT_MAX (0)
#define GYRO_MAP_OUT_MIN (255)

/*
 * MAP_MODE_SIGNED,         // 0 is the middle value, with equal positive and negative ranges
    MAP_MODE_UNSIGNED,       // 0 is the minimum value, with output increasing for positive inputs only
    MAP_MODE_SYMMETRICAL
 * */

#define MAPPING_MODE (MAP_MODE_SYMMETRICAL)

/* LEDFilter_Wave settings*/
#define FILTER_WAVE_SMOOTHING (4.9)
#define FILTER_WAVE_FREQUENCY (0.005)
#define FILTER_WAVE_AMPLITUDE (0.005)

/* LEDFilter_Star settings*/
#define FILTER_STAR_FREQUENCY_FACTOR (.001)
#define FILTER_STAR_FADE_SPEED (4)

#define FRAME_TIME_MS (17)













