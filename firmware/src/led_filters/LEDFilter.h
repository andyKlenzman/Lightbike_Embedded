#pragma once
#include <cstdint>
#include "drivers/ws2812b.h"
#include "drivers/icm20649.h"
#include "defines.h"
class LEDFilter {
public:
    explicit LEDFilter(uint8_t* accel_values); // Constructor with parameter
    virtual int apply_filter () = 0;
    int update_motion_values(); // how should this be doecoupled from the LEDs
    static uint8_t virtual_leds[NUM_PIXELS][3]; // Pointer to the virtual_leds array


protected:
    static uint8_t* accel_values;

};




