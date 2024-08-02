# Lightbike
Project Overview
Lightbike enhances bicycle visibility and aesthetics through motion-responsive LED patterns. The system employs motion-powered LEDs mounted on bike spokes, controlled by the EFR32MG24 MCU. Motion data is collected via the ICM20649 sensor, and LEDs are driven by WS2812s using SPI and level shifting.

## Features
Motion-Responsive LED Patterns: LED patterns change based on motion detected by the ICM20649 sensor.
Customizable LED Filters: Various filters can be applied to create different visual effects.
Power Management: Controlled by toggling the system power based on flags.


## Conventions
Log Modules: Use all uppercase for module names.

Defines: Avoid abbreviations.

Errors: Negative numbers indicate errors; 0 indicates success.

## Deficiencies
LED Filters Documentation: Detailed instructions for adding new LED filters are currently missing.



## Contact
Author: Andy Klenzman
Email: ak@imagineon.de