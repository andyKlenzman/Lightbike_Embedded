#pragma once
#include "LEDFilter.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include "utils/hsv_to_rgb.cpp"
#include "utils/map_value.h"

class LEDFilter_Smooth : public LEDFilter {
public:
    LEDFilter_Smooth(float smoothing_factor, float fade_speed)
            : smoothing_factor(smoothing_factor), fade_speed(fade_speed) {
        // Initialize filtered data to 0
        for (int i = 0; i < 3; ++i) {
            filtered_data[i] = 0;
        }

        // Initialize LED colors to black
        std::fill(&p_hsv_virtual_leds[0][0], &p_hsv_virtual_leds[0][0] + NUM_PIXELS * 3, 0);
    }

    void apply_filter() override {
        // Smooth sensor data



        for (int i = 0; i < 3; ++i) {
            filtered_data[i] = (filtered_data[i] * smoothing_factor + filter_sensor_data[i]) / (smoothing_factor + 1);
        }

        // Create wave effect based on filtered data
        float wave_position = 0.0f;
        for (int i = 0; i < NUM_PIXELS; ++i) {
            float wave_value = sinf(wave_position + (2 * M_PI * i / NUM_PIXELS));

            // Update LED color based on smoothed data
            uint8_t hue = static_cast<uint8_t>(filtered_data[0]);
            uint8_t saturation = 255;
            uint8_t value = static_cast<uint8_t>(filtered_data[1] * (1 + wave_value));

            hsv_to_rgb(hue, saturation, value, p_virtual_leds[i][0], p_virtual_leds[i][1], p_virtual_leds[i][2]);

            wave_position += fade_speed;
        }
    }

private:
    float smoothing_factor;  // Factor for smoothing sensor data
    float fade_speed;        // Speed for wave fading
    float filtered_data[3];  // Smoothed sensor data
};
