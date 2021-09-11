#ifndef AIRTHINGS_METRICS_H
#define AIRTHINGS_METRICS_H

#include <prom.h>

#include "airthings.h"

void metrics_register(const char** label_keys, int labels_count);
void metrics_start_server();

void metrics_update(SensorValues* values, const char** label_values);

#endif //AIRTHINGS_METRICS_H
