#pragma once
#include "defines.h"
#include "led_filters/LEDFilter.h"
#include "led_filters/LEDFilter_Basic.cpp"
#include "led_filters/LEDFilter_Smooth.cpp"
#include "led_filters/LEDFilter_Off.cpp"
#include "led_filters/LEDFilter_Nice.cpp"


extern uint8_t virtual_leds[NUM_PIXELS][3];
extern uint8_t accel_data[3];

extern LEDFilter_Smooth led_filter_smooth;
extern LEDFilter_Basic led_filter_basic;
extern LEDFilter_Nice led_filter_nice;
extern LEDFilter_Off led_filter_off;

extern volatile bool flag_toggle_system_power;  // Flag variable to signal events


