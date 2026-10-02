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
#define CMPS14_REG_CAL_STATE     0x1E
#define CMPS14_CMD_STORE_PROFILE 0x98
#define CMPS14_CMD_ERASE_PROFILE 0x99

typedef struct {
    float heading;
    int8_t pitch;
    int8_t roll;
    uint8_t calib_system;
    uint8_t calib_gyro;
    uint8_t calib_accel;
    uint8_t calib_mag;
} ht_cmps14_data_t;

esp_err_t ht_cmps14_init(i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t *dev);
esp_err_t ht_cmps14_read_data(i2c_master_dev_handle_t dev_handle, ht_cmps14_data_t *data);