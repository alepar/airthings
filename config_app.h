#ifndef AIRTHINGS_C_CONFIG_APP_H
#define AIRTHINGS_C_CONFIG_APP_H

#include <argp.h>
#include <stdlib.h>

// https://www.gnu.org/software/libc/manual/html_node/Argp-Examples.html
// https://www.linuxtopia.org/online_books/programming_books/gnu_c_programming_tutorial/argp-example.html

typedef struct {
    char *devices_config_file;
    int enable_resets;
} AppConfig;

AppConfig* parse_args(int argc, char *argv[]);

#endif //AIRTHINGS_C_CONFIG_APP_H
