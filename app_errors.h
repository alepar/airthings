#ifndef AIRTHINGS_APP_ERRORS_H
#define AIRTHINGS_APP_ERRORS_H

#include <stdbool.h>

// returns true if error is raised
bool app_error();

// if error is raised, prints it to stderr and clears it
void app_error_print();

// if error is raised, prints it to stderr and terminates
void app_error_exit();

// raises error, adds errmsg to the top of the stack
void app_error_push(const char *__restrict format, ...);

#endif //AIRTHINGS_APP_ERRORS_H
