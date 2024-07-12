#pragma once
#include "gpio.h"


int button_init(uint32_t gpio_pin, gpio_irq_cb_t irq_handler);
void mode_button_irq_function();
void power_button_irq_function();