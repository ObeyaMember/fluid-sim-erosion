#include "primitives.h"

float prim_plane_vertices[12] = {
    // positions      // texture coords
    1.0f, 1.0f, 0.0,  // top right
    1.0f, -1.0f, 0.0, // bottom right
    -1.0f, -1.0f, 0.0,// bottom left
    -1.0f, 1.0f, 0.0  // top left
};
unsigned int prim_plane_indices[6] = {
    0, 1, 3, // first triangle
    1, 2, 3 // second triangle
};
float prim_plane_tex_coords[8] = {
    1.0, 1.0,
    1.0, 0.0,
    0.0, 0.0,
    0.0, 1.0
};

float* get_prim_plane(unsigned int* indices, int* n_vertices, int* n_indices, float side_size){
    *n_vertices = 4;
    *n_indices = 6;
    float* vertices = malloc(*n_vertices*3*sizeof(float));
    for (int i = 0; i < *n_vertices*3; i += 1){
        vertices[i] = prim_plane_vertices[i]*side_size/2.0;
    }

    indices = malloc(*n_indices*sizeof(unsigned int));
    for (int i = 0; i < *n_indices; i += 1){
        indices[i] = prim_plane_indices[i];
    }

    return vertices;
};

void get_prim_plane_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords, int* n_vertices, int* n_indices, float side_size){
    *n_vertices = 4;
    *n_indices = 6;
    /* *vertices = malloc((*n_vertices)*3*sizeof(float));
    //printf("PRIMITIVE PLANE VERTICES: \n");
    for (int i = 0; i < *n_vertices*3; i += 1){
        *vertices[i] = prim_plane_vertices[i]*side_size/2.0;
    }

    *indices = malloc((*n_indices)*sizeof(unsigned int));
    //printf("PRIMITIVE PLANE INDICES: \n");
    for (int i = 0; i < *n_indices; i += 1){
        *indices[i] = prim_plane_indices[i];
        //printf("%d\n", indices[i]);
    }

    *tex_coords = malloc((*n_vertices)*2*sizeof(float));
    //printf("PRIMITIVE PLANE TEX COORDS: \n");
    for (int i = 0; i < *n_vertices*2; i += 1){
        *tex_coords[i] = prim_plane_tex_coords[i];
        if (i % 2 == 0){
            //printf("%f %f\n", prim_plane_tex_coords[i], prim_plane_tex_coords[i+1]);
        }
        
    } */

    
    //printf("PRIMITIVE PLANE VERTICES: \n");
    for (int i = 0; i < *n_vertices*3; i += 1){
        vertices[i] = prim_plane_vertices[i]*side_size/2.0;
    }

    
    //printf("PRIMITIVE PLANE INDICES: \n");
    for (int i = 0; i < *n_indices; i += 1){
        indices[i] = prim_plane_indices[i];
        //printf("%d\n", indices[i]);
    }

    
    //printf("PRIMITIVE PLANE TEX COORDS: \n");
    for (int i = 0; i < *n_vertices*2; i += 1){
        tex_coords[i] = prim_plane_tex_coords[i];
        if (i % 2 == 0){
            //printf("%f %f\n", prim_plane_tex_coords[i], prim_plane_tex_coords[i+1]);
        }
        
    }
    
};
void free_prim_plane(float* vertices, unsigned int* indices){
    free(vertices);
    free(indices);
};
void free_prim_plane_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords){
    free(vertices);
    free(indices);
    free(tex_coords);
};


float prim_cube_vertices[24] = {
1,1,1,   1,1,-1,   1,-1,-1,   1,-1,1,
-1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1  
};
unsigned int prim_cube_indices[36] = {
    0,1,3, 1,2,3,
    4,0,7, 0,3,7,
    5,4,6, 4,7,6,
    1,5,2, 5,6,2,
    2,3,6, 3,7,6,
    1,0,5, 0,4,5
};
float prim_cube_tex_coords[16] = {
    1,1, -1,1, -1,-1, 1,-1,
    -1,1, 1,1, 1,-1, -1,-1
};

float* get_prim_cube(unsigned int* indices, int* n_vertices, int* n_indices, float side_size){
    *n_vertices = 8;
    *n_indices = 36;
    float* vertices = malloc(*n_vertices*3*sizeof(float));
    for (int i = 0; i < *n_vertices*3; i += 1){
        vertices[i] = prim_cube_vertices[i]*side_size/2.0;   
    }

    indices = malloc(*n_indices*sizeof(unsigned int));
    for (int i = 0; i < *n_indices; i += 1){
        indices[i] = prim_cube_indices[i];
    }

    return vertices;
    
}

void get_prim_cube_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords, int* n_vertices, int* n_indices, float side_size){
    *n_vertices = 8;
    *n_indices = 36;
    
    //float* vertices = malloc((*n_vertices)*3*sizeof(float));
    for (int i = 0; i < *n_vertices*3; i += 1){
        vertices[i] = prim_cube_vertices[i]*side_size/2.0;
    }

    //indices = malloc((*n_indices)*sizeof(unsigned int));
    for (int i = 0; i < *n_indices; i += 1){
        indices[i] = prim_cube_indices[i];
    }

    //tex_coords = malloc((*n_vertices)*2*sizeof(float));
    for (int i = 0; i < *n_vertices*2; i += 1){
        tex_coords[i] = prim_cube_tex_coords[i];
    }

    
    
}


void free_prim_cube(float* vertices, unsigned int* indices){
    free(vertices);
    free(indices);
}

void free_prim_cube_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords){
    free(vertices);
    free(indices);
    free(tex_coords);
}

