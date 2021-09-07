#ifndef AIRTHINGS_C_CONFIG_DEVICES_H
#define AIRTHINGS_C_CONFIG_DEVICES_H

#include <errno.h>
#include <glib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "toml.h"

typedef struct {
    GHashTable* serial_nos;
} DevicesConfig;

typedef struct {
    char* key;
    char* value;
} KeyValue;

typedef struct {
    KeyValue* keyValues;
    int keyValuesCount;
} DeviceConfig;

DevicesConfig* devices_config_new();
int devices_config_read(char* filename, DevicesConfig* config);
int device_config_get(DevicesConfig* config, uint64_t serialNo, DeviceConfig* info);

#endif //AIRTHINGS_C_CONFIG_DEVICES_H
