#ifndef _HEIGHTMAP_H_
#define _HEIGHTMAP_H_

#include "../../default_libs/default_libs.h"
#include "../../glad/glad.h"
#include "../../cglm/include/cglm/cglm.h"
#include "../../stb_image/stb_image.h"
#include "../../GLFW/glfw3.h"

struct heightmap_s{
    
    float height_factor;
    
    int map_res_x;
    int map_res_y;
    
    float** grid;
    
};
typedef struct heightmap_s heightmap;


void heightmap_setup(heightmap* map, int res_x, int res_y);

void heightmap_black(heightmap* map);
void heightmap_white(heightmap* map);
void heightmap_from_function(void* ctx, heightmap* map, float (*f)(void*, int, int));
void heightmap_from_image(heightmap* map);

void print_heightmap(heightmap* m);
void free_heightmap(heightmap* map);


#endif