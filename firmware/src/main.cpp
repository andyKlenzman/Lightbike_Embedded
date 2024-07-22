/*=============================================================================
Project:  Lightbike
Author:   Andy Klenzman (ak@imagineon.de)
Description:

Lightbike is an embedded system that increases bicycle visibility and aesthetics
through motion-responsive LED patterns. The system utilizes a set of motion-powered
LEDs that are mounted around bike spokes. As the bike moves, the LEDs create
responsive light patterns based on the bike's movement.

The core of the Lightbike system is powered by the EFR32MG24 MCU.
Motion data is collected using the ICM20649 sensor, which integrates both an
 accelerometer and a gyroscope, and communicates via the I2C protocol.

The LEDs used are WS2812, which are controlled through precise level shifting
provided by the SN74LVC1T45DBV and an Serial Peripheral Interface (SPI). This
ensures accurate signal transmission for the LEDs' color and brightness control.

### LED Control and Filtering
LED control is managed by `LEDFilters`, a set of specialized classes designed
 to interact with the accelerometer data and, in future versions, the gyroscope
 data. This design choice ensures consistent formatting and extensibility,
 allowing for easy addition of new features and LED patterns.

To integrate a new LED filter, you will need to modify the `state_handlers`
 file. For detailed instructions on creating and adding new filters,
 please refer to the in-file documentation.

### User Interface
The system's power and lighting pattern modes are controlled via physical buttons.
 The implementation for the power button and state change button is still pending.

### Program Flow
The program starts with the initialization of the hardware components mentioned above.
 Implementation details for these components are contained within their respective files.

Following initialization, the main loop of the program executes, managing the LED
 filters and updating the LED patterns based on the collected data.

### Naming Conventions
- **Log Modules:** Use all uppercase for module names. Example: `LOG_MODULE(ICM_20649)`.
- **Defines:** Avoid abbreviations.

### Deficiencies
- The current documentation lacks detailed instructions on
 how to add new LED filters and what modifications are needed in other files.

*===========================================================================*/


#include <kernel.h>
#include "logging.h"

#include "leds/ws2812b.h"
#include "drivers/icm20649.h"

#include "defines.h"
#include "globals.h"

#include "state_handler/state_handler.h"
#include "buttons/buttons.h"
#include "utils/power_toggle.c"

LOG_MODULE(main)

/* virtual_leds are transformed inside the program, and
 * then pushed to the actual leds */
uint8_t virtual_leds[NUM_PIXELS][3];
uint8_t accel_data[3];

uint8_t *LEDFilter::p_accel_data = accel_data;
uint8_t (*LEDFilter::p_virtual_leds)[3] = virtual_leds;

volatile bool flag_toggle_system_power = false;

int main(void) {
    int result;

    result = led_strip_init(NUM_PIXELS);
    if (result == -1) {
        LOG_ERROR("LED init failed.");
    } else {LOG_DEBUG("LED init succeeded.");}

    result = icm_20649_init();
    if (result == -1) {
        LOG_ERROR("ICM init failed.");
    } else {LOG_DEBUG("ICM init succeeded.");}

    result = button_init(GPIO_PUSH_BTN_1, mode_button_irq_function);
    if (result == -1) {
        LOG_ERROR("Mode button init failed.");
    } else {LOG_DEBUG("Mode button init succeeded.");}

    result = button_init(GPIO_PUSH_BTN_2, power_button_irq_function);
    if (result == -1) {
        LOG_ERROR("Power button init failed.");
    } else {LOG_DEBUG("Power button init succeeded.");}


    while (1) {
        uint32_t ts = osKernelGetTickCount();

        result = icm_20649_read_accel_data(accel_data);
        // LOG_DEBUG("Accelerometer data: X=%u, Y=%u, Z=%u\n", accel_data[0], accel_data[1], accel_data[2]);
        if (result == -1) {
            LOG_ERROR("icm_20649_read_accel_data failed.");
        }

        call_current_led_filter();
        set_leds(virtual_leds, NUM_PIXELS);

        if(flag_toggle_system_power){
            toggle_power();
            flag_toggle_system_power = false;
        }
        update_leds();
        osDelayUntil(ts+30);
    }
};