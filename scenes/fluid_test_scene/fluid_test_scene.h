#ifndef _FLUID_TEST_SCENE_H_
#define _FLUID_TEST_SCENE_H_

#include "../../libraries/utilities/utilities.h"
//#include "../../libraries/utilities/gl_context/gl_context.h"
//#include "../../libraries/utilities/data/data.h"
//#include "../../libraries/utilities/shaders/shaders.h"
#include "../../libraries/default_libs/default_libs.h"
#include "../../libraries/glad/glad.h"
#include "../../libraries/cglm/include/cglm/cglm.h"
/* #define STB_IMAGE_IMPLEMENTATION
#include "../../libraries/stb_image/stb_image.h" */
#include "../../libraries/GLFW/glfw3.h"

#include "../../libraries/fluid_simulation/fluid_simulation.h"

void fluid_test_scene_setup(GLFWwindow* window);
void fluid_test_scene_main_loop(GLFWwindow* window);
void fluid_test_scene_end(GLFWwindow* window);

#endif