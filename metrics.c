#include <unistd.h>

#include <microhttpd.h>
#include <promhttp.h>

#include "metrics.h"
#include "app_errors.h"

static prom_gauge_t *gauge_humidity;
static prom_gauge_t *gauge_radon_short;
static prom_gauge_t *gauge_radon_long;
static prom_gauge_t *gauge_temperature;
static prom_gauge_t *gauge_atm_pressure;
static prom_gauge_t *gauge_co2_level;
static prom_gauge_t *gauge_voc_level;

void metrics_register(const char** label_keys, int labels_count) {
    prom_collector_registry_default_init();

    gauge_humidity = prom_collector_registry_must_register_metric(
            prom_gauge_new("humidity", "% of relative humidity", labels_count, label_keys)
    );
    gauge_radon_short = prom_collector_registry_must_register_metric(
            prom_gauge_new("radon_short", "Bq/m3", labels_count, label_keys)
    );
    gauge_radon_long = prom_collector_registry_must_register_metric(
            prom_gauge_new("radon_long", "Bq/m3", labels_count, label_keys)
    );
    gauge_temperature = prom_collector_registry_must_register_metric(
            prom_gauge_new("temperature", "celsius", labels_count, label_keys)
    );
    gauge_atm_pressure = prom_collector_registry_must_register_metric(
            prom_gauge_new("atm_pressure", "hPa", labels_count, label_keys)
    );
    gauge_co2_level = prom_collector_registry_must_register_metric(
            prom_gauge_new("co2_level", "ppm", labels_count, label_keys)
    );
    gauge_voc_level = prom_collector_registry_must_register_metric(
            prom_gauge_new("voc_level", "ppb", labels_count, label_keys)
    );

    promhttp_set_active_collector_registry(NULL);
}

void metrics_start_server() {
    struct MHD_Daemon *daemon = promhttp_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, 8000, NULL, NULL);
    if (daemon == NULL) {
        app_error_push("failed to start promhttp daemon");
    }
}

void metrics_update(SensorValues* values, const char** label_values) {
    int ret = 0;

    ret = prom_gauge_set(gauge_humidity, values->humidity, label_values);
    if (ret) app_error_push("failed to set humidity: %d", ret);

    ret = prom_gauge_set(gauge_radon_short, values->radonShort, label_values);
    if (ret) app_error_push("failed to set radon_short: %d", ret);

    ret = prom_gauge_set(gauge_radon_long, values->radonLong, label_values);
    if (ret) app_error_push("failed to set radon_long: %d", ret);

    ret = prom_gauge_set(gauge_temperature, values->temperature, label_values);
    if (ret) app_error_push("failed to set temperature: %d", ret);

    ret = prom_gauge_set(gauge_atm_pressure, values->atmPressure, label_values);
    if (ret) app_error_push("failed to set atm_pressure: %d", ret);

    ret = prom_gauge_set(gauge_co2_level, values->co2Level, label_values);
    if (ret) app_error_push("failed to set co2_level: %d", ret);

    ret = prom_gauge_set(gauge_voc_level, values->vocLevel, label_values);
    if (ret) app_error_push("failed to set voc_level: %d", ret);
}