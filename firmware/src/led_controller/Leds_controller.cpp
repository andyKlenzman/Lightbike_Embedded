#include "Leds_controller.h"
#include "drivers/ws2812b.h"

Leds_controller::Leds_controller()  {
    led_strip_init(NUM_PIXELS);
}

int Leds_controller::LEDS[NUM_PIXELS][3];


int Leds_controller::getLEDValue(int index, int colorIndex) const {
    return LEDS[index][colorIndex];
}

void Leds_controller::updateLEDs(){
    for (int i = 0; i <= NUM_PIXELS; ++i) {
        led_strip_set_led(i, LEDS[i][0], LEDS[i][1], LEDS[i][2]);
    };
};

