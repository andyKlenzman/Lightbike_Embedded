/**
 * @file icm20649_registers.h
 * @brief Declarations of register addresses and settings for the ICM20649 motion sensor.
 *
 * This file contains definitions of register addresses and settings for the ICM20649 motion sensor.
 * It provides convenient macros for accessing specific registers and configuring sensor settings.
 *
 * The naming convention used in this file follows the format:
 *   - ICM_20649_BX_[REGISTER_NAME]: Registers that apply to all register banks.
 *   - ICM_20649_B[REGISTER_BANK]_[REGISTER_NAME]: Registers that are specific to a register bank.
 *
 * For example:
 *   - ICM_20649_B0_WHO_AM_I: Represents the WHO_AM_I register in register bank 0.
 *   - ICM_20649_BX_REG_BANK_SEL: Represents the REG_BANK_SEL register applicable to all register banks.
 *
 */
#pragma once

#define ICM_20649_DEVICE_ADDRESS (0b1101000)


/********************************************//**
 * LOCATION
 ***********************************************/
#define ICM_20649_B0_WHO_AM_I (0x00)
#define ICM_20649_B0_USER_CTRL (0x03)
#define ICM_20649_B0_PWR_MGMT_1 (0x06)
#define ICM_20649_B0_LP_CONFIG (0x05)
#define ICM_20649_B0_DISABLE_SENSORS (0x05)

// accelerometer registers
#define ICM_20649_B0_ACCEL_XOUT_H (0x2D)
#define ICM_20649_B0_ACCEL_XOUT_L (0x2E)
#define ICM_20649_B0_ACCEL_YOUT_H (0x2F)
#define ICM_20649_B0_ACCEL_YOUT_L (0x30)
#define ICM_20649_B0_ACCEL_ZOUT_H (0x31)
#define ICM_20649_B0_ACCEL_ZOUT_L (0x32)


// gyroscope registers
#define ICM_20649_B0_GYRO_XOUTL (0x34)

#define ICM_20649_BX_REG_BANK_SEL (0x7F)
#define ICM_20649_B2_ACCEL_CONFIG (0X14)
/********************************************//**
 * SETTINGS
 ***********************************************/
#define ICM_20649_USER_CTRL_SETTINGS (0x20) // 1 enables I2C
#define ICM_20649_LP_CONFIG (0x05)
#define ICM_20649_B0_PWR_MGMT_1_SETTINGS (0x00)
#define ICM_20649_B2_ACCEL_CONFIG_SETTINGS (0X07)
#define ICM_20649_REG_BANK_SEL_SETTINGS(input) \
    ((input == 0) ? 0x00 : \
    ((input == 1) ? 0x10 : \
    ((input == 2) ? 0x20 : \
    ((input == 3) ? 0x30 : -1))))


/********************************************//**
 * OTHER VARIABLES
 ***********************************************/
#define ACCEL_FS_8192_LSB_PER_G (8192)
#define ACCEL_FS_4096_LSB_PER_G (4096)
#define ACCEL_FS_2048_LSB_PER_G (2048)
#define ACCEL_FS_1024_LSB_PER_G (1024)
