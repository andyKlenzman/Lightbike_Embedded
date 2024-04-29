#include "driver.h"
#include "i2c.h"
#include "icm20649_register_defs.h"
#include "logging.h"
#include "icm20689.h"
#include <kernel.h>
#include "utils/combine_bytes.c"
#include "utils/map_value.c"

#define ICM_READ_BUFFER_SIZE_BYTES  (1)
#define ICM_WRITE_BUFFER_SIZE_BYTES  (1)

LOG_MODULE(ICM_20649)

static int i2c_device;
uint8_t *p_icm_read_buffer;
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
void icm_20649_init ()
{
    p_icm_read_buffer = (uint8_t *) malloc (ICM_READ_BUFFER_SIZE_BYTES);
    p_icm_write_buffer = (uint8_t *) malloc (ICM_WRITE_BUFFER_SIZE_BYTES);
    memset (p_icm_read_buffer , 0, ICM_READ_BUFFER_SIZE_BYTES);
    memset (p_icm_write_buffer , 0, ICM_WRITE_BUFFER_SIZE_BYTES);

    i2c_device = open ("i2c1");
    // sets the device to i2c mode
    icm_20649_write_reg (ICM_20649_B0_PWR_MGMT_1, ICM_20649_B0_PWR_MGMT_1_SETTINGS);


    icm_20649_init_accel ();
    icm_20649_write_reg (ICM_20649_BX_REG_BANK_SEL, ICM_20649_REG_BANK_SEL_SETTINGS(0)); //config to bank 2

}


void icm_20649_init_accel ()
{
    icm_20649_write_reg (ICM_20649_BX_REG_BANK_SEL, ICM_20649_REG_BANK_SEL_SETTINGS(2)); //config to bank 2
    icm_20649_write_reg (ICM_20649_B2_ACCEL_CONFIG, ICM_20649_B2_ACCEL_CONFIG_SETTINGS);

    uint8_t read_buffer_value = icm_20649_read_reg_and_return_val (ICM_20649_BX_REG_BANK_SEL);
    LOG_DEBUG("init accel bank select = %u", read_buffer_value);

    read_buffer_value = icm_20649_read_reg_and_return_val (ICM_20649_B2_ACCEL_CONFIG);
    LOG_DEBUG("init accel config settings = %u", read_buffer_value);

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
void icm_20649_read_reg (uint8_t reg) {
    int returnVal = i2c_read_reg(i2c_device,
                                 ICM_20649_DEVICE_ADDRESS,
                                 reg,
                                 p_icm_read_buffer,
                                 ICM_READ_BUFFER_SIZE_BYTES);
    if(returnVal == 0){
        LOG_DEBUG("icm_20649_read_reg(): Successfully read data from register 0x%02X.", reg);
    } else {
        LOG_ERROR("icm_20649_read_reg(): Failed to read data from register 0x%02X.", reg);
    }
}

uint8_t icm_20649_read_reg_and_return_val (uint8_t reg) {
    int returnVal = i2c_read_reg(i2c_device,
                                 ICM_20649_DEVICE_ADDRESS,
                                 reg,
                                 p_icm_read_buffer,
                                 ICM_READ_BUFFER_SIZE_BYTES);
    if(returnVal == 0){
        LOG_DEBUG("icm_20649_read_reg(): Successfully read data from register 0x%02X.", reg);
    } else {
        LOG_ERROR("icm_20649_read_reg(): Failed to read data from register 0x%02X.", reg);
    }

    return *p_icm_read_buffer;
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
        return *p_icm_read_buffer;
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
void icm_20649_write_reg (uint8_t reg, uint8_t data)
{
    uint32_t ts = osKernelGetTickCount();
    memset (p_icm_write_buffer , data, ICM_WRITE_BUFFER_SIZE_BYTES);

    int result = i2c_write_reg (i2c_device,
                                ICM_20649_DEVICE_ADDRESS,
                                reg,
                                p_icm_write_buffer,
                                ICM_WRITE_BUFFER_SIZE_BYTES);

    if (result == 0) {
        LOG_DEBUG("icm_20649_write_reg(): Successfully wrote 0x%02X to register 0x%02X.", data, reg);
    } else {
        LOG_ERROR("icm_20649_write_reg(): Failed to write 0x%02X to register 0x%02X.", data, reg);
    }

    osDelayUntil(ts+15); // unable to read most recent writes without a delay
}

void icm_20649_read_accel_data (uint8_t accel_data[])
{
    uint8_t accel_xout_h = icm_20649_read_reg_and_return_val (ICM_20649_B0_ACCEL_XOUT_H);
    uint8_t accel_xout_l = icm_20649_read_reg_and_return_val (ICM_20649_B0_ACCEL_XOUT_L);

    uint8_t accel_yout_h = icm_20649_read_reg_and_return_val (ICM_20649_B0_ACCEL_YOUT_H);
    uint8_t accel_yout_l = icm_20649_read_reg_and_return_val (ICM_20649_B0_ACCEL_YOUT_L);

    uint8_t accel_zout_h = icm_20649_read_reg_and_return_val (ICM_20649_B0_ACCEL_ZOUT_H);
    uint8_t accel_zout_l = icm_20649_read_reg_and_return_val (ICM_20649_B0_ACCEL_ZOUT_L);

    float accel_xout = (float)combine_bytes(accel_xout_h,accel_xout_l)/ACCEL_FS_8192_LSB_PER_G;
    float accel_yout = (float)combine_bytes(accel_yout_h,accel_yout_l)/ACCEL_FS_8192_LSB_PER_G;
    float accel_zout = (float)combine_bytes(accel_zout_h,accel_zout_l)/ACCEL_FS_8192_LSB_PER_G;


    accel_data[0] = map_value(accel_xout);
    accel_data[1] = map_value(accel_yout);
    accel_data[2] = map_value(accel_zout);

}