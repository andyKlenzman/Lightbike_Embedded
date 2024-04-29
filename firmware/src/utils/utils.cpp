#include "coldwave/coldwave.h"

/*value means brightness*/
void convertSpeedToValue() {

}

void regulateFPS(uint32_t ts) {
    osDelayUntil(ts+ 500);
}

void convertHUEandVALtoRGB() {
    /* this function will take in the hue and value data retrieved above, and convert it into a single
     * pixel data point, which will be pushed to the LED in the first version */

};

/*can use this to attain values for hue from direction and value from velocity */
double mapValueto255(double value, double inMin, double inMax) {
    value = std::max(inMin, std::min(value, inMax));
    return 0 + (value - inMin) * 255 / (inMax - inMin);
}



