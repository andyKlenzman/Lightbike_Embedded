#pragma once
#include "logging.h"
#include "filter_states.h"
#include "globals.h"


LEDFilter_Smooth led_filter_smooth;
LEDFilter_Basic led_filter_basic;

void handle_basic_mode() {
    led_filter_smooth.apply_filter();
}

void handle_smooth_mode() {
    led_filter_basic.apply_filter();
}

void handle_off_mode() {
}