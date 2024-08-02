#include "LEDFilter.h"

class LEDFilter_Basic : public LEDFilter {
public:
    LEDFilter_Basic() {
        for (int i = 0; i < 3; i++) {
            smooth_values_accel[i] = 0;
            smooth_values_gyro[i] = 0;
        }
    }

    void apply_filter() override {
        // Smoothly update smooth_values based on accelerometer and gyroscope data
        for (int i = 0; i < 3; i++) {
            // Smoothly adjust smooth_values_accel towards p_mapped_accel_data
            smooth_values_accel[i] = (smooth_values_accel[i] * SMOOTHING_FACTOR + p_mapped_accel_data[i]) / (SMOOTHING_FACTOR + 1);

            // Smoothly adjust smooth_values_gyro towards p_mapped_gyro_data
            smooth_values_gyro[i] = (smooth_values_gyro[i] * SMOOTHING_FACTOR + p_mapped_gyro_data[i]) / (SMOOTHING_FACTOR + 1);
        }

        // Set LED colors based on smoothed values, combining both accelerometer and gyroscope influences
        for (int i = 0; i < NUM_PIXELS; i++) {
            p_virtual_leds[i][0] = (smooth_values_accel[0] + smooth_values_gyro[0]) / 2;
            p_virtual_leds[i][1] = (smooth_values_accel[1] + smooth_values_gyro[1]) / 2;
            p_virtual_leds[i][2] = (smooth_values_accel[2] + smooth_values_gyro[2]) / 2;
        }
    }

private:
    static constexpr float SMOOTHING_FACTOR = 3.9; // Adjust smoothing factor as needed
    uint8_t smooth_values_accel[3];
    uint8_t smooth_values_gyro[3];
};
