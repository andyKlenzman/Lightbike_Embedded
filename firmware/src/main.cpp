#include <kernel.h>
#include "drivers/ws2812b.h"
#include "drivers/icm20649.h"
#include "logging.h"
#include "led_filters/LEDFilter_Basic.cpp"
#include "led_filters/LEDFilter_Smooth.cpp"
#include "defines.h"

LOG_MODULE(main)



/********************************************//**
 Here we set p_accel_data and p_virtual_leds,
 static members of the LED filter class, with
 the memory location of accel_data and virtual_leds.

 This allows the class to access the two arrays that
 are used throughout the firmware, without using extra
 memory.
 ***********************************************/
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

    LEDFilter_Smooth led_filter_smooth;


    while (1) {
        uint32_t ts = osKernelGetTickCount();
        int result = icm_20649_read_accel_data(accel_data); // what to do with the error, I just want to tell the developer
        if (result == -1) {
            LOG_ERROR("icm_20649_read_accel_data failed.");
        }


        led_filter_smooth.apply_filter();

        for (int i = 0; i < NUM_PIXELS; i++) {
            //LOG_DEBUG("Debug: Value 1 = %u, Value 2 = %u, Value 3 = %u\n", virtual_leds[i][0], virtual_leds[i][1], virtual_leds[i][2]);
            led_strip_set_led(i, virtual_leds[i][0], virtual_leds[i][1], virtual_leds[i][2]);
        }

        led_strip_update();
        osDelayUntil(ts+20);
    }
};