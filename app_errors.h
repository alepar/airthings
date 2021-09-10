#ifndef AIRTHINGS_APP_ERRORS_H
#define AIRTHINGS_APP_ERRORS_H

#include <stdbool.h>

bool app_error();
void app_error_print();
void app_error_exit();
void app_error_push(const char *__restrict format, ...);

#endif //AIRTHINGS_APP_ERRORS_H
