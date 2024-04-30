#include "LEDFilter.h"


class LEDFilter_Smooth : public LEDFilter {
public:
    int apply_filter() override {
        for(int i = 0; i < NUM_PIXELS ; i++){
            led_strip_set_led(i, accel_data[0],accel_data[1],accel_data[2]);
        }
        return 0;
    }
};

