#!/bin/bash

mkdir bin

cd ./raylib/src/

make PLATFORM=PLATFORM_DESKTOP USE_WAYLAND=TRUE

cd ../../

gcc main.c \
    ./raylib/src/libraylib.a \
    -lm -lpthread -ldl -lrt -lX11 \
    -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon \
    -o bin/screen-freeze 
