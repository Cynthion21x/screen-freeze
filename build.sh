#!/bin/bash

mkdir -p bin

cd ./raylib/src/

make PLATFORM=PLATFORM_DESKTOP

cd ../../

gcc main.c \
    ./raylib/src/libraylib.a \
    -lm -lwayland-client \
    -o bin/screen-freeze 
