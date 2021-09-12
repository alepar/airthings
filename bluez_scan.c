#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gattlib.h>

#include "airthings.h"
#include "app_errors.h"
#include "bluez_scan.h"

#define BLE_SCAN_TIMEOUT   10

typedef struct {
    DiscoveredSensor* data;
    size_t discovered_count;
    size_t allocated_count;
} DiscoveryResult;

static void ensure_available(DiscoveryResult* result) {
    if (result->discovered_count < result->allocated_count) {
        return;
    }

    size_t new_count = result->allocated_count * 2 + 8;

    result->data = realloc(result->data, new_count * sizeof(DiscoveredSensor));
    if (!result->data) {
        app_error_push("failed to ensure_available for DiscoveryResult: %d", new_count);
        return;
    }
    result->allocated_count = new_count;
}

static void ble_discovered_device(void *adapter, const char* addr, const char* name, void *user_data) {
    gattlib_advertisement_data_t* adv_data;
    size_t adv_data_count;
    uint16_t manufacturer_id;
    uint8_t* manufacturer_data;
    size_t manufacturer_data_size;

    int errcode = gattlib_get_advertisement_data_from_mac(adapter, addr, &adv_data, &adv_data_count, &manufacturer_id, &manufacturer_data, &manufacturer_data_size);
    if (!errcode) {
        uint64_t serialNo = airthings_parse_serialno(manufacturer_id, manufacturer_data, manufacturer_data_size);
        if (serialNo) {
            DiscoveryResult* result = (DiscoveryResult*) user_data;
            ensure_available(result);
            if (app_iserror()) {
                app_error_push("failed to append discovered device to the result: serialNo %d at %s", serialNo, addr);
                return;
            }

            int i = result->discovered_count++;
            result->data[i].serialNo = serialNo;

            if (strlen(addr)*sizeof(char) >= sizeof(result->data[i].addr)) {
                app_error_push("failed to append discovered device to the result: address does not fit: %s", addr);
            }
            strcpy(result->data[i].addr, addr);
        }
    }
}

void bluez_scan(DiscoveredSensor** discovered_devices, size_t* discovered_devices_count) {
    void* adapter;
    int ret;

    char* adapterName = NULL;
    ret = gattlib_adapter_open(adapterName, &adapter);
    if (ret) {
        app_error_push("failed to open adapter: %d", ret);
        *discovered_devices_count = 0;
        return;
    }

    DiscoveryResult result = {
            .allocated_count = 0,
            .discovered_count = 0,
            .data = NULL,
    };

    ret = gattlib_adapter_scan_enable(adapter, ble_discovered_device, BLE_SCAN_TIMEOUT, &result);
    if (ret) {
        app_error_push("failed to scan: %d", ret);
        goto EXIT;
    }
    gattlib_adapter_scan_disable(adapter);

    *discovered_devices = result.data;
    *discovered_devices_count = result.discovered_count;

    EXIT:
    gattlib_adapter_close(adapter);
    return;
}
