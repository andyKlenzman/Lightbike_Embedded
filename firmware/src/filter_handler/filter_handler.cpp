/*
 * File: filter_handler.cpp
 * Author: Andrew Klenzman
 * Description:
 * Contains functionality for changing the state of the device,
 * including switching types of LED filters (the pattern of the LEDs)
 * and handling an off mode.
 */

#pragma once
#include "logging.h"
#include "filter_handler.h"
#include "globals.h"
#include "led_filters/LEDFilter.h"
#include "led_filters/LEDFilter_Basic.cpp"
#include "led_filters/LEDFilter_Smooth.cpp"
#include "led_filters/LEDFilter_Off.cpp"
#include "led_filters/LEDFilter_Wave.cpp"
#include "led_filters/LEDFilter_BikeWheel.cpp"


LOG_MODULE(STATE_HANDLER)

// Instances of LED filter objects
LEDFilter_Smooth led_filter_smooth;
LEDFilter_Basic led_filter_basic(FILTER_BASIC_SMOOTHING);
LEDFilter_Wave led_filter_wave(FILTER_WAVE_SMOOTHING,
                               FILTER_WAVE_FREQUENCY,
                               FILTER_WAVE_AMPLITUDE);
LEDFilter_BicycleWheel led_filter_bike_wheel;
LEDFilter_Off led_filter_off;

// Global variable to keep track of the current state
volatile AppState current_state = MODE_BASIC;

// Array of LED filter objects corresponding to each state
LEDFilter* led_filters[MODE_MAX_VALUE] = {
        &led_filter_basic,
        &led_filter_smooth,
        &led_filter_wave,
        &led_filter_bike_wheel,
        &led_filter_off
};

/**
 * @brief Cycles to the next state in the sequence.
 *        Wraps around to MODE_BASIC after MODE_OFF.
 */
void increment_state() {
    current_state = static_cast<AppState>((current_state + 1) % MODE_MAX_VALUE);
}

/**
 * @brief Selects a specific state.
 */
void select_state(AppState desired_state) {
    current_state = desired_state;
}

/**
 * @brief Calls the function associated with the current state.
 */
void call_current_led_filter() {
    led_filters[current_state]->apply_filter();
}
