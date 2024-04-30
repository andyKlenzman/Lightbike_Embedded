#include "driver.h"
#include "i2c.h"
#include "icm20649_defs.h"
#include "logging.h"
#include "icm20649.h"
#include <kernel.h>
#include "utils/combine_bytes.c"
#include "utils/map_value.c"


LOG_MODULE(ICM_20649)

#define READ_BUFFER_SIZE_BYTES  (1)
#define WRITE_BUFFER_SIZE_BYTES  (1)

static int i2c_device;
uint8_t *p_read_buffer;
uint8_t *p_icm_write_buffer;

/********************************************//**
 * @brief Initializes data buffers for communication with the ICM.
 *
 *  This function allocates memory for the read and write
 *  buffers used in communication with the ICM20649 sensor.
 *
 *  It opens the sensor and takes it out of sleep mode.
 *  It calls the accelerometer config function.
 *  It restores the user bank to 0, which contains registers for reading sensor data.
 *
 * @note The read buffer is used to store data received from the ICM.
 *       The write buffer is used to store data that will be transmitted to the ICM.
 ***********************************************/
int icm_20649_init ()
{
    p_read_buffer = (uint8_t *) malloc (READ_BUFFER_SIZE_BYTES);
    p_icm_write_buffer = (uint8_t *) malloc (WRITE_BUFFER_SIZE_BYTES);

    if (p_read_buffer == NULL || p_icm_write_buffer == NULL) {
        LOG_ERROR("Failed to allocate memory for read and write buffer.");
        return -1;
    }

    memset (p_read_buffer , 0, READ_BUFFER_SIZE_BYTES);
    memset (p_icm_write_buffer , 0, WRITE_BUFFER_SIZE_BYTES);

    i2c_device = open ("i2c1");
    if (i2c_device == -1) {
        LOG_ERROR("I2C driver failed to open.");
        return -1;
    }

    int result = icm_20649_write_reg (ICM_20649_B0_PWR_MGMT_1, ICM_20649_B0_PWR_MGMT_1_SETTINGS);
    if (result == -1){
        LOG_ERROR("Failed to write initialization settings to ICM_20649.");
        return -1;
    }

    result = icm_20649_init_accel ();
    if(result == -1) {
        LOG_ERROR("Failed to initialize the ICM_20649 Accelerometer.");
        return -1;

    }

    result = icm_20649_write_reg (ICM_20649_BX_REG_BANK_SEL, ICM_20649_REG_BANK_SEL_SETTINGS(0)); //config to bank 2
    if(result == -1) {
        LOG_ERROR("Failed to select register bank 0.");
        return -1;
    }



    return 0;
}


int icm_20649_init_accel ()
{
    int result;
    result = icm_20649_write_reg (ICM_20649_BX_REG_BANK_SEL, ICM_20649_REG_BANK_SEL_SETTINGS(2)); //config to bank 2
    if(result == -1) {
        LOG_ERROR("Failed to select register bank 2 in init_accel.");
        return -1;
    }

    result = icm_20649_write_reg (ICM_20649_B2_ACCEL_CONFIG, ICM_20649_B2_ACCEL_CONFIG_SETTINGS);
    if(result == -1) {
        LOG_ERROR("Failed to set accelerometer setting in init_accel.");
        return -1;
    }

    return 0;
}


/********************************************//**
 * @brief Reads data from a register of the ICM20649 sensor.
 *
 * The data read from the register is stored
 * in the read buffer for further processing.
 *
 * @param reg The register address from which to read data.
 *
 * @note The read buffer (`p_icm_read_buffer`) is used to store the data read
 *       from the register.
 ***********************************************/


uint8_t icm_20649_return_register_val (uint8_t reg) {
    int result = i2c_read_reg(i2c_device,
                                 ICM_20649_DEVICE_ADDRESS,
                                 reg,
                                 p_read_buffer,
                                 READ_BUFFER_SIZE_BYTES);


    if(result == -1){
        LOG_ERROR("icm_20649_read_reg(): Failed to read data from register 0x%02X.", reg);
        return -1;
    }

    return *p_read_buffer;
}


/********************************************//**
 * @brief Retrieves the data stored in the read buffer of the ICM20649 sensor.
 *
 * This function returns the data stored in the read buffer of the ICM20649 sensor.
 *
 * @return The data stored in the read buffer as an unsigned 8-bit integer.
 ***********************************************/
uint8_t icm_20649_get_read_buffer()
{
        return *p_read_buffer;
};



/********************************************//**
 * @brief Writes data to a register of the ICM20649 sensor.
 *
 * This function writes the specified data to the specified register of the ICM20649 sensor
 *
 * @param reg The register address to which the data will be written.
 * @param data The data to be written to the register.
 *
 * @note The write buffer (`p_icm_write_buffer`) is used to store the data to be written
 *       to the register.
 ***********************************************/
int icm_20649_write_reg (uint8_t reg, uint8_t data)
{
    uint32_t ts = osKernelGetTickCount();
    memset (p_icm_write_buffer , data, WRITE_BUFFER_SIZE_BYTES);

    int result = i2c_write_reg (i2c_device,
                                ICM_20649_DEVICE_ADDRESS,
                                reg,
                                p_icm_write_buffer,
                                WRITE_BUFFER_SIZE_BYTES);

    if(result == -1){
        LOG_ERROR("icm_20649_write_reg(): Failed to write 0x%02X to register 0x%02X.", data, reg);
        return -1;
    }

        // I cannot immediately read register after writing.
        // The delay prevents the possibility of am outdated register reads.
        osDelayUntil(ts+15);
        LOG_DEBUG("icm_20649_write_reg(): Successfully wrote 0x%02X to register 0x%02X.", data, reg);
        return 0;


}

int icm_20649_read_accel_data (uint8_t accel_data[])
{
    uint8_t accel_xout_h = icm_20649_return_register_val(ICM_20649_B0_ACCEL_XOUT_H);
    uint8_t accel_xout_l = icm_20649_return_register_val(ICM_20649_B0_ACCEL_XOUT_L);

    uint8_t accel_yout_h = icm_20649_return_register_val(ICM_20649_B0_ACCEL_YOUT_H);
    uint8_t accel_yout_l = icm_20649_return_register_val(ICM_20649_B0_ACCEL_YOUT_L);

    uint8_t accel_zout_h = icm_20649_return_register_val(ICM_20649_B0_ACCEL_ZOUT_H);
    uint8_t accel_zout_l = icm_20649_return_register_val(ICM_20649_B0_ACCEL_ZOUT_L);


    float accel_xout = (float)combine_bytes(accel_xout_h,accel_xout_l)/ACCEL_FS_8192_LSB_PER_G;
    float accel_yout = (float)combine_bytes(accel_yout_h,accel_yout_l)/ACCEL_FS_8192_LSB_PER_G;
    float accel_zout = (float)combine_bytes(accel_zout_h,accel_zout_l)/ACCEL_FS_8192_LSB_PER_G;


    accel_data[0] = map_value(accel_xout);
    accel_data[1] = map_value(accel_yout);
    accel_data[2] = map_value(accel_zout);

    if(accel_xout_l == -1 || accel_xout_h == -1 || accel_yout_l == -1 || accel_yout_h == -1 || accel_zout_l == -1 || accel_zout_h == -1){
        LOG_DEBUG("icm_20649_read_accel_data: one or more failed register read.");
        return -1;
    }

    return 0;
}