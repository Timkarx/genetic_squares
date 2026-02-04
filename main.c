#include "raylib.h"
#include "sort.c"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const int SQR_SIZE = 10;
const int GAP_SIZE = 5;
const int ARRAY_SIZE = 300;

const int CANVAS_WIDTH = (10 * SQR_SIZE) + (11 * GAP_SIZE);
const int rows = (ARRAY_SIZE + 9) / 10;
const int CANVAS_HEIGHT = rows * SQR_SIZE + (rows + 1) * GAP_SIZE;
const int MUTATION_RATE = 1;

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
        if (rand() % 100 <= MUTATION_RATE) {
            child = rand() % 256;
        }
        *(shade + replacementHalf + i) = child;
    }
    return shade;
}

void paint_circles(unsigned char *shade, int size) {
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

int main() {
    srand(time(NULL));
    unsigned char squares[ARRAY_SIZE];

    for (int i = 0; i < ARRAY_SIZE; i++) {
       squares[i] = rand() % 256;
    }

    InitWindow(CANVAS_WIDTH, CANVAS_HEIGHT, "Genetic Squares");
    SetTargetFPS(1);

    while (!WindowShouldClose()) {
        paint_circles(squares, sizeof(squares));
        quickSortFitness(squares, 0, sizeof(squares), 0);
        iterate_generation(squares, sizeof(squares));
    }
    CloseWindow();

    // dummy_squares is char[] so 1 byte per arr element
    // paint_circles(dummy_squares, sizeof(dummy_squares));
    
    return 0;
}
