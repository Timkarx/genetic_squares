#include "raylib.h"
#include "sort.c"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
const int SQR_SIZE = 40;
const int GAP_SIZE = 5;
const int ARRAY_SIZE = 16;

const int CANVAS_WIDTH = (10 * SQR_SIZE) + (11 * GAP_SIZE);
const int rows = (ARRAY_SIZE + 9) / 10;
const int CANVAS_HEIGHT = rows * SQR_SIZE + (rows + 1) * GAP_SIZE;

//unsigned char* generate_candidates() {
//    const int ARRAY_SIZE = 15;
//    unsigned char squares[ARRAY_SIZE];
//
//    for (int i = 0; i < ARRAY_SIZE; i++) {
//       squares[i] = rand() % 256;
//    }
//    return squares;
//}

unsigned char* iterate_generation(unsigned char *shade, int size) {
    int replacementHalf = size / 2;
    for (int i = 0; i < size / 2; i++) {
        unsigned char child = *(shade + i);
        *(shade + replacementHalf + i) = child;
    }
    return shade;
}

void paint_circles(char *shade, int size) {
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
    unsigned char squares[ARRAY_SIZE];

    for (int i = 0; i < ARRAY_SIZE; i++) {
       squares[i] = rand() % 256;
    }

    printf("Generation 1: \n");
    printArray(squares, sizeof(squares));

    for (int i = 1; i < 10; i++) {
        quickSortFitness(squares, 0, sizeof(squares), 100);
        iterate_generation(squares, sizeof(squares));
        printf("Generation %d: \n", i);
        printArray(squares, sizeof(squares));
    }

    // dummy_squares is char[] so 1 byte per arr element
    // paint_circles(dummy_squares, sizeof(dummy_squares));
    
    return 0;
}
