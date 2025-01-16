#include "buttons.h"
#include "filter_handler/filter_handler.h"
#include "logging.h"
#include "gpio.h"
#include "globals.h"

LOG_MODULE(button_interrupt)


void mode_button_irq_function()
{

    increment_filter_selection();
    mode_button_click_counts ++;

}


void power_button_irq_function()
{

    power_button_click_counts++;

}

int button_init(uint32_t gpio_pin, gpio_irq_cb_t irq_handler) {
    int result;

    // Set GPIO pin direction to input
    result = gpio_set_dir(gpio_pin, gpioPinDirInput);
    if (result == -1) {
        return -1;
    }

    // Register the IRQ handler for rising edge
    result = gpio_register_interrupt(gpio_pin, gpioIrqRisingEdge, irq_handler);
    if (result == -1) {
        return -1;
    }

    // Enable the interrupt on the GPIO pin
    result = gpio_enable_interrupt(gpio_pin);
    if (result == -1) {
        return -1;
    }

    return 0;
}


void register_power_button_command(power_button_click_modes_e clicks)
{
    switch (clicks)
    {
        case change_data_source:
            break;
        case toggle_device_power:

            if(DEVICE_STATUS_GET(device_status_field, status_on))
            {
                device_status_field &= ~DEVICE_STATUS_MASK(status_on);
            } else
            {
                device_status_field |= DEVICE_STATUS_MASK(status_on);
                device_status_field &= ~DEVICE_STATUS_MASK(status_cleared);
            }



            break;
    }



}