#ifndef _MESH_H_
#define _MESH_H_

#include "../../default_libs/default_libs.h"
#include "../../glad/glad.h"
#include "../../cglm/include/cglm/cglm.h"
#include "../../stb_image/stb_image.h"
#include "../../GLFW/glfw3.h"

typedef struct heightmap_s heightmap;
typedef struct terrain_s terrain;

struct mesh_s{
    
    vec3 pos;
    vec3 dims;
    
    int mesh_n_vertices;
    float* mesh_vertices;

    int mesh_n_indices;
    unsigned int* mesh_indices;

    // to_implement: rendering options

};
typedef struct mesh_s mesh;

// still needs to  malloc the arrays outside of this function
mesh new_mesh_from_heightmap(heightmap* hmap, vec3 pos, vec3 dims);
mesh new_mesh_from_terrain(terrain* t);

void update_mesh_from_terrain(mesh* m, terrain* t, int n_vertices, int* vertices_to_modify_indices, int* modify_from_indices);
void print_mesh_vertices(mesh* m);
void print_mesh_indices(mesh* m);
void print_mesh_indices_normalized(mesh* m);
void free_mesh(mesh* s);

#endif