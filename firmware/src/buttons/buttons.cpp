#include "buttons.h"
#include "state_handlers/state_handler.h"
#include "logging.h"

LOG_MODULE(button_interrupt)


/**
 * @brief The mode button irq function simply rotates through the possible app states, which are different LEDFilters.
 */
void mode_button_irq_function() {
        current_state = static_cast<AppState>((current_state + 1) % MODE_MAX);
    }


/**
* @brief The power button irq function turns the device on and off.
 *
 * It has not yet been implemented, and the exact specification of what turning off will mean comes later.
*/
void power_button_irq_function() {
}


/**
 * @brief Light abstraction that uses Coldwave's gpio_pin interface to activate buttons with interrupt functions.
 *
 * The interrupt are rising edges, mean it will be activated when the button is pressed.
 * The errors abandon the function, and are handled without clean-up, but are simply there
 * to identify where the error is occuring. are returned to the to trigger rising edge (when the button is   a button to the hardware pin and an interrupt function.
 *
 * @param[in] gpio_pin is the pin idetnification number found inside of hardware's documentation
 * @param[in] irq_handler is a Coldwave alias indicating a Interrupt Request Handler function pointer that takes no parameters.
 *
 * @return 0 if processing was successful, -1 otherwise.
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