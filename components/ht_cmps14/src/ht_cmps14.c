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

}

esp_err_t ht_cmps14_send_command(i2c_master_dev_handle_t dev_handle, uint8_t cmd) {

}

