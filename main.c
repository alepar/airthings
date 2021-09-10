#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app_errors.h"
#include "bluez.h"
#include "bluez_scan.h"
#include "config_app.h"
#include "config_devices.h"
#include "metrics.h"

static void fatal(const char* msg) {
    char *errmsg = strerror(errno);
    fprintf(stderr, "ERROR: %s - %s\n", msg, errmsg?errmsg:"");
    exit(1);
}

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

    DeviceConfig *dev_cfg = device_config_get(cfg, "123");
    prom_counter_inc(my_counter, dev_cfg->label_values);

    int done = 0;
    while(done == 0) {}

    return 0;
}

