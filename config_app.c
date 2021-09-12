#include <argp.h>
#include <stdlib.h>

// https://www.gnu.org/software/libc/manual/html_node/Argp-Examples.html
// https://www.linuxtopia.org/online_books/programming_books/gnu_c_programming_tutorial/argp-example.html

#include "app_errors.h"
#include "config_app.h"

static struct argp_option options[] = {
    {"enable-reset", 'r', 0, 0, "Enables HCI device reset on error. Requires root."},
    {"devices-config",  'd', "devices.toml", 0, "Output to OUTFILE instead of to standard output"},
    {0}
};

static error_t parse_opt (int key, char *arg, struct argp_state *state) {
    AppConfig* config = state->input;

    switch (key) {
        case 'r':
            config->enable_resets = 1;
            break;
        case 'd':
            config->devices_config_file = arg;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static char args_doc[] = {};

static char doc[] = "airthings -- Collects sensor data from Airthings devices and exposes it in prometheus format";

static struct argp argp = {options, parse_opt, args_doc, doc};

AppConfig* parse_args(int argc, char *argv[]) {
    AppConfig* config = (AppConfig*) calloc(1, sizeof(AppConfig));
    if (!config) {
        app_error_push("failed to allocate app config");
        return NULL;
    }

    config->devices_config_file = "devices.toml";
    config->enable_resets = 0;
    argp_parse (&argp, argc, argv, 0, 0, config);
    return config;
}