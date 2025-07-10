#ifndef _PRIMITIVES_H_
#define _PRIMITIVES_H_

#include "../../default_libs/default_libs.h"
#include "../../glad/glad.h"
#include "../../cglm/include/cglm/cglm.h"
/* #define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image/stb_image.h" */
#include "../../GLFW/glfw3.h"

extern float cube_vertices[24];

extern unsigned int cube_indices[36];

float* get_cube(unsigned int* indices, int* n_vertices, int* n_indices, float side_size);
void free_cube(float* vertices, float* indices);

#endif