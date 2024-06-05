#include "LEDFilter.h"


class LEDFilter_Basic : public LEDFilter {
public:
    void apply_filter() override {
        for(int i = 0; i < 50 ; i++){
            p_virtual_leds[i][0] = p_accel_data[0];
            p_virtual_leds[i][1] = p_accel_data[1];
            p_virtual_leds[i][2] = p_accel_data[2];
        }
    }

};

