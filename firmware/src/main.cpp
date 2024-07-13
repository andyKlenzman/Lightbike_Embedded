/*=============================================================================
 |       Project:  Lightbike
 |       Author:  Andy Klenzman ak@imagineon.de
 |
 |
 |  Description:  Lightbike is a...
 |
 |     Language:  [Programming language and version used.]
 | Ex. Packages:  [List names and sources of all external packages
 |                required by this program.]
 |
 | Deficiencies:  [If you know of any problems with the code, provide
 |                details here, otherwise clearly state that you know
 |                of no unsatisfied requirements and no logic errors.]
 *===========================================================================*/


#include <kernel.h>
#include "logging.h"

#include "drivers/ws2812b.h"
#include "drivers/icm20649.h"

#include "defines.h"
#include "globals.h"

#include "state_handler/state_handler.h"
#include "buttons/buttons.h"

LOG_MODULE(main)

uint8_t virtual_leds[NUM_PIXELS][3];
uint8_t accel_data[3];

uint8_t *LEDFilter::p_accel_data = accel_data;
uint8_t (*LEDFilter::p_virtual_leds)[3] = virtual_leds;

int main(void) {
    int result;
    result = led_strip_init(NUM_PIXELS);
    if (result == -1) {
        LOG_ERROR("LED init failed.");
    } else {LOG_DEBUG("LED init succeeded.");}

    result = icm_20649_init();
    if (result == -1) {
        LOG_ERROR("ICM init failed.");
    } else {LOG_DEBUG("ICM init succeeded.");}

    result = button_init(GPIO_PUSH_BTN_1, mode_button_irq_function);
    if (result == -1) {
        LOG_ERROR("Mode button init failed.");
    } else {LOG_DEBUG("Mode button init succeeded.");}

    result = button_init(GPIO_PUSH_BTN_2, power_button_irq_function);
    if (result == -1) {
        LOG_ERROR("Power button init failed.");
    } else {LOG_DEBUG("Power button init succeeded.");}


    while (1) {
        uint32_t ts = osKernelGetTickCount();

        result = icm_20649_read_accel_data(accel_data);
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