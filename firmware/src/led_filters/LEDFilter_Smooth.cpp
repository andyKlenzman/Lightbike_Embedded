#pragma once
#include "LEDFilter.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include "utils/hsv_to_rgb.cpp"

class LEDFilter_Smooth : public LEDFilter {
public:
    LEDFilter_Smooth(float star_frequency_factor, float fade_speed)
            : star_frequency_factor(star_frequency_factor), fade_speed(fade_speed) {
        // Seed random number generator
        std::srand(static_cast<unsigned int>(std::time(0)));

        // Initialize LED colors to black
        std::fill(&p_hsv_virtual_leds[0][0], &p_hsv_virtual_leds[0][0] + NUM_PIXELS * 3, 0);
    }

    void apply_filter() override {
        // Update star frequency based on gyroscope data and factor
        star_frequency = static_cast<float>(*p_magnitude_mapped_gyro_data) * star_frequency_factor;
        star_timer += star_frequency;

        // Check if it's time to create a new star
        if (star_timer >= 1.0f) {
            create_star();
            star_timer -= 1.0f; // Reset timer, but keep the fraction for smooth timing
        }

        // Update star brightness and convert HSV to RGB
        update_stars();
    }

private:
    float star_timer = 0.0f; // Timer for creating new stars
    float star_frequency; // Frequency of new stars
    float fade_speed; // Speed of fading effect
    float star_frequency_factor;

    void create_star() {
        int position = std::rand() % NUM_PIXELS; // Random position
        uint8_t hue = *p_magnitude_mapped_accel_data; // Hue based on accel data

        // Set the new star's properties
        p_hsv_virtual_leds[position][0] = hue; // Hue
        p_hsv_virtual_leds[position][1] = 255; // Saturation
        p_hsv_virtual_leds[position][2] = 255; // Value - bright LED
    }

    void update_stars() {
        // Convert HSV to RGB and apply fading
        for (int i = 0; i < NUM_PIXELS; ++i) {
            uint8_t& hue = p_hsv_virtual_leds[i][0];
            uint8_t& saturation = p_hsv_virtual_leds[i][1];
            uint8_t& value = p_hsv_virtual_leds[i][2];

            // Convert HSV to RGB
            hsv_to_rgb(hue, saturation, value, p_virtual_leds[i][0], p_virtual_leds[i][1], p_virtual_leds[i][2]);

            // Update the HSV value for fading
            if (value <= fade_speed) {
                p_hsv_virtual_leds[i][2] = 0; // Ensure value doesn't go negative
            } else {
                p_hsv_virtual_leds[i][2] -= static_cast<uint8_t>(fade_speed);
            }
        }
    }
};
