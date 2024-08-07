#include "LEDFilter.h"
#include <cmath> // For sin, floor, and M_PI

class LEDFilter_BicycleWheel : public LEDFilter {
public:
    LEDFilter_BicycleWheel(float smoothing_factor,
                          float wave_frequency_factor,
                          float wave_amplitude_factor)
            : smoothing_factor(smoothing_factor),
              wave_frequency_factor(wave_frequency_factor),
              wave_amplitude_factor(wave_amplitude_factor),
              wave_position(0) {
        // Initialize smoothing values
        for (int i = 0; i < 3; i++) {
            smooth_values_accel[i] = 0;
            smooth_values_gyro[i] = 0;
        }
    }

    void apply_filter() override {
        // Smoothly update smooth_values based on accelerometer and gyroscope data
        for (int i = 0; i < 3; i++) {
            smooth_values_accel[i] = (smooth_values_accel[i] * smoothing_factor + p_mapped_accel_data[i]) / (smoothing_factor + 1);
            smooth_values_gyro[i] = (smooth_values_gyro[i] * smoothing_factor + p_mapped_gyro_data[i]) / (smoothing_factor + 1);
        }

        // Calculate wave frequency and amplitude based on smoothed sensor data
        float wave_frequency = (smooth_values_gyro[0] + smooth_values_gyro[1] + smooth_values_gyro[2]) / 3.0f * wave_frequency_factor;
        float wave_amplitude = (smooth_values_accel[0] + smooth_values_accel[1] + smooth_values_accel[2]) / 3.0f * wave_amplitude_factor;

        // Update the wave position
        wave_position += wave_frequency;
        if (wave_position > 2 * M_PI) wave_position -= 2 * M_PI;

        // Create a rainbow wave pattern
        for (int i = 0; i < NUM_PIXELS; i++) {
            float wave_value = sinf(wave_position + (2 * M_PI * i / NUM_PIXELS)) * wave_amplitude;
            float hue = (i / static_cast<float>(NUM_PIXELS) * 360.0f + wave_position * 180.0f / M_PI) / 360.0f * 360.0f;
            float r, g, b;
            HSVtoRGB(hue, 1.0f, 1.0f, r, g, b);

            // Adjust the intensity based on the wave value
            float intensity = 0.5f + 0.5f * wave_value;
            p_virtual_leds[i][0] = static_cast<uint8_t>(r * intensity * 255);
            p_virtual_leds[i][1] = static_cast<uint8_t>(g * intensity * 255);
            p_virtual_leds[i][2] = static_cast<uint8_t>(b * intensity * 255);
        }
    }

private:
    float smoothing_factor;      // Smoothing factor for sensor data
    float wave_frequency_factor; // Frequency factor for wave movement
    float wave_amplitude_factor; // Amplitude factor for wave intensity

    uint8_t smooth_values_accel[3]; // Smoothed acceleration data
    uint8_t smooth_values_gyro[3];  // Smoothed gyroscope data
    float wave_position;            // Current position in the wave cycle

    // Convert HSV to RGB
    void HSVtoRGB(float h, float s, float v, float &r, float &g, float &b) {
        int i;
        float f, p, q, t;

        if (s == 0) {
            r = g = b = v;
            return;
        }

        h /= 60;
        i = std::floor(h);
        f = h - i;
        p = v * (1 - s);
        q = v * (1 - s * f);
        t = v * (1 - s * (1 - f));

        switch (i) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            default: r = v; g = p; b = q; break;
        }
    }
};
