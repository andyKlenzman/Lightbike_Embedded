#include "LEDFilter.h"


class LEDFilter_Smooth : public LEDFilter {
public:
    LEDFilter_Smooth() : LEDFilter() {
        // Initialize smooth_values to zero
        for (int i = 0; i < 3; i++) {
            smooth_values[i] = 0;
        }
    }

    int apply_filter() override {
        // Smoothly update smooth_values based on accelerometer data
        for (int i = 0; i < 3; i++) {
            // Smoothly adjust smooth_values towards accel_data
            smooth_values[i] = (smooth_values[i] * SMOOTHING_FACTOR + accel_data[i]) / (SMOOTHING_FACTOR + 1);
        }

        // Set LED colors based on smoothed values
        for (int i = 0; i < 50 ; i++){
            virtual_leds[i][0] = smooth_values[0];
            virtual_leds[i][1] = smooth_values[1];
            virtual_leds[i][2] = smooth_values[2];
        }
        return 0;
    }

private:
    static constexpr float SMOOTHING_FACTOR = 2.9; // Adjust smoothing factor as needed
    uint8_t smooth_values[3];
};