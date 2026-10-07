#include "ht_cmps14.h"

esp_err_t ht_cmps14_init(i2c_master_dev_handle_t dev_handle) {
    uint8_t firmware_version;
    esp_err_t err = ht_i2c_read_reg8(dev_handle, CMPS14_REG_COMMAND, &firmware_version, 1);
    if(err != ESP_OK) {
        return err;
    }
    return ESP_OK;
}

esp_err_t ht_cmps14_read_all(i2c_master_dev_handle_t dev_handle, ht_cmps14_data_t *data) {
    uint8_t buf_angles[5], buf_imu[18], buf_ext[7];
    esp_err_t err = ht_i2c_read_reg8(dev_handle, CMPS14_REG_BEARING_8BIT, buf_angles, 5);
    if(err != ESP_OK) {
        return err;
    }

    err = ht_i2c_read_reg8(dev_handle, CMPS14_REG_MAG_X_H, buf_imu, 18);
    if(err != ESP_OK) {
        return err;
    }

    err = ht_i2c_read_reg8(dev_handle, CMPS14_REG_TEMP_H, buf_ext, 7);
    if(err != ESP_OK) {
        return err;
    }

    data->yaw_8bit = buf_angles[0];
    data->pitch_8bit = (int8_t)buf_angles[3];
    data->roll_8bit = (int8_t)buf_angles[4];

    data->yaw = ((uint16_t)((buf_angles[1] << 8) | buf_angles[2])) / 10.0f;
    data->pitch = ((int16_t)((buf_ext[2] << 8) | buf_ext[3])) / 10.0f;
    data->roll = ((int16_t)((buf_ext[4] << 8) | buf_ext[5])) / 10.0f;

    data->mag_x   = (int16_t)((buf_imu[0]  << 8) | buf_imu[1]);
    data->mag_y   = (int16_t)((buf_imu[2]  << 8) | buf_imu[3]);
    data->mag_z   = (int16_t)((buf_imu[4]  << 8) | buf_imu[5]);

    data->accel_x = (int16_t)((buf_imu[6]  << 8) | buf_imu[7]);
    data->accel_y = (int16_t)((buf_imu[8]  << 8) | buf_imu[9]);
    data->accel_z = (int16_t)((buf_imu[10] << 8) | buf_imu[11]);

    data->gyro_x  = (int16_t)((buf_imu[12] << 8) | buf_imu[13]);
    data->gyro_y  = (int16_t)((buf_imu[14] << 8) | buf_imu[15]);
    data->gyro_z  = (int16_t)((buf_imu[16] << 8) | buf_imu[17]);

    data->temperature = (int16_t)((buf_ext[0] << 8) | buf_ext[1]);

    uint8_t cal = buf_ext[6];
    data->calibration.mag    = cal & 0x03;
    data->calibration.accel  = (cal >> 2) & 0x03;
    data->calibration.gyro   = (cal >> 4) & 0x03;
    data->calibration.system = (cal >> 6) & 0x03;
    
    return ESP_OK;
}

esp_err_t ht_cmps14_store_calibration(i2c_master_dev_handle_t dev_handle) {
    uint8_t cmd = 0xF0;
    esp_err_t err = ht_i2c_write_reg8(dev_handle, CMPS14_REG_COMMAND, &cmd, 1);
    if(err != ESP_OK) {
        return err;
    } 
    vTaskDelay(pdMS_TO_TICKS(20));

    cmd = 0xF5;
    err = ht_i2c_write_reg8(dev_handle, CMPS14_REG_COMMAND, &cmd, 1);
    if(err != ESP_OK) {
        return err;
    } 
    vTaskDelay(pdMS_TO_TICKS(20));

    cmd = 0xF6;
    err = ht_i2c_write_reg8(dev_handle, CMPS14_REG_COMMAND, &cmd, 1);
    if(err != ESP_OK) {
        return err;
    } 
    vTaskDelay(pdMS_TO_TICKS(20));

    return ESP_OK;
}

esp_err_t ht_cmps14_erase_calibration(i2c_master_dev_handle_t dev_handle) {
    uint8_t cmd = 0xE0;
    esp_err_t err = ht_i2c_write_reg8(dev_handle, CMPS14_REG_COMMAND, &cmd, 1);
    if(err != ESP_OK) {
        return err;
    } 
    vTaskDelay(pdMS_TO_TICKS(20));

    cmd = 0xE5;
    err = ht_i2c_write_reg8(dev_handle, CMPS14_REG_COMMAND, &cmd, 1);
    if(err != ESP_OK) {
        return err;
    } 
    vTaskDelay(pdMS_TO_TICKS(20));

    cmd = 0xE2;
    err = ht_i2c_write_reg8(dev_handle, CMPS14_REG_COMMAND, &cmd, 1);
    if(err != ESP_OK) {
        return err;
    } 
    vTaskDelay(pdMS_TO_TICKS(300));

    return ESP_OK;
}


