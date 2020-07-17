#!/bin/bash

export DBUS_SYSTEM_BUS_ADDRESS=unix:path=/host/run/dbus/system_bus_socket

# run our binary
./hello
#./rpi-rgb-led-matrix/examples-api-use/demo -D0 --led-rows=32 --led-cols=64 --led-multiplexing=2 --led-slowdown-gpio=2 --led-gpio-mapping=adafruit-hat-pwm 