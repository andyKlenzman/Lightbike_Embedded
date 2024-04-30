#pragma once
#include <cstdint>

class LEDFilter {
private:
    const static int NUM_PIXELS = 50;
    static uint8_t virtual_leds[NUM_PIXELS][3];


public:
    LEDFilter();
    virtual int apply_filter () = 0;

};

