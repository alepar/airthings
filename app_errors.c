#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "app_errors.h"

static char temp[10240] = {0};
static char temp2[10240] = {0};
static char errmsg[10240] = {0};

bool app_iserror() {
    return strlen(errmsg) > 0;
}

void app_onerror_print() {
    if (app_iserror()) {
        fprintf(stderr, "ERROR: %s\n", errmsg);
        memset(errmsg, 0, sizeof(errmsg));
    }
}

void app_onerror_exit() {
    if (app_iserror()) {
        fprintf(stderr, "FATAL: %s\n", errmsg);
        exit(1);
    }
}

void app_error_push(const char *__restrict format, ...) {
    va_list argp;
    va_start(argp, format);
    vsprintf(temp, format, argp);
    va_end(argp);

    if (!strlen(errmsg)) {
        strcpy(errmsg, temp);
    } else {
        sprintf(temp2, "%s: %s", temp, errmsg);
        strcpy(errmsg, temp2);
    }
}
