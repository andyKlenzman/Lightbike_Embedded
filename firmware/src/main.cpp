/*=============================================================================
Project:  Lightbike
Author:   Andy Klenzman (ak@imagineon.de)
Description:

Lightbike enhances bicycle visibility and aesthetics through motion-responsive
LED patterns using motion-powered LEDs mounted on bike spokes. The system is
controlled by the EFR32MG24 MCU, with motion data collected via the ICM20649
sensor and LEDs controlled by WS2812s through SPI and level shifting.

Conventions:
- **Log Modules:** Use all uppercase for module names.
- **Defines:** Avoid abbreviations.
- **Errors:** Negative numbers are errors, 0 indicates success.

Deficiencies:
- Detailed instructions for adding new LED filters are missing.
- Gyroscope data integration is incomplete.

*===========================================================================*/

#include <kernel.h>
#include "logging.h"
#include "defines.h"
#include "globals.h"
#include "ws2812b/ws2812b.h"
#include "icm20649/icm20649.h"
#include "buttons/buttons.h"
#include "state_handler/state_handler.h"
#include "utils/power_toggle.c"
#include "utils/map_value.h" // Assuming this header contains the MapMode enum and map_value function

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

/* Mapping mode (set as needed) */
#define MAPPING_MODE MAP_MODE_UNSIGNED

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
            mapped_accel_data[i] = map_value(accel_data[i], -2.0f, 2.0f, 0.0f, 255.0f, MAPPING_MODE);
            mapped_gyro_data[i] = map_value(gyro_data[i], -30.0f, 30.0f, 0.0f, 255.0f, MAPPING_MODE);
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
