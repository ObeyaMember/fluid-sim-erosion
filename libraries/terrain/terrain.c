#include "terrain.h"

terrain new_terrain_from_heightmap(heightmap* h_map, vec3 pos, vec3 dims){
    terrain t = {
        .h_map = h_map,
    };

    glm_vec3_copy(pos, t.pos);
    glm_vec3_copy(dims, t.dims);

    return t;
}

void free_terrain(terrain* t){
    free_heightmap(t->h_map);
}