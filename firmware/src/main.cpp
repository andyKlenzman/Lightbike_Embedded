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

float sensor_data [3];
float *LEDFilter::filter_sensor_data = sensor_data;


uint8_t virtual_leds[NUM_PIXELS][3];
uint8_t hsv_virtual_leds[NUM_PIXELS][3];
volatile uint8_t device_status_field = 0;


uint8_t (*LEDFilter::p_virtual_leds)[3] = virtual_leds;
uint8_t (*LEDFilter::p_hsv_virtual_leds)[3] = hsv_virtual_leds;

data_source_e data_source_selection = accl;


void fetch_sensor_data (data_source_e source, float* selected_sensor_data);

int main(void)
{

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

    int result = 0;
    while (1)
    {
        uint32_t current_time = osKernelGetTickCount();
        uint32_t elapsed_time = current_time - last_execution_time;

        // ToDo: fügen Zahlung von Frame Time Verbrechungen(wort?)
        if (elapsed_time >= FRAME_TIME_MS)
        {
            last_execution_time = current_time;

            if (DEVICE_STATUS_GET(device_status_field, status_on))
            {


                fetch_sensor_data(data_source_selection, sensor_data);




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



// ToDo: Bieten nur ein Daten Punkt dem mit ein Taste drunk verfügbar wird
void fetch_sensor_data (data_source_e source, float* data) {
    int result = 0;

    /* The smoothing function blends the new value with the previous. The higher the smoothing_factor,
     * the more weight that is given to the previous value. */

    switch (source)
    {


        case accl:
            result = icm_20649_read_accel_data(data);
            if (result == -1) {
                LOG_ERROR("icm_20649_read_gyro_data failed.");
            }
            break;
        case accl_smooth:

            result = icm_20649_read_accel_data(data);
            if (result == -1) {
                LOG_ERROR("icm_20649_read_gyro_data failed.");
            }


            for (int i = 0; i < 3; i++) {
                data[i] = (data[i] * FILTER_SMOOTHING_FACTOR + data[i]) / (FILTER_SMOOTHING_FACTOR + 1);
                data[i] = (data[i] * FILTER_SMOOTHING_FACTOR + data[i]) / (FILTER_SMOOTHING_FACTOR + 1);
            }

            break;
        case gyro:

            result = icm_20649_read_gyro_data(data);
            if (result == -1) {
                LOG_ERROR("icm_20649_read_accel_data failed.");
            }

            break;
        case gyro_smooth:

            result = icm_20649_read_gyro_data(data);
            if (result == -1) {
                LOG_ERROR("icm_20649_read_accel_data failed.");
            }

            for (int i = 0; i < 3; i++) {
                data[i] = (data[i] * FILTER_SMOOTHING_FACTOR + data[i]) / (FILTER_SMOOTHING_FACTOR + 1);
                data[i] = (data[i] * FILTER_SMOOTHING_FACTOR + data[i]) / (FILTER_SMOOTHING_FACTOR + 1);
            }
            break;


    }

    map_sensor_values(data);


}



