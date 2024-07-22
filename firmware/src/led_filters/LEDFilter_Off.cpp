#include "LEDFilter.h"

class LEDFilter_Off : public LEDFilter {
public:
    LEDFilter_Off() = default;

    void apply_filter() override {
        // Iterate over all LEDs and set their RGB values to 0
        if (p_virtual_leds) {
            for (int i = 0; i < NUM_PIXELS; ++i) {
                p_virtual_leds[i][0] = 0;  // Set Red component to 0
                p_virtual_leds[i][1] = 0;  // Set Green component to 0
                p_virtual_leds[i][2] = 0;  // Set Blue component to 0
            }
            // Optionally, send the updated LED data to the hardware
            // ws2812b_update(p_virtual_leds, NUM_PIXELS);
        }
    }
};