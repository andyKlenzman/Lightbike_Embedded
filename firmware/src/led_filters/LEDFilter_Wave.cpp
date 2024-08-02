#include "LEDFilter.h"
#include <cmath>

class LEDFilter_Wave : public LEDFilter {
public:
    LEDFilter_Wave() {
        // Initialize smooth values and wave position
        for (int i = 0; i < 3; i++) {
            smooth_values_accel[i] = 0;
            smooth_values_gyro[i] = 0;
        }
        wave_position = 0;
    }

    void apply_filter() override {
        // Smoothly update smooth_values based on accelerometer and gyroscope data
        for (int i = 0; i < 3; i++) {
            smooth_values_accel[i] = (smooth_values_accel[i] * SMOOTHING_FACTOR + p_mapped_accel_data[i]) / (SMOOTHING_FACTOR + 1);
            smooth_values_gyro[i] = (smooth_values_gyro[i] * SMOOTHING_FACTOR + p_mapped_gyro_data[i]) / (SMOOTHING_FACTOR + 1);
        }

        // Calculate wave frequency and amplitude based on smoothed sensor data
        float wave_frequency = (smooth_values_gyro[0] + smooth_values_gyro[1] + smooth_values_gyro[2]) / 3.0f * WAVE_FREQUENCY_FACTOR;
        float wave_amplitude = (smooth_values_accel[0] + smooth_values_accel[1] + smooth_values_accel[2]) / 3.0f * WAVE_AMPLITUDE_FACTOR;

        // Update the wave position
        wave_position += wave_frequency;
        if (wave_position > 2 * M_PI) wave_position -= 2 * M_PI;

        // Set LED colors with a wave effect
        for (int i = 0; i < NUM_PIXELS; i++) {
            float wave_value = sinf(wave_position + (2 * M_PI * i / NUM_PIXELS)) * wave_amplitude;
            uint8_t base_color[3] = {smooth_values_accel[0], smooth_values_accel[1], smooth_values_accel[2]};

            // Calculate a color shift for variety
            uint8_t color_shift[3] = {
                    static_cast<uint8_t>(base_color[0] * (1 + wave_value)),
                    static_cast<uint8_t>(base_color[1] * (1 + wave_value)),
                    static_cast<uint8_t>(base_color[2] * (1 + wave_value))
            };

            // Ensure values stay within valid ranges
            p_virtual_leds[i][0] = color_shift[0] % 256;
            p_virtual_leds[i][1] = color_shift[1] % 256;
            p_virtual_leds[i][2] = color_shift[2] % 256;
        }
    }

private:
    static constexpr float SMOOTHING_FACTOR = 3.9;      // Smoothing factor for sensor data
    static constexpr float WAVE_FREQUENCY_FACTOR = 0.005; // Frequency factor for wave movement
    static constexpr float WAVE_AMPLITUDE_FACTOR = 0.003; // Amplitude factor for wave intensity

    uint8_t smooth_values_accel[3]; // Smoothed acceleration data
    uint8_t smooth_values_gyro[3];  // Smoothed gyroscope data
    float wave_position;            // Current position in the wave cycle
};
