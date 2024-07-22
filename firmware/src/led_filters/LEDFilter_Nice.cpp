#pragma once
#include "LEDFilter.h"
#include <math.h>
#include "kernel.h"


class LEDFilter_Nice : public LEDFilter {
public:
    LEDFilter_Nice() {
        for (int i = 0; i < 3; i++) {
            smooth_values[i] = 0;
        }
        pulse_rate = 2.1f;  // Pulse rate, adjust for faster/slower pulses
    }

    void apply_filter() override {
        // Smoothly update smooth_values based on accelerometer data
        for (int i = 0; i < 3; i++) {
            smooth_values[i] = (smooth_values[i] * SMOOTHING_FACTOR + p_accel_data[i]) / (SMOOTHING_FACTOR + 1);
        }

        // Calculate pulse effect
        float pulse_value = (sin(pulse_rate * millis()) + 1.0f) / 2.0f;  // Pulsates between 0 and 1

        // Set LED colors based on smoothed values and pulse effect
        for (int i = 0; i < 50; i++) {
            // Apply pulse effect to color intensity
            p_virtual_leds[i][0] = smooth_values[0] * pulse_value;
            p_virtual_leds[i][1] = smooth_values[1] * pulse_value;
            p_virtual_leds[i][2] = smooth_values[2] * pulse_value;

            // Optionally, create a gradient effect based on position
            float gradient_factor = float(i) / 49.0f;  // Linear gradient from 0 to 1
            p_virtual_leds[i][0] = (p_virtual_leds[i][0] * gradient_factor);
            p_virtual_leds[i][1] = (p_virtual_leds[i][1] * (1.0f - gradient_factor));
            p_virtual_leds[i][2] = (p_virtual_leds[i][2] * (gradient_factor * (1.0f - gradient_factor)));
        }
    }

private:
    static constexpr float SMOOTHING_FACTOR = 2.9; // Adjust smoothing factor as needed
    uint8_t smooth_values[3];
    float pulse_rate;  // Controls the speed of the pulse effect

    // Function to get the current time in milliseconds
    unsigned long millis() {
        uint32_t ts = osKernelGetTickCount();

        // You need to implement this or use your platform's millis function
        // This is a placeholder for demonstration
        return ts;
    }
};
