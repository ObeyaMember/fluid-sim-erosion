#ifndef _GENERAL_PURPOSE_H_
#define _GENERAL_PURPOSE_H_

#include "../../default_libs/default_libs.h"
#include "../../glad/glad.h"
#include "../../cglm/include/cglm/cglm.h"
/* #define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image/stb_image.h" */
#include "../../GLFW/glfw3.h"


#endif

void print_int_array(int* array, int array_size);
int get_max_int_array(int* array, int array_size);
void radix_sort(int* input, int input_size);
void radix_sort_max_known(int* input, int input_size, int max);
void radix_sort_map(int* input, int* map_input, int input_size);
void radix_sort_map_max_known(int* input, int* map_input, int input_size, int max);
