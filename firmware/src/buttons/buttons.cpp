#include "buttons.h"
#include "state_handler/state_handler.h"
#include "logging.h"
#include "gpio.h"
#include "globals.h"

LOG_MODULE(button_interrupt)

/**
 * @brief Mode button IRQ function.
 *
 * This function is triggered by an interrupt from the mode button.
 * It rotates through different application states, changing the LED filter mode.
 */
void mode_button_irq_function() {
    increment_state();  // Move to the next state
}

/**
 * @brief Power button IRQ function.
 *
 * This function is triggered by an interrupt from the power button.
 * It is intended to turn the device on and off.
 * Note: The implementation details for turning off are pending.
 */
void power_button_irq_function() {
    LOG_DEBUG("POWER BUTTON FIRE");
    flag_toggle_system_power = true;  // Flag variable to signal events
}

/**
 * @brief Initializes a button using GPIO pin and IRQ handler.
 *
 * Sets up a button with interrupt functionality on a specified GPIO pin.
 * This function configures the GPIO pin as an input, registers an interrupt
 * handler for rising edge detection, and enables the interrupt.
 * Errors are reported but not handled further, intended for debugging purposes.
 *
 * @param[in] gpio_pin GPIO pin number as per hardware documentation.
 * @param[in] irq_handler IRQ handler function pointer for button interrupt.
 * @return 0 if successful, -1 on failure.
 */
int button_init(uint32_t gpio_pin, gpio_irq_cb_t irq_handler) {
    int result;

    // Set GPIO pin direction to input
    result = gpio_set_dir(gpio_pin, gpioPinDirInput);
    if (result == -1) {
        return -1;  // Return error if setting direction fails
    }

    // Register the IRQ handler for rising edge
    result = gpio_register_interrupt(gpio_pin, gpioIrqRisingEdge, irq_handler);
    if (result == -1) {
        return -1;  // Return error if registering interrupt fails
    }

    // Enable the interrupt on the GPIO pin
    result = gpio_enable_interrupt(gpio_pin);
    if (result == -1) {
        return -1;  // Return error if enabling interrupt fails
    }

    return 0;  // Return success if all operations succeed
}