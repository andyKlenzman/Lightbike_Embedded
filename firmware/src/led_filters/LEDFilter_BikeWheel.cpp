#include "LEDFilter.h"
#include <cmath> // For sin, floor, and M_PI

class LEDFilter_BicycleWheel : public LEDFilter {
public:
    LEDFilter_BicycleWheel() = default;


    void apply_filter() override {

        for (int i = 0; i < NUM_PIXELS; i = i + 2) {
            p_virtual_leds[i][0] = p_mapped_accel_data[0] % 256;
            p_virtual_leds[i][1] = p_mapped_accel_data[1] % 256;
            p_virtual_leds[i][2] = p_mapped_accel_data[2] % 256;
        }
        for (int i = 1; i < NUM_PIXELS; i = i + 2) {
            p_virtual_leds[i][0] = p_mapped_gyro_data[0] % 256;
            p_virtual_leds[i][1] = p_mapped_gyro_data[1] % 256;
            p_virtual_leds[i][2] = p_mapped_gyro_data[2] % 256;
        }

    }

private:

};
