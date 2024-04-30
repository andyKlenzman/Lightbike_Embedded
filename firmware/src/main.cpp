#include <kernel.h>
#include "drivers/ws2812b.h"
#include "drivers/icm20689.h"
#include "logging.h"

#define NUM_PIXELS (50)
LOG_MODULE(main)
uint8_t accel_data[3];



int main(void) {

    int result = led_strip_init(NUM_PIXELS);
    if (result == -1) {
        LOG_ERROR("LED init failed.");
        return -1;
    }

    result = icm_20649_init();
    if (result == -1) {
        LOG_ERROR("ICM init failed.");
        return -1;
    }


    while (1) {
        uint32_t ts = osKernelGetTickCount();
        icm_20649_read_accel_data(accel_data);


        for(int i = 0; i < NUM_PIXELS ; i++){
            led_strip_set_led(i, accel_data[0],accel_data[1],accel_data[2]);
        }

        led_strip_update();
        //osDelayUntil(ts+100);
    }
};