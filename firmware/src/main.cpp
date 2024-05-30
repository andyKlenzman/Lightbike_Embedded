#include <kernel.h>
#include <gpio.h>
#include "gpio.h"


#include "drivers/ws2812b.h"
#include "drivers/icm20649.h"
#include "logging.h"
#include "led_filters/LEDFilter_Basic.cpp"
#include "led_filters/LEDFilter_Smooth.cpp"
#include "defines.h"

LOG_MODULE(main)



/********************************************//**
 Here we set p_accel_data and p_virtual_leds,
 static members of the LED filter class, with
 the memory location of accel_data and virtual_leds.

 This allows the class to access the two arrays that
 are used throughout the firmware, without using extra
 memory.
 ***********************************************/
uint8_t virtual_leds[NUM_PIXELS][3];
uint8_t accel_data[3];

uint8_t *LEDFilter::p_accel_data = accel_data;
uint8_t (*LEDFilter::p_virtual_leds)[3] = virtual_leds;

bool button_state;



int gpio_irq(){
    LOG_DEBUG("INTERRUPT FIRED");
}

int main(void) {
    gpio_set_dir(GPIO_PUSH_BTN_1, gpioPinDirInput);
    gpio_set_dir(GPIO_PUSH_BTN_2, gpioPinDirInput);


    // Register an interrupt for the GPIO pin with falling edge.
    gpio_register_interrupt(GPIO_PUSH_BTN_1, gpioIrqFallingEdge, reinterpret_cast<gpio_irq_cb_t>(gpio_irq));

    // Enable the interrupt
    gpio_enable_interrupt(GPIO_PUSH_BTN_1);




    int result = led_strip_init(NUM_PIXELS);
    if (result == -1) {
        LOG_ERROR("LED init failed.");
    } else {LOG_DEBUG("LED init succeeded.");}


    result = icm_20649_init();
    if (result == -1) {
        LOG_ERROR("ICM init failed.");
    } else {LOG_DEBUG("ICM init succeeded.");}

    /* could be that everytime one is init, they are added to a list. */
    LEDFilter_Smooth led_filter_smooth;
    LEDFilter_Basic led_filter_basic;


    while (1) {
        uint32_t ts = osKernelGetTickCount();

        button_state = gpio_get(GPIO_PUSH_BTN_1);
        int result = icm_20649_read_accel_data(accel_data); // what to do with the error, I just want to tell the developer
        if (result == -1) {
            LOG_ERROR("icm_20649_read_accel_data failed.");
        }

        if(button_state){
            led_filter_smooth.apply_filter();

        }else{
            led_filter_basic.apply_filter();

        }


        for (int i = 0; i < NUM_PIXELS; i++) {
            led_strip_set_led(i, virtual_leds[i][0], virtual_leds[i][1], virtual_leds[i][2]);
        }

        led_strip_update();
        osDelayUntil(ts+20);
    }
};