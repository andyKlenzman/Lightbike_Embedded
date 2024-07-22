#pragma once

typedef enum {
    MODE_BASIC,
    MODE_SMOOTH,
    MODE_OFF,
    MODE_MAX_VALUE
} AppState;

typedef void (*StateHandler)();

extern AppState current_state;
StateHandler increment_state();
void call_current_led_filter ();
void select_state(AppState desired_state);
extern StateHandler state_handlers[MODE_BASIC];