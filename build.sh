#!/bin/bash

mkdir bin

cd ./raylib/src/

make PLATFORM=PLATFORM_DESKTOP USE_WAYLAND=TRUE

cd ../../

gcc main.c \
    ./raylib/src/libraylib.a \
    -lm -lX11 \
    -o bin/screen-freeze 
