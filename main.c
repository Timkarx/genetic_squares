#include "raylib.h"
#include "rkga.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum {
    GRID_COLUMNS = 10,
    SOLUTION_SIZE = 300,
    POPULATION_SIZE = 60,
    SQUARE_SIZE = 16,
    GAP_SIZE = 4,
    PANEL_PADDING = 24,
    ELITE_COUNT = 12,
    MUTANT_COUNT = 9
};

static const float ELITE_BIAS = 0.70f;

static int grid_rows(void) {
    return (SOLUTION_SIZE + GRID_COLUMNS - 1) / GRID_COLUMNS;
}

static int grid_width(void) {
    return GRID_COLUMNS * SQUARE_SIZE + (GRID_COLUMNS - 1) * GAP_SIZE;
}

static int grid_height(void) {
    int rows = grid_rows();
    return rows * SQUARE_SIZE + (rows - 1) * GAP_SIZE;
}

static void draw_solution_grid(const unsigned char *solution, int origin_x, int origin_y, const char *title) {
    DrawText(title, origin_x, origin_y - 28, 20, DARKGRAY);

    for (int i = 0; i < SOLUTION_SIZE; i++) {
        int col = i % GRID_COLUMNS;
        int row = i / GRID_COLUMNS;
        int x = origin_x + col * (SQUARE_SIZE + GAP_SIZE);
        int y = origin_y + row * (SQUARE_SIZE + GAP_SIZE);
        unsigned char shade = solution[i];
        Color color = {shade, shade, shade, 255};

        DrawRectangle(x, y, SQUARE_SIZE, SQUARE_SIZE, color);
    }
}

static void draw_key_preview(const Individual *individual, int origin_x, int origin_y) {
    const int preview_count = 8;
    char label[128];

    DrawText("First 8 random keys", origin_x, origin_y, 20, DARKGRAY);

    for (int i = 0; i < preview_count; i++) {
        int bar_y = origin_y + 34 + (i * 26);
        int bar_width = 160;
        int fill_width = (int) (individual->keys[i] * (float) bar_width);

        DrawRectangle(origin_x, bar_y, bar_width, 16, LIGHTGRAY);
        DrawRectangle(origin_x, bar_y, fill_width, 16, DARKBLUE);
        snprintf(label, sizeof(label), "key[%d] = %.3f -> shade %3u", i, individual->keys[i], individual->solution[i]);
        DrawText(label, origin_x + bar_width + 12, bar_y - 2, 18, DARKGRAY);
    }
}

int main(void) {
    const int canvas_width = PANEL_PADDING * 3 + grid_width() * 2;
    const int canvas_height = PANEL_PADDING * 2 + grid_height() + 260;

    unsigned char target[SOLUTION_SIZE];
    Population population;

    srand((unsigned int) time(NULL));

    if (!rkga_init_population(
            &population,
            POPULATION_SIZE,
            SOLUTION_SIZE,
            ELITE_COUNT,
            MUTANT_COUNT,
            ELITE_BIAS
        )) {
        fprintf(stderr, "Failed to allocate RKGA population\n");
        return 1;
    }

    rkga_create_target(target, SOLUTION_SIZE, GRID_COLUMNS);
    rkga_seed_population(&population);
    rkga_evaluate_population(&population, target);

    InitWindow(canvas_width, canvas_height, "RKGA Robot Painter");
    SetTargetFPS(12);

    while (!WindowShouldClose()) {
        const Individual *best = &population.individuals[0];
        char generation_text[64];
        char fitness_text[64];

        rkga_next_generation(&population, target);

        best = &population.individuals[0];
        snprintf(generation_text, sizeof(generation_text), "Generation: %d", population.generation);
        snprintf(fitness_text, sizeof(fitness_text), "Best fitness: %d (lower is better)", best->fitness);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("RKGA educational demo: keys are evolved, solutions are decoded.", PANEL_PADDING, 18, 24, BLACK);
        DrawText(generation_text, PANEL_PADDING, 54, 22, DARKGRAY);
        DrawText(fitness_text, PANEL_PADDING + 220, 54, 22, DARKGRAY);

        draw_solution_grid(target, PANEL_PADDING, 100, "Target pattern");
        draw_solution_grid(best->solution, PANEL_PADDING * 2 + grid_width(), 100, "Best decoded solution");
        draw_key_preview(best, PANEL_PADDING, 100 + grid_height() + 40);

        DrawText("Decoder: shade = round(key * 255)", PANEL_PADDING, canvas_height - 78, 20, BLACK);
        DrawText("Evolution: elites survive, offspring use biased crossover, mutants inject fresh random keys.", PANEL_PADDING, canvas_height - 46, 20, BLACK);

        EndDrawing();
    }

    rkga_free_population(&population);
    CloseWindow();

    return 0;
}
