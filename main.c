#include "raylib/src/raylib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SCREENSHOT_MODE 0
#define ZOOM_MODE 1

#define LENS_SIZE 100.0f
#define LENS_ZOOM 2.0f

int main(int argsc, char* argsv[]) {

    SetTraceLogLevel(LOG_NONE);

    int mode = SCREENSHOT_MODE;

    if (argsc > 1 && strcmp(argsv[1], "-s") == 0) {
        mode = SCREENSHOT_MODE;
    }
    else if (argsc > 1 && strcmp(argsv[1], "-z") == 0) {
        mode = ZOOM_MODE;
    }

    // init
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    InitWindow(screenW, screenH, "freeze");
    SetTargetFPS(60);

    // read image from stdin
    unsigned char buffer[1];

    size_t capacity = 8192;
    size_t total = 0;

    unsigned char *data = malloc(capacity);
    size_t n;

    while ((n = fread(buffer, 1, sizeof(buffer), stdin)) > 0) {

        if (total + n > capacity) {

            while (total + n > capacity)
                capacity += 8192;

            data = realloc(data, capacity);

        }

        memcpy(data + total, buffer, n);
        total += n;
    }

    Image img = LoadImageFromMemory(".png", data, total);

    Texture2D tex = LoadTextureFromImage(img);

    if (tex.id == 0) {
        printf("Error!!\n");
    }

    //interface    

    Vector2 start = {0};
    Vector2 end = {0};

    float zoom = 1.0f;
    Vector2 offset = {0};

    int dragging = 0;

    while (!WindowShouldClose()) {

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            start = GetMousePosition();
            end = start;
            dragging = 1;
        }

        if (dragging) {
            end = GetMousePosition();
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            dragging = 0;
        }

        Rectangle rect = {
            fmin(start.x, end.x),
            fmin(start.y, end.y),
            fabs(end.x - start.x),
            fabs(end.y - start.y)
        };

        if (mode == ZOOM_MODE && dragging) {

            Vector2 delta = GetMouseDelta();
            offset.x += delta.x;
            offset.y += delta.y;

        }

        float wheel = GetMouseWheelMove();


        Vector2 pos = GetMousePosition();

        if (wheel != 0 && mode == ZOOM_MODE) {
            float oldZoom = zoom;
            float newZoom = zoom * (wheel > 0 ? 1.1f : 0.9f);


            Vector2 worldBefore = {
                (pos.x - offset.x) / oldZoom,
                (pos.y - offset.y) / oldZoom
            };

            zoom = newZoom;

            Vector2 worldAfter = {
                (pos.x - offset.x) / zoom,
                (pos.y - offset.y) / zoom
            };

            offset.x += (worldAfter.x - worldBefore.x) * zoom;
            offset.y += (worldAfter.y - worldBefore.y) * zoom;
        }

        BeginDrawing();

        ClearBackground(BLACK);

        DrawTextureEx(tex, offset, 0.0f, zoom, WHITE);

        if (mode == ZOOM_MODE) {

            if (IsKeyDown(KEY_LEFT_CONTROL)) {

                float radius = LENS_SIZE;
                float zoomFactor = LENS_ZOOM * zoom;

                Vector2 worldMouse = {
                    (pos.x - offset.x) / zoom,
                    (pos.y - offset.y) / zoom
                };
                                
                Rectangle src = {
                    worldMouse.x - radius / zoomFactor,
                    worldMouse.y - radius / zoomFactor,
                    radius * 2 / zoomFactor,
                    radius * 2 / zoomFactor
                };

                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0,0,0,180});

                DrawTexturePro(
                    tex,
                    src,
                    (Rectangle){pos.x - radius, pos.y - radius, radius * 2, radius * 2},
                    (Vector2){0,0},
                    0.0f,
                    WHITE
                );

                //DrawTextureEx(tex, offset, 0.0f, zoom, WHITE);
            }

        }

        if (mode == SCREENSHOT_MODE) {

            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 180});

            DrawTextureRec(
                tex,
                rect,
                (Vector2){rect.x, rect.y},
                WHITE
            );

            DrawRectangleLinesEx(rect, 2, RED);

        }

        EndDrawing();

        if (mode == SCREENSHOT_MODE) {
            if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_C)) {
                Rectangle crop = rect;

                // clamp to image bounds (VERY important)
                if (crop.x < 0) crop.x = 0;
                if (crop.y < 0) crop.y = 0;
                if (crop.x + crop.width > img.width) crop.width = img.width - crop.x;
                if (crop.y + crop.height > img.height) crop.height = img.height - crop.y;

                Image shot = ImageFromImage(img, crop);
                
                int size = 0;
                unsigned char *png = ExportImageToMemory(shot, ".png", &size);
            
                fwrite(png, 1, size, stdout);
                fflush(stdout);
            
                UnloadImage(shot);

                CloseWindow();
                exit(0);
            }
        }

    }

    UnloadTexture(tex);
    CloseWindow();

    return 0;
}
