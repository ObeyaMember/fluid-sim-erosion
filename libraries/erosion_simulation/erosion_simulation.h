#ifndef _EROSION_SIMULATION_H_
#define _EROSION_SIMULATION_H_

#include "..//default_libs/default_libs.h"
#include "..//glad/glad.h"
#include "..//cglm/include/cglm/cglm.h"
/* #define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image/stb_image.h" */
#include "..//GLFW/glfw3.h"
#include "../utilities/utilities.h"
#include "../fluid_simulation/fluid_simulation.h"



struct erosion_sim_parameters_s{
    int sim_id;

    vec3 map_pos;

    
    
    fluid_sim_parameters* fluid_sim_params;


};
typedef struct erosion_sim_parameters_s erosion_sim_params;

#endif