#ifndef AIRTHINGS_BLUEZ_SCAN_H
#define AIRTHINGS_BLUEZ_SCAN_H

typedef struct {
    char addr[18];
    uint64_t serialNo;
} DiscoveredSensor;

void bluez_scan(DiscoveredSensor** discovered_devices, size_t* discovered_devices_count);

#endif //AIRTHINGS_BLUEZ_SCAN_H
