#include <stdint.h>

uint8_t map_value(float value, float in_min, float in_max, uint8_t out_min, uint8_t out_max) {
    // Ensure the input value is within the range in_min to in_max
    if (value < in_min) {
        value = in_min;
    } else if (value > in_max) {
        value = in_max;
    }

    // Calculate the mapped value using linear mapping
    // Formula: mapped_value = (input_value - input_min) * (output_max - output_min) / (input_max - input_min) + output_min
    uint8_t mapped_value = (uint8_t)((value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);

    return mapped_value;
}
