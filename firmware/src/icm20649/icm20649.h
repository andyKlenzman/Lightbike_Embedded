#pragma once

#include "stdint.h"

/**
 * @brief Initializes the ICM-20649 sensor.
 * @return 0 on success, or a negative error code on failure.
 */
int icm_20649_init();

/**
 * @brief Initializes both the accelerometer and gyroscope of the ICM-20649 sensor.
 * @return 0 on success, or a negative error code on failure.
 */
int icm_20649_init_accel_and_gyro();

/**
 * @brief Reads a value from a specific register.
 * @param reg Register address to read from.
 * @return Register value or a negative error code on failure.
 */
uint8_t icm_20649_return_register_val(uint8_t reg);

/**
 * @brief Writes a value to a specific register.
 * @param reg Register address to write to.
 * @param data Data to write.
 * @return 0 on success, or a negative error code on failure.
 */
int icm_20649_write_reg(uint8_t reg, uint8_t data);

/**
 * @brief Reads acceleration data.
 * @param accel_data Array to store the data.
 * @return 0 on success, or a negative error code on failure.
 */
int icm_20649_read_accel_data(uint8_t accel_data[]);

/**
 * @brief Reads gyroscope data.
 * @param gyro_data Array to store the data.
 * @return 0 on success, or a negative error code on failure.
 */
int icm_20649_read_gyro_data(uint8_t gyro_data[]);
