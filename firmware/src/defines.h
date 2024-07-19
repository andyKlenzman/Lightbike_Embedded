/*
 * File: defines.h
 * Author: Andrew Klenzman
 * Date: YYYY-MM-DD
 * Description: The file contains macros used for configuring
 * pins and global settings. The source for the pin information
 * comes from the link below, which links to hardware this
 * software is adjusted for. The pins use a system where the second
 * letter is denoted by a corresponding number. For example: A = 0 ,
 * B = 1, etc. The original pin description found in the hardware
 * documentation is commented next to the pin.
 */
#pragma once

#define NUM_PIXELS (50)

#define GPIO_PUSH_BTN_1 (100) //PB00
#define GPIO_PUSH_BTN_2 (101) //PB01

#define LEDS_DATA_PIN (102) //PB02
#define LEDS_POWER_PIN  (300) //PD00



#define DOUBLE_PRESS_BUTTON_TIMING (700)

/* I2C pins*/
#define I2C_DATA_IN (203) //PC03
#define I2C_CLOCK (206) //PC06
#define I2C_BAUDRATE (400000) // 400 kHz
#define I2C_MEMORY_MAPPED_ADDRESS (0x50068000UL) //address found on pg 46 of EFR32xG24 Wireless SoC Reference Manual

/* Set 1 or 0 to address two, 7-bit address spaces in the ICM??*/
#define ICM_SLAVE_ADDRESS_PIN (205) // PC05






