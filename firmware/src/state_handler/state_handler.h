#pragma once

enum AppState {
    MODE_BASIC,
    MODE_SMOOTH,
    MODE_OFF,
    MODE_MAX
};

typedef void (*StateHandler)();

extern AppState current_state;
StateHandler select_next_state();
void call_current_led_filter ();
extern StateHandler state_handlers[MODE_MAX];