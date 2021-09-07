#ifndef AIRTHINGS_METRICS_H
#define AIRTHINGS_METRICS_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <microhttpd.h>
#include <prom.h>
#include <promhttp.h>

int metrics_register();
int metrics_start_server();

extern prom_counter_t *my_counter;

#endif //AIRTHINGS_METRICS_H
