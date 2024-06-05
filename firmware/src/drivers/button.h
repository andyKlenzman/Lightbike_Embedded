#pragma once
#include "gpio.h"

void init_button(uint32_t gpio_pin, gpio_irq_cb_t irq_handler);