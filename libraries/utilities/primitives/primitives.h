#ifndef _PRIMITIVES_H_
#define _PRIMITIVES_H_

#include "../../default_libs/default_libs.h"
#include "../../glad/glad.h"
#include "../../cglm/include/cglm/cglm.h"
/* #define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image/stb_image.h" */
#include "../../GLFW/glfw3.h"

int get_prim_plane_n_vertices();
int get_prim_plane_n_indices();
void get_prim_plane(float* vertices, unsigned int* indices,float side_size);
void get_prim_plane_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords, float side_size);
void free_prim_plane(float* vertices, unsigned int* indices);
void free_prim_plane_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords);

int get_prim_cube_n_vertices();
int get_prim_cube_n_indices();
void get_prim_cube(float* vertices, unsigned int* indices, float side_size);
void get_prim_cube_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords, float side_size);
void free_prim_cube(float* vertices, unsigned int* indices);
void free_prim_cube_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords);

int get_prim_rectangle_n_vertices();
int get_prim_rectangle_n_indices();
void get_prim_rectangle(float* vertices, unsigned int* indices, float length, float width, float height);
void get_prim_rectangle_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords, float length, float width, float height);
void free_prim_rectangle(float* vertices, unsigned int* indices);
void free_prim_rectangle_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords);

#endif