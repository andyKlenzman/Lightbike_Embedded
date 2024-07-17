#include "buttons.h"
#include "state_handler/state_handler.h"
#include "logging.h"
#include "gpio.h"


LOG_MODULE(button_interrupt)


/**
 * @brief Mode button IRQ function.
 *
 * Rotates through different application states (LEDFilters).
 */
void mode_button_irq_function() {
        current_state = static_cast<AppState>((current_state + 1) % MODE_MAX);
    }


/**
 * @brief Power button IRQ function.
 *
 * Turns the device on and off.
 * Note: Implementation details for turning off are pending.
 */
void power_button_irq_function() {
}

/**
 * @brief Initializes a button using GPIO pin and IRQ handler.
 *
 * This function sets up a button with interrupt functionality on a specified GPIO pin.
 * Errors are reported but not handled further, intended for debugging purposes.
 *
 * @param[in] gpio_pin GPIO pin number as per hardware documentation.
 * @param[in] irq_handler IRQ handler function pointer for button interrupt.
 * @return 0 if successful, -1 on failure.
 */
int button_init(uint32_t gpio_pin, gpio_irq_cb_t irq_handler) {
    int result;
    result = gpio_set_dir(gpio_pin, gpioPinDirInput);
    if (result == -1 ){
        return -1;
    }

    result = gpio_register_interrupt(gpio_pin, gpioIrqBothEdges, irq_handler);
    if (result == -1 ){
        return -1;
    }

    result = gpio_enable_interrupt(gpio_pin);
    if (result == -1 ){
        return -1;
    }

    return 0;
}