#include "metrics.h"

prom_counter_t *my_counter = NULL;

int metrics_register(const char** label_keys, int labels_count) {
    prom_collector_registry_default_init();
    my_counter = prom_collector_registry_must_register_metric(
            prom_counter_new("my_counter", "counts things", labels_count, label_keys)
    );
    promhttp_set_active_collector_registry(NULL);
    return 0;
}

int metrics_start_server() {
    struct MHD_Daemon *daemon = promhttp_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, 8000, NULL, NULL);
    if (daemon == NULL) {
        return 1;
    }
    return 0;
}