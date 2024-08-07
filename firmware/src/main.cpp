#include <kernel.h>
#include "logging.h"
#include "globals.h"
#include "ws2812b/ws2812b.h"
#include "icm20649/icm20649.h"
#include "buttons/buttons.h"
#include "filter_handler/filter_handler.h"
#include "utils/power_toggle.c"
#include "utils/map_value.h"
#include "led_filters/LEDFilter.h"

LOG_MODULE(main)

/* Global data buffers */
float accel_data[3];
float gyro_data[3];
uint8_t mapped_accel_data[3];
uint8_t mapped_gyro_data[3];
uint8_t virtual_leds[NUM_PIXELS][3];

/* Pointers for LEDFilter access */
float *LEDFilter::p_accel_data = accel_data;
float *LEDFilter::p_gyro_data = gyro_data;
uint8_t *LEDFilter::p_mapped_accel_data = mapped_accel_data;
uint8_t *LEDFilter::p_mapped_gyro_data = mapped_gyro_data;
uint8_t (*LEDFilter::p_virtual_leds)[3] = virtual_leds;

/* Power toggle flag */
volatile bool flag_toggle_system_power = false;


int main(void) {
    int result;

    /* Initialize components */
    if ((result = led_strip_init(NUM_PIXELS)) == -1) {
        LOG_ERROR("LED init failed.");
    } else {
        LOG_DEBUG("LED init succeeded.");
    }

    if ((result = icm_20649_init()) == -1) {
        LOG_ERROR("ICM init failed.");
    } else {
        LOG_DEBUG("ICM init succeeded.");
    }

    if ((result = button_init(GPIO_PUSH_BTN_1, mode_button_irq_function)) == -1) {
        LOG_ERROR("Mode button init failed.");
    } else {
        LOG_DEBUG("Mode button init succeeded.");
    }

    if ((result = button_init(GPIO_PUSH_BTN_2, power_button_irq_function)) == -1) {
        LOG_ERROR("Power button init failed.");
    } else {
        LOG_DEBUG("Power button init succeeded.");
    }

    while (1) {
        uint32_t ts = osKernelGetTickCount();

        /* Read sensor data */
        if ((result = icm_20649_read_gyro_data(gyro_data)) == -1) {
            LOG_ERROR("icm_20649_read_gyro_data failed.");
        } else {
            LOG_DEBUG("GYRO data: X=%f, Y=%f, Z=%f\n", gyro_data[0], gyro_data[1], gyro_data[2]);
        }

        if ((result = icm_20649_read_accel_data(accel_data)) == -1) {
            LOG_ERROR("icm_20649_read_accel_data failed.");
        } else {
            LOG_DEBUG("Accelerometer data: X=%f, Y=%f, Z=%f\n", accel_data[0], accel_data[1], accel_data[2]);
        }

        /* Map sensor data to appropriate ranges */
        for (int i = 0; i < 3; i++) {
            mapped_accel_data[i] = map_value(accel_data[i], ACCEL_MAP_IN_MIN, ACCEL_MAP_IN_MAX, ACCEL_MAP_OUT_MIN, ACCEL_MAP_OUT_MAX, MAPPING_MODE);
            mapped_gyro_data[i] = map_value(gyro_data[i], GYRO_MAP_IN_MIN, GYRO_MAP_IN_MAX, GYRO_MAP_OUT_MAX, GYRO_MAP_OUT_MIN, MAPPING_MODE);
        }

        /* Update LEDs based on current filter */
        call_current_led_filter();
        set_leds(virtual_leds, NUM_PIXELS);
        update_leds();

        /* Toggle system power if flag is set */
        if (flag_toggle_system_power) {
            toggle_power();
            flag_toggle_system_power = false;
        }


    }
}
