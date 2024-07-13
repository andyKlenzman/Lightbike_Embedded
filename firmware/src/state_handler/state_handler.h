#pragma once

enum AppState {
    MODE_BASIC,
    MODE_SMOOTH,
    MODE_OFF,
    MODE_MAX
};

extern AppState current_state;

typedef void (*StateHandler)();

extern StateHandler state_handlers[MODE_MAX];