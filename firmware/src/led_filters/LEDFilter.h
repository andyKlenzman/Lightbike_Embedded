#pragma once

#include "icm20649/icm20649.h"
#include "ws2812b/ws2812b.h"
#include "globals.h"

/**
 * @brief Abstract base class for LED filters.
 *
 * The `LEDFilter` class serves as an abstract base class for implementing various
 * filters that can be applied to LED data. It provides a common interface for
 * applying filters, which derived classes must implement.
 *
 * This file shows the values that are available to each filter, supporting easy and
 * consistent Filter development experience, and controlling memory usage by constriciting
 *  each filter to access the same addresses.
 *
 */
class LEDFilter {
public:
    /**
     * @brief Default constructor.
     *
     * Initializes the `LEDFilter` object. The default constructor does not perform
     * any special initialization.
     */
    LEDFilter() = default;

    /**
     * @brief Pure virtual function to apply the filter.
     *
     * Derived classes must implement this method to apply a specific filter to
     * the LED data. The method does not return any value and modifies the LED data
     * according to the filter's algorithm.
     */
    virtual void apply_filter() = 0;

protected:

    /**
     * @brief Pointer to the array of acceleration data.
     *
     * This static member points to an array containing acceleration data. Derived
     * classes can use this pointer to access or modify the acceleration data.
     */
    static float* p_accel_data;
    static float* p_gyro_data;

    static float* p_smooth_accel_data;
    static float* p_smooth_gyro_data;

    static uint8_t* p_mapped_accel_data;
    static uint8_t* p_mapped_gyro_data;

    static uint8_t* p_magnitude_mapped_accel_data;
    static uint8_t* p_magnitude_mapped_gyro_data;


    /**
     * @brief Pointer to the array of virtual LED data.
     *
     * This static member points to a 2D array where each row contains the RGB values
     * of a virtual LED. Derived classes can use this pointer to access or modify
     * the virtual LED data.
     */
    static uint8_t (*p_virtual_leds)[3];
    static uint8_t (*p_hsv_virtual_leds)[3];

};
