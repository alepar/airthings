#ifndef AIRTHINGS_C_CONFIG_APP_H
#define AIRTHINGS_C_CONFIG_APP_H

typedef struct {
    char *devices_config_file;
    int enable_resets;
} AppConfig;

AppConfig* parse_args(int argc, char *argv[]);

#endif //AIRTHINGS_C_CONFIG_APP_H
