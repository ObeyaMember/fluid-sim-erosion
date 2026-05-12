#include "mesh.h"
#include "../heightmap/heightmap.h"
#include "../../terrain/terrain.h"

mesh new_mesh_from_heightmap(heightmap* hmap, vec3 pos, vec3 dims){
    int res_x = hmap->map_res_x;
    int res_y = hmap->map_res_y;
    mesh s = {
    
        .mesh_n_vertices = res_x * res_y,    
        .mesh_n_indices = (res_x - 1) * (res_y - 1) * 6
   
    };
    s.mesh_vertices = malloc(sizeof(float)*s.mesh_n_vertices * 3);
    s.mesh_indices = malloc(sizeof(int)*s.mesh_n_indices);

    glm_vec3_copy(pos, s.pos);
    glm_vec3_copy(dims, s.dims);
    
    float vertex_sep_x = 2.0 / ((float)res_x -1);
    float vertex_sep_y = 2.0 / ((float)res_y -1);
    

    for (int i = 0; i < res_x; i += 1){
        for (int j = 0; j < res_y; j += 1){
            s.mesh_vertices[(i*res_x + j)*3] = vertex_sep_x * i - 1.0;
            //s.mesh_vertices[(i*res_x + j)*3 + 1] = hmap->grid[i][j]; // * hmap->height_factor;
            //s.mesh_vertices[(i*res_x + j)*3 + 2] = vertex_sep_y * j - 1.0;

            if (i < res_x - 1 && j < res_y - 1){
                //s.mesh_indices[(i*res_x + j)*6 + 0] = (i+0)*res_x + j + 0;
                //s.mesh_indices[(i*res_x + j)*6 + 1] = (i+1)*res_x + j + 1;
                //s.mesh_indices[(i*res_x + j)*6 + 2] = (i+0)*res_x + j + 1;
                //s.mesh_indices[(i*res_x + j)*6 + 3] = (i+0)*res_x + j + 0;
                //s.mesh_indices[(i*res_x + j)*6 + 4] = (i+1)*res_x + j + 1;
                //s.mesh_indices[(i*res_x + j)*6 + 5] = (i+1)*res_x + j + 0;
                //printf("i am an index\n");
                
                //s.mesh_indices[(i*res_x + j)*6 + 0] = ((i+0)*res_x + (j+0))*3;
                //s.mesh_indices[(i*res_x + j)*6 + 1] = ((i+0)*res_x + (j+1))*3;
                //s.mesh_indices[(i*res_x + j)*6 + 2] = ((i+1)*res_x + (j+1))*3;
                //s.mesh_indices[(i*res_x + j)*6 + 3] = ((i+0)*res_x + (j+0))*3;
                //s.mesh_indices[(i*res_x + j)*6 + 4] = ((i+1)*res_x + (j+0))*3;
                //s.mesh_indices[(i*res_x + j)*6 + 5] = ((i+1)*res_x + (j+1))*3;
                printf("(i, j): %d, %d \n", i, j);
                printf("(i*res_x + j): %d \n", i*(res_x-1) + j);
                
                //s.mesh_indices[(i*(res_x-1) + j)*6 + 0] = 1;
                //s.mesh_indices[(i*(res_x-1) + j)*6 + 1] = 1;
                //s.mesh_indices[(i*(res_x-1) + j)*6 + 2] = 1;
                //s.mesh_indices[(i*(res_x-1) + j)*6 + 3] = 1;
                //s.mesh_indices[(i*(res_x-1) + j)*6 + 4] = 1;
                //s.mesh_indices[(i*(res_x-1) + j)*6 + 5] = 1;


            }
        }
    }

    return s;
}

void print_mesh_vertices(mesh* m){
    for (int i = 0; i < m->mesh_n_vertices; i += 1){
        printf("%d - (%f, %f, %f)\n", i, m->mesh_vertices[i*3], m->mesh_vertices[i*3+1], m->mesh_vertices[i*3+2]);
    }
    //for (int i = 0; i < m->mesh_n_vertices*3; i += 1){
    //    printf("%f \n", m->mesh_vertices[i]);
    //}
}

void print_mesh_indices(mesh* m){
    for (int i = 0; i < m->mesh_n_indices; i += 1){
        printf("%d - %d\n", i, m->mesh_indices[i]);
        
    }
}

void print_mesh_indices_normalized(mesh* m){ // divided by 3
    for (int i = 0; i < m->mesh_n_indices; i += 1){
        printf("%d - %d\n", i, m->mesh_indices[i]/ 3);
        
    }
}

mesh new_mesh_from_terrain(terrain* t){
    mesh s = new_mesh_from_heightmap(t->h_map, t->pos, t->dims);
    return s;
}

void free_mesh(mesh* s){
    free(s->mesh_indices);
    free(s->mesh_vertices);
}

