#include "rkga.h"

#include <stdlib.h>
#include <string.h>

static float random_key(void) {
    return (float) rand() / (float) RAND_MAX;
}

static void randomize_keys(float *keys, size_t gene_count) {
    for (size_t i = 0; i < gene_count; i++) {
        keys[i] = random_key();
    }
}

int rkga_init_population(
    Population *population,
    size_t population_size,
    size_t gene_count,
    size_t elite_count,
    size_t mutant_count,
    float elite_bias
) {
    memset(population, 0, sizeof(*population));

    population->individuals = calloc(population_size, sizeof(Individual));
    if (population->individuals == NULL) {
        return 0;
    }

    population->population_size = population_size;
    population->gene_count = gene_count;
    population->elite_count = elite_count;
    population->mutant_count = mutant_count;
    population->elite_bias = elite_bias;

    for (size_t i = 0; i < population_size; i++) {
        population->individuals[i].keys = malloc(sizeof(float) * gene_count);
        population->individuals[i].solution = malloc(sizeof(unsigned char) * gene_count);

        if (population->individuals[i].keys == NULL || population->individuals[i].solution == NULL) {
            rkga_free_population(population);
            return 0;
        }
    }

    return 1;
}

void rkga_free_population(Population *population) {
    if (population->individuals == NULL) {
        return;
    }

    for (size_t i = 0; i < population->population_size; i++) {
        free(population->individuals[i].keys);
        free(population->individuals[i].solution);
    }

    free(population->individuals);
    memset(population, 0, sizeof(*population));
}

void rkga_seed_population(Population *population) {
    for (size_t i = 0; i < population->population_size; i++) {
        randomize_keys(population->individuals[i].keys, population->gene_count);
    }
}

void rkga_create_target(unsigned char *target, size_t gene_count, size_t column_count) {
    for (size_t i = 0; i < gene_count; i++) {
        size_t col = i % column_count;
        size_t row = i / column_count;

        int horizontal_gradient = (int) ((255.0f * (float) col) / (float) (column_count - 1));
        int stripe = (row % 6 < 3) ? 36 : -36;
        int checker = ((row + col) % 2 == 0) ? 18 : -18;
        int value = horizontal_gradient + stripe + checker;

        if (value < 0) {
            value = 0;
        }
        if (value > 255) {
            value = 255;
        }

        target[i] = (unsigned char) value;
    }
}

void rkga_decode_solution(Individual *individual, size_t gene_count) {
    for (size_t i = 0; i < gene_count; i++) {
        float key = individual->keys[i];
        int shade = (int) (key * 255.0f + 0.5f);

        if (shade < 0) {
            shade = 0;
        }
        if (shade > 255) {
            shade = 255;
        }

        individual->solution[i] = (unsigned char) shade;
    }
}

int rkga_evaluate_individual(Individual *individual, const unsigned char *target, size_t gene_count) {
    int total_error = 0;

    rkga_decode_solution(individual, gene_count);

    for (size_t i = 0; i < gene_count; i++) {
        int diff = (int) target[i] - (int) individual->solution[i];
        if (diff < 0) {
            diff = -diff;
        }
        total_error += diff;
    }

    individual->fitness = total_error;
    return total_error;
}

static int compare_individuals(const void *left, const void *right) {
    const Individual *a = left;
    const Individual *b = right;
    return a->fitness - b->fitness;
}

void rkga_sort_population(Population *population) {
    qsort(population->individuals, population->population_size, sizeof(Individual), compare_individuals);
}

void rkga_evaluate_population(Population *population, const unsigned char *target) {
    for (size_t i = 0; i < population->population_size; i++) {
        rkga_evaluate_individual(&population->individuals[i], target, population->gene_count);
    }

    rkga_sort_population(population);
}

static size_t random_index(size_t upper_bound) {
    return (size_t) (rand() % (int) upper_bound);
}

static void copy_keys(float *destination, const float *source, size_t gene_count) {
    memcpy(destination, source, sizeof(float) * gene_count);
}

void rkga_next_generation(Population *population, const unsigned char *target) {
    size_t population_size = population->population_size;
    size_t gene_count = population->gene_count;
    size_t elite_count = population->elite_count;
    size_t mutant_count = population->mutant_count;
    size_t offspring_start = elite_count;
    size_t offspring_end = population_size - mutant_count;
    float *next_keys = malloc(sizeof(float) * population_size * gene_count);

    rkga_evaluate_population(population, target);

    if (next_keys == NULL) {
        return;
    }

    for (size_t i = 0; i < elite_count; i++) {
        copy_keys(&next_keys[i * gene_count], population->individuals[i].keys, gene_count);
    }

    for (size_t i = offspring_start; i < offspring_end; i++) {
        size_t elite_parent = random_index(elite_count);
        size_t non_elite_parent = elite_count + random_index(population_size - elite_count);
        float *child_keys = &next_keys[i * gene_count];
        Individual *elite = &population->individuals[elite_parent];
        Individual *non_elite = &population->individuals[non_elite_parent];

        for (size_t gene = 0; gene < gene_count; gene++) {
            if (random_key() < population->elite_bias) {
                child_keys[gene] = elite->keys[gene];
            } else {
                child_keys[gene] = non_elite->keys[gene];
            }
        }
    }

    for (size_t i = offspring_end; i < population_size; i++) {
        randomize_keys(&next_keys[i * gene_count], gene_count);
    }

    for (size_t i = 0; i < population_size; i++) {
        copy_keys(population->individuals[i].keys, &next_keys[i * gene_count], gene_count);
    }

    free(next_keys);
    population->generation++;
    rkga_evaluate_population(population, target);
}
