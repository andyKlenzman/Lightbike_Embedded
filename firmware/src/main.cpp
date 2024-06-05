#include <kernel.h>
#include "logging.h"

#include "drivers/ws2812b.h"
#include "drivers/icm20649.h"
#include "drivers/button.h"

#include "defines.h"
#include "globals.h"

#include "state_handlers/state_handler.h"
#include "button_ui/button_interrupt.h"


LOG_MODULE(main)

uint8_t virtual_leds[NUM_PIXELS][3];
uint8_t accel_data[3];



uint8_t *LEDFilter::p_accel_data = accel_data;
uint8_t (*LEDFilter::p_virtual_leds)[3] = virtual_leds;

int main(void) {

    int result = led_strip_init(NUM_PIXELS);
    if (result == -1) {
        LOG_ERROR("LED init failed.");
    } else {LOG_DEBUG("LED init succeeded.");}


    result = icm_20649_init();
    if (result == -1) {
        LOG_ERROR("ICM init failed.");
    } else {LOG_DEBUG("ICM init succeeded.");}

    init_button(GPIO_PUSH_BTN_1, button_irq_function);

    while (1) {
        uint32_t ts = osKernelGetTickCount();

        int result = icm_20649_read_accel_data(accel_data); // what to do with the error, I just want to tell the developer

        if (result == -1) {
            LOG_ERROR("icm_20649_read_accel_data failed.");
        }

        state_handlers[current_state]();

        for (int i = 0; i < NUM_PIXELS; i++) {
            led_strip_set_led(i, virtual_leds[i][0], virtual_leds[i][1], virtual_leds[i][2]);
        }

        led_strip_update();
        osDelayUntil(ts+20);
    }
};