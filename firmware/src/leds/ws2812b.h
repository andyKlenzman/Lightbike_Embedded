/*******************************************************************************
 * @file      $file.fileName
 * @license   This file is part of the ImagineOn Coldwave eOS software package 
 *            licensed under the ImagineOn software-licensing terms available 
 *            under https://www.imagineon.de/de/info/licensing-terms
 * @copyright Copyright (c) $originalComment.match("Copyright (c) (\d+)", 1, "-", "$today.year")$today.year. ImagineOn GmbH. www.imagineon.de.
*******************************************************************************/

#ifndef _LED_STRIP_H_
#define _LED_STRIP_H_

#include <stdint.h>

#if defined (__cplusplus)
extern "C" {
#endif
int led_strip_init(int num_pixels);
void led_strip_white_balance (float r, float g, float b);
int  led_strip_set_led(uint16_t index, uint8_t red, uint8_t green, uint8_t blue);
int  led_strip_get_num_pixels();
void update_leds();
void led_strip_select (int strip);
void set_leds(uint8_t (*virtual_leds)[3], int num_pixels);


#if defined (__cplusplus)
}
#endif

#endif //_LED_STRIP_H_
