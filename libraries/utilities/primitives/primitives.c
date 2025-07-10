#include "primitives.h"

float* get_cube(unsigned int* indices, int* n_vertices, int* n_indices, float side_size){
    *n_vertices = 6;
    *n_indices = 36;
    float* vertices = malloc(*n_vertices*3*sizeof(float));
    for (int i = 0; i < *n_vertices*3; i += 1){
        vertices[i] = cube_vertices[i]*side_size/2.0;
    }

    indices = malloc(*n_indices*sizeof(unsigned int));
    for (int i = 0; i < *n_indices; i += 1){
        indices[i] = cube_indices[i];
    }

}

void free_cube(float* vertices, float* indices){
    free(vertices);
    free(indices);
}

