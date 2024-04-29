#include <stdint.h>

uint8_t map_value(float value) {
    // Ensure the input value is within the range 0.0 to 30.0
    if (value < 0.0f) {
        value = 0.0f;
    } else if (value > 30.0f) {
        value = 30.0f;
    }

    // Calculate the mapped value using linear mapping
    // Formula: mapped_value = (input_value - input_min) * (output_max - output_min) / (input_max - input_min) + output_min
    uint8_t mapped_value = (value - 0.0f) * (255 - 0) / (30.0f - 0.0f) + 0;

    return mapped_value;
}