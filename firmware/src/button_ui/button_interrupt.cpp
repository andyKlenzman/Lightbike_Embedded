#include "button_interrupt.h"
#include "state_handlers/state_handler.h"
#include "logging.h"


LOG_MODULE(button_interrupt)

void button_irq_function(){
    LOG_DEBUG("INTERRUPT FIRED %d", current_state);
    current_state = static_cast<AppState>((current_state + 1) % MODE_MAX);

};

