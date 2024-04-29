#include <kernel.h>
#include <cstdio>
#include "utils/utils.h"
#include "drivers/ws2812b.h"
#include "drivers/icm20689.h"
#include "logging.h"
#include "drivers/icm20649_register_defs.h"

#define NUM_PIXELS (50)
LOG_MODULE(main)

// These variables are used to run the test light effect.
uint8_t colorval = 0;
bool colorswitch = false;
uint8_t accel_data[3];



int main(void) {

    led_strip_init(NUM_PIXELS);
    icm_20649_init();


    uint8_t read_buffer_value = icm_20649_read_reg_and_return_val (ICM_20649_B0_ACCEL_XOUT_L);
    LOG_DEBUG("icm register read test value = %u", read_buffer_value);




    while (1) {
        uint32_t ts = osKernelGetTickCount();
        icm_20649_read_accel_data(accel_data);

        LOG_DEBUG("LAUFT ");



        if(colorswitch){
            colorval++;
            if(colorval == 100) { colorswitch = false; }

        } else {
            colorval--;
            if(colorval == 0) { colorswitch = true; }
        }
        for(int i = 0; i < NUM_PIXELS ; i++){

            led_strip_set_led(i, accel_data[0],accel_data[1],accel_data[2]);
        }

        led_strip_update();
        osDelayUntil(ts+100);


    }
};