#include "primitives.h"

static float prim_plane_vertices[12] = {
    // positions      // texture coords
    1.0f, 1.0f, 0.0,  // top right
    1.0f, -1.0f, 0.0, // bottom right
    -1.0f, -1.0f, 0.0,// bottom left
    -1.0f, 1.0f, 0.0  // top left
};

static unsigned int prim_plane_indices[6] = {
    0, 1, 3, // first triangle
    1, 2, 3 // second triangle
};

static float prim_plane_tex_coords[8] = {
    1.0, 1.0,
    1.0, 0.0,
    0.0, 0.0,
    0.0, 1.0
};

int get_prim_plane_n_vertices(){
    return 4;
}

int get_prim_plane_n_indices(){
    return 6;
}

void get_prim_plane(float* vertices, unsigned int* indices, float side_size){

    //float* vertices = malloc(4 * 3 * sizeof(float));
    for (int i = 0; i < 4 * 3; i += 1){
        vertices[i] = prim_plane_vertices[i]*side_size/2.0;
    }

    //indices = malloc(4 * sizeof(unsigned int));
    for (int i = 0; i < 6; i += 1){
        indices[i] = prim_plane_indices[i];
    }

};

void get_prim_plane_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords, float side_size){
    //printf("PRIMITIVE PLANE VERTICES: \n");
    for (int i = 0; i < 4 * 3; i += 1){
        vertices[i] = prim_plane_vertices[i]*side_size/2.0;
    }

    
    //printf("PRIMITIVE PLANE INDICES: \n");
    for (int i = 0; i < 6; i += 1){
        indices[i] = prim_plane_indices[i];
        //printf("%d\n", indices[i]);
    }

    //printf("PRIMITIVE PLANE TEX COORDS: \n");
    for (int i = 0; i < 4 * 2; i += 1){
        tex_coords[i] = prim_plane_tex_coords[i];
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

static float prim_cube_vertices[24] = {
1,1,1,   1,1,-1,   1,-1,-1,   1,-1,1,
-1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1  
};

static unsigned int prim_cube_indices[36] = {
    0,1,3, 1,2,3,
    4,0,7, 0,3,7,
    5,4,6, 4,7,6,
    1,5,2, 5,6,2,
    2,3,6, 3,7,6,
    1,0,5, 0,4,5
};

static float prim_cube_tex_coords[16] = {
    1,1, -1,1, -1,-1, 1,-1,
    -1,1, 1,1, 1,-1, -1,-1
};

int get_prim_cube_n_vertices(){
    return 8;
}

int get_prim_cube_n_indices(){
    return 36;
}

void get_prim_cube(float* vertices, unsigned int* indices, float side_size){
    
    for (int i = 0; i < 8*3; i += 1){
        vertices[i] = prim_cube_vertices[i]*side_size/2.0;   
    }

    for (int i = 0; i < 36; i += 1){
        indices[i] = prim_cube_indices[i];
    }
    
}

void get_prim_cube_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords, float side_size){
    
    //float* vertices = malloc((8)*3*sizeof(float));
    for (int i = 0; i < 8 * 3; i += 1){
        vertices[i] = prim_cube_vertices[i]*side_size/2.0;
    }

    //indices = malloc((36)*sizeof(unsigned int));
    for (int i = 0; i < 36; i += 1){
        indices[i] = prim_cube_indices[i];
    }

    //tex_coords = malloc((8)*2*sizeof(float));
    for (int i = 0; i < 8 * 2; i += 1){
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

int get_prim_rectangle_n_vertices(){
    return 8;
}

int get_prim_rectangle_n_indices(){
    return 36;
}

void get_prim_rectangle(float* vertices, unsigned int* indices, float length, float width, float height){

    for (int i = 0; i < 8; i += 1){
        vertices[i*3] = prim_cube_vertices[i*3] * length/2.0;
        vertices[i*3+1] = prim_cube_vertices[i*3+1] * height/2.0;
        vertices[i*3+2] = prim_cube_vertices[i*3+2] * width/2.0;
    }

    for (int i = 0; i < 36; i += 1){
        indices[i] = prim_cube_indices[i];
    }
    
}

void get_prim_rectangle_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords, float length, float width, float height){
    
    //float* vertices = malloc((8)*3*sizeof(float));
    for (int i = 0; i < 8; i += 1){
        vertices[i*3] = prim_cube_vertices[i*3] * length/2.0;
        vertices[i*3+1] = prim_cube_vertices[i*3+1] * height/2.0;
        vertices[i*3+2] = prim_cube_vertices[i*3+2] * width/2.0;
    }

    //indices = malloc((36)*sizeof(unsigned int));
    for (int i = 0; i < 36; i += 1){
        indices[i] = prim_cube_indices[i];
    }

    //tex_coords = malloc((8)*2*sizeof(float));
    for (int i = 0; i < 8 * 2; i += 1){
        tex_coords[i] = prim_cube_tex_coords[i];
    }

    
    
}

void free_prim_rectangle(float* vertices, unsigned int* indices){
    free(vertices);
    free(indices);
}

void free_prim_rectangle_w_tex_coords(float* vertices, unsigned int* indices, float* tex_coords){
    free(vertices);
    free(indices);
    free(tex_coords);
}

