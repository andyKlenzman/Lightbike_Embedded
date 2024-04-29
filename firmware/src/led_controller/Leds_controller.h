#pragma once

class Leds_controller {
private:
    const static int NUM_PIXELS = 50;
    static int LEDS[NUM_PIXELS][3]; //static allows one instance



public:
    Leds_controller();
    int getLEDValue(int index, int colorIndex) const;
    int get_num_pixels();
    void updateLEDs();


};

