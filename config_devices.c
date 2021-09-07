#include "config_devices.h"

DevicesConfig* devices_config_new() {
    DevicesConfig* cfg = (DevicesConfig*)malloc(sizeof(DevicesConfig));
    if (!cfg) {
        return NULL;
    }

    cfg->serial_nos = g_hash_table_new(g_str_hash, g_str_equal);
    if (!cfg->serial_nos) {
        free(cfg);
        return NULL;
    }

    return cfg;
}

int devices_config_read(char* filename, DevicesConfig* config) {
    FILE* fp;
    char errbuf[1024];

    fp = fopen(filename, "r");
    if (!fp) {
        return 1;
    }

    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);

    if (!conf) {
        return 2;
    }

    for (int i = 0; ; i++) {
        const char* serial_no = toml_key_in(conf, i);
        if (!serial_no) break;

        toml_table_t* labels_table = toml_table_in(conf, serial_no);
        if (!labels_table) {
            return 3;
        }

        int kv_count;
        for (kv_count=0; ; kv_count++) {
            const char* key = toml_key_in(labels_table, kv_count);
            if (!key) break;
        }

        KeyValue* kvs = (KeyValue*)malloc(sizeof(KeyValue) * kv_count);
        if (!kvs) {
            return 4;
        }

        for (int j=0; ; j++) {
            const char* label_name = toml_key_in(labels_table, j);
            if (!label_name) break;

            kvs[0].key = label_name;

            toml_datum_t value = toml_string_in(labels_table, label_name);
            if (!value.ok || !value.u.s) {
                return 5;
            }
            kvs[0].value = value.u.s;
        }

        DeviceConfig* dev_cfg = (DeviceConfig*)malloc(sizeof(DeviceConfig));
        if (!dev_cfg) {
            return 6;
        }
        dev_cfg->keyValues = kvs;
        dev_cfg->keyValuesCount = kv_count;

        g_hash_table_insert(config->serial_nos, serial_no, dev_cfg);
    }

    return 0;
}

int device_config_get(DevicesConfig* config, uint64_t serialNo, DeviceConfig* info) {

}
