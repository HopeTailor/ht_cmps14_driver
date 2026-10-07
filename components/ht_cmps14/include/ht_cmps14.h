/**
 * @file ht_cmps14.h
 * @brief Professional CMPS14 Tilt Compensated Compass Driver for ESP-IDF
 * 
 * Provides a non-blocking interface to read heading (yaw), pitch, roll, 
 * raw IMU data, and manage hardware calibration profiles over I2C.
 */

#pragma once 

#include "esp_err.h"
#include "ht_i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* CMPS14 Default 7-bit I2C Address */
#define CMPS14_I2C_ADDRESS           0x60

/* Core Register Map */
#define CMPS14_REG_COMMAND           0x00
#define CMPS14_REG_BEARING_8BIT      0x01
#define CMPS14_REG_BEARING_16_H      0x02
#define CMPS14_REG_BEARING_16_L      0x03
#define CMPS14_REG_PITCH             0x04
#define CMPS14_REG_ROLL              0x05
#define CMPS14_REG_MAG_X_H           0x06
#define CMPS14_REG_MAG_X_L           0x07
#define CMPS14_REG_MAG_Y_H           0x08
#define CMPS14_REG_MAG_Y_L           0x09
#define CMPS14_REG_MAG_Z_H           0x0A
#define CMPS14_REG_MAG_Z_L           0x0B
#define CMPS14_REG_ACCEL_X_H         0x0C
#define CMPS14_REG_ACCEL_X_L         0x0D
#define CMPS14_REG_ACCEL_Y_H         0x0E
#define CMPS14_REG_ACCEL_Y_L         0x0F
#define CMPS14_REG_ACCEL_Z_H         0x10
#define CMPS14_REG_ACCEL_Z_L         0x11
#define CMPS14_REG_GYRO_X_H          0x12
#define CMPS14_REG_GYRO_X_L          0x13
#define CMPS14_REG_GYRO_Y_H          0x14
#define CMPS14_REG_GYRO_Y_L          0x15
#define CMPS14_REG_GYRO_Z_H          0x16
#define CMPS14_REG_GYRO_Z_L          0x17
#define CMPS14_REG_TEMP_H            0x18
#define CMPS14_REG_TEMP_L            0x19
#define CMPS14_REG_PITCH_16_H        0x1A
#define CMPS14_REG_PITCH_16_L        0x1B
#define CMPS14_REG_ROLL_16_H         0x1C
#define CMPS14_REG_ROLL_16_L         0x1D
#define CMPS14_REG_CAL_STATE         0x1E

/* Calibration Commands */
#define CMPS14_CMD_STORE_CALIBRATION 0x98
#define CMPS14_CMD_ERASE_CALIBRATION 0x99

/**
 * @brief Comprehensive data structure holding all CMPS14 sensor readings.
 */
typedef struct {
    float yaw;               /**< 16-bit Heading/Yaw angle in degrees (0.0 to 359.9) */
    float pitch;             /**< 16-bit Pitch angle in degrees (+/- 90.0) */
    float roll;              /**< 16-bit Roll angle in degrees (+/- 90.0) */
    
    uint8_t yaw_8bit;        /**< 8-bit Heading/Yaw (0-255 representing 0-359.9) */
    int8_t pitch_8bit;       /**< 8-bit Pitch angle (+/- 90) */
    int8_t roll_8bit;        /**< 8-bit Roll angle (+/- 90) */
    
    int16_t mag_x;           /**< Raw Magnetometer X-axis data */
    int16_t mag_y;           /**< Raw Magnetometer Y-axis data */
    int16_t mag_z;           /**< Raw Magnetometer Z-axis data */
    
    int16_t accel_x;         /**< Raw Accelerometer X-axis data */
    int16_t accel_y;         /**< Raw Accelerometer Y-axis data */
    int16_t accel_z;         /**< Raw Accelerometer Z-axis data */
    
    int16_t gyro_x;          /**< Raw Gyroscope X-axis data */
    int16_t gyro_y;          /**< Raw Gyroscope Y-axis data */
    int16_t gyro_z;          /**< Raw Gyroscope Z-axis data */
    
    int16_t temperature;     /**< Sensor internal temperature */
    
    /**
     * @brief Internal calibration status (0 = uncalibrated, 3 = fully calibrated).
     */
    struct {
        uint8_t system;      /**< Overall system calibration status */
        uint8_t gyro;        /**< Gyroscope calibration status */
        uint8_t accel;       /**< Accelerometer calibration status */
        uint8_t mag;         /**< Magnetometer calibration status */
    } calibration;
} ht_cmps14_data_t;

/**
 * @brief Initializes the CMPS14 sensor by reading the firmware version.
 * 
 * @param dev_handle I2C device handle for the sensor.
 * @return esp_err_t ESP_OK if sensor responds, or ESP_ERR_NOT_FOUND/TIMEOUT otherwise.
 */
esp_err_t ht_cmps14_init(i2c_master_dev_handle_t dev_handle);

/**
 * @brief Reads all 30 bytes of data (angles, IMU, temperature, calibration) sequentially.
 * 
 * @param dev_handle I2C device handle for the sensor.
 * @param data Pointer to the structure where the parsed data will be stored.
 * @return esp_err_t ESP_OK on success.
 */
esp_err_t ht_cmps14_read_all(i2c_master_dev_handle_t dev_handle, ht_cmps14_data_t *data);

/**
 * @brief Stores the current calibration profile into the sensor's non-volatile memory (NVM).
 * 
 * @param dev_handle I2C device handle for the sensor.
 * @return esp_err_t ESP_OK on success.
 */
esp_err_t ht_cmps14_store_calibration(i2c_master_dev_handle_t dev_handle);

/**
 * @brief Erases the stored calibration profile from the sensor's non-volatile memory (NVM).
 * 
 * @param dev_handle I2C device handle for the sensor.
 * @return esp_err_t ESP_OK on success.
 */
esp_err_t ht_cmps14_erase_calibration(i2c_master_dev_handle_t dev_handle);