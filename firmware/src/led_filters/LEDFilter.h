#pragma once
#include "../drivers/icm20649.h"
#include "leds/ws2812b.h"
#include "../defines.h"

class LEDFilter {
public:
    LEDFilter() = default;
    virtual void apply_filter() = 0;

protected:
    static uint8_t* p_accel_data;
    static uint8_t (*p_virtual_leds)[3];
};