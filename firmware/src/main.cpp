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
 The lighting pattern modes are a list of function pointers incremented through by
 each button push. The power off mode simply selects an LEDFilter where all the lights are
 turned off.

### Program Flow
The program starts with the initialization of the hardware components: Ws2812b LEDs, ICM20649,
 and the two buttons. Implementation details for these components are contained within their respective files.

Following initialization, the main loop of the program executes, managing the LED
 filters and updating the LED patterns based on data collected from the accelerometer.

### Conventions
- **Log Modules:** Use all uppercase for module names. Example: `LOG_MODULE(ICM_20649)`.
- **Defines:** Avoid abbreviations.
- **Errors:** Negative numbers are errors, 0 is a successful outcome. Use ´result´
 as the variable name to indicate an error message is being returned.

### Deficiencies
- The current documentation lacks detailed instructions on
 how to add new LED filters and what modifications are needed in other files.
- the gyroscope ius not set up or integrated into the flow

*===========================================================================*/


#include <kernel.h>
#include "logging.h"

#include "defines.h"
#include "globals.h"

#include "ws2812b/ws2812b.h"
#include "icm20649/icm20649.h"
#include "buttons/buttons.h"

#include "state_handler/state_handler.h"
#include "utils/power_toggle.c"

LOG_MODULE(main)

/* virtual_leds are an array that are transformed by LED filter,
 * then transformed into a data buffer suitable to control the LEDs
 * through the SPI, and then pushed inside the while loop. */
uint8_t virtual_leds[NUM_PIXELS][3];
float accel_data[3];
uint8_t gyro_data[3];

/* the pointer inside the LEDFilter class, the base class
 * for LEDFilters which transform motion data into LED patterns,
 * is set to these variables, so the same data is available to
 * the main program loop, as it is being manipulated inside of the
 * LEDFilter. */
float *LEDFilter::p_accel_data = accel_data;
uint8_t *LEDFilter::p_gyro_data = gyro_data;

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


        result = icm_20649_read_gyro_data(gyro_data);
        LOG_DEBUG("GYRO data: X=%u, Y=%u, Z=%u\n", gyro_data[0], gyro_data[1], gyro_data[2]);
        if (result == -1) {
            LOG_ERROR("icm_20649_read_gyro_data failed.");
        }


        result = icm_20649_read_accel_data(accel_data);
        LOG_DEBUG("Accelerometer data: X=%u, Y=%u, Z=%u\n", accel_data[0], accel_data[1], accel_data[2]);
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

        /* What happens if I remove this delay? Why did I put this here in the first place? */
        osDelayUntil(ts+15);

    }
};