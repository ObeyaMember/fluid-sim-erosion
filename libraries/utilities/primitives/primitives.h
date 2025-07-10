#ifndef _PRIMITIVES_H_
#define _PRIMITIVES_H_

#include "../../default_libs/default_libs.h"
#include "../../glad/glad.h"
#include "../../cglm/include/cglm/cglm.h"
/* #define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image/stb_image.h" */
#include "../../GLFW/glfw3.h"

float cube_vertices[24] = {
1,1,1,   1,1,-1,   1,-1,-1,   1,-1,1,
-1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1  
};

unsigned int cube_indices[36] = {
    0,1,3, 1,2,3,
    4,0,7, 0,3,7,
    5,4,6, 4,7,6,
    1,5,2, 5,6,2,
    2,3,6, 3,7,6,
    1,0,5, 0,4,5
};

float* get_cube(unsigned int* indices, int* n_vertices, int* n_indices, float side_size);
void free_cube(float* vertices, float* indices);

#endif