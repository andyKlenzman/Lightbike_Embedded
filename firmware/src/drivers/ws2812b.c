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
#include "../utils/gamma8_table.c"
#include "ws2812b.h"
#include "logging.h"

#define LED_DATA_PACKET_SIZE  (9)
#define IS_NORMALIZED(x)      ((x >= 0.0F) && (x <= 1.0F))

/********************************************//**
 *  reset code >= 50us
 *  50us / .4 us = 125
 ***********************************************/
#define RESET_CODE_LENGTH     (150) //
LOG_MODULE(ws2812b.c)


static int spi_dev;
static uint8_t *g_led_strip_data_buf;
static int g_led_strip_num_pixels = 0;
static float g_led_strip_color_bal[3] = {1.0F, 1.0F, 1.0F};
static uint8_t nulls[RESET_CODE_LENGTH];
int buffer_size_bytes;
uint8_t spi_lookup_table[255][3];


#define SPI_LOOKUP_TABLE_SIZE 256

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


/********************************************//**
 *  This function builds a lookup table, in-which each
 *  index corresponds to an SPI bit signal that will
 *  transmit the unit8_t value
 ***********************************************/
void init_spi_lookup_table() {
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


/**
 * @brief Initialize the LED strip.
 *
 * This function initializes the LED strip with the specified number of pixels.
 *
 * @param num_pixels Number of pixels in the LED strip.
 * @return 0 on success, -1 on failure.
 */
int led_strip_init(int num_pixels) {
    init_spi_lookup_table();
    g_led_strip_num_pixels = num_pixels;

    buffer_size_bytes = LED_DATA_PACKET_SIZE * g_led_strip_num_pixels; // 8 * 50 = 450 bytes
    g_led_strip_data_buf = (uint8_t *) malloc(buffer_size_bytes);
    memset(g_led_strip_data_buf, 0, buffer_size_bytes);
    memset(nulls, 0x00, RESET_CODE_LENGTH);

    spi_dev = open("spi0");

    if (spi_dev == -1 || g_led_strip_data_buf == NULL) {




        return -1;
    }

    return 0;
}


int led_strip_set_led(uint16_t index, uint8_t red, uint8_t green, uint8_t blue) {
    red = (uint8_t) ((float) gamma8[red] * g_led_strip_color_bal[0]);
    green = (uint8_t) ((float) gamma8[green] * g_led_strip_color_bal[1]);
    blue = (uint8_t) ((float) gamma8[blue] * g_led_strip_color_bal[2]);

    if (index >= g_led_strip_num_pixels) {
        return -1;
    }

    memcpy(&g_led_strip_data_buf[LED_DATA_PACKET_SIZE * index] + 0, spi_lookup_table[green], 3);
    memcpy(&g_led_strip_data_buf[LED_DATA_PACKET_SIZE * index] + 3, spi_lookup_table[red], 3);
    memcpy(&g_led_strip_data_buf[LED_DATA_PACKET_SIZE * index] + 6, spi_lookup_table[blue], 3);
    return 0;
}


void led_strip_clear(int strip) {
    for (int i = 0; i < g_led_strip_num_pixels; i++) {
        uint32_t index = i;
        memcpy(&g_led_strip_data_buf[LED_DATA_PACKET_SIZE * index] + 0, spi_lookup_table[0], 3);
        memcpy(&g_led_strip_data_buf[LED_DATA_PACKET_SIZE * index] + 3, spi_lookup_table[0], 3);
        memcpy(&g_led_strip_data_buf[LED_DATA_PACKET_SIZE * index] + 6, spi_lookup_table[0], 3);

    }
}


void led_strip_update() {
    spi_write(spi_dev, g_led_strip_data_buf, buffer_size_bytes);
    //spi_write (spi_dev, nulls, RESET_CODE_LENGTH); // I might need to hard code a delay as project timing changes.


}


void led_strip_white_balance(float r, float g, float b) {
    if (IS_NORMALIZED(r) && IS_NORMALIZED(g) && IS_NORMALIZED(b)) {
        g_led_strip_color_bal[0] = r;
        g_led_strip_color_bal[1] = g;
        g_led_strip_color_bal[2] = b;
    }
}

int led_strip_get_num_pixels() {
    return g_led_strip_num_pixels;
}