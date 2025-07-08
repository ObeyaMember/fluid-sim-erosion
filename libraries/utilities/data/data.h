#ifndef _DATA_H_
#define _DATA_H_

#include "../../default_libs/default_libs.h"
#include "../../glad/glad.h"
#include "../../cglm/include/cglm/cglm.h"
#include "../../stb_image/stb_image.h"
#include "../../GLFW/glfw3.h"

void setup_texture_from_jpg(unsigned int* texture, const char* file_name, unsigned int* program, const char* tex_unit_name, GLint v0);
void setup_texture_from_png(unsigned int* texture, const char* file_name, unsigned int* program, const char* tex_unit_name, GLint v0);
void print_vertex_array(float* a, int n_vertices);

#endif