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
    uint8_t buf[30];
    esp_err_t err = ht_i2c_read_reg8(dev_handle, CMPS14_REG_BEARING_8BIT, buf, 30);
    if(err != ESP_OK) {
        return err;
    }

    data->yaw_8bit = buf[0];
    data->pitch_8bit = (int8_t)buf[3];
    data->roll_8bit = (int8_t)buf[4];

    data->yaw = ((uint16_t)((buf[1] << 8) | buf[2])) / 10.0f;
    data->pitch = ((int16_t)((buf[25] << 8) | buf[26])) / 10.0f;
    data->roll = ((int16_t)((buf[27] << 8) | buf[28])) / 10.0f;

    data->mag_x   = (int16_t)((buf[5]  << 8) | buf[6]);
    data->mag_y   = (int16_t)((buf[7]  << 8) | buf[8]);
    data->mag_z   = (int16_t)((buf[9]  << 8) | buf[10]);

    data->accel_x = (int16_t)((buf[11] << 8) | buf[12]);
    data->accel_y = (int16_t)((buf[13] << 8) | buf[14]);
    data->accel_z = (int16_t)((buf[15] << 8) | buf[16]);

    data->gyro_x  = (int16_t)((buf[17] << 8) | buf[18]);
    data->gyro_y  = (int16_t)((buf[19] << 8) | buf[20]);
    data->gyro_z  = (int16_t)((buf[21] << 8) | buf[22]);

    data->temperature = (int16_t)((buf[23] << 8) | buf[24]);

    uint8_t cal = buf[29];
    data->calib_mag    = cal & 0x03;
    data->calib_accel  = (cal >> 2) & 0x03;
    data->calib_gyro   = (cal >> 4) & 0x03;
    data->calib_system = (cal >> 6) & 0x03;

    return ESP_OK;
}

esp_err_t ht_cmps14_send_command(i2c_master_dev_handle_t dev_handle, uint8_t cmd) {

}

