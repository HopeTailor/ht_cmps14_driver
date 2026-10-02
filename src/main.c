#include "ht_cmps14.h"
#include "esp_log.h"

#define I2C_MASTER_SDA 21
#define I2C_MASTER_SCL 22
#define I2C_MASTER_FREQ_HZ 400000

static const char *TAG = "CMPS14";

void app_main(void) {
    ESP_LOGI(TAG, "Initializing I2C Master...");
    i2c_master_bus_handle_t bus_handle;
    esp_err_t err = ht_i2c_bus_init(I2C_MASTER_SDA, I2C_MASTER_SCL, &bus_handle);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "I2C Initialization Failed!");
        return;
    }
    ESP_LOGI(TAG, "I2C Bus Initialized.");

    i2c_master_dev_handle_t cmps14_handle;
    err = ht_i2c_add_device(bus_handle, CMPS14_I2C_ADDRESS, I2C_MASTER_FREQ_HZ, &cmps14_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add CMPS14 device to bus");
        return;
    }

    err = ht_cmps14_init(cmps14_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "CMPS14 Initialization failed! Check wiring or power.");
        return;
    }
    ESP_LOGI(TAG, "CMPS14 Initialized Successfully!");

    ht_cmps14_data_t sensor_data;

    while(1) {
        err = ht_cmps14_read_all(cmps14_handle, &sensor_data);

        if(err == ESP_OK) {
            printf("Yaw: %5.1f | Pitch: %5.1f | Roll: %5.1f  ", 
                   sensor_data.yaw, sensor_data.pitch, sensor_data.roll); 
            printf(">>> Calib [Sys:%d Gyro:%d Accel:%d Mag:%d]\n",
                   sensor_data.calibration.system,
                   sensor_data.calibration.gyro,
                   sensor_data.calibration.accel,
                   sensor_data.calibration.mag);
        } 
        else {
            ESP_LOGW(TAG, "Failed to read data from sensor");
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}