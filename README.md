
# Compiling

## Prereqs
- Install `cmake`, `build-essential`

## Install gattlib  

- Install `python3-dev`, `bluez`, `doxygen`, `libglib2.0-dev`

Reference: https://github.com/labapart/gattlib/blob/master/README.md#build-gattlib
- Install `libbluetooth-dev`, `libreadline-dev`
- Check out `https://github.com/labapart/gattlib.git`
- `mkdir build && cd build && cmake .. && make`
- `sudo make install`
- check `/usr/local/lib` is in your `/etc/ld.so.conf.d/`
- `sudo ldconfig`

## Install libprom
- Install deps `apt install libmicrohttpd-dev`
- Install docker engine `apt install docker-ce`
- Make sure your user has access to docker (eg add to `docker` group)
- check out `https://github.com/digitalocean/prometheus-client-c`
- apply fixes from `https://github.com/digitalocean/prometheus-client-c/issues/58`
- Install generated debs `libprom` and `libpromhttp`