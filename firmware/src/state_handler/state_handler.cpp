#pragma once
#include "globals.h"


// What is the least amount of ways to change a mode.
// could the StateHandler function pointers parameters be
// used to add other options?


void handle_basic_mode();
void handle_smooth_mode();
void handle_off_mode();



enum AppState {
    MODE_BASIC,
    MODE_SMOOTH,
    MODE_OFF,
    MODE_MAX
};

AppState current_state = MODE_BASIC;

/**
 * @brief StateHandler type def is a function pointer
 * that runs various instances of LEDFilters.
 */
typedef void (*StateHandler)();

StateHandler state_handlers[MODE_MAX] = {
        handle_basic_mode,
        handle_smooth_mode,
        handle_off_mode,
};

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

//create a conviencence function that toggles through the various states of the state handler.