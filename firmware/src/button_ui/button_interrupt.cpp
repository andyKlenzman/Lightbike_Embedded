#include "button_interrupt.h"
#include "state_handlers/state_handler.h"
#include "logging.h"
#include <kernel.h>
#include "defines.h"

LOG_MODULE(button_interrupt)

uint32_t click_duration = 0;
uint8_t click_count = 0;

// used to solve for only one function being called for rising and falling edge
uint8_t how_many_function_calls = 0;

void button_irq_function() {
    how_many_function_calls ++;

    if(how_many_function_calls == 2){
        how_many_function_calls = 0;

    } else {
        // static keeps value between calls
        static uint32_t last_click_time = 0;
        uint32_t current_time = osKernelGetTickCount();
        uint32_t elapsed_time = current_time - last_click_time;

        if (elapsed_time < DOUBLE_PRESS_BUTTON_TIMING) {
            click_count++;

        } else {
            click_count = 0;

        }

        click_duration = elapsed_time;
        last_click_time = current_time;

        LOG_DEBUG("INTERRUPT FIRED %d %lu %lu %d", current_state, click_duration, current_time, click_count);

        // Handle other interrupt-related tasks, if any
        current_state = static_cast<AppState>((current_state + 1) % MODE_MAX);


    }



}
