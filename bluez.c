#include "bluez.h"

#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>

#include "app_errors.h"

//https://github.com/pauloborges/bluez/blob/master/tools/hciconfig.c

static int hdev = 0;

static int open_ctl() {
    return socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
}

static int close_ctl(int ctl) {
    return close(ctl);
}

void bluez_device_reset() {
    bluez_device_down();
    if (app_error()) {
        app_error_push("failed to reset bluez device");
        return;
    }

    bluez_device_up();
    if (app_error()) {
        app_error_push("failed to reset bluez device");
    }
}

void bluez_device_up() {
    int ctl = open_ctl();
    if (ctl<0) {
        app_error_push("failed to up bluez device: failed to open ctl");
        return;
    }

    if (ioctl(ctl, HCIDEVUP, hdev) < 0) {
        if (errno == EALREADY) return;
        app_error_push("failed to up bluez device: %s", strerror(errno));
        return;
    }

    if (close_ctl(ctl)) {
        app_error_push("failed to up bluez device: failed to close ctl");
    }
}

void bluez_device_down() {
    int ctl = open_ctl();
    if (ctl<0) {
        app_error_push("failed to down bluez device: failed to open ctl");
        return;
    }

    if (ioctl(ctl, HCIDEVDOWN, hdev) < 0) {
        app_error_push("failed to down bluez device: %s", strerror(errno));
        return;
    }

    if (close_ctl(ctl)) {
        app_error_push("failed to down bluez device: failed to close ctl");
    }
}