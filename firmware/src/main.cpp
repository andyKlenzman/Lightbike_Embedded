#include <kernel.h>
#include "drivers/ws2812b.h"
#include "drivers/icm20649.h"
#include "logging.h"
#include "led_filters/LEDFilter.h"
#include "led_filters/LEDFilter_Smooth.cpp"

LOG_MODULE(main)


int main(void) {

    LEDFilter_Smooth led_filter_smooth;

    while (1) {
        uint32_t ts = osKernelGetTickCount();
        led_filter_smooth.update_motion_values();
        led_filter_smooth.apply_filter();

        led_strip_update();
        osDelayUntil(ts+20);
    }
};