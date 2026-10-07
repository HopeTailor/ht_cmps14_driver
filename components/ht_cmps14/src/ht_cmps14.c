/**
 * @file ht_cmps14.c
 * @brief Implementation of the CMPS14 Compass Driver
 * 
 * Handles I2C communication, sequential data bursting, and non-volatile 
 * calibration management for the CMPS14 tilt-compensated compass.
 */

#include "ht_cmps14.h"

esp_err_t ht_cmps14_init(i2c_master_dev_handle_t dev_handle) {
    uint8_t firmware_version;
    // Read the firmware version (Register 0x00) to verify I2C connectivity
    esp_err_t err = ht_i2c_read_reg8(dev_handle, CMPS14_REG_COMMAND, &firmware_version, 1);
    if(err != ESP_OK) {
        return err;
    }
    return ESP_OK;
}

esp_err_t ht_cmps14_read_all(i2c_master_dev_handle_t dev_handle, ht_cmps14_data_t *data) {
    uint8_t buf[30];
   
    // Perform a 30-byte sequential burst read starting from register 0x01
    esp_err_t err = ht_i2c_read_reg8(dev_handle, CMPS14_REG_BEARING_8BIT, buf, 30);
    if(err != ESP_OK) {
        return err;
    }

    // --- Parse Basic Angles ---
    data->yaw_8bit = buf[0];
    // Combine Big-Endian bytes for 16-bit Yaw and divide by 10 to get degrees (0.0 to 359.9)
    data->yaw = ((uint16_t)((buf[1] << 8) | buf[2])) / 10.0f;
    data->pitch_8bit = (int8_t)buf[3];
    data->roll_8bit = (int8_t)buf[4];

    // --- Parse Raw IMU Data (Magnetometer, Accelerometer, Gyroscope) ---
    data->mag_x   = (int16_t)((buf[5]  << 8) | buf[6]);
    data->mag_y   = (int16_t)((buf[7]  << 8) | buf[8]);
    data->mag_z   = (int16_t)((buf[9]  << 8) | buf[10]);

    data->accel_x = (int16_t)((buf[11] << 8) | buf[12]);
    data->accel_y = (int16_t)((buf[13] << 8) | buf[14]);
    data->accel_z = (int16_t)((buf[15] << 8) | buf[16]);

    data->gyro_x  = (int16_t)((buf[17] << 8) | buf[18]);
    data->gyro_y  = (int16_t)((buf[19] << 8) | buf[20]);
    data->gyro_z  = (int16_t)((buf[21] << 8) | buf[22]);

    // --- Parse Temperature and 16-bit High-Resolution Pitch/Roll ---
    data->temperature = (int16_t)((buf[23] << 8) | buf[24]);

    data->pitch = ((int16_t)((buf[25] << 8) | buf[26])) / 10.0f;
    data->roll = ((int16_t)((buf[27] << 8) | buf[28])) / 10.0f;

    // --- Parse Calibration Status ---
    // Calibration register (0x1E) holds 4 individual 2-bit flags
    uint8_t cal = buf[29];
    data->calibration.mag    = cal & 0x03;          // Bits 0-1
    data->calibration.accel  = (cal >> 2) & 0x03;   // Bits 2-3
    data->calibration.gyro   = (cal >> 4) & 0x03;   // Bits 4-5
    data->calibration.system = (cal >> 6) & 0x03;   // Bits 6-7

    return ESP_OK;
}

esp_err_t ht_cmps14_store_calibration(i2c_master_dev_handle_t dev_handle) {
    // The store calibration command requires a specific 3-byte sequence (0xF0, 0xF5, 0xF6)
    // written to the command register with strict 20ms delays between them.
    uint8_t cmd = 0xF0;
    esp_err_t err = ht_i2c_write_reg8(dev_handle, CMPS14_REG_COMMAND, &cmd, 1);
    if(err != ESP_OK) return err;
    vTaskDelay(pdMS_TO_TICKS(20));

    cmd = 0xF5;
    err = ht_i2c_write_reg8(dev_handle, CMPS14_REG_COMMAND, &cmd, 1);
    if(err != ESP_OK) return err;
    vTaskDelay(pdMS_TO_TICKS(20));

    cmd = 0xF6;
    err = ht_i2c_write_reg8(dev_handle, CMPS14_REG_COMMAND, &cmd, 1);
    if(err != ESP_OK) return err;
    vTaskDelay(pdMS_TO_TICKS(20));

    return ESP_OK;
}

esp_err_t ht_cmps14_erase_calibration(i2c_master_dev_handle_t dev_handle) {
    // The erase calibration command requires a specific 3-byte sequence (0xE0, 0xE5, 0xE2)
    // written to the command register with delays. The final erase takes up to 300ms.
    uint8_t cmd = 0xE0;
    esp_err_t err = ht_i2c_write_reg8(dev_handle, CMPS14_REG_COMMAND, &cmd, 1);
    if(err != ESP_OK) return err;
    vTaskDelay(pdMS_TO_TICKS(20));

    cmd = 0xE5;
    err = ht_i2c_write_reg8(dev_handle, CMPS14_REG_COMMAND, &cmd, 1);
    if(err != ESP_OK) return err;
    vTaskDelay(pdMS_TO_TICKS(20));

    cmd = 0xE2;
    err = ht_i2c_write_reg8(dev_handle, CMPS14_REG_COMMAND, &cmd, 1);
    if(err != ESP_OK) return err;
    vTaskDelay(pdMS_TO_TICKS(300)); // Allow flash memory to fully erase

    return ESP_OK;
}