#ifndef _SHADERS_H_
#define _SHADERS_H_


#include "../../default_libs/default_libs.h"
#include "../../glad/glad.h"
#include "../../cglm/include/cglm/cglm.h"
#include "../../stb_image/stb_image.h"
#include "../../GLFW/glfw3.h"


char* get_shader_content(const char* fileName);
void setup_fvc_shader(unsigned int* shader, const char** shaderSource, const char* type);
void setup_fvc_shader_prog(unsigned int* program, unsigned int* fragShader, unsigned int* vertexShader, unsigned int* computeShader, const char* type);
void fvc_shader_comp_error(unsigned int* shader, const char* type);
void fvc_shader_prog_link_error(unsigned int* shaderProgram, const char* type);

#endif