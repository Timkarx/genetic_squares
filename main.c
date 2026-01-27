#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    const int SQR_SIZE = 40;
    const int NUM_OF_SQRS = 10;
    const int GAP_SIZE = 5;

    const int CANVAS_WIDTH = (NUM_OF_SQRS * SQR_SIZE) + (NUM_OF_SQRS * GAP_SIZE) + GAP_SIZE;

    InitWindow(CANVAS_WIDTH, 400, "raygui - controls test suite");
    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(RAYWHITE);
            int i;
            for (i = 0; i <= NUM_OF_SQRS; i++) {
                int x_pos = i * SQR_SIZE + (i + 1) *GAP_SIZE;
                DrawRectangle(x_pos, 20, SQR_SIZE, SQR_SIZE, RED);
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
