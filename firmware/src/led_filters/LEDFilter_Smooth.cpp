#pragma once
#include "LEDFilter.h"
#include <cmath>
#include <vector>
#include <algorithm>

class LEDFilter_Smooth : public LEDFilter {
public:
    LEDFilter_Smooth() {
        // Initialize dots with positions, speeds, and colors
        for (int i = 0; i < dot_count; ++i) {
            dots[i].position = static_cast<float>(i * 20);
            dots[i].speed = 0.1f * (i + 1);
            dots[i].color[0] = 20; // Red
            dots[i].color[1] = 70;
            dots[i].color[2] = 60;
            dots[i].direction = 1; // 1 for forward, -1 for backward
        }
        // Initialize color changes
        for (int i = 0; i < dot_count; ++i) {
            dots[i].color_target[0] = 255;
            dots[i].color_target[1] = 255;
            dots[i].color_target[2] = 255;
            dots[i].color_speed = 0.001f; // Smooth color transition speed
        }
    }

    void apply_filter() override {
        // Update dot positions and colors
        update_dots();
        update_colors();

        // Clear the LED strip
        std::fill(&p_virtual_leds[0][0], &p_virtual_leds[0][0] + NUM_PIXELS * 3, 0);

        // Draw dots with fading tails
        for (int i = 0; i < NUM_PIXELS; ++i) {
            for (int d = 0; d < dot_count; ++d) {
                float pos = dots[d].position;
                float distance = fabs(i - pos);
                if (distance < TAIL_LENGTH) {
                    float intensity = 1.0f - (distance / TAIL_LENGTH);
                    p_virtual_leds[i][0] += static_cast<uint8_t>(dots[d].color[0] * intensity);
                    p_virtual_leds[i][1] += static_cast<uint8_t>(dots[d].color[1] * intensity);
                    p_virtual_leds[i][2] += static_cast<uint8_t>(dots[d].color[2] * intensity);
                }
            }
        }
    }

private:
    static const int dot_count = 3;     // Number of dots
    float TAIL_LENGTH = 5.0f; // Length of the fading tail



    struct Dot {
        float position; // Position on the strip (0 to NUM_PIXELS-1)
        float speed;    // Speed of the dot
        int direction;  // Direction of movement (1 or -1)
        uint8_t color[3]; // Current color of the dot
        uint8_t color_target[3]; // Target color for smooth transition
        float color_speed; // Speed of color transition
    };

    Dot dots[dot_count]; // Array to hold dots



    // Function to update dot positions
    void update_dots() {
        for (int d = 0; d < dot_count; ++d) {
            dots[d].position += dots[d].speed * dots[d].direction;
            if (dots[d].position >= NUM_PIXELS || dots[d].position < 0) {
                dots[d].direction *= -1; // Reverse direction when reaching limits
                // Optional: change color target randomly
//                dots[d].color_target[0] = static_cast<uint8_t>(rand() % 256);
//                dots[d].color_target[1] = static_cast<uint8_t>(rand() % 256);
//                dots[d].color_target[2] = static_cast<uint8_t>(rand() % 256);
            }
        }
    }

    // Function to update dot colors
    // Function to update dot colors
    void update_colors() {
        for (int d = 0; d < dot_count; ++d) {
            for (int c = 0; c < 3; ++c) {
                uint8_t current_color = dots[d].color[c];
                uint8_t target_color = dots[d].color_target[c];
                if (current_color < target_color) {
                    dots[d].color[c] = std::min<uint8_t>(current_color + static_cast<uint8_t>(dots[d].color_speed * 255), target_color);
                } else {
                    dots[d].color[c] = std::max<uint8_t>(current_color - static_cast<uint8_t>(dots[d].color_speed * 255), target_color);
                }
            }
        }
    }

};
