/*******************************************************************************
 * @file      $file.fileName
 * @license   This file is part of the ImagineOn Coldwave eOS software package 
 *            licensed under the ImagineOn software-licensing terms available 
 *            under https://www.imagineon.de/de/info/licensing-terms
 * @copyright Copyright (c) $originalComment.match("Copyright (c) (\d+)", 1, "-", "$today.year")$today.year. ImagineOn GmbH. www.imagineon.de.
*******************************************************************************/


#include <string.h>
#include "driver.h"
#include "spi.h"
#include "kernel.h"

#include "ws2812b.h"
#include "logging.h"


/********************************************//**
 *  LED_DATA_PACKET_SIZE = the bytes needed to control one GRB LED
 *
 *  With our SPI implementation of the WS2812B
 *  communication protocol, one bit is represented by 3 bits
 *
 *  SPI writes 110 / LED reads 1
 *  SPI writes 100 / LED reads 0
 *
 *  spi_lookup_table = index corresponds the equivalent number,
 *  represented in WS2812B communication protocol.
 *
 *  spi_lookup_table[0] == 10010010 01001001 00100100
 *
 *  -- or how WS2812B reads it
 *
 *  spi_lookup_table[0] == 100 100 100 100 100 100 100 100 == 0
 *
 ***********************************************/
#define LED_DATA_PACKET_SIZE  (9)
#define SPI_LOOKUP_TABLE_SIZE 256


#define IS_NORMALIZED(x)      ((x >= 0.0F) && (x <= 1.0F))

/********************************************//**
 *  reset code >= 50us
 *  50us / .4 us = 125
 ***********************************************/
#define RESET_CODE_LENGTH     (150) //
LOG_MODULE(main)


static int spi_dev;
static uint8_t *g_led_strip_data_buf;
static int g_led_strip_num_pixels = 0;
static float g_led_strip_color_bal[3] = {1.0F, 1.0F, 1.0F};
static uint8_t nulls[RESET_CODE_LENGTH];
int buffer_size_bytes;


static const uint8_t gamma8[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
    2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
    5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
    90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
    115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
    144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
    177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
    215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255};

/********************************************//**
 *  DATA STRUCTURE:
 *  most_significant_byte 100 | 100 | 10 -->
 *  med_significant_byte 0 | 100 | 100 | 1 -->
 *  least_significant_byte 00 | 100 | 100
 *
 *  Will this work at the hinge points?
 *  I think so, because it never has to
 *  change in two places to signifiy another value
 *
 *  What is important is that the hexes here move from low to high,
 *  so when the spi_lookup_table is built, the index corresponds to the
 *  correct wave form
 ***********************************************/
const uint8_t most_significant_byte[] = {
        0x92, // 100 100 10
        0x93, // 100 100 11
        0x9A, // 100 110 10
        0x9B, // 100 110 11
        0xD2, // 110 100 10
        0XD3, // 110 100 11
        0xDA, // 110 110 10
        0xDB, // 110 110 11 *double-checked
};


const uint8_t med_significant_byte[] = {
        0x49, // 0 100 100 1
        0X4D, // 0 100 110 1
        0x69, // 0 110 100 1
        0x6D, // 0 110 110 1 *double-checked
};

const uint8_t least_significant_byte[] = {
        0x24, // 00 100 100
        0x26, // 00 100 110
        0x34, // 00 110 100
        0x36, // 00 110 110
        0xA4, // 10 100 100
        0XA6, // 10 100 110
        0xB4, // 10 110 100
        0xB6, // 10 110 110 *double-checked
};

uint8_t spi_lookup_table[255][3] = {};


/********************************************//**
 *  This function builds a lookup table, in-which each
 *  index corresponds to an SPI bit signal that will
 *  transmit the unit8_t value
 ***********************************************/
void init_spi_lookup_table () {
    int index = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 8; k++) {
                if (index >= SPI_LOOKUP_TABLE_SIZE) break;
                spi_lookup_table[index][0] = most_significant_byte[i];
                spi_lookup_table[index][1] = med_significant_byte[j];
                spi_lookup_table[index][2] = least_significant_byte[k];
                index++;
            }
        }
    }



}


void led_strip_init (int num_pixels)
{
    init_spi_lookup_table();
    g_led_strip_num_pixels = num_pixels;

    buffer_size_bytes = LED_DATA_PACKET_SIZE * g_led_strip_num_pixels; // 8 * 50 = 450 bytes
    g_led_strip_data_buf = (uint8_t *) malloc (buffer_size_bytes);
    memset (g_led_strip_data_buf, 0, buffer_size_bytes);
    memset (nulls, 0x00, RESET_CODE_LENGTH);

    spi_dev = open ("spi0");
}



int led_strip_set_led (uint16_t index, uint8_t red, uint8_t green, uint8_t blue)
{
  red = (uint8_t) ((float) gamma8[red] * g_led_strip_color_bal[0]);
  green = (uint8_t) ((float) gamma8[green] * g_led_strip_color_bal[1]);
  blue = (uint8_t) ((float) gamma8[blue] * g_led_strip_color_bal[2]);

  if (index >= g_led_strip_num_pixels)
    {
      return -1;
    }

      /* from what direction does the SPI read? */
      memcpy (&g_led_strip_data_buf[LED_DATA_PACKET_SIZE * index] + 0, spi_lookup_table[green], 3);
      memcpy (&g_led_strip_data_buf[LED_DATA_PACKET_SIZE * index] + 3, spi_lookup_table[red], 3);
      memcpy (&g_led_strip_data_buf[LED_DATA_PACKET_SIZE * index] + 6, spi_lookup_table[blue], 3);
    return 0;
}




void led_strip_clear (int strip)
{
	for (int i = 0; i < g_led_strip_num_pixels; i++)
	{
		uint32_t index = i;
            memcpy (&g_led_strip_data_buf[LED_DATA_PACKET_SIZE * index] + 0, spi_lookup_table[0], 3);
            memcpy (&g_led_strip_data_buf[LED_DATA_PACKET_SIZE * index] + 3, spi_lookup_table[0], 3);
            memcpy (&g_led_strip_data_buf[LED_DATA_PACKET_SIZE * index] + 6, spi_lookup_table[0], 3);

	}
}


void led_strip_update ()
{
  spi_write (spi_dev, g_led_strip_data_buf, buffer_size_bytes);
  //spi_write (spi_dev, nulls, RESET_CODE_LENGTH);


}


void led_strip_white_balance (float r, float g, float b)
{
    if (IS_NORMALIZED(r) && IS_NORMALIZED(g) && IS_NORMALIZED(b))
    {
        g_led_strip_color_bal[0] = r;
        g_led_strip_color_bal[1] = g;
        g_led_strip_color_bal[2] = b;
    }
}

int led_strip_get_num_pixels ()
{
    return g_led_strip_num_pixels;
}



