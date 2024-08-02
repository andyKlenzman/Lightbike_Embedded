#pragma once
#include "defines.h"
#include "led_filters/LEDFilter.h"
#include "led_filters/LEDFilter_Basic.cpp"
#include "led_filters/LEDFilter_Smooth.cpp"
#include "led_filters/LEDFilter_Off.cpp"
#include "led_filters/LEDFilter_Wave.cpp"


extern uint8_t virtual_leds[NUM_PIXELS][3];


extern LEDFilter_Smooth led_filter_smooth;
extern LEDFilter_Basic led_filter_basic;
extern LEDFilter_Wave led_filter_wave;
extern LEDFilter_Off led_filter_off;

extern volatile bool flag_toggle_system_power;  // Flag variable to signal events


