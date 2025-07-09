#ifndef _GL_CONTEXT_H_
#define _GL_CONTEXT_H_

#include "../../default_libs/default_libs.h"
#include "../../glad/glad.h"
#include "../../cglm/include/cglm/cglm.h"
/* #define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image/stb_image.h" */
#include "../../GLFW/glfw3.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void setup_window(GLFWwindow** window, const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT, const char* WIN_TITLE);
void while_loop_window(GLFWwindow** window);
void end_window(GLFWwindow** window);
void end_process();
void print_yes();


#endif