#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gattlib.h>

#include "app_errors.h"
#include "airthings.h"
#include "bluez.h"
#include "bluez_scan.h"
#include "config_app.h"
#include "config_devices.h"
#include "metrics.h"

int main(int argc, char *argv[]) {
    AppConfig *app_config = parse_args(argc, argv);
    app_onerror_exit();

    DevicesConfig *dev_cfg = devices_config_new();
    app_onerror_exit();

    if (app_config->devices_config_file) {
        devices_config_read(app_config->devices_config_file, dev_cfg);
        app_onerror_exit();
    }

    metrics_register(dev_cfg->label_keys, dev_cfg->label_keys_count);
    app_onerror_exit();
    metrics_start_server();
    app_onerror_exit();

    if (app_config->enable_resets) {
        bluez_device_reset();
        app_onerror_exit();
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    bool ble_error = false;

    while(true) {
        if (ble_error) {
            if (app_config->enable_resets) {
                printf("resetting bluez device\n");
                bluez_device_reset();
                app_onerror_exit();
            }
            ble_error = false;
        }

        DiscoveredSensor *sensors;
        size_t sensors_count;
        bluez_scan(&sensors, &sensors_count);
        if (app_onerror_print()) {
            ble_error = true;
            continue;
        }

        printf("found: %zu\n", sensors_count);
        for (int i = 0; i < sensors_count; i++) {
            DiscoveredSensor *sensor = &sensors[i];
            printf("- %lu at %s\n", sensor->serialNo, sensor->addr);
            uint8_t *data;
            size_t data_len;
            airthings_read_characteristic(NULL, sensor->addr, &data, &data_len);
            if (app_onerror_print()) {
                ble_error = true;
                continue;
            }

            SensorValues values;
            airthings_parse_sensor_values(&values, data, data_len);
            if (app_onerror_print()) {
                ble_error = true;
                continue;
            }

            char serialNoStr[128];
            sprintf(serialNoStr, "%lu", sensor->serialNo);
            DeviceConfig *dev = device_config_get(dev_cfg, serialNoStr);
            metrics_update(&values, dev->label_values);
            if (app_onerror_print()) {
                ble_error = true;
                continue;
            }
        }
        free(sensors);

        //TODO remove stale sensor values
    }
#pragma clang diagnostic pop
}

