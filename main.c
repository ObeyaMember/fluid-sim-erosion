#include "libraries/utilities/utilities.h"
#include "libraries/default_libs/default_libs.h"
#include "libraries/glad/glad.h"
#include "libraries/cglm/include/cglm/cglm.h"
#include "libraries/stb_image/stb_image.h"
#include "libraries/GLFW/glfw3.h"


#define GLFW_INCLUDE_NONE // why idk
#define GLM_SWIZZLE // why idk



int main(){
    GLFWwindow* beloved_window;

    setup_window(&beloved_window, 500, 500, "Most  Loved  Window");
    
    while (!glfwWindowShouldClose(beloved_window)){



        while_loop_window(&beloved_window);
    }
    end_window(&beloved_window);
    end_process();
}
