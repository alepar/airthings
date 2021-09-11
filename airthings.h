#ifndef AIRTHINGS_AIRTHINGS_H
#define AIRTHINGS_AIRTHINGS_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    float humidity; // units: % of relative Humidity
    uint16_t radonShort; // units: Bq/m3
    uint16_t radonLong; // units: Bq/m3
    float temperature; // units: degrees Celsius
    float atmPressure; // units: hPa
    float co2Level; // units: ppm
    float vocLevel; // units: ppb
} SensorValues;

uint64_t airthings_parse_serialno(uint16_t manufacturer_id, const uint8_t* manufacturer_data, size_t manufacturer_data_size);
void airthings_read_characteristic(void *adapter, const char* addr, uint8_t** data, size_t* data_len);
void airthings_parse_sensor_values(SensorValues* values, uint8_t* data, size_t data_len);

#endif //AIRTHINGS_AIRTHINGS_H
