#pragma once
#include <cstdint>  // For uint8_t
#include <cmath>

// Function to convert HSV to RGB
static void hsv_to_rgb(uint8_t h, uint8_t s, uint8_t v, uint8_t &r, uint8_t &g, uint8_t &b) {
    // Normalize HSV values to [0, 1] range
    float hue = h / 255.0f;
    float saturation = s / 255.0f;
    float value = v / 255.0f;

    // Convert HSV to RGB
    float c = value * saturation;
    float x = c * (1.0f - fabs(std::fmod(hue * 6.0f, 2.0f) - 1.0f));
    float m = value - c;

    float r1, g1, b1;

    if (hue < 1.0f / 6.0f) {
        r1 = c; g1 = x; b1 = 0;
    } else if (hue < 2.0f / 6.0f) {
        r1 = x; g1 = c; b1 = 0;
    } else if (hue < 3.0f / 6.0f) {
        r1 = 0; g1 = c; b1 = x;
    } else if (hue < 4.0f / 6.0f) {
        r1 = 0; g1 = x; b1 = c;
    } else if (hue < 5.0f / 6.0f) {
        r1 = x; g1 = 0; b1 = c;
    } else {
        r1 = c; g1 = 0; b1 = x;
    }

    r = static_cast<uint8_t>((r1 + m) * 255.0f);
    g = static_cast<uint8_t>((g1 + m) * 255.0f);
    b = static_cast<uint8_t>((b1 + m) * 255.0f);
}
