#include <stdio.h>
#include <stdlib.h>

#include <gattlib.h>

#include "airthings.h"
#include "bluez_scan.h"

#define BLE_SCAN_TIMEOUT   10

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
            printf("Discovered %ld at %s\n", serialNo, addr);
        }
    }
}

void bluez_scan() {
    void* adapter;
    int ret;

    char* adapterName = NULL;
    ret = gattlib_adapter_open(adapterName, &adapter);
    if (ret) {
        fprintf(stderr, "ERROR: Failed to open adapter.\n");
        return;
    }

    ret = gattlib_adapter_scan_enable(adapter, ble_discovered_device, BLE_SCAN_TIMEOUT, NULL /* user_data */);
    if (ret) {
        fprintf(stderr, "ERROR: Failed to scan.\n");
        goto EXIT;
    }

    gattlib_adapter_scan_disable(adapter);

    puts("Scan completed");

    EXIT:
    gattlib_adapter_close(adapter);
    return;
}
