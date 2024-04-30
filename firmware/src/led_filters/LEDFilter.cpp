#include "LEDFilter.h"
#include "logging.h"

LOG_MODULE(LEDFilter.cpp)

LEDFilter::LEDFilter() {
    int result = led_strip_init(NUM_PIXELS);
    if (result == -1) {
        LOG_ERROR("LED init failed.");
    }

    result = icm_20649_init();
    if (result == -1) {
        LOG_ERROR("ICM init failed.");
    }
}


int LEDFilter::update_motion_values() {
    int result = icm_20649_read_accel_data(accel_data); // what to do with the error, I just want to tell the developer

    if(result == -1){
        LOG_DEBUG("Failed to update accelerometer values.");
        return -1;
    }

    return 0;
}

