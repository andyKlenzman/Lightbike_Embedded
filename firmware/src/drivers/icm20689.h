#pragma once


void icm_20649_init ();
uint8_t icm_20649_get_read_buffer();
void icm_20649_init_accel ();

void icm_20649_read_reg (uint8_t reg);
uint8_t icm_20649_read_reg_and_return_val (uint8_t reg);
void icm_20649_write_reg (uint8_t reg, uint8_t data);
void icm_20649_read_accel_data (uint8_t accel_data[]);
