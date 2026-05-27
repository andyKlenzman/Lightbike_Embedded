# Lightbike_Embedded

Embedded firmware for motion-responsive bike lights, built during my internship at [ImagineOn](https://www.imagineon.de/).

An ICM20649 IMU is mounted on each axle of the bike. The EFR32MG24 MCU reads acceleration and gyroscope data, applies filters, and drives WS2812b LEDs wound through the spokes over SPI. Two buttons let the rider power off or cycle through light modes.

This is the companion firmware to the [Bike-Light](https://github.com/andyklenzman/Bike-Light) React Native app.
