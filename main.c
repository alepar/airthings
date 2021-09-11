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
    app_error_exit();

    DevicesConfig *cfg = devices_config_new();
    app_error_exit();

    if (app_config->devices_config_file) {
        devices_config_read(app_config->devices_config_file, cfg);
        app_error_exit();
    }

    metrics_register(cfg->label_keys, cfg->label_keys_count);
    app_error_exit();
    metrics_start_server();
    app_error_exit();

    if (app_config->enable_resets) {
        bluez_device_up();
        app_error_exit();
    }

//    bluez_scan();

    const char* addr = "00:81:F9:F8:17:AC";

    size_t data_len;
    uint8_t* data;
    airthings_read_characteristic(NULL, addr, &data, &data_len);

    SensorValues values;
    airthings_parse_sensor_values(&values, data, data_len);

    char* label_values[] = {"temp"};

    metrics_update(&values, label_values);
    while (true);

//    DeviceConfig *dev_cfg = device_config_get(cfg, "123");
//    prom_counter_inc(my_counter, dev_cfg->label_values);
/*
    while(true) {
        bool bluez_err = false;
        if (bluez_err) {
            printf("bluez error while scanning detected");
            if (app_config->enable_resets) {
                printf("device reset enabled, resetting\n");
                if (bluez_device_reset()) {
                    printf("failed to reset device, exiting\n");
                    exit(1);
                }
            } else {
                printf("device reset disabled, ignoring\n");
            }
            bluez_err_msg = NULL;
        }

        bluez_err_msg = bluez_scan_airthings(&found_devices);
        foreach (dev in found_devices) {
            // update_known_devices
        }
        free(found_devices);

        foreach (dev in known_devices) {
            // has_been_seen_recently? remove if not
            if (time_to_query(dev)) {
                bluez_err_msg = bluez_query_airthings_waveplus(dev, &sensor_values);
                metrics_update(sensor_values, dev);
            }
        }
    }*/
}

