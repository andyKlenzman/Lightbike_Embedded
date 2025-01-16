#include <kernel.h>
#include <algorithm>


#include "ws2812b/ws2812b.h"
#include "icm20649/icm20649.h"
#include "buttons/buttons.h"

#include "logging.h"
#include "globals.h"


#include "filter_handler/filter_handler.h"
#include "utils/map_value.h"


#include "led_filters/LEDFilter.h"
#include <cli/cli.h>



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
volatile uint8_t device_status_field = 0;


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

//
volatile bool flag_toggle_system_power = false;


void process_data ();

int main(void)
{
    int result;

    /* Initialize components */
    if (led_strip_init(NUM_PIXELS) == -1) {
        LOG_ERROR("LED init failed.");
    } else {
        LOG_DEBUG("LED init succeeded.");
    }

    if (icm_20649_init() == -1) {
        LOG_ERROR("ICM init failed.");
    } else {
        LOG_DEBUG("ICM init succeeded.");
    }

    if (button_init(GPIO_PUSH_BTN_1, mode_button_irq_function) == -1) {
        LOG_ERROR("Mode button init failed.");
    } else {
        LOG_DEBUG("Mode button init succeeded.");
    }

    if (button_init(GPIO_PUSH_BTN_2, power_button_irq_function)== -1) {
        LOG_ERROR("Power button init failed.");
    } else {
        LOG_DEBUG("Power button init succeeded.");
    }


    cli_init(0);

    uint32_t last_execution_time = 0;

    while (1)
    {
        uint32_t current_time = osKernelGetTickCount();
        uint32_t elapsed_time = current_time - last_execution_time;

        // ToDo: fügen Zahlung von Frame Time Vio
        if (elapsed_time >= FRAME_TIME_MS)
        {
            last_execution_time = current_time;

            if (DEVICE_STATUS_GET(device_status_field, status_on))
            {

                /* Read sensor data */
                if ((result = icm_20649_read_gyro_data(gyro_data)) == -1)
                {
                    // LOG_ERROR("icm_20649_read_gyro_data failed.");
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
                call_current_led_filter(); //ToDo: switch to a switch case.

                /* Push LED values created inside the LED_filter to the LEDs */
                set_leds(virtual_leds);
                update_leds();
            } else
            {

                if(!DEVICE_STATUS_GET(device_status_field, status_cleared))
                {
                    clear_leds(virtual_leds);
                    update_leds();
                    device_status_field |= DEVICE_STATUS_MASK(status_cleared);

                }

                osDelay(100);

            }
        }
    }
}


void process_data (){
    /* The smoothing function blends the new value with the previous. The higher the smoothing_factor,
     * the more weight that is given to the previous value. */
    for (int i = 0; i < 3; i++)
    {
        smooth_accel_data[i] = (smooth_accel_data[i] * FILTER_SMOOTHING_FACTOR + accel_data[i]) / (FILTER_SMOOTHING_FACTOR + 1);
        smooth_gyro_data[i] = (smooth_gyro_data[i] * FILTER_SMOOTHING_FACTOR + gyro_data[i]) / (FILTER_SMOOTHING_FACTOR + 1);
    }


    for (int i = 0; i < 3; i++)
    {
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