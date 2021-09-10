#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "toml.h"

#include "app_errors.h"
#include "config_devices.h"

DevicesConfig* devices_config_new() {
    DevicesConfig* cfg = (DevicesConfig*)malloc(sizeof(DevicesConfig));
    if (!cfg) {
        app_error_push("failed to allocate devices config");
        return NULL;
    }

    cfg->serial_nos = g_hash_table_new(g_str_hash, g_str_equal);
    if (!cfg->serial_nos) {
        free(cfg);
        app_error_push("failed to allocate hast table for devices config");
        return NULL;
    }

    return cfg;
}

void devices_config_read(char* filename, DevicesConfig* config) {
    FILE* fp;
    char errbuf[1024];

    fp = fopen(filename, "r");
    if (!fp) {
        app_error_push("failed to open file '%s': %s", filename, strerror(errno));
        return;
    }

    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);
    if (!conf) {
        app_error_push("failed to parse devices config toml file: %s", errbuf);
        return;
    }

    // round 1 - collect all label keys
    GHashTable *label_keys_set = g_hash_table_new(g_str_hash, g_str_equal);
    for (int i = 0; ; i++) {
        const char* serial_no = toml_key_in(conf, i);
        if (!serial_no) break;

        toml_table_t* labels_table = toml_table_in(conf, serial_no);
        if (!labels_table) {
            app_error_push("failed to parse devices config toml file for device '%s': %s", serial_no, errbuf);
            return;
        }

        for (int j=0; ; j++) {
            const char* label_name = toml_key_in(labels_table, j);
            if (!label_name) break;

            g_hash_table_add(label_keys_set, g_strdup(label_name));
        }
    }

    // collect label keys to char array
    gpointer *gptr = g_hash_table_get_keys_as_array(label_keys_set, &config->label_keys_count);
    if (!gptr) {
        app_error_push("failed to parse devices config toml file: failed to collect label keys");
        return;
    }
    config->label_keys = (const char * *) malloc(sizeof(char*) * config->label_keys_count);
    if (!config->label_keys) {
        app_error_push("failed to parse devices config toml file: failed to allocate label keys array");
        return;
    }

    for (int i=0; i<config->label_keys_count; i++) {
        config->label_keys[i] = gptr[i];
    }
    g_free(gptr);
    g_hash_table_destroy(label_keys_set); // note that the keys themselves are preserved for future use below

    // round 2 - fill in label values
    for (int i = 0; ; i++) {
        const char* serial_no = toml_key_in(conf, i);
        if (!serial_no) break;

        toml_table_t* labels_table = toml_table_in(conf, serial_no);
        if (!labels_table) {
            app_error_push("failed to parse devices config toml file for device '%s': %s", serial_no, errbuf);
            return;
        }

        DeviceConfig* dev_cfg = (DeviceConfig *) malloc(sizeof(DeviceConfig));
        if (!dev_cfg) {
            app_error_push("failed to parse devices config toml file for device '%s': failed to allocate DeviceConfig", serial_no);
            return;
        }
        char** label_values = (char**)malloc(sizeof(char*) * config->label_keys_count);
        if (!label_values) {
            app_error_push("failed to parse devices config toml file for device '%s': failed to allocate label_values", serial_no);
            return;
        }
        for (int j=0; j<config->label_keys_count; j++) {
            const char* label_name = config->label_keys[j];
            label_values[j] = toml_string_in(labels_table, label_name).u.s;
        }

        dev_cfg->label_values = (const char**)label_values;
        g_hash_table_insert(config->serial_nos, g_strdup(serial_no), dev_cfg);
    }

    toml_free(conf);
}

DeviceConfig* device_config_get(DevicesConfig* config, char* serialNo) {
    return (DeviceConfig*) g_hash_table_lookup(config->serial_nos, serialNo);
}
