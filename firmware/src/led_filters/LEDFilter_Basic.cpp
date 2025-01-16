#include "LEDFilter.h"
#include "utils/map_value.h"

class LEDFilter_Basic : public LEDFilter {
public:
    LEDFilter_Basic() = default;

    void apply_filter() override
    {

        for (int i = 0; i < NUM_PIXELS; i++)
        {
            p_virtual_leds[i][0] = filter_sensor_data[0];
            p_virtual_leds[i][1] = filter_sensor_data[1];
            p_virtual_leds[i][2] = filter_sensor_data[2];
        }
    }

};
