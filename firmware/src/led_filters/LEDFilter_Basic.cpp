#include "LEDFilter.h"

class LEDFilter_Basic : public LEDFilter {
public:
    LEDFilter_Basic() = default;

    void apply_filter() override {
        // Set LED colors based on smoothed values, combining both accelerometer and gyroscope influences
        for (int i = 0; i < NUM_PIXELS; i++) {
            p_virtual_leds[i][0] = (p_mapped_accel_data[0] + p_mapped_gyro_data[0]) / 2;
            p_virtual_leds[i][1] = (p_mapped_accel_data[1] + p_mapped_gyro_data[1]) / 2;
            p_virtual_leds[i][2] = (p_mapped_accel_data[2] + p_mapped_gyro_data[2]) / 2;
        }
    }

};
