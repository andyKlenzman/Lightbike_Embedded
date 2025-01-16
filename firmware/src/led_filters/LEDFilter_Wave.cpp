#include "LEDFilter.h"
#include <cmath>

class LEDFilter_Wave : public LEDFilter {
public:
    LEDFilter_Wave(float smoothing_factor, float wave_frequency_factor, float wave_amplitude_factor)
            : smoothing_factor(smoothing_factor),
              wave_frequency_factor(wave_frequency_factor),
              wave_amplitude_factor(wave_amplitude_factor),
              wave_position(0) {
        // Initialize filtered sensor data
        for (int i = 0; i < 3; i++) {
            filtered_sensor_data[i] = 0;
        }
    }

    void apply_filter() override {


        // Calculate wave parameters based on filtered sensor data
        float wave_frequency = (filtered_sensor_data[0] + filtered_sensor_data[1] + filtered_sensor_data[2]) / 3.0f * wave_frequency_factor;
        float wave_amplitude = wave_frequency * wave_amplitude_factor;

        // Update wave position
        wave_position += wave_frequency;
        if (wave_position > 2 * M_PI) wave_position -= 2 * M_PI;

        // Apply wave effect to LEDs
        for (int i = 0; i < NUM_PIXELS; i++) {
            float wave_value = sinf(wave_position + (2 * M_PI * i / NUM_PIXELS)) * wave_amplitude;

            uint8_t base_color[3] = {
                    static_cast<uint8_t>(filtered_sensor_data[0]),
                    static_cast<uint8_t>(filtered_sensor_data[1]),
                    static_cast<uint8_t>(filtered_sensor_data[2])
            };

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
    float smoothing_factor;      // Smoothing factor for sensor data
    float wave_frequency_factor; // Frequency factor for wave movement
    float wave_amplitude_factor; // Amplitude factor for wave intensity
    float filtered_sensor_data[3]; // Combined filtered sensor data (accelerometer + gyroscope)
    float wave_position;         // Current position in the wave cycle
};
