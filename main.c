#include "raylib.h"
#include "sort.c"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void paint_circles(char *shade, int size) {
    const int SQR_SIZE = 40;
    const int GAP_SIZE = 5;

    const int CANVAS_WIDTH = (10 * SQR_SIZE) + (11 * GAP_SIZE);
    const int rows = (size + 9) / 10;
    const int CANVAS_HEIGHT = rows * SQR_SIZE + (rows + 1) * GAP_SIZE;

    InitWindow(CANVAS_WIDTH, CANVAS_HEIGHT, "raygui - controls test suite");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        int i;
        int x_pos = GAP_SIZE;
        int y_pos = GAP_SIZE;

        for (i = 0; i <= size; i++) {
            int col = i % 10;
            int row = i / 10;
            
            x_pos = GAP_SIZE + col * (SQR_SIZE + GAP_SIZE); 
            y_pos = GAP_SIZE + row * (SQR_SIZE + GAP_SIZE); 
            char square_shade = *(shade + i);
            struct Color color = { 0, 0, 0, square_shade };

            DrawRectangle(x_pos, y_pos, SQR_SIZE, SQR_SIZE, color);
        }

        EndDrawing();
    }
    CloseWindow();
}

int main() {
    srand(time(NULL));
    const int DUMMY_ARRAY_SIZE = 15;
    int i;
    unsigned char dummy_squares[DUMMY_ARRAY_SIZE];

    for (i = 0; i < DUMMY_ARRAY_SIZE; i++) {
       dummy_squares[i] = rand() % 256;
    }

    printArray(dummy_squares, sizeof(dummy_squares));
    quickSort(dummy_squares, 0, sizeof(dummy_squares) - 1);
    printf("Sorted array in ascending order: \n");
    printArray(dummy_squares, sizeof(dummy_squares));

    // dummy_squares is char[] so 1 byte per arr element
    // paint_circles(dummy_squares, sizeof(dummy_squares));
    
    return 0;
}
