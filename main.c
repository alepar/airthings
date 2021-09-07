#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    DevicesConfig *cfg = devices_config_new();
    if (!cfg) {
        fatal("failed to allocate config");
    }

    if (app_config->devices_config_file) {
        int err = devices_config_read(app_config->devices_config_file, cfg);
        if (err) {
            fatal("failed to read config from file");
        }
    }

    guint size = g_hash_table_size(cfg->serial_nos);
    printf("%d\n", size);

    metrics_register();
    metrics_start_server();
    prom_counter_inc(my_counter, NULL);

    int done = 0;
    while(done == 0) {}

    return 0;
}

