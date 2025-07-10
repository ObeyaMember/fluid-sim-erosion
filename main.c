#include "libraries/utilities/utilities.h"
#include "libraries/default_libs/default_libs.h"
#include "libraries/glad/glad.h"
#include "libraries/cglm/include/cglm/cglm.h"
#include "libraries/GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "libraries/stb_image/stb_image.h"
#include "scenes/default_scene/default_scene.h"


#define GLFW_INCLUDE_NONE // why idk
#define GLM_SWIZZLE // why idk




int main(){

    
    //print_yes_shaders();
    //print_lmao();

    GLFWwindow* beloved_window = setup_window(800, 800, "Most  Loved  Window");

    
    default_scene_setup(beloved_window);
    
    while (!glfwWindowShouldClose(beloved_window)){

        default_scene_main_loop(beloved_window);

        while_loop_window(beloved_window);
    }
    end_window(beloved_window);
    end_process();
}