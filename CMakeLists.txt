cmake_minimum_required(VERSION 3.18)
project(airthings C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)

pkg_search_module(GATTLIB REQUIRED gattlib)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0 libpcre libmicrohttpd)

link_directories(/usr/lib)
add_executable(airthings main.c toml.c toml.h config_devices.c config_devices.h config_app.c config_app.h metrics.c metrics.h bluez_scan.c bluez_scan.h bluez.c bluez.h app_errors.c app_errors.h airthings.c airthings.h)
target_link_libraries(airthings PkgConfig::deps ${GATTLIB_LIBRARIES} ${GATTLIB_LDFLAGS} pthread prom promhttp)
