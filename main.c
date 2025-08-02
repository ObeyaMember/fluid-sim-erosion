#include "libraries/utilities/utilities.h"
#include "libraries/default_libs/default_libs.h"
#include "libraries/glad/glad.h"
#include "libraries/cglm/include/cglm/cglm.h"
#include "libraries/GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "libraries/stb_image/stb_image.h"
#include "scenes/default_scene/default_scene.h"
#include "scenes/fluid_test_scene/fluid_test_scene.h"


#define GLFW_INCLUDE_NONE // why idk
#define GLM_SWIZZLE // why idk




int main(){

    
    //print_yes_shaders();
    //print_lmao();


    // random testing ---------

    /* int grid[10] = {0,1,2,3,4,5,6,7,8,9};
                            // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 
    int particles_cells[10] = {1, 2, 2, 3, 5, 1, 2, 10, 4, 5};
    radix_sort_map(grid, particles_cells, 10);
    print_int_array(grid, 10);
 */
    // ----------

    GLFWwindow* beloved_window = setup_window(800, 800, "Most  Loved  Window");

    
    //default_scene_setup(beloved_window);
    fluid_test_scene_setup(beloved_window);
    
    while (!glfwWindowShouldClose(beloved_window)){

        //default_scene_main_loop(beloved_window);
        fluid_test_scene_main_loop(beloved_window);
    

        while_loop_window(beloved_window);
    }

    //default_scene_end(beloved_window);
    fluid_test_scene_end(beloved_window);

    end_window(beloved_window);
    end_process();
}