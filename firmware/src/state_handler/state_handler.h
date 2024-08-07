#pragma once

// Enum to define different application states.
typedef enum {
    MODE_BASIC,
    MODE_SMOOTH,
    MODE_NICE,
    MODE_BIKE_WHEEL,
    MODE_OFF,
    MODE_MAX_VALUE
} AppState;

// Function declarations
extern volatile AppState current_state;    // Current state of the application

void increment_state();                    // Function to cycle to the next state
void select_state(AppState desired_state); // Function to select a specific state
void call_current_led_filter();            // Function to call the LED filter for the current state