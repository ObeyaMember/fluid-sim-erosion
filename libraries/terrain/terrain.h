#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "..//default_libs/default_libs.h"
#include "..//glad/glad.h"
#include "..//cglm/include/cglm/cglm.h"
/* #define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image/stb_image.h" */
#include "..//GLFW/glfw3.h"
#include "../utilities/utilities.h"
#include "../fluid_simulation/fluid_simulation.h"


struct terrain_s {
    
    vec3 pos;
    vec3 dims;
    //vec3 scale;

    heightmap* h_map;  

    //to_implement: int*** terrain_type_grid;

};
typedef struct terrain_s terrain;

terrain new_terrain_from_heightmap(heightmap* h_map, vec3 pos, vec3 dims);

void print_terrain_data(terrain* t);

void free_terrain(terrain* t);
#endif