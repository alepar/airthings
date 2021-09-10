#ifndef AIRTHINGS_C_CONFIG_DEVICES_H
#define AIRTHINGS_C_CONFIG_DEVICES_H

#include <glib.h>

typedef struct {
    GHashTable* serial_nos;
    const char** label_keys;
    int  label_keys_count;
} DevicesConfig;

typedef struct {
    const char** label_values;
} DeviceConfig;

DevicesConfig* devices_config_new();
void devices_config_read(char* filename, DevicesConfig* config);
DeviceConfig* device_config_get(DevicesConfig* config, char* serialNo);

#endif //AIRTHINGS_C_CONFIG_DEVICES_H
