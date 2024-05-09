#include "LEDFilter.h"


class LEDFilter_Basic : public LEDFilter {
public:
    int apply_filter() override {
        for(int i = 0; i < 50 ; i++){
            virtual_leds[i][0] = accel_data[0];
            virtual_leds[i][1] = accel_data[1];
            virtual_leds[i][2] = accel_data[2];
        }
        return 0;
    }

};

