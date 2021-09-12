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

    DevicesConfig *cfg = devices_config_new();
    app_onerror_exit();

    if (app_config->devices_config_file) {
        devices_config_read(app_config->devices_config_file, cfg);
        app_onerror_exit();
    }

    metrics_register(cfg->label_keys, cfg->label_keys_count);
    app_onerror_exit();
    metrics_start_server();
    app_onerror_exit();

    if (app_config->enable_resets) {
        bluez_device_reset();
        app_onerror_exit();
    }

    DiscoveredSensor* sensors;
    size_t sensors_count;
    bluez_scan(&sensors, &sensors_count);
    app_onerror_exit();

    printf("found: %zu\n", sensors_count);
    free(sensors);

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

