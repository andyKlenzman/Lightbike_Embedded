#include "LEDFilter.h"
#include <cmath> // For sin, floor, and M_PI

class LEDFilter_BicycleWheel : public LEDFilter {
public:
    LEDFilter_BicycleWheel(float offset_factor)
            : offset_factor(offset_factor) {}

    void apply_filter() override {
        float offset = offset_factor * M_PI;

        for (int i = 0; i < NUM_PIXELS; i++) {
            float wave_value = sinf(offset + (2 * M_PI * i / NUM_PIXELS));

            if (i % 2 == 0)
            {
                p_virtual_leds[i][0] = static_cast<uint8_t>(static_cast<uint8_t>((filter_sensor_data[0]) * (1 + wave_value)) % 256);
                p_virtual_leds[i][1] = static_cast<uint8_t>(static_cast<uint8_t>((filter_sensor_data[1]) * (1 + wave_value)) % 256);
                p_virtual_leds[i][2] = static_cast<uint8_t>(static_cast<uint8_t>((filter_sensor_data[2]) * (1 + wave_value)) % 256);
            } else
            {
                // Use gyro data for odd indices
                p_virtual_leds[i][0] = static_cast<uint8_t>(filter_sensor_data[0] * (1 - wave_value)) % 256;
                p_virtual_leds[i][1] = static_cast<uint8_t>(filter_sensor_data[1] * (1 - wave_value)) % 256;
                p_virtual_leds[i][2] = static_cast<uint8_t>(filter_sensor_data[2] * (1 - wave_value)) % 256;
            }
        }
    }

private:
    float offset_factor; // Offset factor for the wave effect
};
