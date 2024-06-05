#pragma once
#include "filter_states.h"

enum AppState {
    MODE_BASIC,
    MODE_SMOOTH,
    MODE_OFF,
    MODE_MAX
};

AppState current_state = MODE_BASIC;


// creates name for an existing type. pointer to a function that returns void
typedef void (*StateHandler)();
StateHandler state_handlers[MODE_MAX] = {
        handle_basic_mode,
        handle_smooth_mode,
        handle_off_mode,
};
