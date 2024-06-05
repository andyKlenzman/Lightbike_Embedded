#pragma once
#include "defines.h"

#include "led_filters/LEDFilter.h"
#include "led_filters/LEDFilter_Basic.cpp"
#include "led_filters/LEDFilter_Smooth.cpp"

extern uint8_t virtual_leds[NUM_PIXELS][3];
extern uint8_t accel_data[3];

extern LEDFilter_Smooth led_filter_smooth;
extern LEDFilter_Basic led_filter_basic;

