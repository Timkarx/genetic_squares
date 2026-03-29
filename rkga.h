#ifndef RKGA_H
#define RKGA_H

#include <stddef.h>

typedef struct {
    float *keys;
    unsigned char *solution;
    int fitness;
} Individual;

typedef struct {
    Individual *individuals;
    size_t population_size;
    size_t gene_count;
    size_t elite_count;
    size_t mutant_count;
    float elite_bias;
    int generation;
} Population;

int rkga_init_population(
    Population *population,
    size_t population_size,
    size_t gene_count,
    size_t elite_count,
    size_t mutant_count,
    float elite_bias
);

void rkga_free_population(Population *population);
void rkga_seed_population(Population *population);
void rkga_create_target(unsigned char *target, size_t gene_count, size_t column_count);
void rkga_decode_solution(Individual *individual, size_t gene_count);
int rkga_evaluate_individual(Individual *individual, const unsigned char *target, size_t gene_count);
void rkga_sort_population(Population *population);
void rkga_evaluate_population(Population *population, const unsigned char *target);
void rkga_next_generation(Population *population, const unsigned char *target);

#endif
