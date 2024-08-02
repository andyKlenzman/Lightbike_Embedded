#Lightbike
Project Overview
Lightbike enhances bicycle visibility and aesthetics through motion-responsive LED patterns. The system employs motion-powered LEDs mounted on bike spokes, controlled by the EFR32MG24 MCU. Motion data is collected via the ICM20649 sensor, and LEDs are driven by WS2812s using SPI and level shifting.

#Features
Motion-Responsive LED Patterns: LED patterns change based on motion detected by the ICM20649 sensor.
Customizable LED Filters: Various filters can be applied to create different visual effects.
Power Management: Controlled by toggling the system power based on flags.

#Project Structure
Includes:

kernel.h: Kernel functionalities.
logging.h: Logging utilities.
defines.h: Definitions and constants.
globals.h: Global variables and configurations.
ws2812b/ws2812b.h: WS2812 LED driver.
icm20649/icm20649.h: ICM20649 sensor driver.
buttons/buttons.h: Button handling.
state_handler/state_handler.h: State management.
utils/power_toggle.c: Power toggle utilities.
utils/map_value.h: Value mapping utilities.
Global Data Buffers:

accel_data[3]: Raw accelerometer data.
gyro_data[3]: Raw gyroscope data.
mapped_accel_data[3]: Mapped accelerometer data for LED control.
mapped_gyro_data[3]: Mapped gyroscope data for LED control.
virtual_leds[NUM_PIXELS][3]: Buffer for LED colors.
Power Toggle Flag:

volatile bool flag_toggle_system_power: Flag for toggling system power.
Mapping Mode:

#define MAPPING_MODE MAP_MODE_UNSIGNED: Defines the mapping mode for sensor data.
Initialization and Main Loop
Initialization
LED Strip Initialization: Initializes the LED strip with led_strip_init(NUM_PIXELS).
ICM20649 Initialization: Initializes the ICM20649 sensor with icm_20649_init().
Button Initialization: Initializes buttons for mode and power with button_init().
Main Loop
Read Sensor Data: Retrieves accelerometer and gyroscope data from the ICM20649 sensor.
Map Sensor Data: Maps raw sensor data to LED control ranges using map_value().
Update LEDs: Calls the current LED filter, updates LED colors, and refreshes the LED strip.
Toggle System Power: Toggles power if the flag_toggle_system_power flag is set.
Conventions
Log Modules: Use all uppercase for module names.
Defines: Avoid abbreviations.
Errors: Negative numbers indicate errors; 0 indicates success.
Deficiencies
LED Filters Documentation: Detailed instructions for adding new LED filters are currently missing.
Gyroscope Data Integration: Integration of gyroscope data into LED patterns is incomplete.
License

Contact
Author: Andy Klenzman
Email: ak@imagineon.de