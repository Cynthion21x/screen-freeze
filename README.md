# screen-freeze

Designed to be used with a pipe from grim like so

`grim - | screen-freeze -s | wl-copy`

You can use the `-z` option to zoom. Drag to pan around and press control to use a magnifying lens in zoom mode. Use the scroll wheel to zoom in and out.

Each mode takes in an image, and screenshot lets you cut from that image and zoom lets you zoom that image. So it could be used as an image croping tool potentially. 

Depends on raylib. The build.sh here builds for X11 but to build for wayland set wayland to be true in the raylib makefile and change -lX11 to -lwayland-client in the build.sh 

To build the project you can use the provided `build.sh` or compile main.c and link it against raylib
