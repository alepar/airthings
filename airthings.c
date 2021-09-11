#include "airthings.h"
#include "app_errors.h"

#include <gattlib.h>

uint64_t airthings_parse_serialno(uint16_t manufacturer_id, const uint8_t* manufacturer_data, size_t manufacturer_data_size) {
    if (manufacturer_id == 820 && manufacturer_data_size >= 4) {
        uint64_t serialNo = 0;
        serialNo += (uint64_t) manufacturer_data[0];
        serialNo += ((uint64_t) manufacturer_data[1]) << 8;
        serialNo += ((uint64_t) manufacturer_data[2]) << 16;
        serialNo += ((uint64_t) manufacturer_data[3]) << 24;
        return serialNo;
    }

    return 0;
}

static const uuid_t airthings_waveplus_sensor_char = {
    .type = SDP_UUID128,
    .value.uuid128 = {0xb4, 0x2e, 0x2a, 0x68, 0xad, 0xe7, 0x11, 0xe4, 0x89, 0xd3, 0x12, 0x3b, 0x93, 0xf7, 0x5c, 0xba}
};

void airthings_read_characteristic(void *adapter, const char* addr, uint8_t** data, size_t* data_len) {
    gatt_connection_t* connection = gattlib_connect(adapter, addr, GATTLIB_CONNECTION_OPTIONS_LEGACY_DEFAULT);
    if (connection == NULL) {
        app_error_push("failed to read sensor values: connection failed");
        return;
    }

    int ret = gattlib_read_char_by_uuid(connection, &airthings_waveplus_sensor_char, (void **)data, data_len);
    if (ret != GATTLIB_SUCCESS) {
        app_error_push("failed to read sensor values: read failed: %d", ret);
    }

    ret = gattlib_disconnect(connection);
    if (ret != GATTLIB_SUCCESS) {
        app_error_push("failed to read sensor values: disconnect failed: %d", ret);
    }
}

static float read_float(uint8_t* ptr) {
    return (float)ptr[0] + ((float)ptr[1])*256;
}
static uint16_t read_uint16t(uint8_t* ptr) {
    return (uint16_t)ptr[0] + ((uint16_t)ptr[1]<<8);
}

void airthings_parse_sensor_values(SensorValues* values, uint8_t* data, size_t data_len) {
    if (data_len < 16) {
        app_error_push("failed to parse sensor values: data is too short: %d", data_len);
        return;
    }

    values->humidity = data[1] / 2.0f;
    values->radonShort = read_uint16t(&data[4]);
    values->radonLong = read_uint16t(&data[6]);
    values->temperature = read_float(&data[8]) / 100.0f;
    values->atmPressure = read_float(&data[10]) / 50.0f;
    values->co2Level = read_float(&data[12]);
    values->vocLevel = read_float(&data[14]);
}