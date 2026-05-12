#include "heightmap.h"


static float dome(int x, int y){
    
}

void heightmap_setup(heightmap* map, int res_x, int res_y){
    map->grid = malloc(sizeof(float*)*res_x);
    for (int i = 0; i < res_x; i += 1){
        map->grid[i] = malloc(sizeof(float)*res_y);
    }
    map->map_res_x = res_x;
    map->map_res_y = res_y;

}

void heightmap_black(heightmap* map){
    for (int i = 0; i < map->map_res_x; i += 1){
        for (int j = 0; j < map->map_res_y; j += 1){
            map->grid[i][j] = 0.0;
        }   
    }
}

void heightmap_white(heightmap* map){
    for (int i = 0; i < map->map_res_x; i += 1){
        for (int j = 0; j < map->map_res_y; j += 1){
            map->grid[i][j] = 1.0;
            //printf("%f ", map->grid[i][j]);
        }   
    }
}

void heightmap_from_function(void* ctx, heightmap* map, float (*f)(void*, int, int)){
    for (int i = 0; i < map->map_res_x; i += 1){
        for (int j = 0; j < map->map_res_y; j += 1){
            map->grid[i][j] = f(ctx, i, j);
        }   
    }
}

void heightmap_from_image(heightmap* map){

}

void print_heightmap(heightmap* h_map){
    int res_x = h_map->map_res_x;
    int res_y = h_map->map_res_y;
    for (int i = 0; i < res_y; i += 1){
        for (int j = 0; j < res_x; j += 1){
            printf("%f ", h_map->grid[i][j]);
        }
        printf("\n");
    }
}

void free_heightmap(heightmap* map){
    for (int i = 0; i < map->map_res_x; i += 1){
        free(map->grid[i]);
    }
    free(map->grid);
}