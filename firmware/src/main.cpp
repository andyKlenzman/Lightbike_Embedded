#include <kernel.h>
#include <algorithm>
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
float smooth_accel_data[3];
float smooth_gyro_data[3];
uint8_t mapped_accel_data[3];
uint8_t mapped_gyro_data[3];
uint8_t magnitude_mapped_accel_data;
uint8_t magnitude_mapped_gyro_data;
uint8_t virtual_leds[NUM_PIXELS][3];
uint8_t hsv_virtual_leds[NUM_PIXELS][3];


/* Pointers for LEDFilter access */
float *LEDFilter::p_accel_data = accel_data;
float *LEDFilter::p_gyro_data = gyro_data;

float *LEDFilter::p_smooth_accel_data = smooth_accel_data;
float *LEDFilter::p_smooth_gyro_data = smooth_gyro_data;

uint8_t *LEDFilter::p_mapped_accel_data = mapped_accel_data;
uint8_t *LEDFilter::p_mapped_gyro_data = mapped_gyro_data;

uint8_t *LEDFilter::p_magnitude_mapped_accel_data = &magnitude_mapped_accel_data;
uint8_t *LEDFilter::p_magnitude_mapped_gyro_data = &magnitude_mapped_gyro_data;

uint8_t (*LEDFilter::p_virtual_leds)[3] = virtual_leds;
uint8_t (*LEDFilter::p_hsv_virtual_leds)[3] = hsv_virtual_leds;


/* Power toggle flag */
volatile bool flag_toggle_system_power = false;
volatile bool is_system_on = true;

void process_data ();

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

    uint32_t last_execution_time = 0;

    while (1) {
        uint32_t current_time = osKernelGetTickCount();
        uint32_t elapsed_time = current_time - last_execution_time;

        // Check if it's time to execute the next frame
        if (elapsed_time >= FRAME_TIME_MS) {
            last_execution_time = current_time;


            if (!is_system_on) {

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

                /* Process sensor data into useful values. Available to all LED_Filters */
                process_data();


                /* Apply the current filter as determined by the filter handler. */
                call_current_led_filter();

                /* Push LED values created inside the LED_filter to the LEDs */
                set_leds(virtual_leds);
                update_leds();
            }

            /* When the flag is set, clear LEDs and toggle system power so above functions in while loop are skipped. */
            if (flag_toggle_system_power) {
                clear_leds(virtual_leds);
                update_leds();
                toggle_power();
                flag_toggle_system_power = false;


            }
        }



    }
}

/* Process sensor data into useful values. Available to all LED_Filters
 * ADD
 * - smooth values, to be used for others
 *
 *
 * Description:
 * The mapped values are fed with the smoothing values, not the raw accelerometer data.
 *
 * */



void process_data (){
    /* The smoothing function blends the new value with the previous. The higher the smoothing_factor,
     * the more weight that is given to the previous value. */
    for (int i = 0; i < 3; i++) {
        smooth_accel_data[i] = (smooth_accel_data[i] * FILTER_SMOOTHING_FACTOR + accel_data[i]) / (FILTER_SMOOTHING_FACTOR + 1);
        smooth_gyro_data[i] = (smooth_gyro_data[i] * FILTER_SMOOTHING_FACTOR + gyro_data[i]) / (FILTER_SMOOTHING_FACTOR + 1);
    }


    for (int i = 0; i < 3; i++) {
        mapped_accel_data[i] = map_value(smooth_accel_data[i], ACCEL_MAP_IN_MIN, ACCEL_MAP_IN_MAX, ACCEL_MAP_OUT_MIN, ACCEL_MAP_OUT_MAX, MAPPING_MODE);
        mapped_gyro_data[i] = map_value(smooth_gyro_data[i], GYRO_MAP_IN_MIN, GYRO_MAP_IN_MAX, GYRO_MAP_OUT_MAX, GYRO_MAP_OUT_MIN, MAPPING_MODE);
    }

    // Calculate signal magnitude for accel data

    uint16_t magnitude_accel_data_bucket = 0;
    uint16_t magnitude_gyro_data_bucket = 0;

    for (int i = 0; i < 3; i++) {
        magnitude_accel_data_bucket += mapped_accel_data[i];
        magnitude_gyro_data_bucket += mapped_gyro_data[i];
    }

    // Compute averages and ensure they fit in a uint8_t range
    magnitude_mapped_accel_data = static_cast<uint8_t>(std::clamp(magnitude_accel_data_bucket / 3, 0, 255));
    magnitude_mapped_gyro_data = static_cast<uint8_t>(std::clamp(magnitude_gyro_data_bucket / 3, 0, 255));
}