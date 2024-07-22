/*
 * File: state_handler.cpp
 * Author: Andrew Klenzman
 * Description:
 * Contains functionality for changing the state of the device,
 * including switching types of LED filters (the pattern of the LEDs)
 * and handling an off mode.
 */

#pragma once
#include "globals.h"

// Function declarations for handling different modes
void handle_basic_mode();
void handle_smooth_mode();
void handle_off_mode();

// Enum to define different application states
enum AppState {
    MODE_BASIC,    // Basic LED filter mode
    MODE_SMOOTH,   // Smooth LED filter mode
    MODE_OFF,      // LED off mode
    MODE_MAX       // Total number of modes (used for array bounds)
};

// Global variable to keep track of the current state
volatile AppState current_state = MODE_BASIC;

/**
 * @brief StateHandler is a function pointer type for handling different LED filter modes.
 */
typedef void (*StateHandler)();

// Array of function pointers for handling each mode
StateHandler state_handlers[MODE_MAX] = {
        handle_basic_mode,   // Handler for MODE_BASIC
        handle_smooth_mode,  // Handler for MODE_SMOOTH
        handle_off_mode      // Handler for MODE_OFF
};

// Instances of LED filter objects
LEDFilter_Smooth led_filter_smooth;
LEDFilter_Basic led_filter_basic;

/**
 * @brief Handles the basic LED filter mode.
 */
void handle_basic_mode() {
    led_filter_basic.apply_filter();
}

/**
 * @brief Handles the smooth LED filter mode.
 */
void handle_smooth_mode() {
    led_filter_smooth.apply_filter();
}

/**
 * @brief Handles the off mode (currently does nothing).
 */
void handle_off_mode() {
    // No action needed for OFF mode
}

/**
 * @brief Cycles to the next state in the sequence.
 *        Wraps around to MODE_BASIC after MODE_OFF.
 */
void select_next_state() {
    current_state = static_cast<AppState>((current_state + 1) % MODE_MAX);
}

/**
 * @brief Calls the function associated with the current state.
 */
void call_current_led_filter() {
    state_handlers[current_state]();
}

/*
 * To add a new mode:
 * 1. Add a new entry to the AppState enum:
 *    enum AppState {
 *        MODE_BASIC,
 *        MODE_SMOOTH,
 *        MODE_OFF,
 *        MODE_NEW_MODE,  // Add new mode here
 *        MODE_MAX
 *    };
 *
 * 2. Declare and define a new function for the new mode:
 *    void handle_new_mode();
 *
 *    void handle_new_mode() {
 *        // Implement behavior for the new mode here
 *    }
 *
 * 3. Add the new function to the state_handlers array:
 *    StateHandler state_handlers[MODE_MAX] = {
 *        handle_basic_mode,
 *        handle_smooth_mode,
 *        handle_off_mode,
 *        handle_new_mode,  // Add new mode handler here
 *    };
 *
 * 4. Ensure select_next_state() correctly cycles through the new mode if needed:
 *    void select_next_state() {
 *        current_state = static_cast<AppState>((current_state + 1) % MODE_MAX);
 *    }
 */
