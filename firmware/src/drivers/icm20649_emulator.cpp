#include "driver.h"
#include "i2c.h"
#include "icm20649_defs.h"
#include "logging.h"
#include "icm20649.h"
#include <kernel.h>
#include "../utils/combine_bytes.c"
#include "../utils/map_value.c"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

LOG_MODULE(ICM_20649_Emulator)

#define ARRAY_SIZE (3)
#define MAX_VALUE (30)


uint8_t* p_read_buffer;


uint8_t *generate_random_numbers() {
    static uint8_t numbers[ARRAY_SIZE];

    // Seed the random number generator
    srand(time(NULL));

    // Generate random numbers between 0 and 30
    for (int i = 0; i < ARRAY_SIZE; i++) {
        numbers[i] = rand() % (MAX_VALUE + 1);
    }

    return numbers;
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







uint8_t icm_20649_return_register_val (uint8_t reg) {

    p_read_buffer = generate_random_numbers();
    return *p_read_buffer;

}


int icm_20649_init () {

    return 0;

}



