#include "terrain.h"

terrain new_terrain_from_heightmap(heightmap* h_map, vec3 pos, vec3 dims){
    terrain t = {
        .h_map = h_map,
    };

    glm_vec3_copy(pos, t.pos);
    glm_vec3_copy(dims, t.dims);

    return t;
}

void print_terrain_data(terrain* t){
    if (t == NULL){
        printf("terrain -  NULL\n");
    }else{
        printf("terrain -\n");
        printf("pos: (%f; %f, %f) - ", t->pos[0], t->pos[1],t->pos[2]);
        printf("dims: (%f; %f, %f) - \n", t->dims[0],t->dims[1],t->dims[2]);
        print_heightmap(t->h_map);
    }
    
}

void free_terrain(terrain* t){
    free_heightmap(t->h_map);
}