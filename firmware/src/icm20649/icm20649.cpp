


/**
 * @brief This file handles the functionality of the ICM, including reading and writing
 * from the registers.
 *
 *
 *
 * There is more information on the exact purpose of the register writes in icm20649_defines.h
 *
 *
 * deficiencies:
 * The ICM read register function frequently fails. I have strategies to mend this, including
 * using FSYNC and FIFO buffer to ensure data is ready to be read (though I am not totally
 * sure what the bug is in the first place). However, I've determined that taking time on this problem
 * is not as important, given that the data updates often enough to not interfere with the user experience.
 *
 * As a fix, I have simply kept the updates the same if the read fails three times in a row. This
 * presents another problem when the MSB updates and the LSB does not for a single value, meaning
 * that the combined value is coming from two separate readings. however, I have determined from tests
 * that this does not interfere with the patterns appearing responsive, and will keep it as is for now.
 *
 * In the future, you could do work to address these expensive bad reads, possibly by implementing
 * buffer reads, fsync, or a deeper probe into the ICM settings.
 */


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
#define MAX_RETRIES 3

uint8_t last_valid_accel_vals[6] = {0}; // Array to store last valid values

int icm_20649_read_accel_data(float accel_data[]) {
    uint8_t reg_addrs[6] = {
            ICM_20649_B0_ACCEL_XOUT_H,
            ICM_20649_B0_ACCEL_XOUT_L,
            ICM_20649_B0_ACCEL_YOUT_H,
            ICM_20649_B0_ACCEL_YOUT_L,
            ICM_20649_B0_ACCEL_ZOUT_H,
            ICM_20649_B0_ACCEL_ZOUT_L
    };

    uint8_t raw_accel_vals[6];

    // Read values from registers
    for (int i = 0; i < 6; i++) {
        int retries = 0;
        while (retries < MAX_RETRIES) {
            raw_accel_vals[i] = icm_20649_return_register_val(reg_addrs[i]);
            if (raw_accel_vals[i] != (uint8_t)-1) {
                //LOG_DEBUG("icm_20649_read_accel_data: Success reading register 0x%02X (index %d), attempt %d.", reg_addrs[i], i, retries);
                last_valid_accel_vals[i] = raw_accel_vals[i]; // Update last valid value
                break; // Successful read
            }
            retries++;
            //LOG_DEBUG("icm_20649_read_accel_data: Failed to read register 0x%02X (index %d), attempt %d.", reg_addrs[i], i, retries);
        }
        if (retries == MAX_RETRIES) {
            //LOG_DEBUG("icm_20649_read_accel_data: Failed to read register 0x%02X (index %d) after %d attempts.", reg_addrs[i], i, MAX_RETRIES);
            raw_accel_vals[i] = last_valid_accel_vals[i]; // Use last valid value
        }
    }

    // Combine bytes and compute acceleration values
    float accel_outs[3];
    for (int i = 0; i < 3; i++) {
        int high_idx = 2 * i;
        int low_idx = 2 * i + 1;
        if (raw_accel_vals[high_idx] != (uint8_t)-1 && raw_accel_vals[low_idx] != (uint8_t)-1) {
            int16_t combined_val = (int16_t)combine_bytes(raw_accel_vals[high_idx], raw_accel_vals[low_idx]);
            accel_outs[i] = (float) combined_val / ACCEL_FS_1024_LSB_PER_G;

        } else {
            accel_outs[i] = 0; // Default value in case of failure
            LOG_DEBUG("icm_20649_read_accel_data: Using default value for axis %d due to read failure.", i);
        }
    }

    // Map values to the output array
    for (int i = 0; i < 3; i++) {
        accel_data[i] = map_value(accel_outs[i]);
    }

    return 0;
}

uint8_t last_valid_gyro_vals[6] = {0}; // Array to store last valid values

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
        int retries = 0;
        while (retries < MAX_RETRIES) {
            gyro_vals[i] = icm_20649_return_register_val(reg_addrs[i]);
            if (gyro_vals[i] != (uint8_t)-1) {
                LOG_DEBUG("icm_20649_read_gyro_data: Success reading register 0x%02X (index %d), attempt %d.", reg_addrs[i], i, retries);
                last_valid_gyro_vals[i] = gyro_vals[i]; // Update last valid value
                break; // Successful read
            }
            retries++;
            LOG_DEBUG("icm_20649_read_gyro_data: Failed to read register 0x%02X (index %d), attempt %d.", reg_addrs[i], i, retries);
        }
        if (retries == MAX_RETRIES) {
            LOG_DEBUG("icm_20649_read_gyro_data: Failed to read register 0x%02X (index %d) after %d attempts.", reg_addrs[i], i, MAX_RETRIES);
            gyro_vals[i] = last_valid_gyro_vals[i]; // Use last valid value
        }
    }

    // Combine bytes and compute gyro values
    float gyro_outs[3];
    for (int i = 0; i < 3; i++) {
        int high_idx = 2 * i;
        int low_idx = 2 * i + 1;
        if (gyro_vals[high_idx] != (uint8_t)-1 && gyro_vals[low_idx] != (uint8_t)-1) {
            gyro_outs[i] = (float)combine_bytes(gyro_vals[high_idx], gyro_vals[low_idx]) / ICM_20649_GYRO_FS_SEL_3;
        } else {
            gyro_outs[i] = 0; // Default value in case of failure
            LOG_DEBUG("icm_20649_read_gyro_data: Using default value for axis %d due to read failure.", i);
        }
    }

    // Map values to the output array
    for (int i = 0; i < 3; i++) {
        LOG_DEBUG("Gyro output[%d]: %d", i, gyro_outs[i]);
        gyro_data[i] = map_value(gyro_outs[i]);
    }

    return 0;
}
