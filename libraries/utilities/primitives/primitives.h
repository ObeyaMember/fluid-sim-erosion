#ifndef _PRIMITIVES_H_
#define _PRIMITIVES_H_

#include "../../default_libs/default_libs.h"
#include "../../glad/glad.h"
#include "../../cglm/include/cglm/cglm.h"
/* #define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image/stb_image.h" */
#include "../../GLFW/glfw3.h"

extern float prim_plane_vertices[12];
extern unsigned int prim_plane_indices[6];
extern float prim_plane_tex_coords[8];

float* get_prim_plane(unsigned int* indices, int* n_vertices, int* n_indices, float side_size);
void get_prim_plane_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords, int* n_vertices, int* n_indices, float side_size);
void free_prim_plane(float* vertices, unsigned int* indices);
void free_prim_plane_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords);

extern float prim_cube_vertices[24];
extern unsigned int prim_cube_indices[36];
extern float prim_cube_tex_coords[16];

float* get_prim_cube(unsigned int* indices, int* n_vertices, int* n_indices, float side_size);
void get_prim_cube_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords, int* n_vertices, int* n_indices, float side_size);
void free_prim_cube(float* vertices, unsigned int* indices);
void free_prim_cube_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords);

#endif