#include "button.h"


//needs error handling
void init_button(uint32_t gpio_pin, gpio_irq_cb_t irq_handler) {

    gpio_set_dir(gpio_pin, gpioPinDirInput);
    gpio_register_interrupt(gpio_pin, gpioIrqFallingEdge, irq_handler);
    gpio_enable_interrupt(gpio_pin);
}