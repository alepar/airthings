#ifndef AIRTHINGS_METRICS_H
#define AIRTHINGS_METRICS_H

#include <prom.h>

void metrics_register(const char** label_keys, int labels_count);
void metrics_start_server();

extern prom_counter_t *my_counter;

#endif //AIRTHINGS_METRICS_H
