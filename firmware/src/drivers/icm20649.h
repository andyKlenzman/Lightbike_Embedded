#pragma once


int icm_20649_init ();
uint8_t icm_20649_get_read_buffer();
int icm_20649_init_accel ();

int icm_20649_read_reg (uint8_t reg);
uint8_t icm_20649_return_register_val (uint8_t reg);
int icm_20649_write_reg (uint8_t reg, uint8_t data);
int icm_20649_read_accel_data (uint8_t accel_data[]);
