#include "gl_context.h"


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){

    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
}

void setup_window(GLFWwindow** window, const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT, const char* WIN_TITLE){



    glfwSetErrorCallback(error_callback);

    assert(glfwInit());
    
    *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WIN_TITLE, NULL, NULL);
    

    if (!*window){
        printf("%d", -1);  
        glfwTerminate();
    }

    glfwSetKeyCallback(*window, key_callback);
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
    gladLoadGL();

}

void while_loop_window(GLFWwindow** window){
    glfwSwapBuffers(*window);
    glfwPollEvents();
}

void end_window(GLFWwindow** window){
    glfwDestroyWindow(*window);
}

void end_process(){
    glfwTerminate();
}
