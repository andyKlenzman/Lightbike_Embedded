#pragma once
#include <cstdint>
#include "../drivers/icm20649.h"
#include "../drivers/ws2812b.h"
#include "../defines.h"


class LEDFilter {
public:
    LEDFilter();
    virtual int apply_filter () = 0;




protected:
    static uint8_t *p_accel_data;
    static uint8_t (*p_virtual_leds)[3];
};




