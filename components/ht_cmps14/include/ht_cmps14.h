#pragma once 

#include "esp_err.h"
#include "ht_i2c.h"

#define CMPS14_12C_ADDRESS       0x60
#define CMPS14_REG_COMMAND       0x00
#define CMPS14_REG_BEARING_8BIT  0x01
#define CMPS14_REG_BEARING_16_H  0x02
#define CMPS14_REG_BEARING_16_L  0x03
#define CMPS14_REG_PITCH         0x04
#define CMPS14_REG_ROLL          0x05
#define CMPS14_REG_MAG_X_H       0x06
#define CMPS14_REG_MAG_X_L       0x07
#define CMPS14_REG_MAG_Y_H       0x08
#define CMPS14_REG_MAG_Y_L       0x09
#define CMPS14_REG_MAG_Z_H       0x0A
#define CMPS14_REG_MAG_Z_L       0x0B
#define CMPS14_REG_ACCEL_X_H     0x0C
#define CMPS14_REG_ACCEL_X_L     0x0D
#define CMPS14_REG_ACCEL_Y_H     0x0E
#define CMPS14_REG_ACCEL_Y_L     0x0F
#define CMPS14_REG_ACCEL_Z_H     0x10
#define CMPS14_REG_ACCEL_Z_L     0x11
#define CMPS14_REG_GYRO_X_H      0x12
#define CMPS14_REG_GYRO_X_L      0x13
#define CMPS14_REG_GYRO_Y_H      0x14
#define CMPS14_REG_GYRO_Y_L      0x15
#define CMPS14_REG_GYRO_Z_H      0x16
#define CMPS14_REG_GYRO_Z_L      0x17
#define CMPS14_REG_TEMP_H        0x18
#define CMPS14_REG_TEMP_L        0x19
#define CMPS14_REG_PITCH_16_H    0x1A
#define CMPS14_REG_PITCH_16_L    0x1B
#define CMPS14_REG_ROLL_16_H     0x1C
#define CMPS14_REG_ROLL_16_L     0x1D
#define CMPS14_REG_CAL_STATE     0x1E
#define CMPS14_CMD_STORE_PROFILE 0x98
#define CMPS14_CMD_ERASE_PROFILE 0x99

typedef struct {
    float yaw;
    float pitch;
    float roll;
    uint8_t yaw_8bit;
    int8_t pitch_8bit;
    int8_t roll_8bit;
    int16_t mag_x;
    int16_t mag_y;
    int16_t mag_z;
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
    int16_t temperature;
    uint8_t calib_system;
    uint8_t calib_gyro;
    uint8_t calib_accel;
    uint8_t calib_mag;
} ht_cmps14_data_t;

esp_err_t ht_cmps14_init(i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t *dev);
esp_err_t ht_cmps14_read_all(i2c_master_dev_handle_t dev_handle, ht_cmps14_data_t *data);
esp_err_t ht_cmps14_send_command(i2c_master_dev_handle_t dev_handle, uint8_t cmd);



