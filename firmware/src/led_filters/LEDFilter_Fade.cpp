#include "LEDFilter.h"
#include "defines.h"

class LEDFilter_Fade : public LEDFilter {
public:
    LEDFilter_Fade() : LEDFilter() {
        for (int i = 0; i < 3; i++) {
            faded_values[i] = 0;

        }
    }
private:
    static constexpr float FADE_FACTOR_PER_CYCLE = 1; // Adjust smoothing factor as needed
    uint8_t faded_values[3];

    int apply_filter() override {


    }


};