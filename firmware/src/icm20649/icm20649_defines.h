/**
 * @file icm20649_registers.h
 * @brief Declarations of register addresses and settings for the ICM20649 motion sensor.
 *
 * This file contains definitions of register addresses and settings for the ICM20649 motion sensor.
 * Keep in mind that the device has multiple register banks, which need to be switched to in order
 * to execute reads and writes.
 * Also note that some of these values are register values and others are the
 * binary values written to the register for the desired settings for this build.
 * When writing binary in this file, the left most bit is the most significant.
 *
 * It provides convenient macros for accessing specific registers and configuring sensor settings.
 *
 *
 * The naming convention used in this file follows the format:
 *   - ICM_20649_BX_[REGISTER_NAME]: Registers that apply to all register banks.
 *   - ICM_20649_B[REGISTER_BANK]_[REGISTER_NAME]: Registers that are specific to a register bank.
 *
 * For example:
 *   - ICM_20649_B0_WHO_AM_I: Represents the WHO_AM_I register in register bank 0.
 *   - ICM_20649_BX_REG_BANK_SEL: Represents the REG_BANK_SEL register applicable to all register banks.
 *
 * Source of device information: https://invensense.tdk.com/download-pdf/icm-20649-data-sheet/
 */
#pragma once

#define ICM_20649_DEVICE_ADDRESS (0b1101001)

/********************************************//**
 * SELECT REGISTER BANKS - ADDRESS AND SETTINGS
 ***********************************************/
#define ICM_20649_BX_REG_BANK_SEL (0x7F)
#define ICM_20649_REG_BANK_SEL_SETTINGS(input) \
    ((input == 0) ? 0x00 : \
    ((input == 1) ? 0x10 : \
    ((input == 2) ? 0x20 : \
    ((input == 3) ? 0x30 : -1))))

/********************************************//**
 * ACCELEROMETER CONFIG AND DATA REGISTERS -
 * ADDRESSES, SETTINGS, AND IMPORTANT VALUES
 ***********************************************/
#define ICM_20649_B2_ACCEL_CONFIG (0X14)
#define ICM_20649_B2_ACCEL_CONFIG_SETTINGS (0X07)

#define ICM_20649_B0_ACCEL_XOUT_H (0x2D)
#define ICM_20649_B0_ACCEL_XOUT_L (0x2E)
#define ICM_20649_B0_ACCEL_YOUT_H (0x2F)
#define ICM_20649_B0_ACCEL_YOUT_L (0x30)
#define ICM_20649_B0_ACCEL_ZOUT_H (0x31)
#define ICM_20649_B0_ACCEL_ZOUT_L (0x32)

// values needed to calculate accel values from raw data
#define ACCEL_FS_8192_LSB_PER_G (8192)
#define ACCEL_FS_4096_LSB_PER_G (4096)
#define ACCEL_FS_2048_LSB_PER_G (2048)
#define ACCEL_FS_1024_LSB_PER_G (1024)

/********************************************//**
 * GYROSCOPE CONFIG AND DATA REGISTERS -
 * ADDRESSES, SETTINGS, AND KEY VALUES
 ***********************************************/
#define ICM_20649_B2_GYRO_CONFIG_1 (0x01)
#define ICM_20649_B2_GYRO_CONFIG_2 (0x02)
/* Setting this register 0010 1111 to:
 * - turns on the low pass filter
 * - sets dps to +-4000
 * - uses 20 mHz oscillator for the internal clock */
#define ICM_20649_B2_GYRO_CONFIG_1_SETTINGS (0x47)

#define ICM_20649_B0_GYRO_XOUT_H (0x33)
#define ICM_20649_B0_GYRO_XOUT_L (0x34)
#define ICM_20649_B0_GYRO_YOUT_H (0x35)
#define ICM_20649_B0_GYRO_YOUT_L (0x36)
#define ICM_20649_B0_GYRO_ZOUT_H (0x37)
#define ICM_20649_B0_GYRO_ZOUT_L (0x38)


#define ICM_20649_GYRO_FS_SEL_0 (65.5)
#define ICM_20649_GYRO_FS_SEL_1 (32.8)
#define ICM_20649_GYRO_FS_SEL_2 (16.4)
#define ICM_20649_GYRO_FS_SEL_3 (8.2)


/********************************************//**
 * OTHER SETTINGS - ADDRESSES
 ***********************************************/
#define ICM_20649_B0_WHO_AM_I (0x00)
#define ICM_20649_B0_USER_CTRL (0x03)
#define ICM_20649_B0_PWR_MGMT_1 (0x06)
#define ICM_20649_B0_LP_CONFIG (0x05)
#define ICM_20649_B0_DISABLE_SENSORS (0x05)
#define ICM_20649_USER_CTRL_SETTINGS (0x20)
#define ICM_20649_LP_CONFIG (0x05)


/********************************************//**
 * OTHER SETTINGS - SETTINGS
 ***********************************************/
/* Setting this register to 0000 0000:
 * - exits sleep mode and low power mode,
 * - keeps temperature sensor on
 * - uses 'best clock source' according to docs,
 *   for internal clock */
#define ICM_20649_B0_PWR_MGMT_1_SETTINGS (0x01)