#include "driver.h"
#include "i2c.h"
#include "icm20649_defines.h"
#include "logging.h"
#include "icm20649.h"
#include <kernel.h>
#include "utils/combine_bytes.h"
#include "utils/map_value.h"

LOG_MODULE(ICM_20649)

#define READ_BUFFER_SIZE_BYTES  (1)
#define WRITE_BUFFER_SIZE_BYTES (1)

static int i2c_device;
static uint8_t *p_read_buffer;
static uint8_t *p_write_buffer;


uint32_t ts = osKernelGetTickCount();


/**
 * @brief Initializes data buffers for communication with the ICM20649 sensor.
 *
 * This function allocates memory for the read and write buffers, opens the I2C device,
 * takes the sensor out of sleep mode, and initializes the accelerometer configuration.
 * It also restores the user bank to 0, which contains registers for reading sensor data.
 *
 * There is more information on the exact purpose of the register writes in icm20649_defines.h
 *
 *
 * @return 0 on success, -1 on failure.
 */
int icm_20649_init() {
    p_read_buffer = (uint8_t *)malloc(READ_BUFFER_SIZE_BYTES);
    p_write_buffer = (uint8_t *)malloc(WRITE_BUFFER_SIZE_BYTES);

    if (p_read_buffer == NULL || p_write_buffer == NULL) {
        LOG_ERROR("Failed to allocate memory for read and write buffer.");
        return -1;
    }

    memset(p_read_buffer, 0, READ_BUFFER_SIZE_BYTES);
    memset(p_write_buffer, 0, WRITE_BUFFER_SIZE_BYTES);

    i2c_device = open("i2c1");
    if (i2c_device == -1) {
        LOG_ERROR("I2C driver failed to open.");
        return -1;
    }

    int result = icm_20649_write_reg(ICM_20649_B0_PWR_MGMT_1, ICM_20649_B0_PWR_MGMT_1_SETTINGS);
    if (result == -1) {
        LOG_ERROR("Failed to write initialization settings to ICM_20649.");
        return -1;
    }

    result = icm_20649_init_accel_and_gyro();
    if (result == -1) {
        LOG_ERROR("Failed to initialize the ICM_20649 accelerometer and gyroscope.");
        return -1;
    }

    /* The program returns to register bank one, because all the data registers
     * for reading the accelerometer and gyroscope data are located there. */
    result = icm_20649_write_reg(ICM_20649_BX_REG_BANK_SEL, ICM_20649_REG_BANK_SEL_SETTINGS(0));
    if (result == -1) {
        LOG_ERROR("Failed to select register bank 0.");
        return -1;
    }

    return 0;
}

/**
 * @brief Initializes the accelerometer of the ICM20649 sensor.
 *
 * This function selects register bank 2 and sets the accelerometer configuration.
 *
 * @return 0 on success, -1 on failure.
 */
int icm_20649_init_accel_and_gyro() {
    int result = icm_20649_write_reg(ICM_20649_BX_REG_BANK_SEL, ICM_20649_REG_BANK_SEL_SETTINGS(2)); // config to bank 2
    if (result == -1) {
        LOG_ERROR("Failed to select register bank 2 in icm config function.");
    }

    result = icm_20649_write_reg(ICM_20649_B2_ACCEL_CONFIG, ICM_20649_B2_ACCEL_CONFIG_SETTINGS);
    if (result == -1) {
        LOG_ERROR("Failed to set accelerometer settings.");
    }

    result = icm_20649_write_reg(ICM_20649_B2_GYRO_CONFIG_1, ICM_20649_B2_GYRO_CONFIG_1_SETTINGS);
    if (result == -1) {
        LOG_ERROR("Failed to set gyroscope settings.");
    }

    return 0;
}


/**
 * @brief Reads data from a register of the ICM20649 sensor.
 *
 * @param reg The register address from which to read data.
 * @return The value read from the register, or -1 on failure.
 */
uint8_t icm_20649_return_register_val(uint8_t reg) {

    // delay prevents bad reads...maybe
    osDelayUntil(ts + 15);


    int result = i2c_read_reg(i2c_device, ICM_20649_DEVICE_ADDRESS, reg, p_read_buffer, READ_BUFFER_SIZE_BYTES);
    if (result == -1) {
        LOG_ERROR("icm_20649_read_reg(): Failed to read data from register 0x%02X.", reg);
        return -1;
    }

    return *p_read_buffer;
}

/**
 * @brief Writes data to a register of the ICM20649 sensor.
 *
 * @param reg The register address to which the data will be written.
 * @param data The data to be written to the register.
 * @return 0 on success, -1 on failure.
 */
int icm_20649_write_reg(uint8_t reg, uint8_t data) {
    memset(p_write_buffer, data, WRITE_BUFFER_SIZE_BYTES);

    int result = i2c_write_reg(i2c_device, ICM_20649_DEVICE_ADDRESS, reg, p_write_buffer, WRITE_BUFFER_SIZE_BYTES);
    if (result < 0) {
        LOG_ERROR("icm_20649_write_reg(): Failed to write 0x%02X to register 0x%02X.", data, reg);
        return -1;
    }

    // Delay to prevent outdated register reads
    osDelayUntil(ts + 15);
    LOG_DEBUG("icm_20649_write_reg(): Successfully wrote 0x%02X to register 0x%02X.", data, reg);
    return 0;
}

/**
 * @brief Reads accelerometer data from the ICM20649 sensor.
 *
 * @param accel_data Array to store the accelerometer data.
 * @return 0 on success, -1 on failure.
 */
int icm_20649_read_accel_data_old(uint8_t accel_data[]) {
    uint8_t reg_addrs[6] = {
            ICM_20649_B0_ACCEL_XOUT_H,
            ICM_20649_B0_ACCEL_XOUT_L,
            ICM_20649_B0_ACCEL_YOUT_H,
            ICM_20649_B0_ACCEL_YOUT_L,
            ICM_20649_B0_ACCEL_ZOUT_H,
            ICM_20649_B0_ACCEL_ZOUT_L
    };

    uint8_t accel_vals[6];
    // Read values from registers
    for (int i = 0; i < 6; i++) {
        accel_vals[i] = icm_20649_return_register_val(reg_addrs[i]);
        if (accel_vals[i] == (uint8_t)-1) {
            LOG_DEBUG("icm_20649_read_accel_data: Failed to read register 0x%02X (index %d).", reg_addrs[i], i);
            return -1;
        }
    }

    // Combine bytes and compute acceleration values
    float accel_outs[3];
    for (int i = 0; i < 3; i++) {
        int high_idx = 2 * i;
        int low_idx = 2 * i + 1;
        accel_outs[i] = (float)combine_bytes(accel_vals[high_idx], accel_vals[low_idx]) / ACCEL_FS_8192_LSB_PER_G;
    }

    // Map values to the output array
    for (int i = 0; i < 3; i++) {
        accel_data[i] = map_value(accel_outs[i]);
    }

    return 0;
}

int icm_20649_read_gyro_data(uint8_t gyro_data[]) {
    uint8_t reg_addrs[6] = {
            ICM_20649_B0_GYRO_XOUT_H,
            ICM_20649_B0_GYRO_XOUT_L,
            ICM_20649_B0_GYRO_YOUT_H,
            ICM_20649_B0_GYRO_YOUT_L,
            ICM_20649_B0_GYRO_ZOUT_H,
            ICM_20649_B0_GYRO_ZOUT_L
    };

    uint8_t gyro_vals[6];
    // Read values from registers
    for (int i = 0; i < 6; i++) {
        gyro_vals[i] = icm_20649_return_register_val(reg_addrs[i]);
        if (gyro_vals[i] == (uint8_t)-1) {
            LOG_DEBUG("icm_20649_read_gyro_data: Failed to read register 0x%02X (index %d).", reg_addrs[i], i);
            return -1;
        }
    }

    // Combine bytes and compute gyro values
    float gyro_outs[3];
    for (int i = 0; i < 3; i++) {
        int high_idx = 2 * i;
        int low_idx = 2 * i + 1;
        gyro_outs[i] = (float)combine_bytes(gyro_vals[high_idx], gyro_vals[low_idx]) / ICM_20649_GYRO_FS_SEL_3 ;
    }

    // Map values to the output array
    for (int i = 0; i < 3; i++) {
        gyro_data[i] = map_value(gyro_outs[i]);
    }

    return 0;
}



int icm_20649_read_accel_data(uint8_t accel_data[]) {
    uint8_t accel_xout_h = icm_20649_return_register_val(ICM_20649_B0_ACCEL_XOUT_H);
    uint8_t accel_xout_l = icm_20649_return_register_val(ICM_20649_B0_ACCEL_XOUT_L);
    uint8_t accel_yout_h = icm_20649_return_register_val(ICM_20649_B0_ACCEL_YOUT_H);
    uint8_t accel_yout_l = icm_20649_return_register_val(ICM_20649_B0_ACCEL_YOUT_L);
    uint8_t accel_zout_h = icm_20649_return_register_val(ICM_20649_B0_ACCEL_ZOUT_H);
    uint8_t accel_zout_l = icm_20649_return_register_val(ICM_20649_B0_ACCEL_ZOUT_L);

    if (accel_xout_l == (uint8_t)-1 || accel_xout_h == (uint8_t)-1 ||
        accel_yout_l == (uint8_t)-1 || accel_yout_h == (uint8_t)-1 ||
        accel_zout_l == (uint8_t)-1 || accel_zout_h == (uint8_t)-1) {
        LOG_DEBUG("icm_20649_read_accel_data: One or more failed register reads.");
        return -1;
    }

    float accel_xout = (float)combine_bytes(accel_xout_h, accel_xout_l) / ACCEL_FS_8192_LSB_PER_G;
    float accel_yout = (float)combine_bytes(accel_yout_h, accel_yout_l) / ACCEL_FS_8192_LSB_PER_G;
    float accel_zout = (float)combine_bytes(accel_zout_h, accel_zout_l) / ACCEL_FS_8192_LSB_PER_G;

    accel_data[0] = map_value(accel_xout);
    accel_data[1] = map_value(accel_yout);
    accel_data[2] = map_value(accel_zout);

    return 0;
}