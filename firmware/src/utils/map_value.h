#include <math.h>

typedef enum {
    MAP_MODE_SIGNED,         // 0 is the middle value, with equal positive and negative ranges
    MAP_MODE_UNSIGNED,       // 0 is the minimum value, with output increasing for positive inputs only
    MAP_MODE_SYMMETRICAL     // 0 is the midpoint, with both positive and negative inputs mapping to the same output
} MapMode;

/**
 * @brief Maps a float value from one range to another.
 *
 * @param x The input value to map.
 * @param in_min The minimum of the input range.
 * @param in_max The maximum of the input range.
 * @param out_min The minimum of the output range.
 * @param out_max The maximum of the output range.
 * @param mode The mapping mode (signed, unsigned, or symmetrical).
 * @return The mapped value.
 */
float map_value(float x, float in_min, float in_max, float out_min, float out_max, MapMode mode) {
    float mapped_value = 0;

    switch (mode) {
        case MAP_MODE_SIGNED:
            // Normalize x in the range -1 to 1
            x = (x - in_min) / (in_max - in_min) * 2.0f - 1.0f;
            // Map x to the output range
            mapped_value = (x + 1.0f) / 2.0f * (out_max - out_min) + out_min;
            break;

        case MAP_MODE_UNSIGNED:
            // Map x from in_min to in_max to out_min to out_max
            mapped_value = (x - in_min) / (in_max - in_min) * (out_max - out_min) + out_min;
            break;

        case MAP_MODE_SYMMETRICAL:
            // Normalize x in the range -1 to 1
            x = (x - in_min) / (in_max - in_min) * 2.0f - 1.0f;
            // Map the absolute value of x to the output range
            mapped_value = fabs(x) * (out_max - out_min) + out_min;
            break;

        default:
            // Handle unexpected mode value
            mapped_value = out_min; // Default to the minimum output value
            break;
    }

    return mapped_value;
}
