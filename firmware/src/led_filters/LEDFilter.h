#pragma once
#include <cstdint>
#include "drivers/ws2812b.h"
#include "drivers/icm20649.h"

class LEDFilter {

public:
    LEDFilter();
    virtual int apply_filter () = 0;
    int update_motion_values();

protected:
    const static int NUM_PIXELS = 50;
    static uint8_t virtual_leds[NUM_PIXELS][3]; //static can share behavior amongst call instances
    uint8_t accel_data[3]{};
};

