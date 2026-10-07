# Professional CMPS14 Tilt-Compensated Compass Driver for ESP-IDF (`ht_cmps14`)

A robust, industrial-grade C driver for the CMPS14 tilt-compensated compass, optimized for the Espressif ESP-IDF framework and FreeRTOS. 

This library efficiently handles the I2C interface to extract 16-bit precision Yaw, Pitch, and Roll angles, raw IMU data (Accelerometer, Gyroscope, Magnetometer), and manages the internal Non-Volatile Memory (NVM) calibration profiles using optimized sequential burst reads.

## 🚀 Features
* **High-Speed Sequential Reads:** Extracts all 30 bytes of sensor data (angles, IMU raw data, and temperature) in a single optimized I2C burst to prevent bus saturation.
* **16-Bit Precision:** Fully parses and combines Big-Endian byte pairs to deliver floating-point degrees with 0.1° resolution.
* **Hardware Calibration Management:** Includes dedicated sequences with precise timing requirements to store or erase the sensor's internal background calibration profiles.
* **ESP-IDF v5+ Compatible:** Built on top of a custom, modern I2C abstraction layer (`ht_i2c`) utilizing the latest handle-based ESP-IDF APIs.

## 🔌 Wiring & Hardware Setup

| CMPS14 Pin | ESP32 Pin (Default) | Notes |
| :--- | :--- | :--- |
| **+5V / VCC** | 3.3V or 5V | *Operates typically at 3.3V to match ESP32 logic levels without level shifters.* |
| **GND** | GND | Common ground. |
| **SCL** | GPIO 22 | I2C Clock (Requires 4.7kΩ pull-up resistor). |
| **SDA** | GPIO 21 | I2C Data (Requires 4.7kΩ pull-up resistor). |

> **🔥 CRITICAL HARDWARE WARNING:** The CMPS14 is highly sensitive to reverse polarity and short circuits. If you connect VCC and GND incorrectly, or if there is a solder bridge on your PCB, **the main IC will rapidly overheat and permanently destroy the module.** Always verify your power rails with a multimeter before powering the sensor.

## 💻 Quick Start Usage

```c
#include "ht_i2c.h"
#include "ht_cmps14.h"
#include "esp_log.h"

void app_main(void) {
    // 1. Initialize I2C Master
    i2c_master_bus_handle_t bus_handle;
    ht_i2c_bus_init(21, 22, &bus_handle);

    // 2. Add CMPS14 to the Bus (Default Address: 0x60)
    i2c_master_dev_handle_t cmps14_handle;
    ht_i2c_add_device(bus_handle, CMPS14_I2C_ADDRESS, 400000, &cmps14_handle);

    // 3. Initialize and Verify Connection
    if (ht_cmps14_init(cmps14_handle) != ESP_OK) {
        printf("Failed to initialize CMPS14!\n");
        return;
    }

    ht_cmps14_data_t sensor_data;

    // 4. Main Reading Loop
    while(1) {
        if(ht_cmps14_read_all(cmps14_handle, &sensor_data) == ESP_OK) {
            printf("Yaw: %5.1f° | Pitch: %5.1f° | Roll: %5.1f°\n", 
                   sensor_data.yaw, sensor_data.pitch, sensor_data.roll); 
            
            // Check calibration status (0 = Uncalibrated, 3 = Fully Calibrated)
            printf("Calib -> Sys: %d | Gyro: %d | Accel: %d | Mag: %d\n",
                   sensor_data.calibration.system,
                   sensor_data.calibration.gyro,
                   sensor_data.calibration.accel,
                   sensor_data.calibration.mag);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
```

## ⚙️ Calibration Management

The CMPS14 features an advanced background calibration algorithm. The calibration state for each component (System, Gyro, Accel, Mag) is returned as a value from `0` to `3`:
* `0`: Uncalibrated
* `3`: Fully Calibrated

To save the current calibration state so it persists after a power cycle, call:
```c
ht_cmps14_store_calibration(cmps14_handle);
```

If the sensor becomes "stuck" or outputs incorrect data due to a bad magnetic environment, erase the calibration profile (takes ~300ms) and restart the calibration process:
```c
ht_cmps14_erase_calibration(cmps14_handle);
```

## 🛠️ Troubleshooting

*   **Values are stuck at `0`:** This usually indicates an I2C buffer overflow or a wiring fault. Ensure you are using the 30-byte sequential read correctly and that pull-up resistors are installed.
*   **Sensor is Not Found during I2C Scan:** Check that the I2C address is `0x60` (7-bit format). Some documentation refers to `0xC0`, which is the 8-bit shifted address. ESP-IDF requires the 7-bit `0x60`.